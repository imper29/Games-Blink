#pragma once
#include "App/app.h"

struct CmpText
{
	void* font;
	float scale;
	const char* text;

	CmpText(const char* value, float scale)
		: font(GLUT_STROKE_MONO_ROMAN)
		, scale(scale)
		, text(value)
	{

	}
	CmpText(const char* value, float scale, void* font)
		: font(font)
		, scale(scale)
		, text(value)
	{

	}
};
