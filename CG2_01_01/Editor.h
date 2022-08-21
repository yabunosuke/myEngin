#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include <memory>
class Editor
{
public:
	void Initialize(std::shared_ptr<AbstractScene> nowScene) { this->nowScene = nowScene; }
	void Draw();

	/// <summary>
	/// プルダウンメニュー
	/// </summary>
	void MainMenu();
	/// <summary>
	/// ファイル
	/// </summary>
	void FileMenu();

	/// <summary>
	/// 編集
	/// </summary>
	void EditMenu();

	/// <summary>
	/// 表示するウィンドウ
	/// </summary>
	void WindowMenu();

	/// <summary>
	/// ヒエラルキー描画
	/// </summary>
	void HierarchyWindow();
	// 親用
	void Hierarchy(std::vector<std::shared_ptr<GameObject>> &objects, bool is_child = false);
	// 子用
	void Hierarchy(std::vector<std::weak_ptr<GameObject>> &objects, bool is_child = false);

	/// <summary>
	/// インスペクター描画
	/// </summary>
	void DrawInspector();
	/// <summary>
	/// マルチレンダー描画
	/// </summary>
	void DrawMulutiRender();

private:	//メニュー
	bool isDemo = true;			//デモウィンドウ
	bool isHierarchy = true;	//ヒエラルキー
	bool isInsupector = true;	//デモウィンドウ

	int selected_object_id = -1;

	std::shared_ptr<AbstractScene> nowScene;
	
};

