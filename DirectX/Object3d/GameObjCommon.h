#pragma once
#include "Vector3.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "CollisionInfo.h"
#include <vector>
#include <map>
#include <string>

class GameObjCommon:
	public Object3d
{
public://サブクラス
	enum RenderQueue {			//レンダーキュー
		BackGround =	1000,	//背景
		Geometry =		2000,	//不透明オブジェクト
		AlphaTest =		2450,	//アルファテスト
		Transparent =	3000,	//アルファブレンド用
		Overlay =		4000,	//エフェクトなど
	};
public:
	GameObjCommon(
		std::string modelName,				//モデルの名前
		const std::string tag = "Notag",	//識別用タグ
		bool isGravity = false,				//重力処理の有無
		Vector3 pos = { 0,0,0 },			//初期位置
		Vector3 scale = { 1,1,1 },			//初期サイズ
		Vector3 rotate = { 0,0,0 }			//初期回転
	) :
		Tag(tag),
		isGravity(isGravity),
		velocity(0, 0, 0),
		isActive(true),
		isInvisible(false),
		isDelete(false),
		gravityPow(0.98f)
	{
		Object3d::Initialize();
		SetModel(ModelManager::GetIns()->GetModel(modelName));
		name = &modelName;
		/*objectData->*/SetModel(ModelManager::GetIns()->GetModel(modelName));
		/*objectData->*/SetPosition(pos);
		/*objectData->*/SetRotation(rotate);
		/*objectData->*/SetScale(scale);

		Initialize();
		
	}

	~GameObjCommon();

	virtual void Initialize();		//初期化
	virtual void Update();			//毎フレーム更新
	virtual void FixedUpdate() {};	//定期更新
	virtual void LustUpdate() {};	//
	void ColliderUpdate();
	virtual void Move();			//移動量計算
	virtual void VelocityReset();								//移動量初期化
	virtual void VelocityReset(bool isStap, float gain = 0.9f);	//移動量初期化
	//変更を適応
	void Adaptation();
	//アニメーション再生
	void PlayAnimation(int playNumber, bool isLoop = false) { /*objectData->*/PlayAnimation(isLoop); };
	//描画
	virtual void Draw() const;

	////正面ベクトル取得
	//Vector3 GetFrontVector() { return Vector3(object->matWorldf._31, object->matWorldf._32, object->matWorldf._33).Normalize(); }
	////右ベクトル取得
	//Vector3 GetRightVector() { return Vector3(object->matWorldf._11, object->matWorldf._12, object->matWorldf._13).Normalize(); }
	////上ベクトル取得
	//Vector3 GetUpVector() { return Vector3(object->matWorldf._21, object->matWorldf._22, object->matWorldf._23).Normalize(); }

	//オブジェクト取得
	//std::unique_ptr<Object3d> &GetObjectDate() { return objectData; }

public:
	Vector3 pos;
	Vector3 velocity;	//移動量
	float gravityPow;	//重力の強さ

	float N;			//質量
	float e;			//反発係数



	bool isGravity;		//重力の影響	trueで重力の影響を受ける
	bool isActive;		//更新フラグ	falseだとUpdate()やColliderUpdate()の処理をを行わない
	bool isInvisible;	//非表示フラグ	trueで描画を行わない
	bool isDelete;		//消去フラグ	trueになるとそのフレームで消滅する

	std::string Tag;			//当たり判定などで使うオブジェクトタグ
	
	//当たり判定除外リスト
	std::vector<std::string> exclusionList;

protected:
	//std::unique_ptr<Object3d> objectData;	//オブジェクトデータ

public:	

	//ブロードフェイズコライダーのセット
	void SetBroadCollider(BaseCollider *collider);
	//ナローフェイズコライダーのセット
	void SetNarrowCollider(BaseCollider *collider);
	//ナローフェイズコライダーのゲット
	std::map<std::string, BaseCollider *> GetNarrowCollider() { return narrowColliders; }

	// 衝突時コールバック関数
	virtual void OnCollision(const CollisionInfo &info) {}
protected:
	//ブロードフェイズコライダー
	std::map<std::string, BaseCollider *> broadColliders;
	//厳密なコライダー
	std::map<std::string, BaseCollider *> narrowColliders;
	
};

