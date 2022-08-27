#pragma once
#include "DirectXCommon.h"
#include "Transform.h"
#include "Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	Camera();

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

	static void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
	);

	//===========================================
	//
	//		�A�N�Z�b�T
	//
	//===========================================
	Property<std::weak_ptr<Camera>> main{
		main_camera_, AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
private:

	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	// ���ݗL���ȃJ����
	static std::weak_ptr<Camera> main_camera_;
	// �J�����R���e�i
	static std::vector<std::weak_ptr<Camera>> cameras_;


	//===========================================
	//
	//		�ÓI�����o�ϐ�
	//
	//===========================================

	DirectX::XMFLOAT4 view_position;
	DirectX::XMMATRIX mat_view;
	DirectX::XMMATRIX mat_projection;

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

