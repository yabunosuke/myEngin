#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

class Enemy :
	public MonoBehaviour
{
public:
	Enemy();

	void OnCollisionEnter(Collision &collision) override;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;

private:
	// アニメーションステート
	enum class AnimationState {
		NONE = -1,
		DEAD,
		DAMAGE,
		FIGHT_IDOLE,
		IDOLE,
		MAX
	}state = AnimationState::FIGHT_IDOLE;

	// 座標
	std::weak_ptr<Rigidbody> regidbody_;
	Fbx *game_object{ nullptr };

	int hp_ = 10;
};

