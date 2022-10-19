#pragma once
#include "../Engine/Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
#include "../Engine/Object/Component/Camera.h"
class CameraController :
    public MonoBehaviour
{
public:
    CameraController(GameObject *target_object);
    void Start() override;
    void FixedUpdate() override;
    void Update() override;

private:
    
    Camera *camera_;
    // �Ǐ]�Ώ�
    GameObject *target_object_;
    Vector3 target_position_;

    const float radius_distance_{ 100.0f };
    Vector3 rotate_axis_{0,1,0};
};
