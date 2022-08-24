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
enum class ComponentID
{
	None,
	Sqript,		// スクリプト
	RigitBody,	// リジッドボディ
	TRANSFORM,	// トランスフォーム
	Light,		// ライト
	Camera,		// カメラ
	Mesh,		// メッシュ
};

class Component : public Object
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
	virtual ~Component() = default;
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


	// コンポーネント比較演算子
	bool operator <(const Component &component) const{
		return static_cast<int>(component_id_) < static_cast<int>(component.component_id_);
	}
	bool operator == (const Component &component) const {
		return static_cast<int>(component_id_) == static_cast<int>(component.component_id_);
	}
	bool operator != (const Component &component) const {
		return static_cast<int>(component_id_) != static_cast<int>(component.component_id_);
	}


public:	//ゲッター＆セッタ
	

	// isRemove
	void Remove() { isRemove = true; }
	bool GetIsRemove() { return isRemove; }


	// ゲームオブジェクト
	GameObject *game_object_ = nullptr;
	// 重複チェック用のタグ
	std::string tag_ = "";
	// ゲームオブジェクトが持つTransform
	Transform *transform_ = nullptr;


protected:	//関数
	Component(std::string name, ComponentID component_id,bool dontRemove = false);

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



	bool operator >(std::shared_ptr<Component> rhs)
	{
		return static_cast<int>(this->component_id_) > static_cast<int>(rhs->component_id_);
	}
	bool operator <(std::shared_ptr<Component> rhs)
	{
		return static_cast<int>(this->component_id_) < static_cast<int>(rhs->component_id_);
	}

	

	// アクティブフラグ
	bool is_active_;
	// リムーブフラグ
	bool isRemove;
private:
	// コンポーネントの名前
	std::string name;
	// 削除不可
	bool isDontRemove;
	//
	ComponentID component_id_ = ComponentID::None;
	
};

