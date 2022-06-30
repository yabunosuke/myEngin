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
	/// 有効なオブジェクトを描画
	/// </summary>
	void Draw();


	void DrawHierarchy(int &selectNum);

	/// <summary>
	/// 有効なオブジェクトを全てファイナライズ
	/// </summary>
	void Finalize();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	void CreateObject();

	//オブジェクト
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

