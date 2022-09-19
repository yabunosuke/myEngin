#pragma once
#include "Object/Component/Component.h"
#include "BaseCollider.h"

enum class CollisonType
{
	None,
	Sphere,
	Box,
	Capsule,
	Mesh,

};

class Collider :
	public Component
{
public:
	Collider();
	
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
	yEngin::Property<CollisonType> collisionType
	{
		collision_type_,
		yEngin::AccessorType::ReadOnly
	};
protected:
	// コライダーがトリガーかどうか
	bool is_trigger_ = false;
	CollisonType collision_type_ = CollisonType::None;
};

