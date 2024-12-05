#pragma once
#include <memory>
#include <typeinfo>

class EntityComponent
{
public:
	uint64_t id;
	uint64_t size;
	const type_info& type;
	uint8_t* data;

	EntityComponent(uint64_t id, uint64_t size, const type_info& type, uint8_t* data) : id(id), size(size), type(type), data(data) {

	}
};
