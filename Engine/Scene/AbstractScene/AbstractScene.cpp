#include "AbstractScene.h"
#include  "DirectXCommon.h"
#include "PipelineManager.h"
#include "3d/DrawFbx.h"
#include "Object/Component/Light.h"

#include "../Manager/CameraManager.h"

AbstractScene::AbstractScene(IoChangedListener *impl,std::string sceneName)
	:implSceneChanged(impl),
	name(sceneName)
{
	post_effect_ = std::make_unique<PostEffect>();
	post_effect_->InitializePostEffect(DirectXCommon::dev);

	muluti_render_target_ = std::make_unique<MulutiRenderTarget>();
	muluti_render_target_->InitializeMulutiRenderTarget(DirectXCommon::dev);

	light_manager_ = std::make_shared<LightManager>();
	game_object_manager_ = std::make_unique<GameObjectManager>();
	camera_manager_ = std::make_unique<CameraManager>();
	//camera_manager_ = std::make_shared<CameraManager>();


}

AbstractScene::~AbstractScene()
{
	
}


void AbstractScene::Initialize()
{
	game_object_manager_->Initialize();
}

void AbstractScene::FixedUpdate()
{
	game_object_manager_->FixedUpdate();


	// 当たり判定チェック
	//collision_manager_.CheckBroadCollisions(game_object_manager_.game_objects_);
}

void AbstractScene::Update()
{
	// オブジェクト更新
	game_object_manager_->Update();


	// 最終更新
	game_object_manager_->LastUpdate();


}


void AbstractScene::PreDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->PreDrawScene(dev,cmd_list);

}

void AbstractScene::Draw() const
{
	// マネージャー描画）
	game_object_manager_->Draw();

	//カメラマネージャをセット
	PipelineManager::GetInstance()->SetPipline(DirectXCommon::cmdList, "GBuffer");
	CameraManager::BufferTransfer(DirectXCommon::cmdList, 0, 0);
	//camera_manager_->BufferTransfer(DirectXCommon::cmdList, 0, 0);
	// モデルを描画
	Renderer::GetIns()->DrawDeferred(DirectXCommon::dev,DirectXCommon::cmdList);

	//ウィンドウ名定義
	ImGui::Begin("PostEffectShader");
	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);
	for (int i = 0; i < _countof(PipelineManager::GetInstance()->posteffect_shader_list_); ++i)
	{
		if (ImGui::Button(PipelineManager::GetInstance()->posteffect_shader_list_[i].c_str())) {
			post_effect_->shader_name_ = PipelineManager::GetInstance()->posteffect_shader_list_[i];
		}
		if (i % 4 != 0 || i == 0)
		{
			ImGui::SameLine();

		}
	}

	//終了
	ImGui::End();
}

void AbstractScene::PostDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->PostDraw(cmd_list);
}

void AbstractScene::PreDrawPostEffect(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->PreDraw(cmd_list);
}

void AbstractScene::DrawMulutiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	muluti_render_target_->DrawRenderTarget(
		cmd_list,
		dev,
		light_manager_
		);
}

void AbstractScene::PostDrawPoseEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->PostDraw(cmd_list);
}

void AbstractScene::DrawPostEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	post_effect_->DrawPostEffect(cmd_list);
}

void AbstractScene::Finalize()
{

}