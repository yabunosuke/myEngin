#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

enum class PlayerState
{
	// 通常時
	IDOLE,

	// 歩行
	WALK_FRONT,
	WALK_RIGHT,
	WALK_LEFT,
	WALK_BACK,
	
	// ダッシュ
	DASH_FRONT,
	DASH_RIGHT,
	DASH_LEFT,
	DASH_BACK,

	// ジャンプ
	JUMP,

	// 緊急回避
	DODGE_FRONT,
	DODGE_RIGHT,
	DODGE_LEFT,
	DODGE_BACK,

	// 攻撃
	MELEE_ATTACK_1,
	MELEE_ATTACK_2,
	MELEE_ATTACK_3,

};

class PlayerController :
    public MonoBehaviour
{
public:
    PlayerController();

	void OnCollisionEnter(Collision& collision) override;

	void Start() override;
    void FixedUpdate() override;
    void Update() override;

private:
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
	}state = AnimationState::T_POSE;

    // 座標
    Rigidbody *regidbody_;
    Fbx *game_object{ nullptr };

	// 入力量
	float input_horizontal_{ 0.0f };	// 横
	float input_vertical_{ 0.0f };		// 縦

	int hp_ = 10;

	bool is_dash_ = false;
};

