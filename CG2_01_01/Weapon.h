#pragma once

#include "Component/ScriptComponent.h"
class Weapon
	: public ScriptComponent
{
public:
	Weapon();

	void Update() override;
	void FixedUpdate() override;

private:
	DirectX::XMFLOAT4X4 *bone_;
};

