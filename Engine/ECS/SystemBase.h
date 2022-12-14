#pragma once
#include "World.h"

namespace ecs
{
	class SystemBase
	{
	public:
		SystemBase(SystemBase &) = delete;
		SystemBase(SystemBase &&) = delete;
		
		// Œ^•ÏŠ·‹ÖŽ~
		explicit SystemBase(World *p_world);
		virtual ~SystemBase() = default;

		virtual void OnCreate() {};
		virtual void OnFixedUpdate() {};
		virtual void OnUpdate() {};
		virtual void OnDestroy() {};

		[[nodiscard]] int GetExecutionOrder() const;

	protected:

	private:
		World *p_world_{ nullptr };
	};
}

