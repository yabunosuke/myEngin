#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"
#include "Assets/Scripts/CameraController/PlayerCameraController.h"

enum class PlayerState
{
	// �ʏ펞
	IDOLE,

	// ���s
	WALK,
	
	// �_�b�V��
	DASH,

	// �W�����v
	JUMP,
	JUMP_DROP,	//�W�����v��̗���

	// �ً}���
	DODGE,

	// �U��
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

	bool can_jump_{true};		// �W�����v�\Flag
	bool is_dash_ {false};
	bool is_dodge_{ false };

	// �A�j���[�V�����X�e�[�g
	enum class AnimationState {
		NONE = -1,
		Attack1,
		Attack2,
		Damage,
		Deth,
		Idol,
		Jump,
		Roll,
		Run,
		Tpose,
		Walk,
		MAX
	}animation_state_ = AnimationState::Tpose;

    // ���W
    Rigidbody *rigidbody_;
    Fbx *model_data_{ nullptr };

	// ���͗�
	Vector3 input_stick_r_{0,0,0};
	Vector3 input_stick_l_{0,0,0};

	// �ړ����x
	float move_speed{ 2.0f };

	int hp_ = 10;

	// �J��������p
	PlayerCameraController *camera_controller_{ nullptr };
};

