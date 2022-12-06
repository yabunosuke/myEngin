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
		/// エンティティマネージャーの取得
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] EntityManager *GetEntityManager() const;


	private:

	};
}

