#pragma once
// �Q�l
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

#include <string>
#include <vector>
#include "Property.h"


class Object
{
public:
	Object();

	/// <summary>
	/// t�b��ɃI�u�W�F�N�g���폜����
	/// </summary>
	/// <param name="object">�폜�Ώۂ̃I�u�W�F�N�g</param>
	/// <param name="t">�폜�܂ł̃f�B���C����</param>
	static void Destroy(Object *object, float t);

	/// <summary>
	/// �C���X�^���XID��Ԃ�
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// �I�u�W�F�N�g�� (AllAccess)
	/// </summary>
	Property<std::string> name{ name_ ,AccessorType::AllAccess };
private:
	
	static std::vector<Object*> objects_;		// �S�I�u�W�F�N�g���i�[����R���e�i


	// �I�u�W�F�N�gID�̏d�����p
	static int static_id_;

	// �C���X�^���XID
	unsigned const int instance_id_;
	// �I�u�W�F�N�g��
	std::string name_;

};

