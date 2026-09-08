#ifndef _PLAYERBOT_TANKMONKSTRATEGY_H
#define _PLAYERBOT_TANKMONKSTRATEGY_H

#include "GenericMonkStrategy.h"

class PlayerbotAI;

class TankMonkStrategy : public GenericMonkStrategy
{
public:
    TankMonkStrategy(PlayerbotAI* botAI) : GenericMonkStrategy(botAI) {}
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "tank"; }
    NextAction** getDefaultActions() override;
    uint32 GetType() const override { return STRATEGY_TYPE_TANK | STRATEGY_TYPE_MELEE; }
};

#endif
