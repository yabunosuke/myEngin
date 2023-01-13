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
	Sqript,		// �폜�\��
	Behaviour,		// 
	MonoBehaviour,		// 
	RigitBody,	// ���W�b�h�{�f�B
	Transform,	// �g�����X�t�H�[��
	Collider,
	Light,		// ���C�g
	Camera,		// �J����
	Renderer,	// �`�揈��
	Mesh,		// ���b�V��
};

class Component : 
	public Object
{
	friend cereal::access;
protected:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	using XMMATRIX = DirectX::XMMATRIX;

	friend class GameObject;

public:
	
	~Component() override;
	/// <summary>
	/// �A�N�e�B�u�ȂƂ�����������
	/// </summary>
	void CheckInitialize();
	/// <summary>
	/// �A�N�e�B�u�ȂƂ������X�V
	/// </summary>
	void CheckFixedUpdate();
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
	virtual void ImGuiDraw();



	//===========================================
	//
	//		�A�N�Z�b�T
	//
	//===========================================


	
	/// <summary>
	/// �R���|�[�l���g���� (get = true, set = false)
	/// </summary>
	yEngine::Property<ComponentType> type{ &type_,yEngine::AccessorType::ReadOnly };



	// �R���|�[�l���g��r���Z�q
	bool operator <(const Component &component) const{
		return static_cast<int>(type_) < static_cast<int>(component.type_);
	}
	bool operator >(const Component &component) const {
 		return static_cast<int>(type_) > static_cast<int>(component.type_);
	}

	bool operator == (const Component &component) const {
		return static_cast<int>(type_) == static_cast<int>(component.type_);
	}
	bool operator != (const Component &component) const {
		return static_cast<int>(type_) != static_cast<int>(component.type_);
	}

	// �Q�[���I�u�W�F�N�g
	std::weak_ptr<GameObject> game_object_;
	// �Q�[���I�u�W�F�N�g������Transform

	void SetTransform(std::weak_ptr<Transform> trans);
	yEngine::Property<std::weak_ptr<Transform>> transform
	{
		&transform_,yEngine::AccessorType::ReadOnly,

	};


	// �V���A���C�Y
	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(
			cereal::base_class<Object>(this),
			//cereal::make_nvp("GameObject", game_object_),
			cereal::make_nvp("DontRemove", dont_remove_)
			//cereal::make_nvp("Transform", transform_)
		);
	}


protected:	//�֐�
	Component(std::string name = "Component", ComponentType component_id = ComponentType::None, bool dontRemove = false);

	/// <summary>
	/// ������
	/// </summary>
	virtual void ComponentInitialize() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void ComponentFixedUpdate() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void ComponentUpdate() {};
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void ComponentLustUpdate() { };
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

	

private:

	// �폜�s��
	bool dont_remove_;
	// �g�����X�t�H�[��
	std::weak_ptr<Transform> transform_;

	ComponentType type_ = ComponentType::None;

};


CEREAL_REGISTER_TYPE(Component)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Object, Component)
