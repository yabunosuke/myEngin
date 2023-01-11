#include "GameObject.h"
#include "ImGui/imguiManager.h"

#include "Object/Component/Collider/Collider.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "Object/Component/Transform/RectTransform.h"
#include "Scene/Manager/GameObjectManager.h"



#include <fstream>
#include <filesystem>
#include <sstream>

GameObjectManager *GameObject::game_object_manager_;



GameObject::GameObject(const std::string &name, const std::string &tag, bool is_2d)
	:Object(name)
{
	// ���O�������Ă��Ȃ����
	if (name.size() == 0)
	{
		this->name = "GameObject(" + std::to_string(game_object_manager_->game_objects_.size()) + ")";
	}

	active_self_ = true;
	is_blind_ = false;

	// �������Ƀg�����X�t�H�[��
	if (is_2d)
	{
		transform_ = AddComponent<RectTransform>();
	}
	else
	{
		transform_ = AddComponent<Transform>();
	}

}

GameObject::~GameObject()
{

	// component�폜
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component.lock()->game_object_.reset();
		}
	}

	// �q�̃I�u�W�F�N�g���j��
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			if (child.expired())
			{
				continue;
			}
			child.lock()->pearent_game_object_.reset();
		}
	}

	// �e������ꍇ�͐e�̃��X�g����폜
	if (!pearent_game_object_.expired())
	{
		auto game_obj = pearent_game_object_.lock()->child_game_object_.begin();
		for (; game_obj != pearent_game_object_.lock()->child_game_object_.end(); ++game_obj)
		{
			if (game_obj->expired())
			{
				continue;
			}
			if (game_obj->lock()->GetInstanceID()  == this->GetInstanceID())
			{
				pearent_game_object_.lock()->child_game_object_.erase(game_obj);
				break;
			}
		}
	}

	// �Q�[���I�u�W�F�N�g�}�l�[�W���[����폜
	if (game_object_manager_->game_objects_.size() != 0)
	{
		auto game_obj = game_object_manager_->game_objects_.begin();
		for (; game_obj != game_object_manager_->game_objects_.end(); ++game_obj)
		{
			if (game_obj->lock().get() == this)
			{
				game_object_manager_->game_objects_.erase(game_obj);
				break;
			}

		}
	}
}

void GameObject::SetGameObjectManager(GameObjectManager *game_object_manager)
{
	game_object_manager_ = game_object_manager;
}

std::weak_ptr<GameObject> GameObject::FindObject(const std::string &name)
{
	//for (const auto &object : game_object_manager_->game_objects_)
	//{
	//	if (object.lock()->name == name)
	//	{
	//		return object;
	//	}
	//}

	return std::weak_ptr<GameObject>();
}


bool GameObject::CompareTag(const std::string &tag)
{
	return tag_ == tag;
}

void GameObject::FixedUpdate()
{
	// �S�ẴR���|�[�l���g���X�V
	for (const auto &component : component_list_) {
		component.lock()->CheckFixedUpdate();
	}
}

void GameObject::Update()
{
	// �e�I�u�W�F�N�g�̃A�N�e�B�u�i�e�����݂��Ȃ��Ƃ���false�j
	bool parent_is_active = true;
	if (!pearent_game_object_.expired())
	{
		parent_is_active = pearent_game_object_.lock()->active_self_;
	}
	// �e�I�u�W�F�N�g����A�N�e�B�u�ȂƂ��ƁA���g����A�N�e�B�u�ȂƂ��͍X�V���Ȃ�
	if (!parent_is_active || !active_self_) return;


	// �S�ẴR���|�[�l���g���X�V
	for (const auto &component : component_list_) {
		component.lock()->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	////�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	//if (!active_self_) return;
	//for (auto &component : component_list_) {
	//	component.lock()->CheckLustUpdate();
	//}

	//// �R���|�[�l���g�̍폜
	//auto &itr = component_list_.begin();
	//while (itr != component_list_.end()) {
	//	if ((*itr)->GetIsRemove()) {
	//		delete *itr;
	//		itr = component_list_.erase(itr);
	//	}
	//	else {
	//		++itr;
	//	}
	//}
}

void GameObject::Draw()
{
	//��\���Ȃ�`�悵�Ȃ�
	if (is_blind_) return;
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!active_self_) return;

	//�S�ẴR���|�[�l���g��`��
	for (auto &component : component_list_) {
		component.lock()->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	if (ImGui::Button("TestOutPut"))
	{

		{
			// �o�C�i�������o��
			std::ofstream ofs("test.json", std::ios::binary);
			cereal::JSONOutputArchive serealization(ofs);
			serealization(cereal::make_nvp("root", *this));
		}
	}

	//�A�N�e�B�u�t���O
	ImGui::Checkbox("##Active", &active_self_); ImGui::SameLine();
	//���O�̕ύX�ƕ`��
	char buf[64] = "";
	sprintf_s(buf, name->c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name = buf;
	}
	ImGui::SameLine();
	// �ÓI�t���O
	ImGui::Checkbox("Static", &is_static_);
	if (ImGui::Button("Remove"))
	{
		Object::Destroy(this);
	}
	ImGui::Separator();

	//�R���|�[�l���g�̕\��
	int i = 0;
	for (auto &component : component_list_) {
		ImGui::PushID(i);
		component.lock()->ImGuiDraw();
		ImGui::Separator();
		ImGui::PopID();
		i++;
	}


}

void GameObject::SetParent(std::weak_ptr<GameObject> parent)
{
	// �q�ɐe���Z�b�g
	pearent_game_object_ = parent;
	// �e�Ɏq���Z�b�g
	parent.lock()->child_game_object_.emplace_back(std::dynamic_pointer_cast<GameObject>(weak_from_this().lock()));
	// �q�̃g�����X�t�H�[���ɐe�̃g�����X�t�H�[�����Z�b�g����
	transform->lock()->parent_ = parent.lock()->GetComponent<Transform>();

}
void GameObject::AddCollider(Collider *collider)
{
	//// �^�O���ŊǗ�
	//colliders_.emplace_back(collider);
}

void GameObject::RemoveCollider(Collider *collider)
{
	/*if (colliders_.size() == 0) return;
	for (auto &col = colliders_.begin(); col != colliders_.end();++col) {
		if (*col == collider) {
			colliders_.erase(col);
			break;
		}
	}*/

}

void GameObject::AddMonoBehaviour(MonoBehaviour *monobehaviour)
{
	/*monobehaviour->Awake();
	mono_behaviours_.emplace_back(monobehaviour);*/
}

const std::vector<MonoBehaviour *>& GameObject::GetMonoBehaviours()
{
	return mono_behaviours_;
}

void GameObject::RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour)
{
}

void GameObject::DestoryRelated()
{
	// component�폜
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component.lock()->game_object_.reset();
			Destroy(component.lock().get());
		}
	}

	//// �q�̃I�u�W�F�N�g���j��
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			child.lock()->pearent_game_object_.reset();
			Destroy(child.lock().get());
			child.lock()->DestoryRelated();
		}
	}
}
