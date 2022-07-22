#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>




class PrimitiveRenderer
{
private:	//エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

	// シングルトンパターン
	PrimitiveRenderer() = default;
	~PrimitiveRenderer() = default;
	PrimitiveRenderer(const PrimitiveRenderer & r) = default;
	PrimitiveRenderer &operator= (const PrimitiveRenderer & r) = default;

public:
	struct ConstantBuffer
	{
		XMFLOAT4X4 world;	// ワールド行列
		XMFLOAT4 color;		// カラー
	};

	struct Line
	{
		DirectX::XMFLOAT3 start_positon;
		DirectX::XMFLOAT3 end_positon;
	};
	struct Box
	{
		DirectX::XMFLOAT3 positon;

	};

	enum class PrimitiveType
	{
		LINE,
		BOX,
		SPHERE,
		CAPSULE,

		MAX,

	};

	static PrimitiveRenderer &GetInstance();

	// 全プリミティブ生成
	void Initialize(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list);


	void DrawLine(ComPtr<ID3D12GraphicsCommandList> cmd_list,Line line);
	// ラインプリミティブ生成
private:
	void CreateLine(ComPtr<ID3D12Device> dev);


	// コマンドリスト

	ComPtr<ID3D12Resource> vertex_buffer_[static_cast<int>(PrimitiveType::MAX)];			// 頂点バッファ
	ComPtr<ID3D12Resource> constant_buffer_;		// 定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView[static_cast<int>(PrimitiveType::MAX)] = {};			// 頂点ビュー
};

