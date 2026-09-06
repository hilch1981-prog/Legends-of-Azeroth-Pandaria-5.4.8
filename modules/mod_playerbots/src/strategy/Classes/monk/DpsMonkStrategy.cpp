#include "DpsMonkStrategy.h"

#include "Playerbots.h"

NextAction** DpsMonkStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("rising sun kick", ACTION_DEFAULT + 0.5f),
        new NextAction("tiger palm", ACTION_DEFAULT + 0.4f),
        new NextAction("blackout kick", ACTION_DEFAULT + 0.3f),
        new NextAction("jab", ACTION_DEFAULT + 0.2f),
        new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void DpsMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMonkStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("no stance of the fierce tiger", NextAction::array(0, new NextAction("stance of the fierce tiger", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("tigereye brew ready", NextAction::array(0, new NextAction("tigereye brew", ACTION_HIGH + 6), nullptr)));
    triggers.push_back(new TriggerNode("rising sun kick", NextAction::array(0, new NextAction("rising sun kick", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("fists of fury", NextAction::array(0, new NextAction("fists of fury", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("spinning crane kick", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("touch of karma", ACTION_HIGH + 5), new NextAction("expel harm", ACTION_HIGH + 4), nullptr)));
}
