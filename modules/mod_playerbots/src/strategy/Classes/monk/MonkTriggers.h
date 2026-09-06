#ifndef _PLAYERBOT_MONKTRIGGERS_H
#define _PLAYERBOT_MONKTRIGGERS_H

#include "CureTriggers.h"
#include "GenericTriggers.h"
#include "SharedDefines.h"

class NoStanceOfTheSturdyOxTrigger : public HasNoAuraTrigger
{
public:
    NoStanceOfTheSturdyOxTrigger(PlayerbotAI* botAI) : HasNoAuraTrigger(botAI, "stance of the sturdy ox") {}
};

class NoStanceOfTheWiseSerpentTrigger : public HasNoAuraTrigger
{
public:
    NoStanceOfTheWiseSerpentTrigger(PlayerbotAI* botAI) : HasNoAuraTrigger(botAI, "stance of the wise serpent") {}
};

class NoStanceOfTheFierceTigerTrigger : public HasNoAuraTrigger
{
public:
    NoStanceOfTheFierceTigerTrigger(PlayerbotAI* botAI) : HasNoAuraTrigger(botAI, "stance of the fierce tiger") {}
};

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

// Target-core spell_monk.cpp confirms 125195 as the accumulated Tigereye Brew
// stack aura and 116740 as the active cast that consumes up to 10 stacks.
// Use the exact stack aura ID here instead of HasAuraStackTrigger: the generic
// helper forces a duration check, while this stack aura's DBC duration semantics
// have not yet been verified in the build-18414 runtime.
class TigereyeBrewReadyTrigger : public Trigger
{
public:
    TigereyeBrewReadyTrigger(PlayerbotAI* botAI) : Trigger(botAI, "tigereye brew ready") {}
    bool IsActive() override;
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

class MonkCureMagicTrigger : public NeedCureTrigger
{
public:
    MonkCureMagicTrigger(PlayerbotAI* botAI) : NeedCureTrigger(botAI, "detox", DISPEL_MAGIC) {}
    std::string const getName() override { return "cure magic"; }
    bool IsActive() override;
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

class MonkCureMagicOnPartyTrigger : public PartyMemberNeedCureTrigger
{
public:
    MonkCureMagicOnPartyTrigger(PlayerbotAI* botAI) : PartyMemberNeedCureTrigger(botAI, "detox", DISPEL_MAGIC) {}
    std::string const getName() override { return "cure magic on party"; }
    bool IsActive() override;
};

#endif
