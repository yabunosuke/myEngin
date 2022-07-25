#include "AbstractScene.h"
#include  "DirectXCommon.h"
AbstractScene::AbstractScene(IoChangedListener *impl,std::string sceneName)
	:implSceneChanged(impl),
	name(sceneName)
{
	post_effect_ = std::make_unique<PostEffect>();
	post_effect_->InitializePostEffect(DirectXCommon::dev);

	muluti_render_target_ = std::make_unique<MulutiRenderTarget>();
	muluti_render_target_->InitializeMulutiRenderTarget(DirectXCommon::dev);
}

void AbstractScene::Update()
{
	// オブジェクト更新
	game_object_manager_.Update();

	// 当たり判定チェック
	collision_manager_.CheckBroadCollisions(game_object_manager_.gameObjects);

	// 最終更新
	game_object_manager_.LastUpdate();
}

void AbstractScene::PreDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->PreDrawScene(dev,cmd_list);

}

void AbstractScene::PostDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->PostDraw(cmd_list);
}

void AbstractScene::PreDrawPostEffect(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->PreDraw(cmd_list);
}

void AbstractScene::DrawMulutiRenderTarget(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->DrawRenderTarget(cmd_list);
}

void AbstractScene::PostDrawPoseEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->PostDraw(cmd_list);
}

void AbstractScene::DrawPostEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->DrawPostEffect(cmd_list);
}

