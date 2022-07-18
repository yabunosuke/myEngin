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

	XMFLOAT3 GetPositon()	const	{ return position_; }
	XMFLOAT3 GetRotate()	const	{ return rotate_; }
	XMFLOAT3 GetScale()		const	{ return scale_; }

	void SetPositon(XMFLOAT3 pos)	{ position_ = pos; }
	void SetRotate(XMFLOAT3 rot)	{ rotate_ = rot; }
	void SetScale(XMFLOAT3 sca)		{ scale_ = sca; }


	XMFLOAT4X4 GetTransform() const;

private:
	XMFLOAT3 position_;
	XMFLOAT3 rotate_;
	XMFLOAT3 scale_;

	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

};

