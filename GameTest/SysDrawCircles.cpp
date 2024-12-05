#include "stdafx.h"
#include "SysDrawCircles.h"
#include "App/app.h"
#include "EntityGroup.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpOverlay.h"
#include "CmpShapeCircle.h"

#define CIRCLE_VERTEX_COUNT (16.0f)

static void Draw(const EntityGroup& e) {
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& shp = e.GetComponent<CmpShapeCircle>(i);
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(col.value.x, col.value.y, col.value.z, col.value.w);
		glVertex2f(pos.value.x, pos.value.y);
		for (float j = 0; j <= CIRCLE_VERTEX_COUNT; j += 1) {
			float r = j * (2.0f * PI / CIRCLE_VERTEX_COUNT);
			glVertex2f(pos.value.x + cosf(r) * shp.radius, pos.value.y + sinf(r) * shp.radius);
		}
		glEnd();
	}
}

void SysDrawCircles::Render() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapeCircle>()
		| scene.entities.GetComponentMask<CmpColor>();
	auto e = scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, e));
}
void SysDrawCircles::RenderOverlay() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapeCircle>()
		| scene.entities.GetComponentMask<CmpColor>()
		| scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, 0));
}
