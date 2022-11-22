#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"

class FlyEnemyEye:
	public MonoBehaviour
{
public:
	FlyEnemyEye();
	void OnTriggerStay(Collider &other) override;
	
};

