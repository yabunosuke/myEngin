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
	// 親オブジェクトのアクティブ（親が存在しないときはfalse）
	bool pealentIsActive = !(pearent_game_object_.lock().get() && !pearent_game_object_.lock().get()->isActive);
	// 親オブジェクトが非アクティブなときと、自身が非アクティブなときは更新しない
	if (!pealentIsActive || !isActive) return;

	// コンポーネントの削除
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

	// 全てのコンポーネントを更新
	for (auto &component : component_list_) {
		component->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//アクティブでなければ描画しない
	if (!isActive) return;
	for (auto &component : component_list_) {
		component->CheckLustUpdate();
	}
}

void GameObject::Draw()
{
	//非表示なら描画しない
	if (isBlind) return;
	//アクティブでなければ描画しない
	if (!isActive) return;

	//全てのコンポーネントを描画
	for (auto &component : component_list_) {
		component->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	//アクティブフラグ
	ImGui::Checkbox("##Active", &isActive); ImGui::SameLine();
	//名前の変更と描画
	char buf[64] = "";
	sprintf_s(buf, name_.c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name_ = buf;
	}
	ImGui::Separator();

	//コンポーネントの表示
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
