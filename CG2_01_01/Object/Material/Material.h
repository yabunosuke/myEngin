#pragma once

#include <DirectXMath.h>
#include "Object/Object.h"

class Material :
    public Object
{

private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	// ID��UINT64���g�p����
	using NodeId = uint64_t;

public:


private:

	XMFLOAT4 color_;

};

