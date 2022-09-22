#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <memory>
#include "FbxResource.h"

class Fbx
{
private:	//エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:

	// ノード
	struct Node	{
		const char *name;
		Node *parent;
		DirectX::XMFLOAT3	scale;				// 拡大率
		DirectX::XMFLOAT4	rotate;				// 回転
		DirectX::XMFLOAT3	translate;			// 移動
		DirectX::XMFLOAT4X4	local_transform;	// ローカル
		DirectX::XMFLOAT4X4	world_transform;	// ワールド

		std::vector<Node *>	children;			// 子のノード
	};


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="file_path">使いたいモデルのパス</param>
	Fbx(ID3D12Device *dev, const char *file_path);

	/// <summary>
	/// トランスフォーム更新
	/// </summary>
	/// <param name="transform"></param>
	void UpdateTransform(const XMFLOAT4X4 &transform);

	/// <summary>
	/// 描画コマンド発行
	/// </summary>
	/// <param name="dev"></param>
	void Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// ノードの検索
	/// </summary>
	/// <param name="name">検索したいノード名</param>
	/// <returns>ノード</returns>
	Node *FindNode(const char *name);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const FbxResource *GetResource() const { return resource_.get(); }

	void UpdateAnimation(float elapsed_time = 0.016f);
	void PlayAnimation(uint64_t index, bool loop = true, bool reset = false, float blend_seconds = 1.0f);
	float GetCurrentAnimationSeconds() const { return current_animation_seconds_; }

	// アニメーション中か判定
	bool IsPlayAnimation() const;

	// 色を設定
	void SetColor(XMFLOAT4 color) { color_ = color; }

private:
	// リソース
	std::shared_ptr<FbxResource>	resource_;	// モデルデータ
	std::vector<Node>				nodes_;		// コピーしたノード

	// アニメーション管理
	int		current_animation_index_ = -1;		// アニメーション番号
	float	current_animation_seconds_ = 0.0f;	// 現在時間
	bool	animation_loop_flag_ = false;		// ループフラグ
	bool	animation_end_flag_ = false;		// アニメーション終了フラグ
	float	animation_blend_time_ = 0.0f;		// 補完にかける秒数
	float	animation_blend_seconds_ = 0.0f;	// 補完の現在時間


	XMFLOAT4 color_ = { 1,1,1,1 };	//カラー
	
};

