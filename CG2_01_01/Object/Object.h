#pragma once
#include <string>

// �Q�l
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

class Object
{
public:
	/// <summary>
	/// t�b��ɃI�u�W�F�N�g���폜����
	/// </summary>
	/// <param name="object">�폜�Ώۂ̃I�u�W�F�N�g</param>
	/// <param name="t">�폜�܂ł̃f�B���C����</param>
	static void Destroy(Object *object, float t);

	


	// �I�u�W�F�N�g��
	std::string name_;
private:
};

