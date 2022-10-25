#pragma once
// 参考
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
	//		静的変数
	//
	//===========================================
	/// <summary>
	/// ゲームオブジェクトやコンポーネント、アセットの削除関数
	/// </summary>
	/// <param name="objct">削除するオブジェクト</param>
	/// <param name="t">削除するまでのディレイ時間</param>
	static void Destroy(Object *destroy_object, float t = 0.0f);

	/// <summary>
	/// 指定された型に一致するオブジェクトを返す
	/// </summary>
	/// <typeparam name="Type">見つけたいオブジェクトの型</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();


	/// <summary>
	/// 指定された型に一致するオブジェクトを返す
	/// </summary>
	/// <typeparam name="Type">見つけたいオブジェクトの型</typeparam>
	/// <param name="include_inactive">非同期を含むかかどうか</param>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType(bool include_inactive);

	/// <summary>
	/// オブジェクト生成関数
	/// </summary>
	/// <typeparam name="T">生成する派生クラス</typeparam>
	/// <param name="...parameter">パラメータ</param>
	/// <returns>オブジェクトのweak_ptr</returns>
	template<class T, class ...Parameter>
	static T *CreateObject(Parameter ...parameter);


	static void Destroyer();

	/// <summary>
	/// インスタンスIDを返す
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// オブジェクト名 (AllAccess)
	/// </summary>
	yEngine::Property<std::string> name{ &name_ ,yEngine::AccessorType::AllAccess };

protected:
	virtual void DestoryRelated() {};

private:

	/// <summary>
	/// オブジェクトコンテナ
	/// </summary>
	static std::vector<std::unique_ptr<Object>> objects_;

	// オブジェクトIDの重複回避用
	static int static_id_;

	// インスタンスID
	unsigned const int instance_id_;
	// オブジェクト名
	std::string name_;

	// 削除までの時間
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
