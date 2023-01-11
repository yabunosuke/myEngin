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

public:

	//===========================================
	//
	//		�R���X�g���N�^
	//
	//===========================================
	GameObject(const std::string &name = "", const std::string &tag = "", bool is_2d = false);
	~GameObject() override;

	//===========================================
	//
	//		�ÓI�����o�֐�
	//
	//===========================================

	static void SetGameObjectManager(GameObjectManager *game_object_manager);

	/// <summary>
	/// �I�u�W�F�N�g�𖼑O�Ō������ĕԂ��B
	/// </summary>
	/// <param name="name">��������I�u�W�F�N�g��</param>
	/// <returns>���������I�u�W�F�N�g�̃|�C���^�B
	/// ������Ȃ������ꍇ��nullptr��Ԃ��B</returns>
	static std::weak_ptr<GameObject> FindObject(const std::string &name);

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
	/*template<class T>
	std::vector<std::weak_ptr<T>>  GetComponents();*/

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

	void SetParent(std::weak_ptr<GameObject> parent);

	// isBlind
	void SetIsBlind(bool blind) { is_blind_ = blind; }
	bool GetIsBlind() { return is_blind_; }

	// �e�I�u�W�F�N�g�̎擾
	std::weak_ptr<GameObject> GetPearent() { return pearent_game_object_; }
	// �q�̃I�u�W�F�N�g�R���e�i�̎擾
	std::vector<std::weak_ptr<GameObject>> &GetChildren() { return child_game_object_; }

	/// <summary>
	/// �R���|�[�l���g���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	std::list<std::weak_ptr<Component>> &GetComponentList() { return component_list_; }


	// �R���C�_�[
	void AddCollider(Collider *collider);
	const std::vector<Collider*> &GetColliders() { return colliders_; }
	void RemoveCollider(Collider *collider);

	// MonoBehaviour
	void AddMonoBehaviour(MonoBehaviour *monobehaviour);
	const std::vector<MonoBehaviour *> &GetMonoBehaviours();
	void RemoveMonoBehaviour(std::weak_ptr<MonoBehaviour> monobehaviour);
	
	
	yEngine::Property<std::weak_ptr<GameObject>> top
	{
		nullptr, yEngine::AccessorType::ReadOnly,
		[this]()
		{

			std::function<std::weak_ptr<GameObject>(std::weak_ptr<GameObject>)> parent_acquisition{
				[&](std::weak_ptr<GameObject> object)
				{
					auto pearent = object.lock()->pearent_game_object_;
					if (!pearent.expired())
					{
						return parent_acquisition(pearent);
					}
					else
					{
						return object;
					}
				}
			};
			return parent_acquisition(std::dynamic_pointer_cast<GameObject>(weak_from_this().lock()));
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
	yEngine::Property<bool> activeSelf{ &active_self_ ,yEngine::AccessorType::AllAccess };
	/// <summary>
	/// �I�u�W�F�N�g�������Ȃ����Ȃ�true (get = true, set = true)
	/// </summary>
	yEngine::Property<bool> isStatic{ &is_static_ ,yEngine::AccessorType::AllAccess };

	yEngine::Property<std::weak_ptr<Transform>> transform
	{
		&transform_ ,yEngine::AccessorType::ReadOnly
	};


	// �V���A���C�Y
	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(
			cereal::base_class<Object>(this),
			cereal::make_nvp("LocalActive", active_self_),
			cereal::make_nvp("LocalBlind", is_blind_),
			cereal::make_nvp("IsStatic", is_static_)

		);
	}

private:

	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	static GameObjectManager *game_object_manager_;
	//static std::vector<std::shared_ptr<GameObject>> game_objects_;	// �I�u�W�F�N�g�R���e�i

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

	// ���[�J���̃A�N�e�B�u���
	bool active_self_{ true };
	// ��\���ɂȂ��Ă��邩
	bool is_blind_;
	// �s���t���O
	bool is_static_{ false };

	// �g�����X�t�H�[��
	std::weak_ptr<Transform> transform_;
	

	// �Փ˃��C���[
	uint16_t layer_{ 0 };


	// �Q�[���I�u�W�F�N�g�̃^�O
	std::string tag_ = "Notag";

	// �e�I�u�W�F�N�g
	std::weak_ptr<GameObject> pearent_game_object_;
	// �q�I�u�W�F�N�g�̃R���e�i
	std::vector<std::weak_ptr<GameObject>> child_game_object_;
	// �R���|�[�l���g
	std::list<std::weak_ptr<Component>> component_list_;
	// �R���C�_�[
	std::vector<Collider*> colliders_;
	// MonoBehaviour
	std::vector<MonoBehaviour*> mono_behaviours_;
	
};

template<class T, class ...Args>
inline std::weak_ptr<T> GameObject::AddComponent(Args ...args)
{
	std::weak_ptr<T> temp = Object::CreateObject<T>(args...);
	component_list_.emplace_back(temp);
	temp.lock()->transform_ = GetComponent<Transform>();
	temp.lock()->game_object_ = std::dynamic_pointer_cast<GameObject>(weak_from_this().lock());
	temp.lock()->CheckInitialize();
	// �R���|�[�l���g���X�V���ɕ��ёւ�
	component_list_.sort(
		[](std::weak_ptr<Component> lhs, std::weak_ptr<Component> rhs)
		{
			return static_cast<int>(lhs.lock()->type.r_) < static_cast<int>(rhs.lock()->type.r_);
		}
	);

	return temp;
}

template<class T>
std::weak_ptr<T> GameObject::GetComponent()
{
	std::weak_ptr<T>temp;
	for (auto component : component_list_)
	{
		temp = std::dynamic_pointer_cast<T>(component.lock());
		if (!temp.expired())
		{
			return temp;
		}
	}

	return temp;
}
