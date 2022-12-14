#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include <memory>
class Editor
{
public:
	void Initialize(AbstractScene *nowScene) { this->nowScene = nowScene; }
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
	void Hierarchy(std::vector<GameObject*> &objects, bool is_child = false);

	void FileBrowser();

	/// <summary>
	/// インスペクター描画
	/// </summary>
	void DrawInspector();
	/// <summary>
	/// マルチレンダー描画
	/// </summary>
	void DrawMulutiRender();

	/// <summary>
	/// マルチレンダー描画
	/// </summary>
	void ProjectSettingsWindow();

private:	//メニュー
	bool isDemo = false;			//デモウィンドウ
	bool isHierarchy = true;	//ヒエラルキー
	bool isInsupector = true;	//デモウィンドウ
	bool open_project_settings_window_ = false;

	int selected_object_id = -1;

	AbstractScene *nowScene;

};

