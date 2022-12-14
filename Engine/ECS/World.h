#pragma once
#include <vector>
#include <memory>

namespace ecs
{

	class SystemBase;
	
	class World
	{
		friend class EntityManager;
	public:
		World();
		~World();


		void FixedUpdate();
		void Update();

		template<class T>
		void AddSystem()
		{
		}

		/// <summary>
		/// エンティティマネージャーの取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] EntityManager *GetEntityManager() const;


	private:
		void SortSystem();

		std::vector<std::unique_ptr<SystemBase>> system_list_;

	};
}

