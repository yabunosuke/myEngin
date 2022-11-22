#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "Object/Component/Rigidbody.h"

class Enemy :
	public MonoBehaviour
{
public:
	Enemy();

	void OnTriggerEnter(Collider &other) override;

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
	Rigidbody *regidbody_;
	Fbx *model_data_{ nullptr };

	float destroy_timer_test_{ 0.0f };//仮
	int hp_ = 10;
};

