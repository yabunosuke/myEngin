#pragma once
#include <wrl.h>
#include <d3d12.h>

class ConstantBufferLapper
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// バッファの生成
	bool CreateConstantBuffer(size_t size, ComPtr<ID3D12Device> dev);
	// ゲッッター
	ID3D12Resource *GetConstantBuffer() const { return constant_buffer_.Get(); }

private:
	ComPtr<ID3D12Resource> constant_buffer_;

};

