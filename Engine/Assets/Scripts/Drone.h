#pragma once
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"


class Drone :
    public MonoBehaviour
{
public:

    Drone(Vector3 *axis_pos);
    void Start() override;
    void FixedUpdate() override;
    void Update() override;
private:
    Vector3 *axis_pos_;
};

