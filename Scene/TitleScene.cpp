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
#include "Assets/Scripts/TitleManager.h"



TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	//LoadSceneData("Scene/Data/title.scene");
}

void TitleScene::Initialize()
{
	// カメラ
	auto camera =GameObject::AddGameObjet("Camera");
	camera.lock()->AddComponent<Camera>();
	camera.lock()->transform->lock()->localPosition = {0,0,-10};
	camera.lock()->transform->lock()->localQuaternion = Quaternion::Euler(0, 0, 0);

	auto test = GameObject::AddGameObjet("test");
	test.lock()->transform->lock()->localPosition = {1,2,3};
	auto test1 = GameObject::AddGameObjet("test1");
	test.lock()->SetParent(test1);



}

void TitleScene::Draw() const
{
	AbstractScene::Draw();
}


