#pragma once
#include <map>
#include <memory>
#include <functional>
#include "Component.h"

#include "TransformComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"

#include "GameObject.h"

class ComponentList
{
private:	//シングルトンパターン
	ComponentList() = default;
	~ComponentList() = default;
	ComponentList(const ComponentList &r) = default;
	ComponentList &operator= (const ComponentList &r) = default;

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
	
	std::map<ComponentList::Group, std::vector<std::string>> &GetComponentGroup() { return componentGroup; }
	std::map<std::string, void(*)(GameObject *)> &GetComponentList() { return componentList; }
private:
	static void addDummy(GameObject *object) {};
	//トランスフォームコンポーネント追加
	static void addTransform(GameObject *object) {object->AddComponent<TransformComponent>();}
	//ライトコンポーネント追加
	static void addLight(GameObject *object) { object->AddComponent<LightComponent>(); }
	//トランスフォームコンポーネント追加
	static void addColision(GameObject *object) { object->AddComponent<TransformComponent>(); }
	//トランスフォームコンポーネント追加
	static void addCamera(GameObject *object) { object->AddComponent<CameraComponent>(); }


	//コンポーネントグループ
	static std::map<ComponentList::Group, std::vector<std::string>> componentGroup;
	//コンポーネント名とAddComponentのセット
	static std::map<std::string, void(*)(GameObject*)> componentList;


};

