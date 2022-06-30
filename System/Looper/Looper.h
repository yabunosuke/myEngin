#pragma once
#include "AbstractScene.h"
#include "IoChangedListener.h"
#include <stack>
#include <memory>
#include "imguiManager.h"
#include "Editor.h"

using namespace std;

enum class Scenes {
	Test,
	Title,
	Tutorial,
	Game,
	MAX
};

class Looper
	: public IoChangedListener {	
public:
	Looper();
	bool Loop();
	/// <summary>
	/// シーン遷移
	/// </summary>
	/// <param name="scene">次のシーン</param>
	/// <param name="stacClear">スタックを空にするか</param>
	void OnSceneChanged(const Scenes scene, const bool stacClear) override;
	/// <summary>
	/// 前のシーンへ戻る
	/// </summary>
	void SceneStackPop() override;	//ポップアウト
	
private:
	//シーンスタック
	stack<shared_ptr<AbstractScene>> sceneStack;

private:	//メニュー
	Editor editor;


};

