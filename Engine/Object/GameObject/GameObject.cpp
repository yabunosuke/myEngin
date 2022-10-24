
#include "GameObject.h"
#include "ImGui/imguiManager.h"
#include "BaseCollider.h"

#include "Object/Component/Collider/Collider.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Scene/Manager/GameObjectManager.h"


GameObjectManager *GameObject::game_object_manager_;


GameObject::GameObject(const std::string &name) :
	active_self_(true),
	isBlind(false)
{
	// 名前をセット
	this->name = name;
	tag_ = "Notag";
	
}

GameObject::~GameObject()
{

	// component削除
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component->game_object_ = nullptr;
		}
	}

	// 子のオブジェクトも破棄
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			child->pearent_game_object_ = nullptr;
		}
	}

	// 親がいる場合は親のリストから削除
	if (pearent_game_object_ != nullptr)
	{
		auto game_obj = pearent_game_object_->child_game_object_.begin();
		for (; game_obj != pearent_game_object_->child_game_object_.end(); ++game_obj)
		{
			if (*game_obj == this)
			{
				pearent_game_object_->child_game_object_.erase(game_obj);
				break;
			}
		}
	}

	// ゲームオブジェクトマネージャーから削除
	if (game_object_manager_->game_objects_.size() != 0)
	{
		auto game_obj = game_object_manager_->game_objects_.begin();
		for (; game_obj != game_object_manager_->game_objects_.end(); ++game_obj)
		{
			if (*game_obj == this)
			{
				game_object_manager_->game_objects_.erase(game_obj);
				break;
			}

		}
	}
}

GameObject* GameObject::CreateObject(const std::string &object_name)
{
	GameObject *game_object;
	// 名前が入っていなければ
	if (object_name.size() == 0) 
	{
		game_object = Object::CreateObject<GameObject>("GameObject(" + std::to_string(game_object_manager_->game_objects_.size()) + ")");
	}
	else 
	{
		game_object = Object::CreateObject<GameObject>(object_name);
	}
	// 生成時にトランスフォーム
	game_object->transform_ = game_object->AddComponent<Transform>();

	return game_object_manager_->add_objects_.emplace_back(game_object);
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
	
}

void GameObject::FixedUpdate()
{
	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component->CheckFixedUpdate();
	}
}

void GameObject::Update()
{
	// 親オブジェクトのアクティブ（親が存在しないときはfalse）
	bool parent_is_active = true;
	if (pearent_game_object_ != nullptr)
	{
		parent_is_active = pearent_game_object_->active_self_;
	}
	// 親オブジェクトが非アクティブなときと、自身が非アクティブなときは更新しない
	if (!parent_is_active || !active_self_) return;


	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	//アクティブでなければ描画しない
	if (!active_self_) return;
	for (auto &component : component_list_) {
		component->CheckLustUpdate();
	}

	//// コンポーネントの削除
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
	//非表示なら描画しない
	if (isBlind) return;
	//アクティブでなければ描画しない
	if (!active_self_) return;

	//全てのコンポーネントを描画
	for (auto &component : component_list_) {
		component->CheckDraw();
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
	ImGui::SameLine();
	// 静的フラグ
	ImGui::Checkbox("Static", &is_static_);
	if (ImGui::Button("Remove"))
	{
		Object::Destroy(this);
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

void GameObject::SetParent(GameObject* parent)
{
	// 子に親をセット
	pearent_game_object_ = parent;
	// 親に子をセット
	parent->child_game_object_.emplace_back(static_cast<GameObject*>(this));
	// 子のトランスフォームに親のトランスフォームをセットする
	transform_->parent_ = parent->GetComponent<Transform>();
}

void GameObject::AddCollider(Collider *collider)
{
	// タグ名で管理
	colliders_.emplace_back(collider);
}

void GameObject::RemoveCollider(Collider *collider)
{
	if (colliders_.size() == 0) return;
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

void GameObject::DestoryRelated()
{
	// component削除
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component->game_object_ = nullptr;
			Destroy(component);
		}
	}

	// 子のオブジェクトも破棄
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			child->pearent_game_object_ = nullptr;
			Destroy(child);
			child->DestoryRelated();
		}
	}
}
