#pragma once
#include <DirectXMath.h>
#include "Math/Vector3.h"
using namespace DirectX;

class Camera
{
public:
	Camera(Vector3 eye, Vector3 target = { 0,0,0 }, Vector3 up = { 0,1,0 });
	//�r���[�s��X�V
	void UpdateViewMatrix();
	//�ˉe�s��X�V
	void UpdateProjectionMatrix();

	static void Initialize();
	static void SetCam(Camera *cam);
	//�J�������擾
	static Camera *GetCam() { return NowCamera; }

	//�r���[�s����擾
	const XMMATRIX &GetViewMatrix() { return matView; }
	//�ˉe�s����擾
	const XMMATRIX &GetProjectionMatrix() { return matProjection; }
	//�r���[�ˉe�s����擾
	const XMMATRIX &GetViewProjectionMatrix() { return matViewProjection; }


	Vector3 eye;				//���_���W
	Vector3 target;			//�����_���W
	Vector3 up;				//������x�N�g��
private:
	//���ݎg�p���Ă���J����
	static Camera *NowCamera;

private:
	//�r���[�s��
	XMMATRIX matView = XMMatrixIdentity();
	//�ˉe�s��
	XMMATRIX matProjection = XMMatrixIdentity();
	//�r���[�ˉe�s��
	XMMATRIX matViewProjection = XMMatrixIdentity();
	//�r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	//�ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	//�A�X�y�N�g��
	float aspectRatio = 1.0f;

};

