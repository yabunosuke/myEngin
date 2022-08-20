#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(std::string name = "Sphere", XMVECTOR offset = {0,0,0,0}, float radius = 1.0f) :
		offset(offset),
		radius_(radius)
	{
		// 球形状をセット
		shapeType = SHAPE_SPHERE;
		this->collisionName = name;
	}

	// 更新
	void Update() override;

	/// <summary>
	/// 描画用
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui用
	/// </summary>
	void Infomation() override;

	const XMVECTOR& GetOffset() { return offset; }

	void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	float GetRadius() { return radius_; }

	void SetRadius(float radius) { this->radius_ = radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float radius_;
};