#include "AbstractScene.h"
#include  "DirectXCommon.h"
#include "PipelineManager.h"
#include "3d/Render.h"
#include "Object/Component/Light.h"

#include "../Manager/CameraManager.h"
#include "FileManager.h"





AbstractScene::AbstractScene(IoChangedListener *impl,std::string scene_name)
	:implSceneChanged(impl),
	name(scene_name)
{
	post_effect_ = std::make_unique<PostEffect>();
	post_effect_->InitializePostEffect(DirectXCommon::dev);

	muluti_render_target_ = std::make_unique<MulutiRenderTarget>();
	muluti_render_target_->InitializeMulutiRenderTarget(DirectXCommon::dev);

	light_manager_ = std::make_unique<LightManager>();
	game_object_manager_ = std::make_unique<GameObjectManager>();
	camera_manager_ = std::make_unique<CameraManager>();
	//camera_manager_ = std::make_shared<CameraManager>();

}

AbstractScene::~AbstractScene()
{
	for (auto &object : game_object_manager_->game_objects_)
	{
		Object::Destroy(object.lock().get());
	}
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

void AbstractScene::LoadSceneData(const std::string &filename)
{
	//std::ifstream ifs(filename.c_str());
	//cereal::BinaryInputArchive deserialization(ifs);
	//deserialization(*this);
}

void AbstractScene::SeveSceneData()
{

	//test_val_ = 1;


	//static OPENFILENAME open_file;
	////初期フルパス
	//static TCHAR first_file_path[MAX_PATH];
	////保存先フルパス
	//static TCHAR save_file_path[MAX_PATH];
	//// 拡張子
	//static const std::wstring ext{ L"scene" };
	//static const std::wstring filter{ L"SceneData(*.scene)\0*.scene\0" };
	//static const std::wstring title{ L"Save the scene" };

	//FileManager::GetIns()->FileSave(
	//	open_file,
	//	first_file_path,
	//	save_file_path,
	//	ext,
	//	filter,
	//	title
	//);

	//if (GetSaveFileName(&open_file))
	//{
	//	// バイナリ書き出し
	//	std::ofstream ofs(save_file_path, std::ios::binary);
	//	cereal::BinaryOutputArchive serealization(ofs);
	//	serealization(*this);
	//}

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
		light_manager_.get()
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

void AbstractScene::DrawSkyBox(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	PipelineManager::GetInstance()->SetPipline(cmd_list, "SkyBox");
}

void AbstractScene::Finalize()
{
}
