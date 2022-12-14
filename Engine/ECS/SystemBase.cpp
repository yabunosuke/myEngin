#include "SystemBase.h"

ecs::SystemBase::SystemBase(World *p_world) :
	p_world_(p_world)
{
}

int ecs::SystemBase::GetExecutionOrder() const
{
	return 0;
}
