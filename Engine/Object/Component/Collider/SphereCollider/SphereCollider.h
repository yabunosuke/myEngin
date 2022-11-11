#pragma once
#include "../Engine/Collision/Primitive.h"
#include "../Collider.h"
class SphereCollider final :
    public Collider,
    public yEngine::Sphere
{
public:
    SphereCollider(float radius = 1.0f, Vector3 center = { 0,0,0 });
    /// <summary>
    /// 初期化
    /// </summary>
    void ComponentUpdate() override;

    /// <summary>
    /// 情報
    /// </summary>
    void Infomation() override;

    yEngine::Property<Vector3> offset
    {
	    &local_center_,
        yEngine::AccessorType::AllAccess
    };

private:
    Vector3 local_center_{ 0,0,0 };   // ローカルの中央
    float local_radius_{1.0f};  // ローカルの直径
};

