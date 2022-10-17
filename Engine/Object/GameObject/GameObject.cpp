
#include "GameObject.h"
#include "ImGui/imguiManager.h"
#include "BaseCollider.h"

#include "Object/Component/Collider/Collider.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "GameObjectManager.h"


GameObjectManager *GameObject::game_object_manager_;


GameObject::GameObject(const std::string &name) :
	active_self_(true),
	isBlind(false),
	isRemove(false)
{
	// ���O���Z�b�g
	this->name = name;
	tag_ = "Notag";
	
}

GameObject::~GameObject()
{
}

GameObject* GameObject::CreateObject(const std::string &object_name)
{
	GameObject *game_object;
	// ���O�������Ă��Ȃ����
	if (object_name.size() == 0) 
	{
		game_object = Object::CreateObject<GameObject>("GameObject(" + std::to_string(game_object_manager_->game_objects_.size()) + ")");
	}
	else 
	{
		game_object = Object::CreateObject<GameObject>(object_name);
	}
	// �������Ƀg�����X�t�H�[��
	game_object->transform_ = game_object->AddComponent<Transform>();

	return game_object_manager_->game_objects_.emplace_back(game_object);
}

void GameObject::SetGameObjectManager(GameObjectManager *game_object_manager)
{
	game_object_manager_ = game_object_manager;
}


bool GameObject::CompareTag(const std::string &tag)
{
	return tag_ == tag;
}



void GameObject::Initialize()
{
	/*scripts_.clear();
	for (auto component : component_list_) {
		if (dynamic_cast<ScriptComponent *>(component.lock().get())) {
			scripts_.emplace_back(component);
		}
	}*/
}

void GameObject::FixedUpdate()
{
	// �S�ẴR���|�[�l���g���X�V
	for (const auto &component : component_list_) {
		component->CheckFixedUpdate();
	}
}

void GameObject::Update()
{
	// �e�I�u�W�F�N�g�̃A�N�e�B�u�i�e�����݂��Ȃ��Ƃ���false�j
	bool parent_is_active = true;
	if (pearent_game_object_ != nullptr)
	{
		parent_is_active = pearent_game_object_->active_self_;
	}
	// �e�I�u�W�F�N�g����A�N�e�B�u�ȂƂ��ƁA���g����A�N�e�B�u�ȂƂ��͍X�V���Ȃ�
	if (!parent_is_active || !active_self_) return;

	// �R���|�[�l���g�̍폜
	auto &itr = component_list_.begin();
	while (itr != component_list_.end()) {
		if ((*itr)->GetIsRemove()) {
			delete *itr;
			itr = component_list_.erase(itr);
		}
		else {
			++itr;
		}
	}

	// �S�ẴR���|�[�l���g���X�V
	for (const auto &component : component_list_) {
		component->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//�A�N�e�B�u�łȂ���Ε`�悵�Ȃ�
	if (!active_self_) return;
	for (auto &component : component_list_) {
		component->CheckLustUpdate();
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
		component->CheckDraw();
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
	ImGui::SameLine();
	// �ÓI�t���O
	ImGui::Checkbox("static", &is_static_);

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

void GameObject::SetParent(GameObject* parent)
{
	// �q�ɐe���Z�b�g
	pearent_game_object_ = parent;
	// �e�Ɏq���Z�b�g
	parent->child_game_object_.emplace_back(static_cast<GameObject*>(this));
	// �q�̃g�����X�t�H�[���ɐe�̃g�����X�t�H�[�����Z�b�g����
	transform_->parent_ = parent->GetComponent<Transform>();
}

void GameObject::AddCollider(Collider *collider)
{
	// �^�O���ŊǗ�
	colliders_.emplace_back(collider);
}

void GameObject::RemoveCollider(Collider *collider)
{
	for (auto &col = colliders_.begin(); col != colliders_.end();++col) {
		if (*col == collider) {
			colliders_.erase(col);
			break;
		}
	}

}

void GameObject::AddMonoBehaviour(MonoBehaviour *monobehaviour)
{
	mono_behaviours_.emplace_back(monobehaviour);
}

const std::vector<MonoBehaviour *>& GameObject::GetMonoBehaviours()
{
	return mono_behaviours_;
}

void GameObject::RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour)
{
}
