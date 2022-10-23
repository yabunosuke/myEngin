#pragma once
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"


class Drone :
    public MonoBehaviour
{
public:

    Drone();
    void Start() override;
    void FixedUpdate() override;
    void Update() override;
private:

};

