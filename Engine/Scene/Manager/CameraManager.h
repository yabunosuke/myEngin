#pragma once
#include "../../Engine/Object/Component/Camera.h"

class CameraManager
{
public:
	static void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
	);


	Camera *main_camera_;
	//オブジェクト
	std::vector<Camera *> cameras_;
};

