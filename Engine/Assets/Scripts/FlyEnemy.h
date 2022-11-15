#pragma once
#include "Assets/Scripts/EnemyBase.h"

class FlyEnemy :
    public EnemyBase
{

public:
	FlyEnemy();

	void OnCollisionEnter(Collision &collision) override;
	void OnTriggerEnter(Collider &other) override;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;

	bool is_invincible_{ false };
	const float k_invincible_time_{ 0.5f };
	float invincible_timer_{ 0.0f };
};

