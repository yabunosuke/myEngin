#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include "imgui.h"
//#include "GameObject.h"

class GameObject;

class Component
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �A�N�e�B�u�ȂƂ�����������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������I������
	/// </summary>
	void Finalize();
	/// <summary>
	/// �C���X�y�N�^�[�p
	/// </summary>
	void ImGuiDraw();

public:	//�Q�b�^�[���Z�b�^
	
	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


protected:	//�֐�
	Component(std::string name,bool dontRemove = false);

	/// <summary>
	/// ������
	/// </summary>
	virtual void VirtualInitialize() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void VirtualUpdate() {};
	/// <summary>
	/// �`��
	/// </summary>
	virtual void VirtualDraw() {};
	/// <summary>
	/// �I������
	/// </summary>
	virtual void VirtualFinalize() {};
	
	/// <summary>
	/// ���
	/// </summary>
	virtual void Infomation() {};


protected:
	// �I�u�W�F�N�g
	std::weak_ptr<GameObject> object;
	// �A�N�e�B�u�t���O
	bool isActive;
	// �����[�u�t���O
	bool isRemove;
private:
	//�R���|�[�l���g�̖��O
	std::string name;
	// �폜�s��
	bool isDontRemove;
	// �d���s��
	
};

