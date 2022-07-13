#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <string>

class Texture
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// デスクリプタを生成
	/// </summary>
	static void CreateDescriptorHeap(ID3D12Device *dev);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="filename">ファイル名</param>
	/// <param name="shader_resource_view"></param>
	/// <returns></returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE LoadTextureFromFile(ID3D12Device *dev,const wchar_t *filename);

	/// <summary>
	/// 単色ダミーテクスチャ生成
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="color">色 AABBGGRR</param>
	/// <param name="dimension"></param>
	/// <returns></returns>
	static HRESULT MakeDummmyTexture(ID3D12Device *dev, DWORD color = 0xffff00ff, UINT dimension = 8);

public:
	// テクスチャの最大数
	static const int srv_count_ = 512;
	// テクスチャバッファ
	static std::vector<ComPtr<ID3D12Resource>> texture_buffer_;
	// シェーダーリソース
	static ComPtr<ID3D12DescriptorHeap> descriptor_heap_;

	
};

