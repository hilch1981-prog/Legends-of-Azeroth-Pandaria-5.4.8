#ifndef _PLAYERBOT_DPSMONKSTRATEGY_H
#define _PLAYERBOT_DPSMONKSTRATEGY_H

#include "GenericMonkStrategy.h"

class PlayerbotAI;

class DpsMonkStrategy : public GenericMonkStrategy
{
public:
    DpsMonkStrategy(PlayerbotAI* botAI) : GenericMonkStrategy(botAI) {}
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "melee"; }
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_COMBAT | STRATEGY_TYPE_DPS | STRATEGY_TYPE_MELEE; }
};

#endif
