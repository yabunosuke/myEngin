#pragma once
#include "../Component.h"
class Behaviour :
    public Component
{
private:
    // 更新フラグ
    bool enabled_;
    bool is_active_;
};

