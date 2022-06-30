#include "GameObjCommon.h"
#include "DirectXCommon.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

GameObjCommon::~GameObjCommon()
{
	// �����蔻��X�V
	//�u���[�h�t�F�C�Y�R���C�_�[����X�V
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			CollisionManager::GetInstance()->RemoveBroadCollider(iter->second);
			++iter;
		}
	}
}

void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{

}

void GameObjCommon::ColliderUpdate()
{
	//�u���[�h�t�F�C�Y�R���C�_�[����X�V
	if (broadColliders.size() != 0)
	{
		auto iter = broadColliders.begin();
		while (iter != broadColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
	//�i���[�t�F�C�Y�R���C�_�[����X�V
	if (narrowColliders.size() != 0)
	{
		auto iter = narrowColliders.begin();
		while (iter != narrowColliders.end()) {
			iter->second->Update();
			++iter;
		}
	}
}

void GameObjCommon::Move()
{
	if (isGravity) {
		velocity.y -= gravityPow;
	}
	position += velocity;

}

void GameObjCommon::VelocityReset()
{
	velocity = 0;
}

void GameObjCommon::VelocityReset(bool isStap, float gain)
{
	if (isStap) {
		velocity = 0;
	}
	else {
		velocity *= gain;
	}
}


void GameObjCommon::Adaptation()
{

	//objectData->SetScale(scale);
	//objectData->SetPosition(pos);
	//objectData->SetRotation(rotate);

	Object3d::Update();


	ColliderUpdate();
}

void GameObjCommon::Draw() const
{
	if (isInvisible) return;	//�����ȂƂ��͕`�悵�Ȃ�
	Object3d::Draw(DirectXCommon::cmdList);
}

void GameObjCommon::SetBroadCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	broadColliders[collider->GetCollisionName()] = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(collider);
	//�s��̍X�V
	//objectData->UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}


void GameObjCommon::SetNarrowCollider(BaseCollider *collider)
{
	collider->SetObject(this);
	narrowColliders[collider->GetCollisionName()] = collider;
	collider->Update();
}