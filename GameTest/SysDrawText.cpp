#include "stdafx.h"
#include "SysDrawText.h"
#include "App/app.h"
#include "EntityGroup.h"

#include "CmpColor.h"
#include "CmpPosition.h"
#include "CmpOverlay.h"
#include "CmpText.h"

static void Draw(Scene& scene, const EntityGroup& e) {
	glLineWidth(2);
	for (uint32_t i = 0; i < e.GetEntityCount(); ++i) {
		auto& col = e.GetComponent<CmpColor>(i);
		auto& pos = e.GetComponent<CmpPosition>(i);
		auto& tex = e.GetComponent<CmpText>(i);

		auto h = glutStrokeHeight(tex.font);
		auto w = glutStrokeLengthf(tex.font, (unsigned char*)tex.text);
		float2 vs = scene.GetWorldViewMax() - scene.GetWorldViewMin();
		auto xs = vs.y / vs.x * tex.scale / h;
		auto ys = tex.scale / h;

		glColor4f(col.value.x, col.value.y, col.value.z, col.value.w);
		glPushMatrix();
		glTranslatef(pos.value.x - w * xs * 0.5f, pos.value.y - h * ys * 0.5f, 0.0f);

		glScalef(xs, ys, 1.0f);
		glutStrokeString(tex.font, (unsigned char*)tex.text);
		glScalef(1.0f / xs, 1.0f / ys, 1.0f);

		glPopMatrix();
	}
	glLineWidth(1);
}

void SysDrawText::Render() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpText>()
		| scene.entities.GetComponentMask<CmpColor>();
	auto e = scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene, scene.entities.GetEntities(r, e));
}
void SysDrawText::RenderOverlay() {
	auto r = scene.entities.GetComponentMask<CmpPosition>()
		| scene.entities.GetComponentMask<CmpText>()
		| scene.entities.GetComponentMask<CmpColor>()
		| scene.entities.GetComponentMask<CmpOverlay>();
	Draw(scene, scene.entities.GetEntities(r, 0));
}
