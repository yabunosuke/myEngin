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
#include "Assets/Scripts/SpikeEnemy.h"
#include "Weapon.h"
#include "Assets/Scripts/Drone.h"
#include "Assets/Scripts/SetBoneParent.h"
#include "Assets/Scripts/EnemySearchPlayer.h"
#include "Assets/Scripts/Boss.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"


GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{
	srand(time(NULL));

	// ライト
	auto directional_light_ = GameObject::AddGameObjet("Directional Light");
	directional_light_.lock()->AddComponent<Light>(LightType::Directional);
	XMStoreFloat4(&directional_light_.lock()->transform->lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(45, 45, 0));

	auto floors = GameObject::AddGameObjet("Floors");

	for( int i = 0; i<2;++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			auto floor2_big = GameObject::AddGameObjet("Floor2 big");
			floor2_big.lock()->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			floor2_big.lock()->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big.lock()->transform->lock()->position = {i * 6.0f,0.0f,j * 6.0f};
			floor2_big.lock()->tag = "Stage";
			floor2_big.lock()->SetParent(floors);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for(int j = 0;j<3;++j)
		{
			auto floor2_big = GameObject::AddGameObjet("Floor2 big");
			floor2_big.lock()->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			floor2_big.lock()->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big.lock()->transform->lock()->position  = { j * 6.0f - 3.0f,3.4f,36.0f + i * 6.0f };
			floor2_big.lock()->tag = "Stage";
			floor2_big.lock()->SetParent(floors);
		}
	}


	for (int i = 0; i < 2; ++i)
	{
		auto pillar_big = GameObject::AddGameObjet("Pillar big");
		pillar_big.lock()->tag = "Stage";
		pillar_big.lock()->isStatic = true;
		pillar_big.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/pillar_big.fbx");
		pillar_big.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 1.0f,4.5f,1.0f }, Vector3{ 0,2.25f,0 });
		if (i == 0)
		{
			pillar_big.lock()->transform->lock()->position  = { -3.0f,0,6.0f };
		}
		else
		{
			pillar_big.lock()->transform->lock()->position  = { 3.0f,0,6.0f };

		}
	}

	auto walls = GameObject::AddGameObjet("walls");
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::AddGameObjet("Wall");
		wall.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall.lock()->isStatic = true;
		wall.lock()->transform->lock()->position  = { -6.0f,0,i * 6.0f };
		wall.lock()->tag = "Stage";
		wall.lock()->isStatic = true;
		wall.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall.lock()->SetParent(walls);
	}
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::AddGameObjet("Wall");
		wall.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall.lock()->isStatic = true;
		wall.lock()->transform->lock()->position  = { 6.0f,0,i * 6.0f - 6.0f };
		wall.lock()->transform->lock()->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
		wall.lock()->tag = "Stage";
		wall.lock()->isStatic = true;
		wall.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall.lock()->SetParent(walls);
	}
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::AddGameObjet("Wall");
		wall.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall.lock()->isStatic = true;
		wall.lock()->transform->lock()->position  = { i * 6.0f -6.0f ,0,-6.0};
		wall.lock()->transform->lock()->quaternion = Quaternion::Euler(0, -90.0f * Mathf::deg_to_rad, 0);
		wall.lock()->tag = "Stage";
		wall.lock()->isStatic = true;
		wall.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
		wall.lock()->SetParent(walls);
	}

	float buf[2]{ -3.0f,3.0f };
	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::AddGameObjet("Wall");
		wall.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall.lock()->isStatic = true;
		wall.lock()->transform->lock()->position  = { i * 6.0f + buf[i],0,6.0};
		wall.lock()->transform->lock()->quaternion = Quaternion::Euler(0, 90.0f * Mathf::deg_to_rad, 0);
		wall.lock()->tag = "Stage";
		wall.lock()->isStatic = true;
		wall.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/wall.fbx");
		wall.lock()->SetParent(walls);
	}

	int buf_z{ 0 };
	for (int i = 0; i < 6; ++i)
	{
		if (i % 2 == 0)
		{
			++buf_z;


			auto floor2_big = GameObject::AddGameObjet("Floor2 big");
			floor2_big.lock()->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			floor2_big.lock()->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big.lock()->transform->lock()->position  = { 3.0f,0.0f, 12.0f + 6.0f * (buf_z - 1) };
			floor2_big.lock()->tag = "Stage";
			floor2_big.lock()->SetParent(floors);

		}
		auto wall = GameObject::AddGameObjet("Wall");
		wall.lock()->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall.lock()->isStatic = true;
		if (i < 3)
		{
			wall.lock()->transform->lock()->position  = { buf[0],0,6.0f * i + 6.0f };
			wall.lock()->transform->lock()->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
		}
		else
		{
			wall.lock()->transform->lock()->position  = { buf[1],0,6.0f * (i - 3) + 12.0f };
		}
		wall.lock()->tag = "Stage";
		wall.lock()->isStatic = true;
		wall.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/wall.fbx");
		wall.lock()->SetParent(walls);
		
	}

	auto stairs = GameObject::AddGameObjet("Stairs Big");
	auto collider = GameObject::AddGameObjet("Collder");
	collider.lock()->AddComponent<OBBCollider>(Quaternion::Euler(0, 0, 0), Vector3{ 6.0f,1.0f,7.6f }, Vector3{ -3.0f,-0.5f,-3.0f });
	collider.lock()->transform->lock()->quaternion = Quaternion::Euler(30.0f * Mathf::deg_to_rad, 0, 0);
	collider.lock()->SetParent(stairs);
	stairs.lock()->isStatic = true;
	stairs.lock()->transform->lock()->position  = { -3.5f,0,24.0f };
	stairs.lock()->transform->lock()->quaternion = Quaternion::Euler(0, 180.0f * Mathf::deg_to_rad, 0);
	stairs.lock()->tag = "Stage";
	stairs.lock()->isStatic = true;
	stairs.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dungeon/fbx walls/stairs_big.fbx");
	


	// カメラ
	auto camera_root = GameObject::AddGameObjet("CameraRoot");
	camera_root.lock()->AddComponent<PlayerCameraController>();
	auto camera_rotation_root = GameObject::AddGameObjet("RotationRoot");
	camera_rotation_root.lock()->SetParent(camera_root);
	auto camera_height_root = GameObject::AddGameObjet("HeightRoot");
	camera_height_root.lock()->SetParent(camera_rotation_root);
	auto main_camera = GameObject::AddGameObjet("MainCamera");
	main_camera.lock()->SetParent(camera_height_root);
	main_camera.lock()->transform->lock()->localPosition = { 0.0f,1.5f,2.0f };
	main_camera.lock()->AddComponent<Camera>();


	// プレイヤー
	auto player = GameObject::AddGameObjet("Player");
	player.lock()->tag = "Player";
	player.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/RPG Characters - Nov 2020/Humanoid Rig Versions/FBX/Warrior.fbx");
	auto rg = player.lock()->AddComponent<Rigidbody>();
	rg.lock()->useGravity = true;
	player.lock()->AddComponent<PlayerController>();
	player.lock()->AddComponent<SphereCollider>(2, Vector3{ 0,0.5f,0 });
	auto player_light = GameObject::AddGameObjet("Player Light");
	player_light.lock()->transform->lock()->localPosition = { 0.0f,0.0f,0.0f };
	auto ligh_info = player_light.lock()->AddComponent<Light>();
	ligh_info.lock()->range = 1.0f;
	ligh_info.lock()->intensity = 1.0f;
	player_light.lock()->SetParent(player);

	// ドローン
	auto drone = GameObject::AddGameObjet("Drone");
	drone.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Drone/drone.fbx");
	drone.lock()->AddComponent<Drone>(&player.lock()->transform->lock()->position .r_);

	// 武器
	auto weapon = GameObject::AddGameObjet("Weapon");
	weapon.lock()->tag = "Weapon";
	weapon.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	weapon.lock()->SetParent(player);
	//game_object_manager_->SetPearentChild(player, weapon);
	weapon.lock()->AddComponent<Weapon>();
	auto weapon_collider = weapon.lock()->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f },Vector3{1.8f,6.2f,1.8f});
	weapon_collider.lock()->isTrigger = true;
	

	//for (int i = 0; i < 2; ++i)
	//{
	//	auto s_enemy = GameObject::AddGameObjet("Spike Enemy");
	//	s_enemy->tag = "Enemy";
	//	s_enemy->transform_->scale = { 0.3f,0.3f,0.3f };
	//	s_enemy->transform->lock()->position  = { i - 1.0f,0.0f,0.0f };
	//	auto s_rg = s_enemy.lock()->AddComponent<Rigidbody>();
	//	s_rg->useGravity = true;
	//	s_enemy.lock()->AddComponent<Object3dComponent>(
	//		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//		"Assets/3d/Ultimate Monsters/Blob/FBX/GreenSpikyBlob.fbx");
	//	s_enemy.lock()->AddComponent<SpikeEnemy>();
	//	s_enemy.lock()->AddComponent<SphereCollider>(2.0f, Vector3{ 0.0f,0.5f,0.0f });
	//	{
	//		// 索敵用コライダー
	//		{
	//			auto s_search = GameObject::AddGameObjet("Search");
	//			auto s_search_collider = s_search.lock()->AddComponent<SphereCollider>(12.0f, Vector3{ 0.0f,0.5f,0.0f });
	//			s_search_collider->isTrigger = true;
	//			s_search.lock()->AddComponent<EnemySearchPlayer>();
	//			s_search->SetParent(s_enemy);
	//		}
	//	}
	//}


	{
		auto boss = GameObject::AddGameObjet("Boss");
		boss.lock()->transform->lock()->scale = { 0.1f,0.1f, 0.1f };
		boss.lock()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Boss/fire_elemental.fbx");
		boss.lock()->AddComponent<Boss>();
		auto rg = boss.lock()->AddComponent<Rigidbody>();
		rg.lock()->useGravity = true;
		// 地形衝突判定用
		boss.lock()->AddComponent<SphereCollider>(10, Vector3{ 0,0.5f,0 });
		// 被弾判定用
		{
			auto hit_collider = GameObject::AddGameObjet("BossHitSphere");
			hit_collider.lock()->SetParent(boss);
			auto collider = hit_collider.lock()->AddComponent<SphereCollider>(20, Vector3{ 0,0.5f,0 });
			collider.lock()->isTrigger = true;

		}
		// 攻撃判定用
		{
			{
				auto attack_collider_R = GameObject::AddGameObjet("AttackSphereR");
				attack_collider_R.lock()->activeSelf = false;
				attack_collider_R.lock()->tag = "Enemy Attack Area";
				attack_collider_R.lock()->SetParent(boss);
				attack_collider_R.lock()->AddComponent< SetBoneParent>("RightHandMiddle1");
				auto collider = attack_collider_R.lock()->AddComponent<SphereCollider>(0.1);
				collider.lock()->isTrigger = true;
			}
			{
				auto attack_collider_L = GameObject::AddGameObjet("AttackSphereL");
				attack_collider_L.lock()->activeSelf = false;
				attack_collider_L.lock()->tag = "Enemy Attack Area";
				attack_collider_L.lock()->SetParent(boss);
				attack_collider_L.lock()->AddComponent< SetBoneParent>("LeftHandMiddle1");
				auto collider = attack_collider_L.lock()->AddComponent<SphereCollider>(0.1);
				collider.lock()->isTrigger = true;
			}

		}
	}

	auto eye = GameObject::AddGameObjet("Eye");
	eye.lock()->transform->lock()->localPosition = { 0, 1.4f, 0.0f };
	eye.lock()->SetParent(player);

	//auto canvase = GameObject::AddGameObjet("Canvase",true);
	//auto image_test = GameObject::AddGameObjet("Image",true);
	//image_test.lock()->SetParent(canvase);

}

void GameScene::Update()
{
	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->SceneStackPop();
	}
}
