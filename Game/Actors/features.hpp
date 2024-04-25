#include "engine_canvas.hpp"
//#include "framework.hpp"

#include "fort_athena_containers.hpp"
#include "fort_athena_animals.hpp"
#include "fort_athena_vehicle.hpp"
#include "fort_pickup.hpp"

void tick_callback() {
	init();
	BoneInit();
	FortPawn::Tick();
	FortAthenaVehicle::Tick();
}