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
	/// �L���ȃI�u�W�F�N�g���A�b�v�f�[�g
	/// </summary>
	void LastUpdate();

	/// <summary>
	/// �L���ȃI�u�W�F�N�g��`��
	/// </summary>
	void Draw() const;


	void DrawHierarchy(int &selectNum);

	/// <summary>
	/// �L���ȃI�u�W�F�N�g��S�ăt�@�C�i���C�Y
	/// </summary>
	void Finalize();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="object_name">�I�u�W�F�N�g���i�d���j</param>
	/// <returns>�I�u�W�F�N�g��weak_ptr</returns>
	std::weak_ptr<GameObject> CreateObject(std::string object_name = "");

	//�I�u�W�F�N�g
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

