#pragma once
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"
class PlayerBullet:
    public MonoBehaviour
{
public:
    PlayerBullet(const Vector3 &start_position, const Quaternion &q);

    void Start() override;
    void FixedUpdate() override;
private:
    Vector3 start_position_;
    Quaternion start_q_;
    Rigidbody *rigidbody_;
    float live_timer_{0.0f};
};

