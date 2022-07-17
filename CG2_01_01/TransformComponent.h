#pragma once
#include <DirectXMath.h>
#include "Component.h"
class TransformComponent :
	public Component
{
public:
	TransformComponent(XMFLOAT3 position = {0,0,0}, XMFLOAT3 rotate = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	void Infomation() override;

	void VirtualUpdate() override;

private:
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;

	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

};

