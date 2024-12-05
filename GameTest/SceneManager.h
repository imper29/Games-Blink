#pragma once
#include "Scene.h"
#include <memory>

class SceneManager
{
private:
	static std::unique_ptr<Scene> m_Scene;
	static std::unique_ptr<Scene> m_SceneToLoad;

public:
	static void Init();
	static void Shutdown();
	static void Render();
	static void Update(float deltaTime);

	static void LoadScene(std::unique_ptr<Scene>&& scene) {
		m_SceneToLoad = std::move(scene);
	}
};
