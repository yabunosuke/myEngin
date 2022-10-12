#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

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
	float input_horizontal_{ 0.0f };
	float input_vertical_{ 0.0f };

	int hp_ = 10;
};

