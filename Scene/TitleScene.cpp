#include "TitleScene.h"
#include "KeyboardInput.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "DirectXCommon.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Collision.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

#include "Easing.h"
#include "yMath.h"


#include "PipelineManager.h"


#include "Player.h"

TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	//skinnedMeshes[0] = std::make_shared<SkinnedMesh>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/Drone166/Drone166.1.fbx",false);
	//skinnedMeshes[0] = std::make_shared<SkinnedMesh>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/AimTest/MNK_Mesh.fbx");
	//skinnedMeshes[0] = std::make_shared<SkinnedMesh>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/nico.fbx");
	//skinnedMeshes[0]->AppendAnimations("Assets/3d/UNIT/AimTest/Aim_Space.fbx");

	PipelineManager::GetInstance()->CreatePipline(DirectXCommon::dev.Get(), "Lambert");
	test[0] = std::make_shared<Fbx>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/drone166/drone166.1.fbx");


	gameObjectManager.CreateObject();
	auto player = gameObjectManager.CreateObject("Player");
	player.lock().get()->AddComponent(std::make_shared<Player>());
}

void TitleScene::Initialize()
{
	cam = new Camera({ 0,0,-50 });
	Camera::SetCam(cam);
	//model = ModelLoader::GetInstance()->LoadModelFromFile("ball");
	//obj = Object3d::Create(model.get());
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	test[0]->UpdateAnimation();
	
	//ゲームオブジェクト全てをアップデート
	gameObjectManager.Update();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();


 	CollisionManager::GetInstance()->CheckBroadCollisions();
}

void TitleScene::Draw() const
{

	static XMFLOAT3 pos = { 0,0,0 }, rot = { 0,0,0 }, sca = { 1,1,1 };

	ImGui::Begin("testWindow");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rot", &rot.x);
	ImGui::DragFloat3("sca", &sca.x);

	if (ImGui::Button("0")) {
		test[0]->PlayAnimation(0);
	}
	if (ImGui::Button("1")) {
		test[0]->PlayAnimation(1);
	}
	if (ImGui::Button("6")) {
		test[0]->PlayAnimation(6);
	}
	ImGui::End();
	
	XMMATRIX S = XMMatrixScaling(sca.x, sca.y, sca.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(DegToRad(rot.x), DegToRad(rot.y), DegToRad(rot.z));
	XMMATRIX T = XMMatrixTranslation(pos.x, pos.y, pos.z);

	XMFLOAT4X4 world;
	XMMATRIX mat = S * R * T;
	DirectX::XMStoreFloat4x4(&world, mat);
	
	test[0].get()->UpdateTransform(world);

	test[0].get()->Draw(DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get());
	
	//const float scale_factor = 100.0f;
	
	

	////アニメーション
	//int clip_index = 0;
	//int frame_index = 0;
	//static float animation_tick = 0;
	//Animation &animation = skinnedMeshes[0]->animation_clips_.at(clip_index);
	//frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
	//if (frame_index > animation.sequence.size() - 1) {
	//	frame_index = 0;
	//	animation_tick = 0;
	//}
	//else {
	//	animation_tick += 0.016;
	//}
	//Animation::Keyframe &keyframe = animation.sequence.at(frame_index);

	//XMStoreFloat4(&keyframe.nodes.at(30).rotation, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), 1.5f));
	//keyframe.nodes.at(30).translation.x = -300.0f;
	/*static float kubi[3] = { 0,0,0 };
	ImGui::Begin("kubi");
	ImGui::DragFloat3("kubi", kubi);
	ImGui::End();
	keyframe.nodes.at(30).translation.x = kubi[0];
	keyframe.nodes.at(30).translation.y = kubi[1];
	keyframe.nodes.at(30).translation.z = kubi[2];*/

	//skinnedMeshes[0]->UpdateAnimation(keyframe);

	//描写テスト
	//skinnedMeshes[0]->Render(DirectXCommon::dev.Get(),DirectXCommon::cmdList.Get(), world, { 1,1,1,1 },&keyframe);
	//obj->Draw(DirectXCommon::cmdList.Get());
	
	gameObjectManager.Draw();


	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

