#include "Component/Transform.h"
#include "yMath.h"
#include "ImGui/ImGuizmo.h"
#include "Object/GameObject/GameObject.h"
#include "Camera.h"


Transform::Transform() :
	Component("Transform",  ComponentType::TRANSFORM,true)
{
}

void Transform::Infomation()
{
    // ���Z�b�g�{�^��
    if(ImGui::Button("Reset"))
    {
        local_position_ = { 0,0,0 };		// ���W
        local_quaternion_ = { 0,1,0,0, };		// �N�I�[�^�j�I����]
        local_scale_ = { 1,1,1 };		// �g��

        UpdateMatrix();
    }

    // �M�Y��
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);    
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
   
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;


    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }
    static bool useSnap(false);
    ImGui::Checkbox("", &useSnap);
    ImGui::SameLine();
    XMFLOAT3 snap;
    switch (mCurrentGizmoOperation)
    {
    case ImGuizmo::TRANSLATE:
        snap = {1,1,1};
        ImGui::InputFloat3("Snap", &snap.x);
        break;
    case ImGuizmo::ROTATE:
        snap = { 1,1,1 };
        ImGui::InputFloat("Angle Snap", &snap.x);
        break;
    case ImGuizmo::SCALE:
        snap = { 1,1,1 };
        ImGui::InputFloat("Scale Snap", &snap.x);
        break;
    }
    ImGuiIO &io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	// ���O�̃}�g���b�N�X
    XMMATRIX old_matrix = world_matrix_;
    ImGuizmo::Manipulate(
        Camera::main->lock()->viewMatrix->r->m128_f32,
        Camera::main->lock()->projectionMatrix->r->m128_f32,
        mCurrentGizmoOperation, mCurrentGizmoMode, world_matrix_.r->m128_f32, NULL, useSnap ? &snap.x : NULL);
    
    // �e������ꍇ�̓��[�J���ɕϊ����Ȃ���
    if (user_set_parent_ != nullptr)
    {
        XMVECTOR scale, rotate, position;
        XMMatrixDecompose(&scale, &rotate, &position, world_matrix_);

        XMVECTOR old_scale, old_rotate, old_position;
        XMMatrixDecompose(&old_scale, &old_rotate, &old_position, old_matrix);

        // �������ړ�����
        XMVECTOR difference_scale = old_scale - scale;
        local_scale_ += difference_scale;

        XMStoreFloat4(&local_quaternion_, XMQuaternionMultiply(XMLoadFloat4(&local_quaternion_), XMQuaternionMultiply(old_rotate, XMQuaternionInverse(rotate))));

        XMVECTOR difference_position = old_position - position;
        local_position_ += difference_position;

        // ���W
        ImGui::DragFloat3("Position", (float *)&local_position_);
        // ��]
        XMFLOAT3 euler = QuaternionToEuler(local_quaternion_);
        ImGui::DragFloat3("Rotation", &euler.x);
        XMStoreFloat4(&local_quaternion_,
            XMQuaternionRotationRollPitchYaw(
                euler.x * Mathf::deg_to_rad,
                euler.y * Mathf::deg_to_rad,
                euler.z * Mathf::deg_to_rad
            )
        );
        // �X�P�[��
        ImGui::DragFloat3("Scale", (float *)&local_scale_);

    }
    else if(!parent_.expired())
    {
		XMVECTOR scale, rotate, position;
		XMMatrixDecompose(&scale, &rotate, &position, world_matrix_);

        XMVECTOR old_scale, old_rotate, old_position;
        XMMatrixDecompose(&old_scale, &old_rotate, &old_position, old_matrix);

        // �������ړ�����
        XMVECTOR difference_scale = old_scale - scale;
        local_scale_ += difference_scale;

        XMStoreFloat4(&local_quaternion_,XMQuaternionMultiply(XMLoadFloat4(&local_quaternion_),XMQuaternionMultiply(old_rotate , XMQuaternionInverse(rotate))));

        XMVECTOR difference_position = old_position - position;
        local_position_ += difference_position;

        // ���W
        ImGui::DragFloat3("Position", (float *)&local_position_);
        // ��]
        XMFLOAT3 euler = QuaternionToEuler(local_quaternion_);
        ImGui::DragFloat3("Rotation", &euler.x);
        XMStoreFloat4(&local_quaternion_,
            XMQuaternionRotationRollPitchYaw(
                euler.x * Mathf::deg_to_rad,
                euler.y * Mathf::deg_to_rad,
                euler.z * Mathf::deg_to_rad
            )
        );
        // �X�P�[��
        ImGui::DragFloat3("Scale", (float *)&local_scale_);
    }
    // ��������ԏ�̎�
    else
    {
        XMVECTOR scale, rotate, position;
        XMMatrixDecompose(&scale, &rotate, &position, world_matrix_);
        XMStoreFloat3(&local_scale_, scale);
        XMStoreFloat4(&local_quaternion_, rotate);
        XMStoreFloat3(&local_position_, position);

        //MatrixDecompose(world_matrix_, local_scale_, local_quaternion_, local_position_);

        // ���W
        ImGui::DragFloat3("Position", (float *)&local_position_);
        // ��]
        XMFLOAT3 euler = QuaternionToEuler(local_quaternion_);
        ImGui::DragFloat3("Rotation", &euler.x);
        XMStoreFloat4(&local_quaternion_,
            XMQuaternionRotationRollPitchYaw(
                euler.x * Mathf::deg_to_rad,
                euler.y * Mathf::deg_to_rad,
                euler.z * Mathf::deg_to_rad
            )
        );
        // �X�P�[��
        ImGui::DragFloat3("Scale", (float *)&local_scale_);
    }

    
}

void Transform::ComponentUpdate()
{
    UpdateMatrix();
}

void Transform::ComponentDraw()
{
    
}

void Transform::UpdateMatrix()
{
    //�e�̏��
    if (game_object_->GetPearent().lock().use_count() > 0 &&
        !parent_.expired())
    {
        parent_ = game_object_->GetPearent().lock()->GetComponent<Transform>();
    }
    // ���[�J���s����v�Z
    XMMATRIX S = DirectX::XMMatrixScaling(
        local_scale_.x,
        local_scale_.y,
        local_scale_.z
    );
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(
        XMLoadFloat4(&local_quaternion_)
    );

    XMMATRIX T = DirectX::XMMatrixTranslation(
        local_position_.x,
        local_position_.y,
        local_position_.z
    );
    local_matrix_ = S * R * T;

    // �e������ꍇ�͐e�̍s���������
    if (user_set_parent_ != nullptr)
    {
        XMMATRIX user_parent_matrix_ = DirectX::XMLoadFloat4x4(user_set_parent_);
        world_matrix_ = local_matrix_ * user_parent_matrix_;

    }
    else if (!parent_.expired())
    {
        world_matrix_ = local_matrix_ * parent_.lock()->GetWorldMatrix();
    }
    else
    {
        world_matrix_ = local_matrix_;
    }

    // �擾�p�̃��[���h���Čv�Z
    {
        MatrixDecompose(world_matrix_, world_scale_, world_quaternion_, world_position_);
    }
    
}

void Transform::MatrixDecompose(const XMMATRIX &matrix, XMFLOAT3 &scale, XMFLOAT4 &quaternion, XMFLOAT3 &position)
{
    XMVECTOR scale_v, rotate_v, position_v;
    XMMatrixDecompose(&scale_v, &rotate_v, &position_v, matrix);
    XMStoreFloat3(&scale, scale_v);
    XMStoreFloat4(&quaternion, rotate_v);
    XMStoreFloat3(&position, position_v);
}
