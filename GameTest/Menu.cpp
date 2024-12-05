#include "stdafx.h"
#include "Menu.h"

#include "SysDrawCircles.h"
#include "SysDrawPoints.h"
#include "SysDrawAabbs.h"
#include "SysDrawText.h"
#include "SysButtonHandler.h"
#include "SysButtonColorizer.h"

Menu::Menu() : Scene() {
	//Register systems.
	AddOrGetSystem<SysButtonHandler>();
	AddOrGetSystem<SysButtonColorizer>();
	AddOrGetSystem<SysDrawCircles>();
	AddOrGetSystem<SysDrawPoints>();
	AddOrGetSystem<SysDrawAabbs>();
	AddOrGetSystem<SysDrawText>();
}
