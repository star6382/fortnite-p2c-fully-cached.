#include "engine.hpp"

//Canvas
uobject* k2_draw_line = { nullptr };
uobject* k2_draw_text = { nullptr };

//Pawn
uobject* k2_bone_name = { nullptr };
uobject* k2_socket_location = { nullptr };
uobject* k2_projectworld_location_to_screen = { nullptr };
uobject* k2_fort_pawn_athena = { nullptr };
uobject* k2_get_all_actors_of_class = { nullptr };
uobject* k2_get_game_instance = { nullptr };
uobject* k2_getplayer_controller = { nullptr };
uobject* k2_get_objectname = { nullptr };


//Function structs
namespace engine_structs {
	struct {
		fvector2d screen_position_a;
		fvector2d screen_position_b;
		float thickness;
		flinearcolor render_color;
	} k2_draw_line_params;



	struct get_bone_name_parms {
		int32_t BoneIndex = 0;
		fname ReturnValue;
	} k2_bone_name_parms;


	struct {
		fname InSocketName;
		fvector ReturnValue;
	} k2_get_socket_location_params;

	struct {
		fvector a1;
		fvector2d a2;
		bool a3;
		bool ret;
	} k2_world_to_screen_params;

	struct {
		uobject* world_context_object;
		uobject* actor_class;
		tarray<uobject*> out_actors;
	} k2_get_all_actors_of_class_params;


	struct {
		uobject* object;
		fstring return_value;
	} k2_get_object_name_params;
}
