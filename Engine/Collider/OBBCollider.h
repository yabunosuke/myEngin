#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
class OBBCollider :
	public BaseCollider , public OBB
{
public:
	OBBCollider(std::string name = "OBB", XMFLOAT3 offset = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 }) :
		offset_(offset),
		scale_(scale)
	{
		// 球形状をセット
		shapeType = SHAPE_OBB;
		this->collisionName = name;
	}

	void Update() override;
	/// <summary>
	/// 描画用
	/// </summary>
	void Draw() override;


	/// <summary>
	/// ImGui用
	/// </summary>
	void Infomation() override;

private:
	// オブジェクト中心からのオフセット
	XMFLOAT3 offset_;
	XMFLOAT3 scale_;

};

