#pragma once

enum class Scenes {
	Test,
	Title,
	Tutorial,
	Game,
	MAX
};

class IoChangedListener		//仮想クラス
{
public:
	virtual void OnSceneChanged(const Scenes scene, const bool stackClear = false) = 0;
	virtual void SceneStackPop() = 0;
	virtual void ExitWindow() = 0;
};

