#pragma once
#include "DirectXCommon.h"
#include "Transform.h"
#include "Object/Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	//===========================================
	//
	//		�R���X�g���N�^
	//
	//===========================================

	Camera();
	~Camera();

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

	//===========================================
	//
	//		�ÓI�֐�
	//
	//===========================================

	static void SetCameraManager(CameraManager *camera_manager);

	//===========================================
	//
	//		�A�N�Z�b�T
	//
	//===========================================

	//static DirectX::XMMATRIX GetProjection() { return  now_camera->projection_matrix; }
	//static DirectX::XMMATRIX GetView() { return  now_camera->view_matrix; }

	
	static yEngine::Property<Camera*> main;

	DirectX::XMFLOAT4 view_position;
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	yEngine::Property<DirectX::XMFLOAT4> viewPosition{
		&view_position, yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	yEngine::Property<DirectX::XMMATRIX> viewMatrix{
		&view_matrix,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	yEngine::Property<DirectX::XMMATRIX> projectionMatrix{
		&projection_matrix, yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};

private:

	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	// ���ݗL���ȃJ�����i�J�����R���e�i�̒��ň�ԏ�ɂ���L���ȃJ�����j
	static Camera* main_camera_;
	// �J�����R���e�i
	static CameraManager *camera_manager_;

	//===========================================
	//
	//		�����o�ϐ�
	//
	//===========================================


	// ��ʃN���A���@
	enum class ClearFlag
	{
		SkyBox,			
		SoildColor,
		DepthOnly,
		DontColor
	}clear_flag_;
	
	XMFLOAT4 background_color_	{ 0,0,1,1 };	// �w�i�̐F

	enum ProjectionType			// ���e���@
	{
		Perspective,			// �������e�ϊ�
		Orthographic			// ���t���e�ϊ�
	}projection_type_ = Perspective;

	float fov_of_view_		{ 60.0f };			// �J�����̃r���[�p�x
	float view_point_size_	{ 5.0f };			// Orthographic�ݒ肵���ꍇ�̃T�C�Y
	float near_plane_		{ 0.3f };			// �`�悳���ł��߂��_
	float far_plane_		{ 1000.0f };		// �`�悳���ł������_
	float focus_			{ 1.0f };			// �œ_����;

};

