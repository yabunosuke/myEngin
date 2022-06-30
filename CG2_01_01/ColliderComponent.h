#pragma once
#include "Component.h"
class ColliderComponent :
	public Component
{
public:
	ColliderComponent();
	void SetTestF(float f) { testF = f; }
	float GetTestF() { return testF; }
private:
	float testF = 1.0f;

};

