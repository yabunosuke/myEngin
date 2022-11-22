#include "RectTransform.h"

#include "WinApp.h"

RectTransform::RectTransform()
{
    name = "Rect Transform";
}

void RectTransform::Infomation()
{
    // リセットボタン
    if (ImGui::Button("Reset"))
    {
        localMatrix = XMMatrixIdentity();
    }
    // 座標
    if (ImGui::DragFloat3("Position", (float *)&local_position_, 0.2f))
    {
        UpdateMatrix();
    }

    // 回転
    Vector3 euler = local_quaternion_.EulerAngles() * Mathf::rad_to_deg;
    if (ImGui::DragFloat3("Rotation", (float *)&euler, 0.2f))
    {
        euler *= Mathf::deg_to_rad;
        local_quaternion_ = Quaternion::Euler(
            euler
        );
        UpdateMatrix();
    }
    // スケール
    if (ImGui::DragFloat3("Scale", (float *)&local_scale_, 0.2f))
    {
        UpdateMatrix();

    }
}

void RectTransform::UpdateMatrix()
{
        // ローカル行列を計算
        XMMATRIX S = DirectX::XMMatrixScaling(
            local_scale_.x,
            local_scale_.y,
            local_scale_.z
        );
        //Matrix4x4 S = Matrix4x4::Scale(local_scale_);
        Matrix4x4 R = Matrix4x4::Rotate(local_quaternion_);
        Matrix4x4 T = Matrix4x4::Translation(
			{
				local_position_.x + (ancheor_positions_[static_cast<int>(ancher_point_)].x * WinApp::windowWidth),
			local_position_.y + (ancheor_positions_[static_cast<int>(ancher_point_)].y * WinApp::windowHeight),
			local_position_.z
			});

        local_matrix_ = S * R * T;

        // 親がいる場合は親の行列をかける
        if (user_set_parent_ != nullptr)
        {
            XMMATRIX user_parent_matrix_ = DirectX::XMLoadFloat4x4(user_set_parent_);
            world_matrix_ = local_matrix_ * user_parent_matrix_;

        }
        else if (parent_ != nullptr)
        {
            world_matrix_ = local_matrix_ * parent_->matrix;
        }
        else
        {
            world_matrix_ = local_matrix_;
        }

        // 取得用のワールドを再計算
        MatrixDecompose(world_matrix_, world_scale_, world_quaternion_, world_position_);
}
