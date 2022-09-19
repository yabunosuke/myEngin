#pragma once
#include "../Collider.h"
class SphereCollider final :
    public Collider
{
public:
    SphereCollider(float radius = 1.0f, Vector3 center = { 0,0,0 });
    /// <summary>
    /// 初期化
    /// </summary>
    void ComponentInitialize() override;

    /// <summary>
    /// 情報
    /// </summary>
    void Infomation() override;

    
    // オブジェクトの座標に対するオフセット
    yEngin::Property<Vector3> center
    {
        center_,
        yEngin::AccessorType::AllAccess
    };
    // 半径
    yEngin::Property<float> radius
    {
        radius_,
        yEngin::AccessorType::AllAccess
    };

private:
    // オブジェクトの座標に対するオフセット
    Vector3 center_;
    // 半径
	float radius_;
};

