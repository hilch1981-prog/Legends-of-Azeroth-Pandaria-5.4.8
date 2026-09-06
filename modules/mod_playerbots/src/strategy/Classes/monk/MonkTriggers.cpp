#include "MonkTriggers.h"

#include "Player.h"
#include "SpellAuras.h"

namespace
{
constexpr uint32 SPELL_MONK_INTERNAL_MEDICINE = 115451;
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
