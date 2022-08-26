#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include "ImGui/imgui.h"
#include "Math/Mathf.h"
#include "Object/Object.h"



class GameObject;
class Transform;

// �X�V��
enum class ComponentType
{
	None,
	Sqript,		// �X�N���v�g
	RigitBody,	// ���W�b�h�{�f�B
	TRANSFORM,	// �g�����X�t�H�[��
	Light,		// ���C�g
	Camera,		// �J����
	Mesh,		// ���b�V��
};

class Component : public Object
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



	//===========================================
	//
	//		�A�N�Z�b�T
	//
	//===========================================


	/// <summary>
	/// �g�����X�t�H�[�� (get = true, set = false)
	/// </summary>
	/*Property <Transform> transform{
		transform_,
		AccessorType::AllAccess,
		
	};*/
	/// <summary>
	/// �R���|�[�l���g���� (get = true, set = false)
	/// </summary>
	Property<ComponentType> type{ type_,AccessorType::ReadOnly };



	// �R���|�[�l���g��r���Z�q
	bool operator <(const Component &component) const{
		return static_cast<int>(type_) < static_cast<int>(component.type_);
	}
	bool operator == (const Component &component) const {
		return static_cast<int>(type_) == static_cast<int>(component.type_);
	}
	bool operator != (const Component &component) const {
		return static_cast<int>(type_) != static_cast<int>(component.type_);
	}


public:	//�Q�b�^�[���Z�b�^
	
	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


	// �Q�[���I�u�W�F�N�g
	GameObject *game_object_ = nullptr;
	// �d���`�F�b�N�p�̃^�O
	std::string tag_ = "";
	// �Q�[���I�u�W�F�N�g������Transform
	Transform *transform_;


protected:	//�֐�
	Component(std::string name, ComponentType component_id,bool dontRemove = false);

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
		return static_cast<int>(this->type_) > static_cast<int>(rhs->type_);
	}
	bool operator <(std::shared_ptr<Component> rhs)
	{
		return static_cast<int>(this->type_) < static_cast<int>(rhs->type_);
	}

	

	// �A�N�e�B�u�t���O
	bool is_active_;
	// �����[�u�t���O
	bool isRemove;
private:
	// �폜�s��
	bool isDontRemove;
	//
	ComponentType type_ = ComponentType::None;

public:	// �A�N�Z�b�T
	
};

