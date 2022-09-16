#pragma once
#include "Object/Component/Component.h"
#include "BaseCollider.h"

class CollisionManager;

class AbstractScene;

class Collider :
	public Component
{
public:
	Collider(AbstractScene *scene, CollisionShapeType type = SHAPE_SPHERE);
	//~ColliderComponent();
	/// <summary>
	/// 初期化
	/// </summary>
	void ComponentInitialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void ComponentUpdate() override;
	/// <summary>
	/// 描画
	/// </summary>
	void ComponentDraw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void ComponentFinalize() override;

	/// <summary>
	/// 情報
	/// </summary>
	void Infomation() override;


	//===========================================
	//
	//	  アクセッサ
	//
	//===========================================
	
	// コライダーがトリガーかどうか
	yEngin::Property<bool> isTrigger
	{
		is_trigger_,
		yEngin::AccessorType::AllAccess
	};

private:
	// コライダーがトリガーかどうか
	bool is_trigger_ = false;

	// コライダー
	std::shared_ptr<BaseCollider> collider;
	CollisionShapeType type_;
	std::shared_ptr<CollisionManager> collision_manager_;
};

