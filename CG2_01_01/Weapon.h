#pragma once

#include "Component/ScriptComponent.h"
class Weapon
	: public ScriptComponent
{
public:
	Weapon();

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;

	void OnCollisionEnter() override;

private:
	DirectX::XMFLOAT4X4 *hand_bone_;
};

