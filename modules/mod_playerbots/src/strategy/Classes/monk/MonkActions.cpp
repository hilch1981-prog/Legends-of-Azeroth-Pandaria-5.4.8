#include "MonkActions.h"

#include "Playerbots.h"

namespace
{
constexpr uint32 SPELL_MONK_ELUSIVE_BREW_STACKS = 128939;
constexpr uint32 SPELL_MONK_MANA_TEA_STACKS = 115867;
}

bool CastElusiveBrewAction::isUseful()
{
    Aura* stacks = bot->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
    return stacks && stacks->GetStackAmount() >= 5 && CastBuffSpellAction::isUseful();
}

bool CastManaTeaAction::isUseful()
{
    Aura* stacks = bot->GetAura(SPELL_MONK_MANA_TEA_STACKS);
    return stacks && stacks->GetStackAmount() >= 2 && CastBuffSpellAction::isUseful();
}
