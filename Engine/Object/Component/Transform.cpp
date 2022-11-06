#include "Object/Component/Transform.h"
#include "yMath.h"
#include "ImGui/ImGuizmo.h"
#include "Object/GameObject/GameObject.h"
#include "Camera.h"

Transform::Transform() :
	Component("Transform",  ComponentType::Transform,true)
{
}

Transform::~Transform()
{
}

void Transform::Infomation()
{
    // ���Z�b�g�{�^��
    if(ImGui::Button("Reset"))
    {
        localMatrix = XMMatrixIdentity();
        matrix = XMMatrixIdentity();
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
    XMFLOAT3 snap{1,1,1};
    ImGuiIO &io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	// ���O�̃}�g���b�N�X
    auto next_matrix = world_matrix_;
    if(
	ImGuizmo::Manipulate(
        Camera::main.r_->viewMatrix->r->m128_f32,
        Camera::main.r_->projectionMatrix->r->m128_f32,
        mCurrentGizmoOperation, mCurrentGizmoMode, next_matrix.r->m128_f32, NULL, false ? &snap.x : NULL)
        )
    {
        matrix = next_matrix;
    }
    
    // ���W
    if(ImGui::DragFloat3("Position", (float *)&local_position_),0.2f)
    {
        UpdateMatrix();

    }

    // ��]
	Vector3 euler = local_quaternion_.EulerAngles() * Mathf::rad_to_deg;
    if (ImGui::DragFloat3("Rotation", &euler.x), 0.2f)
    {
        euler *= Mathf::deg_to_rad;
        local_quaternion_ = Quaternion::Euler(
			euler
        );
        UpdateMatrix();
    }
	// �X�P�[��
	if(ImGui::DragFloat3("Scale", (float *)&local_scale_), 0.2f)
    {
        UpdateMatrix();

    }
}

void Transform::ComponentUpdate()
{
    UpdateMatrix();
}

void Transform::ComponentDraw()
{
    
}

void Transform::RotateAround(Vector3 point, Vector3 axis, float deg)
{
    Quaternion q = Quaternion(axis, deg * Mathf::deg_to_rad);

    Vector3 distance{ world_position_ - point };
    
    Vector3 renge{
		q *distance
    };
    Vector3 assignment_position{
        point + renge
    };
    // ���[�J���s����v�Z
    XMMATRIX S = DirectX::XMMatrixScaling(
        world_scale_.x,
        world_scale_.y,
        world_scale_.z
    );
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(
        XMLoadFloat4(&quaternion)
    );

    XMMATRIX T = DirectX::XMMatrixTranslation(
        assignment_position.x,
        assignment_position.y,
        assignment_position.z
    );
    XMMATRIX temp = S * R * T;
    matrix = temp;
    //Quaternion angle_axis = Quaternion(axis, deg);
    
    //Vector3 position = transform_->position;

    //position -= point;
    //position = angle_axis * position;
    //position += point;

    //transform_->position = position;
}

void Transform::LookAt(const Vector3 &target)
{
    Vector3 front = (target - position).Normalized();
	Vector3 right = Vector3::Cross(Vector3::up, front).Normalized();
    Vector3 up = Vector3::Cross(front,right).Normalized();

    Matrix4x4 m
    {
        right.x,right.y,right.z,0.0f,
        up.x,up.y,up.z,0.0f,
        front.x,front.y,front.z,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };

    quaternion = Matrix4x4::GetRotation(m);
    // �N�I�[�^�j�I���̔��o
}

void Transform::UpdateMatrix()
{
    // ���[�J���s����v�Z
    XMMATRIX S = DirectX::XMMatrixScaling(
        local_scale_.x,
        local_scale_.y,
        local_scale_.z
    );
    //Matrix4x4 S = Matrix4x4::Scale(local_scale_);
    Matrix4x4 R = Matrix4x4::Rotate(local_quaternion_);
    Matrix4x4 T = Matrix4x4::Translation(local_position_);
    
    local_matrix_ = S * R * T;

    // �e������ꍇ�͐e�̍s���������
    if (user_set_parent_ != nullptr)
    {
        XMMATRIX user_parent_matrix_ = DirectX::XMLoadFloat4x4(user_set_parent_);
        world_matrix_ = local_matrix_ * user_parent_matrix_;

    }
    else if (parent_ != nullptr)
    {
        world_matrix_ = local_matrix_ * parent_->GetWorldMatrix();
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
    DirectX::XMMatrixDecompose(&scale_v, &rotate_v, &position_v, matrix);
    XMStoreFloat3(&scale, scale_v);
    XMStoreFloat4(&quaternion, rotate_v);
    XMStoreFloat3(&position, position_v);
}

DirectX::XMMATRIX Transform::InverseMatrixAllParent()
{
    XMMATRIX inverse = DirectX::XMMatrixIdentity();
    if (parent_ != nullptr)
    {
		inverse = DirectX::XMMatrixInverse(nullptr, parent_->world_matrix_);
        inverse *= parent_->InverseMatrixAllParent();
    }
    return inverse;
}
