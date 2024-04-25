#include "core.hpp"
#include "hook.hpp"

void main() {

   // if (auto world = uobject::static_find_object(nullptr, nullptr, L"Frontend", reinterpret_cast<uobject*>(-1))) {
    auto gworld = game_base + 0x124BF388;
    auto world = (uworld*)*(uintptr_t*)(gworld);

    MessageBoxA(nullptr, "game_instance", "game_instance", MB_OK);
    auto game_instance = ue::k2_statics->get_game_instance((uobject*)world);

    auto local_player = game_instance->get_local_players()[0];

    MessageBoxA(nullptr, "get_player_controller", "get_player_controller", MB_OK);
    auto controller = ue::k2_statics->get_player_controller((uobject*)world, std::int32_t(0));
    auto viewport = local_player->get_viewport();

    MessageBoxA(nullptr, "hook", "hook", MB_OK);
    original = hook::vmt<decltype(original)>(reinterpret_cast<void*>(viewport), postrender, 0x71);
      // original = hook::vmt<decltype(original)>(std::uintptr_t(viewport), std::uintptr_t(postrender), 0x71);
  //  }
}
 