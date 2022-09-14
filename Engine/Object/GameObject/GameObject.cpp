
#include "GameObject.h"
#include "ImGui/imguiManager.h"
#include "BaseCollider.h"




GameObject::GameObject(const std::string &name) :
	active_self_(true),
	isBlind(false),
	isRemove(false)
{
	// 名前をセット
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
	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component.lock()->CheckFixedUpdate();
	}
}

void GameObject::Update()
{
	// 親オブジェクトのアクティブ（親が存在しないときはfalse）
	bool parent_is_active = !(pearent_game_object_.lock().get() && !pearent_game_object_.lock().get()->active_self_);
	// 親オブジェクトが非アクティブなときと、自身が非アクティブなときは更新しない
	if (!parent_is_active || !active_self_) return;

	// コンポーネントの削除
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

	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component.lock()->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//アクティブでなければ描画しない
	if (!active_self_) return;
	for (auto &component : component_list_) {
		component.lock()->CheckLustUpdate();
	}
}

void GameObject::Draw()
{
	//非表示なら描画しない
	if (isBlind) return;
	//アクティブでなければ描画しない
	if (!active_self_) return;

	//全てのコンポーネントを描画
	for (auto &component : component_list_) {
		component.lock()->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	//アクティブフラグ
	ImGui::Checkbox("##Active", &active_self_); ImGui::SameLine();
	//名前の変更と描画
	char buf[64] = "";
	sprintf_s(buf, name->c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name = buf;
	}
	ImGui::Separator();

	//コンポーネントの表示
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
	// タグ名で管理
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
