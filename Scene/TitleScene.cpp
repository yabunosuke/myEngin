#include "TitleScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

#include "Object/GameObject/GameObject.h"

// コンポーネント
#include "Object3dComponent.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Camera.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"
#include <Assets/Scripts/TitleManager.h>



TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

}

void TitleScene::Initialize()
{


	// ライト
	auto directional_light_ = GameObject::CreateObject("Directional Light");
	directional_light_->AddComponent<Light>(LightType::Directional);
	directional_light_->transform_->localQuaternion = Quaternion::Euler(32.0f * Mathf::deg_to_rad, -150.0f * Mathf::deg_to_rad, 180.0f * Mathf::deg_to_rad);

	// カメラ
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,0,-10 };
	camera->transform_->localQuaternion = Quaternion::Euler(0, 0, 0);

	auto title_manager = GameObject::CreateObject("TitleManager");
	title_manager->transform_->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);

	auto titile_text = GameObject::CreateObject("TitleText");
	titile_text->SetParent(title_manager);
	titile_text->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/title.fbx");

	auto start_text = GameObject::CreateObject("StartText");
	start_text->SetParent(title_manager);
	start_text->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/Start.fbx");

	auto end_text = GameObject::CreateObject("EndText");
	end_text->SetParent(title_manager);
	end_text->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/End.fbx");

	title_manager->AddComponent<TitleManager>(titile_text, start_text, end_text, implSceneChanged);


	auto player = GameObject::CreateObject("Knight");
	player->transform_->position = { -5.0f,-6.6f,-2.8f };
	player->transform_->localQuaternion = Quaternion::Euler(0, 115.0f * Mathf::deg_to_rad, 0);
	player->transform_->scale = {3,3,3};
	auto player_model =  player->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/RPG Characters - Nov 2020/Humanoid Rig Versions/FBX/Warrior.fbx");
	player_model->GetObjectData()->PlayAnimation(9);
}

void TitleScene::Draw() const
{
	AbstractScene::Draw();
}


