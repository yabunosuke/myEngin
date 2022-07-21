#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"
#include "ScriptComponent.h"

using namespace DirectX;

//ここでの処理を2ms以内に収める
void CollisionManager::CheckBroadCollisions(const std::vector<std::shared_ptr<GameObject>> &game_objects)
{ 
	// オブジェクトのイテレーター
	std::vector<std::shared_ptr<GameObject>>::const_iterator game_object_it_a;
	std::vector<std::shared_ptr<GameObject>>::const_iterator game_object_it_b;
	// コライダーのイテレーター
	std::vector<std::weak_ptr<BaseCollider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<BaseCollider>>::const_iterator collider_it_b;
	//std::forward_list<std::weak_ptr<BaseCollider>>::iterator itA;
	//std::forward_list<std::weak_ptr<BaseCollider>>::iterator itB;

	// 全てのオブジェクトをチェック
	for (game_object_it_a = game_objects.begin(); game_object_it_a != game_objects.end(); ++game_object_it_a) {
		
		// Bオブジェクトのイテレータを一つずらす
		game_object_it_b = game_object_it_a;
		++game_object_it_b;


		for (; game_object_it_b != game_objects.end(); ++game_object_it_b) {

			// ブロードフェーズ
			if (false) {
				// ブロードフェーズが衝突していなければ次のオブジェクトへ
				continue;
			}

			// ナローフェーズ
			for (collider_it_a = game_object_it_a->get()->GetColliders().begin();
				collider_it_a != game_object_it_a->get()->GetColliders().end();
				++collider_it_a) {

				collider_it_b = collider_it_a;
				++collider_it_b;

				// Aコライダー
				std::weak_ptr<BaseCollider> colider_a = collider_it_a->lock();

				// コライダー未設定は無視する
				if (colider_a.lock()->GetShapeType() == SHAPE_UNKNOWN) continue;
				
				for (collider_it_b = game_object_it_b->get()->GetColliders().begin();
					collider_it_b != game_object_it_b->get()->GetColliders().end();
					++collider_it_b) {

					// Bコライダー
					std::weak_ptr<BaseCollider>  colider_b = collider_it_b->lock();
					
					// コライダー未設定は無視する
					if (colider_b.lock()->GetShapeType() == SHAPE_UNKNOWN) continue;
					
					DirectX::XMVECTOR interBroad;
					if (CheckHitCollision(colider_a.lock().get(), colider_b.lock().get(), &interBroad)) {
						// 衝突コールバック
						for (const auto &script_a : game_object_it_a->get()->GetScripts()) {
							dynamic_cast<ScriptComponent*>(script_a.get())->OnCollisionEnter();
						}
						
					}
				}

			}


		}
	}
	//// 衝突判定
	//for (itA = broadColliders.begin(); itA != broadColliders.end(); ++itA)
	//{
	//	itB = itA;
	//	++itB;
	//	for (; itB != broadColliders.end(); ++itB)
	//	{
	//		BaseCollider *colABroad = itA->lock().get();
	//		BaseCollider *colBBroad = itB->lock().get();

	//		//自分同士の衝突や、コライダー未設定の者は無視する
	//		if (itB == itA ||
	//			colABroad->GetShapeType() == SHAPE_UNKNOWN ||
	//			colBBroad->GetShapeType() == SHAPE_UNKNOWN) {
	//			continue;
	//		}
	//		//除外リストに載っているものをパスする
	//		/*if (colABroad->object->exclusionList.size() != 0) {
	//			auto result = std::find(
	//				colABroad->object->exclusionList.begin(),
	//				colABroad->object->exclusionList.end(),
	//				colBBroad->object->Tag);
	//			if (result != colABroad->object->exclusionList.end()) {
	//				continue;
	//			}
	//		}*/

	//		//ブロードフェーズ判定
	//		DirectX::XMVECTOR interBroad;
	//		if (CheckHitCollision(colABroad, colBBroad, &interBroad)) {
	//			//仮
	//			GameObject *a = colABroad->GetObject3d();
	//			GameObject *b = colBBroad->GetObject3d();
	//			//合計質量
	//			float TotalN = a->N + b->N;
	//			//反発係数もどき
	//			float RefRate = 1 + a->e * b->e;
	//			//衝突軸
	//			Vector3 Direction = Vector3(b->GetPos()) - Vector3(a->GetPos());
	//			Direction.Normalize();
	//			//内積
	//			float Dot = Vector3(a->velocity - b->velocity).VDot(Direction);
	//			//定数ベクトル
	//			Vector3 ConstVec = RefRate * Dot / TotalN * Direction;

	//			//衝突後の速度
	//			a->velocity = ConstVec * -b->N + a->velocity;
	//			b->velocity = ConstVec * a->N + b->velocity;
	//			//衝突後の位置
	//			a->pos = /*a->velocity + */(interBroad - Direction * dynamic_cast<Sphere*>(colABroad)->radius);
	//			b->pos = /*b->velocity + */(interBroad + Direction * dynamic_cast<Sphere *>(colBBroad)->radius);
	//			
	//			/*colABroad->OnCollision(CollisionInfo(colBBroad->GetObject3d(), colBBroad, interBroad, colABroad->GetCollisionName()));
	//			colBBroad->OnCollision(CollisionInfo(colABroad->GetObject3d(), colABroad, interBroad, colBBroad->GetCollisionName()));
	//			*/
	//			
	//			////衝突していればナローフェイズの当たり判定
	//			//std::map<std::string, BaseCollider *>::iterator itANarrow;
	//			//std::map<std::string, BaseCollider *>::iterator itBNarrow;
	//			//itANarrow = colABroad->GetObject3d()->GetNarrowCollider().begin();
	//			//for (;itANarrow != colABroad->GetObject3d()->GetNarrowCollider().end(); ++itANarrow) {
	//			//	itBNarrow = colBBroad->GetObject3d()->GetNarrowCollider().begin();
	//			//	for (; itBNarrow != colBBroad->GetObject3d()->GetNarrowCollider().end(); ++itBNarrow) {

	//			//		BaseCollider *colANarrow = itANarrow->second;
	//			//		BaseCollider *colBNarrow = itBNarrow->second;

	//			//		//自分同士の衝突や、コライダー未設定の者は無視する
	//			//		if (itBNarrow->second == itANarrow->second ||
	//			//			colANarrow->GetShapeType() == SHAPE_UNKNOWN ||
	//			//			colBNarrow->GetShapeType() == SHAPE_UNKNOWN) {
	//			//			continue;
	//			//		}

	//			//		DirectX::XMVECTOR interNarrow;
	//			//		//ナローフェイズの処理
	//			//		if (CheckHitCollision(colANarrow, colBNarrow,&interNarrow)) {
	//			//			colANarrow->OnCollision(CollisionInfo(colBNarrow->GetObject3d(), colBNarrow, interNarrow, colANarrow->GetCollisionName()));
	//			//			colBNarrow->OnCollision(CollisionInfo(colANarrow->GetObject3d(), colANarrow, interNarrow, colBNarrow->GetCollisionName()));
	//			//		}
	//			//	}
	//			//}
	//			
	//		}
	//	}
	//}
} 

bool CollisionManager::CheckHitCollision(BaseCollider *colA, BaseCollider *colB, DirectX::XMVECTOR *inter)
{
	int collisions = colA->GetShapeType() | colB->GetShapeType();

	//ブロードに使用するものからチェック
	//球と球
	if (collisions == SHAPE_SPHERE)
	{
		Sphere *SphereA = dynamic_cast<Sphere *>(colA);
		Sphere *SphereB = dynamic_cast<Sphere *>(colB);
		//判定
		if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, inter))
		{
			return true;
		}
	}
	//AABBとAABB
	else if (collisions == SHAPE_AABB) {

	}

	//異なる組み合わせ
	//球とAABB
	else if (collisions == (SHAPE_SPHERE | SHAPE_AABB)) {
		Sphere *sohereCollider;
		AABB *boxCollider;
		//引数様に変換合わせる
		if (colA->GetShapeType() == SHAPE_SPHERE) {
			sohereCollider = dynamic_cast<Sphere *>(colA);
			boxCollider = dynamic_cast<AABB *>(colB);
		}
		else {
			sohereCollider = dynamic_cast<Sphere *>(colB);
			boxCollider = dynamic_cast<AABB *>(colA);
		}
		DirectX::XMVECTOR inter;
		//判定
		if (Collision::CheckSphere2AABB(*sohereCollider, *boxCollider)) {
			return true;
		}
	}
	//球とメッシュ
	else if (collisions == (SHAPE_SPHERE | SHAPE_MESH))
	{
		MeshCollider *meshCollider;
		Sphere *sphere;
		//引数様に変換合わせる
		if (colA->GetShapeType() == SHAPE_MESH) {
			meshCollider = dynamic_cast<MeshCollider *>(colA);
			sphere = dynamic_cast<Sphere *>(colB);
		}
		else {
			meshCollider = dynamic_cast<MeshCollider *>(colB);
			sphere = dynamic_cast<Sphere *>(colA);
		}
		DirectX::XMVECTOR inter;
		if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {

			return true;
		}
	}


	//カプセル系
	//カプセルとAABB
	else if (collisions == (SHAPE_CAPSULE | SHAPE_AABB)) {
		Capsule *capsuleCollider;
		Box *boxCollider;
		//引数様に変換合わせる
		if (colA->GetShapeType() == SHAPE_CAPSULE) {
			capsuleCollider = dynamic_cast<Capsule *>(colA);
			boxCollider = dynamic_cast<Box *>(colB);
		}
		else {
			capsuleCollider = dynamic_cast<Capsule *>(colB);
			boxCollider = dynamic_cast<Box *>(colA);
		}
		DirectX::XMVECTOR inter;
		//判定
		if (Collision::CheckCapsule2Box(*capsuleCollider, *boxCollider)) {
			return true;
		}
	}
}

//bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
//{
//	//全属性有効にして属性版を実行
//	return Raycast(ray, 0xffff, hitInfo, maxDistance);
//}
//
//bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
//{
//	bool result = false;
//	//走査用のイテレータ
//	std::forward_list<BaseCollider*>::iterator it;
//	//今までで最も近いコライダーを記録する為のイテレータ
//	std::forward_list<BaseCollider*>::iterator it_hit;
//	//今までで最も近いコライダーの距離を記録する変数
//	float distance = maxDistance;
//	//今までで最も近いコライダーとの交点を記録する変数
//	XMVECTOR inter;
//
//	// 全てのコライダーと総当りチェック
//	it = colliders.begin();
//	for (; it != colliders.end(); ++it)
//	{
//		BaseCollider* colA = *it;
//
//
//		//球の場合
//		if (colA->GetShapeType() == SHAPE_SPHERE)
//		{
//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
//
//			float tempDistance;
//			XMVECTOR tempInter;
//
//			//当たらなければ除外
//			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
//			//距離が最小でなければ除外
//			if (tempDistance >= distance) continue;
//			//今までで最も近いので記録を取る
//			result = true;
//			distance = tempDistance;
//			inter = tempInter;
//			it_hit = it;
//		}
//		else if (colA->GetShapeType() == SHAPE_MESH)
//		{
//			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
//		
//			float tempDistance;
//			DirectX::XMVECTOR tempInter;
//			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
//			if (tempDistance >= distance) continue;
//		
//			result = true;
//			distance = tempDistance;
//			inter = tempInter;
//			it_hit = it;
//		}
//	}
//
//	//最終的になにかに当たっていたら結果を書き込む
//	if (result && hitInfo)
//	{
//		hitInfo->distance = distance;
//		hitInfo->inter = inter;
//		hitInfo->collider = *it_hit;
//		hitInfo->object = hitInfo->collider->GetObject3d();
//	}
//
//	return result;
//}

void CollisionManager::QuerySphere(const Sphere &sphere, QueryCallback *callback)
{
	//assert(callback);

	//std::forward_list<BaseCollider *>::iterator it;

	//// 全てのコライダーと総当りチェック
	//it = broadColliders.begin();
	//for (; it != broadColliders.end(); ++it)
	//{
	//	BaseCollider *col = *it;

	//	// 球
	//	if (col->GetShapeType() == SHAPE_SPHERE)
	//	{
	//		Sphere *sphereB = dynamic_cast<Sphere *>(col);

	//		XMVECTOR tempInter;
	//		XMVECTOR tempReject;
	//		if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

	//		// 交差情報をセット
	//		QueryHit info;
	//		info.collider = col;
	//		info.object = col->GetObject3d();
	//		info.inter = tempInter;
	//		info.reject = tempReject;

	//		// クエリーコールバック呼び出し
	//		if (!callback->OnQueryHit(info))
	//		{
	//			// 戻り値がfalseの場合、継続せず終了
	//			return;
	//		}
	//	}
	//	// メッシュ
	//	else if (col->GetShapeType() == SHAPE_MESH)
	//	{
	//		MeshCollider *meshCollider = dynamic_cast<MeshCollider *>(col);

	//		XMVECTOR tempInter;
	//		XMVECTOR tempReject;
	//		if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

	//		// 交差情報をセット
	//		QueryHit info;
	//		info.collider = col;
	//		info.object = col->GetObject3d();
	//		info.inter = tempInter;
	//		info.reject = tempReject;

	//		// クエリーコールバック呼び出し
	//		if (!callback->OnQueryHit(info))
	//		{
	//			// 戻り値がfalseの場合、継続せず終了
	//			return;
	//		}
	//	}
	//}
}