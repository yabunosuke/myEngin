#pragma once

#include "CollisionInfo.h"
//#include "GameObjCommon.h"
#include "GameObject.h"
#include <string>


//衝突判定形状の型定義
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定

	//3d
	SHAPE_POINT		= 0x00001,	//点
	SHAPE_RAY		= 0x00002,	//線（終端なし）
	SHAPE_LINE		= 0x00004,	//線（終端あり）
	SHAPE_TRIANGLE	= 0x00008,	//三角形
	SHAPE_SPHERE	= 0x00010,	//球
	SHAPE_CYLINDER	= 0x00020,	//筒
	SHAPE_AABB		= 0x00040,	//AABB
	SHAPE_OBB		= 0x00080,	//OBB
	SHAPE_CAPSULE	= 0x00100,	//カプセル
	SHAPE_MESH		= 0x00200,	//メッシュ
	SHAPE_PLANE		= 0x00400,	//無限平面
	// = 0x000	//コリジョンタイプ

	BOX_2D			= 0x10000,
};

class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;		//仮想デストラクタ

	//オブジェクトをセット
	inline void SetObject(GameObject *object) { this->object = object; }
	//セットされているオブジェクトをリターン
	inline GameObject *GetObject3d() { return object; }

	//更新
	virtual void Update() = 0;

	// コライダー描画用
	virtual void Draw() = 0;

	/// <summary>
	/// ImGui用
	/// </summary>
	virtual void Infomation();

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }
	inline std::string GetCollisionName() { return collisionName; }

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info)
	{
		//object->OnCollision(info);
	}

	// 当たり判定属性をセット
	inline void SetAttribute(std::string collision_name)
	{
		this->collisionName = collision_name;
	}

	// 当たり判定属性を削除
	inline void RemoveAttribute(std::string tag)
	{
		this->collisionName  = nullptr;
	}

protected:
	GameObject *object = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//当たり判定タグ
	std::string collisionName;
};