#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"


class GameObjectManager
{
public:
	/// <summary>
	/// �L���ȃI�u�W�F�N�g��������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �L���ȃI�u�W�F�N�g���A�b�v�f�[�g
	/// </summary>
	void Update();

	/// <summary>
	/// �L���ȃI�u�W�F�N�g��`��
	/// </summary>
	void Draw();


	void DrawHierarchy(int &selectNum);

	/// <summary>
	/// �L���ȃI�u�W�F�N�g��S�ăt�@�C�i���C�Y
	/// </summary>
	void Finalize();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	void CreateObject();

	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

