#include "stdafx.h"
#include "EntityRegistry.h"
#include "EntityGroup.h"
#include <memory>

EntityRegistry::EntityRegistry()
{
	//Setup entities.
	m_EntitiesCapacity = 100;
	m_FreeHead = 0;
	m_Entities = std::make_unique<EntityMeta[]>(m_EntitiesCapacity);
	//Setup entities free list.
	for (uint32_t i = 0; i < m_EntitiesCapacity; ++i)
		m_Entities[i].componentIndex = i + 1;
	m_Entities[m_EntitiesCapacity - 1].componentIndex = std::numeric_limits<uint32_t>::max();
	//Setup components
	m_ComponentDataCapacity = 100;
	m_ComponentDataCount = 0;
	m_ComponentDataEntity = std::make_unique<uint32_t[]>(m_ComponentDataCapacity);
	m_ComponentDataMask = std::make_unique<uint64_t[]>(m_ComponentDataCapacity);
}
EntityRegistry::~EntityRegistry()
{
	for (int i = 0; i < m_ComponentTypes.size(); ++i)
		delete[] m_ComponentTypes.at(i).data;
}

Entity EntityRegistry::CreateEntity() {
	//Resize entity buffer if there are no free entities.
	if (m_FreeHead == std::numeric_limits<uint32_t>::max()) {
		//Resize entity buffer.
		auto oEntitiesCapacity = m_EntitiesCapacity;
		auto nEntitiesCapacity = (oEntitiesCapacity + 1) * 2;
		auto nEntities = std::make_unique<EntityMeta[]>(nEntitiesCapacity);
		memcpy(nEntities.get(), m_Entities.get(), sizeof(EntityMeta) * oEntitiesCapacity);
		m_Entities = std::move(nEntities);
		m_EntitiesCapacity = nEntitiesCapacity;
		//Initialize free list for expanded entity buffer.
		for (uint32_t i = oEntitiesCapacity; i < nEntitiesCapacity; ++i)
			m_Entities[i].componentIndex = i + 1;
		m_Entities[nEntitiesCapacity - 1].componentIndex = m_FreeHead;
		m_FreeHead = oEntitiesCapacity;
	}
	//Resize component buffers if count exceeds capacity.
	if (m_ComponentDataCount >= m_ComponentDataCapacity) {
		auto oComponentDataCount = m_ComponentDataCount;
		auto oComponentDataCapacity = m_ComponentDataCapacity;
		auto nComponentDataCapacity = (oComponentDataCapacity + 1) * 2;
		m_ComponentDataCapacity = nComponentDataCapacity;

		//Resize component data mask.
		auto nComponentDataMask = std::make_unique<uint64_t[]>(nComponentDataCapacity);
		memcpy(nComponentDataMask.get(), m_ComponentDataMask.get(), sizeof(uint64_t) * oComponentDataCount);
		m_ComponentDataMask = std::move(nComponentDataMask);

		//Resize component data entity.
		auto nComponentDataEntity = std::make_unique<uint32_t[]>(nComponentDataCapacity);
		memcpy(nComponentDataEntity.get(), m_ComponentDataEntity.get(), sizeof(uint32_t) * oComponentDataCount);
		m_ComponentDataEntity = std::move(nComponentDataEntity);

		//Resize component data buffers.
		auto componentTypeCount = m_ComponentTypes.size();
		for (int i = 0; i < componentTypeCount; ++i) {
			auto& component = m_ComponentTypes.at(i);
			auto nComponentData = new uint8_t[nComponentDataCapacity * component.size];
			memcpy(nComponentData, component.data, oComponentDataCount * component.size);
			delete[] component.data;
			component.data = nComponentData;
		}
	}
	//Create entity.
	Entity entity;
	entity.index = m_FreeHead;
	EntityMeta& meta = m_Entities[entity.index];
	m_FreeHead = meta.componentIndex;
	meta.componentIndex = m_ComponentDataCount++;
	entity.version = meta.version;
	m_ComponentDataMask[meta.componentIndex] = 0;
	m_ComponentDataEntity[meta.componentIndex] = entity.index;
	return entity;
}
bool EntityRegistry::Exists(Entity entity)
{
	if (entity.index >= m_EntitiesCapacity)
		return false;
	EntityMeta& meta = m_Entities[entity.index];
	return meta.version == entity.version;
}
void EntityRegistry::DestroyEntity(Entity entity) {
	EntityMeta& meta = m_Entities[entity.index];
	if (meta.version == entity.version) {
		--m_ComponentDataCount;
		//Move last entity components back to fill empty cell.
		auto componentTypeCount = m_ComponentTypes.size();
		for (int i = 0; i < componentTypeCount; ++i) {
			auto& component = m_ComponentTypes.at(i);
			memcpy(component.data + (component.size * meta.componentIndex), component.data + (component.size * m_ComponentDataCount), component.size);
		}
		//Notify last entity that it's components were moved.
		auto lastComponent = m_ComponentDataCount;
		auto lastEntity = m_ComponentDataEntity[lastComponent];
		m_ComponentDataEntity[meta.componentIndex] = lastEntity;
		m_ComponentDataMask[meta.componentIndex] = m_ComponentDataMask[lastComponent];
		m_Entities[lastEntity].componentIndex = meta.componentIndex;
		//Notify current entity that it is dead.
		meta.componentIndex = m_FreeHead;
		++meta.version;
		m_FreeHead = entity.index;
	}
}
EntityGroup EntityRegistry::GetEntities(uint64_t requiredComponents, uint64_t excludedComponenets) {
	//Collect entities matching constraints.
	uint32_t entitiesCount = 0;
	uint32_t* componentIndicies = new uint32_t[m_ComponentDataCount];
	for (uint32_t i = 0; i < m_ComponentDataCount; ++i) {
		if ((m_ComponentDataMask[i] & requiredComponents) == requiredComponents) {
			if ((m_ComponentDataMask[i] & excludedComponenets) == 0) {
				componentIndicies[entitiesCount++] = i;
			}
		}
	}
	//Create entity group with entities.
	return EntityGroup(this, componentIndicies, entitiesCount);
}
