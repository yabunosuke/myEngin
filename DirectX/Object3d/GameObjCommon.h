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
public://�T�u�N���X
	enum RenderQueue {			//�����_�[�L���[
		BackGround =	1000,	//�w�i
		Geometry =		2000,	//�s�����I�u�W�F�N�g
		AlphaTest =		2450,	//�A���t�@�e�X�g
		Transparent =	3000,	//�A���t�@�u�����h�p
		Overlay =		4000,	//�G�t�F�N�g�Ȃ�
	};
public:
	GameObjCommon(
		std::string modelName,				//���f���̖��O
		const std::string tag = "Notag",	//���ʗp�^�O
		bool isGravity = false,				//�d�͏����̗L��
		Vector3 pos = { 0,0,0 },			//�����ʒu
		Vector3 scale = { 1,1,1 },			//�����T�C�Y
		Vector3 rotate = { 0,0,0 }			//������]
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

	virtual void Initialize();		//������
	virtual void Update();			//���t���[���X�V
	virtual void FixedUpdate() {};	//����X�V
	virtual void LustUpdate() {};	//
	void ColliderUpdate();
	virtual void Move();			//�ړ��ʌv�Z
	virtual void VelocityReset();								//�ړ��ʏ�����
	virtual void VelocityReset(bool isStap, float gain = 0.9f);	//�ړ��ʏ�����
	//�ύX��K��
	void Adaptation();
	//�A�j���[�V�����Đ�
	void PlayAnimation(int playNumber, bool isLoop = false) { /*objectData->*/PlayAnimation(isLoop); };
	//�`��
	virtual void Draw() const;

	////���ʃx�N�g���擾
	//Vector3 GetFrontVector() { return Vector3(object->matWorldf._31, object->matWorldf._32, object->matWorldf._33).Normalize(); }
	////�E�x�N�g���擾
	//Vector3 GetRightVector() { return Vector3(object->matWorldf._11, object->matWorldf._12, object->matWorldf._13).Normalize(); }
	////��x�N�g���擾
	//Vector3 GetUpVector() { return Vector3(object->matWorldf._21, object->matWorldf._22, object->matWorldf._23).Normalize(); }

	//�I�u�W�F�N�g�擾
	//std::unique_ptr<Object3d> &GetObjectDate() { return objectData; }

public:
	Vector3 pos;
	Vector3 velocity;	//�ړ���
	float gravityPow;	//�d�͂̋���

	float N;			//����
	float e;			//�����W��



	bool isGravity;		//�d�͂̉e��	true�ŏd�͂̉e�����󂯂�
	bool isActive;		//�X�V�t���O	false����Update()��ColliderUpdate()�̏��������s��Ȃ�
	bool isInvisible;	//��\���t���O	true�ŕ`����s��Ȃ�
	bool isDelete;		//�����t���O	true�ɂȂ�Ƃ��̃t���[���ŏ��ł���

	std::string Tag;			//�����蔻��ȂǂŎg���I�u�W�F�N�g�^�O
	
	//�����蔻�菜�O���X�g
	std::vector<std::string> exclusionList;

protected:
	//std::unique_ptr<Object3d> objectData;	//�I�u�W�F�N�g�f�[�^

public:	

	//�u���[�h�t�F�C�Y�R���C�_�[�̃Z�b�g
	void SetBroadCollider(BaseCollider *collider);
	//�i���[�t�F�C�Y�R���C�_�[�̃Z�b�g
	void SetNarrowCollider(BaseCollider *collider);
	//�i���[�t�F�C�Y�R���C�_�[�̃Q�b�g
	std::map<std::string, BaseCollider *> GetNarrowCollider() { return narrowColliders; }

	// �Փˎ��R�[���o�b�N�֐�
	virtual void OnCollision(const CollisionInfo &info) {}
protected:
	//�u���[�h�t�F�C�Y�R���C�_�[
	std::map<std::string, BaseCollider *> broadColliders;
	//�����ȃR���C�_�[
	std::map<std::string, BaseCollider *> narrowColliders;
	
};

