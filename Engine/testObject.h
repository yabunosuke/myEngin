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
	//		静的メンバ変数
	//
	//===========================================


	/// <summary>
	/// ゲームオブジェクトやコンポーネント、アセットの削除関数
	/// </summary>
	/// <param name="obj">削除するオブジェクト</param>
	/// <param name="t">削除するまでのディレイ時間</param>
	static void Destroy(testObject obj, float t = 0.0f);

	/// <summary>
	/// 指定された型に一致するオブジェクトを返す
	/// </summary>
	/// <typeparam name="Type">見つけたいオブジェクトの型</typeparam>
	/// <returns></returns>
	template<class Type>
	static std::weak_ptr<Type> FindObjectOfType();

	/// <summary>
	/// オブジェクト生成関数
	/// </summary>
	/// <typeparam name="T">生成する派生クラス</typeparam>
	/// <param name="...parameter">パラメータ</param>
	/// <returns>オブジェクトのweak_ptr</returns>
	template<class T,class ...Parameter>
	static std::weak_ptr<T> CreateObject(Parameter ...parameter);


	testObject();


	/// <summary>
	/// オブジェクト名
	/// </summary>
	std::string name_;

	

protected:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

private:
	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	/// <summary>
	/// オブジェクトコンテナ
	/// </summary>
	static std::vector<std::shared_ptr<testObject>> objects_;


	//===========================================
	//
	//		メンバ変数
	//
	//===========================================


	/// <summary>
	/// インスタンスID
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
