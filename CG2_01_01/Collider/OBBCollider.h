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
		// ���`����Z�b�g
		shapeType = SHAPE_OBB;
		this->collisionName = name;
	}

	void Update() override;
	/// <summary>
	/// �`��p
	/// </summary>
	void Draw() override;


	/// <summary>
	/// ImGui�p
	/// </summary>
	void Infomation() override;

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMFLOAT3 offset_;
	XMFLOAT3 scale_;

};

