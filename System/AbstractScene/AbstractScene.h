#pragma once

#include <string>
#include "imguiManager.h"
#include "IoChangedListener.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"

class AbstractScene
{
protected:
	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	virtual ~AbstractScene() = default;
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// ���Ԋu�ōX�V
	/// </summary>
	//virtual void FixUpdate() = 0;
	/// <summary>
	/// �`��R�}���h���s
	/// </summary>
	virtual void Draw() const = 0;
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;

	//�V�[����
	std::string name;


	GameObjectManager *GetObjectManager() { return &game_object_manager_; }
	CollisionManager *GetCollisionManager() { return &collision_manager_; }


protected:
	// �I�u�W�F�N�g�}�l�[�W���[
	GameObjectManager game_object_manager_;
	// �R���W�����}�l�[�W���[
	CollisionManager collision_manager_;

private:

};

