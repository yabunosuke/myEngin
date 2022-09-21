#include "TitleScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"
//#include "Quaternion.h"

#include "Texture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"
#include "Object/GameObject/GameObject.h"

// コンポーネント
#include "Object3dComponent.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Camera.h"
#include "Assets/Scripts/Player.h"
#include "Weapon.h"



TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

}

void TitleScene::Initialize()
{


	auto directional_light_ = GameObject::CreateObject("Directional Light");
	directional_light_.lock()->AddComponent<Light>(light_manager_, LightType::Directional);
	directional_light_.lock()->transform->lock()->localQuaternion = { 1,0,0,30 };
	// プレイヤー
	auto player = GameObject::CreateObject("Human");
	player.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/sword human/Sword man.fbx");
	player.lock()->AddComponent<SphereCollider>();
	player.lock()->AddComponent<Rigidbody>();
	player.lock()->AddComponent<Player>();
	player.lock()->transform->lock()->localScale = { 0.4f,0.4f,0.4f };
	player.lock()->transform->lock()->localPosition = { 0.0f,0.0f,20.0f };

	//auto player_light = game_object_manager_->CreateObject("Player light");
	//player_light.lock()->AddComponent<Light>(light_manager_);
	//player_light.lock()->transform->lock()->localPosition = { 0.0f,50.0f,0.0f };
	//game_object_manager_->SetPearentChild(player, player_light);

	//auto sphere = GameObject::CreateObject("sphere");
	//sphere.lock()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/test/test.fbx");

	//auto castle = GameObject::CreateObject("Castle");
	//castle.lock()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Castle/Castle FBX.fbx");
	//castle.lock()->transform->lock()->localScale = { 20,20,20 };
	//castle.lock()->transform->lock()->localPosition = { 0,-7,0 };

	auto danbo = GameObject::CreateObject("Bot");
	danbo.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/danbo_fbx/danbo_taiki.fbx");
	danbo.lock()->AddComponent<SphereCollider>();


	auto test = GameObject::CreateObject("test");
	test.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Test/test.fbx");

	auto camera = GameObject::CreateObject("Camera");
	camera.lock()->AddComponent<Camera>();
	camera.lock()->transform->lock()->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera.lock()->transform->lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	game_object_manager_->SetPearentChild(player, camera);



	//// 武器
	//auto weapon = GameObject::CreateObject("Leona's sword");
	//weapon.lock()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	//game_object_manager_->SetPearentChild(player, weapon);
	//weapon.lock()->AddComponent<Weapon>();

	//Vector3 test = player.lock()->transform->lock()->scale;
}


