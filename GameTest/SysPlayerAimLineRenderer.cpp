#include "stdafx.h"
#include "SysPlayerAimLineRenderer.h"

#include "App/app.h"
#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpPlayerCharacter.h"

void SysPlayerAimLineRenderer::Render()
{
	auto r = scene.entities.GetComponentMask<CmpColor>()
		   | scene.entities.GetComponentMask<CmpPosition>()
		   | scene.entities.GetComponentMask<CmpPlayerCharacter>();
	auto e = scene.entities.GetEntities(r, 0);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& chr = e.GetComponent<CmpPlayerCharacter>(i);

		auto controller = App::GetController(chr.gamepadId);

		float2 direction;
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2);
		if (float2::normalizeSafe(float2(controller.GetRightThumbStickX(), controller.GetRightThumbStickY()), direction)) {
			glBegin(GL_LINES);
			glLineWidth(30);
			glColor4f(col.value.x, col.value.y, col.value.z, 0.5f);
			glVertex2f(pos.value.x, pos.value.y);
			glVertex2f(pos.value.x + direction.x * 2.0f, pos.value.y + direction.y * 2.0f);
			glEnd();
		}
		glLineWidth(1);
	}
}
