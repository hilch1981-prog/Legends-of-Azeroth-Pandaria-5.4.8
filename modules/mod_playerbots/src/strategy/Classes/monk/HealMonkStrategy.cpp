#include "HealMonkStrategy.h"

#include "Playerbots.h"

NextAction** HealMonkStrategy::getDefaultActions()
{
    // Keep the healer's baseline focused on healing first. Jab is the melee
    // fallback that can generate Chi and the target-core Muscle Memory proc;
    // Tiger Palm is consumed only from the explicit Muscle Memory trigger below
    // instead of spending healing Chi unconditionally on every default pass.
    return NextAction::array(0,
        new NextAction("soothing mist on party", ACTION_DEFAULT + 0.4f),
        new NextAction("renewing mist on party", ACTION_DEFAULT + 0.3f),
        new NextAction("jab", ACTION_DEFAULT + 0.2f),
        new NextAction("melee", ACTION_DEFAULT), nullptr);
}

void HealMonkStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMonkStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("no stance of the wise serpent", NextAction::array(0, new NextAction("stance of the wise serpent", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("cure magic", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure magic on party", NextAction::array(0, new NextAction("detox magic on party", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0, new NextAction("life cocoon on party", ACTION_EMERGENCY), new NextAction("surging mist on party", ACTION_INTERRUPT), nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0, new NextAction("enveloping mist on party", ACTION_HIGH + 5), new NextAction("surging mist on party", ACTION_HIGH + 4), nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0, new NextAction("renewing mist on party", ACTION_HIGH + 3), new NextAction("soothing mist on party", ACTION_HIGH + 2), nullptr)));
    triggers.push_back(new TriggerNode("medium group heal setting", NextAction::array(0, new NextAction("revival", ACTION_HIGH + 6), new NextAction("uplift", ACTION_HIGH + 5), nullptr)));
    triggers.push_back(new TriggerNode("low mana", NextAction::array(0, new NextAction("mana tea", ACTION_HIGH + 4), nullptr)));

    // Consume Muscle Memory only when healing actions are not more relevant.
    // This keeps the Jab -> proc -> Tiger Palm mana-return loop available to a
    // healthy-party Mistweaver without letting the melee spender preempt heals.
    triggers.push_back(new TriggerNode("muscle memory", NextAction::array(0, new NextAction("tiger palm", ACTION_DEFAULT + 0.25f), nullptr)));
}
