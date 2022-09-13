#pragma once
#include <memory>
#include <vector>
#include <string>
class testObject : 
	public std::enable_shared_from_this<testObject>
{
public:
	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================


	/// <summary>
	/// �Q�[���I�u�W�F�N�g��R���|�[�l���g�A�A�Z�b�g�̍폜�֐�
	/// </summary>
	/// <param name="obj">�폜����I�u�W�F�N�g</param>
	/// <param name="t">�폜����܂ł̃f�B���C����</param>
	static void Destroy(testObject obj, float t = 0.0f);

	/// <summary>
	/// �w�肳�ꂽ�^�Ɉ�v����I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <typeparam name="Type">���������I�u�W�F�N�g�̌^</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();

	/// <summary>
	/// �I�u�W�F�N�g�����֐�
	/// </summary>
	/// <typeparam name="T">��������h���N���X</typeparam>
	/// <param name="...parameter">�p�����[�^</param>
	/// <returns>�I�u�W�F�N�g��weak_ptr</returns>
	template<class T,class ...Parameter>
	static std::weak_ptr<T> CreateObject(Parameter ...parameter);


	testObject();


	/// <summary>
	/// �I�u�W�F�N�g��
	/// </summary>
	std::string name_;

	

protected:
	//===========================================
	//
	//		�R���X�g���N�^
	//
	//===========================================

private:
	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	/// <summary>
	/// �I�u�W�F�N�g�R���e�i
	/// </summary>
	static std::vector<std::shared_ptr<testObject>> objects_;


	//===========================================
	//
	//		�����o�ϐ�
	//
	//===========================================


	/// <summary>
	/// �C���X�^���XID
	/// </summary>
	int instance_id_;



};

template<class Type>
inline std::weak_ptr<Type> testObject::FindObjectOfType()
{
	return std::weak_ptr<Type>();
}

template<class T, class ...Parameter>
inline std::weak_ptr<T> testObject::CreateObject(Parameter ...parameter)
{
	std::shared_ptr<T> temp = std::make_shared<T>(parameter...);
	objects_.emplace_back(temp);
	return  temp;
}
