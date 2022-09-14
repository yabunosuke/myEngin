#pragma once
#include <vector>
#include <memory>
#include "Object/GameObject/GameObject.h"


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
	void FixedUpdate();
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

	// �e�q�ݒ�
	void SetPearentChild(std::weak_ptr<GameObject> pearent, std::weak_ptr<GameObject> child);

	/// <summary>
	/// ID����I�u�W�F�N�g���擾
	/// </summary>
	GameObject *GetGameObject(int id);

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
	std::vector<std::weak_ptr<GameObject>> game_objects_;
};

