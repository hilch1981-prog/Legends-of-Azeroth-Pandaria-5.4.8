#ifndef _PLAYERBOT_HEALMONKSTRATEGY_H
#define _PLAYERBOT_HEALMONKSTRATEGY_H

#include "GenericMonkStrategy.h"

class PlayerbotAI;

class HealMonkStrategy : public GenericMonkStrategy
{
public:
    HealMonkStrategy(PlayerbotAI* botAI) : GenericMonkStrategy(botAI) {}
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "heal"; }
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_RANGED | STRATEGY_TYPE_HEAL; }
};

#endif
