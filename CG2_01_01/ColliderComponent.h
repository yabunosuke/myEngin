#pragma once
#include "Component.h"
#include "BaseCollider.h"

class AbstractScene;

class ColliderComponent :
	public Component
{
public:
	ColliderComponent(CollisionShapeType type, AbstractScene &scene);

	/// <summary>
	/// 初期化
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// 描画
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// 情報
	/// </summary>
	void Infomation() override;


private:
	// コライダー
	std::shared_ptr<BaseCollider> collider;

};

