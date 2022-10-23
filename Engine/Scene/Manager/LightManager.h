#pragma once
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <d3d12.h>
#include "ConstantBufferManager/ConstantBufferManager.h"


class LightManager
{
public:

	// ライトをリストに追加
	void AddLight(LightDate *light);
	/// <summary>
	/// ライトをバッファに転送
	/// </summary>
	void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
		);

	std::vector<LightDate *> &GetLightList() { return light_list_; }
private:
	// ソート用関数
	static bool active(const LightDate *lhs, const LightDate *rhs);


	std::vector<LightDate *> light_list_;
};

