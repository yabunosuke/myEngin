#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "SpikeEnemy.h"

class EnemySearchPlayer :
    public MonoBehaviour
{
public:

    EnemySearchPlayer();

    /// <summary>
   /// ���ꂽ�u�ԂɌĂяo�����
   /// </summary>
	void OnTriggerExit(Collider &other) override;
    /// <summary>
    /// �U��Ă���ԌĂяo�����
    /// </summary>
	void OnTriggerStay(Collider &other) override;

    void Start() override;

private:
    SpikeEnemy *spike_enemy_;
};

