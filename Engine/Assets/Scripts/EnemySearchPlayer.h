#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "SpikeEnemy.h"

class EnemySearchPlayer :
    public MonoBehaviour
{
public:

    EnemySearchPlayer();

    /// <summary>
   /// 離れた瞬間に呼び出される
   /// </summary>
	void OnTriggerExit(Collider &other) override;
    /// <summary>
    /// 振れている間呼び出される
    /// </summary>
	void OnTriggerStay(Collider &other) override;

    void Start() override;

private:
    SpikeEnemy *spike_enemy_;
};

