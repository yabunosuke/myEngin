#pragma once
namespace ECS
{
	class World
	{
		friend class EntityManager;
	public:
		World();
		~World();

		/// <summary>
		/// �G���e�B�e�B�}�l�[�W���[�̎擾
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] EntityManager *GetEntityManager() const;


	private:

	};
}

