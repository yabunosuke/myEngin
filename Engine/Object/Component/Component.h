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
	Sqript,		// 削除予定
	Behaviour,		// 
	MonoBehaviour,		// 
	RigitBody,	// リジッドボディ
	Transform,	// トランスフォーム
	Collider,
	Light,		// ライト
	Camera,		// カメラ
	Renderer,	// 描画処理
	Mesh,		// メッシュ
};

class Component : 
	public Object
{
	friend cereal::access;
protected:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	using XMMATRIX = DirectX::XMMATRIX;

	friend class GameObject;

public:
	
	~Component() override;
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
	virtual void ImGuiDraw();



	//===========================================
	//
	//		アクセッサ
	//
	//===========================================


	
	/// <summary>
	/// コンポーネント識別 (get = true, set = false)
	/// </summary>
	yEngine::Property<ComponentType> type{ &type_,yEngine::AccessorType::ReadOnly };



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

	// ゲームオブジェクト
	std::weak_ptr<GameObject> game_object_;
	// ゲームオブジェクトが持つTransform

	void SetTransform(std::weak_ptr<Transform> trans);
	yEngine::Property<std::weak_ptr<Transform>> transform
	{
		&transform_,yEngine::AccessorType::ReadOnly,

	};


	// シリアライズ
	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(
			cereal::base_class<Object>(this),
			//cereal::make_nvp("GameObject", game_object_),
			cereal::make_nvp("DontRemove", dont_remove_)
			//cereal::make_nvp("Transform", transform_)
		);
	}


protected:	//関数
	Component(std::string name = "Component", ComponentType component_id = ComponentType::None, bool dontRemove = false);

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
	virtual void ComponentLustUpdate() { };
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

	

private:

	// 削除不可
	bool dont_remove_;
	// トランスフォーム
	std::weak_ptr<Transform> transform_;

	ComponentType type_ = ComponentType::None;

};


CEREAL_REGISTER_TYPE(Component)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Object, Component)
