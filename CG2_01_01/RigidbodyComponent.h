#pragma once
#include "Component.h"
#include <DirectXMath.h>

struct Regidbody {
    float mass;         // 質量
    float drag;         // 力で動く場合の空気抵抗
    float angular_drag; // トルクで動く場合の空気抵抗
    bool use_gravity;   // 重力を使用するか

    struct FreezePosition { // ワールド座標の指定した軸でリジッドボディの移動を停止
        bool x;
        bool y;
        bool z;
    }freeze_position;

    struct FreezeRotation { // ローカル座標の指定した軸でリジッドボディの回転を停止
        bool x;
        bool y;
        bool z;
    }freeze_rotation;
};

class RigidbodyComponent :
    public Component
{
public:

private:
    // リジッドボディ
    Regidbody regidody;

};

