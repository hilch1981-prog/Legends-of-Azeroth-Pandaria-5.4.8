#include "HealMonkStrategy.h"

#include "Playerbots.h"

NextAction** HealMonkStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("soothing mist on party", ACTION_DEFAULT + 0.4f),
        new NextAction("renewing mist on party", ACTION_DEFAULT + 0.3f),
        new NextAction("tiger palm", ACTION_DEFAULT + 0.2f),
        new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void HealMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMonkStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0, new NextAction("life cocoon on party", ACTION_EMERGENCY), new NextAction("surging mist on party", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0, new NextAction("enveloping mist on party", ACTION_HIGH + 5), new NextAction("surging mist on party", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0, new NextAction("renewing mist on party", ACTION_HIGH + 3), new NextAction("soothing mist on party", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium group heal setting", NextAction::array(0, new NextAction("revival", ACTION_HIGH + 6), new NextAction("uplift", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("mana tea", ACTION_HIGH + 4), nullptr)));
}
