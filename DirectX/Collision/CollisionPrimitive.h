#pragma once
#include "Vector3.h"
#include <DirectXMath.h>

struct Sphere //��
{
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;
};

struct Plane //����
{
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};

class Triangle //�@���t���̎O�p�`
{
public:
	//���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//�@���x�N�g��
	DirectX::XMVECTOR normal;

public:
	//�@���̌v�Z
	void ComputeNormal();
};

struct Ray //���C(������)
{
	//�n�_���W
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };
	//����
	DirectX::XMVECTOR dir = { 1, 0, 0, 0 };
};

struct Capsule //�J�v�Z��
{
	//�ŏ��̋��̒��S���W
	Vector3 startPosition;
	//�Ō�̋��̒��S���W
	Vector3 endPosition;
	//���a
	float radius = 1.0f;
};


//
//���AABB�ɏC������B
//
struct Box //��
{
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
};

//�����s���E�{�b�N�X
struct AABB {
	//���S���W
	Vector3 center = { 0,0,0 };
	//�W�_���W
	Vector3 length = { 1,1,1 };
};


//2D
struct BOX2D {
	float Bottom = 1.0f;
	float Top	 = 1.0f;
	float Right	 = 1.0f;
	float Left	 = 1.0f;
};
enum EdgeType
{
	EdgeTypeNon = -1,
	EdgeTypeLeft,
	EdgeTypeRight,
	EdgeTypeTop,
	EdgeTypeBottom,
	EdgeTypeMax,
};