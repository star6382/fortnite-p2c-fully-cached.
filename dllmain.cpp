#include "Game/impl.hpp"

void(*DrawTransitionOriginal)(uobject*, UCanvas*) = nullptr;
void DrawTransition(uobject* _this, UCanvas* Canvas) {
	uengine = _this;
	canvas = Canvas;
	if (!_this || !Canvas) {
		return;
	}

	screenx = reinterpret_cast<UCanvas*>(Canvas)->SZE().X;
	screeny = reinterpret_cast<UCanvas*>(Canvas)->SZE().Y;
	if (!screenx || !screeny) {
		return;
	}

	tick_callback();

	return DrawTransitionOriginal(_this, Canvas);
}

bool entry(bool caller) {
	if (caller) {
		sdk.world_data.world();
		DrawTransitionOriginal = vmt<decltype(DrawTransitionOriginal)>(reinterpret_cast<void*>(sdk.world_data.viewport_client), DrawTransition, 0x71);
	}

	return true;
}

bool DllMain(void* exports, int caller, void* lp) {
    if (caller == 0x1) {
		//erase_pe(exports);
		entry(true);
	}

	return true;
}