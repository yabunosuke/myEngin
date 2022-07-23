#include "TitleScene.h"
#include "DirectXCommon.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Easing.h"
#include "yMath.h"
//#include "Quaternion.h"

#include "Texture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"

// コンポーネント
#include "Object3dComponent.h"
#include "ColliderComponent.h"
#include "PlayerTest.h"

TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	PipelineManager::GetInstance()->CreateSpriutePipline(DirectXCommon::dev.Get(), "Sprite");
	PipelineManager::GetInstance()->CreatePipline(DirectXCommon::dev.Get(), "Lambert");

	// ポストエフェクトパイプライン
	for(int i = 0; i < _countof(posteffect_shader_list_);++i)
	{
		PipelineManager::GetInstance()->CreatePostEffectPipline(DirectXCommon::dev.Get(), posteffect_shader_list_[i]);

	}


	/*auto cube = game_object_manager_.CreateObject("cube");
	cube.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/cube.004.fbx");
	cube.lock().get()->AddComponent<ColliderComponent>(this);
	*/
	auto test = game_object_manager_.CreateObject("human");
	test.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		//"Assets/3d/UNIT/cube.000.fbx");
		"Assets/3d/test/human.fbx");
	//"Assets/3d/Test/stage.fbx");



	auto player = game_object_manager_.CreateObject("human");
	player.lock().get()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(), 
			//"Assets/3d/UNIT/cube.000.fbx");
			"Assets/3d/test/human.fbx");
			//"Assets/3d/Test/stage.fbx");

	player.lock().get()->AddComponent<ColliderComponent>(this);
	player.lock().get()->AddComponent<PlayerTest>();


	//画像読み込みテスト
	int test_tex = Texture::LoadTextureFromFile(DirectXCommon::dev.Get(), L"Assets/2d/circle.png");
	test_sprite = Sprite::Create(DirectXCommon::dev, test_tex);
}

void TitleScene::Initialize()
{
	cam_ = new Camera({ 0,50,-50 });
	cam_->eye = { 0,50,0 };
	Camera::SetCam(cam_);

	game_object_manager_.Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//test[0]->UpdateAnimation();

	//ゲームオブジェクト全てをアップデート
	game_object_manager_.Update();

	//当たり判定チェック
	collision_manager_.CheckBroadCollisions(game_object_manager_.gameObjects);

	game_object_manager_.LastUpdate();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();
 	
}

void TitleScene::Draw() const
{
	// 背景スプライト

	PrimitiveRenderer::Line line_1 = {
		{-10,0,0},
		{10,0,0}
	};
	PrimitiveRenderer::Line line_2 = {
		{30,-10,0},
		{30,10,0}
	};

	PrimitiveRenderer::GetInstance().DrawLine(DirectXCommon::cmdList,line_1);
	PrimitiveRenderer::GetInstance().DrawLine(DirectXCommon::cmdList,line_2);
	game_object_manager_.Draw();
	//test_sprite->Draw(DirectXCommon::dev, DirectXCommon::cmdList, "Sprite");

	//ウィンドウ名定義
	ImGui::Begin("PostEffectShader");
	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);
	for (int i = 0; i < _countof(posteffect_shader_list_); ++i)
	{
		if (ImGui::Button(posteffect_shader_list_[i].c_str())) {
			post_effect_->shader_name_ = posteffect_shader_list_[i];
		}
		if(i % 4 != 0 || i == 0)
		{
			ImGui::SameLine();

		}
	}

	//終了
	ImGui::End();

	////ウィンドウ名定義
	//ImGui::Begin("ObjectShader");
	//ImGui::SetWindowSize(
	//	ImVec2(400, 500),
	//	ImGuiCond_::ImGuiCond_FirstUseEver
	//);
	//for (int i = 0; i < _countof(object_shader_list_); ++i)
	//{
	//	if (ImGui::Button(object_shader_list_[i].c_str())) {
	//		//post_effect_->shader_name_ = posteffect_shader_list_[i];
	//	}
	//	if (i % 4 != 0 || i == 0)
	//	{
	//		ImGui::SameLine();

	//	}
	//}

	////終了
	//ImGui::End();
}

