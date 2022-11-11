#pragma once

// 参考
// https://docs.unity3d.com/ja/2018.4/ScriptReference/GameObject.html

#include <list>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <map>


// 基底クラス
#include "Object/Object.h"
#include "Object/Component/Transform.h"


//コンポーネント基底クラス
#include "Object/Component/Component.h"

#include "Property.h"

class Collider;
class MonoBehaviour;
class GameObjectManager;

class GameObject final :
public Object
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//関数

	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	GameObject(const std::string &name);
	~GameObject() override;
	//===========================================
	//
	//		静的メンバ関数
	//
	//===========================================

	static GameObject *CreateObject(const std::string &object_name = "");

	static void SetGameObjectManager(GameObjectManager *game_object_manager);

	/// <summary>
	/// オブジェクトを名前で検索して返す。
	/// </summary>
	/// <param name="name">検索するオブジェクト名</param>
	/// <returns>発見したオブジェクトのポインタ。
	/// 見つからなかった場合はnullptrを返す。</returns>
	static GameObject *Find(const std::string &name);

	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	//===========================================
	//
	//		メンバ関数
	//
	//===========================================
	
	/// <summary>
	/// クラス名
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<class T, class... Args>
	inline T *AddComponent(Args ...args);
	/// <summary>
	/// タグ名の比較
	/// </summary>
	/// <param name="tag"確認したいタグ></param>
	/// <returns>bool タグが一致したかどうか</returns>
	bool CompareTag(const std::string &tag);

	/// <summary>
	/// コンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	T *GetComponent();

	/// <summary>
	/// コンポーネントの取得 調整中
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	/*template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();*/


	//初期化
	void Initialize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void FixedUpdate();
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void LastUpdate();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 
	/// </summary>
	void DrawInspector();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() {};

	void SetParent(GameObject *parent);

	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// 親オブジェクトの取得
	GameObject* GetPearent() { return pearent_game_object_; }
	// 子のオブジェクトコンテナの取得
	std::vector<GameObject*> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// コンポーネントリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<Component*> &GetComponentList() { return component_list_; }


	// コライダー
	void AddCollider(Collider *collider);
	const std::vector<Collider*> &GetColliders() { return colliders_; }
	void RemoveCollider(Collider *collider);

	// MonoBehaviour
	void AddMonoBehaviour(MonoBehaviour *monobehaviour);
	const std::vector<MonoBehaviour *> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	
	/// <summary>
	/// 一番上の親を取得する( ReadOnly )
	/// </summary>
	yEngine::Property<GameObject *> top
	{
		nullptr, yEngine::AccessorType::ReadOnly,
		[this]()
		{

			std::function<GameObject *(GameObject *)> parent_acquisition{
				[&](GameObject *object)
				{
					auto pearent = object->pearent_game_object_;
					if (pearent != nullptr)
					{
						parent_acquisition(pearent);
					}
					else
					{
						return object;
					}
				} 
			};
			return parent_acquisition(this);
		},
		nullptr
	};

	/// <summary>
	/// オブジェクト識別タグ (get = true, set = true)
	/// </summary>
	yEngine::Property<std::string> tag{ &tag_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// ローカルのアクティブ状態 (get = true, set = false)
	/// </summary>
	yEngine::Property<bool> activeSelf{ &active_self_ ,yEngine::AccessorType::ReadOnly };
	/// <summary>
	/// オブジェクトが動かない物ならtrue (get = true, set = true)
	/// </summary>
	yEngine::Property<bool> isStatic{ &is_static_ ,yEngine::AccessorType::AllAccess };


	Transform *transform_;

	uint16_t layer_{0};

private:
	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	// ゲームオブジェクトマネージャーのポインタ
	static GameObjectManager *game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// オブジェクトコンテナ

	static std::vector<GameObject*> add_object_list_;


	//===========================================
	//
	//		メンバ関数
	//
	//===========================================

	void DestoryRelated() override;

	//===========================================
	//
	//		メンバ変数
	//
	//===========================================

	bool is_static_ = false;



	// 属しているシーン
	//std::weak_ptr<AbstractScene> scene_;
	// ゲームオブジェクトのタグ
	std::string tag_ = "Notag";

	// 親オブジェクト
	GameObject *pearent_game_object_{ nullptr };
	// 子オブジェクトのコンテナ
	std::vector<GameObject*> child_game_object_;
	// コンポーネント
	std::list<Component*> component_list_;
	// コライダー
	std::vector<Collider*> colliders_;
	// MonoBehaviour
	std::vector<MonoBehaviour*> mono_behaviours_;

	bool active_self_{true};				// ローカルのアクティブ状態
	// 非表示になっているか
	bool isBlind;
};

template<class T, class ...Args>
inline T *GameObject::AddComponent(Args ...args)
{
	T *temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp->transform_ = GetComponent<Transform>();
	temp->game_object_ = this;
	temp->CheckInitialize();
	// コンポーネントを更新順に並び替え
	component_list_.sort(
		[](Component *lhs, Component *rhs)
		{
			return static_cast<int>(lhs->type.r_) < static_cast<int>(rhs->type.r_);
		}
	);

	return temp;
}

template<class T>
T *GameObject::GetComponent()
{
	T *temp = nullptr;
	for (auto component : component_list_)
	{
		temp = dynamic_cast<T*>(component);
		if (temp != nullptr)
		{
			return temp;
		}
	}

	return temp;
}

//template<class T>
//inline std::vector<std::weak_ptr<T>> GameObject::GetComponents()
//{
//	std::vector<std::weak_ptr<T>> components;
//	return std::weak_ptr<T>();
//}
