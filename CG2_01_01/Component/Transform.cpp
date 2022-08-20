#include "Component/Transform.h"
#include "yMath.h"
#include "ImGui/ImGuizmo.h"
#include  "Camera.h"
#include "GameObject.h"

Transform::Transform() :
	Component("Transform",  ComponentID::TRANSFORM,true)
{
}

void Transform::Infomation()
{
    // リセットボタン
    if(ImGui::Button("Reset"))
    {
        position_ = { 0,0,0 };		// 座標
        quaternion_ = { 0,1,0,0, };		// クオータニオン回転
        scale_ = { 1,1,1 };		// 拡大

        UpdateMatrix();
    }

  //  // ギズモ
  //  static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);    
  //  static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
  //  if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
  //      mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
  //  ImGui::SameLine();
  //  if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
  //      mCurrentGizmoOperation = ImGuizmo::ROTATE;
  //  ImGui::SameLine();
  //  if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
  //      mCurrentGizmoOperation = ImGuizmo::SCALE;
  //  
  //  static bool useSnap(false);
  //  ImGui::Checkbox("", &useSnap);
  //  ImGui::SameLine();
  //  XMFLOAT3 snap;
  //  switch (mCurrentGizmoOperation)
  //  {
  //  case ImGuizmo::TRANSLATE:
  //      snap = {1,1,1};
  //      ImGui::InputFloat3("Snap", &snap.x);
  //      break;
  //  case ImGuizmo::ROTATE:
  //      snap = { 1,1,1 };
  //      ImGui::InputFloat("Angle Snap", &snap.x);
  //      break;
  //  case ImGuizmo::SCALE:
  //      snap = { 1,1,1 };
  //      ImGui::InputFloat("Scale Snap", &snap.x);
  //      break;
  //  }
  //  ImGuiIO &io = ImGui::GetIO();
  //  ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

  //  // 親がいなければif
  //  if (parent->GetPearent().use_count() == 0)
  //  {
		//ImGuizmo::Manipulate(Camera::GetCam()->GetViewMatrix().r->m128_f32, Camera::GetCam()->GetProjectionMatrix().r->m128_f32, mCurrentGizmoOperation, mCurrentGizmoMode, world_matrix_.r->m128_f32, NULL, useSnap ? &snap.x : NULL);
  //      XMVECTOR scale, rotate, position;
  //      XMMatrixDecompose(&scale, &rotate, &position, local_matrix_);
  //      XMStoreFloat3(&scale_, scale);
  //      XMStoreFloat4(&quaternion_, rotate);
  //      XMStoreFloat3(&position_, position);

  //      // 座標
  //      ImGui::DragFloat3("Position", (float *)&position_);
  //      // 回転
  //      XMFLOAT3 euler = QuaternionToEuler(quaternion_);
  //      ImGui::DragFloat3("Rotation", &euler.x);
  //      XMStoreFloat4(&quaternion_,
  //          XMQuaternionRotationRollPitchYaw(
  //              euler.x * Mathf::deg_to_rad,
  //              euler.y * Mathf::deg_to_rad,
  //              euler.z * Mathf::deg_to_rad
  //          )
  //      );
  //      // スケール
  //      ImGui::DragFloat3("Scale", (float *)&scale_);
  //      UpdateMatrix();

  //  }
  //  /*else
  //  {
  //      ImGuizmo::Manipulate(Camera::GetCam()->GetViewMatrix().r->m128_f32, Camera::GetCam()->GetProjectionMatrix().r->m128_f32, mCurrentGizmoOperation, mCurrentGizmoMode, world_matrix_.r->m128_f32, NULL, useSnap ? &snap.x : NULL);
  //  }*/
  //  ////
  //  //XMVECTOR scale, rotate, position;
  //  //XMMatrixDecompose(&scale, &rotate, &position, local_matrix_);
  //  //XMStoreFloat3(&scale_, scale);
  //  //XMStoreFloat4(&quaternion_, rotate);
  //  //XMStoreFloat3(&position_, position);

  //  //// 座標
  //  //ImGui::DragFloat3("Position", (float *)&position_);
  //  //// 回転
  //  //XMFLOAT3 euler = QuaternionToEuler(quaternion_);
  //  //ImGui::DragFloat3("Rotation", &euler.x);
  //  //XMStoreFloat4(&quaternion_,
  //  //    XMQuaternionRotationRollPitchYaw(
  //  //        euler.x * Mathf::deg_to_rad,
  //  //        euler.y * Mathf::deg_to_rad,
  //  //        euler.z * Mathf::deg_to_rad
  //  //    )
  //  //);
  //  //// スケール
  //  //ImGui::DragFloat3("Scale", (float *)&scale_);

  //  //UpdateMatrix();
  //  
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
    // ローカル更新
    // 座標変換行列
    XMMATRIX S = DirectX::XMMatrixScaling(
        scale_.x,
        scale_.y,
        scale_.z
    );
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(
        XMLoadFloat4(&quaternion_)
    );

    XMMATRIX T = DirectX::XMMatrixTranslation(
        position_.x,
        position_.y,
        position_.z
    );
    local_matrix_ = S * R * T;

    // ワールド座標更新
    if (parent->GetPearent().lock() != nullptr)
    {
        world_matrix_ = local_matrix_ * parent->GetPearent().lock()->GetComponent<Transform>()->world_matrix_;
    }
    else
    {
        world_matrix_ = local_matrix_;
    }
}
