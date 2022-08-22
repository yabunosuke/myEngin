#pragma once
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <d3d12.h>
#include "ConstantBufferManager/ConstantBufferManager.h"


class LightManager
{
public:

	// ライトコンポーネントをリストに追加
	void AddLight(std::weak_ptr<LightDate> light);
	
	/// <summary>
	/// ライトをバッファに転送
	/// </summary>
	void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
		);

private:
	// ソート用関数
	static bool active(const std::weak_ptr<LightDate> &lhs, const std::weak_ptr<LightDate> &rhs);


	std::vector<std::weak_ptr<LightDate>> light_list_;
};

