#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>
#include <array>
#include <map>

class TextureManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	struct Texture
	{
		ComPtr<ID3D12Resource> resource;
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle_SRV;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle_SRV;
		std::wstring filename;
		uint32_t handle;
	};

	static TextureManager *GetInstance();

	static uint32_t LoadTexture(ID3D12Device *dev, const wchar_t *filename);
	static uint32_t MakeTexture(ID3D12Device *dev, DWORD color = 0xffffffff, DirectX::XMFLOAT2 size = { 8,8 });
	
	// デスクリプタの数
	static const size_t k_max_descriptor_{ 512 };


	/// <summary>
	/// デスクリプタを生成
	/// </summary>
	void CreateDescriptorHeap(ID3D12Device *dev);

	/// <summary>
	/// デスクリプタセット
	/// </summary>
	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList *cmd_list,
		UINT root_param_index,
		uint32_t texture_handle
	);

	const D3D12_RESOURCE_DESC GetResoureDesc(uint32_t texture_handle);

	const Texture *GetTexture(const std::wstring &filename);
private:
	
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;


	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="filename">ファイル名</param>
	/// <param name="shader_resource_view"></param>
	/// <returns></returns>
	uint32_t LoadTextureFromFile(ID3D12Device *dev, const wchar_t *filename);

	/// <summary>
	/// 単色ダミーテクスチャ生成
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="color">色 AABBGGRR</param>
	/// <param name="dimension"></param>
	/// <returns></returns>
	uint32_t MakeTextureFromColor(ID3D12Device *dev, DWORD color = 0xffffffff, DirectX::XMFLOAT2 size = { 8,8 });



	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	UINT descriptor_size_{0};
	// 次に使うインデックス
	uint32_t next_index_{0u};
	// テクスチャコンテナ
	std::array<Texture, k_max_descriptor_> textures_;

	std::map<std::string, std::pair<int, Texture>> test_;

};

