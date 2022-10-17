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
	stack<unique_ptr<AbstractScene>> scene_stack_;

private:	//���j���[
	Editor editor;


};

