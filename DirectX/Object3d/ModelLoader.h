#pragma once

#include "fbxsdk.h"
#include "Model.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class ModelLoader
{
private: // エイリアス
	// std::を省略
	using string = std::string;
public: // 定数
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	// シングルトンインスタンスの取得
	static ModelLoader *GetInstance();
	//FBXの行列をXMMatrixに変換
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src);

public:
	// 初期化
	void Initialize(ID3D12Device *device);
	// 後始末
	void Finalize();
	// ファイルからFBXモデル読込
	std::unique_ptr<Model> LoadModelFromFile(const string &modelName);

private:
	// D3D12デバイス
	ID3D12Device *device = nullptr;
	// FBXマネージャ
	FbxManager *fbxManager = nullptr;
	// FBXインポータ
	FbxImporter *fbxImporter = nullptr;
	// コントロールポイントのデータ
	std::vector<std::vector<int>> controlPointsData;

private:
	// privateなコンストラクタ（シングルトンパターン）
	ModelLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~ModelLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	ModelLoader(const ModelLoader &obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const ModelLoader &obj) = delete;
	// 再帰的にノード構成を解析
	void ParseNodeRecursive(Model *model, FbxNode *fbxNode, Node *parent = nullptr);
	// メッシュ読み取り
	void ParseMesh(Model *model, FbxNode *fbxNode);
	// 面情報読み取り
	void ParseMeshFaces(Model *model, FbxMesh *fbxMesh);
	// マテリアル読み取り
	void ParseMaterial(Model *model, FbxNode *fbxNode);
	// テクスチャ読み込み
	void LoadTexture(Model *model, const std::string &fullpath);
	//スキニング情報の読み取り
	void ParseSkin(Model *model, FbxMesh *fbxMesh);
	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string &path);
};