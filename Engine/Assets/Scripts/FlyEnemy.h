#pragma once
#include "Assets/Scripts/EnemyBase.h"

class FlyEnemy :
    public EnemyBase
{

public:
	FlyEnemy();

	void OnCollisionEnter(Collision &collision) override;
	void OnTriggerEnter(Collision &collision) override;
	void OnTriggerStay(Collision &collision) override;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;
};

