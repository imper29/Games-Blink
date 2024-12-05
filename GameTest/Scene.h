#pragma once
#include "EntityRegistry.h"
class SceneSystem;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Render();
	virtual void RenderOverlay();
	virtual void Update(float timestep);

	template<typename TSystem, typename ...Args>
	TSystem& AddOrGetSystem(Args... args) {
		for (auto i = 0; i < m_Systems.size(); ++i)
			if (typeid(*m_Systems.at(i)) == typeid(TSystem))
				return *((TSystem*)m_Systems.at(i));
		TSystem* system = new TSystem(*this, std::forward<Args>(args)...);
		m_Systems.push_back(system);
		return *system;
	}
	const std::vector<SceneSystem*> GetSystems() const {
		return m_Systems;
	}

	const float3x3 GetViewToWorld() const;
	const float3x3 GetWorldToView() const;
	const float2 GetWorldMousePos() const;
	const float2 GetViewMousePos() const;
	const float2 GetWorldViewMin() const;
	const float2 GetWorldViewMax() const;

public:
	EntityRegistry entities;
	float viewWorldHeight;
	float time;
	float2 viewWorldPosition;
private:
	std::vector<SceneSystem*> m_Systems;
};
