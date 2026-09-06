#ifndef _PLAYERBOT_MONKACTIONS_H
#define _PLAYERBOT_MONKACTIONS_H

#include "GenericSpellActions.h"
#include "SharedDefines.h"

class CastJabAction : public CastMeleeSpellAction
{
public:
    CastJabAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "jab") {}
};

class CastTigerPalmAction : public CastMeleeSpellAction
{
public:
    CastTigerPalmAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "tiger palm") {}
};

class CastBlackoutKickAction : public CastMeleeSpellAction
{
public:
    CastBlackoutKickAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "blackout kick") {}
};

class CastSpinningCraneKickAction : public CastMeleeSpellAction
{
public:
    CastSpinningCraneKickAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "spinning crane kick") {}
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastExpelHarmAction : public CastHealingSpellAction
{
public:
    CastExpelHarmAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "expel harm") {}
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
};

class CastDetoxAction : public CastCureSpellAction
{
public:
    CastDetoxAction(PlayerbotAI* botAI) : CastCureSpellAction(botAI, "detox") {}
};

class CastDetoxPoisonOnPartyAction : public CurePartyMemberAction
{
public:
    CastDetoxPoisonOnPartyAction(PlayerbotAI* botAI) : CurePartyMemberAction(botAI, "detox", DISPEL_POISON) {}
    std::string const getName() override { return "detox poison on party"; }
};

class CastDetoxDiseaseOnPartyAction : public CurePartyMemberAction
{
public:
    CastDetoxDiseaseOnPartyAction(PlayerbotAI* botAI) : CurePartyMemberAction(botAI, "detox", DISPEL_DISEASE) {}
    std::string const getName() override { return "detox disease on party"; }
};

class CastKegSmashAction : public CastMeleeSpellAction
{
public:
    CastKegSmashAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "keg smash") {}
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastGuardAction : public CastBuffSpellAction
{
public:
    CastGuardAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "guard") {}
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
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastSoothingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastSoothingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "soothing mist") {}
};

class CastRenewingMistOnPartyAction : public HealPartyMemberAction
{
public:
    CastRenewingMistOnPartyAction(PlayerbotAI* botAI) : HealPartyMemberAction(botAI, "renewing mist") {}
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
};

class CastRevivalAction : public CastHealingSpellAction
{
public:
    CastRevivalAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "revival") {}
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastUpliftAction : public CastHealingSpellAction
{
public:
    CastUpliftAction(PlayerbotAI* botAI) : CastHealingSpellAction(botAI, "uplift") {}
    bool isUseful() override;
    ActionThreatType getThreatType() override { return ActionThreatType::Aoe; }
};

class CastManaTeaAction : public CastBuffSpellAction
{
public:
    CastManaTeaAction(PlayerbotAI* botAI) : CastBuffSpellAction(botAI, "mana tea") {}
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
};

class CastFistsOfFuryAction : public CastMeleeSpellAction
{
public:
    CastFistsOfFuryAction(PlayerbotAI* botAI) : CastMeleeSpellAction(botAI, "fists of fury") {}
};

class CastTouchOfKarmaAction : public CastSpellAction
{
public:
    CastTouchOfKarmaAction(PlayerbotAI* botAI) : CastSpellAction(botAI, "touch of karma") {}
};

#endif
