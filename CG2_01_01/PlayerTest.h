#pragma once
#include "Component/ScriptComponent.h"
#include "Object/Component/Transform.h"

#include "3d/Fbx/Fbx.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"

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
	void FixedUpdate() override;
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
	std::weak_ptr<Rigidbody> regidbody_;

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
	};
	AnimationState state = AnimationState::T_POSE;
	bool isRifle{ false };
	bool isDead{ false };

	Fbx *game_object{ nullptr };

};

