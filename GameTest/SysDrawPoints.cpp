#include "stdafx.h"
#include "SysDrawPoints.h"
#include "App/app.h"
#include "EntityGroup.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpOverlay.h"
#include "CmpShapePoint.h"

static void Draw(const EntityGroup& e) {
	glPointSize(6);
	glBegin(GL_POINTS);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		glColor4f(col.value.x, col.value.y, col.value.z, col.value.w);
		glVertex2f(pos.value.x, pos.value.y);
	}
	glEnd();
}

void SysDrawPoints::Render() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapePoint>()
		| scene.entities.GetComponentMask<CmpColor>();
	auto e = scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, e));
}
void SysDrawPoints::RenderOverlay() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapePoint>()
		| scene.entities.GetComponentMask<CmpColor>()
		| scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, 0));
}
