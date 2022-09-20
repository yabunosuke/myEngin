#pragma once

// �Q�l
// https://docs.unity3d.com/ja/2018.4/ScriptReference/GameObject.html

#include <list>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <map>


// ���N���X
#include "Object/Object.h"
#include "Object/Component/Transform.h"


//�R���|�[�l���g���N���X
#include "Object/Component/Component.h"


#include "Property.h"

// �R���C�_�[
class Collider;
class MonoBehaviour;
class GameObjectManager;

class GameObject :
public Object
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�֐�

	//===========================================
	//
	//		�R���X�g���N�^
	//
	//===========================================

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�I�u�W�F�N�g��</param>
	GameObject(const std::string &name);

	//===========================================
	//
	//		�ÓI�����o�֐�
	//
	//===========================================


	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	static std::weak_ptr<GameObject> CreateObject(const std::string &object_name = "");

	static void SetGameObjectManager(std::weak_ptr<GameObjectManager> game_object_manager);
	//===========================================
	//
	//		�����o�֐�
	//
	//===========================================
	
	/// <summary>
	/// �N���X��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<class T, class... Args>
	inline std::weak_ptr<T> AddComponent(Args ...args);
	/// <summary>
	/// �^�O���̔�r
	/// </summary>
	/// <param name="tag"�m�F�������^�O></param>
	/// <returns>bool �^�O����v�������ǂ���</returns>
	bool CompareTag(const std::string &tag);

	/// <summary>
	/// �R���|�[�l���g�̎擾
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	std::weak_ptr<T> GetComponent();

	/// <summary>
	/// �R���|�[�l���g�̎擾 ������
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();



	//������
	void Initialize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void FixedUpdate();
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void LastUpdate();


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

	


	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// �폜
	void Remove() { isRemove = true; }

	// �e�I�u�W�F�N�g�̎擾
	void SetPearentObject(std::weak_ptr<GameObject> pearent);
	std::weak_ptr<GameObject> &GetPearent() { return pearent_game_object_; }
	// �q�̃I�u�W�F�N�g�R���e�i�̎擾
	void SetCildrenObject(std::weak_ptr<GameObject> children) { child_game_object_.emplace_back(children); }
	std::vector<std::weak_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// �R���|�[�l���g���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	std::list<std::weak_ptr<Component>> GetComponentList() { return component_list_; }


	// �R���C�_�[
	void AddCollider(std::weak_ptr<Collider> collider);
	const std::vector<std::weak_ptr<Collider>> &GetColliders() { return colliders_; }
	void RemoveCollider(std::weak_ptr<Collider> collider);

	// MonoBehaviour
	void AddMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	const std::vector<std::weak_ptr<MonoBehaviour>> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);


	/// <summary>
	/// �������Ă���V�[�� (get = true, set = false)
	/// </summary>
	//Property<std::weak_ptr<AbstractScene>> scene{ scene_ ,AccessorType::ReadOnly };

	
	/// <summary>
	/// �I�u�W�F�N�g���ʃ^�O (get = true, set = true)
	/// </summary>
	yEngine::Property<std::string> tag{ tag_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// ���[�J���̃A�N�e�B�u��� (get = true, set = false)
	/// </summary>
	yEngine::Property<bool> activeSelf{ active_self_ ,yEngine::AccessorType::ReadOnly };
	/// <summary>
	/// ���[�J���̃A�N�e�B�u��� (get = true, set = false)
	/// </summary>
	yEngine::Property<std::weak_ptr<Transform>> transform{ transform_,yEngine::AccessorType::AllAccess };


private:
	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	static std::weak_ptr<GameObjectManager> game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// �I�u�W�F�N�g�R���e�i

	//===========================================
	//
	//		�����o�ϐ�
	//
	//===========================================




	std::weak_ptr<Transform> transform_;

	// �����Ă���V�[��
	//std::weak_ptr<AbstractScene> scene_;
	// �Q�[���I�u�W�F�N�g�̃^�O
	std::string tag_ = "Notag";

	// �e�I�u�W�F�N�g
	std::weak_ptr<GameObject> pearent_game_object_;
	// �q�I�u�W�F�N�g�̃R���e�i
	std::vector<std::weak_ptr<GameObject>> child_game_object_;
	// �R���|�[�l���g
	std::list<std::weak_ptr<Component>> component_list_;
	// �R���C�_�[
	std::vector<std::weak_ptr<Collider>> colliders_;
	// MonoBehaviour
	std::vector<std::weak_ptr<MonoBehaviour>> mono_behaviours_;

	bool active_self_;				// ���[�J���̃A�N�e�B�u���
	// ��\���ɂȂ��Ă��邩
	bool isBlind;
	// �폜���邩
	bool isRemove;

	


};

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddComponent(Args ...args)
{

	std::weak_ptr<T> temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp.lock()->transform_ = GetComponent<Transform>();
	temp.lock()->game_object_ = std::static_pointer_cast<GameObject>(shared_from_this());
	temp.lock()->CheckInitialize();
	// �R���|�[�l���g���X�V���ɕ��ёւ�
	component_list_.sort(
		[](std::weak_ptr<Component> lhs,std::weak_ptr<Component> rhs)
		{
			return static_cast<int>(lhs.lock()->type.r_) < static_cast<int>(rhs.lock()->type.r_);
		}
	);


	return temp;
}

template<class T>
inline std::weak_ptr<T> GameObject::GetComponent()
{
	std::weak_ptr<T> temp;
	for (auto &component : component_list_)
	{
		temp = std::dynamic_pointer_cast<T>(component.lock());
		if (!temp.expired())
		{
			return temp;
		}
	}

	return temp;
}

template<class T>
inline std::vector<std::weak_ptr<T>> GameObject::GetComponents()
{
	std::vector<std::weak_ptr<T>> components;
	return std::weak_ptr<T>();
}
