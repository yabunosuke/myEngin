#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include "ImGui/imgui.h"
#include "Math/Mathf.h"

class GameObject;

enum class ComponentID
{
	None,
	Sqript,
	RigitBody,	
	TRANSFORM,
	LIGHT,
	Mesh,
};

class Component
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	virtual ~Component() = default;
	/// <summary>
	/// �A�N�e�B�u�ȂƂ�����������
	/// </summary>
	void CheckInitialize();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������X�V
	/// </summary>
	void CheckUpdate();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������X�V
	/// </summary>
	void CheckLustUpdate();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������`��
	/// </summary>
	void CheckDraw();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������I������
	/// </summary>
	void CheckFinalize();
	/// <summary>
	/// �C���X�y�N�^�[�p
	/// </summary>
	void ImGuiDraw();


	// �R���|�[�l���g��r���Z�q
	bool operator <(const Component &component) const{
		return static_cast<int>(component_id_) < static_cast<int>(component.component_id_);
	}
	bool operator == (const Component &component) const {
		return static_cast<int>(component_id_) == static_cast<int>(component.component_id_);
	}
	bool operator != (const Component &component) const {
		return static_cast<int>(component_id_) != static_cast<int>(component.component_id_);
	}


public:	//�Q�b�^�[���Z�b�^
	
	void SetObject(GameObject *obj) { object_ = obj; }

	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


	std::string GetTag() { return tag; }

protected:	//�֐�
	Component(std::string name, ComponentID component_id,bool dontRemove = false);

	/// <summary>
	/// ������
	/// </summary>
	virtual void ComponentInitialize() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void ComponentUpdate() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void ComponentLustUpdate() {};
	/// <summary>
	/// �`��
	/// </summary>
	virtual void ComponentDraw() {};
	/// <summary>
	/// �I������
	/// </summary>
	virtual void ComponentFinalize() {};
	
	/// <summary>
	/// ���
	/// </summary>
	virtual void Infomation() {};



	bool operator >(std::shared_ptr<Component> rhs)
	{
		return static_cast<int>(this->component_id_) > static_cast<int>(rhs->component_id_);
	}

protected:
	// �I�u�W�F�N�g
	GameObject *object_ = nullptr;
	// �A�N�e�B�u�t���O
	bool isActive;
	// �����[�u�t���O
	bool isRemove;
private:
	// �R���|�[�l���g�̖��O
	std::string name;
	// �d���`�F�b�N�p�̃^�O
	std::string tag = "";
	// �폜�s��
	bool isDontRemove;
	//
	ComponentID component_id_ = ComponentID::None;
	
};

