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


TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	skinnedMeshes[0] = std::make_shared<SkinnedMesh>(DirectXCommon::dev.Get(), "Resources/3d/box/box.fbx");
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

	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_SPACE)) {
		t = 0.0f;
		//obj->SetPosition({ -10,5,0 });

	}
	//obj->SetPosition(Ease(In, Quad, t, { -10,5,0 }, { 10,5,0 }));

	if (t >= 1.0f) {
		t = 1.0f;
	}
	else {
		t += 0.005f;

	}

	//obj->Update();
	
	//ゲームオブジェクト全てをアップデート
	gameObjectManager.Update();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();


 	CollisionManager::GetInstance()->CheckBroadCollisions();
}

void TitleScene::Draw() const
{
	XMFLOAT4X4 world = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	static XMFLOAT3 pos = { 0,0,0 }, rot = { 0,0,0 }, sca = { 1,1,1 };

	ImGui::Begin("testWindow");
	ImGui::DragFloat3("rot", &rot.x);
	
	ImGui::End();

	XMMATRIX transfome = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX rotX = DirectX::XMMatrixRotationX(rot.x);
	XMMATRIX rotY = DirectX::XMMatrixRotationY(rot.y);
	XMMATRIX rotZ = DirectX::XMMatrixRotationZ(rot.z);
	XMMATRIX rotation = rotY * rotZ * rotX;
	XMMATRIX scale = DirectX::XMMatrixScaling(sca.x, sca.y, sca.z);
	
	XMMATRIX mat = scale * rotation * transfome;
	DirectX::XMStoreFloat4x4(&world,mat);
	//描写テスト
	skinnedMeshes[0]->Render(DirectXCommon::cmdList.Get(), world, { 1,0,0,1 });
	//obj->Draw(DirectXCommon::cmdList.Get());
	
	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

