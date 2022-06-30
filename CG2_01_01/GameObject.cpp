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
	// 親オブジェクトのアクティブ（親が存在しないときはfalse）
	bool pealentIsActive = !(pearentGameObject.lock().get() && !pearentGameObject.lock().get()->isActive);
	// 親オブジェクトが非アクティブなときと、自身が非アクティブなときは更新しない
	if (!pealentIsActive || !isActive) return;

	// コンポーネントの削除
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

	// 全てのコンポーネントを更新
	for (auto &component : componentList) {
		component->Update();
	}
}

void GameObject::Draw()
{
	//非表示なら描画しない
	if (isBlind) return;
	//アクティブでなければ描画しない
	if (!isActive) return;

	//全てのコンポーネントを描画
	for (auto &component : componentList) {
		component->Draw();
	}
}

void GameObject::DrawInspector()
{
	//アクティブフラグ
	ImGui::Checkbox("##Active", &isActive); ImGui::SameLine();
	//名前の変更と描画
	char buf[64] = "";
	sprintf_s(buf, name.c_str());
	if (ImGui::InputText("##", buf, 64,ImGuiInputTextFlags_EnterReturnsTrue)) {
		name = buf;
	}
	ImGui::Separator();

	//コンポーネントの表示
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
