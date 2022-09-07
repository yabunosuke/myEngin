#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Component/Manager/LightManager.h"

// サブクラス
enum class RenderName
{
	color,	//	0.色情報
	normal,	//	1.法線情報
	metal_roughness,		//	2.メタリック・ラフネス情報
	depth,	//	3.深度情報
	world_position,	//	4.ワールド情報
	id,		//	5.処理分割用のID
	MAX
};

class MulutiRenderTarget
{

private:	//エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:
	

	MulutiRenderTarget();

	void InitializeMulutiRenderTarget(ComPtr<ID3D12Device> dev);
	void PreDrawScene(ComPtr<ID3D12Device> dev,ComPtr<ID3D12GraphicsCommandList> cmd_list);
	void DrawRenderTarget(
		ComPtr<ID3D12GraphicsCommandList> cmd_list,
		ComPtr<ID3D12Device> dev,
		const std::weak_ptr<LightManager> light_manager
	);
	void PostDraw(ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// マルチレンダーの数
	/// </summary>
	static const int buffer_count_ = 6;


private:
	// サブクラス
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z座標
		XMFLOAT2 uv;	//uv座標
	};

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertex_buffer_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_{};
	// 深度バッファ
	ComPtr<ID3D12Resource> depth_buffer_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texture_buffer_[buffer_count_];
	DXGI_FORMAT format_list_[buffer_count_] = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
	};
	// SRVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_SRV_;
	// RTVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
	// DSVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	// 画面クリアする色
	const float clear_color_[4] = { 0.0f,0.0f,1.0f,1.0f };


};

