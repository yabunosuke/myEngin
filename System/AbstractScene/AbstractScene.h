#pragma once
#include <string>
#include "IoChangedListener.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "2d/PostEffect.h"
#include <wrl.h>

class AbstractScene
{
protected:

	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	virtual ~AbstractScene() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 一定間隔で更新
	/// </summary>
	//virtual void FixUpdate() = 0;
	/// <summary>
	/// 描画コマンド発行
	/// </summary>
	void PreDrawScene(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list) { post_effect_->PreDrawScene(cmd_list); }
	virtual void Draw() const = 0;
	void PostDrawScene(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list) { post_effect_->PostDrawScene(cmd_list); }

	virtual void DrawPostEffect();
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;


	// ゲッター
	GameObjectManager *GetObjectManager() { return &game_object_manager_; }	// オブジェクトマネージャー
	CollisionManager *GetCollisionManager() { return &collision_manager_; }			// 子リジョンマネージャー
	std::string GetName() { return name; }

protected:
	// オブジェクトマネージャー
	GameObjectManager game_object_manager_;
	// コリジョンマネージャー
	CollisionManager collision_manager_;
	// ポストエフェクト
	std::unique_ptr<PostEffect> post_effect_;

private:
	// シーン名
	std::string name;

};

