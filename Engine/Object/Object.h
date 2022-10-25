#pragma once
// �Q�l
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

#include <memory>
#include <string>
#include <vector>

#include "Property.h"


class Object
{
public:
	
	Object();
	virtual ~Object();
	//===========================================
	//
	//		�ÓI�ϐ�
	//
	//===========================================
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��R���|�[�l���g�A�A�Z�b�g�̍폜�֐�
	/// </summary>
	/// <param name="objct">�폜����I�u�W�F�N�g</param>
	/// <param name="t">�폜����܂ł̃f�B���C����</param>
	static void Destroy(Object *destroy_object, float t = 0.0f);

	/// <summary>
	/// �w�肳�ꂽ�^�Ɉ�v����I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <typeparam name="Type">���������I�u�W�F�N�g�̌^</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();


	/// <summary>
	/// �w�肳�ꂽ�^�Ɉ�v����I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <typeparam name="Type">���������I�u�W�F�N�g�̌^</typeparam>
	/// <param name="include_inactive">�񓯊����܂ނ����ǂ���</param>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType(bool include_inactive);

	/// <summary>
	/// �I�u�W�F�N�g�����֐�
	/// </summary>
	/// <typeparam name="T">��������h���N���X</typeparam>
	/// <param name="...parameter">�p�����[�^</param>
	/// <returns>�I�u�W�F�N�g��weak_ptr</returns>
	template<class T, class ...Parameter>
	static T *CreateObject(Parameter ...parameter);


	static void Destroyer();

	/// <summary>
	/// �C���X�^���XID��Ԃ�
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// �I�u�W�F�N�g�� (AllAccess)
	/// </summary>
	yEngine::Property<std::string> name{ &name_ ,yEngine::AccessorType::AllAccess };

protected:
	virtual void DestoryRelated() {};

private:

	/// <summary>
	/// �I�u�W�F�N�g�R���e�i
	/// </summary>
	static std::vector<std::unique_ptr<Object>> objects_;

	// �I�u�W�F�N�gID�̏d�����p
	static int static_id_;

	// �C���X�^���XID
	unsigned const int instance_id_;
	// �I�u�W�F�N�g��
	std::string name_;

	// �폜�܂ł̎���
	float destroy_timer_{ 0.0f };
	bool is_destroy_ = false;
};

template<class Type>
inline std::weak_ptr<Type> Object::FindObjectOfType()
{

	return std::weak_ptr<Type>();
}

template<class Type>
inline std::weak_ptr<Type> Object::FindObjectOfType(bool include_inactive)
{
	return std::weak_ptr<Type>();
}

template<class T, class ...Parameter>
inline T *Object::CreateObject(Parameter ...parameter)
{
	T *temp = new T(parameter...);
	objects_.emplace_back(temp);
	return temp;
}
