#include "TitleScene.h"
#include "DirectXCommon.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Easing.h"
#include "Math/Mathf.h"
//#include "Quaternion.h"

#include "Texture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"

// コンポーネント
#include "Object3dComponent.h"
#include "Component/ColliderComponent.h"
#include "Component/LightComponent.h"
#include "Component/Rigidbody.h"
#include "PlayerTest.h"

TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

	auto cube = game_object_manager_.CreateObject("cube");
	cube.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/cube.004.fbx");
	cube.lock().get()->AddComponent<ColliderComponent>(this);
	cube.lock().get()->AddComponent<LightComponent>();
	cube.lock().get()->AddComponent<Rigidbody>();

	//auto test = game_object_manager_.CreateObject("plantune");
	//test.lock().get()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/UNIT/plantune.fbx");
	////"Assets/3d/test/human.fbx");
	////"Assets/3d/Test/stage.fbx");
	//test.lock().get()->GetComponent<TransformComponent>()->SetScale({ 0.05f,0.05f ,0.05f });
	//test.lock().get()->AddComponent<ColliderComponent>(this, CollisionShapeType::SHAPE_OBB);

	float a = Mathf::Max(1.0f, 6.0f, 2.0f, 3.0f);
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

	if(KeyboardInput::GetIns()->GetKeyPress(DIK_UPARROW))
	{
		Camera::GetCam()->eye.z++;
	}
	if(KeyboardInput::GetIns()->GetKeyPress(DIK_DOWNARROW))
	{
		Camera::GetCam()->eye.z--;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_RIGHTARROW))
	{
		Camera::GetCam()->eye.x++;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_LEFTARROW))
	{
		Camera::GetCam()->eye.x--;
	}
	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();
 	
}

void TitleScene::Draw() const
{
	// 背景スプライト

	PrimitiveRenderer::Line line = {
		{0,0,0},
		{10,0,0}
	};

	//PrimitiveRenderer::GetInstance().DrawLine(DirectXCommon::cmdList,line);
	//PrimitiveRenderer::GetInstance().DrawBox(DirectXCommon::cmdList, box);
	game_object_manager_.Draw();


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
		if(i % 4 != 0 || i == 0)
		{
			ImGui::SameLine();

		}
	}

	//終了
	ImGui::End();
}

