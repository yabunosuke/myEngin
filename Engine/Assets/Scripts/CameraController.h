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
    void Infomation() override;

private:
    
    Camera *camera_;
    // í«è]ëŒè€
    GameObject *target_object_;
    Vector3 target_position_;

    float radius_distance_{ 2.0f };
    Vector3 rotate_axis_{0,1,0};
};

