#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"


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
	void Update();
	/// <summary>
	/// 有効なオブジェクトをアップデート
	/// </summary>
	void LastUpdate();

	/// <summary>
	/// 有効なオブジェクトを描画
	/// </summary>
	void Draw() const;


	void DrawHierarchy(int &selectNum);

	/// <summary>
	/// 有効なオブジェクトを全てファイナライズ
	/// </summary>
	void Finalize();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="object_name">オブジェクト名（重複可）</param>
	/// <returns>オブジェクトのweak_ptr</returns>
	std::weak_ptr<GameObject> CreateObject(std::string object_name = "");

	//オブジェクト
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

