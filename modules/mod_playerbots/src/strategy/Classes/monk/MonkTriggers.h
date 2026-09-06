#ifndef _PLAYERBOT_MONKTRIGGERS_H
#define _PLAYERBOT_MONKTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"
#include "SharedDefines.h"

class KegSmashTrigger : public SpellCanBeCastTrigger
{
public:
    KegSmashTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "keg smash") {}
};

class RisingSunKickTrigger : public SpellCanBeCastTrigger
{
public:
    RisingSunKickTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "rising sun kick") {}
};

class FistsOfFuryTrigger : public SpellCanBeCastTrigger
{
public:
    FistsOfFuryTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "fists of fury") {}
};

class TouchOfDeathTrigger : public SpellCanBeCastTrigger
{
public:
    TouchOfDeathTrigger(PlayerbotAI* botAI) : SpellCanBeCastTrigger(botAI, "touch of death") {}
};

class SpearHandStrikeTrigger : public InterruptSpellTrigger
{
public:
    SpearHandStrikeTrigger(PlayerbotAI* botAI) : InterruptSpellTrigger(botAI, "spear hand strike") {}
};

class MonkCurePoisonTrigger : public NeedCureTrigger
{
public:
    MonkCurePoisonTrigger(PlayerbotAI* botAI) : NeedCureTrigger(botAI, "detox", DISPEL_POISON) {}
    std::string const getName() override { return "cure poison"; }
};

class MonkCureDiseaseTrigger : public NeedCureTrigger
{
public:
    MonkCureDiseaseTrigger(PlayerbotAI* botAI) : NeedCureTrigger(botAI, "detox", DISPEL_DISEASE) {}
    std::string const getName() override { return "cure disease"; }
};

class MonkCurePoisonOnPartyTrigger : public PartyMemberNeedCureTrigger
{
public:
    MonkCurePoisonOnPartyTrigger(PlayerbotAI* botAI) : PartyMemberNeedCureTrigger(botAI, "detox", DISPEL_POISON) {}
    std::string const getName() override { return "cure poison on party"; }
};

class MonkCureDiseaseOnPartyTrigger : public PartyMemberNeedCureTrigger
{
public:
    MonkCureDiseaseOnPartyTrigger(PlayerbotAI* botAI) : PartyMemberNeedCureTrigger(botAI, "detox", DISPEL_DISEASE) {}
    std::string const getName() override { return "cure disease on party"; }
};

#endif
