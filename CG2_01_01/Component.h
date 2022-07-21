#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include "imgui.h"

//#include "GameObject.h"

class GameObject;

class Component
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// アクティブなときだけ初期化
	/// </summary>
	void CheckInitialize();
	/// <summary>
	/// アクティブなときだけ更新
	/// </summary>
	void CheckUpdate();
	/// <summary>
	/// アクティブなときだけ更新
	/// </summary>
	void CheckLustUpdate();
	/// <summary>
	/// アクティブなときだけ描画
	/// </summary>
	void CheckDraw();
	/// <summary>
	/// アクティブなときだけ終了処理
	/// </summary>
	void CheckFinalize();
	/// <summary>
	/// インスペクター用
	/// </summary>
	void ImGuiDraw();

	void SetParent(GameObject *obj) { parent = obj; }


public:	//ゲッター＆セッタ
	
	void SetObject(GameObject *obj) { parent = obj; }

	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


	std::string GetTag() { return tag; }

protected:	//関数
	Component(std::string name,bool dontRemove = false);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void ComponentInitialize() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void ComponentUpdate() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void ComponentLustUpdate() {};
	/// <summary>
	/// 描画
	/// </summary>
	virtual void ComponentDraw() {};
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void ComponentFinalize() {};
	
	/// <summary>
	/// 情報
	/// </summary>
	virtual void Infomation() {};


protected:
	// オブジェクト
	GameObject *parent = nullptr;
	// アクティブフラグ
	bool isActive;
	// リムーブフラグ
	bool isRemove;
	// 削除不可
	bool isDontRemove;
private:
	// コンポーネントの名前
	std::string name;
	// 重複チェック用のタグ
	std::string tag = "";

	// 重複不可
	
};

