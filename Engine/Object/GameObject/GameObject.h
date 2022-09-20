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

// コライダー
class Collider;
class MonoBehaviour;
class GameObjectManager;

class GameObject :
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

	//===========================================
	//
	//		静的メンバ関数
	//
	//===========================================


	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	static std::weak_ptr<GameObject> CreateObject(const std::string &object_name = "");

	static void SetGameObjectManager(std::weak_ptr<GameObjectManager> game_object_manager);
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
	inline std::weak_ptr<T> AddComponent(Args ...args);
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
	std::weak_ptr<T> GetComponent();

	/// <summary>
	/// コンポーネントの取得 調整中
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();



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

	


	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// 削除
	void Remove() { isRemove = true; }

	// 親オブジェクトの取得
	void SetPearentObject(std::weak_ptr<GameObject> pearent);
	std::weak_ptr<GameObject> &GetPearent() { return pearent_game_object_; }
	// 子のオブジェクトコンテナの取得
	void SetCildrenObject(std::weak_ptr<GameObject> children) { child_game_object_.emplace_back(children); }
	std::vector<std::weak_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// コンポーネントリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<std::weak_ptr<Component>> GetComponentList() { return component_list_; }


	// コライダー
	void AddCollider(std::weak_ptr<Collider> collider);
	const std::vector<std::weak_ptr<Collider>> &GetColliders() { return colliders_; }
	void RemoveCollider(std::weak_ptr<Collider> collider);

	// MonoBehaviour
	void AddMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	const std::vector<std::weak_ptr<MonoBehaviour>> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);


	/// <summary>
	/// 所属しているシーン (get = true, set = false)
	/// </summary>
	//Property<std::weak_ptr<AbstractScene>> scene{ scene_ ,AccessorType::ReadOnly };

	
	/// <summary>
	/// オブジェクト識別タグ (get = true, set = true)
	/// </summary>
	yEngine::Property<std::string> tag{ tag_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// ローカルのアクティブ状態 (get = true, set = false)
	/// </summary>
	yEngine::Property<bool> activeSelf{ active_self_ ,yEngine::AccessorType::ReadOnly };
	/// <summary>
	/// ローカルのアクティブ状態 (get = true, set = false)
	/// </summary>
	yEngine::Property<std::weak_ptr<Transform>> transform{ transform_,yEngine::AccessorType::AllAccess };


private:
	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	// ゲームオブジェクトマネージャーのポインタ
	static std::weak_ptr<GameObjectManager> game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// オブジェクトコンテナ

	//===========================================
	//
	//		メンバ変数
	//
	//===========================================




	std::weak_ptr<Transform> transform_;

	// 属しているシーン
	//std::weak_ptr<AbstractScene> scene_;
	// ゲームオブジェクトのタグ
	std::string tag_ = "Notag";

	// 親オブジェクト
	std::weak_ptr<GameObject> pearent_game_object_;
	// 子オブジェクトのコンテナ
	std::vector<std::weak_ptr<GameObject>> child_game_object_;
	// コンポーネント
	std::list<std::weak_ptr<Component>> component_list_;
	// コライダー
	std::vector<std::weak_ptr<Collider>> colliders_;
	// MonoBehaviour
	std::vector<std::weak_ptr<MonoBehaviour>> mono_behaviours_;

	bool active_self_;				// ローカルのアクティブ状態
	// 非表示になっているか
	bool isBlind;
	// 削除するか
	bool isRemove;

	


};

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddComponent(Args ...args)
{

	std::weak_ptr<T> temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp.lock()->transform_ = GetComponent<Transform>();
	temp.lock()->game_object_ = std::static_pointer_cast<GameObject>(shared_from_this());
	temp.lock()->CheckInitialize();
	// コンポーネントを更新順に並び替え
	component_list_.sort(
		[](std::weak_ptr<Component> lhs,std::weak_ptr<Component> rhs)
		{
			return static_cast<int>(lhs.lock()->type.r_) < static_cast<int>(rhs.lock()->type.r_);
		}
	);


	return temp;
}

template<class T>
inline std::weak_ptr<T> GameObject::GetComponent()
{
	std::weak_ptr<T> temp;
	for (auto &component : component_list_)
	{
		temp = std::dynamic_pointer_cast<T>(component.lock());
		if (!temp.expired())
		{
			return temp;
		}
	}

	return temp;
}

template<class T>
inline std::vector<std::weak_ptr<T>> GameObject::GetComponents()
{
	std::vector<std::weak_ptr<T>> components;
	return std::weak_ptr<T>();
}
