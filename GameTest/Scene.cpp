#include "stdafx.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "App/app.h"

Scene::Scene() : time(0.0f), viewWorldPosition(0, 0), viewWorldHeight(10) {

}
Scene::~Scene() {
	for (int i = (int)m_Systems.size() - 1; i >= 0; --i)
		delete m_Systems.at(i);
}

void Scene::Render() {
	float windowWidth = (float)glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = (float)glutGet(GLUT_WINDOW_HEIGHT);
	float aspectRatio = windowWidth / windowHeight;
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glOrtho(viewWorldPosition.x - viewWorldHeight * aspectRatio
		  , viewWorldPosition.x + viewWorldHeight * aspectRatio
		  , viewWorldPosition.y - viewWorldHeight
		  , viewWorldPosition.y + viewWorldHeight, -1, 1);
	for (auto i = 0; i < m_Systems.size(); ++i)
		m_Systems.at(i)->Render();
	glPopMatrix();
}
void Scene::RenderOverlay() {
	glPushMatrix();
	glOrtho(0, 100, 0, 100, -1, 1);
	for (auto i = 0; i < m_Systems.size(); ++i)
		m_Systems.at(i)->RenderOverlay();
	glPopMatrix();
}
void Scene::Update(float timestep) {
	time += timestep;
	for (auto i = 0; i < m_Systems.size(); ++i)
		m_Systems.at(i)->Update(time, timestep);
}

const float3x3 Scene::GetViewToWorld() const
{
	float2 windowSize = float2((float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
	float2 viewSize = float2(viewWorldHeight * windowSize.x / windowSize.y, viewWorldHeight);

	return float3x3(float3(2.0f / windowSize.x, 0.0f, -1.0f),
					float3(0.0f, 2.0f / windowSize.y, -1.0f),
					float3(0.0f, 0.0f, 1.0f))
		 * float3x3(float3(viewSize.x, 0.0f, 0.0f),
					float3(0.0f, viewSize.y, 0.0f),
					float3(0.0f, 0.0f, 1.0f))
		 * float3x3(float3(1.0f, 0.0f, viewWorldPosition.x),
					float3(0.0f, 1.0f, viewWorldPosition.y),
					float3(0.0f, 0.0f, 1.0f));
}
const float3x3 Scene::GetWorldToView() const
{
	return GetViewToWorld().inverse();
}
const float2 Scene::GetWorldMousePos() const {
	float3 mousePos = float3(0.0f, 0.0f, 1.0f);
	App::GetMousePos(mousePos.x, mousePos.y);
	mousePos = Scene::GetViewToWorld() * mousePos;
	return float2(mousePos.x, mousePos.y);
}
const float2 Scene::GetViewMousePos() const {
	float2 mousePos;
	App::GetMousePos(mousePos.x, mousePos.y);
	mousePos = mousePos / float2((float)APP_VIRTUAL_WIDTH, (float)APP_VIRTUAL_HEIGHT);
	return mousePos * 100.0f;
}
const float2 Scene::GetWorldViewMin() const
{
	float2 windowSize = float2((float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
	return float2(viewWorldPosition.x - viewWorldHeight * windowSize.x / windowSize.y, viewWorldPosition.y - viewWorldHeight);
}
const float2 Scene::GetWorldViewMax() const
{
	float2 windowSize = float2((float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
	return float2(viewWorldPosition.x + viewWorldHeight * windowSize.x / windowSize.y, viewWorldPosition.y + viewWorldHeight);
}
