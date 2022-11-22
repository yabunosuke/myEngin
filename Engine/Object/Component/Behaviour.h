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


    yEngine::Property<bool> enabled
    {
        &enabled_,
        yEngine::AccessorType::AllAccess
    };

private:
    // 更新フラグ
    bool enabled_ = true;
    bool is_active_;
};

