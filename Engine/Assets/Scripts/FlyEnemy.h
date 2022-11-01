#pragma once
#include "Assets/Scripts/EnemyBase.h"

class FlyEnemy :
    public EnemyBase
{

public:
	FlyEnemy();

	void OnCollisionEnter(Collision &collision) override;
	void OnTriggerEnter(Collider &other) override;
	void OnTriggerStay(Collider &other) override;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;
};

