#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(Model* model)
{
	////�O�p�`���X�g���N���A
	//triangles.clear();
	////���f���̎����b�V�����X�g���擾
	//const std::vector<std::unique_ptr<Mesh>>& meshes = model->GetMeshes();
	////���݂̃��b�V���̊J�n�O�p�`�ԍ������Ă����ϐ�(0�ŏ�����)
	//int start = 0;
	////�S���b�V���ɂ��ď��ɏ�������
	//std::vector<std::unique_ptr<Mesh>>::const_iterator it = meshes.cbegin();
	//for (; it != meshes.cend(); ++it)
	//{
	//	Mesh* mesh = (*it).get();
	//	const std::vector<Mesh::VertexPosNormalUv>& vertices = mesh->GetVertices();
	//	const std::vector<unsigned short>& indices = mesh->GetIndices();
	//	//�C���f�b�N�X�́A�O�p�`�̐�x3����̂ŁA
	//	//�������烁�b�V�����̎O�p�`�̐����t�Z����
	//	size_t triangleNum = indices.size() / 3;
	//	//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
	//	triangles.resize(triangles.size() + triangleNum);
	//	//�S�O�p�`�ɂ��ď��ɏ�������
	//	for (int i = 0; i < triangleNum; i++)
	//	{
	//		//������v�Z����O�p�`�̎Q��
	//		Triangle& tri = triangles[start + i];
	//		int idx0 = indices[i * 3 + 0];
	//		int idx1 = indices[i * 3 + 1];
	//		int idx2 = indices[i * 3 + 2];
	//		//�O�p�`��3���_�̍��W����
	//		tri.p0 = {
	//			vertices[idx0].pos.x,
	//			vertices[idx0].pos.y,
	//			vertices[idx0].pos.z,
	//			1 };

	//		tri.p1 = {
	//			vertices[idx1].pos.x,
	//			vertices[idx1].pos.y,
	//			vertices[idx1].pos.z,
	//			1 };

	//		tri.p2 = {
	//			vertices[idx2].pos.x,
	//			vertices[idx2].pos.y,
	//			vertices[idx2].pos.z,
	//			1 };
	//		//3���_����@�����v�Z
	//		tri.ComputeNormal();
	//	}
	//	//���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
	//	start += (int)triangleNum;
	//}
}

void MeshCollider::Update()
{
	//���[���h�s��̋t�s����v�Z
	invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	// �I�u�W�F�N�g�̃��[�J�����W�n�ł̋��𓾂�i���a��X�X�P�[�����Q��)
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];
	//���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		//���ƎO�p�`�̓����蔻��
		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter, reject))
		{
			if (inter)
			{
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				//���[���h���W�n�ł̌�_�𓾂�
				*inter = XMVector3Transform(*inter, matWorld);
			}
			if (reject)
			{
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				//���[���h���W�n�ł̔r�˃x�N�g���ɕϊ�
				*reject = XMVector3TransformNormal(*reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
{
	// �I�u�W�F�N�g�̃��[�J�����W�n�ł̃��C�𓾂�
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);
	//���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;

		XMVECTOR tempInter;
		//���C�ƎO�p�`�̓����蔻��
		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter))
		{

			const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
			//���[���h���W�n�ł̌�_�𓾂�
			tempInter = XMVector3Transform(tempInter, matWorld);

			if (distance)
			{
				//��_�ƃ��C�n�_�̋������v�Z
				XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
			}

			if (inter)
			{
				*inter = tempInter;
			}

			return true;
		}
	}

	return false;
}