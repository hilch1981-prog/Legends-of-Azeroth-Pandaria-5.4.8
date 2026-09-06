#include "GenericMonkNonCombatStrategy.h"

#include "Playerbots.h"

void GenericMonkNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);
    triggers.push_back(new TriggerNode("cure poison", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure poison on party", NextAction::array(0, new NextAction("detox poison on party", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure disease on party", NextAction::array(0, new NextAction("detox disease on party", ACTION_DISPEL), nullptr)));

    // The non-combat engine always loads the class-specific "nc" strategy. Keep
    // Mistweaver-only healing and magic dispel behavior behind a specialization
    // gate so Brewmaster/Windwalker retain only the generic Monk utility above.
    if (bot->GetSpecialization() != Specializations::SPEC_MONK_MISTWEAVER)
        return;

    triggers.push_back(new TriggerNode("no stance of the wise serpent", NextAction::array(0, new NextAction("stance of the wise serpent", ACTION_HIGH + 9), nullptr)));
    triggers.push_back(new TriggerNode("cure magic", NextAction::array(0, new NextAction("detox", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("cure magic on party", NextAction::array(0, new NextAction("detox magic on party", ACTION_DISPEL), nullptr)));
    triggers.push_back(new TriggerNode("party member to heal out of spell range", NextAction::array(0, new NextAction("reach party member to heal", ACTION_CRITICAL_HEAL + 10), nullptr)));
    triggers.push_back(new TriggerNode("party member critical health", NextAction::array(0,
        new NextAction("surging mist on party", ACTION_CRITICAL_HEAL + 5),
        new NextAction("soothing mist on party", ACTION_CRITICAL_HEAL + 4), nullptr)));
    triggers.push_back(new TriggerNode("party member low health", NextAction::array(0,
        new NextAction("enveloping mist on party", ACTION_MEDIUM_HEAL + 5),
        new NextAction("surging mist on party", ACTION_MEDIUM_HEAL + 4),
        new NextAction("soothing mist on party", ACTION_MEDIUM_HEAL + 3), nullptr)));
    triggers.push_back(new TriggerNode("party member medium health", NextAction::array(0,
        new NextAction("renewing mist on party", ACTION_LIGHT_HEAL + 3),
        new NextAction("soothing mist on party", ACTION_LIGHT_HEAL + 2), nullptr)));
}
