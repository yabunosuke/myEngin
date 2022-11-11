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

class Collider;
class MonoBehaviour;
class GameObjectManager;

class GameObject final :
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
	~GameObject() override;
	//===========================================
	//
	//		�ÓI�����o�֐�
	//
	//===========================================

	static GameObject *CreateObject(const std::string &object_name = "");

	static void SetGameObjectManager(GameObjectManager *game_object_manager);

	/// <summary>
	/// �I�u�W�F�N�g�𖼑O�Ō������ĕԂ��B
	/// </summary>
	/// <param name="name">��������I�u�W�F�N�g��</param>
	/// <returns>���������I�u�W�F�N�g�̃|�C���^�B
	/// ������Ȃ������ꍇ��nullptr��Ԃ��B</returns>
	static GameObject *Find(const std::string &name);

	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

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
	inline T *AddComponent(Args ...args);
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
	T *GetComponent();

	/// <summary>
	/// �R���|�[�l���g�̎擾 ������
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	/*template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();*/


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

	void SetParent(GameObject *parent);

	// isBlind
	void SetIsBlind(bool blind) { isBlind = blind; }
	bool GetIsBlind() { return isBlind; }

	// �e�I�u�W�F�N�g�̎擾
	GameObject* GetPearent() { return pearent_game_object_; }
	// �q�̃I�u�W�F�N�g�R���e�i�̎擾
	std::vector<GameObject*> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// �R���|�[�l���g���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	std::list<Component*> &GetComponentList() { return component_list_; }


	// �R���C�_�[
	void AddCollider(Collider *collider);
	const std::vector<Collider*> &GetColliders() { return colliders_; }
	void RemoveCollider(Collider *collider);

	// MonoBehaviour
	void AddMonoBehaviour(MonoBehaviour *monobehaviour);
	const std::vector<MonoBehaviour *> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	
	/// <summary>
	/// ��ԏ�̐e���擾����( ReadOnly )
	/// </summary>
	yEngine::Property<GameObject *> top
	{
		nullptr, yEngine::AccessorType::ReadOnly,
		[this]()
		{

			std::function<GameObject *(GameObject *)> parent_acquisition{
				[&](GameObject *object)
				{
					auto pearent = object->pearent_game_object_;
					if (pearent != nullptr)
					{
						parent_acquisition(pearent);
					}
					else
					{
						return object;
					}
				} 
			};
			return parent_acquisition(this);
		},
		nullptr
	};

	/// <summary>
	/// �I�u�W�F�N�g���ʃ^�O (get = true, set = true)
	/// </summary>
	yEngine::Property<std::string> tag{ &tag_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// ���[�J���̃A�N�e�B�u��� (get = true, set = false)
	/// </summary>
	yEngine::Property<bool> activeSelf{ &active_self_ ,yEngine::AccessorType::ReadOnly };
	/// <summary>
	/// �I�u�W�F�N�g�������Ȃ����Ȃ�true (get = true, set = true)
	/// </summary>
	yEngine::Property<bool> isStatic{ &is_static_ ,yEngine::AccessorType::AllAccess };


	Transform *transform_;

	uint16_t layer_{0};

private:
	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	static GameObjectManager *game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// �I�u�W�F�N�g�R���e�i

	static std::vector<GameObject*> add_object_list_;


	//===========================================
	//
	//		�����o�֐�
	//
	//===========================================

	void DestoryRelated() override;

	//===========================================
	//
	//		�����o�ϐ�
	//
	//===========================================

	bool is_static_ = false;



	// �����Ă���V�[��
	//std::weak_ptr<AbstractScene> scene_;
	// �Q�[���I�u�W�F�N�g�̃^�O
	std::string tag_ = "Notag";

	// �e�I�u�W�F�N�g
	GameObject *pearent_game_object_{ nullptr };
	// �q�I�u�W�F�N�g�̃R���e�i
	std::vector<GameObject*> child_game_object_;
	// �R���|�[�l���g
	std::list<Component*> component_list_;
	// �R���C�_�[
	std::vector<Collider*> colliders_;
	// MonoBehaviour
	std::vector<MonoBehaviour*> mono_behaviours_;

	bool active_self_{true};				// ���[�J���̃A�N�e�B�u���
	// ��\���ɂȂ��Ă��邩
	bool isBlind;
};

template<class T, class ...Args>
inline T *GameObject::AddComponent(Args ...args)
{
	T *temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp->transform_ = GetComponent<Transform>();
	temp->game_object_ = this;
	temp->CheckInitialize();
	// �R���|�[�l���g���X�V���ɕ��ёւ�
	component_list_.sort(
		[](Component *lhs, Component *rhs)
		{
			return static_cast<int>(lhs->type.r_) < static_cast<int>(rhs->type.r_);
		}
	);

	return temp;
}

template<class T>
T *GameObject::GetComponent()
{
	T *temp = nullptr;
	for (auto component : component_list_)
	{
		temp = dynamic_cast<T*>(component);
		if (temp != nullptr)
		{
			return temp;
		}
	}

	return temp;
}

//template<class T>
//inline std::vector<std::weak_ptr<T>> GameObject::GetComponents()
//{
//	std::vector<std::weak_ptr<T>> components;
//	return std::weak_ptr<T>();
//}
