#pragma once

#include <string>
#include "imguiManager.h"
#include "IoChangedListener.h"
#include "GameObjectManager.h"

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

	GameObjectManager gameObjectManager;

private:
};

