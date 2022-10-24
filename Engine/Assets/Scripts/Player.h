#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

enum class PlayerState
{
	// �ʏ펞
	IDOLE,

	// ���s
	WALK_FRONT,
	WALK_RIGHT,
	WALK_LEFT,
	WALK_BACK,
	
	// �_�b�V��
	DASH_FRONT,
	DASH_RIGHT,
	DASH_LEFT,
	DASH_BACK,

	// �W�����v
	JUMP,

	// �ً}���
	DODGE_FRONT,
	DODGE_RIGHT,
	DODGE_LEFT,
	DODGE_BACK,

	// �U��
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
	// �A�j���[�V�����X�e�[�g
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

    // ���W
    Rigidbody *regidbody_;
    Fbx *game_object{ nullptr };

	// ���͗�
	float input_horizontal_{ 0.0f };	// ��
	float input_vertical_{ 0.0f };		// �c

	int hp_ = 10;

	bool is_dash_ = false;
};

