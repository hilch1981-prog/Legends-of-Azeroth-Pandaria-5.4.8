#ifndef _PLAYERBOT_MONKACTIONS_H
#define _PLAYERBOT_MONKACTIONS_H

#include "GenericSpellActions.h"
#include "SharedDefines.h"

class CastStanceOfTheSturdyOxAction : public CastBuffSpellAction
{
public:
    CastStanceOfTheSturdyOxAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "stance of the sturdy ox") {}
};

class CastStanceOfTheWiseSerpentAction : public CastBuffSpellAction
{
public:
    CastStanceOfTheWiseSerpentAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "stance of the wise serpent") {}
};

class CastStanceOfTheFierceTigerAction : public CastBuffSpellAction
{
public:
    CastStanceOfTheFierceTigerAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "stance of the fierce tiger") {}
};

class CastJabAction : public CastMeleeSpellAction
{
public:
    CastJabAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "jab") {}
    bool isPossible() override;
};

class CastTigerPalmAction : public CastMeleeSpellAction
{
public:
    CastTigerPalmAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "tiger palm") {}
    bool isPossible() override;
};

class CastBlackoutKickAction : public CastMeleeSpellAction
{
public:
    CastBlackoutKickAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "blackout kick") {}
    bool isPossible() override;
};

class CastSpinningCraneKickAction : public CastMeleeSpellAction
{
public:
    CastSpinningCraneKickAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "spinning crane kick") {}
    bool isPossible() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastExpelHarmAction : public CastHealingSpellAction
{
public:
    CastExpelHarmAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "expel harm") {}
    bool isPossible() override;
};

class CastFortifyingBrewAction : public CastBuffSpellAction
{
public:
    CastFortifyingBrewAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "fortifying brew") {}
};

class CastSpearHandStrikeAction : public CastMeleeSpellAction
{
public:
    CastSpearHandStrikeAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "spear hand strike") {}
};

class CastTouchOfDeathAction : public CastMeleeSpellAction
{
public:
    CastTouchOfDeathAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "touch of death") {}
    bool isPossible() override;
};

class CastDetoxAction : public CastCureSpellAction
{
public:
    CastDetoxAction(PlayerbotAI* botAI) : CastCureSpellAction(botAI, "detox") {}
    bool isPossible() override;
};

class CastDetoxPoisonOnPartyAction : public CurePartyMemberAction
{
public:
    CastDetoxPoisonOnPartyAction(PlayerbotAI* botAI) : CurePartyMemberAction(botAI, "detox", DISPEL_POISON) {}
    std::string const getName() override { return "detox poison on party"; }
    bool isPossible() override;
};

class CastDetoxDiseaseOnPartyAction : public CurePartyMemberAction
{
public:
    CastDetoxDiseaseOnPartyAction(PlayerbotAI* botAI) : CurePartyMemberAction(botAI, "detox", DISPEL_DISEASE) {}
    std::string const getName() override { return "detox disease on party"; }
    bool isPossible() override;
};

class CastDetoxMagicOnPartyAction : public CurePartyMemberAction
{
public:
    CastDetoxMagicOnPartyAction(PlayerbotAI* botAI) : CurePartyMemberAction(botAI, "detox", DISPEL_MAGIC) {}
    std::string const getName() override { return "detox magic on party"; }
    bool isPossible() override;
};

class CastKegSmashAction : public CastMeleeSpellAction
{
public:
    CastKegSmashAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "keg smash") {}
    bool isPossible() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastGuardAction : public CastBuffSpellAction
{
public:
    CastGuardAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "guard") {}
    bool Execute(Event event) override;
    bool isPossible() override;
    bool isUseful() override;
};

class CastElusiveBrewAction : public CastBuffSpellAction
{
public:
    CastElusiveBrewAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "elusive brew") {}
    bool isUseful() override;
};

class CastPurifyingBrewAction : public CastSpellAction
{
public:
    CastPurifyingBrewAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "purifying brew") {}
    std::string const GetTargetName() override { return "self target"; }
    bool isPossible() override;
};

class CastProvokeAction : public CastSpellAction
{
public:
    CastProvokeAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "provoke") {}
    std::string const GetTargetName() override { return "tank target"; }
    bool isUseful() override;
};

class CastBreathOfFireAction : public CastMeleeSpellAction
{
public:
    CastBreathOfFireAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "breath of fire") {}
    bool isPossible() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastSoothingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastSoothingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "soothing mist") {}
    bool isPossible() override;
};

class CastRenewingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastRenewingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "renewing mist") {}
    Unit* GetTarget() override;
    bool isPossible() override;
    bool isUseful() override;
};

class CastEnvelopingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastEnvelopingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "enveloping mist") {}
    Unit* GetTarget() override;
    bool isPossible() override;
    bool isUseful() override;
};

class CastSurgingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastSurgingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "surging mist") {}
    Unit* GetTarget() override;
    bool isPossible() override;
    bool isUseful() override;
};

class CastLifeCocoonOnPartyAction : public CastSpellAction
{
public:
    CastLifeCocoonOnPartyAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "life cocoon") {}
    std::string const GetTargetName() override { return "party member to heal"; }
    std::string const getName() override { return "life cocoon on party"; }
    bool isPossible() override;
};

class CastRevivalAction : public CastHealingSpellAction
{
public:
    CastRevivalAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "revival") {}
    bool isPossible() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastUpliftAction : public CastHealingSpellAction
{
public:
    CastUpliftAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "uplift") {}
    bool isPossible() override;
    bool isUseful() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastManaTeaAction : public CastBuffSpellAction
{
public:
    CastManaTeaAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "mana tea") {}
    bool Execute(Event event) override;
    bool isPossible() override;
    bool isUseful() override;
};

class CastTigereyeBrewAction : public CastBuffSpellAction
{
public:
    CastTigereyeBrewAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "tigereye brew") {}
};

class CastRisingSunKickAction : public CastMeleeSpellAction
{
public:
    CastRisingSunKickAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "rising sun kick") {}
    bool isPossible() override;
};

class CastFistsOfFuryAction : public CastMeleeSpellAction
{
public:
    CastFistsOfFuryAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "fists of fury") {}
    bool isPossible() override;
};

class CastTouchOfKarmaAction : public CastSpellAction
{
public:
    CastTouchOfKarmaAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "touch of karma") {}
};

#endif
