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

private:	//�V���O���g���p�^�[��
	ComponentList() = default;
	~ComponentList() = default;
	ComponentList(const ComponentList &r) = default;
	ComponentList &operator= (const ComponentList &r) = default;

	// �G�C���A�X

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
	
	std::map<ComponentList::Group, std::vector<std::string>> &GetComponentGroup() { return component_group_; }
	std::map<std::string, void(*)(GameObject *)> &GetComponentList() { return components_; }
private:
	static void ComponentGroupSet(Group group, std::string name);


	void addDummy(GameObject *object) {};
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	void addTransform(GameObject *object) {object->AddComponent<Transform>();}
	//���C�g�R���|�[�l���g�ǉ�
	//void addLight(GameObject *object) { object->AddComponent<LightComponent>(); }
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	void addColision(GameObject *object) { object->AddComponent<Transform>(); }
	//�g�����X�t�H�[���R���|�[�l���g�ǉ�
	void addCamera(GameObject *object) { object->AddComponent<CameraComponent>(); }


	//�R���|�[�l���g�O���[�v
	static std::map<ComponentList::Group, std::vector<std::string>> component_group_;
	//�R���|�[�l���g����AddComponent�̃Z�b�g
	static std::map<std::string, void(*)(GameObject*)> components_;

	// �I�u�W�F�N�g�̃|�C���^
	
};

