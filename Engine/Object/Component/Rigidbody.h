#pragma once
#include "Math/Vector3.h"
#include "Object/Component/Component.h"
#include "Object/Component/Transform.h"
enum class ForceMode
{
	Force,			// 継続的に力を加える 質量を考慮する
	Acceleration,	// 継続的に力を加える 質量を無視する
	Impulse,		// 瞬間的に力を加える 質量を考慮する
	VelocityChange,	// 瞬間的に力を加える 質量を無視する
};

class Rigidbody :
	public Component
{
public:
	Rigidbody();

	void Infomation() override;

	void ComponentInitialize() override;
	void ComponentFixedUpdate() override;
	void ComponentUpdate() override;


	void AddForce(XMFLOAT3 force, ForceMode force_mode = ForceMode::Force);

	yEngine::Property<Vector3> velocity
	{
		velocity_,
		yEngine::AccessorType::AllAccess
	};
private:
	Vector3 velocity_{ 0.0f,0.0f ,0.0f };		// 加速度（操作非推奨）

	// リジッドボディ
	float mass_			{ 1.0f };			// 質量
	int drag_			{ 1 };				// 力で動く場合の空気抵抗係数
	float angular_drag_ { 1.0f };			// トルクで動く場合の空気抵抗
	bool use_gravity_	{ false };			// 重力を使用するか

	float dinamic_friction_{ 0.6f };		// 動くオブジェクトに対する摩擦
	float static_friction_{ 0.6f };			// 静止したオブジェクトに対する摩擦
	float bounciness_{ 0.0f };				// 反発係数
	enum class FrictionCombine				// 衝突したオブジェクト間の摩擦をどう処理するか
	{
		Average,	// 平均
		Minimum,	// 小さい方を使用
		Maximum,	// 大きい方を使用
		Multiply,	// 乗算
	}friction_combine_ = FrictionCombine::Average;
	enum class BounceCombine				// 衝突したオブジェクト間の跳ね返しをどう処理するか
	{
		Average,	// 平均
		Minimum,	// 小さい方を使用
		Maximum,	// 大きい方を使用
		Multiply,	// 乗算
	}bounce_combine_ = BounceCombine::Average;

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

