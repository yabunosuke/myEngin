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
}

void TitleScene::Initialize()
{
	cam = new Camera({ 0,0,-20 });
	Camera::SetCam(cam);
	model = ModelLoader::GetInstance()->LoadModelFromFile("ball");
	for (auto &object : obj) {
		object = Object3d::Create(model.get());
	}
	obj[0]->SetPosition({ -10,5,0 });
	obj[1]->SetPosition({ -10,0,0 });
	obj[2]->SetPosition({ -10,-5,0 });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	static float t = 0;

	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_SPACE)) {
		t = 0.0f;
		obj[0]->SetPosition({ -10,5,0 });
		obj[1]->SetPosition({ -10,0,0 });
		obj[2]->SetPosition({ -10,-5,0 });

	}
	obj[0]->SetPosition(Ease(In,	Quad,	t, { -10,5,0 }, { 10,5,0 }));
	obj[1]->SetPosition(Ease(InOut, Quad, t, { -10,0,0 }, { 10,0,0 }));
	obj[2]->SetPosition(Ease(Out, Quad, t, { -10,-5,0 },	{10,-5,0}));

	if (t >= 1.0f) {
		t = 1.0f;
	}
	else {
		t += 0.005f;

	}

	for (auto &object : obj) {
		object->Update();
	}
	//ゲームオブジェクト全てをアップデート
	gameObjectManager.Update();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();


 	CollisionManager::GetInstance()->CheckBroadCollisions();
}

void TitleScene::Draw() const
{

	for (auto &object : obj) {
		object->Draw(DirectXCommon::cmdList.Get());
	}
	//obj.get()->Draw(DirectXCommon::cmdList.Get());
	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

