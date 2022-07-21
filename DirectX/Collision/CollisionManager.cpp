#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"
#include "ScriptComponent.h"

using namespace DirectX;

//�����ł̏�����2ms�ȓ��Ɏ��߂�
void CollisionManager::CheckBroadCollisions(const std::vector<std::shared_ptr<GameObject>> &game_objects)
{ 
	// �I�u�W�F�N�g�̃C�e���[�^�[
	std::vector<std::shared_ptr<GameObject>>::const_iterator game_object_it_a;
	std::vector<std::shared_ptr<GameObject>>::const_iterator game_object_it_b;
	// �R���C�_�[�̃C�e���[�^�[
	std::vector<std::weak_ptr<BaseCollider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<BaseCollider>>::const_iterator collider_it_b;
	//std::forward_list<std::weak_ptr<BaseCollider>>::iterator itA;
	//std::forward_list<std::weak_ptr<BaseCollider>>::iterator itB;

	// �S�ẴI�u�W�F�N�g���`�F�b�N
	for (game_object_it_a = game_objects.begin(); game_object_it_a != game_objects.end(); ++game_object_it_a) {
		
		// B�I�u�W�F�N�g�̃C�e���[�^������炷
		game_object_it_b = game_object_it_a;
		++game_object_it_b;


		for (; game_object_it_b != game_objects.end(); ++game_object_it_b) {

			// �u���[�h�t�F�[�Y
			if (false) {
				// �u���[�h�t�F�[�Y���Փ˂��Ă��Ȃ���Ύ��̃I�u�W�F�N�g��
				continue;
			}

			// �i���[�t�F�[�Y
			for (collider_it_a = game_object_it_a->get()->GetColliders().begin();
				collider_it_a != game_object_it_a->get()->GetColliders().end();
				++collider_it_a) {

				collider_it_b = collider_it_a;
				++collider_it_b;

				// A�R���C�_�[
				std::weak_ptr<BaseCollider> colider_a = collider_it_a->lock();

				// �R���C�_�[���ݒ�͖�������
				if (colider_a.lock()->GetShapeType() == SHAPE_UNKNOWN) continue;
				
				for (collider_it_b = game_object_it_b->get()->GetColliders().begin();
					collider_it_b != game_object_it_b->get()->GetColliders().end();
					++collider_it_b) {

					// B�R���C�_�[
					std::weak_ptr<BaseCollider>  colider_b = collider_it_b->lock();
					
					// �R���C�_�[���ݒ�͖�������
					if (colider_b.lock()->GetShapeType() == SHAPE_UNKNOWN) continue;
					
					DirectX::XMVECTOR interBroad;
					if (CheckHitCollision(colider_a.lock().get(), colider_b.lock().get(), &interBroad)) {
						// �Փ˃R�[���o�b�N
						for (const auto &script_a : game_object_it_a->get()->GetScripts()) {
							dynamic_cast<ScriptComponent*>(script_a.get())->OnCollisionEnter();
						}
						
					}
				}

			}


		}
	}
	//// �Փ˔���
	//for (itA = broadColliders.begin(); itA != broadColliders.end(); ++itA)
	//{
	//	itB = itA;
	//	++itB;
	//	for (; itB != broadColliders.end(); ++itB)
	//	{
	//		BaseCollider *colABroad = itA->lock().get();
	//		BaseCollider *colBBroad = itB->lock().get();

	//		//�������m�̏Փ˂�A�R���C�_�[���ݒ�̎҂͖�������
	//		if (itB == itA ||
	//			colABroad->GetShapeType() == SHAPE_UNKNOWN ||
	//			colBBroad->GetShapeType() == SHAPE_UNKNOWN) {
	//			continue;
	//		}
	//		//���O���X�g�ɍڂ��Ă�����̂��p�X����
	//		/*if (colABroad->object->exclusionList.size() != 0) {
	//			auto result = std::find(
	//				colABroad->object->exclusionList.begin(),
	//				colABroad->object->exclusionList.end(),
	//				colBBroad->object->Tag);
	//			if (result != colABroad->object->exclusionList.end()) {
	//				continue;
	//			}
	//		}*/

	//		//�u���[�h�t�F�[�Y����
	//		DirectX::XMVECTOR interBroad;
	//		if (CheckHitCollision(colABroad, colBBroad, &interBroad)) {
	//			//��
	//			GameObject *a = colABroad->GetObject3d();
	//			GameObject *b = colBBroad->GetObject3d();
	//			//���v����
	//			float TotalN = a->N + b->N;
	//			//�����W�����ǂ�
	//			float RefRate = 1 + a->e * b->e;
	//			//�Փˎ�
	//			Vector3 Direction = Vector3(b->GetPos()) - Vector3(a->GetPos());
	//			Direction.Normalize();
	//			//����
	//			float Dot = Vector3(a->velocity - b->velocity).VDot(Direction);
	//			//�萔�x�N�g��
	//			Vector3 ConstVec = RefRate * Dot / TotalN * Direction;

	//			//�Փˌ�̑��x
	//			a->velocity = ConstVec * -b->N + a->velocity;
	//			b->velocity = ConstVec * a->N + b->velocity;
	//			//�Փˌ�̈ʒu
	//			a->pos = /*a->velocity + */(interBroad - Direction * dynamic_cast<Sphere*>(colABroad)->radius);
	//			b->pos = /*b->velocity + */(interBroad + Direction * dynamic_cast<Sphere *>(colBBroad)->radius);
	//			
	//			/*colABroad->OnCollision(CollisionInfo(colBBroad->GetObject3d(), colBBroad, interBroad, colABroad->GetCollisionName()));
	//			colBBroad->OnCollision(CollisionInfo(colABroad->GetObject3d(), colABroad, interBroad, colBBroad->GetCollisionName()));
	//			*/
	//			
	//			////�Փ˂��Ă���΃i���[�t�F�C�Y�̓����蔻��
	//			//std::map<std::string, BaseCollider *>::iterator itANarrow;
	//			//std::map<std::string, BaseCollider *>::iterator itBNarrow;
	//			//itANarrow = colABroad->GetObject3d()->GetNarrowCollider().begin();
	//			//for (;itANarrow != colABroad->GetObject3d()->GetNarrowCollider().end(); ++itANarrow) {
	//			//	itBNarrow = colBBroad->GetObject3d()->GetNarrowCollider().begin();
	//			//	for (; itBNarrow != colBBroad->GetObject3d()->GetNarrowCollider().end(); ++itBNarrow) {

	//			//		BaseCollider *colANarrow = itANarrow->second;
	//			//		BaseCollider *colBNarrow = itBNarrow->second;

	//			//		//�������m�̏Փ˂�A�R���C�_�[���ݒ�̎҂͖�������
	//			//		if (itBNarrow->second == itANarrow->second ||
	//			//			colANarrow->GetShapeType() == SHAPE_UNKNOWN ||
	//			//			colBNarrow->GetShapeType() == SHAPE_UNKNOWN) {
	//			//			continue;
	//			//		}

	//			//		DirectX::XMVECTOR interNarrow;
	//			//		//�i���[�t�F�C�Y�̏���
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

	//�u���[�h�Ɏg�p������̂���`�F�b�N
	//���Ƌ�
	if (collisions == SHAPE_SPHERE)
	{
		Sphere *SphereA = dynamic_cast<Sphere *>(colA);
		Sphere *SphereB = dynamic_cast<Sphere *>(colB);
		//����
		if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, inter))
		{
			return true;
		}
	}
	//AABB��AABB
	else if (collisions == SHAPE_AABB) {

	}

	//�قȂ�g�ݍ��킹
	//����AABB
	else if (collisions == (SHAPE_SPHERE | SHAPE_AABB)) {
		Sphere *sohereCollider;
		AABB *boxCollider;
		//�����l�ɕϊ����킹��
		if (colA->GetShapeType() == SHAPE_SPHERE) {
			sohereCollider = dynamic_cast<Sphere *>(colA);
			boxCollider = dynamic_cast<AABB *>(colB);
		}
		else {
			sohereCollider = dynamic_cast<Sphere *>(colB);
			boxCollider = dynamic_cast<AABB *>(colA);
		}
		DirectX::XMVECTOR inter;
		//����
		if (Collision::CheckSphere2AABB(*sohereCollider, *boxCollider)) {
			return true;
		}
	}
	//���ƃ��b�V��
	else if (collisions == (SHAPE_SPHERE | SHAPE_MESH))
	{
		MeshCollider *meshCollider;
		Sphere *sphere;
		//�����l�ɕϊ����킹��
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


	//�J�v�Z���n
	//�J�v�Z����AABB
	else if (collisions == (SHAPE_CAPSULE | SHAPE_AABB)) {
		Capsule *capsuleCollider;
		Box *boxCollider;
		//�����l�ɕϊ����킹��
		if (colA->GetShapeType() == SHAPE_CAPSULE) {
			capsuleCollider = dynamic_cast<Capsule *>(colA);
			boxCollider = dynamic_cast<Box *>(colB);
		}
		else {
			capsuleCollider = dynamic_cast<Capsule *>(colB);
			boxCollider = dynamic_cast<Box *>(colA);
		}
		DirectX::XMVECTOR inter;
		//����
		if (Collision::CheckCapsule2Box(*capsuleCollider, *boxCollider)) {
			return true;
		}
	}
}

//bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
//{
//	//�S�����L���ɂ��đ����ł����s
//	return Raycast(ray, 0xffff, hitInfo, maxDistance);
//}
//
//bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
//{
//	bool result = false;
//	//�����p�̃C�e���[�^
//	std::forward_list<BaseCollider*>::iterator it;
//	//���܂łōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
//	std::forward_list<BaseCollider*>::iterator it_hit;
//	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
//	float distance = maxDistance;
//	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
//	XMVECTOR inter;
//
//	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
//	it = colliders.begin();
//	for (; it != colliders.end(); ++it)
//	{
//		BaseCollider* colA = *it;
//
//
//		//���̏ꍇ
//		if (colA->GetShapeType() == SHAPE_SPHERE)
//		{
//			Sphere* sphere = dynamic_cast<Sphere*>(colA);
//
//			float tempDistance;
//			XMVECTOR tempInter;
//
//			//������Ȃ���Ώ��O
//			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
//			//�������ŏ��łȂ���Ώ��O
//			if (tempDistance >= distance) continue;
//			//���܂łōł��߂��̂ŋL�^�����
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
//	//�ŏI�I�ɂȂɂ��ɓ������Ă����猋�ʂ���������
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

	//// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	//it = broadColliders.begin();
	//for (; it != broadColliders.end(); ++it)
	//{
	//	BaseCollider *col = *it;

	//	// ��
	//	if (col->GetShapeType() == SHAPE_SPHERE)
	//	{
	//		Sphere *sphereB = dynamic_cast<Sphere *>(col);

	//		XMVECTOR tempInter;
	//		XMVECTOR tempReject;
	//		if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

	//		// ���������Z�b�g
	//		QueryHit info;
	//		info.collider = col;
	//		info.object = col->GetObject3d();
	//		info.inter = tempInter;
	//		info.reject = tempReject;

	//		// �N�G���[�R�[���o�b�N�Ăяo��
	//		if (!callback->OnQueryHit(info))
	//		{
	//			// �߂�l��false�̏ꍇ�A�p�������I��
	//			return;
	//		}
	//	}
	//	// ���b�V��
	//	else if (col->GetShapeType() == SHAPE_MESH)
	//	{
	//		MeshCollider *meshCollider = dynamic_cast<MeshCollider *>(col);

	//		XMVECTOR tempInter;
	//		XMVECTOR tempReject;
	//		if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

	//		// ���������Z�b�g
	//		QueryHit info;
	//		info.collider = col;
	//		info.object = col->GetObject3d();
	//		info.inter = tempInter;
	//		info.reject = tempReject;

	//		// �N�G���[�R�[���o�b�N�Ăяo��
	//		if (!callback->OnQueryHit(info))
	//		{
	//			// �߂�l��false�̏ꍇ�A�p�������I��
	//			return;
	//		}
	//	}
	//}
}