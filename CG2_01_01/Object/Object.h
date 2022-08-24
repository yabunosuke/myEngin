#pragma once
#include <string>

// 参考
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

class Object
{
public:
	/// <summary>
	/// t秒後にオブジェクトを削除する
	/// </summary>
	/// <param name="object">削除対象のオブジェクト</param>
	/// <param name="t">削除までのディレイ時間</param>
	static void Destroy(Object *object, float t);

	


	// オブジェクト名
	std::string name_;
private:
};

