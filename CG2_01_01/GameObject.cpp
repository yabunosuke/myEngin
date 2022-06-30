#include "GameObject.h"
#include "TransformComponent.h"
#include "imguiManager.h"
#include "LightComponent.h"

uint64_t GameObject::ID = 0;


GameObject::GameObject(std::string name) :
	isActive(true),
	isBlind(false),
	isRemove(false),
	name(name),
	id(++ID)
{
	std::shared_ptr<TransformComponent> t = std::make_shared<TransformComponent>();
	AddComponent(t);
}

void GameObject::Initialize()
{
}

void GameObject::Update()
{
	// �e�I�u�W�F�N�g�̃A�N�e�B�u�i�e�����݂��Ȃ��Ƃ���false�j
	bool pealentIsActive = !(pearentGameObject.lock().get() && !pearentGameObject.lock().get()->isActive);
	// �e�I�u�W�F�N�g����A�N�e�B�u�ȂƂ��ƁA���g����A�N�e�B�u�ȂƂ��͍X�V���Ȃ�
	if (!pealentIsActive || !isActive) return;

	// �R���|�[�l���g�̍폜
	auto &itr = componentList.begin();
	while (itr != componentList.end()) {
		if ((*itr)->GetIsRemove()) {
			itr->reset();
			itr = componentList.erase(itr);
		}
		else {
			itr++;
		}
	}

	// �S�ẴR���|�[�l���g���X�V
	for (auto &component : componentList) {
		component->Update();
	}
}

void GameObject::Draw()
{
	//��\���Ȃ�`�悵�Ȃ�
	if (isBlind) return;
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!isActive) return;

	//�S�ẴR���|�[�l���g��`��
	for (auto &component : componentList) {
		component->Draw();
	}
}

void GameObject::DrawInspector()
{
	//�A�N�e�B�u�t���O
	ImGui::Checkbox("##Active", &isActive); ImGui::SameLine();
	//���O�̕ύX�ƕ`��
	char buf[64] = "";
	sprintf_s(buf, name.c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name = buf;
	}
	ImGui::Separator();

	//�R���|�[�l���g�̕\��
	int i = 0;
	for (auto &component : componentList) {
		ImGui::PushID(i);
		component.get()->ImGuiDraw();
		ImGui::Separator();
		ImGui::PopID();
		i++;
	}


}

void GameObject::AddComponent(const std::shared_ptr<Component> &component)
{
	componentList.emplace_back(component);
}
