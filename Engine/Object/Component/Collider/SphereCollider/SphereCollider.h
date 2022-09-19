#pragma once
#include "../Collider.h"
class SphereCollider final :
    public Collider
{
public:
    SphereCollider(Vector3 center = {0,0,0}, float radius = 0.0f);
    /// <summary>
    /// ������
    /// </summary>
    void ComponentInitialize() override;

    /// <summary>
    /// ���
    /// </summary>
    void Infomation() override;

    
    // �I�u�W�F�N�g�̍��W�ɑ΂���I�t�Z�b�g
    yEngin::Property<Vector3> center
    {
        center_,
        yEngin::AccessorType::AllAccess
    };
    // ���a
    yEngin::Property<float> radius
    {
        radius_,
        yEngin::AccessorType::AllAccess
    };

private:
    // �I�u�W�F�N�g�̍��W�ɑ΂���I�t�Z�b�g
    Vector3 center_;
    // ���a
	float radius_;
};
