# Monk PlayerBot Spell Audit — MoP 5.4.8

This file is a development-time verification ledger for the Monk PlayerBot contribution. It records target-core evidence from `src/server/scripts/Spells/spell_monk.cpp`, repository-local PlayerBot behavior, and later build/runtime evidence. Do not promote an enum helper/aura ID to an active player cast ID merely because it has a Monk symbol, and do not substitute IDs from another emulator or expansion.

## Active casts confirmed directly in the target core

These IDs are backed by an explicit target-core spell-script comment/class or by the target-core Monk damage map where the player spell is named directly.

| PlayerBot spell/behavior | Active cast ID | Target-core evidence | Status |
| --- | ---: | --- | --- |
| Jab | 100780 | `spell_monk_specific_damage_base` map; also listed as Jab by Power Strikes handling | active cast confirmed |
| Tiger Palm | 100787 | `spell_monk_specific_damage_base` map | active cast confirmed |
| Blackout Kick | 100784 | `// Blackout Kick - 100784`; Brewmaster hit logic applies/extends Shuffle | active cast confirmed |
| Keg Smash | 121253 | `// 121253 - Keg Smash`; applies Weakened Blows/Dizzying Haze and energizes after cast | active cast confirmed |
| Breath of Fire | 115181 | `// Breath of Fire - 115181` | active cast confirmed |
| Expel Harm | 115072 | `// 115072 - Expel Harm` and damage map | active cast confirmed |
| Detox | 115450 | `// 115450 - Detox`; magic effect is prevented unless caster has Internal Medicine `115451` | active cast confirmed |
| Purifying Brew | 119582 | `// 119582 - Purifying Brew`; removes Stagger/Light/Moderate/Heavy Stagger | active cast confirmed |
| Elusive Brew | 115308 | `// 115308 - Elusive Brew`; duration consumes Elusive Brew stack aura | active cast confirmed |
| Spinning Crane Kick | 101546 | `// 101546 - Spinning Crane Kick` aura implementation | active cast confirmed |
| Rising Sun Kick | 107428 | `// 107428 - Rising Sun Kick`; applies debuff 130320 | active cast confirmed |
| Fists of Fury | 113656 | `// 113656 - Fists of Fury` and damage map | active cast confirmed |
| Spear Hand Strike | 116705 | `// Spear Hand Strike - 116705`; target core applies a 15-second cooldown | active cast confirmed |
| Touch of Karma | 122470 | `// 122470 - Touch of Karma` | active cast confirmed |
| Provoke | 115546 | target-core Provoke cast script triggers the Provoke effect helper | active cast confirmed; static `tank target` selection implemented, runtime pending |
| Soothing Mist | 115175 | target-core Soothing Mist channel implementation | active cast confirmed |
| Renewing Mist | 115151 | `119611` aura explicitly calculates from player spell `115151` | player cast confirmed by target-core linkage |
| Surging Mist | 116694 | target-core script casts instantly onto the current Soothing Mist channel target when channeling | active cast confirmed |
| Enveloping Mist | 124682 | target-core script targets the current Soothing Mist channel target when channeling | active cast confirmed |
| Uplift | 116670 | target-core Uplift target filter operates on caster-owned Renewing Mist | active cast confirmed |
| Mana Tea | 115294 | `// 115294 - Mana Tea`; consumes Mana Tea stacks over channel ticks | active cast confirmed |
| Life Cocoon | 116849 | target-core Life Cocoon implementation | active cast confirmed |
| Revival | 115310 | target-core Revival raid-area healing logic | active cast confirmed |
| Tigereye Brew | 116740 | `// 116740 - Tigereye Brew`; consumes 10 Tigereye Brew stacks when present | active cast confirmed |
| Touch of Death | 115080 | target-core Touch of Death validity logic | active cast confirmed |

## Confirmed related/internal IDs

| Behavior | Target-core ID | Meaning |
| --- | ---: | --- |
| Provoke effect | 118635 | helper/effect cast by active Provoke; not the primary PlayerBot cast ID |
| Blackout Kick DoT | 128531 | secondary damage-over-time effect |
| Blackout Kick heal | 128591 | secondary heal effect |
| Shuffle | 115307 | Brewmaster defensive aura extended/applied by Blackout Kick |
| Breath of Fire DoT | 123725 | conditional secondary DoT |
| Elusive Brew stacks | 128939 | stack aura consumed by active Elusive Brew |
| Fortifying Brew aura/script | 120954 | target-core `AuraScript` ID; this is **not** sufficient evidence that `120954` is the learned player cast ID |
| Keg Smash visual | 123662 | secondary visual/effect |
| Keg Smash energize | 127796 | post-cast energize effect |
| Weakened Blows | 115798 | Keg Smash-applied debuff |
| Dizzying Haze | 116330 | Keg Smash-applied debuff; enables Breath of Fire DoT |
| Stagger | 124255 | underlying stagger periodic-damage aura |
| Light Stagger | 124275 | stagger severity marker |
| Moderate Stagger | 124274 | stagger severity marker |
| Heavy Stagger | 124273 | stagger severity marker |
| Spear Hand Strike silence | 116709 | secondary silence effect |
| Internal Medicine | 115451 | Mistweaver passive/aura checked by target-core Detox script before allowing its magic-dispel effect |
| Renewing Mist HoT | 119611 | caster-bound periodic heal used by jump logic and Uplift eligibility |
| Renewing Mist jump | 119607 | jump selector excludes units already carrying the same caster's `119611` |
| Uplift allowing cast | 123757 | caster-side helper maintained while bound Renewing Mist auras exist |
| Surging Mist heal | 116995 | heal effect triggered by active Surging Mist 116694 |
| Enveloping Mist heal | 132120 | heal effect triggered by active Enveloping Mist 124682 |
| Mana Tea stacks | 115867 | stack aura consumed by Mana Tea |
| Mana Tea driver | 115869 | hidden Chi-consumption driver |
| Tigereye Brew stacks | 125195 | stack aura generated from Chi consumption and consumed by active Tigereye Brew |
| Tigereye Brew +1 visual | 125196 | visual/effect emitted when a stack is generated |
| Tigereye Brew full-stack visual | 137591 | visual controller threshold |
| Combo Breaker: Tiger Palm | 118864 | target-core proc aura emitted by `spell_monk_combo_breaker` |
| Combo Breaker: Blackout Kick | 116768 | target-core proc aura emitted by `spell_monk_combo_breaker` |
| Rising Sun Kick debuff | 130320 | debuff applied after active RSK hit |
| Expel Harm damage | 115129 | damage effect generated from Expel Harm healing |
| Touch of Karma redirected damage | 124280 | redirected-damage helper |
| Stance of the Wise Serpent | 115070 | Mistweaver stance/aura |
| Glyph of Guard aura | 123401 | target-core glyph aura symbol used by Guard-related script logic |
| Guard player variants | 115295 / 123402 | target-core Monk ability variants; target-core script wiring and MoP-era data identify `115295` as normal Guard and `123402` as the spellbook override used by Glyph of Guard |
| Guard statue variants | 118604 / 136070 | Black Ox statue Guard effects, not PlayerBot player-cast choices |

## Target-core behavior confirmed for strategy design

### Brewmaster

- Blackout Kick `100784` applies or extends Shuffle `115307` for Brewmaster.
- Keg Smash `121253` applies Weakened Blows `115798` and Dizzying Haze `116330`, then energizes through `127796`.
- Breath of Fire `115181` gains its extra DoT only when Dizzying Haze is present.
- Stagger is represented by `124255` plus Light/Moderate/Heavy markers `124275/124274/124273`.
- Purifying Brew `119582` removes Stagger and all severity markers.
- Elusive Brew `115308` derives duration from accumulated stack aura `128939` and removes those stacks.
- Target-core `spell_monk_fortifying_brew` is an AuraScript registered on `120954`; that proves `120954` is an effect/aura in this core, not that it is the learned cast. PlayerBot deliberately continues to resolve `"fortifying brew"` from the live spellbook rather than hardcoding this aura ID. MoP-era data commonly identifies player cast `115203`; target DBC/runtime must settle that mapping before it is marked confirmed.
- Repository-local `ValueContext` provides `tank target`; current upstream `TankTargetValue` uses ThreatManager state to prioritize a tank target needing aggro. Monk Provoke targets that value rather than generic `current target`.
- Target-core `spell_monk_guard` is registered for player ability variants `115295` and `123402`, and `SPELL_MONK_GLYPH_OF_GUARD` is `123401`.
- Monk Guard resolves these two variants explicitly rather than depending on generic same-name ordering: if `HasActiveSpell(123402)` is true it uses the override; otherwise it falls back to active `115295`. The exact resolved ID is used for power preflight, `CanCastSpell`, and execution, and either aura ID suppresses redundant Guard use. Runtime still must prove how this fork exposes the glyph override in the live spellbook.

### Mistweaver

- Detox `115450` always supplies ordinary poison/disease dispel behavior, while the target-core script explicitly calls `PreventDefaultEffect` on its magic-dispel effect unless the caster has Internal Medicine `115451`.
- PlayerBot keeps poison/disease Detox in generic Monk cure/non-combat utility. Magic Detox triggers/actions require `115451` and are wired only for Mistweaver: in the combat `heal` strategy and in the specialization-gated Mistweaver section of the universal non-combat `nc` strategy. Brewmaster/Windwalker therefore do not request impossible magic dispels.
- `AiFactory::AddDefaultNonCombatStrategies` has no separate Monk branch, so all Monk specs receive the class `nc` strategy through the universal non-combat path. Mistweaver-only Wise Serpent stance maintenance, magic Detox, heal-target range recovery, and basic party healing are therefore explicitly gated by `SPEC_MONK_MISTWEAVER` inside `GenericMonkNonCombatStrategy`.
- Soothing Mist `115175` is a channel.
- Surging Mist `116694` and Enveloping Mist `124682` detect Soothing Mist, become directly castable, and redirect their effective heal to the current Soothing Mist channel target.
- Renewing Mist player spell `115151` produces caster-bound HoT `119611`. On apply/tick the target core maintains Uplift-allowing helper `123757` on the caster.
- Renewing Mist jump `119607` explicitly excludes units already carrying `119611` from the same caster, then prefers an injured eligible unit when one exists.
- Uplift `116670` builds its heal target list only from units carrying caster-owned Renewing Mist. Thunder Focus Tea controls whether the refresh effect is applied; the Uplift heal target set remains the caster's Renewing Mist targets.
- PlayerBot mirrors those semantics conservatively: Renewing Mist prefers a valid group player not already carrying this Monk's `119611`, and Uplift is considered useful only when at least two injured group players carry this Monk's `119611`.
- Repository `PartyMemberToHeal::Check` accepts same-map LOS targets within `< healDistance * 2`; the Monk Renewing Mist fallback uses the same range/LOS envelope.
- Mana Tea `115294` consumes `115867` stacks over periodic ticks; the Mana Tea driver generates stacks from Chi consumption while in Stance of the Wise Serpent `115070`.
- Revival `115310` is a raid-area heal and excludes minor guardians from its target list.
- Mistweaver combat and non-combat healing both use repository-native `party member to heal` plus `reach party member to heal` for out-of-range recovery.

### Windwalker

- Rising Sun Kick `107428` applies debuff `130320`.
- Fists of Fury `113656` is an aura/channel-style periodic damage ability in the target core; its periodic damage amount comes from the Monk-specific damage calculation.
- Repository `PlayerbotAI::UpdateAI` can resume engine evaluation after the initial channel preparation state, but routine self-interruption is narrower than that fact alone suggests: generic `PlayerbotAI::CanCastSpell` rejects ordinary spell casts while `CURRENT_CHANNELED_SPELL` exists, `ReachTargetAction::isUseful` rejects `reach melee` while channeling, and `set facing` changes facing without issuing movement. No Monk-specific blanket channel lock is justified statically. Runtime must still verify global safety/formation/avoid-AoE movement, where interrupting Fists may be intentional.
- Chi-consuming spells feed the Tigereye Brew driver and generate stack aura `125195`.
- Active Tigereye Brew `116740` removes 10 stacks and scales its buff from the stack aura.
- PlayerBot checks exact aura `125195` and requests Tigereye Brew at 10 stacks. It intentionally avoids the repository generic `HasAuraStackTrigger` because that helper also imposes duration semantics not yet validated for this aura in build 18414.
- Target core explicitly emits Combo Breaker proc auras `118864` (Tiger Palm) and `116768` (Blackout Kick). MoP-era spell data corroborates Tiger Power aura `125359`; target DBC/runtime verification of `125359` is still pending.
- Windwalker Tiger Palm is not an unconditional default filler. The PlayerBot requests it when Tiger Power `125359` is absent or when Combo Breaker: Tiger Palm `118864` is active. This prevents a one-Chi maintenance ability from repeatedly sitting ahead of Blackout Kick in the default queue and starving stronger Chi spenders. Brewmaster's separate Tiger Palm baseline is intentionally unchanged.
- Touch of Death is kept in the Windwalker strategy rather than the generic Monk strategy, preventing tank/healer baselines from spending the high-priority offensive resource action.

## PlayerBot resource-preflight evidence

Repository-local `PlayerbotAI::CanCastSpell(Unit*)` constructs its preflight `Spell` with `TRIGGERED_IGNORE_POWER_AND_REAGENT_COST`. That means a generic `SpellCanBeCastTrigger` / `CastSpellAction::isPossible()` result does not prove that the bot currently has enough Mana, Energy, or Chi. The real `PlayerbotAI::CastSpell` path later uses `TRIGGERED_NONE` and `Spell::CheckCast(false)`, so an underfunded high-priority action can otherwise be selected repeatedly and fail only at execution time.

The Monk implementation adds a class-local power preflight instead of altering global PlayerBot behavior. It resolves the bot's learned spell ID, then uses target-core `SpellInfo::GetPowerType` plus `SpellInfo::CalcPowerCost` and compares the result with the bot's current power. Target `SpellInfo.h` exposes the exact signatures used by the helper. No Energy/Chi/Mana cost is hardcoded. Positive-cost `POWER_HEALTH` is checked against current health, and unexpected special power values outside `MAX_POWERS` fail closed rather than indexing normal power storage.

This gate covers the main Monk resource-sensitive combat/heal actions, including Jab, Tiger Palm, Blackout Kick, Spinning Crane Kick, Expel Harm, Touch of Death, Detox and party Detox variants, Keg Smash, Guard, Purifying Brew, Breath of Fire, Soothing/Renewing/Surging/Enveloping Mist, Life Cocoon, Revival, Uplift, Rising Sun Kick, and Fists of Fury. Final cast legality still remains with the normal target-core `Spell::CheckCast` path.

## PlayerBot name-resolution evidence

`SpellIdValue` searches the bot's active learned, non-passive spell map for an exact requested spell name. For each same-name candidate it stores the ID in an ordered set and iterates the set in reverse. If a candidate has no numeric rank text, it assigns that ID to `castSpellId` and continues. Therefore, when multiple unranked same-name spells are simultaneously active, the loop can finish on the **numerically lowest** matching ID rather than a semantic spellbook override.

`PlayerbotAI::CanCastSpell(std::string, ...)` and `PlayerbotAI::CastSpell(std::string, ...)` both consume this `"spell id"` value, so the ambiguity affects both preflight and execution. The fix is intentionally Monk-local: Guard checks `HasActiveSpell(123402)` first and `HasActiveSpell(115295)` second, then uses the exact returned ID. Global `SpellIdValue` is not changed because its ordering behavior may be relied upon by unrelated legacy/ranked spell paths.

This does not yet claim the live glyph path is fully validated. If the core exposes only normal `115295` in `PlayerSpellMap` and performs the `123402` override elsewhere at cast time, the fallback preserves normal behavior. If it exposes `123402` as an active spellbook replacement, the Monk action selects it deterministically.

## Static factory/key audit

The Monk strategy, trigger, and action names referenced by `AiFactory` and all three specialization strategies were cross-checked against `MonkAiObjectContext`. No missing or misspelled Monk-local factory keys were found. Global names used by Monk strategies such as `tank assist`, `dps assist`, `reach party member to heal`, and standard health/group triggers are repository-wide contexts and intentionally are not re-registered in `MonkAiObjectContext`.

## Still requiring direct runtime/DBC validation

- Fortifying Brew — verify the learned player spell ID in build 18414. Target core directly scripts aura/effect `120954`; MoP-era data points to player cast `115203`, so `120954` must not be hardcoded as the cast without DBC/runtime proof.
- Guard — verify `HasActiveSpell(115295/123402)`, glyph/no-glyph casting, power cost, cooldown, and resulting aura.
- Tiger Power / Combo Breaker — verify live Tiger Power aura `125359`, Combo Breaker consumption, and Windwalker Tiger Palm cadence.
- Tigereye Brew — prove action-name lookup resolves learned active `116740` and live stack aura is `125195`.
- Provoke — verify actual multi-attacker threat selection and successful taunt.
- Detox magic — verify live magic-dispel selection and party targeting in both combat and Mistweaver non-combat recovery.
- Life Cocoon — verify emergency party target and range behavior.
- Soothing/Surging/Enveloping — verify compile and live channel behavior, including non-combat recovery.
- Renewing Mist/Uplift — verify target spread, Chi use, range, and cadence in a live group.
- Fists of Fury — verify movement/channel interruption and Energy/Chi rotation interaction before adding any channel lock.

## Source inconsistency noted

One Storm/Earth/Fire proc switch labels `100780` as `TigerPalm`, while the dedicated Monk damage map and Power Strikes handling identify `100780` as Jab and `100787` as Tiger Palm. The dedicated damage map is treated as stronger local evidence. Do not encode the conflicting comment as a spell-ID mapping.

## Validation rule

A spell mapping or behavioral assumption is marked verified only with one of these forms of evidence:

1. explicit target-core spell script/comment plus matching implementation use;
2. target-core DBC-backed learned spell/resource data; or
3. successful `PLAYERBOTS=1` build followed by target 5.4.8 runtime lookup/cast evidence.

External MoP-era databases/addons may corroborate or narrow an ambiguity, but they do not replace target-core/DBC/runtime evidence for a final `[x]` verification decision.
