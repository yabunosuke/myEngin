#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include "imgui.h"
//#include "GameObject.h"

class GameObject;

class Component
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// アクティブなときだけ初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// アクティブなときだけ更新
	/// </summary>
	void Update();
	/// <summary>
	/// アクティブなときだけ描画
	/// </summary>
	void Draw();
	/// <summary>
	/// アクティブなときだけ終了処理
	/// </summary>
	void Finalize();
	/// <summary>
	/// インスペクター用
	/// </summary>
	void ImGuiDraw();

public:	//ゲッター＆セッタ
	
	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


protected:	//関数
	Component(std::string name,bool dontRemove = false);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void VirtualInitialize() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void VirtualUpdate() {};
	/// <summary>
	/// 描画
	/// </summary>
	virtual void VirtualDraw() {};
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void VirtualFinalize() {};
	
	/// <summary>
	/// 情報
	/// </summary>
	virtual void Infomation() {};


protected:
	// オブジェクト
	std::weak_ptr<GameObject> object;
	// アクティブフラグ
	bool isActive;
	// リムーブフラグ
	bool isRemove;
private:
	//コンポーネントの名前
	std::string name;
	// 削除不可
	bool isDontRemove;
	// 重複不可
	
};

