#pragma once
#include <string>
#include "IoChangedListener.h"
#include "Scene/Manager/GameObjectManager.h"
#include "2d/PostEffect.h"
#include "MultiRenderTarget/MulutiRenderTarget.h"


#include "Scene/Manager/LightManager.h"
#include <wrl.h>

#include "Scene/Manager/CameraManager.h"
class AbstractScene
{
protected:

	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	~AbstractScene();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 一定間隔で更新
	/// </summary>
	virtual void FixedUpdate();
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// マルチレンダーターゲット描画前処理
	/// </summary>
	void PreDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// 描画コマンド発行
	/// </summary>
	virtual void Draw() const;

	/// <summary>
	/// マルチレンダーターゲット描画後処理
	/// </summary>
	void PostDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// ポストエフェクト描画前準備
	/// </summary>
	void PreDrawPostEffect(Microsoft::WRL::ComPtr<ID3D12Device>dev,Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);
	
	/// <summary>
	/// マルチレンダーターゲット描画
	/// </summary>
	virtual void DrawMulutiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	void PostDrawPoseEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	virtual void DrawPostEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);


	void DrawSkyBox(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();


	// ゲッター
	GameObjectManager *GetObjectManager() { return game_object_manager_.get(); }	// オブジェクトマネージャー
	CameraManager *GetCameraManager() { return camera_manager_.get(); }				// カメラマネージャー
	LightManager *GetLightManager() { return light_manager_.get(); }				// ライトマネージャー
	//CollisionManager *GetCollisionManager() { return &collision_manager_; }			// コリジョンマネージャー
	std::string GetName() { return name; }
	std::unique_ptr<MulutiRenderTarget> *GetMulutiRenderTarget() { return &muluti_render_target_; }

protected:	
	// オブジェクトマネージャー
	std::unique_ptr<GameObjectManager> game_object_manager_;
	std::unique_ptr<CameraManager> camera_manager_;
	// コリジョンマネージャー
	//CollisionManager collision_manager_;
	// ライトマネージャー
	std::unique_ptr<LightManager> light_manager_;
	// ポストエフェクト
	std::unique_ptr<PostEffect> post_effect_;
	// マルチレンダーターゲット
	std::unique_ptr<MulutiRenderTarget> muluti_render_target_;

private:
	// シーン名
	std::string name;

};

