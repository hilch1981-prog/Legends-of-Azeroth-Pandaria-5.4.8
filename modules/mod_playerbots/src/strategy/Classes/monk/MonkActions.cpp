#include "MonkActions.h"

#include "Playerbots.h"
#include "Spell.h"
#include "SpellAuras.h"

namespace
{
constexpr uint32 SPELL_MONK_SOOTHING_MIST = 115175;
constexpr uint32 SPELL_MONK_ELUSIVE_BREW_STACKS = 128939;
constexpr uint32 SPELL_MONK_MANA_TEA_STACKS = 115867;
constexpr uint32 SPELL_MONK_RENEWING_MIST_HOT = 119611;

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
    if (!spellInfo)
        return false;

    int32 powerEntryIndex = -1;
    Powers powerType = spellInfo->GetPowerType(bot, &powerEntryIndex);
    int32 powerCost = spellInfo->CalcPowerCost(bot, spellInfo->GetSchoolMask(), powerEntryIndex);
    if (powerCost > 0 && bot->GetPower(powerType) < powerCost)
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

bool CastElusiveBrewAction::isUseful()
{
    auto* stacks = bot->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
    return stacks && stacks->GetStackAmount() >= 5 && CastBuffSpellAction::isUseful();
}

bool CastProvokeAction::isUseful()
{
    Unit* target = GetTarget();
    return target && target->GetTarget() != bot->GetGUID() && CastSpellAction::isUseful();
}

Unit* CastEnvelopingMistOnPartyAction::GetTarget()
{
    if (Unit* channelTarget = GetSoothingMistTarget(bot))
        return channelTarget;

    return HealPartyMemberAction::GetTarget();
}

bool CastEnvelopingMistOnPartyAction::isPossible()
{
    if (!GetSoothingMistTarget(bot))
        return HealPartyMemberAction::isPossible();

    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
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
    if (!GetSoothingMistTarget(bot))
        return HealPartyMemberAction::isPossible();

    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
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

bool CastManaTeaAction::isUseful()
{
    auto* stacks = bot->GetAura(SPELL_MONK_MANA_TEA_STACKS);
    return stacks && stacks->GetStackAmount() >= 2 && CastBuffSpellAction::isUseful();
}
