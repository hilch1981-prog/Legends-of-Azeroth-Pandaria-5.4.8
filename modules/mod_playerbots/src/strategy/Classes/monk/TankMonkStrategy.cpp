#include "TankMonkStrategy.h"

#include "Playerbots.h"

NextAction** TankMonkStrategy::getDefaultActions()
{
    return NextAction::array(0,
        new NextAction("keg smash", ACTION_DEFAULT + 0.5f),
        new NextAction("tiger palm", ACTION_DEFAULT + 0.4f),
        new NextAction("blackout kick", ACTION_DEFAULT + 0.3f),
        new NextAction("jab", ACTION_DEFAULT + 0.2f),
        new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void TankMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMonkStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode("no stance of the sturdy ox", NextAction::array(0, new NextAction("stance of the sturdy ox", ACTION_HIGH + 9), nullptr)));

    // Target-core spell_monk.cpp confirms Blackout Kick applies/extends Shuffle for Brewmaster.
    triggers.push_back(new TriggerNode("no shuffle", NextAction::array(0, new NextAction("blackout kick", ACTION_HIGH + 6), nullptr)));

    // Purifying Brew removes the Stagger aura and all severity markers in the target core.
    triggers.push_back(new TriggerNode("heavy stagger", NextAction::array(0, new NextAction("purifying brew", ACTION_EMERGENCY), nullptr)));
    triggers.push_back(new TriggerNode("moderate stagger", NextAction::array(0, new NextAction("purifying brew", ACTION_HIGH + 7), nullptr)));

    triggers.push_back(new TriggerNode("keg smash", NextAction::array(0, new NextAction("keg smash", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("medium health", NextAction::array(0, new NextAction("guard", ACTION_HIGH + 3), nullptr)));
    triggers.push_back(new TriggerNode("low health", NextAction::array(0, new NextAction("elusive brew", ACTION_HIGH + 5), new NextAction("guard", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("critical health", NextAction::array(0, new NextAction("fortifying brew", ACTION_INTERRUPT), new NextAction("guard", ACTION_INTERRUPT - 1), nullptr)));
    triggers.push_back(new TriggerNode("lose aggro", NextAction::array(0, new NextAction("provoke", ACTION_INTERRUPT + 1), nullptr)));
    triggers.push_back(new TriggerNode("medium aoe", NextAction::array(0, new NextAction("breath of fire", ACTION_HIGH + 2), new NextAction("keg smash", ACTION_HIGH + 1), nullptr)));
}
