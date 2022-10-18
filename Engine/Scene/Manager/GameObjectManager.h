#pragma once
#include <vector>
#include <memory>
#include "Object/GameObject/GameObject.h"


class GameObjectManager
{
public:

	~GameObjectManager();

	/// <summary>
	/// 有効なオブジェクトを初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 有効なオブジェクトをアップデート
	/// </summary>
	void FixedUpdate();
	/// <summary>
	/// 有効なオブジェクトをアップデート
	/// </summary>
	void Update();
	/// <summary>
	/// 有効なオブジェクトをアップデート
	/// </summary>
	void LastUpdate();

	/// <summary>
	/// 有効なオブジェクトを描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// IDからオブジェクトを取得
	/// </summary>
	GameObject *GetGameObject(int id);
	GameObject *GetGameObject(const std::string &name);

	/// <summary>
	/// 有効なオブジェクトを全てファイナライズ
	/// </summary>
	void Finalize();

	//オブジェクト
	std::vector<GameObject*> game_objects_;
};

