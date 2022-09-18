#pragma once
#include "../Collider.h"
class SphereCollider final :
    public Collider
{
public:
    SphereCollider(Vector3 center = {0,0,0}, float radius = 0.0f);
    /// <summary>
    /// 初期化
    /// </summary>
    void ComponentInitialize() override;

    /// <summary>
    /// 情報
    /// </summary>
    void Infomation() override;
private:
    // オブジェクトの座標に対するオフセット
    Vector3 center_;
    // 半径
	float radius_;
};

