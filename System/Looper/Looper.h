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
	
	void ExitWindow() override { exit_window_ = true; };
private:
	enum class ChangeType
	{
		None,
		Push,
		Pop
	};
	struct SceneChangeData
	{
		ChangeType type;
		Scenes scene_name;
		bool is_clear;
	}scene_change_data_{};

	bool exit_window_{ false };

	
	// シーンスタック
	stack<unique_ptr<AbstractScene>> scene_stack_;

private:	//メニュー
	Editor editor;


};

