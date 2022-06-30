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
	/// �V�[���J��
	/// </summary>
	/// <param name="scene">���̃V�[��</param>
	/// <param name="stacClear">�X�^�b�N����ɂ��邩</param>
	void OnSceneChanged(const Scenes scene, const bool stacClear) override;
	/// <summary>
	/// �O�̃V�[���֖߂�
	/// </summary>
	void SceneStackPop() override;	//�|�b�v�A�E�g
	
private:
	//�V�[���X�^�b�N
	stack<shared_ptr<AbstractScene>> sceneStack;

private:	//���j���[
	Editor editor;


};

