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



TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

}

void TitleScene::Initialize()
{
	// カメラ
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,10,-10 };
	camera->transform_->localQuaternion = Quaternion::Euler(0, 0, 0);

	auto title = GameObject::CreateObject("Title");
	title->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/title.fbx");
	title->AddComponent<SphereCollider>();
	Rigidbody *rg = title->AddComponent<Rigidbody>();
	rg->useGravity = true;
	title->transform_->quaternion = Quaternion::Euler(90.0f*Mathf::deg_to_rad, 180.0f * Mathf::deg_to_rad, 0);
	title->transform_->position = {0,20,0};
	auto child_test= GameObject::CreateObject("Title");
	child_test->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/title.fbx");
	child_test->AddComponent<SphereCollider>();
	child_test->AddComponent<Rigidbody>();
	child_test->isStatic = true;

}

void TitleScene::Update()
{

	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->OnSceneChanged(Scenes::Game);
	}
}

void TitleScene::Draw() const
{
	AbstractScene::Draw();
}


