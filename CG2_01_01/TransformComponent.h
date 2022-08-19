#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Vector3.h"

struct Transform {
	Vector3 position = { 0,0,0 };		// ���W
	XMFLOAT4 quaternion = { 0,1,0,0, };		// �N�I�[�^�j�I����]
	Vector3 scale = { 1,1,1 };		// �g��

	// �x�N�g���ňړ�
	void Translate(float x, float y, float z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}
	void Translate(Vector3 velocity) {
		position += velocity;
	}

	// ���ʃx�N�g��
	Vector3 forward() {

		return Vector3(0, 0, 0);
	};
	// �E�x�N�g��
	Vector3 right() {
		return Vector3(0, 0, 0);
	};
	// ��x�N�g��
	Vector3 up() {
		return Vector3(0, 0, 0);
	};
};

class TransformComponent :
	public Component
{
public:
	TransformComponent();

	void Infomation() override;

	void ComponentUpdate() override;

	// �g�����X�t�H�[��
	Transform *GetTransform() { return &transform_; }
	// void SetTransform(const TRANSFORM &transform) { transform_ = transform; }

	// �s��̎󂯓n��
	XMMATRIX GetMatrix() { return matrix_; }

	// �e�Z�b�^�[
	void SetPosition(XMFLOAT3 position)	{ transform_.position = position;}
	void SetRotate(XMFLOAT3 rotate)
	{
		XMStoreFloat4(&transform_.quaternion,
			XMQuaternionRotationRollPitchYaw(
				rotate.x,
				rotate.y,
				rotate.z
			)
		);
	}
	void SetScale(XMFLOAT3 scale)		{ transform_.scale = scale;}


private:
	
	Transform transform_;
	XMMATRIX matrix_;

};

