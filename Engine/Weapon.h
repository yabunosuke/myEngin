#pragma once

#include "Object/Component/Behaviour/MonoBehaviour.h"
class Weapon
	: public MonoBehaviour
{
public:
	Weapon();

	void Start() override;

private:
	DirectX::XMFLOAT4X4 *hand_bone_ = nullptr;
};

