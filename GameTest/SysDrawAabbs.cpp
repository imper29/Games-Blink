#include "stdafx.h"
#include "SysDrawAabbs.h"
#include "App/app.h"
#include "EntityGroup.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpOverlay.h"
#include "CmpShapeAabb.h"

static void Draw(const EntityGroup& e) {
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& shp = e.GetComponent<CmpShapeAabb>(i);
		auto bl = pos.value - shp.extents;
		auto tr = pos.value + shp.extents;
		glBegin(GL_QUADS);
		glColor4f(col.value.x, col.value.y, col.value.z, col.value.w);
		glVertex2f(bl.x, bl.y);
		glVertex2f(bl.x, tr.y);
		glVertex2f(tr.x, tr.y);
		glVertex2f(tr.x, bl.y);
		glEnd();
	}
}

void SysDrawAabbs::Render() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapeAabb>()
		| scene.entities.GetComponentMask<CmpColor>();
	auto e = scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, e));
}
void SysDrawAabbs::RenderOverlay() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpShapeAabb>()
		| scene.entities.GetComponentMask<CmpColor>()
		| scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene.entities.GetEntities(r, 0));
}
