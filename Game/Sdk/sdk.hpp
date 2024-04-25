#include <chrono>
#include "hook.hpp"
#include <algorithm>
#include <__msvc_chrono.hpp>
#include "sdk_structure.hpp"

constexpr uintptr_t uworld_offset = 0x1B7493C;
//USkeletalMeshComponent* mesh = nullptr;
UWorld* uworld = nullptr;
UCanvas* canvas = nullptr;

double screenx, screeny;

void init() {
  defines.gameplaystatistics = reinterpret_cast<ugameplaystatistics*>(uengine->find_object(L"Engine.GameplayStatics"));

}

AFortPawn* player_structure::world_structure::GetLocalPawn() {
	return player_controller->AcknowledgedPawn();
}

USkeletalMeshComponent* player_structure::world_structure::GetMesh() {
	return player_controller->AcknowledgedPawn()->Mesh();
}

bool player_structure::world_structure::world() {
	uworld = reinterpret_cast<UWorld*>((uintptr_t)game_address - uworld_offset);
    if (!uworld || !uworld->GameInstance() || !uworld->GameInstance()->LocalPlayers() || !uworld->GameInstance()->LocalPlayers()->LocalPlayer()->PlayerController()) {
        return false;
    }

	player_controller = uworld->GameInstance()->LocalPlayers()->LocalPlayer()->PlayerController();

	viewport_client = uworld->GameInstance()->LocalPlayers()->LocalPlayer()->ViewPortClient();
	if (!viewport_client) {
		return false;
	}

    return true;
}

APlayerController* player_structure::world_structure::GetLocalPlayer() {
	return uworld->GameInstance()->LocalPlayers()->LocalPlayer()->PlayerController();
}

/* Represents the bone names of a FortPawn */
struct BoneNames {
	fname Head;
	fname Neck;

	fname ChestLeft;
	fname ChestRight;

	fname LeftShoulder;
	fname LeftElbow;
	fname LeftHand;
	fname RightShoulder;
	fname RightElbow;
	fname RightHand;

	fname LeftLeg;
	fname LeftKnee;
	fname LeftFoot;
	fname RightLeg;
	fname RightKnee;
	fname RightFoot;

	fname Pelvis;

	fname Root;
	fname None;
};

/* The bone names of a FortPawn */
inline BoneNames Names;

enum BoneID_ : uint8_t {
	Head = 1,           // "head"
	Neck = 2,           // "neck_01"

	// Bottom contains the root component position. We have two with the same value for code readability
	Root = 3,           // "Root"

	ChestLeft = 4,	    // "clavicle_l"
	ChestRight = 5,     // "clavicle_r"

	// There is no chest bone, so this is a dummy value used for the hierarchy, chest should be calculated based on left and right chest bones
	Chest = 6,          // none

	LeftShoulder = 7,   // "upperarm_l"
	LeftElbow = 8,	    // "lowerarm_l"
	LeftHand = 9,	    // "Hand_L"
	RightShoulder = 10, // "upperarm_r"
	RightElbow = 11,    // "lowerarm_r"
	RightHand = 12,	    // "hand_r"

	LeftLeg = 13,	    // "thigh_l"
	LeftKnee = 14,	    // "calf_l"
	LeftFoot = 15,	    // "foot_l"
	RightLeg = 16,	    // "thigh_r"
	RightKnee = 17,	    // "calf_r"
	RightFoot = 18,	    // "foot_r"

	Pelvis = 19,	    // "pelvis"

	BONEID_MAX = 20,           // Max value for looping

	None = 0,		    // "None"
};

fname GetBoneName(uint8_t BoneID) {
	switch (BoneID) {
	case Head: return Names.Head;
	case Neck: return Names.Neck;
	case LeftShoulder: return Names.LeftShoulder;
	case LeftElbow: return Names.LeftElbow;
	case LeftHand: return Names.LeftHand;
	case RightShoulder: return Names.RightShoulder;
	case RightElbow: return Names.RightElbow;
	case RightHand: return Names.RightHand;
	case LeftLeg: return Names.LeftLeg;
	case LeftKnee: return Names.LeftKnee;
	case LeftFoot: return Names.LeftFoot;
	case RightLeg: return Names.RightLeg;
	case RightKnee: return Names.RightKnee;
	case RightFoot: return Names.RightFoot;
	case Pelvis: return Names.Pelvis;
	case Root: return Names.Root;
	case ChestLeft: return Names.ChestLeft;
	case ChestRight: return Names.ChestRight;
	case None: return Names.None;
	}

	return Names.None;
}

void BoneInit() {
	Names.Head = fname(L"head");
	Names.Neck = fname((L"neck_01"));

	Names.ChestLeft = fname((L"clavicle_l"));
	Names.ChestRight = fname((L"clavicle_r"));

	Names.LeftShoulder = fname((L"upperarm_l"));
	Names.LeftElbow = fname((L"lowerarm_l"));
	Names.LeftHand = fname((L"Hand_L"));
	Names.RightShoulder = fname((L"upperarm_r"));
	Names.RightElbow = fname((L"lowerarm_r"));
	Names.RightHand = fname((L"hand_r"));

	Names.LeftLeg = fname((L"thigh_l"));
	Names.LeftKnee = fname((L"calf_l"));
	Names.LeftFoot = fname((L"foot_l"));
	Names.RightLeg = fname((L"thigh_r"));
	Names.RightKnee = fname((L"calf_r"));
	Names.RightFoot = fname((L"foot_r"));

	Names.Pelvis = fname((L"pelvis"));

	Names.Root = fname((L"Root"));
	Names.None = fname((L"None"));
}

/* The pairs of bones that are connected to each other in the player skeleton */
inline const inline std::vector<std::pair<BoneID_, BoneID_>> SkeletonBonePairs = {
	{Head,            Chest},
	{Chest,           LeftShoulder},
	{Chest,           RightShoulder},
	{LeftShoulder,    LeftElbow},
	{RightShoulder,   RightElbow},
	{LeftElbow,       LeftHand},
	{RightElbow,      RightHand},
	{Pelvis,          LeftLeg},
	{Pelvis,          RightLeg},
	{LeftLeg,         LeftKnee},
	{RightLeg,        RightKnee},
	{LeftKnee,        LeftFoot},
	{RightKnee,       RightFoot},
	{Chest,           Pelvis},
};

namespace Caches {
	/* Cache for FortPawns, stores information like bone positions, distance from local pawn, visibilities etc */
	struct FortPawnCache {
		AFortPawn* FortPawn = nullptr;					            // FortPawn
		USkeletalMeshComponent* Mesh = nullptr;						// Player mesh

		int								TeamIndex = -1;				// Team index
		fstring					        PlayerName = fstring();		// Player name

		float							DistanceFromLocalPawn = 0.f;		// Distance from the local player (meters)
		bool							IsAnyBoneVisible = false;			// If any bones were visible from LineTraceSingle
		bool							IsPlayerVisibleOnScreen = false;	// If the player is on the screen
		bool							IsBoneRegister2DPopulated = false;	// If the 2D bone register was fully populated

		std::vector<FVector>		    BonePositions3D;					// Contains all bones
		std::vector<FVector2D>		    BonePositions2D;					// Contains all bones in 2D (world to screen)
		std::vector<bool>				BoneVisibilityStates;				// Contains all bones visibilities (from LineTraceSingle)

		FVector GetBonePosition(USkeletalMeshComponent* mesh, uint8_t BoneID) {
			//auto _controller = uworld->GameInstance()->LocalPlayers()->LocalPlayer()->PlayerController();
			//auto _mesh = _controller->AcknowledgedPawn()->Mesh();
			return mesh->GetSocketLocation(GetBoneName(BoneID));
		}
	};

	/* Minimal Cache for FortPawns, stores position and team index */
	struct MinimalFortPawnCache {
		FVector Position = FVector();
		uint8_t TeamIndex = 0;
	};

	/* Cache for the camera, stores position, rotation and FOV */
	struct CameraCache {
		FVector Position = FVector();
		FRotator Rotation = FRotator();
		float FOV = 0.f;
	};
}

namespace FortPawn {
	void Tick();

	std::vector<Caches::FortPawnCache> CachedPlayers;

	const float IntervalSeconds = 0.25f;
    std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
}


namespace FortPickup {
	void Tick();

	inline tarray<AFortPawn*> CachedWeapons;

	inline const float IntervalSeconds = 0.10f;
	inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();

	inline std::chrono::steady_clock::time_point LastAutoPickupTime = std::chrono::steady_clock::now();
}

namespace FortAthenaVehicle {
	void Tick();

	inline tarray<AFortPawn*> CachedVehicles;

	inline const float IntervalSeconds = 0.25f;
	inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
}

namespace FortContainer {
	void Tick();

	inline tarray<AFortPawn*> CachedContainers;

	inline const float IntervalSeconds = 0.10f;
	inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
}

namespace FortAnimals {
	void Tick();

	inline tarray<AFortPawn*> CachedAnimals;

	inline const float IntervalSeconds = 0.10f;
	inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
}


inline std::chrono::high_resolution_clock::time_point LastAimbotFrameTime;
inline float FPSScale;

// Cache
inline Caches::CameraCache MainCamera;					// Cache for the main camera
inline Caches::CameraCache AimbotCamera;				// Cache for the aimbot camera (used for silent aim)
inline Caches::MinimalFortPawnCache LocalPawnCache;		// Cache for the local pawn (position, team index)

float CalculateInterpolatedValue(float CurrentScalar, float MaxScalar, float MinValue, float MaxValue) {
	MaxScalar = min(MaxScalar, CurrentScalar);

	float InterpolatedValue = MaxValue - (MaxValue - MinValue) * (MaxScalar / CurrentScalar);

	InterpolatedValue = std::clamp(InterpolatedValue, MinValue, MaxValue);

	return InterpolatedValue;
}

bool IsOnScreen(const FVector2D& Position) {
	bool OnScreenX = (Position.X >= 0.f) && (Position.X <= screenx);
	bool OnScreenY = (Position.Y >= 0.f) && (Position.Y <= screeny);

	return OnScreenX && OnScreenY;
}

FVector2D Project(FVector WorldLocation) {
	FVector ScreenLocation = canvas->K2_Project(WorldLocation);

	if (ScreenLocation.Z > 0.f) {
		return FVector2D(ScreenLocation.X, ScreenLocation.Y);
	}

	return FVector2D(-1.f, -1.f);
}

FVector Project3D(FVector WorldLocation) {
	FVector ReturnValue = canvas->K2_Project(WorldLocation);

	// Invert X and Y if the player is behind the camera
	if (ReturnValue.Z <= 0.f) {
		ReturnValue.X *= -1.f;
		ReturnValue.Y *= -1.f;

		ReturnValue.X += screenx;
		ReturnValue.Y += screeny;
	}

	return ReturnValue;
}

bool IsPositionVisible(uobject* WorldContextObj, FVector TargetPosition, AFortPawn* ActorToIgnore, AFortPawn* ActorToIgnore2) {
	//FHitResult Hit{};
	//tarray<AFortPawn*> IgnoredActors;

	//if (TargetPosition == FVector()) return false;

	//if (ActorToIgnore) IgnoredActors.Add(ActorToIgnore);
	//if (ActorToIgnore2) IgnoredActors.Add(ActorToIgnore2);

	//bool bHitSomething = UKismetSystemLibrary::LineTraceSingle(WorldContextObj, Actors::MainCamera.Position, TargetPosition, ETraceTypeQuery::TraceTypeQuery6, true, IgnoredActors, EDrawDebugTrace::None, Hit, true, flinearcolor(0.f, 0.f, 0.f, 0.f), flinearcolor(0.f, 0.f, 0.f, 0.f), 0.f);

	//return !bHitSomething;

	return true;
}

bool PopulateBones(Caches::FortPawnCache& FortPawnCache) {
	// Resize the bone register to avoid out of range errors
	FortPawnCache.BonePositions3D.resize(BONEID_MAX);
	FortPawnCache.BonePositions2D.resize(BONEID_MAX);
	FortPawnCache.BoneVisibilityStates.resize(BONEID_MAX);

	bool FoundBoneOnScreen = false;

	for (int i = Head; i < BONEID_MAX; i++) {
		FortPawnCache.BonePositions3D[i] = FortPawnCache.GetBonePosition(sdk.world_data.GetMesh(), i);

		if (i == Head) {
			if (FortPawnCache.BonePositions3D[Head] == FVector(0, 0, 0)) {
				return false;
			}
		}

		if (i == Chest) {
			FortPawnCache.BonePositions3D[Chest] = (FortPawnCache.BonePositions3D[ChestLeft] + FortPawnCache.BonePositions3D[ChestRight]) /*/ 2*/;
		}

		// To avoid W2Sing players that aren't on the screen
		if (i <= 5 || FoundBoneOnScreen) {
			FortPawnCache.BonePositions2D[i] = Project(FortPawnCache.BonePositions3D[i]);

			if (IsOnScreen(FortPawnCache.BonePositions2D[i])) {
				FoundBoneOnScreen = true;
			}
		}
	}

	return FoundBoneOnScreen;
}

void PopulateVisibilities(Caches::FortPawnCache& FortPawnCache) {
	FortPawnCache.BonePositions3D.resize(BONEID_MAX);
	FortPawnCache.BonePositions2D.resize(BONEID_MAX);
	FortPawnCache.BoneVisibilityStates.resize(BONEID_MAX);

	std::vector<BoneID_> bones = { Head, Chest, LeftShoulder, RightShoulder, LeftElbow, RightElbow, LeftHand, RightHand, LeftLeg, RightLeg, LeftKnee, RightKnee, LeftFoot, RightFoot, Pelvis };
	for (const auto& bone : bones) {
		FortPawnCache.BoneVisibilityStates[bone] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[bone], FortPawnCache.FortPawn, sdk.world_data.GetLocalPawn());
	}
}

//void OgPopulateVisibilities(Caches::FortPawnCache& FortPawnCache) {
//	FortPawnCache.BonePositions3D.resize(BONEID_MAX);
//	FortPawnCache.BonePositions2D.resize(BONEID_MAX);
//	FortPawnCache.BoneVisibilityStates.resize(BONEID_MAX);
//
//	FortPawnCache.BoneVisibilityStates[Head] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Head], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[Chest] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Chest], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftShoulder] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftShoulder], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightShoulder] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightShoulder], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftElbow] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftElbow], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightElbow] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightElbow], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftHand] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftHand], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightHand] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightHand], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftLeg] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftLeg], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightLeg] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightLeg], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftKnee] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftKnee], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightKnee] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightKnee], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[LeftFoot] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[LeftFoot], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[RightFoot] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[RightFoot], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//	FortPawnCache.BoneVisibilityStates[Pelvis] = IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Pelvis], FortPawnCache.FortPawn, player_struct._world.GetLocalPawn());
//}

void PopulateBoundCorners(Caches::FortPawnCache& FortPawnCache, FVector2D& TopLeft, FVector2D& BottomRight) {
	TopLeft = FVector2D(FLT_MAX, FLT_MAX);
	BottomRight = FVector2D(-FLT_MAX, -FLT_MAX);

	for (int i = Head; i < BONEID_MAX; i++) {
		TopLeft.X = min(TopLeft.X, FortPawnCache.BonePositions2D[i].X);
		TopLeft.Y = min(TopLeft.Y, FortPawnCache.BonePositions2D[i].Y);

		BottomRight.X = max(BottomRight.X, FortPawnCache.BonePositions2D[i].X);
		BottomRight.Y = max(BottomRight.Y, FortPawnCache.BonePositions2D[i].Y);
	}

	// Adjust the bounding box to make it more visually appealing
	float BoxSizeMultiplier = CalculateInterpolatedValue(FortPawnCache.DistanceFromLocalPawn, 100.f, 1.0f, 4.0f);

	// Increase the size of the bounding box by a percentage of the original size. This is to make the bounding box more visually appealing.
	float LeftRightOffset = (BottomRight.X - TopLeft.X) * (0.36f * BoxSizeMultiplier);
	float TopBottomOffset = (BottomRight.Y - TopLeft.Y) * (0.14f * BoxSizeMultiplier);

	TopLeft.X -= LeftRightOffset;
	TopLeft.Y -= TopBottomOffset;

	BottomRight.X += LeftRightOffset;
	BottomRight.Y += TopBottomOffset;
}

bool IsValidPointer(void* Address) {
	if (!Address) {
		return false;
	}

#if USING_SEH
	__try {
		volatile auto value = *static_cast<char*>(Address);
		(void)value;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
#else
	if ((custom::is_bad_write_ptr)(Address, 8)) {
		return false;
	}
#endif // USING_SEH

	return true;
}

template <typename T, bool ForceCast = false>
static inline T* cast(uobject* Object) {
	if (ForceCast || !Object) {
		return (T*)Object;
	}

	return nullptr;
}