#pragma once
#include <cstdint>

namespace ecs
{
	struct ComponentdData
	{
	};

	constexpr int  kComponentSize{ 16 };


	struct Entity
	{
		explicit Entity(const std::uint32_t index)
		{
			index_ = index;
		}

		Entity(const std::uint32_t chunk_index, const std::uint32_t index)
		{
			chunk_index_ = chunk_index;
			index_ = index;
		}
	};

	std::uint32_t index_;
	std::uint32_t chunk_index_;
}

struct Position : ecs::ComponentdData
{

};