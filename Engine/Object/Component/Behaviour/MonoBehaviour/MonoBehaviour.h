#pragma once
#include "Object/Component/Behaviour/Behaviour.h"

class MonoBehaviour :
    public Behaviour
{
public:
    virtual void Awake(){};
    virtual void FixedUpadate(){};
    virtual void LateUpdate();
    virtual void Reset(){};
    virtual void Start(){};
    virtual void Update(){};
};

