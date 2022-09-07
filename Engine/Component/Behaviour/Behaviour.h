#pragma once
#include "Object/Component/Component.h"
#include "Property.h"

class Behaviour :
    public Component
{
public:

private:
    bool enabled_;      // 更新フラグ


public: //アクセッサ

    /// <summary>
    /// アタッチされたゲームオブジェクトが有効かつコンポーネントが有効か
    /// </summary>
    Property<bool>isActiveAndEnabled{
    	enabled_ ,AccessorType::ReadOnly,
        [this]() {return enabled_; },
    };
};

