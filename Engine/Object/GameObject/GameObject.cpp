
#include "GameObject.h"
#include "ImGui/imguiManager.h"
#include "BaseCollider.h"

#include "Object/Component/Collider/Collider.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "GameObjectManager.h"


std::weak_ptr<GameObjectManager> GameObject::game_object_manager_;


GameObject::GameObject(const std::string &name) :
	active_self_(true),
	isBlind(false),
	isRemove(false)
{
	// 名前をセット
	this->name = name;
	tag_ = "Notag";
	
}

std::weak_ptr<GameObject> GameObject::CreateObject(const std::string &object_name)
{
	std::weak_ptr<GameObject> game_object;
	// 名前が入っていなければ
	if (object_name.size() == 0) {
		game_object = Object::CreateObject<GameObject>("GameObject(" + std::to_string(game_object_manager_.lock()->game_objects_.size()) + ")");
	}
	else {
		game_object = Object::CreateObject<GameObject>(object_name);
	}
	// 生成時にトランスフォーム
	game_object.lock()->transform = game_object.lock()->AddComponent<Transform>();

	return game_object_manager_.lock()->game_objects_.emplace_back(game_object);
}

void GameObject::SetGameObjectManager(std::weak_ptr<GameObjectManager> game_object_manager)
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

void GameObject::SetParent(std::weak_ptr<GameObject> parent)
{
	// 子に親をセット
	pearent_game_object_ = parent;
	// 親に子をセット
	parent.lock()->child_game_object_.emplace_back(std::static_pointer_cast<GameObject>((weak_from_this().lock())));
	// 子のトランスフォームに親のトランスフォームをセットする
	transform_.lock()->parent_ = parent.lock().get()->GetComponent<Transform>();
}

void GameObject::AddCollider(std::weak_ptr<Collider> collider)
{
	// タグ名で管理
	colliders_.emplace_back(collider);
}

void GameObject::RemoveCollider(std::weak_ptr<Collider> collider)
{
	for (auto col = colliders_.begin(); col != colliders_.end();++col) {
		if (col->lock() = collider.lock()) {
			colliders_.erase(col);
			break;
		}
	}

}

void GameObject::AddMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour)
{
	mono_behaviours_.emplace_back(monobehaviour);
}

const std::vector<std::weak_ptr<MonoBehaviour>>& GameObject::GetMonoBehaviours()
{
	return mono_behaviours_;
}

void GameObject::RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour)
{
}
