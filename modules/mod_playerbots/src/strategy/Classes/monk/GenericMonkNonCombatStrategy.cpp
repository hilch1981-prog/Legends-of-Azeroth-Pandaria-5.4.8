#include "GenericMonkNonCombatStrategy.h"

#include "Playerbots.h"

void GenericMonkNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("cure poison", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure poison on party", NextAction::array(0, new NextAction("detox poison on party", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease on party", NextAction::array(0, new NextAction("detox disease on party", ACTION_DISPEL), nullptr)));
}
