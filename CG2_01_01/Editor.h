#pragma once
#include "AbstractScene.h"
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
	void DrawHierarchy();
	/// <summary>
	/// インスペクター描画
	/// </summary>
	void DrawInspector();

private:	//メニュー
	bool isDemo = true;			//デモウィンドウ
	bool isHierarchy = true;	//ヒエラルキー
	bool isInsupector = true;	//デモウィンドウ

	int selectedObjectNum = -1;

	std::shared_ptr<AbstractScene> nowScene;
	
};

