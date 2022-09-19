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
	void ImGuiDraw() override;

private:
    // 更新フラグ
    bool enabled_;
    bool is_active_;
};

