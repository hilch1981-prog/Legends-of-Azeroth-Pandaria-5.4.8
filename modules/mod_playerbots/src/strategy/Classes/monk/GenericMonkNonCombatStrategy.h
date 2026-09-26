#ifndef _PLAYERBOT_GENERICMONKNONCOMBATSTRATEGY_H
#define _PLAYERBOT_GENERICMONKNONCOMBATSTRATEGY_H

#include "NonCombatStrategy.h"

class PlayerbotAI;

class GenericMonkNonCombatStrategy : public NonCombatStrategy
{
public:
    GenericMonkNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI) {}
    void InitTriggers(std::vector<TriggerNode*>& triggers) override;
    std::string const getName() override { return "nc"; }
};

#endif
