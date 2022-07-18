#pragma once
#include <list>
#include <vector>
#include <memory>
#include <string>

//コンポーネント基底クラス
#include "Component.h"

class GameObject
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:	//ゲッタ&セッタ
	// ID
	const int &GetID() const { return id; }

	// 名前
	void SetName(const std::string &name) { this->name = name; }
	const std::string &GetName() { return name; }

	// タグ
	void SetTag(const std::string &tag) { this->tag = tag; }
	const std::string &GetTag() { return tag; }

	// isActive
	void SetIsActive(bool active) { isActive = active; }
	bool GetIsActive() { return isActive; }
	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }
	// 削除
	void Remove() { isRemove = true; }

	// 親オブジェクトの取得
	std::weak_ptr<GameObject> &GetPearent() { return pearentGameObject; }
	// このオブジェクトコンテナの取得
	std::vector<std::shared_ptr<GameObject>> &GetChildren() { return childGameObject; }

public:	//関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	GameObject(std::string name);

	//初期化
	void Initialize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

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

	/// <summary>
	/// コンポーネントリストの取得
	/// </summary>
	/// <returns></returns>
	std::list<Component *> GetComponentList() { return componentList; }

	/// <summary>
	/// コンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	T *GetComponent() {
		for (auto &component : componentList) {
			T *temp = dynamic_cast<T *>(component);
			if (temp != nullptr) {
				return temp;
			}
		}
		return nullptr;
	};

	//コンポーネントの追加
	template<class T, class... Args>
	T *AddComponent(Args ...args)
	{
		T *buff = new T(args...);
		buff->SetParent(this);
		componentList.emplace_back(buff);
		buff->Initialize();
		return buff;
	}
	
protected:
	// 名前
	std::string name;
	// タグ
	std::string tag = "None";
	// 親オブジェクト
	std::weak_ptr<GameObject> pearentGameObject;
	// 子オブジェクトのコンテナ
	std::vector<std::shared_ptr<GameObject>> childGameObject;
	// コンポーネント
	std::list<Component *> componentList;

private:	// 静的メンバ変数
	// オブジェクトIDの重複回避用
	static uint64_t ID;

private://変数
	// オブジェクトID（重複しない）
	unsigned int id;

	// アクティブか
	bool isActive;
	// 非表示になっているか
	bool isBlind;
	// 削除するか
	bool isRemove;

};

