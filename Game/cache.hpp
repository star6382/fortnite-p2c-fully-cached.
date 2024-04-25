#include "menu.hpp"

inline unsigned __int64 CurrentFrame = 0;
inline std::chrono::time_point<std::chrono::steady_clock> CurrentTime;

std::vector<Caches::FortPawnCache> TempCache;
tarray<AFortPawn*> ReturnArray;

bool player_cache(std::vector<Caches::FortPawnCache> cache) {
	// Update FPS scale
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - LastAimbotFrameTime).count();
		LastAimbotFrameTime = currentTime;

		float targetFrameTime = 1.0f / 60.0f; // Targeting 60 FPS frame time

		FPSScale = targetFrameTime / deltaTime;
	}

	cache = FortPawn::CachedPlayers;
	for (auto it = cache.begin(); it != cache.end(); ++it) {
		Caches::FortPawnCache& CurrentPlayer = *it;

		AFortPawn* FortPawn = CurrentPlayer.FortPawn;
		CurrentPlayer.Mesh = FortPawn->Mesh();

		// LocalPawn caching and exploit ticks
		if (FortPawn == sdk.world_data.GetLocalPawn()) {
			LocalPawnCache.Position = CurrentPlayer.GetBonePosition(CurrentPlayer.Mesh, Head);
			// or call expoilts such as chams etc.
			continue;
		}

		// Bone positions and visibility caching
		// If this returns false, the player isn't on the screen and only 5 of the bones were WorldToScreened
		CurrentPlayer.IsBoneRegister2DPopulated = PopulateBones(CurrentPlayer);
		PopulateVisibilities(CurrentPlayer);

		// Update IsPlayerVisibleOnScreen based on if any of the bones 2D positions are on the screen
		CurrentPlayer.IsPlayerVisibleOnScreen = false;
		for (int i = 0; i < CurrentPlayer.BonePositions2D.size(); i++) {
			if (CurrentPlayer.BonePositions2D[i] == FVector2D()) continue;

			if (IsOnScreen(CurrentPlayer.BonePositions2D[i])) {
				CurrentPlayer.IsPlayerVisibleOnScreen = true;
			}
		}

		CurrentPlayer.DistanceFromLocalPawn = LocalPawnCache.Position.Distance(CurrentPlayer.BonePositions3D[Root]) / 100.f;

		// Hardcoded max distance, should move to bone population for optimisation
		if (CurrentPlayer.DistanceFromLocalPawn > 500.f) continue;

		// Update any bone visibility
		CurrentPlayer.IsAnyBoneVisible = false;
		for (int i = 0; i < CurrentPlayer.BoneVisibilityStates.size(); i++) {
			if (CurrentPlayer.BoneVisibilityStates[i]) {
				CurrentPlayer.IsAnyBoneVisible = true;
				break;
			}
		}

		// Visuals
		if (CurrentPlayer.IsPlayerVisibleOnScreen) {
			FVector2D TopLeft, BottomRight;
			PopulateBoundCorners(CurrentPlayer, TopLeft, BottomRight);

			float FontSize = CalculateInterpolatedValue(150.f, CurrentPlayer.DistanceFromLocalPawn, 12.f, 20.f);
			float PrimaryThicknessMultiplier = CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 3.f);
			float SecondaryThicknessMultiplier = CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 2.f);

			float PrimaryThickness = 1.f * PrimaryThicknessMultiplier;
			float SecondaryThickness = 1.f * SecondaryThicknessMultiplier;

			flinearcolor PrimaryColor = flinearcolor(1.f, 1.f, 1.f, 1.f);
			if (CurrentPlayer.IsAnyBoneVisible) {
				PrimaryColor = flinearcolor(1.f, 0.f, 0.f, 1.f);
			}

			flinearcolor SecondaryColor = flinearcolor(1.0f, 0.f, 0.f, 1.0f);
			if (CurrentPlayer.IsAnyBoneVisible) {
				SecondaryColor = flinearcolor(0.0f, 1.f, 1.f, 1.0f);
			}

			if (CurrentPlayer.IsPlayerVisibleOnScreen) {
				for (const auto& Pair : SkeletonBonePairs) {
					int BoneIDs[2] = { (int)Pair.first, (int)Pair.second };
					FVector2D ScreenPos[2];

					bool BoneVisibleToPlayer = false;
					if (IsOnScreen(ScreenPos[0]) == false && IsOnScreen(ScreenPos[1]) == false) {
						continue;
					}

					for (int i = 0; i <= 1; ++i) {
						int BoneID = BoneIDs[i];
						if (BoneID <= None || BoneID >= BONEID_MAX) {
							break;
						}

						ScreenPos[i] = CurrentPlayer.BonePositions2D[BoneID];
						if (CurrentPlayer.BoneVisibilityStates[BoneID]) {
							BoneVisibleToPlayer = true;
						}
					}

				    Line(canvas, FVector2D(ScreenPos[0].X, ScreenPos[0].Y), FVector2D(ScreenPos[1].X, ScreenPos[1].Y), SecondaryThicknessMultiplier, true ? BoneVisibleToPlayer ? flinearcolor(0.0f, 1.f, 1.f, 1.0f) : flinearcolor(1.0f, 0.f, 0.f, 1.0f) : SecondaryColor, false);
				}




			}
		}
	}




	return true;
}
