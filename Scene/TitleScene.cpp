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
#include "Component/Light.h"
#include "Component/Rigidbody.h"
#include "Component/Camera.h"
#include "Object/Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "PlayerTest.h"



TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

	// プレイヤー
	auto player = game_object_manager_.CreateObject("human");
	player.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/test/human.fbx");
	player.lock().get()->AddComponent<ColliderComponent>(this);
	player.lock().get()->AddComponent<Rigidbody>();
	player.lock().get()->AddComponent<PlayerTest>();
	player.lock().get()->AddComponent<Light>(light_manager_);
	player.lock()->transform->lock()->localScale = { 0.2f,0.2f,0.2f };

	auto test = game_object_manager_.CreateObject("Test");
	test.lock()->AddComponent<MeshRenderer>();

	/*auto dorone = game_object_manager_.CreateObject("Dorone");
	dorone.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/Drone166/Drone166.1.fbx");*/

	auto castle = game_object_manager_.CreateObject("Castle");
	castle.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Castle/Castle FBX.fbx");
	castle.lock()->transform->lock()->localScale = { 20,20,20 };
	castle.lock()->transform->lock()->localPosition = { 0,-7,0 };
	/*auto streat = game_object_manager_.CreateObject("streat");
	streat.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/streat/Street environment_V01.FBX");*/

	//auto test = game_object_manager_.CreateObject("plantune");
	//test.lock()->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/UNIT/danbo_fbx/danbo_taiki.fbx");
	//test.lock().get()->AddComponent<ColliderComponent>(this, CollisionShapeType::SHAPE_SPHERE);

	auto camera = game_object_manager_.CreateObject("Camera");
	camera.lock()->AddComponent<Camera>();
	camera.lock()->transform->lock()->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera.lock()->transform->lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	game_object_manager_.SetPearentChild(player, camera);



	// 武器
	auto weapon = game_object_manager_.CreateObject("GunBraid");

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
	collision_manager_.CheckBroadCollisions(game_object_manager_.game_objects_);

	game_object_manager_.LastUpdate();
	
 	
}