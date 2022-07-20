#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Vector3.h"
class TransformComponent :
	public Component
{
public:
	struct Transform {
		Vector3 position	=	{ 0,0,0 };	// ���W
		// �x�N�g���ňړ�
		void Translate(float x, float y, float z) {
			position.x += x;
			position.y += y;
			position.z += z;
		}
		void Translate(Vector3 velocity) {
			position += velocity;
		}

		Vector3 rotate		=	{ 0,0,0 };		// ��]
		Vector3 scale		=	{ 1,1,1 };		// �g��
		

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
public:
	TransformComponent();

	void Infomation() override;

	void VirtualUpdate() override;

	// �g�����X�t�H�[��
	Transform *GetTransform() { return &transform_; }
	// void SetTransform(const Transform &transform) { transform_ = transform; }

	// �s��̎󂯓n��
	XMMATRIX GetMatrix() { return matrix_; }

	// �e�Z�b�^�[
	void SetPosition(XMFLOAT3 position)	{ transform_ .position = position;}
	void SetRotate(XMFLOAT3 rotate)		{ transform_.rotate = rotate; }
	void SetScale(XMFLOAT3 scale)		{ transform_.scale = scale;}


private:
	
	Transform transform_;
	XMMATRIX matrix_;

	//XMFLOAT3 position_;
	//XMFLOAT3 rotate_;
	//XMFLOAT3 scale_;

	

};

