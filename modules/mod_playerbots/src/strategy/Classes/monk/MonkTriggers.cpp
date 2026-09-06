#include "MonkTriggers.h"

#include "Player.h"
#include "SpellAuras.h"

namespace
{
constexpr uint32 SPELL_MONK_INTERNAL_MEDICINE = 115451;
constexpr uint32 SPELL_MONK_TIGER_POWER = 125359;
constexpr uint32 SPELL_MONK_COMBO_BREAKER_TIGER_PALM = 118864;
}

bool NoTigerPowerTrigger::IsActive()
{
    return !bot->HasAura(SPELL_MONK_TIGER_POWER);
}

bool ComboBreakerTigerPalmTrigger::IsActive()
{
    return bot->HasAura(SPELL_MONK_COMBO_BREAKER_TIGER_PALM);
}

bool TigereyeBrewReadyTrigger::IsActive()
{
    constexpr uint32 TIGEREYE_BREW_STACKS = 125195;
    Aura* stacks = bot->GetAura(TIGEREYE_BREW_STACKS);
    return stacks && stacks->GetStackAmount() >= 10;
}

bool MonkCureMagicTrigger::IsActive()
{
    return bot->HasAura(SPELL_MONK_INTERNAL_MEDICINE) && NeedCureTrigger::IsActive();
}

bool MonkCureMagicOnPartyTrigger::IsActive()
{
    return bot->HasAura(SPELL_MONK_INTERNAL_MEDICINE) && PartyMemberNeedCureTrigger::IsActive();
}
