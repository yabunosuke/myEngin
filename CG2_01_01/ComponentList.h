#pragma once
#include <map>
#include <memory>
#include <functional>
#include "Component/Component.h"

#include "Component/Transform.h"
#include "Component/LightComponent.h"
#include "Component/CameraComponent.h"

#include "GameObject.h"


class ComponentList
{

private:	//シングルトンパターン
	ComponentList() = default;
	~ComponentList() = default;
	ComponentList(const ComponentList &r) = default;
	ComponentList &operator= (const ComponentList &r) = default;

	// エイリアス

public:
	//コンポーネントの種類
	enum class Group
	{
		Audio,
		Transfome,
		Light,
		Physics,
		Physics2D,
		Rendering,

		End,
	};
public:	
	//インスタンスの取得
	static ComponentList *GetIns();

	static void Initialize();
	
	std::map<ComponentList::Group, std::vector<std::string>> &GetComponentGroup() { return component_group_; }
	std::map<std::string, void(*)(GameObject *)> &GetComponentList() { return components_; }
private:
	static void ComponentGroupSet(Group group, std::string name);


	void addDummy(GameObject *object) {};
	//トランスフォームコンポーネント追加
	void addTransform(GameObject *object) {object->AddComponent<Transform>();}
	//ライトコンポーネント追加
	//void addLight(GameObject *object) { object->AddComponent<LightComponent>(); }
	//トランスフォームコンポーネント追加
	void addColision(GameObject *object) { object->AddComponent<Transform>(); }
	//トランスフォームコンポーネント追加
	void addCamera(GameObject *object) { object->AddComponent<CameraComponent>(); }


	//コンポーネントグループ
	static std::map<ComponentList::Group, std::vector<std::string>> component_group_;
	//コンポーネント名とAddComponentのセット
	static std::map<std::string, void(*)(GameObject*)> components_;

	// オブジェクトのポインタ
	
};

