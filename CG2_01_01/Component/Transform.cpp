#include "Component/Transform.h"
#include "yMath.h"
#include "ImGui/ImGuizmo.h"
#include  "Camera.h"

Transform::Transform() :
	Component("Transform",  ComponentID::TRANSFORM)
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
        matrix_ = S * R * T;
    }

    // ギズモ
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);    
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(90))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
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
    if (ImGui::IsKeyPressed(83))
        useSnap = !useSnap;
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
    ImGuizmo::Manipulate(Camera::GetCam()->GetViewMatrix().r->m128_f32, Camera::GetCam()->GetProjectionMatrix().r->m128_f32, mCurrentGizmoOperation, mCurrentGizmoMode, matrix_.r->m128_f32, NULL, useSnap ? &snap.x : NULL);

    //
    XMVECTOR scale, rotate, position;
    XMMatrixDecompose(&scale, &rotate, &position, matrix_);
    XMStoreFloat3(&scale_, scale);
    XMStoreFloat4(&quaternion_,rotate);
    XMStoreFloat3(&position_, position);

    // 座標
    ImGui::DragFloat3("Position", (float *)&position_);
    // 回転
	XMFLOAT3 euler = QuaternionToEuler(quaternion_);
    ImGui::DragFloat3("Rotation", &euler.x);
    XMStoreFloat4(&quaternion_,
        XMQuaternionRotationRollPitchYaw(
            DegToRad(euler.x),
            DegToRad(euler.y),
                DegToRad(euler.z)
        )
    );
    // スケール
	ImGui::DragFloat3("Scale", (float *)&scale_);

    
}

void Transform::ComponentUpdate()
{
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
	matrix_ = S * R * T;
}

void Transform::ComponentDraw()
{
    XMMATRIX test = XMMatrixIdentity();
    
}
