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

enum class RigidbodyInterpolation
{
	None,			// 補間を行わない
	Interpolate,	// 補間を行う
	Extrapolate,	// 外挿補間を行う
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

	/// <summary>
	/// リジッドボディに力を加える
	/// </summary>
	/// <param name="force">ワールド座標上での力のベクトル</param>
	/// <param name="force_mode">適応する力のタイプ</param>
	void AddForce(const Vector3 &force, ForceMode force_mode = ForceMode::Force);
	/// <summary>
	/// リジッドボディに力を加える
	/// </summary>
	/// <param name="x, y, z">ワールド座標上での力のベクトル</param>
	/// <param name="force_mode">適応する力のタイプ</param>
	void AddForce(float x,float y,float z, ForceMode force_mode = ForceMode::Force);


	/// <summary>
	/// オブジェクトがスリープ状態化を
	/// </summary>
	/// <returns></returns>
	bool IsSleep();


	yEngine::Property<Vector3> velocity
	{
		&velocity_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr,
	
	};

	yEngine::Property<bool> useGravity
	{
		&use_gravity_,
		yEngine::AccessorType::AllAccess
	};


private:

	// リジッドボディ
	float angular_drag_ { 1.0f };			// トルクで動く場合の空気抵抗

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

	// オブジェクトの抗力(空気抵抗)
	float drag_{ 1.0f };			
	// 固定フレームレート時に移動を保管する
	RigidbodyInterpolation interpolation_;
	// 質量
	float mass_{ 1.0f };
	// オブジェクトがスリープ状態に入る閾値
	float sleep_threshold_{ 0.005f };
	// 重力の影響を受けるか
	bool use_gravity_{ false };
	// 速度ベクトル
	Vector3 velocity_{ 0.0f,0.0f ,0.0f };		
	//ワールド座標上での質量の中心位置
	Vector3 world_center_of_mass_;





};

