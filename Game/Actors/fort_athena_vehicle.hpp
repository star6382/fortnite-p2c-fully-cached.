flinearcolor VehicleColor = flinearcolor(1.f, 1.f, 1.f, 1.f);

void FortAthenaVehicle::Tick() {
	if (settings.vehicle) {
		for (int i = 0; i < CachedVehicles.Num(); i++) {
			if (CachedVehicles.IsValidIndex(i) == false) continue;

			AFortPawn* Actor = CachedVehicles[i];
			if (IsValidPointer(Actor) == false) continue;

			AFortAthenaVehicle* FortAthenaVehicle = cast<AFortAthenaVehicle>(Actor);
			if (IsValidPointer(FortAthenaVehicle) == false) continue;

			FVector2D project = FVector2D();
			FVector RootPosition = Actor->RootComponent()->RelativeLocation();
			if (RootPosition.Distance(LocalPawnCache.Position) / 100 > settings.vehicle_maxdistance) {
				continue;
			}

			float FontSize = CalculateInterpolatedValue(150.f, LocalPawnCache.Position, 12.f, 20.f);
			sdk.world_data.GetLocalPlayer()->worldtoscreen(RootPosition, project);
			if (IsOnScreen(project) == false) continue;

			std::string VehicleName = ("Vehicle");
			Text(VehicleName.c_str(), project, FontSize, VehicleColor, true, true);
		}
	}
}