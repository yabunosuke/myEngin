#include "World.h"

#include "SystemBase.h"

ecs::World::World()
{

}

ecs::World::~World()
{
	for (auto &&system : system_list_)
	{
		system->OnDestroy();
	}
}

void ecs::World::FixedUpdate()
{
	for (auto &&system : system_list_)
	{
		system->OnFixedUpdate();
	}
}

void ecs::World::Update()
{
	for (auto &&system : system_list_)
	{
		system->OnUpdate();
	}
}

void ecs::World::SortSystem()
{
}
