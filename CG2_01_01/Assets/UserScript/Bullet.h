#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Rigidbody.h"

class Bullet :
    public ScriptComponent
{
public:
	Bullet(Vector3 start_pos);
private:


	Transform *transform_;
	Rigidbody *regidbody_;
};
