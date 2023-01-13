#include "GameObject.h"
#include "ImGui/imguiManager.h"

#include "Object/Component/Collider/Collider.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "Object/Component/Transform/RectTransform.h"
#include "Scene/Manager/GameObjectManager.h"



GameObjectManager *GameObject::game_object_manager_;



GameObject::GameObject(const std::string &name, const std::string &tagk)
	:Object(name)
{
	// 名前が入っていなければ
	if (name.size() == 0)
	{
		this->name = "GameObject(" + std::to_string(game_object_manager_->game_objects_.size()) + ")";
	}

	active_self_ = true;
	is_blind_ = false;

}

GameObject::~GameObject()
{

	// component削除
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component.lock()->game_object_.reset();
		}
	}

	// 子のオブジェクトも破棄
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			if (child.expired())
			{
				continue;
			}
			child.lock()->parent_game_object_.reset();
		}
	}

	// 親がいる場合は親のリストから削除
	if (!parent_game_object_.expired())
	{
		auto game_obj = parent_game_object_.lock()->child_game_object_.begin();
		for (; game_obj != parent_game_object_.lock()->child_game_object_.end(); ++game_obj)
		{
			if (game_obj->expired())
			{
				continue;
			}
			if (game_obj->lock()->GetInstanceID()  == this->GetInstanceID())
			{
				parent_game_object_.lock()->child_game_object_.erase(game_obj);
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
			if (game_obj->lock().get() == this)
			{
				game_object_manager_->game_objects_.erase(game_obj);
				break;
			}

		}
	}
}

std::weak_ptr<GameObject> GameObject::AddGameObjet(const std::string &name)
{
	std::weak_ptr<GameObject> temp = Object::CreateObject<GameObject>(name);
	game_object_manager_->add_objects_.emplace_back(temp);


	// 生成時にトランスフォーム
	if (false)
	{
		temp.lock()->transform_ = temp.lock()->AddComponent<RectTransform>();
	}
	else
	{
		temp.lock()->transform_ = temp.lock()->AddComponent<Transform>();
	}

	temp.lock()->transform_.lock()->game_object_ = temp;



	return temp;
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
	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component.lock()->CheckFixedUpdate();
	}
}

void GameObject::Update()
{
	// 親オブジェクトのアクティブ（親が存在しないときはfalse）
	bool parent_is_active = true;
	if (!parent_game_object_.expired())
	{
		parent_is_active = parent_game_object_.lock()->active_self_;
	}
	// 親オブジェクトが非アクティブなときと、自身が非アクティブなときは更新しない
	if (!parent_is_active || !active_self_) return;


	// 全てのコンポーネントを更新
	for (const auto &component : component_list_) {
		component.lock()->CheckUpdate();
	}
}

void GameObject::LastUpdate()
{
	////アクティブでなければ描画しない
	//if (!active_self_) return;
	//for (auto &component : component_list_) {
	//	component.lock()->CheckLustUpdate();
	//}

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
	if (is_blind_) return;
	//アクティブでなければ描画しない
	if (!active_self_) return;

	//全てのコンポーネントを描画
	for (auto &component : component_list_) {
		component.lock()->CheckDraw();
	}
}

void GameObject::DrawInspector()
{
	if (ImGui::Button("TestOutPut"))
	{
		Singleton<ObjectManager>::GetInstance().Save();
	}

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
		component.lock()->ImGuiDraw();
		ImGui::Separator();
		ImGui::PopID();
		i++;
	}


}

void GameObject::SetParent(std::weak_ptr<GameObject> parent)
{
	// 子に親をセット
	parent_game_object_ = parent;
	// 親に子をセット
	parent.lock()->child_game_object_.emplace_back(std::dynamic_pointer_cast<GameObject>(weak_from_this().lock()));
	// 子のトランスフォームに親のトランスフォームをセットする
	transform->lock()->parent_ = parent.lock()->GetComponent<Transform>();

}
void GameObject::AddCollider(Collider *collider)
{
	//// タグ名で管理
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
	// component削除
	if (component_list_.size() != 0)
	{
		for (auto &component : component_list_)
		{
			component.lock()->game_object_.reset();
			Destroy(component.lock().get());
		}
	}

	//// 子のオブジェクトも破棄
	if (child_game_object_.size() != 0)
	{
		for (auto &child : child_game_object_)
		{
			child.lock()->parent_game_object_.reset();
			Destroy(child.lock().get());
			child.lock()->DestoryRelated();
		}
	}
}
