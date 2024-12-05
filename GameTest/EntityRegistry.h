#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include "EntityComponent.h"

class EntityGroup;

class EntityRegistry
{
	friend EntityGroup;

public:
	EntityRegistry();
	EntityRegistry(EntityRegistry&) = delete;
	EntityRegistry(EntityRegistry&&) = delete;
	~EntityRegistry();

	template<typename TComponent>
	EntityComponent GetComponentInfo() {
		//Find component info.
		for (int i = 0; i < m_ComponentTypes.size(); ++i)
			if (m_ComponentTypes.at(i).type == typeid(TComponent))
				return m_ComponentTypes.at(i);
		if (m_ComponentTypes.size() == 64)
			throw "Cannot register more than 64 unique component types.";
		//Add component info.
		EntityComponent c = EntityComponent(m_ComponentTypes.size(),
			sizeof(TComponent),
			typeid(TComponent),
			new uint8_t[m_ComponentDataCapacity * sizeof(TComponent)]);
		m_ComponentTypes.push_back(c);
		return c;
	}
	template<typename TComponent>
	uint64_t GetComponentId() {
		return GetComponentInfo<TComponent>().id;
	}
	template<typename TComponent>
	uint64_t GetComponentSize() {
		return GetComponentInfo<TComponent>().size;
	}
	template<typename TComponent>
	uint64_t GetComponentType() {
		return GetComponentInfo<TComponent>().type;
	}
	template<typename TComponent>
	uint64_t GetComponentMask() {
		return ((uint64_t)1) << GetComponentId<TComponent>();
	}

	/// <summary>
	/// Creates an entity.
	/// </summary>
	/// <returns>The entity.</returns>
	Entity CreateEntity();
	/// <summary>
	/// True if the entity exists.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <returns>True if the entity exists.</returns>
	bool Exists(Entity entity);
	/// <summary>
	/// Destroys an entity.
	/// </summary>
	/// <param name="entity">The entity.</param>
	void DestroyEntity(Entity entity);
	/// <summary>
	/// Gets the entities that have all required components and no excluded components.
	/// </summary>
	/// <param name="requiredComponents">The required components.</param>
	/// <param name="excludedComponents">The excluded components.</param>
	/// <returns>The entities that have all required components and no excluded components.</returns>
	EntityGroup GetEntities(uint64_t requiredComponents, uint64_t excludedComponents);
	/// <summary>
	/// Adds a component to an entity.
	/// </summary>
	/// <typeparam name="TComponent">The component type.</typeparam>
	/// <typeparam name="...Args">The argument types.</typeparam>
	/// <param name="entity">The entity.</param>
	/// <param name="...args">The args.</param>
	/// <returns>The component.</returns>
	template<typename TComponent, typename... Args>
	TComponent& AddComponent(Entity entity, Args... args) {
		uint64_t componentId = GetComponentId<TComponent>();
		EntityMeta& meta = m_Entities[entity.index];
		m_ComponentDataMask[meta.componentIndex] |= GetComponentMask<TComponent>();
		TComponent& result = ((TComponent*)m_ComponentTypes.at(componentId).data)[meta.componentIndex];
		result = TComponent(std::forward<Args>(args)...);
		return result;
	}
	/// <summary>
	/// Gets a component in an entity.
	/// </summary>
	/// <typeparam name="TComponent">The component type.</typeparam>
	/// <param name="entity">The entity.</param>
	/// <returns>The component.</returns>
	template<typename TComponent>
	TComponent& GetComponent(Entity entity) {
		uint64_t componentId = GetComponentId<TComponent>();
		EntityMeta& meta = m_Entities[entity.index];
		return ((TComponent*)m_ComponentTypes.at(componentId).data)[meta.componentIndex];
	}
	/// <summary>
	/// Gets a component in an entity or returns a fallback value.
	/// </summary>
	/// <typeparam name="TComponent">The component type.</typeparam>
	/// <param name="entity">The entity.</param>
	/// <returns>The component.</returns>
	template<typename TComponent>
	TComponent GetComponentOrDefault(Entity entity, const TComponent& fallback) {
		if (HasComponent<TComponent>(entity))
			return GetComponent<TComponent>(entity);
		return fallback;
	}
	/// <summary>
	/// True if the entity has the component.
	/// </summary>
	/// <typeparam name="TComponent">The component type.</typeparam>
	/// <param name="entity">The entity.</param>
	template<typename TComponent>
	bool HasComponent(Entity entity) {
		uint64_t componentMask = GetComponentMask<TComponent>();
		EntityMeta& meta = m_Entities[entity.index];
		return (m_ComponentDataMask[meta.componentIndex] & componentMask) != (uint64_t)(0);
	}
	/// <summary>
	/// Removes a component from an entity.
	/// </summary>
	/// <typeparam name="TComponent">The component type.</typeparam>
	/// <param name="entity">The entity.</param>
	template<typename TComponent>
	void RemoveComponent(Entity entity) {
		EntityMeta& meta = m_Entities[entity.index];
		m_ComponentDataMask[meta.componentIndex] &= ~GetComponentMask<TComponent>();
	}

private:
	uint32_t m_EntitiesCapacity;
	uint32_t m_FreeHead;
	std::unique_ptr<EntityMeta[]> m_Entities;

	uint32_t m_ComponentDataCapacity;
	uint32_t m_ComponentDataCount;
	std::unique_ptr<uint32_t[]> m_ComponentDataEntity;
	std::unique_ptr<uint64_t[]> m_ComponentDataMask;

	std::vector<EntityComponent> m_ComponentTypes;
};

/*
[Implementation Notes]
Right now the entity registry is implemented hazardously with c methods like memcpy.
This is bad because deconstructors will not be called for components. I don't care because I don't have time.
CmpText is the only complex component I have so I'm doing nasty shit to work around the lack of deconstructors.
Maybe use a virtual container for each component buffer so I can properly construct and deconstruct components?
Oh well, no time to fix.
*/
