
#include "GameObject.h"
#include "ImGui/imguiManager.h"
#include "BaseCollider.h"




GameObject::GameObject(const std::string &name) :
	active_self_(true),
	isBlind(false),
	isRemove(false)
{
	// ���O���Z�b�g
	this->name = name;
	tag_ = "Notag";
	
}


bool GameObject::CompareTag(const std::string &tag)
{
	return tag_ == tag;
}


void GameObject::SetActive(bool value)
{
}
void GameObject::SetPearentObject(std::weak_ptr<GameObject> pearent) {
	pearent_game_object_ = pearent;
	transform_.lock()->parent_ = pearent.lock().get()->GetComponent<Transform>();
}


void GameObject::Initialize()
{
	scripts_.clear();
	for (auto component : component_list_) {
		if (dynamic_cast<ScriptComponent *>(component.lock().get())) {
			scripts_.emplace_back(component);
		}
	}
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
	bool parent_is_active = !(pearent_game_object_.lock().get() && !pearent_game_object_.lock().get()->active_self_);
	// �e�I�u�W�F�N�g����A�N�e�B�u�ȂƂ��ƁA���g����A�N�e�B�u�ȂƂ��͍X�V���Ȃ�
	if (!parent_is_active || !active_self_) return;

	// �R���|�[�l���g�̍폜
	auto &itr = component_list_.begin();
	while (itr != component_list_.end()) {
		if ((*itr).lock()->GetIsRemove()) {
			itr->reset();
			itr = component_list_.erase(itr);
		}
		else {
			++itr;
		}
	}

	// �S�ẴR���|�[�l���g���X�V
	for (const auto &component : component_list_) {
		component.lock()->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!active_self_) return;
	for (auto &component : component_list_) {
		component.lock()->CheckLustUpdate();
	}
}

void GameObject::Draw()
{
	//��\���Ȃ�`�悵�Ȃ�
	if (isBlind) return;
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!active_self_) return;

	//�S�ẴR���|�[�l���g��`��
	for (auto &component : component_list_) {
		component.lock()->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	//�A�N�e�B�u�t���O
	ImGui::Checkbox("##Active", &active_self_); ImGui::SameLine();
	//���O�̕ύX�ƕ`��
	char buf[64] = "";
	sprintf_s(buf, name->c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name = buf;
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
