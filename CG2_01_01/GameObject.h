#pragma once
#include <list>
#include <vector>
#include <memory>
#include <string>
#include <map>

//�R���|�[�l���g���N���X
#include "Component/Component.h"
#include "Component/ScriptComponent.h"

// �R���C�_�[
class BaseCollider;

class GameObject
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�I�u�W�F�N�g��</param>
	GameObject(const std::string &name);

	//������
	void Initialize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void LastUpdate();

	//���̊Ԋu�ōX�V
	void FixUpdate() {};

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// 
	/// </summary>
	void DrawInspector();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() {};

///////////////////
// 
//�Q�b�^&�Z�b�^
//
///////////////////

	// ID
	const int &GetID() const { return id_; }

	// ���O
	const std::string &GetName() { return name_; }

	// �^�O
	void SetTag(const std::string &tag) { this->tags_.emplace_back(tag); }
	const std::vector<std::string> &GetTag() { return tags_; }

	// isActive
	void SetIsActive(bool active) { isActive = active; }
	bool GetIsActive() { return isActive; }

	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// �폜
	void Remove() { isRemove = true; }

	// �e�I�u�W�F�N�g�̎擾
	std::weak_ptr<GameObject> &GetPearent() { return pearent_game_object_; }
	// ���̃I�u�W�F�N�g�R���e�i�̎擾
	std::vector<std::shared_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// �R���|�[�l���g���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	std::list<std::shared_ptr<Component>> GetComponentList() { return component_list_; }

	/// <summary>
	/// �R���|�[�l���g�̎擾
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	T *GetComponent(std::string tag = "");

	//�R���|�[�l���g�̒ǉ�
	template<class T, class... Args>
	T *AddComponent(Args ...args);

	// �R���C�_�[
	void AddCollider(std::weak_ptr<BaseCollider> collider);
	const std::vector<std::weak_ptr<BaseCollider>> &GetColliders() { return colliders_; }
	void RemoveCollider(std::weak_ptr<BaseCollider> collider);

	// Script
	const std::vector<std::weak_ptr<Component>> &GetScripts() { return scripts_; }

private:	// �ÓI�����o�ϐ�
	// �I�u�W�F�N�gID�̏d�����p
	static uint64_t ID;

private://�ϐ�
	// �I�u�W�F�N�gID�i�d�����Ȃ��j
	unsigned int id_;
	
	// ���O
	std::string name_;
	// �^�O
	std::vector<std::string> tags_;

	// �e�I�u�W�F�N�g
	std::weak_ptr<GameObject> pearent_game_object_;
	// �q�I�u�W�F�N�g�̃R���e�i
	std::vector<std::shared_ptr<GameObject>> child_game_object_;
	// �R���|�[�l���g
	std::list<std::shared_ptr<Component>> component_list_;
	// �R���C�_�[���X�g
	std::vector<std::weak_ptr<BaseCollider>> colliders_;

	// Script���X�g
	std::vector<std::weak_ptr<Component>> scripts_;

	// �A�N�e�B�u��
	bool isActive;
	// ��\���ɂȂ��Ă��邩
	bool isBlind;
	// �폜���邩
	bool isRemove;



};

template<class T>
inline T *GameObject::GetComponent(std::string tag)
{
	for (auto &component : component_list_) {
		T *temp = dynamic_cast<T *>(component.get());
		if (temp != nullptr) {
			if (temp->GetTag() == tag) {

				return temp;
			}
		}
	}

	return nullptr;
}

template<class T, class ...Args>
inline T *GameObject::AddComponent(Args ...args)
{
	T *buff = new T(args...);
	buff->SetParent(this);
	component_list_.emplace_back(buff);
	buff->CheckInitialize();


	return buff;
}
