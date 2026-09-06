#include "GenericMonkStrategy.h"

#include "Playerbots.h"

GenericMonkStrategy::GenericMonkStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

void GenericMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    CombatStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("enemy out of melee", NextAction::array(0, new NextAction("reach melee", ACTION_MOVE + 8), nullptr)));
    triggers.push_back(new TriggerNode("not facing target", NextAction::array(0, new NextAction("set facing", ACTION_MOVE + 7), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("expel harm", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("fortifying brew", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("spear hand strike", NextAction::array(0, new NextAction("spear hand strike", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("touch of death", NextAction::array(0, new NextAction("touch of death", ACTION_HIGH + 8), nullptr)));
}

MonkAoeStrategy::MonkAoeStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI) {}

void MonkAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    CombatStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("light aoe", NextAction::array(0, new NextAction("spinning crane kick", ACTION_HIGH), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("spinning crane kick", ACTION_HIGH + 2), nullptr)));
}

void MonkCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode("cure poison", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure poison on party", NextAction::array(0, new NextAction("detox poison on party", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease on party", NextAction::array(0, new NextAction("detox disease on party", ACTION_DISPEL), nullptr)));
}
