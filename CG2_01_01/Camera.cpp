#include "Camera.h"
#include "WinApp.h"
Camera *Camera::NowCamera = nullptr;

Camera::Camera(Vector3 eye, Vector3 target, Vector3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;

	//�A�X�y�N�g��
	aspectRatio = static_cast<float>(WinApp::windowWidth) / static_cast<float>(WinApp::windowHeight);

	//�r���[�s��v�Z
	UpdateViewMatrix();
	//�ˉe�s��v�Z
	UpdateProjectionMatrix();
	//����
	matViewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target), 
		XMLoadFloat3(&up));
}

void Camera::UpdateProjectionMatrix()
{
	matProjection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1000.0f
	);
}

void Camera::Initialize()
{
	NowCamera = new Camera({ 0, 100, 100 });
	NowCamera->matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Vector3{0,100,100}),
		XMLoadFloat3(&Vector3{0,0,0}),
		XMLoadFloat3(&Vector3{ 0,1,0 })
	);
}

void Camera::SetCam(Camera *cam)
{
	NowCamera = cam;
}
