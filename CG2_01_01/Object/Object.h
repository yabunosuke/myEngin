#pragma once
// 参考
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

#include <string>
#include "Property.h"


class Object
{
public:
	Object();

	/// <summary>
	/// t秒後にオブジェクトを削除する
	/// </summary>
	/// <param name="object">削除対象のオブジェクト</param>
	/// <param name="t">削除までのディレイ時間</param>
	static void Destroy(Object *object, float t);
	

	/// <summary>
	/// オブジェクト名 (get = true, set = true)
	/// </summary>
	Property<std::string> name{ name_ ,AccessorType::AllAccess };
private:
	// オブジェクト名
	std::string name_;
};

