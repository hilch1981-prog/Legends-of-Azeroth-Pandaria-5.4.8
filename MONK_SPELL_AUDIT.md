# Monk PlayerBot Spell Audit — MoP 5.4.8

This file is a development-time verification ledger for the Monk PlayerBot contribution. It records only spell data confirmed from the target Legends-of-Azeroth Pandaria 5.4.8 source or later runtime/build evidence. Do not replace an unresolved entry with a guessed ID from another expansion.

Primary target-source reference: `src/server/scripts/Spells/spell_monk.cpp`.

## Confirmed target-core spell IDs

The following IDs are explicitly present in the target core's `MonkSpells` enum and are relevant to the initial PlayerBot strategy work.

| PlayerBot spell/behavior | Target-core ID | Target-core symbol / note | Status |
| --- | ---: | --- | --- |
| Blackout Kick | 100784 | `SPELL_MONK_BLACKOUT_KICK` | confirmed active spell ID |
| Fortifying Brew | 120954 | `SPELL_MONK_FORTIFYING_BREW` | confirmed active spell ID |
| Provoke | 118635 | `SPELL_MONK_PROVOKE` | confirmed active spell ID; bot target-selection still needs runtime validation |
| Soothing Mist | 115175 | `SPELL_MONK_SOOTHING_MIST` | confirmed active/channel spell ID |
| Elusive Brew | 115308 | `SPELL_MONK_ELUSIVE_BREW` | confirmed active spell ID |
| Spinning Crane Kick | 101546 | `SPELL_MONK_SPINNING_CRANE_KICK` | confirmed active spell ID |
| Renewing Mist | 115151 | `SPELL_MONK_RENEWING_MIST` | confirmed active spell ID |
| Mana Tea regeneration | 115294 | `SPELL_MONK_MANA_TEA_REGEN` | confirmed target-core Mana Tea regeneration spell; PlayerBot cast-name resolution still needs runtime validation |
| Uplift | 116670 | `SPELL_MONK_UPLIFT` | confirmed active spell ID |
| Touch of Death | 115080 | `SPELL_MONK_TOUCH_OF_DEATH` | confirmed active spell ID |

## Confirmed related/internal IDs

These IDs are explicitly present in the same target-core Monk spell script, but are secondary auras/effects/helpers rather than sufficient proof of the PlayerBot's active cast ID by themselves.

| Behavior | Target-core ID | Target-core symbol / meaning |
| --- | ---: | --- |
| Blackout Kick DoT | 128531 | `SPELL_MONK_BLACKOUT_KICK_DOT` |
| Blackout Kick heal | 128591 | `SPELL_MONK_BLACKOUT_KICK_HEAL` |
| Shuffle | 115307 | `SPELL_MONK_SHUFFLE` |
| Breath of Fire DoT | 123725 | `SPELL_MONK_BREATH_OF_FIRE_DOT` |
| Elusive Brew stacks | 128939 | `SPELL_MONK_ELUSIVE_BREW_STACKS` |
| Keg Smash visual | 123662 | `SPELL_MONK_KEG_SMASH_VISUAL` |
| Keg Smash energize | 127796 | `SPELL_MONK_KEG_SMASH_ENERGIZE` |
| Weakened Blows | 115798 | `SPELL_MONK_WEAKENED_BLOWS` |
| Stagger | 124255 | `SPELL_MONK_STAGGER` |
| Light Stagger | 124275 | `SPELL_MONK_LIGHT_STAGGER` |
| Moderate Stagger | 124274 | `SPELL_MONK_MODERATE_STAGGER` |
| Heavy Stagger | 124273 | `SPELL_MONK_HEAVY_STAGGER` |
| Spear Hand Strike silence effect | 116709 | `SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE` |
| Surging Mist heal effect | 116995 | `SPELL_MONK_SURGING_MIST_HEAL` |
| Enveloping Mist heal effect | 132120 | `SPELL_MONK_ENVELOPING_MIST_HEAL` |
| Mana Tea stacks | 115867 | `SPELL_MONK_MANA_TEA_STACKS` |
| Mana Tea driver | 115869 | `SPELL_MONK_MANA_TEA_DRIVER` |
| Rising Sun Kick debuff | 130320 | `SPELL_MONK_RISING_SUN_KICK_DEBUFF` |
| Expel Harm damage | 115129 | `SPELL_MONK_EXPEL_HARM_DAMAGE` |
| Touch of Karma redirected damage | 124280 | `SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE` |
| Stance of the Wise Serpent | 115070 | `SPELL_MONK_STANCE_OF_THE_WISE_SERPENT` |

## Active cast IDs still requiring direct confirmation

The initial PlayerBot code resolves these by spell name, but the contribution gate requires the actual MoP 5.4.8 cast spell and semantics to be confirmed before the related strategy is declared complete:

- Jab
- Tiger Palm
- Keg Smash
- Guard
- Breath of Fire
- Expel Harm
- Spear Hand Strike
- Detox
- Enveloping Mist
- Surging Mist
- Life Cocoon
- Revival
- Rising Sun Kick
- Fists of Fury
- Touch of Karma

For each unresolved spell, confirm the active cast ID plus resource cost (Energy/Chi/Mana), stance/spec restriction, cooldown, target type/range, aura/debuff dependency, and any channel/cast-time behavior. Prefer the target core's spell scripts/DBC runtime over donor-project constants.

## PlayerBot semantics that need runtime/build evidence

- Brewmaster: `Provoke` target selection must actually select the unit on which the bot has lost aggro; a generic current-target cast is only a temporary baseline.
- Brewmaster: Keg Smash / Breath of Fire interaction, Shuffle/Blackout Kick maintenance, Guard timing, Elusive Brew stacks, and Stagger purification logic need explicit triggers rather than only health thresholds.
- Mistweaver: Soothing Mist is channeled; Surging/Enveloping interaction during the channel, Renewing Mist coverage, Uplift eligibility, Mana Tea stacks, and Jade Serpent stance must be modeled before the healer strategy is complete.
- Windwalker: Energy-to-Chi generation, Tiger Palm maintenance, Rising Sun Kick debuff/cooldown, Fists of Fury channel safety, Touch of Karma defensive targeting, and Tigereye Brew should be validated before the DPS strategy is complete.

## Validation rule

A row is considered verified only when one of these is available:

1. explicit target-core `spell_monk.cpp` / DBC-backed evidence for the active spell, or
2. a successful `PLAYERBOTS=1` build plus runtime spell lookup/cast evidence on the target 5.4.8 environment.

Do not infer a verified MoP 5.4.8 ID solely from a modern WoW database or another emulator branch.
