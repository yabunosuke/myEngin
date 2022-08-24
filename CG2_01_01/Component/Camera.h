#pragma once
#include "Transform.h"
#include "Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	Camera(
		std::weak_ptr<CameraManager> camera_manager
	);

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

private:
	// �]���p�̃J�����f�[�^
	std::shared_ptr<CameraDeta> camera_date_;

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
	}projection_type_;

	float fov_of_view_		{ 60.0f };			// �J�����̃r���[�p�x
	float view_point_size_	{ 5.0f };			// Orthographic�ݒ肵���ꍇ�̃T�C�Y
	float near_plane_		{ 0.3f };			// �`�悳���ł��߂��_
	float far_plane_		{ 1000.0f };		// �`�悳���ł������_
	float focus_			{ 1.0f };			// �œ_����;
};

