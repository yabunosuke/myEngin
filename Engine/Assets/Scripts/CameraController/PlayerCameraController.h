#pragma once
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "Object/Component/Camera.h"
#include "Math/Vector2.h"

class PlayerCameraController :
    public MonoBehaviour
{
public:
    PlayerCameraController();
    void Start() override;
	void Infomation() override;

    void UpdateCameraSpin();
    void UpdateCameraLook(const Transform &player);
    void FixedUpdateCameraPosition(const Transform &player);
    void DashCameraFov();
    void DefaultCameraFov();
private:
    Transform *rotation_root_{ nullptr };
    Transform *height_root_{ nullptr };
    Camera *main_camera_{ nullptr };

    // カメラ上下制限
    Vector2 height_limit_min_max{ -1.0f,3.0f };
    // カメラ回転変化スピード
    float rotation_speed_ = 0.1f;
    // カメラ高さ変化スピード
    float height_speed_ = 0.1f;
    // カメラ
    const float k_defo_fov_{ 60.0f };
    float fov_speed_{ 30.0f };
};

