#pragma once
#include <memory>

struct Entity {
	uint32_t index;
	uint32_t version;
};
struct EntityMeta {
	uint32_t componentIndex;
	uint32_t version;
};
