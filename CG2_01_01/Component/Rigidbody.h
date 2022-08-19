#pragma once

#include "Vector3.h"
#include "Component/Component.h"

#include <DirectXMath.h>

enum class ForceMode
{
	Force,
	Impulse,
};

class Rigidbody :
	public Component
{
public:
	Rigidbody();

	void Infomation() override;


	void AddForce(XMFLOAT3 force, ForceMode foce_mode = ForceMode::Force);
	void AddForce(float x, float y, float z, ForceMode foce_mode = ForceMode::Force);
private:

	// リジッドボディ
	float mass_;         // 質量
	float drag_;         // 力で動く場合の空気抵抗
	float angular_drag_; // トルクで動く場合の空気抵抗
	bool use_gravity_;   // 重力を使用するか
	Vector3 velocity_;	// 加速度（操作非推奨）

	struct FreezePosition { // ワールド座標の指定した軸でリジッドボディの移動を停止
		bool x;
		bool y;
		bool z;
	}freeze_position_;

	struct FreezeRotation { // ローカル座標の指定した軸でリジッドボディの回転を停止
		bool x;
		bool y;
		bool z;
	}freeze_rotation_;
};

