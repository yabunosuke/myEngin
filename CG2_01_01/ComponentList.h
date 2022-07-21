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
private:	//�V���O���g���p�^�[��
	ComponentList() = default;
	~ComponentList() = default;
	ComponentList(const ComponentList &r) = default;
	ComponentList &operator= (const ComponentList &r) = default;

public:
	//�R���|�[�l���g�̎��
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
	//�C���X�^���X�̎擾
	static ComponentList *GetIns();

	static void Initialize();
	
	std::map<ComponentList::Group, std::vector<std::string>> &GetComponentGroup() { return componentGroup; }
	std::map<std::string, void(*)(GameObject *)> &GetComponentList() { return componentList; }
private:
	static void addDummy(GameObject *object) {};
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	static void addTransform(GameObject *object) {object->AddComponent<TransformComponent>();}
	//���C�g�R���|�[�l���g�ǉ�
	static void addLight(GameObject *object) { object->AddComponent<LightComponent>(); }
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	static void addColision(GameObject *object) { object->AddComponent<TransformComponent>(); }
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	static void addCamera(GameObject *object) { object->AddComponent<CameraComponent>(); }


	//�R���|�[�l���g�O���[�v
	static std::map<ComponentList::Group, std::vector<std::string>> componentGroup;
	//�R���|�[�l���g����AddComponent�̃Z�b�g
	static std::map<std::string, void(*)(GameObject*)> componentList;


};

