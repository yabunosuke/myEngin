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

public:

	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================
	GameObject(const std::string &name = "", const std::string &tag = "", bool is_2d = false);
	~GameObject() override;

	//===========================================
	//
	//		静的メンバ関数
	//
	//===========================================

	static void SetGameObjectManager(GameObjectManager *game_object_manager);

	/// <summary>
	/// オブジェクトを名前で検索して返す。
	/// </summary>
	/// <param name="name">検索するオブジェクト名</param>
	/// <returns>発見したオブジェクトのポインタ。
	/// 見つからなかった場合はnullptrを返す。</returns>
	static std::weak_ptr<GameObject> FindObject(const std::string &name);

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
	/*template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();*/

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

	void SetParent(std::weak_ptr<GameObject> parent);

	// isBlind
	void SetIsBlind(bool blind) { is_blind_ = blind; }
	bool GetIsBlind() { return is_blind_; }

	// 親オブジェクトの取得
	std::weak_ptr<GameObject> GetPearent() { return pearent_game_object_; }
	// 子のオブジェクトコンテナの取得
	std::vector<std::weak_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// コンポーネントリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<std::weak_ptr<Component>> &GetComponentList() { return component_list_; }


	// コライダー
	void AddCollider(Collider *collider);
	const std::vector<Collider*> &GetColliders() { return colliders_; }
	void RemoveCollider(Collider *collider);

	// MonoBehaviour
	void AddMonoBehaviour(MonoBehaviour *monobehaviour);
	const std::vector<MonoBehaviour *> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	
	
	yEngine::Property<std::weak_ptr<GameObject>> top
	{
		nullptr, yEngine::AccessorType::ReadOnly,
		[this]()
		{

			std::function<std::weak_ptr<GameObject>(std::weak_ptr<GameObject>)> parent_acquisition{
				[&](std::weak_ptr<GameObject> object)
				{
					auto pearent = object.lock()->pearent_game_object_;
					if (!pearent.expired())
					{
						return parent_acquisition(pearent);
					}
					else
					{
						return object;
					}
				}
			};
			return parent_acquisition(std::dynamic_pointer_cast<GameObject>(weak_from_this().lock()));
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
	yEngine::Property<bool> activeSelf{ &active_self_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// オブジェクトが動かない物ならtrue (get = true, set = true)
	/// </summary>
	yEngine::Property<bool> isStatic{ &is_static_ ,yEngine::AccessorType::AllAccess };

	yEngine::Property<std::weak_ptr<Transform>> transform
	{
		&transform_ ,yEngine::AccessorType::ReadOnly
	};


	// シリアライズ
	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(
			cereal::base_class<Object>(this),
			cereal::make_nvp("LocalActive", active_self_),
			cereal::make_nvp("LocalBlind", is_blind_),
			cereal::make_nvp("IsStatic", is_static_)

		);
	}

private:

	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	// ゲームオブジェクトマネージャーのポインタ
	static GameObjectManager *game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// オブジェクトコンテナ

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

	// ローカルのアクティブ状態
	bool active_self_{ true };
	// 非表示になっているか
	bool is_blind_;
	// 不動フラグ
	bool is_static_{ false };

	// トランスフォーム
	std::weak_ptr<Transform> transform_;
	

	// 衝突レイヤー
	uint16_t layer_{ 0 };


	// ゲームオブジェクトのタグ
	std::string tag_ = "Notag";

	// 親オブジェクト
	std::weak_ptr<GameObject> pearent_game_object_;
	// 子オブジェクトのコンテナ
	std::vector<std::weak_ptr<GameObject>> child_game_object_;
	// コンポーネント
	std::list<std::weak_ptr<Component>> component_list_;
	// コライダー
	std::vector<Collider*> colliders_;
	// MonoBehaviour
	std::vector<MonoBehaviour*> mono_behaviours_;
	
};

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddComponent(Args ...args)
{
	std::weak_ptr<T> temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp.lock()->transform_ = GetComponent<Transform>();
	temp.lock()->game_object_ = std::dynamic_pointer_cast<GameObject>(weak_from_this().lock());
	temp.lock()->CheckInitialize();
	// コンポーネントを更新順に並び替え
	component_list_.sort(
		[](std::weak_ptr<Component> lhs, std::weak_ptr<Component> rhs)
		{
			return static_cast<int>(lhs.lock()->type.r_) < static_cast<int>(rhs.lock()->type.r_);
		}
	);

	return temp;
}

template<class T>
std::weak_ptr<T> GameObject::GetComponent()
{
	std::weak_ptr<T>temp;
	for (auto component : component_list_)
	{
		temp = std::dynamic_pointer_cast<T>(component.lock());
		if (!temp.expired())
		{
			return temp;
		}
	}

	return temp;
}
