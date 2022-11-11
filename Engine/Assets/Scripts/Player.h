#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

enum class PlayerState
{
	// 通常時
	IDOLE,

	// 歩行
	WALK,
	
	// ダッシュ
	DASH,

	// ジャンプ
	JUMP,
	JUMP_DROP,	//ジャンプ後の落下

	// 緊急回避
	DODGE,

	// 攻撃
	MELEE_ATTACK_1,
	MELEE_ATTACK_2,
	MELEE_ATTACK_3,


	MAX,
};

class PlayerController :
    public MonoBehaviour
{
public:
    PlayerController();

	void OnCollisionEnter(Collision& collision) override;
	void OnCollisionStay(Collision& collision) override;

	void Awake() override;
	void Start() override;
    void FixedUpdate() override;
    void Update() override;

private:
	yEngine::Property<PlayerState> playerState
	{
		&player_state_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		[&](PlayerState next_state)
		{
			player_state_ = next_state;
			(this->*state_update_[next_state])(false);
		}
	};
	PlayerState player_state_ = PlayerState::IDOLE;
	void Idole(bool is_fixed);
	void Walk(bool is_fixed);
	void Dash(bool is_fixed);
	void Jump(bool is_fixed);
	void JumpDrop(bool is_fixed);
	void Dodge(bool is_fixed);
	void MeleeAttack1(bool is_fixed);

	std::map<PlayerState,void(PlayerController::*)(bool)> state_update_;

	bool can_jump_{true};		// ジャンプ可能Flag
	bool is_dash_ {false};
	bool is_dodge_{ false };

	// アニメーションステート
	enum class AnimationState {
		NONE = -1,
		IDOLE,
		Jump,
		RUN_FRONT,
		SLASH,
		T_POSE,
		WALK_BACK,
		WALK_FRONT,
		WALK_LEFT,
		WALK_RIGHT,
		MAX
	}animation_state_ = AnimationState::T_POSE;

    // 座標
    Rigidbody *regidbody_;
    Fbx *model_data_{ nullptr };

	// 入力量
	float input_horizontal_{ 0.0f };	// 横
	float input_vertical_{ 0.0f };		// 縦

	int hp_ = 10;

};

