#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include "ImGui/imgui.h"
#include "Math/Mathf.h"
#include "Object/Object.h"



class GameObject;
class Transform;

// 更新順
enum class ComponentType
{
	None,
	Sqript,		// スクリプト
	RigitBody,	// リジッドボディ
	TRANSFORM,	// トランスフォーム
	Light,		// ライト
	Camera,		// カメラ
	Renderer,	// 描画処理
	Mesh,		// メッシュ
};

class Component : 
	public Object
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
	void CheckFixedUpdate();
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



	//===========================================
	//
	//		アクセッサ
	//
	//===========================================


	/// <summary>
	/// トランスフォーム (get = true, set = false)
	/// </summary>
	yEngin::Property <Transform*> transform{
		transform_.lock().get(),
		yEngin::AccessorType::AllAccess,
		
	};
	/// <summary>
	/// コンポーネント識別 (get = true, set = false)
	/// </summary>
	yEngin::Property<ComponentType> type{ type_,yEngin::AccessorType::ReadOnly };



	// コンポーネント比較演算子
	bool operator <(const Component &component) const{
		return static_cast<int>(type_) < static_cast<int>(component.type_);
	}
	bool operator >(const Component &component) const {
 		return static_cast<int>(type_) > static_cast<int>(component.type_);
	}

	bool operator == (const Component &component) const {
		return static_cast<int>(type_) == static_cast<int>(component.type_);
	}
	bool operator != (const Component &component) const {
		return static_cast<int>(type_) != static_cast<int>(component.type_);
	}


	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


	// ゲームオブジェクト
	std::weak_ptr<GameObject>game_object_;
	// 重複チェック用のタグ
	std::string tag_ = "";
	// ゲームオブジェクトが持つTransform
	std::weak_ptr<Transform>  transform_;


protected:	//関数
	Component(std::string name, ComponentType component_id,bool dontRemove = false);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void ComponentInitialize() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void ComponentFixedUpdate() {};
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




	

	// アクティブフラグ
	bool is_active_;
	// リムーブフラグ
	bool isRemove;
private:
	// 削除不可
	bool isDontRemove;
	//
	ComponentType type_ = ComponentType::None;

public:	// アクセッサ
	
};