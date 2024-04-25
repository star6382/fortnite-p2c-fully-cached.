#include "cache.hpp"

void FortPawn::Tick() {
	double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortPawn::LastCacheTime).count();
	if (ElapsedTime >= FortPawn::IntervalSeconds) {
		FortPawn::LastCacheTime = CurrentTime;

		uobject* process_array = { nullptr };
		ReturnArray = defines.gameplaystatistics->get_all_actors_of_class((uobject*)uworld, process_array);
		for (int i = 0; i < ReturnArray.Num(); i++) {
			Caches::FortPawnCache FortPawnCache{};

			FortPawnCache.FortPawn = (AFortPawn*)cast<AFortPawn*, true>(ReturnArray[i]);
			/*	SDK::APlayerState* PlayerState = FortPawnCache.FortPawn->PlayerState();
				if (SDK::IsValidPointer(PlayerState)) {
					FortPawnCache.PlayerName = PlayerState->GetPlayerName();
					FortPawnCache.TeamIndex = SDK::Cast<SDK::AFortPlayerState>(PlayerState)->TeamIndex();
				}*/

			FortPawnCache.BonePositions3D.resize(BONEID_MAX);
			FortPawnCache.BonePositions2D.resize(BONEID_MAX);
			FortPawnCache.BoneVisibilityStates.resize(BONEID_MAX);

			TempCache.push_back(FortPawnCache);
		}

		FortPawn::CachedPlayers = TempCache;
	}

	player_cache(TempCache);

	// FortAthenaVehicle Cache
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortAthenaVehicle::LastCacheTime).count();
		if (ElapsedTime >= FortAthenaVehicle::IntervalSeconds) {
			FortAthenaVehicle::LastCacheTime = CurrentTime;
			FortAthenaVehicle::CachedVehicles = defines.gameplaystatistics->get_all_actors_of_class((uobject*)uworld, AFortAthenaVehicle::StaticClass());
		}
	}
}