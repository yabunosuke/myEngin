#pragma once

// 参考
// https://docs.unity3d.com/ja/2018.4/ScriptReference/GameObject.html

#include <list>
#include <vector>
#include <memory>
#include <string>
#include <map>

// 基底クラス
#include "Object/Object.h"

//コンポーネント基底クラス
#include "Component/Component.h"
#include "Component/ScriptComponent.h"


#include "Property.h"

// コライダー
class BaseCollider;

class GameObject :
public Object
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	GameObject(const std::string &name);

	//初期化
	void Initialize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void LastUpdate();

	//一定の間隔で更新
	void FixUpdate() {};

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

///////////////////
// 
//ゲッタ&セッタ
//
///////////////////

	// ID
	const int &GetID() const { return id_; }
	

	// isActive
	void SetIsActive(bool active) { isActive = active; }
	bool GetIsActive() { return isActive; }

	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// 削除
	void Remove() { isRemove = true; }

	// 親オブジェクトの取得
	void SetPearentObject(std::weak_ptr<GameObject> pearent) { pearent_game_object_ = pearent; }
	std::weak_ptr<GameObject> &GetPearent() { return pearent_game_object_; }
	// 子のオブジェクトコンテナの取得
	void SetCildrenObject(std::weak_ptr<GameObject> children) { child_game_object_.emplace_back(children); }
	std::vector<std::weak_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// コンポーネントリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<std::shared_ptr<Component>> GetComponentList() { return component_list_; }

	/// <summary>
	/// コンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	T *GetComponent(std::string tag = "");

	//コンポーネントの追加
	template<class T, class... Args>
	T *AddComponent(Args ...args);

	// コライダー
	void AddCollider(std::weak_ptr<BaseCollider> collider);
	const std::vector<std::weak_ptr<BaseCollider>> &GetColliders() { return colliders_; }
	void RemoveCollider(std::weak_ptr<BaseCollider> collider);


	// Script
	const std::vector<std::weak_ptr<Component>> &GetScripts() { return scripts_; }


	/// <summary>
	/// 所属しているシーン (get = true, set = false)
	/// </summary>
	//Property<std::weak_ptr<AbstractScene>> scene{ scene_ ,AccessorType::ReadOnly };
	/// <summary>
	/// オブジェクト識別タグ (get = true, set = true)
	/// </summary>
	Property<std::string> tag{ tag_ ,AccessorType::AllAccess };

	// アタッチされているトランスフォーム
	Transform *transform_ = nullptr;



private:	// 静的メンバ変数
	// オブジェクトIDの重複回避用
	static uint64_t ID;


	// 属しているシーン
	//std::weak_ptr<AbstractScene> scene_;
	// ゲームオブジェクトのタグ
	std::string tag_ = "Notag";

	// オブジェクトID（重複しない）
	unsigned int id_;



	// 親オブジェクト
	std::weak_ptr<GameObject> pearent_game_object_;
	// 子オブジェクトのコンテナ
	std::vector<std::weak_ptr<GameObject>> child_game_object_;
	// コンポーネント
	std::list<std::shared_ptr<Component>> component_list_;
	// コライダーリスト
	std::vector<std::weak_ptr<BaseCollider>> colliders_;

	// Scriptリスト
	std::vector<std::weak_ptr<Component>> scripts_;

	// アクティブか
	bool isActive;
	// 非表示になっているか
	bool isBlind;
	// 削除するか
	bool isRemove;

	


};

template<class T>
inline T *GameObject::GetComponent(std::string tag)
{
	for (auto &component : component_list_) {
		T *temp = dynamic_cast<T *>(component.get());
		if (temp != nullptr) {
			return temp;
		}
	}

	return nullptr;
}

template<class T, class ...Args>
inline T *GameObject::AddComponent(Args ...args)
{
	T *buff = new T(args...);
	buff->game_object_=this;
	component_list_.emplace_back(buff);
	buff->CheckInitialize();
	buff->transform_ = GetComponent<Transform>();

	component_list_.sort();

	return buff;
}
