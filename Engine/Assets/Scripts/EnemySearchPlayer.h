#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include "SpikeEnemy.h"

class EnemySearchPlayer :
    public MonoBehaviour
{
public:

    EnemySearchPlayer();

    /// <summary>
   /// —£‚ê‚½uŠÔ‚ÉŒÄ‚Ño‚³‚ê‚é
   /// </summary>
	void OnTriggerExit(Collider &other) override;
    /// <summary>
    /// U‚ê‚Ä‚¢‚éŠÔŒÄ‚Ño‚³‚ê‚é
    /// </summary>
	void OnTriggerStay(Collider &other) override;

    void Start() override;

private:
    SpikeEnemy *spike_enemy_;
};

