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


TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	skinnedMeshes[0] = std::make_shared<SkinnedMesh>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/cube.003.0.fbx");
}

void TitleScene::Initialize()
{
	cam = new Camera({ 0,0,-20 });
	Camera::SetCam(cam);
	//model = ModelLoader::GetInstance()->LoadModelFromFile("ball");
	//obj = Object3d::Create(model.get());
	

}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	static float t = 0;

	
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
	
	ImGui::End();

	const XMFLOAT4X4 coordinate_system_transforms[] = {
		{	-1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		}, // 0:RHS Y-UP
		
		{	1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 
		}, // 1:LHS Y-UP
		
		{	-1, 0,  0, 0,
			 0, 0, -1, 0,
			 0, 1,  0, 0,
			 0, 0,  0, 1
		}, // 2:RHS Z-UP

		{	1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1 }, // 3:LHS Z-UP
	};
	const float scale_factor = 1.0f;
	//XMMATRIX C = XMLoadFloat4x4(&coordinate_system_transforms[0]) * XMMatrixScaling(scale_factor, scale_factor, scale_factor);
	XMMATRIX S = XMMatrixScaling(1, 1, 1);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(DegToRad(rot.x), DegToRad(rot.y), DegToRad(rot.z));
	XMMATRIX T = XMMatrixTranslation(pos.x,pos.y,pos.z);

	XMFLOAT4X4 world;
	//XMStoreFloat4x4(&world, C * S * R * T);

	//XMMATRIX transfome = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//XMMATRIX rotX = DirectX::XMMatrixRotationX(DegToRad(rot.x));
	//XMMATRIX rotY = DirectX::XMMatrixRotationY(DegToRad(rot.y));
	//XMMATRIX rotZ = DirectX::XMMatrixRotationZ(DegToRad(rot.z));
	//XMMATRIX rotation = rotY * rotZ * rotX;
	//XMMATRIX scale = DirectX::XMMatrixScaling(sca.x, sca.y, sca.z);

	XMMATRIX mat = S * R * T;
	DirectX::XMStoreFloat4x4(&world,mat);
	//描写テスト
	skinnedMeshes[0]->Render(DirectXCommon::dev.Get(),DirectXCommon::cmdList.Get(), world, { 1,1,1,1 });
	//obj->Draw(DirectXCommon::cmdList.Get());
	
	gameObjectManager.Draw();


	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

