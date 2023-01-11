#pragma once
#include <vector>
#include <memory>
#include "Object/GameObject/GameObject.h"


class GameObjectManager
{
public:

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
	std::weak_ptr<GameObject> GetGameObject(int id);
	std::weak_ptr<GameObject> GetGameObject(const std::string &name);

	void AddObjects();

	/// <summary>
	/// 有効なオブジェクトを全てファイナライズ
	/// </summary>
	void Finalize();

	//オブジェクト
	std::vector<std::weak_ptr<GameObject>> game_objects_;
	std::vector<std::weak_ptr<GameObject>> add_objects_;	// 一時格納コンテナ
};

