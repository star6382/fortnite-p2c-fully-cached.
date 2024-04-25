#include "settings.h"
#include "sdk.hpp"

void Line(UCanvas* canvas, FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, flinearcolor RenderColor, bool Outlined) {
	if (Outlined) {
		canvas->DrawLine(ScreenPositionA, ScreenPositionB, Thickness + 1.f, flinearcolor(0.f, 0.f, 0.f, 1.f));
		canvas->DrawLine(ScreenPositionA, ScreenPositionB, Thickness - 1.f, flinearcolor(0.f, 0.f, 0.f, 1.f));
	}

	canvas->DrawLine(ScreenPositionA, ScreenPositionB, Thickness, RenderColor);
}

void Text(const char* RenderText, FVector2D ScreenPosition, float FontSize, flinearcolor RenderColor, bool CentredX, bool Outlined) {
	std::string String(RenderText);
	std::wstring WideString = std::wstring(String.begin(), String.end());
	canvas->ADrawText(ScreenPosition, FontSize, WideString.c_str(), RenderColor, CentredX, Outlined);
}