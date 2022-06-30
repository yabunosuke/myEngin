#pragma once
#include "AbstractScene.h"
#include <memory>
class Editor
{
public:
	void Initialize(std::shared_ptr<AbstractScene> nowScene) { this->nowScene = nowScene; }
	void Draw();

	/// <summary>
	/// �v���_�E�����j���[
	/// </summary>
	void MainMenu();
	/// <summary>
	/// �t�@�C��
	/// </summary>
	void FileMenu();

	/// <summary>
	/// �ҏW
	/// </summary>
	void EditMenu();

	/// <summary>
	/// �\������E�B���h�E
	/// </summary>
	void WindowMenu();

	/// <summary>
	/// �q�G�����L�[�`��
	/// </summary>
	void DrawHierarchy();
	/// <summary>
	/// �C���X�y�N�^�[�`��
	/// </summary>
	void DrawInspector();

private:	//���j���[
	bool isDemo = true;			//�f���E�B���h�E
	bool isHierarchy = true;	//�q�G�����L�[
	bool isInsupector = true;	//�f���E�B���h�E

	int selectedObjectNum = -1;

	std::shared_ptr<AbstractScene> nowScene;
	
};

