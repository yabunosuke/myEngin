#include "GameObject.h"
#include "TransformComponent.h"
#include "imguiManager.h"
#include "BaseCollider.h"

uint64_t GameObject::ID = 0;


GameObject::GameObject(std::string name) :
	isActive(true),
	isBlind(false),
	isRemove(false),
	name_(name),
	id_(++ID)
{
	//std::shared_ptr<TransformComponent> t = std::make_shared<TransformComponent>();
	//AddComponent(t);
	AddComponent<TransformComponent>();
}

void GameObject::Initialize()
{
}

void GameObject::Update()
{
	// �e�I�u�W�F�N�g�̃A�N�e�B�u�i�e�����݂��Ȃ��Ƃ���false�j
	bool pealentIsActive = !(pearent_game_object_.lock().get() && !pearent_game_object_.lock().get()->isActive);
	// �e�I�u�W�F�N�g����A�N�e�B�u�ȂƂ��ƁA���g����A�N�e�B�u�ȂƂ��͍X�V���Ȃ�
	if (!pealentIsActive || !isActive) return;

	// �R���|�[�l���g�̍폜
	auto &itr = component_list_.begin();
	while (itr != component_list_.end()) {
		if ((*itr)->GetIsRemove()) {
			itr->reset();
			itr = component_list_.erase(itr);
		}
		else {
			itr++;
		}
	}

	// �S�ẴR���|�[�l���g���X�V
	for (auto &component : component_list_) {
		component->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!isActive) return;
	for (auto &component : component_list_) {
		component->CheckLustUpdate();
	}
}

void GameObject::Draw()
{
	//��\���Ȃ�`�悵�Ȃ�
	if (isBlind) return;
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!isActive) return;

	//�S�ẴR���|�[�l���g��`��
	for (auto &component : component_list_) {
		component->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	//�A�N�e�B�u�t���O
	ImGui::Checkbox("##Active", &isActive); ImGui::SameLine();
	//���O�̕ύX�ƕ`��
	char buf[64] = "";
	sprintf_s(buf, name_.c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name_ = buf;
	}
	ImGui::Separator();

	//�R���|�[�l���g�̕\��
	int i = 0;
	for (auto &component : component_list_) {
		ImGui::PushID(i);
		component->ImGuiDraw();
		ImGui::Separator();
		ImGui::PopID();
		i++;
	}


}

void GameObject::AddCollider(std::weak_ptr<BaseCollider> collider)
{
	// �^�O���ŊǗ�
	colliders_.emplace_back(collider);
}

void GameObject::RemoveCollider(std::weak_ptr<BaseCollider> collider)
{
	for (auto col = colliders_.begin(); col != colliders_.end();++col) {
		if (col->lock() = collider.lock()) {
			colliders_.erase(col);
			break;
		}
	}

}
