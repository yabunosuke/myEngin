#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include "IoChangedListener.h"
#include <stack>
#include <memory>
#include "ImGui/imguiManager.h"
#include "ImGui/ImGuizmo.h"
#include "Editor.h"

using namespace std;


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
	stack<unique_ptr<AbstractScene>> scene_stack_;

private:	//メニュー
	Editor editor;


};

