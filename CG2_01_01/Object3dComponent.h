#pragma once
#include "Component.h"
#include "Fbx.h"

class Object3dComponent :
    public Component
{
public:
	Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path);

	/// <summary>
	/// 初期化
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// 描画
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// 情報
	/// </summary>
	void Infomation() override;

private:

	// FBXオブジェクト
	std::shared_ptr<Fbx> object_fbx;

	
	ComPtr<ID3D12Device> dev;					// デバイス
	ComPtr<ID3D12GraphicsCommandList> cmd_list;	// コマンドリスト

};

