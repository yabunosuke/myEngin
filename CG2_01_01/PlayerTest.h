#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Transform.h"

#include "3d/Fbx/Fbx.h"
#include "Component/Light.h"
#include "Component/Rigidbody.h"

class PlayerTest :
	public ScriptComponent
{
public:
	PlayerTest();
	
	void OnCollisionEnter() override;
private:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	void LustUpdate() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	// ���W
	Transform *transform_;
	Rigidbody *regidbody_;

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
	bool isRifle{ false };
	bool isDead{ false };

	Fbx *game_object{ nullptr };

};

