#include "GameScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

#include "oldTexture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"
#include "Object/GameObject/GameObject.h"

// �R���|�[�l���g
#include "Object3dComponent.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Camera.h"
#include "Assets/Scripts/Player.h"
#include "Assets/Scripts/Enemy.h"
#include "Assets/Scripts/CameraController.h"
#include "Weapon.h"


GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{

	// ���C�g
	auto directional_light_ = GameObject::CreateObject("Directional Light");
	directional_light_->AddComponent<Light>(light_manager_, LightType::Directional);
	XMStoreFloat4(&directional_light_->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(45, 45, 0));

	//// ��
	//auto castle = GameObject::CreateObject("Castle");
	//castle->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Castle/Castle FBX.fbx");
	//castle->transform_->localScale = { 20,20,20 };
	//castle->transform_->localPosition = { 0,-7,0 };

	auto floor = GameObject::CreateObject("Town");
	floor->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dungeon/fbx walls/floor_big.fbx");
	floor->transform_->position = { 1000.0f, 0.0f, 1000.0f };
	floor->transform_->scale = { 10.0f, 10.0f, 10.0f };

	// �v���C���[
	auto player = GameObject::CreateObject("Human");
	player->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/sword human/Sword man.fbx");
	player->AddComponent<Rigidbody>();
	player->AddComponent<PlayerController>();
	XMStoreFloat4(&player->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));

	auto eye = GameObject::CreateObject("Eye");
	eye->SetParent(player);
	eye->transform_->localPosition = { 0.0f,150.0f,0.0f };


	//auto test = GameObject::CreateObject("Test");
	//test.lock()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Test/BotH.fbx");

	auto enemy = GameObject::CreateObject("Enemy");
	enemy->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dwarf/Dwarf.fbx");
	enemy->AddComponent<SphereCollider>(25, Vector3{ 0,20,0 });
	enemy->transform_->localScale = { 0.4f,0.4f,0.4f };
	enemy->transform_->localPosition = { 0.0f,0.0f,20.0f };
	enemy->AddComponent<Enemy>();

	// �J����
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	camera->AddComponent<CameraController>(eye);
	//camera->SetParent(eye);

	////auto miku_test = GameObject::CreateObject("Miku");
	////miku_test->AddComponent<Object3dComponent>(
	////	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	////	"Assets/3d/RunMiku/RunMiku.fbx");

	// ����
	auto weapon = GameObject::CreateObject("Weapon");
	weapon->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	weapon->SetParent(player);
	//game_object_manager_->SetPearentChild(player, weapon);
	weapon->AddComponent<Weapon>();
	weapon->AddComponent<SphereCollider>(4);

}

void GameScene::Update()
{
	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->SceneStackPop();
	}
}
