#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include <memory>
class Editor
{
public:
	void Initialize(AbstractScene *nowScene) { this->nowScene = nowScene; }
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
	void HierarchyWindow();
	// �e�p
	void Hierarchy(std::vector<GameObject*> &objects, bool is_child = false);

	void FileBrowser();

	/// <summary>
	/// �C���X�y�N�^�[�`��
	/// </summary>
	void DrawInspector();
	/// <summary>
	/// �}���`�����_�[�`��
	/// </summary>
	void DrawMulutiRender();

	/// <summary>
	/// �}���`�����_�[�`��
	/// </summary>
	void ProjectSettingsWindow();

private:	//���j���[
	bool isDemo = false;			//�f���E�B���h�E
	bool isHierarchy = true;	//�q�G�����L�[
	bool isInsupector = true;	//�f���E�B���h�E
	bool open_project_settings_window_ = false;

	int selected_object_id = -1;

	AbstractScene *nowScene;

};

