#pragma once
#include "../Component.h"
class Behaviour :
    public Component
{
public:

    //===========================================
    //
    //		コンストラクタ
    //
    //===========================================

    Behaviour(const std::string &name, ComponentType component_type = ComponentType::Behaviour);

    void ComponentFixedUpdate() override;
    void ComponentUpdate() override;

	void ImGuiDraw() override;


    yEngin::Property<bool> enabled
    {
        enabled_,
        yEngin::AccessorType::AllAccess
    };

private:
    // 更新フラグ
    bool enabled_ = true;
    bool is_active_;
};

