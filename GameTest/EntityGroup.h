#pragma once
#include <memory>
#include "Entity.h"
#include "EntityRegistry.h"

/// <summary>
/// Represents a group of entities. Note that accessing components through entity groups is only valid until an entity is created or destryed.
/// </summary>
class EntityGroup
{
	friend EntityRegistry;

public:
	EntityGroup() = delete;
	EntityGroup(EntityGroup& source) = delete;
	EntityGroup(EntityGroup&& source) noexcept :
		m_Registry(source.m_Registry),
		m_ComponentIndicies(source.m_ComponentIndicies),
		m_EntitiesCount(source.m_EntitiesCount) {
		source.m_Registry = nullptr;
		source.m_ComponentIndicies = nullptr;
		source.m_EntitiesCount = 0;
	}
	~EntityGroup() {
		if (m_ComponentIndicies != nullptr) {
			delete[] m_ComponentIndicies;
		}
	}

	/// <summary>
	/// Gets the entity count.
	/// </summary>
	/// <returns></returns>
	uint32_t GetEntityCount() const {
		return m_EntitiesCount;
	}
	/// <summary>
	/// Gets the entity at the specified index.
	/// </summary>
	/// <param name="index">The index.</param>
	/// <returns>The entity.</returns>
	const Entity GetEntity(uint32_t index) const {
		uint32_t cIndex = m_ComponentIndicies[index];
		Entity e;
		e.index = m_Registry->m_ComponentDataEntity[cIndex];
		auto& meta = m_Registry->m_Entities[e.index];
		e.version = meta.version;
		return e;
	}
	/// <summary>
	/// Gets the component at the specified index.
	/// </summary>
	/// <typeparam name="TComponent"></typeparam>
	/// <param name="index">The index.</param>
	/// <returns>The component.</returns>
	template<typename TComponent>
	TComponent& GetComponent(uint32_t index) const {
		auto cId = m_Registry->GetComponentId<TComponent>();
		auto cIndex = m_ComponentIndicies[index];
		return *(((TComponent*)m_Registry->m_ComponentTypes.at(cId).data) + cIndex);
	}
	/// <summary>
	/// True if the component exists at the specified index.
	/// </summary>
	/// <typeparam name="TComponent"></typeparam>
	/// <param name="index">The index.</param>
	/// <returns>True if the components exist at the specified index.</returns>
	template<typename TComponent>
	bool HasComponent(uint32_t index) const {
		auto cIndex = m_ComponentIndicies[index];
		auto cMask = m_Registry->GetComponentMask<TComponent>();
		return (m_Registry->m_ComponentDataMask[cIndex] & cMask) == cMask;
	}
	/// <summary>
	/// True if the components exist at the specified index.
	/// </summary>
	/// <param name="index">The index.</param>
	/// <param name="mask">The component mask.</param>
	/// <returns>True if the components exist at the specified index.</returns>
	bool HasComponents(uint32_t index, uint64_t mask) const {
		uint32_t cIndex = m_ComponentIndicies[index];
		return (m_Registry->m_ComponentDataMask[cIndex] & mask) == mask;
	}
private:
	EntityRegistry* m_Registry;
	uint32_t* m_ComponentIndicies;
	uint32_t m_EntitiesCount;

	EntityGroup(EntityRegistry* m_Registry, uint32_t* m_ComponentIndicies, const uint32_t& m_EntitiesCount)
		: m_Registry(m_Registry), m_ComponentIndicies(m_ComponentIndicies), m_EntitiesCount(m_EntitiesCount)
	{
	}
};
