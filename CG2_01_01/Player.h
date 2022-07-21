#pragma once
#include "Component.h"
#include "Vector3.h"
#include "TransformComponent.h"
#include "Fbx.h"

class Player :
    public Component
{
public:
    Player();

	/// <summary>
	/// ������
	/// </summary>
	void ComponentInitialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void ComponentUpdate() override;

	void ComponentLustUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void ComponentDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void ComponentFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;

private:
	Transform *transform_;
	void Move();

	// �A�j���[�V�����X�e�[�g
	enum class AnimationState {
		NONE = -1,
		DETH,
		IDLE,
		IDLE_LIFRE,
		PULL_RIFLE,
		PUT_RIFLE,
		WALK_BACK,
		RIFLE_WALK_BACK,
		WALK,
		RIFLE_WALK,
		WALK_RIGHT,
		WALK_LEFT,
		RIFLE_WALK_LEFT,
		RIFLE_WALK_RIGHT,
		MAX
	};
	AnimationState state = AnimationState::IDLE;
	bool isRifle = false;
	bool isDead = false;

	Fbx *game_object;
};

