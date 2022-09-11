#pragma once
// 参考
// https://docs.unity3d.com/ja/2018.4/ScriptReference/Object.html

#include <string>
#include <vector>
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
	/// インスタンスIDを返す
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// オブジェクト名 (AllAccess)
	/// </summary>
	Property<std::string> name{ name_ ,AccessorType::AllAccess };
private:
	
	static std::vector<Object*> objects_;		// 全オブジェクトを格納するコンテナ


	// オブジェクトIDの重複回避用
	static int static_id_;

	// インスタンスID
	unsigned const int instance_id_;
	// オブジェクト名
	std::string name_;

};

