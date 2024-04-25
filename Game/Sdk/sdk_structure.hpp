#include "unreal_structs.hpp"
uobject* uengine = nullptr;

struct UCanvas : uobject {
public:
	__forceinline void ADrawText(FVector2D ScreenPosition, float font_size, fstring RenderText, flinearcolor RenderColor, bool centered, bool bOutlined)
	{
		struct textparams
		{
			uobject* RenderFont;
			fstring RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			flinearcolor RenderColor;
			double Kerning;
			flinearcolor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			flinearcolor OutlineColor;
		};
		static uobject* fn = nullptr;
		static uobject* font = nullptr;
		static bool setonce = false;
		if (!setonce)
		{
			fn = find_object((L"Function Engine.Canvas.K2_DrawText"));
			font = find_object((L"Font Roboto.Roboto"));
			setonce = true;
		}
		textparams params;
		params.RenderFont = (uobject*)font;
		params.RenderText = RenderText;
		params.ScreenPosition = ScreenPosition;
		params.Scale = FVector2D{ font_size, font_size };// defualt size FVector2D{1.f, 1.f}
		params.RenderColor = RenderColor;
		params.Kerning = false;
		params.ShadowColor = flinearcolor{ 0.0f, 0.0f, 0.0f, 0.0f };
		params.ShadowOffset = ScreenPosition;
		if (centered) { params.bCentreX = true; params.bCentreY = true; }
		else { params.bCentreX = false; params.bCentreY = false; }
		params.bOutlined = bOutlined;
		params.OutlineColor = { 0.0f, 0.0f, 0.0f, 1.f };
		process_event(fn, &params);
	}
	// 
	//void text(UCanvas* canvas, FString content, Vector2 location, FLinearColor color, bool center, bool outline)
	//{
	//	canvas->ADrawText(location, content, color, false, false);
	//}

	FVector K2_Project(FVector& WorldLocation) {
		uobject* obj = static_find_object(nullptr, nullptr, L"Function Engine.Canvas.K2_Project");

		struct {
			FVector WorldLocation;

			FVector return_value;
		} params_K2_Project{};

		params_K2_Project.WorldLocation = WorldLocation;

		process_event(obj, &params_K2_Project);

		return params_K2_Project.return_value;
	}

	auto DrawLine(FVector2D a, FVector2D b, float thickness, flinearcolor rendercolor) -> void {
		uobject* obj = static_find_object(nullptr, nullptr, L"Function Engine.Canvas.K2_DrawLine");	

		struct {
			FVector2D a, b;
			float thickness;
			flinearcolor rendercolor;
		} params = { a, b, thickness, rendercolor };

		process_event(obj, &params);
	}

	__forceinline FVector2D SZE() const {
		return { ScreenX, ScreenY };
	};

private:
	uint8_t Padding_0[0x40];
	double ScreenX;
	double ScreenY;

};

struct USkeletalMeshComponent : public uobject {
public:

	FVector GetSocketLocation(fname InSocketName) {
		uobject* obj = static_find_object(nullptr, nullptr, L"SceneComponent.GetSocketLocation");

		struct {
			fname InSocketName;

			FVector return_value;
		} params_GetSocketLocation{};

		params_GetSocketLocation.InSocketName = InSocketName;

		this->process_event(obj, &params_GetSocketLocation);

		return params_GetSocketLocation.return_value;
	}
	

};

struct ARootComponent {
public:
	FVector RelativeLocation() {
		return *reinterpret_cast<FVector*>(reinterpret_cast<uintptr_t>(this) + 0x128);
	}
};

struct AFortPawn : public uobject {
public:

	USkeletalMeshComponent* Mesh() {
		return reinterpret_cast<USkeletalMeshComponent*>(this + 0x310);
	}

	ARootComponent* RootComponent() {
		return (ARootComponent*)*(uint64_t*)(this + 0x190);
	}

};

class AFortAthenaVehicle : AFortPawn {
public:



	static uobject* StaticClass() {
		static class uobject* Class = nullptr;
		if (!Class)
			Class = find_object(L"FortniteGame.FortAthenaVehicle");

		return Class;
	}
};


struct APlayerController : AFortPawn {
public:
	AFortPawn* AcknowledgedPawn()
	{
		return (AFortPawn*)*(uintptr_t*)(this + 0x338);
	}
	 
	bool worldtoscreen(FVector world, FVector2D screen) {
		static uobject* fn = NULL;
		if (!fn)
		{
			fn = find_object(L"Function Engine.PlayerController.ProjectWorldLocationToScreen");
		}

		struct
		{
			FVector world;
			FVector2D screen;
			bool relative_viewport = true;
			bool return_value;
		}
		params = { world, FVector2D(), true };
		
		process_event(fn, &params);
		screen = params.screen;
		return params.return_value;
	}
	//bool IsInputKeyDown(const struct FKey& Key)
	//{
	//	static uintptr_t fn = NULL;
	//	if (!fn)
	//		fn = UObject::FindObject(("Function Engine.PlayerController.IsInputKeyDown"));
	//	struct keystruct
	//	{
	//		struct FKey Key;
	//		bool ReturnValue;
	//	};
	//	keystruct params;
	//	params.Key = Key;
	//	UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);
	//	return params.ReturnValue;
	//}

};

class UGameViewportClient {

public:
};

class ULocalPlayer {
public:

	APlayerController* PlayerController()
	{
		if (!this) return 0;
		return (APlayerController*)(*(uintptr_t*)(this + 0x30));
	}

	UGameViewportClient* ViewPortClient()
	{
		if (!this) return 0;
		return (UGameViewportClient*)(*(uintptr_t*)(this + 0x78));
	}

};

class ULocalPlayers {
public:

	ULocalPlayer* LocalPlayer()
	{
		if (!this) return 0;
		return (ULocalPlayer*)(*(uintptr_t*)(this));
	}

};

class UGameInstance {
public:

	ULocalPlayers* LocalPlayers()
	{
		if (!this) return 0;
		return (ULocalPlayers*)(*(uintptr_t*)(this + 0x38));
	}

};

struct UWorld {
public:


	UGameInstance* GameInstance()
	{
		return (UGameInstance*)*(uintptr_t*)(this + 0x1B8);
	}

};


// engine library

class ugameplaystatistics : uobject {
public:

	tarray<AFortPawn*> get_all_actors_of_class(uobject* world_context_object, uobject* actor_class) {
		static uobject* function;
		if (!function) function = find_object(L"GameplayStatics.GetAllActorsOfClass");

		struct {
			uobject* world_context_object;
			uobject* actor_class;
			tarray<AFortPawn*> out_actors;
		}

		params = { world_context_object, actor_class };

		process_event(function, &params);
		return params.out_actors;
	}
};

class player_structure {
public:

	struct world_structure {
		AFortPawn* GetLocalPawn();
		USkeletalMeshComponent* GetMesh();
		APlayerController* GetLocalPlayer();
		APlayerController* player_controller = nullptr;
		UGameViewportClient* viewport_client = nullptr;
		bool world();

	} world_data;



public:
	player_structure() = default;

	player_structure(const player_structure& other) {

	}

	player_structure(player_structure&& other) noexcept {

	}

	player_structure& operator=(const player_structure& other) {
		if (this == &other)
			return *this;
		return *this;
	}

	player_structure& operator=(player_structure&& other) noexcept {
		if (this == &other)
			return *this;
		return *this;
	}
} sdk;

struct engine_objects : public uobject {
	ugameplaystatistics* gameplaystatistics = nullptr;


public:
	engine_objects() = default;

	engine_objects(const engine_objects& other) {

	}

	engine_objects(engine_objects&& other) noexcept {

	}

	engine_objects& operator=(const engine_objects& other) {
		if (this == &other)
			return *this;
		return *this;
	}

	engine_objects& operator=(engine_objects&& other) noexcept {
		if (this == &other)
			return *this;
		return *this;
	}
} defines;

void init();
