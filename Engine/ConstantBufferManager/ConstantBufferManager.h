#pragma once
#include "ConstantBufferManager/ConstantBufferLapper.h"
#include "ConstantBufferManager/ConstantBuffer.h"
#include <wrl.h>
#include <vector>

enum class BufferName
{
	Test,
	Light,
	Camera,
	MAX,
};

class ConstantBufferManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// シングルトンパターン生成
	ConstantBufferManager(ComPtr<ID3D12Device> dev);

	// インスタンスの取得
	static ConstantBufferManager *GetInstance() { return instance; };

	// バッファにデータ転送
	template<class T>
	bool BufferTransfer(
		ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index, 
		BufferName buffername,
		T *buffer_data)
	{
		// 転送
		HRESULT result = S_OK;
		T *constMap = nullptr;
		result = constant_buffers_[static_cast<int>(buffername)].
			GetConstantBuffer()->Map(subresource, nullptr, (void **)&constMap);
		if (SUCCEEDED(result)) {
			memcpy(constMap, buffer_data, sizeof(*constMap));
			constant_buffers_[static_cast<int>(buffername)].
				GetConstantBuffer()->Unmap(subresource, nullptr);
		}
		// セット
		cmd_list->SetGraphicsRootConstantBufferView(rootparameta_index, constant_buffers_[static_cast<int>(buffername)].GetConstantBuffer()->GetGPUVirtualAddress());


		return true;
	}


	
private:

	// コンスタントバッファの生成と追加
	bool AddConstantBuffer(size_t size, BufferName buffername, ComPtr<ID3D12Device> dev);
	// コンスタントバッファーコンテナ
	std::vector<ConstantBufferLapper> constant_buffers_;
	// 自分のポインタ
	static ConstantBufferManager *instance;
};

