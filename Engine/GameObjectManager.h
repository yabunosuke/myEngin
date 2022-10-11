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

	/// <summary>
	/// ID����I�u�W�F�N�g���擾
	/// </summary>
	GameObject *GetGameObject(int id);

	/// <summary>
	/// �L���ȃI�u�W�F�N�g��S�ăt�@�C�i���C�Y
	/// </summary>
	void Finalize();

	//�I�u�W�F�N�g
	std::vector<GameObject*> game_objects_;
};

