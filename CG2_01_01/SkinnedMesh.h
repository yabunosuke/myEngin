#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>

struct Scene {
	// ノード
	struct Node {
		// ノードID
		uint64_t uniqueID = 0;
		// ノード名
		std::string name;
		// ノードタイプ
		FbxNodeAttribute::EType atribute = FbxNodeAttribute::EType::eUnknown;
		// 親のID
		int64_t pearentIndex = -1;
	};

	// ノードコンテナ
	std::vector<Node> nodes;

	/// <summary>
	/// ノードIDの検索
	/// </summary>
	/// <param name="uniqueID"></param>
	/// <returns>index</returns>
	int64_t indexof(uint64_t uniqueID) const {
		int64_t index = 0;
		for (const Node &node : nodes) {
			//IDが合致したらindexを返す
			if (node.uniqueID == uniqueID) {
				return index;
			}
			index++;
		}
		return -1;
	}
};

class SkinnedMesh
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;


public:	// サブクラス
	// 頂点データ構造体
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// 定数バッファ用
	struct Constants {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="fileName">リソースのファイル名</param>
	/// <param name="trianglate">三角形化の有無</param>
	SkinnedMesh(
		ID3D12Device *dev,
		const char *fileName,
		bool trianglate = false);
	virtual ~SkinnedMesh() = default;

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;

protected:
	// シーンビュー
	Scene sceneView;
};

