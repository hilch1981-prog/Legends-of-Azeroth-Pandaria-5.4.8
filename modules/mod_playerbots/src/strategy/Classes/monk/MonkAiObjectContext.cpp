#include "MonkAiObjectContext.h"

#include "DpsMonkStrategy.h"
#include "GenericMonkNonCombatStrategy.h"
#include "GenericMonkStrategy.h"
#include "HealMonkStrategy.h"
#include "MonkActions.h"
#include "MonkTriggers.h"
#include "NamedObjectContext.h"
#include "Playerbots.h"
#include "TankMonkStrategy.h"

class MonkStrategyFactoryInternal : public NamedObjectContext<Strategy>
{
public:
    MonkStrategyFactoryInternal()
    {
        creators["nc"] = &MonkStrategyFactoryInternal::nc;
        creators["aoe"] = &MonkStrategyFactoryInternal::aoe;
        creators["melee aoe"] = &MonkStrategyFactoryInternal::melee_aoe;
        creators["cure"] = &MonkStrategyFactoryInternal::cure;
    }

private:
    static Strategy* nc(PlayerbotAI* botAI) { return new GenericMonkNonCombatStrategy(botAI); }
    static Strategy* aoe(PlayerbotAI* botAI) { return new MonkAoeStrategy(botAI); }
    static Strategy* melee_aoe(PlayerbotAI* botAI) { return new MeleeAoeMonkStrategy(botAI); }
    static Strategy* cure(PlayerbotAI* botAI) { return new MonkCureStrategy(botAI); }
};

class MonkCombatStrategyFactoryInternal : public NamedObjectContext<Strategy>
{
public:
    MonkCombatStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
    {
        creators["tank"] = &MonkCombatStrategyFactoryInternal::tank;
        creators["heal"] = &MonkCombatStrategyFactoryInternal::heal;
        creators["melee"] = &MonkCombatStrategyFactoryInternal::melee;
        creators["dps"] = &MonkCombatStrategyFactoryInternal::melee;
    }

private:
    static Strategy* tank(PlayerbotAI* botAI) { return new TankMonkStrategy(botAI); }
    static Strategy* heal(PlayerbotAI* botAI) { return new HealMonkStrategy(botAI); }
    static Strategy* melee(PlayerbotAI* botAI) { return new DpsMonkStrategy(botAI); }
};

class MonkTriggerFactoryInternal : public NamedObjectContext<Trigger>
{
public:
    MonkTriggerFactoryInternal()
    {
        creators["no stance of the sturdy ox"] = &MonkTriggerFactoryInternal::no_sturdy_ox;
        creators["no stance of the wise serpent"] = &MonkTriggerFactoryInternal::no_wise_serpent;
        creators["no stance of the fierce tiger"] = &MonkTriggerFactoryInternal::no_fierce_tiger;
        creators["keg smash"] = &MonkTriggerFactoryInternal::keg_smash;
        creators["rising sun kick"] = &MonkTriggerFactoryInternal::rising_sun_kick;
        creators["fists of fury"] = &MonkTriggerFactoryInternal::fists_of_fury;
        creators["touch of death"] = &MonkTriggerFactoryInternal::touch_of_death;
        creators["spear hand strike"] = &MonkTriggerFactoryInternal::spear_hand_strike;
        creators["no shuffle"] = &MonkTriggerFactoryInternal::no_shuffle;
        creators["moderate stagger"] = &MonkTriggerFactoryInternal::moderate_stagger;
        creators["heavy stagger"] = &MonkTriggerFactoryInternal::heavy_stagger;
        creators["no tiger power"] = &MonkTriggerFactoryInternal::no_tiger_power;
        creators["combo breaker tiger palm"] = &MonkTriggerFactoryInternal::combo_breaker_tiger_palm;
        creators["combo breaker blackout kick"] = &MonkTriggerFactoryInternal::combo_breaker_blackout_kick;
        creators["tigereye brew ready"] = &MonkTriggerFactoryInternal::tigereye_brew_ready;
        creators["cure poison"] = &MonkTriggerFactoryInternal::cure_poison;
        creators["cure disease"] = &MonkTriggerFactoryInternal::cure_disease;
        creators["cure magic"] = &MonkTriggerFactoryInternal::cure_magic;
        creators["cure poison on party"] = &MonkTriggerFactoryInternal::cure_poison_on_party;
        creators["cure disease on party"] = &MonkTriggerFactoryInternal::cure_disease_on_party;
        creators["cure magic on party"] = &MonkTriggerFactoryInternal::cure_magic_on_party;
    }

private:
    static Trigger* no_sturdy_ox(PlayerbotAI* botAI) { return new NoStanceOfTheSturdyOxTrigger(botAI); }
    static Trigger* no_wise_serpent(PlayerbotAI* botAI) { return new NoStanceOfTheWiseSerpentTrigger(botAI); }
    static Trigger* no_fierce_tiger(PlayerbotAI* botAI) { return new NoStanceOfTheFierceTigerTrigger(botAI); }
    static Trigger* keg_smash(PlayerbotAI* botAI) { return new KegSmashTrigger(botAI); }
    static Trigger* rising_sun_kick(PlayerbotAI* botAI) { return new RisingSunKickTrigger(botAI); }
    static Trigger* fists_of_fury(PlayerbotAI* botAI) { return new FistsOfFuryTrigger(botAI); }
    static Trigger* touch_of_death(PlayerbotAI* botAI) { return new TouchOfDeathTrigger(botAI); }
    static Trigger* spear_hand_strike(PlayerbotAI* botAI) { return new SpearHandStrikeTrigger(botAI); }
    static Trigger* no_shuffle(PlayerbotAI* botAI) { return new NoShuffleTrigger(botAI); }
    static Trigger* moderate_stagger(PlayerbotAI* botAI) { return new ModerateStaggerTrigger(botAI); }
    static Trigger* heavy_stagger(PlayerbotAI* botAI) { return new HeavyStaggerTrigger(botAI); }
    static Trigger* no_tiger_power(PlayerbotAI* botAI) { return new NoTigerPowerTrigger(botAI); }
    static Trigger* combo_breaker_tiger_palm(PlayerbotAI* botAI) { return new ComboBreakerTigerPalmTrigger(botAI); }
    static Trigger* combo_breaker_blackout_kick(PlayerbotAI* botAI) { return new ComboBreakerBlackoutKickTrigger(botAI); }
    static Trigger* tigereye_brew_ready(PlayerbotAI* botAI) { return new TigereyeBrewReadyTrigger(botAI); }
    static Trigger* cure_poison(PlayerbotAI* botAI) { return new MonkCurePoisonTrigger(botAI); }
    static Trigger* cure_disease(PlayerbotAI* botAI) { return new MonkCureDiseaseTrigger(botAI); }
    static Trigger* cure_magic(PlayerbotAI* botAI) { return new MonkCureMagicTrigger(botAI); }
    static Trigger* cure_poison_on_party(PlayerbotAI* botAI) { return new MonkCurePoisonOnPartyTrigger(botAI); }
    static Trigger* cure_disease_on_party(PlayerbotAI* botAI) { return new MonkCureDiseaseOnPartyTrigger(botAI); }
    static Trigger* cure_magic_on_party(PlayerbotAI* botAI) { return new MonkCureMagicOnPartyTrigger(botAI); }
};

class MonkAiObjectContextInternal : public NamedObjectContext<Action>
{
public:
    MonkAiObjectContextInternal()
    {
        creators["stance of the sturdy ox"] = &MonkAiObjectContextInternal::sturdy_ox;
        creators["stance of the wise serpent"] = &MonkAiObjectContextInternal::wise_serpent;
        creators["stance of the fierce tiger"] = &MonkAiObjectContextInternal::fierce_tiger;
        creators["jab"] = &MonkAiObjectContextInternal::jab;
        creators["tiger palm"] = &MonkAiObjectContextInternal::tiger_palm;
        creators["blackout kick"] = &MonkAiObjectContextInternal::blackout_kick;
        creators["spinning crane kick"] = &MonkAiObjectContextInternal::spinning_crane_kick;
        creators["expel harm"] = &MonkAiObjectContextInternal::expel_harm;
        creators["fortifying brew"] = &MonkAiObjectContextInternal::fortifying_brew;
        creators["spear hand strike"] = &MonkAiObjectContextInternal::spear_hand_strike;
        creators["touch of death"] = &MonkAiObjectContextInternal::touch_of_death;
        creators["detox"] = &MonkAiObjectContextInternal::detox;
        creators["detox poison on party"] = &MonkAiObjectContextInternal::detox_poison_on_party;
        creators["detox disease on party"] = &MonkAiObjectContextInternal::detox_disease_on_party;
        creators["detox magic on party"] = &MonkAiObjectContextInternal::detox_magic_on_party;
        creators["keg smash"] = &MonkAiObjectContextInternal::keg_smash;
        creators["guard"] = &MonkAiObjectContextInternal::guard;
        creators["elusive brew"] = &MonkAiObjectContextInternal::elusive_brew;
        creators["purifying brew"] = &MonkAiObjectContextInternal::purifying_brew;
        creators["provoke"] = &MonkAiObjectContextInternal::provoke;
        creators["breath of fire"] = &MonkAiObjectContextInternal::breath_of_fire;
        creators["soothing mist on party"] = &MonkAiObjectContextInternal::soothing_mist_on_party;
        creators["renewing mist on party"] = &MonkAiObjectContextInternal::renewing_mist_on_party;
        creators["enveloping mist on party"] = &MonkAiObjectContextInternal::enveloping_mist_on_party;
        creators["surging mist on party"] = &MonkAiObjectContextInternal::surging_mist_on_party;
        creators["life cocoon on party"] = &MonkAiObjectContextInternal::life_cocoon_on_party;
        creators["revival"] = &MonkAiObjectContextInternal::revival;
        creators["uplift"] = &MonkAiObjectContextInternal::uplift;
        creators["mana tea"] = &MonkAiObjectContextInternal::mana_tea;
        creators["tigereye brew"] = &MonkAiObjectContextInternal::tigereye_brew;
        creators["rising sun kick"] = &MonkAiObjectContextInternal::rising_sun_kick;
        creators["fists of fury"] = &MonkAiObjectContextInternal::fists_of_fury;
        creators["touch of karma"] = &MonkAiObjectContextInternal::touch_of_karma;
    }

private:
    static Action* sturdy_ox(PlayerbotAI* botAI) { return new CastStanceOfTheSturdyOxAction(botAI); }
    static Action* wise_serpent(PlayerbotAI* botAI) { return new CastStanceOfTheWiseSerpentAction(botAI); }
    static Action* fierce_tiger(PlayerbotAI* botAI) { return new CastStanceOfTheFierceTigerAction(botAI); }
    static Action* jab(PlayerbotAI* botAI) { return new CastJabAction(botAI); }
    static Action* tiger_palm(PlayerbotAI* botAI) { return new CastTigerPalmAction(botAI); }
    static Action* blackout_kick(PlayerbotAI* botAI) { return new CastBlackoutKickAction(botAI); }
    static Action* spinning_crane_kick(PlayerbotAI* botAI) { return new CastSpinningCraneKickAction(botAI); }
    static Action* expel_harm(PlayerbotAI* botAI) { return new CastExpelHarmAction(botAI); }
    static Action* fortifying_brew(PlayerbotAI* botAI) { return new CastFortifyingBrewAction(botAI); }
    static Action* spear_hand_strike(PlayerbotAI* botAI) { return new CastSpearHandStrikeAction(botAI); }
    static Action* touch_of_death(PlayerbotAI* botAI) { return new CastTouchOfDeathAction(botAI); }
    static Action* detox(PlayerbotAI* botAI) { return new CastDetoxAction(botAI); }
    static Action* detox_poison_on_party(PlayerbotAI* botAI) { return new CastDetoxPoisonOnPartyAction(botAI); }
    static Action* detox_disease_on_party(PlayerbotAI* botAI) { return new CastDetoxDiseaseOnPartyAction(botAI); }
    static Action* detox_magic_on_party(PlayerbotAI* botAI) { return new CastDetoxMagicOnPartyAction(botAI); }
    static Action* keg_smash(PlayerbotAI* botAI) { return new CastKegSmashAction(botAI); }
    static Action* guard(PlayerbotAI* botAI) { return new CastGuardAction(botAI); }
    static Action* elusive_brew(PlayerbotAI* botAI) { return new CastElusiveBrewAction(botAI); }
    static Action* purifying_brew(PlayerbotAI* botAI) { return new CastPurifyingBrewAction(botAI); }
    static Action* provoke(PlayerbotAI* botAI) { return new CastProvokeAction(botAI); }
    static Action* breath_of_fire(PlayerbotAI* botAI) { return new CastBreathOfFireAction(botAI); }
    static Action* soothing_mist_on_party(PlayerbotAI* botAI) { return new CastSoothingMistOnPartyAction(botAI); }
    static Action* renewing_mist_on_party(PlayerbotAI* botAI) { return new CastRenewingMistOnPartyAction(botAI); }
    static Action* enveloping_mist_on_party(PlayerbotAI* botAI) { return new CastEnvelopingMistOnPartyAction(botAI); }
    static Action* surging_mist_on_party(PlayerbotAI* botAI) { return new CastSurgingMistOnPartyAction(botAI); }
    static Action* life_cocoon_on_party(PlayerbotAI* botAI) { return new CastLifeCocoonOnPartyAction(botAI); }
    static Action* revival(PlayerbotAI* botAI) { return new CastRevivalAction(botAI); }
    static Action* uplift(PlayerbotAI* botAI) { return new CastUpliftAction(botAI); }
    static Action* mana_tea(PlayerbotAI* botAI) { return new CastManaTeaAction(botAI); }
    static Action* tigereye_brew(PlayerbotAI* botAI) { return new CastTigereyeBrewAction(botAI); }
    static Action* rising_sun_kick(PlayerbotAI* botAI) { return new CastRisingSunKickAction(botAI); }
    static Action* fists_of_fury(PlayerbotAI* botAI) { return new CastFistsOfFuryAction(botAI); }
    static Action* touch_of_karma(PlayerbotAI* botAI) { return new CastTouchOfKarmaAction(botAI); }
};

MonkAiObjectContext::MonkAiObjectContext(PlayerbotAI* botAI) : AiObjectContext(botAI)
{
    strategyContexts.Add(new MonkStrategyFactoryInternal());
    strategyContexts.Add(new MonkCombatStrategyFactoryInternal());
    actionContexts.Add(new MonkAiObjectContextInternal());
    triggerContexts.Add(new MonkTriggerFactoryInternal());
}
