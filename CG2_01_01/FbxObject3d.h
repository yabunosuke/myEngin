#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "SkinnedMesh.h"
class FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FbxObject3d(SkinnedMesh *skinnedMesh);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipeline(ID3D12Device *dev);
	// 描画
	void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList) const;
private:
	//メッシュデータ
	SkinnedMesh *mesh;
	
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;

};

