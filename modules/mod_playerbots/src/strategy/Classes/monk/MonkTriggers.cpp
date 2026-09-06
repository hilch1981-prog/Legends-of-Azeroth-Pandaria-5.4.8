#include "MonkTriggers.h"

#include "Player.h"
#include "SpellAuras.h"

bool TigereyeBrewReadyTrigger::IsActive()
{
    constexpr uint32 TIGEREYE_BREW_STACKS = 125195;
    Aura* stacks = bot->GetAura(TIGEREYE_BREW_STACKS);
    return stacks && stacks->GetStackAmount() >= 10;
}
