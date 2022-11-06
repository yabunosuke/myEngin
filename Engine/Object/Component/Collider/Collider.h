#pragma once
#include "Object/Component/Component.h"
#include "Math/Mathf.h"
#include "Math/Quaternion.h"
#include <map>

enum class CollisonType
{
	None	= -1,
	AABB	= 1 << 0,
	Sphere	= 1 << 1,
	OBB		= 1 << 2,
	Capsule = 1 << 3,

};

class Collider :
	public Component
{
public:
	Collider();
	~Collider();
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
	yEngine::Property<bool> isTrigger
	{
		&is_trigger_,
		yEngine::AccessorType::AllAccess
	};
	yEngine::Property<CollisonType> collisionType
	{
		&collision_type_,
		yEngine::AccessorType::ReadOnly
	};


	std::map<int, bool> hitlist_;
protected:
	// コライダーがトリガーかどうか
	bool is_trigger_{ false };
	CollisonType collision_type_ = CollisonType::None;
};

