#include "MonkActions.h"

#include "Playerbots.h"
#include "Spell.h"
#include "SpellAuras.h"

namespace
{
constexpr uint32 SPELL_MONK_SOOTHING_MIST = 115175;
constexpr uint32 SPELL_MONK_GUARD = 115295;
constexpr uint32 SPELL_MONK_GUARD_GLYPH_OVERRIDE = 123402;
constexpr uint32 SPELL_MONK_ELUSIVE_BREW_STACKS = 128939;
constexpr uint32 SPELL_MONK_MANA_TEA = 115294;
constexpr uint32 SPELL_MONK_MANA_TEA_STACKS = 115867;
constexpr uint32 SPELL_MONK_MANA_TEA_GLYPH_OVERRIDE = 123761;
constexpr uint32 SPELL_MONK_RENEWING_MIST_HOT = 119611;
constexpr uint32 SPELL_MONK_COMBO_BREAKER_TIGER_PALM = 118864;
constexpr uint32 SPELL_MONK_COMBO_BREAKER_BLACKOUT_KICK = 116768;

bool HasPowerForSpell(Player* bot, uint32 spellId)
{
    if (!spellId)
        return false;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return false;

    int32 powerEntryIndex = -1;
    Powers powerType = spellInfo->GetPowerType(bot, &powerEntryIndex);
    int32 powerCost = spellInfo->CalcPowerCost(bot, spellInfo->GetSchoolMask(), powerEntryIndex);
    if (powerCost <= 0)
        return true;

    if (powerType == POWER_HEALTH)
        return bot->GetHealth() > uint32(powerCost);

    if (static_cast<uint32>(powerType) >= MAX_POWERS)
        return false;

    return bot->GetPower(powerType) >= powerCost;
}

uint32 GetGuardSpellId(Player* bot)
{
    // MoP exposes 123402 as the Glyph of Guard spellbook override for normal Guard 115295.
    // Prefer the active override only when the target core actually exposes it in the bot's
    // spellbook; otherwise fall back to the normal learned Guard. This avoids the generic
    // name resolver's ambiguous ordering when both same-name, unranked spells are active.
    if (bot->HasActiveSpell(SPELL_MONK_GUARD_GLYPH_OVERRIDE))
        return SPELL_MONK_GUARD_GLYPH_OVERRIDE;

    if (bot->HasActiveSpell(SPELL_MONK_GUARD))
        return SPELL_MONK_GUARD;

    return 0;
}

bool HasGuardAura(Player* bot)
{
    return bot->HasAura(SPELL_MONK_GUARD) || bot->HasAura(SPELL_MONK_GUARD_GLYPH_OVERRIDE);
}

uint32 GetManaTeaSpellId(Player* bot)
{
    // The target core has both the normal 115294 Mana Tea channel and the
    // 123761 glyphed instant variant. Prefer an active spellbook override when
    // present instead of relying on ambiguous same-name spell resolution.
    if (bot->HasActiveSpell(SPELL_MONK_MANA_TEA_GLYPH_OVERRIDE))
        return SPELL_MONK_MANA_TEA_GLYPH_OVERRIDE;

    if (bot->HasActiveSpell(SPELL_MONK_MANA_TEA))
        return SPELL_MONK_MANA_TEA;

    return 0;
}

Unit* GetSoothingMistTarget(Player* bot)
{
    Spell* channel = bot->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
    if (!channel || !channel->GetSpellInfo() || channel->GetSpellInfo()->Id != SPELL_MONK_SOOTHING_MIST)
        return nullptr;

    return channel->GetExplTargetUnit();
}

bool CanCastMistHealWhileSoothing(PlayerbotAI* botAI, Player* bot, uint32 spellId, Unit* target)
{
    if (!spellId || !target || !target->IsAlive() || !target->IsInWorld() || target->GetMapId() != bot->GetMapId())
        return false;

    if (botAI->IsInVehicle() && !botAI->IsInVehicle(false, false, true))
        return false;

    if (bot->HasUnitState(UNIT_STATE_LOST_CONTROL) || !bot->HasSpell(spellId) || bot->HasSpellCooldown(spellId))
        return false;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo || !HasPowerForSpell(bot, spellId))
        return false;

    if (target->IsImmunedToSpell(spellInfo, spellInfo->GetAllEffectsMechanicMask()))
        return false;

    if (bot != target && bot->GetDistance2d(target) > sPlayerbotAIConfig->sightDistance)
        return false;

    // The target core marks Surging Mist and Enveloping Mist as directly castable when
    // Soothing Mist is the current channel. Generic PlayerbotAI::CanCastSpell rejects all
    // casts while channeling before those Monk spell scripts can apply that exception, so
    // defer the final range/script validation to PlayerbotAI::CastSpell/Spell::CheckCast.
    return true;
}
}

bool CastJabAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastTigerPalmAction::isPossible()
{
    // Combo Breaker makes Tiger Palm free. Do not let the class-local power preflight
    // suppress the proc at zero Chi; the normal cast path still performs all other legality checks.
    return (bot->HasAura(SPELL_MONK_COMBO_BREAKER_TIGER_PALM) || HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell))) &&
        CastMeleeSpellAction::isPossible();
}

bool CastBlackoutKickAction::isPossible()
{
    // Combo Breaker makes Blackout Kick free. As with Tiger Palm, bypass only the
    // conservative power preflight while the exact proc aura is active.
    return (bot->HasAura(SPELL_MONK_COMBO_BREAKER_BLACKOUT_KICK) || HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell))) &&
        CastMeleeSpellAction::isPossible();
}

bool CastSpinningCraneKickAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastExpelHarmAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastHealingSpellAction::isPossible();
}

bool CastTouchOfDeathAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastDetoxAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastCureSpellAction::isPossible();
}

bool CastDetoxPoisonOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CurePartyMemberAction::isPossible();
}

bool CastDetoxDiseaseOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CurePartyMemberAction::isPossible();
}

bool CastDetoxMagicOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CurePartyMemberAction::isPossible();
}

bool CastKegSmashAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastGuardAction::Execute([[maybe_unused]] Event event)
{
    uint32 spellId = GetGuardSpellId(bot);
    return spellId && botAI->CastSpell(spellId, bot);
}

bool CastGuardAction::isPossible()
{
    if (botAI->IsInVehicle() && !botAI->IsInVehicle(false, false, true))
        return false;

    uint32 spellId = GetGuardSpellId(bot);
    return HasPowerForSpell(bot, spellId) && botAI->CanCastSpell(spellId, bot);
}

bool CastGuardAction::isUseful()
{
    return GetGuardSpellId(bot) && !HasGuardAura(bot);
}

bool CastElusiveBrewAction::isUseful()
{
    auto* stacks = bot->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
    return stacks && stacks->GetStackAmount() >= 5 && CastBuffSpellAction::isUseful();
}

bool CastPurifyingBrewAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastSpellAction::isPossible();
}

bool CastProvokeAction::isUseful()
{
    Unit* target = GetTarget();
    return target && target->GetTarget() != bot->GetGUID() && CastSpellAction::isUseful();
}

bool CastBreathOfFireAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastSoothingMistOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && HealPartyMemberAction::isPossible();
}

Unit* CastRenewingMistOnPartyAction::GetTarget()
{
    Unit* priorityTarget = HealPartyMemberAction::GetTarget();
    if (priorityTarget && !priorityTarget->GetAura(SPELL_MONK_RENEWING_MIST_HOT, bot->GetGUID()))
        return priorityTarget;

    Player* bestTarget = nullptr;
    float bestHealth = 101.0f;
    for (ObjectGuid const guid : AI_VALUE(GuidVector, "group members"))
    {
        Player* member = ObjectAccessor::FindPlayer(guid);
        if (!member || !member->IsAlive() || !member->IsInWorld() || member->GetMapId() != bot->GetMapId())
            continue;

        if (member->GetAura(SPELL_MONK_RENEWING_MIST_HOT, bot->GetGUID()))
            continue;

        if (bot->GetDistance2d(member) >= sPlayerbotAIConfig->healDistance * 2 || !bot->IsWithinLOSInMap(member))
            continue;

        float health = member->GetHealthPct();
        if (health < bestHealth)
        {
            bestHealth = health;
            bestTarget = member;
        }
    }

    return bestTarget ? bestTarget : priorityTarget;
}

bool CastRenewingMistOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && HealPartyMemberAction::isPossible();
}

bool CastRenewingMistOnPartyAction::isUseful()
{
    Unit* target = GetTarget();
    return target && !target->GetAura(SPELL_MONK_RENEWING_MIST_HOT, bot->GetGUID()) && HealPartyMemberAction::isUseful();
}

Unit* CastEnvelopingMistOnPartyAction::GetTarget()
{
    if (Unit* channelTarget = GetSoothingMistTarget(bot))
        return channelTarget;

    return HealPartyMemberAction::GetTarget();
}

bool CastEnvelopingMistOnPartyAction::isPossible()
{
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    if (!GetSoothingMistTarget(bot))
        return HasPowerForSpell(bot, spellId) && HealPartyMemberAction::isPossible();

    return CanCastMistHealWhileSoothing(botAI, bot, spellId, GetTarget());
}

bool CastEnvelopingMistOnPartyAction::isUseful()
{
    if (Unit* channelTarget = GetSoothingMistTarget(bot))
    {
        Unit* priorityTarget = AI_VALUE(Unit*, "party member to heal");
        if (priorityTarget && priorityTarget != channelTarget)
            return false;
    }

    return HealPartyMemberAction::isUseful();
}

Unit* CastSurgingMistOnPartyAction::GetTarget()
{
    if (Unit* channelTarget = GetSoothingMistTarget(bot))
        return channelTarget;

    return HealPartyMemberAction::GetTarget();
}

bool CastSurgingMistOnPartyAction::isPossible()
{
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    if (!GetSoothingMistTarget(bot))
        return HasPowerForSpell(bot, spellId) && HealPartyMemberAction::isPossible();

    return CanCastMistHealWhileSoothing(botAI, bot, spellId, GetTarget());
}

bool CastSurgingMistOnPartyAction::isUseful()
{
    if (Unit* channelTarget = GetSoothingMistTarget(bot))
    {
        Unit* priorityTarget = AI_VALUE(Unit*, "party member to heal");
        if (priorityTarget && priorityTarget != channelTarget)
            return false;
    }

    return HealPartyMemberAction::isUseful();
}

bool CastLifeCocoonOnPartyAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastSpellAction::isPossible();
}

bool CastRevivalAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastHealingSpellAction::isPossible();
}

bool CastUpliftAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastHealingSpellAction::isPossible();
}

bool CastUpliftAction::isUseful()
{
    if (!CastSpellAction::isUseful())
        return false;

    uint32 injuredRenewingMistTargets = 0;
    for (ObjectGuid const guid : AI_VALUE(GuidVector, "group members"))
    {
        Player* member = ObjectAccessor::FindPlayer(guid);
        if (!member || !member->IsAlive() || !member->IsInWorld() || member->GetMapId() != bot->GetMapId())
            continue;

        if (member->GetHealthPct() >= sPlayerbotAIConfig->mediumHealth)
            continue;

        if (!member->GetAura(SPELL_MONK_RENEWING_MIST_HOT, bot->GetGUID()))
            continue;

        if (++injuredRenewingMistTargets >= 2)
            return true;
    }

    return false;
}

bool CastManaTeaAction::Execute([[maybe_unused]] Event event)
{
    uint32 spellId = GetManaTeaSpellId(bot);
    return spellId && botAI->CastSpell(spellId, bot);
}

bool CastManaTeaAction::isPossible()
{
    if (botAI->IsInVehicle() && !botAI->IsInVehicle(false, false, true))
        return false;

    uint32 spellId = GetManaTeaSpellId(bot);
    return spellId && botAI->CanCastSpell(spellId, bot);
}

bool CastManaTeaAction::isUseful()
{
    uint32 spellId = GetManaTeaSpellId(bot);
    Aura* stacks = bot->GetAura(SPELL_MONK_MANA_TEA_STACKS);
    if (!spellId || !stacks)
        return false;

    // Normal Mana Tea consumes one stack on channel application and then one per
    // periodic tick. The glyphed 123761 variant has a target-core CheckCast gate
    // requiring at least two stacks, so do not impose that two-stack minimum on
    // the normal 115294 channel.
    uint8 requiredStacks = spellId == SPELL_MONK_MANA_TEA_GLYPH_OVERRIDE ? 2 : 1;
    return stacks->GetStackAmount() >= requiredStacks;
}

bool CastRisingSunKickAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}

bool CastFistsOfFuryAction::isPossible()
{
    return HasPowerForSpell(bot, AI_VALUE2(uint32, "spell id", spell)) && CastMeleeSpellAction::isPossible();
}
