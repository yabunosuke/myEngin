#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Vector3.h"
class TransformComponent :
	public Component
{
public:
	struct Transform {
		Vector3 position	=	{ 0,0,0 };	// 座標
		// ベクトルで移動
		void Translate(float x, float y, float z) {
			position.x += x;
			position.y += y;
			position.z += z;
		}
		void Translate(Vector3 velocity) {
			position += velocity;
		}

		Vector3 rotate		=	{ 0,0,0 };		// 回転
		Vector3 scale		=	{ 1,1,1 };		// 拡大
		

		// 正面ベクトル
		Vector3 forward() {

			return Vector3(0, 0, 0);
		};
		// 右ベクトル
		Vector3 right() {
			return Vector3(0, 0, 0);
		};
		// 上ベクトル
		Vector3 up() {
			return Vector3(0, 0, 0);
		};
	};
public:
	TransformComponent();

	void Infomation() override;

	void VirtualUpdate() override;

	// トランスフォーム
	Transform *GetTransform() { return &transform_; }
	// void SetTransform(const Transform &transform) { transform_ = transform; }

	// 行列の受け渡し
	XMMATRIX GetMatrix() { return matrix_; }

	// 各セッター
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

