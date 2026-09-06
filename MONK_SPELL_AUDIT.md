# Monk PlayerBot Spell Audit — MoP 5.4.8

This file is a development-time verification ledger for the Monk PlayerBot contribution. It records target-core evidence from `src/server/scripts/Spells/spell_monk.cpp` and later build/runtime evidence. Do not promote an enum helper/aura ID to an active player cast ID merely because it has a Monk symbol, and do not substitute IDs from another emulator or expansion.

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
| Detox | 115450 | `// 115450 - Detox` | active cast confirmed |
| Purifying Brew | 119582 | `// 119582 - Purifying Brew`; removes Stagger/Light/Moderate/Heavy Stagger | active cast confirmed |
| Fortifying Brew | 120954 | `// 120954 - Fortifying Brew` | active cast/aura confirmed |
| Elusive Brew | 115308 | `// 115308 - Elusive Brew`; duration consumes Elusive Brew stack aura | active cast confirmed |
| Spinning Crane Kick | 101546 | `// 101546 - Spinning Crane Kick` aura implementation | active cast confirmed |
| Rising Sun Kick | 107428 | `// 107428 - Rising Sun Kick`; applies debuff 130320 | active cast confirmed |
| Fists of Fury | 113656 | `// 113656 - Fists of Fury` and damage map | active cast confirmed |
| Spear Hand Strike | 116705 | `// Spear Hand Strike - 116705`; target core applies a 15-second cooldown | active cast confirmed |
| Touch of Karma | 122470 | `// 122470 - Touch of Karma` | active cast confirmed |
| Provoke | 115546 | `// 115546 - Provoke`; cast script triggers the Provoke effect helper | active cast confirmed; PlayerBot aggro-target selection remains unverified |
| Soothing Mist | 115175 | `// 115175 - Soothing Mist`; target core treats it as a channel aura | active cast confirmed |
| Renewing Mist | 115151 | target-core `SPELL_MONK_RENEWING_MIST`; 119611 aura implementation explicitly calculates from 115151 | player cast confirmed by target-core linkage |
| Surging Mist | 116694 | `// 116694 - Surging Mist`; casts instantly onto the current Soothing Mist channel target when channeling | active cast confirmed |
| Enveloping Mist | 124682 | `// 124682 - Enveloping Mist`; targets the current Soothing Mist channel target when channeling | active cast confirmed |
| Uplift | 116670 | `// 116670 - Uplift` target-core handling | active cast confirmed |
| Mana Tea | 115294 | `// 115294 - Mana Tea`; consumes Mana Tea stacks over channel ticks | active cast confirmed |
| Life Cocoon | 116849 | `// 116849 - Life Cocoon` | active cast confirmed |
| Revival | 115310 | `// 115310 - Revival`; target selector filters minor guardians from the raid-area heal | active cast confirmed |
| Tigereye Brew | 116740 | `// 116740 - Tigereye Brew`; consumes up to 10 Tigereye Brew stack aura charges and scales the resulting buff | active cast confirmed |
| Touch of Death | 115080 | `// 115080 - Touch of Death` with target validity rules | active cast confirmed |

## Confirmed related/internal IDs

These IDs are real target-core Monk auras/effects/helpers, but they are not the active player cast ID for the corresponding button unless explicitly stated above.

| Behavior | Target-core ID | Meaning |
| --- | ---: | --- |
| Provoke effect | 118635 | helper/effect cast by active Provoke 115546; **not** the primary PlayerBot cast ID |
| Blackout Kick DoT | 128531 | secondary damage-over-time effect |
| Blackout Kick heal | 128591 | secondary heal effect |
| Shuffle | 115307 | Brewmaster defensive aura extended/applied by Blackout Kick |
| Breath of Fire DoT | 123725 | conditional secondary DoT |
| Elusive Brew stacks | 128939 | stack aura consumed by active Elusive Brew |
| Keg Smash visual | 123662 | secondary visual/effect |
| Keg Smash energize | 127796 | post-cast energize effect |
| Weakened Blows | 115798 | Keg Smash-applied debuff |
| Dizzying Haze | 116330 | Keg Smash-applied debuff; enables Breath of Fire DoT in target code |
| Stagger | 124255 | underlying stagger periodic-damage aura |
| Light Stagger | 124275 | stagger severity marker |
| Moderate Stagger | 124274 | stagger severity marker |
| Heavy Stagger | 124273 | stagger severity marker |
| Spear Hand Strike silence | 116709 | secondary silence effect |
| Surging Mist heal | 116995 | heal effect triggered by active Surging Mist 116694 |
| Enveloping Mist heal | 132120 | heal effect triggered by active Enveloping Mist 124682 |
| Mana Tea stacks | 115867 | stack aura consumed by Mana Tea |
| Mana Tea driver | 115869 | hidden Chi-consumption driver |
| Tigereye Brew stacks | 125195 | stack aura generated from Chi consumption and consumed by active Tigereye Brew 116740 |
| Tigereye Brew +1 visual | 125196 | visual/effect emitted when a Tigereye Brew stack is generated |
| Tigereye Brew full-stack visual | 137591 | visual controller threshold for accumulated Tigereye Brew stacks |
| Rising Sun Kick debuff | 130320 | debuff applied after active RSK hit |
| Expel Harm damage | 115129 | damage effect generated from Expel Harm healing |
| Touch of Karma redirected damage | 124280 | redirected-damage helper |
| Stance of the Wise Serpent | 115070 | Mistweaver stance/aura |

## Target-core behavior confirmed for strategy design

### Brewmaster

- Blackout Kick `100784` applies Shuffle `115307`, or extends an existing Shuffle duration.
- Keg Smash `121253` applies Weakened Blows `115798` and Dizzying Haze `116330`, then triggers energize `127796` after the cast.
- Breath of Fire `115181` gains its extra DoT only when Dizzying Haze is present on the target.
- Stagger is represented by `124255` plus Light/Moderate/Heavy severity auras `124275/124274/124273`.
- Purifying Brew `119582` removes the underlying Stagger and all three severity markers.
- Elusive Brew `115308` derives its duration from accumulated stack aura `128939` and then removes those stacks.

These facts justify explicit PlayerBot triggers for Shuffle maintenance and moderate/heavy Stagger purification rather than only generic health thresholds.

### Mistweaver

- Soothing Mist `115175` is a channel.
- Surging Mist `116694` and Enveloping Mist `124682` detect an active Soothing Mist channel and redirect their heal to that channel target; their casts can become directly triggered while channeling.
- Renewing Mist's 119611 periodic aura is linked to player spell `115151` and enables Uplift eligibility through target-core aura logic.
- Mana Tea `115294` consumes `115867` stacks over periodic ticks; the target-core Mana Tea driver only generates stacks from Chi consumption while in Stance of the Wise Serpent `115070`.
- Revival `115310` is a raid-area spell and explicitly excludes minor guardians from its target list.

These facts mean the final healer AI must become channel-aware rather than treating all Mistweaver heals as unrelated generic party heals.

### Windwalker

- Rising Sun Kick `107428` applies target debuff `130320`.
- Fists of Fury `113656` is implemented as an aura/channel-style periodic damage ability in the target core.
- The target core explicitly tracks Chi-consuming spells to generate Tigereye Brew stacks `125195` through the `123980` Brewing driver.
- Active Tigereye Brew `116740` consumes up to 10 accumulated stacks and scales its resulting damage buff from the consumed stack aura.
- The PlayerBot implementation now checks the exact stack aura ID `125195` and requests the active `tigereye brew` action at 10 stacks. It intentionally does not use the repository's generic `HasAuraStackTrigger`, because that helper forces `checkDuration=true` and the build-18414 DBC duration semantics for `125195` have not yet been verified.

These facts confirm that Energy/Chi flow and a 10-stack Tigereye Brew consumption decision are now represented in the static strategy, while full runtime validation remains required.

## Still requiring direct target-core confirmation or runtime/DBC validation

The initial PlayerBot framework resolves actions by spell name, so an ID alone is not enough. The following still need one or more of: exact active ID, spell-name resolution, resource cost, stance/spec restriction, cooldown, target type/range, aura dependency, or channel/cast behavior.

- Guard — target core explicitly binds `spell_monk_guard` to `123402` and `115295` as Monk ability variants, while `118604/136070` are statue-proc Guard variants. The exact PlayerBot spell-name resolution between the two player variants remains DBC/runtime pending; do not hardcode one blindly.
- Tigereye Brew — active cast `116740`, stack aura `125195`, and the static 10-stack decision are confirmed. Runtime still must prove that the PlayerBot `spell id` lookup for action name `tigereye brew` resolves the learned active player cast `116740` and that the exact stack aura is present as expected.
- Provoke — active ID is confirmed as 115546, but the current generic PlayerBot action targets the current target rather than a repository-native lost-aggro target.
- Life Cocoon — active ID is confirmed; correct emergency party target selection still needs runtime validation.
- Soothing/Surging/Enveloping — active IDs and channel interaction are confirmed; PlayerBot engine behavior while a channel is active still needs build/runtime verification.
- Fists of Fury — active ID is confirmed; movement/channel interruption safety still needs PlayerBot runtime validation.

## Source inconsistency noted

One Storm/Earth/Fire proc switch contains a comment labelling spell ID `100780` as `TigerPalm`, while the target core's dedicated Monk damage map and Power Strikes handling identify `100780` as Jab and `100787` as Tiger Palm. For this audit, the dedicated damage map is treated as the stronger local evidence. Do not encode the conflicting comment as a spell-ID mapping.

## Validation rule

A spell mapping or behavioral assumption is marked verified only with one of these forms of evidence:

1. an explicit target-core spell script/comment plus matching implementation use;
2. target-core DBC-backed spell lookup/resource data; or
3. a successful `PLAYERBOTS=1` build followed by target 5.4.8 runtime lookup/cast evidence.

Do not infer a verified MoP 5.4.8 ID solely from a modern WoW database, an external wiki, or another emulator branch.
