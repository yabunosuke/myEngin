#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

class Player :
    public MonoBehaviour
{
public:
    Player();

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
    std::weak_ptr<Rigidbody> regidbody_;
    Fbx *game_object{ nullptr };

	int hp_ = 10;
};

