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

// Factory key is "no shuffle", but HasNoAuraTrigger must keep its internal name as
// "shuffle" because its IsActive() uses getName() as the aura lookup key.
class NoShuffleTrigger : public HasNoAuraTrigger
{
public:
    NoShuffleTrigger(PlayerbotAI* botAI) : HasNoAuraTrigger(botAI, "shuffle") {}
};

class ModerateStaggerTrigger : public HasAuraTrigger
{
public:
    ModerateStaggerTrigger(PlayerbotAI* botAI) : HasAuraTrigger(botAI, "moderate stagger") {}
};

class HeavyStaggerTrigger : public HasAuraTrigger
{
public:
    HeavyStaggerTrigger(PlayerbotAI* botAI) : HasAuraTrigger(botAI, "heavy stagger") {}
};

// The target core consumes up to 10 Tigereye Brew stacks when casting the active
// buff. HasAuraStackTrigger keeps the internal aura lookup key as "tigereye brew"
// while the factory exposes the semantic trigger name "tigereye brew ready".
class TigereyeBrewReadyTrigger : public HasAuraStackTrigger
{
public:
    TigereyeBrewReadyTrigger(PlayerbotAI* botAI) : HasAuraStackTrigger(botAI, "tigereye brew", 10) {}
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
