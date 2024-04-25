#include "SDK.hpp"
#include <string>

void* original;
int post_render_index = 0x71;

void postrender(ugameviewportclient* viewport_client, ucanvas* canvas) {
	if (viewport_client) {  
		if (canvas) {
			canvas->draw_line(fvector2d{ 50, 50 }, fvector2d{ 100, 350 }, 1.f, flinearcolor{ 1.f, 1.f, 1.f, 1.f });

			auto world = reinterpret_cast<uworld*>(viewport_client->world());
			if (!world) {
				MessageBoxA(nullptr, "UWorld", "Failed!", MB_OK);
			}

			auto gameinstance = reinterpret_cast<ugameinstance*>(viewport_client->gameinstsance());
			if (!gameinstance) {
				MessageBoxA(nullptr, "OwingGameInstance", "Failed!", MB_OK);
			}

			auto localplayer = gameinstance->get_local_players()[0];

			auto controller = localplayer->playercontroller();

			MessageBoxA(nullptr, "Hook", "Hooked!", MB_OK);

			//tarray<uobject*> list = ue::k2_statics->get_all_actors_of_class((uobject*)world, k2_fort_pawn_athena);
			//for (int i = 0; i < list.size(); i++) {
			//	afortpawn* pawn = reinterpret_cast<afortpawn*>(list[i]);
			//	if (!pawn) continue;

			//	afortpawn* acknowledgedpawn = controller->acknowledgedpawn();
			//	if (!acknowledgedpawn) {
			//		MessageBoxA(nullptr, "AcknowledgedPawn", "Failed!", MB_OK);
			//	}

			//	if (pawn == acknowledgedpawn) continue;

			//	auto mesh = pawn->_mesh_();
			//	if (!mesh) {
			//		MessageBoxA(nullptr, "Mesh", "Failed!", MB_OK);
			//	}

			//	int32_t head = bones::Head;
			//	int32_t root = bones::Root;

			//	fvector2d _2d_head{};
			//	fvector _3d_head = mesh->getsocketlocation(mesh->getbonename(head));
			//	controller->world_to_screen({ _3d_head.x, _3d_head.y, _3d_head.z }, _2d_head);
			//	//controller->world_to_screen(_3d_head, _2d_head);
			//	if (!_3d_head.null_check()) {
			//		continue;
			//	}

			//	fvector2d _2d_root{};
			//	fvector _3d_root = mesh->getsocketlocation(mesh->getbonename(root));
			//	controller->world_to_screen({ _3d_root.x, _3d_root.y, _3d_root.z }, _2d_root);
			//	//controller->world_to_screen(_3d_root, _2d_root);
			//	if (!_3d_root.null_check()) {
			//		continue;
			//	}

			//	float Height = (float)(_2d_root.y - _2d_head.y);
			//	float Width = Height * 0.50f;

			//	canvas->draw_line(fvector2d(_2d_head.x, _2d_head.y), fvector2d(_2d_head.x + Width, _2d_head.y), 1.f, flinearcolor{ 1.f, 1.f, 1.f, 1.f });
			//	canvas->draw_line(fvector2d(_2d_head.x, _2d_head.y), fvector2d(_2d_head.x, _2d_head.y + Height), 1.f, flinearcolor{ 1.f, 1.f, 1.f, 1.f });
			//	canvas->draw_line(fvector2d(_2d_head.x + Width, _2d_head.y), fvector2d(_2d_head.x + Width, _2d_head.y + Height), 1.f, flinearcolor{ 1.f, 1.f, 1.f, 1.f });
			//	canvas->draw_line(fvector2d(_2d_head.x, _2d_head.y + Height), fvector2d(_2d_head.x + Width, _2d_head.y + Height), 1.f, flinearcolor{ 1.f, 1.f, 1.f, 1.f });
			//}

		}

		else if (!viewport_client) {
			return ((void(*)(ugameviewportclient*, ucanvas*))(original))(viewport_client, canvas);
		}
		else if (!canvas) {
			return ((void(*)(ugameviewportclient*, ucanvas*))(original))(viewport_client, canvas);
		}

	}

	return ((void(*)(ugameviewportclient*, ucanvas*))(original))(viewport_client, canvas);
}