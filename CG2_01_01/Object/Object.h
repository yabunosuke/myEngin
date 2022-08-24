#pragma once
// �Q�l
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

#include <string>
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
	/// �I�u�W�F�N�g�� (get = true, set = true)
	/// </summary>
	Property<std::string> name{ name_ ,AccessorType::AllAccess };
private:
	// �I�u�W�F�N�g��
	std::string name_;
};

