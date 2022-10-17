#pragma once

enum class Scenes {
	Test,
	Title,
	Tutorial,
	Game,
	MAX
};

class IoChangedListener		//���z�N���X
{
public:
	virtual void OnSceneChanged(const Scenes scene, const bool stackClear = false) = 0;
	virtual void SceneStackPop() = 0;
};

