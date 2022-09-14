#pragma once

#include "CollisionInfo.h"
//#include "GameObjCommon.h"
#include "Object/GameObject/GameObject.h"
#include <string>


//�Փ˔���`��̌^��`
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // ���ݒ�

	//3d
	SHAPE_POINT		= 0x00001,	//�_
	SHAPE_RAY		= 0x00002,	//���i�I�[�Ȃ��j
	SHAPE_LINE		= 0x00004,	//���i�I�[����j
	SHAPE_TRIANGLE	= 0x00008,	//�O�p�`
	SHAPE_SPHERE	= 0x00010,	//��
	SHAPE_CYLINDER	= 0x00020,	//��
	SHAPE_AABB		= 0x00040,	//AABB
	SHAPE_OBB		= 0x00080,	//OBB
	SHAPE_CAPSULE	= 0x00100,	//�J�v�Z��
	SHAPE_MESH		= 0x00200,	//���b�V��
	SHAPE_PLANE		= 0x00400,	//��������
	// = 0x000	//�R���W�����^�C�v

	BOX_2D			= 0x10000,
};

class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;		//���z�f�X�g���N�^

	//�I�u�W�F�N�g���Z�b�g
	inline void SetObject(std::weak_ptr<GameObject> object) { this->object = object; }
	//�Z�b�g����Ă���I�u�W�F�N�g�����^�[��
	inline std::weak_ptr<GameObject> GetObject3d() { return object; }

	//�X�V
	virtual void Update() = 0;

	// �R���C�_�[�`��p
	virtual void Draw() = 0;

	/// <summary>
	/// ImGui�p
	/// </summary>
	virtual void Infomation();

	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType; }
	inline std::string GetCollisionName() { return collisionName; }

	//�Փˎ��R�[���o�b�N�֐�
	inline void OnCollision(const CollisionInfo& info)
	{
		//object->OnCollision(info);
	}

	// �����蔻�葮�����Z�b�g
	inline void SetAttribute(std::string collision_name)
	{
		this->collisionName = collision_name;
	}

	// �����蔻�葮�����폜
	inline void RemoveAttribute(std::string tag)
	{
		this->collisionName  = nullptr;
	}

protected:
	std::weak_ptr<GameObject> object;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//�����蔻��^�O
	std::string collisionName;
};