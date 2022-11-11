#pragma once
#include "../Engine/Collision/Primitive.h"
#include "../Collider.h"
class OBBCollider final :
    public Collider,
    public yEngine::OBB
{
public:
    OBBCollider(Quaternion q = {0.0f,0.0f,0.0f,0.0f}, Vector3 extent = { 1.0f, 1.0f, 1.0f }, Vector3 center = { 0.0f,0.0f,0.0f });
    void ComponentUpdate() override;

    void Infomation() override;
private:
    Vector3 local_center_{ 0,0,0 };             // ローカルの中央
    Vector3 local_extent_{ 1.0f,1.0f,1.0f };    // ローカルの大きさ
    Quaternion local_quaternion_{};             // ローカルの回転
};

