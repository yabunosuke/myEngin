#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
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
	void HierarchyWindow();
	// �e�p
	void Hierarchy(std::vector<std::shared_ptr<GameObject>> &objects, bool is_child = false);
	// �q�p
	void Hierarchy(std::vector<std::weak_ptr<GameObject>> &objects, bool is_child = false);

	/// <summary>
	/// �C���X�y�N�^�[�`��
	/// </summary>
	void DrawInspector();
	/// <summary>
	/// �}���`�����_�[�`��
	/// </summary>
	void DrawMulutiRender();

private:	//���j���[
	bool isDemo = true;			//�f���E�B���h�E
	bool isHierarchy = true;	//�q�G�����L�[
	bool isInsupector = true;	//�f���E�B���h�E

	int selected_object_id = -1;

	std::shared_ptr<AbstractScene> nowScene;
	
};

