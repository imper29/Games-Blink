#include "stdafx.h"
#include "SceneManager.h"
#include "MenuMain.h"
#include <gl/GLU.h>
#include "Gamepad.h"

void Init() {
	Gamepad::Init();
	SceneManager::Init();
}
void Update(float deltaTime) {
	Gamepad::Update();
	SceneManager::Update(deltaTime / 1000.0f);
}
void Render() {
	SceneManager::Render();
}
void Shutdown() {
	SceneManager::Shutdown();
}

std::unique_ptr<Scene> SceneManager::m_Scene;
std::unique_ptr<Scene> SceneManager::m_SceneToLoad;

void SceneManager::Init() {
	m_Scene = std::unique_ptr<Scene>(new MenuMain());
}
void SceneManager::Shutdown() {
	m_Scene = nullptr;
}
void SceneManager::Render() {
	if (m_Scene != nullptr) {
		m_Scene->Render();
		m_Scene->RenderOverlay();
	}
}
void SceneManager::Update(float deltaTime) {
	if (m_Scene != nullptr) {
		float dt = 1.0f / 256.0f;
		float tt = 0.0f;
		while (tt < deltaTime) {
			tt += dt;
			m_Scene->Update(dt);
		}
	}
	if (m_SceneToLoad != nullptr) {
		m_Scene = std::move(m_SceneToLoad);
	}
}
