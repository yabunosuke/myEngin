#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(std::string name = "Sphere", XMVECTOR offset = {0,0,0,0}, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		// ���`����Z�b�g
		shapeType = SHAPE_SPHERE;
		this->collisionName = name;
	}

	// �X�V
	void Update() override;

	/// <summary>
	/// �`��p
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui�p
	/// </summary>
	void Infomation() override;

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline float GetRadius() { return radius; }

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���a
	float radius;
};