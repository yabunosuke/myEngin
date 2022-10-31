#include "TitleScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

#include "oldTexture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"
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
	camera->transform_->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));


	auto title = GameObject::CreateObject("Title");
	title->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/title.fbx");
	title->AddComponent<SphereCollider>();
	title->AddComponent<Rigidbody>();
	title->transform_->quaternion = Quaternion::Euler(90.0f*Mathf::deg_to_rad, 180.0f * Mathf::deg_to_rad, 0);
	auto child_test= GameObject::CreateObject("Title");
	child_test->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/temp/title.fbx");
	child_test->AddComponent<SphereCollider>();
	child_test->AddComponent<Rigidbody>();

}

void TitleScene::Update()
{

	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->OnSceneChanged(Scenes::Game);
	}
}


