#pragma once
#include "Component/Component.h"
#include "3d/Fbx/Fbx.h"

class Object3dComponent :
    public Component
{
public:
	Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path);

	/// <summary>
	/// 初期化
	/// </summary>
	void ComponentInitialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void ComponentUpdate() override;
	/// <summary>
	/// 描画
	/// </summary>
	void ComponentDraw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void ComponentFinalize() override;

	/// <summary>
	/// 情報
	/// </summary>
	void Infomation() override;

	// オブジェクトの取得
	Fbx *GetObjectData() const { return object_fbx.get(); }
private:

	// FBXオブジェクト
	std::shared_ptr<Fbx> object_fbx;

	
	ComPtr<ID3D12Device> dev;					// デバイス
	ComPtr<ID3D12GraphicsCommandList> cmd_list;	// コマンドリスト

};

