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
		/// �G���e�B�e�B�}�l�[�W���[�̎擾
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] EntityManager *GetEntityManager() const;


	private:
		void SortSystem();

		std::vector<std::unique_ptr<SystemBase>> system_list_;

	};
}

