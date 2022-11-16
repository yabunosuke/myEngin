#include "GameScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

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
#include "Assets/Scripts/Enemy.h"
#include "Assets/Scripts/FlyEnemy.h"
#include "Assets/Scripts/FlyEnemyEye.h"
#include "Assets/Scripts/CameraController.h"
#include "Assets/Scripts/CameraController/PlayerCameraController.h"
#include "Weapon.h"
#include "Assets/Scripts/Drone.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"


GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{
	srand(time(NULL));

	// ライト
	auto directional_light_ = GameObject::CreateObject("Directional Light");
	directional_light_->AddComponent<Light>(LightType::Directional);
	XMStoreFloat4(&directional_light_->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(45, 45, 0));

	auto floors = GameObject::CreateObject("Floors");

	for( int i = 0; i<2;++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			auto floor2_big = GameObject::CreateObject("Floor2 big");
			floor2_big->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			floor2_big->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big->transform_->position = { i * 6.0f,0.0f,j * 6.0f };
			floor2_big->SetParent(floors);
		}
	}
	for (int i = 0; i < 3; ++i)
	{

		auto floor2_big = GameObject::CreateObject("Floor2 big");
		floor2_big->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
		floor2_big->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
		floor2_big->transform_->position = { 3.0f,3.4f,36.0f + i * 6.0f };
		floor2_big->SetParent(floors);
	}


	for (int i = 0; i < 2; ++i)
	{
		auto pillar_big = GameObject::CreateObject("Pillar big");
		pillar_big->tag = "Pillar";
		pillar_big->isStatic = true;
		pillar_big->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/pillar_big.fbx");
		pillar_big->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 1.0f,4.5f,1.0f }, Vector3{ 0,2.25f,0 });
		if (i == 0)
		{
			pillar_big->transform_->position = { -3.0f,0,6.0f };
		}
		else
		{
			pillar_big->transform_->position = { 3.0f,0,6.0f };

		}
	}

	auto walls = GameObject::CreateObject("walls");
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { -6.0f,0,i * 6.0f };
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall->SetParent(walls);
	}
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { 6.0f,0,i * 6.0f - 6.0f };
		wall->transform_->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall->SetParent(walls);
	}
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { i * 6.0f -6.0f ,0,-6.0};
		wall->transform_->quaternion = Quaternion::Euler(0, -90.0f * Mathf::deg_to_rad, 0);
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall->SetParent(walls);
	}

	float buf[2]{ -3.0f,3.0f };
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { i * 6.0f + buf[i],0,6.0};
		wall->transform_->quaternion = Quaternion::Euler(0, 90.0f * Mathf::deg_to_rad, 0);
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/wall.fbx");
		wall->SetParent(walls);
	}

	int buf_z{ 0 };
	for (int i = 0; i < 6; ++i)
	{
		if (i % 2 == 0)
		{
			++buf_z;


			auto floor2_big = GameObject::CreateObject("Floor2 big");
			floor2_big->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			floor2_big->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big->transform_->position = { 3.0f,0.0f, 12.0f + 6.0f * (buf_z - 1) };
			floor2_big->SetParent(floors);

		}
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		if (i < 3)
		{
			wall->transform_->position = { buf[0],0,6.0f * i + 6.0f };
			wall->transform_->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
		}
		else
		{
			wall->transform_->position = { buf[1],0,6.0f * (i - 3) + 12.0f };
		}
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/wall.fbx");
		wall->SetParent(walls);
		
	}

	auto stairs = GameObject::CreateObject("Stairs Big");
	auto collider = GameObject::CreateObject("Collder");
	collider->AddComponent<OBBCollider>(Quaternion::Euler(0, 0, 0), Vector3{ 6.0f,1.0f,7.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
	collider->transform_->quaternion = Quaternion::Euler(30.0f * Mathf::deg_to_rad, 0, 0);
	collider->SetParent(stairs);
	stairs->isStatic = true;
	stairs->transform_->position = { -3.5f,0,24.0f };
	stairs->transform_->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
	stairs->tag = "Stage";
	stairs->isStatic = true;
	stairs->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dungeon/fbx walls/stairs_big.fbx");
	/*wall->SetParent(road_one);*/
	


	// カメラ
	auto camera_root = GameObject::CreateObject("CameraRoot");
	camera_root->AddComponent<PlayerCameraController>();
	auto camera_rotation_root = GameObject::CreateObject("RotationRoot");
	camera_rotation_root->SetParent(camera_root);
	auto camera_height_root = GameObject::CreateObject("HeightRoot");
	camera_height_root->SetParent(camera_rotation_root);
	auto main_camera = GameObject::CreateObject("MainCamera");
	main_camera->SetParent(camera_height_root);
	main_camera->transform_->localPosition = { 0.0f,1.5f,2.0f };
	main_camera->AddComponent<Camera>();


	// プレイヤー
	auto player = GameObject::CreateObject("Knight");
	player->tag = "Player";
	player->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/RPG Characters - Nov 2020/Humanoid Rig Versions/FBX/Warrior.fbx");
	auto rg = player->AddComponent<Rigidbody>();
	rg->useGravity = true;
	player->AddComponent<PlayerController>();
	player->AddComponent<SphereCollider>(2, Vector3{ 0,0.5f,0 });
	auto player_light = GameObject::CreateObject("Player Light");
	player_light->transform_->localPosition = { 0.0f,0.0f,0.0f };
	auto ligh_info = player_light->AddComponent<Light>();
	ligh_info->range = 1.0f;
	ligh_info->intensity = 1.0f;
	player_light->SetParent(player);

	// ドローン
	auto drone = GameObject::CreateObject("Drone");
	drone->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Drone/drone.fbx");
	drone->AddComponent<Drone>(&player->transform_->position.r_);

	// 武器
	auto weapon = GameObject::CreateObject("Weapon");
	weapon->tag = "Weapon";
	weapon->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	weapon->SetParent(player);
	//game_object_manager_->SetPearentChild(player, weapon);
	weapon->AddComponent<Weapon>();
	auto weapon_collider = weapon->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f },Vector3{1.8f,6.2f,1.8f});
	weapon_collider->isTrigger = true;
	

	auto s_enemy = GameObject::CreateObject("Spike Enemy");
	s_enemy->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Ultimate Monsters/Blob/FBX/GreenSpikyBlob.fbx");
	//auto enemy = GameObject::CreateObject("Enemy");
	//enemy->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Dwarf/Dwarf.fbx");
	//auto enemy_rg = enemy->AddComponent<Rigidbody>();
	//enemy_rg->useGravity = true;
	//enemy->AddComponent<Enemy>();
	//enemy->transform_->position = { 0.0f,4.0f,40.0f };
	//enemy->AddComponent<SphereCollider>(1, Vector3{ 0,0.5f,0 });

	auto eye = GameObject::CreateObject("Eye");
	eye->transform_->localPosition = { 0, 1.4f, 0.0f };
	eye->SetParent(player);

}

void GameScene::Update()
{
	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->SceneStackPop();
	}
}
