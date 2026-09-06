# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-06)

### Synchronization / overlap check

- [x] Upstream, fork `master`, and feature branch checked at the start of the current run.
- [x] Upstream `master`: `6f264eea5ac21c4315e529e91554f8c0dd45b232` (`[Core/StatSystem] Modernize Statesystem (#425)`).
- [x] Fork `master`: same `6f264eea5ac21c4315e529e91554f8c0dd45b232` SHA.
- [ ] Feature rebase onto `6f264eea...` remains pending. Current compare before this progress commit reports **67 ahead / 1 behind**, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`; feature HEAD before this progress commit was `d8a37aeaef807ab04184a1c885fccf79ed2f8a41`.
- [x] Upstream #425 does not modify any Monk feature path; its ThreatManager/TankTarget changes remain compatible with Monk Provoke's repository-native `tank target` selection.
- [x] No force update or invented rebase resolution was used.
- [x] Fresh upstream Issue/PR overlap search found no dedicated Monk PlayerBot combat-AI implementation. Issue #413 is bot command management; issue #150 is Tushui Monk NPC scripting.

### Environment state

- [ ] Real local Git/rebase is currently unavailable because the execution environment still cannot resolve `github.com` (`Could not resolve host: github.com`). A fresh `git ls-remote` retry in this run failed with the same DNS error.
- [ ] Real `PLAYERBOTS=1` compile is therefore still unavailable in this environment.
- [x] GitHub connector access is healthy and is used for source inspection/writes.
- [x] Fork workflow `.github/workflows/linux_gcc.yml` is configured for pushes, but GitHub Actions has not provided a usable feature-branch build result; no CI result is being treated as a build substitute.

## Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] Existing Monk specialization/role pipeline is present and no current evidence justifies changing it.
- [x] `RandomPlayerbotFactory.cpp` has no concrete Monk defect requiring modification and remains untouched.
- [x] Module source discovery is recursive: `modules/CMakeLists.txt` uses `file(GLOB_RECURSE ... *.cpp *.h)` for every valid module, so the new `modules/mod_playerbots/src/strategy/Classes/monk/` files are automatically part of the `modules` target when `PLAYERBOTS` is enabled and require no per-file CMake list entry.
- [x] Existing class contexts/strategies provide the Strategy / Trigger / Action / AiObjectContext architecture used by the Monk implementation.
- [x] `ValueContext` provides repository-native `tank target`; current upstream `TankTargetValue` uses ThreatManager state.
- [x] `PartyMemberToHealValue`, `party member to dispel`, and `group members` are the repository-local primitives used for Monk healing/dispels.
- [x] Generic `PlayerbotAI::CanCastSpell` preflight ignores power/reagent costs, while real `CastSpell` uses normal cast checks; Monk therefore requires its local DBC-backed power preflight to avoid repeated underfunded high-priority actions.
- [x] `Player::HasActiveSpell(uint32) const` is a real target-core API, so the Guard override resolver's `HasActiveSpell(123402/115295)` calls are statically API-compatible.
- [x] `SpellIdValue` resolves spell names only from active learned, non-passive spellbook entries. For multiple same-name unranked entries it iterates IDs in reverse order while repeatedly assigning `castSpellId`, so the numerically lowest matching ID wins. Monk Guard now avoids depending on this ambiguous same-name ordering.
- [x] Routine Fists of Fury retention has stronger repository-local protection than the earlier audit implied: generic `PlayerbotAI::CanCastSpell` returns false whenever `CURRENT_CHANNELED_SPELL` exists, and `ReachTargetAction::isUseful` also returns false while channeling. Thus normal Monk spell actions and the generic `reach melee` action cannot replace a running Fists channel. `set facing` only changes facing and does not issue movement. Runtime testing is still required for repository-wide emergency/formation/avoid-AoE movement, which may intentionally interrupt a channel for safety.

## Implementation checklist

### Common / context

- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/`.
- [x] Implement `MonkAiObjectContext` strategy/action/trigger factories.
- [x] Implement generic Monk combat strategy.
- [x] Implement generic Monk non-combat strategy.
- [x] Implement generic poison/disease Detox cure strategy.
- [x] Implement Monk action wrappers and DBC-derived power preflight for resource-sensitive actions.
- [x] Implement Monk triggers and factory keys.
- [x] Static cross-check of Monk-local action/trigger/strategy names against `MonkAiObjectContext`.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` construction in `Factory/AiFactory.cpp` **only after a real build passes**.

### Brewmaster

- [x] Stance of the Sturdy Ox maintenance.
- [x] Keg Smash / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Shuffle maintenance through Blackout Kick.
- [x] Moderate/Heavy Stagger -> Purifying Brew behavior.
- [x] Elusive Brew stack gating.
- [x] Provoke uses repository-native `tank target` and avoids redundant taunt when target already attacks the bot.
- [x] Fortifying Brew defensive trigger.
- [x] Guard no longer relies on generic same-name ordering: `CastGuardAction` prefers active spellbook override `123402`, falls back to active normal Guard `115295`, uses that exact ID for power/cast checks and execution, and treats either Guard aura as already active.
- [ ] Runtime prove how this target core exposes Glyph of Guard override `123402` through `HasActiveSpell`, then verify actual cast/aura behavior with and without the glyph.
- [ ] Runtime tanking/taunt/Guard/rotation validation.

### Mistweaver

- [x] Stance of the Wise Serpent maintenance.
- [x] Soothing Mist baseline.
- [x] Surging Mist and Enveloping Mist narrow exception for legal casts during Soothing Mist channel, preserving the channel target.
- [x] Renewing Mist prefers an eligible group member without this Monk's caster-owned HoT `119611`.
- [x] Uplift requires useful caster-owned Renewing Mist coverage before spending Chi.
- [x] Mana Tea stack gate.
- [x] Life Cocoon / Revival baseline.
- [x] Heal-target out-of-range movement recovery using the same repository pattern as existing healers.
- [x] Magic Detox implemented for Mistweaver: target core proves Detox `115450` suppresses its magic-dispel effect unless Internal Medicine `115451` is present. Magic self/party triggers require `115451`, and the magic party action is registered only through the Mistweaver strategy path.
- [x] Poison/disease Detox remains available to the generic Monk cure strategy.
- [ ] Runtime channel, healing cadence, Renewing Mist spread, Uplift, Mana Tea, and magic-dispel validation.

### Windwalker

- [x] Stance of the Fierce Tiger maintenance.
- [x] Rising Sun Kick / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Fists of Fury baseline with DBC-backed power gate.
- [x] Tigereye Brew uses exact stack aura `125195` and requests active `116740` at 10 stacks.
- [x] Touch of Karma low-health defensive path.
- [x] Touch of Death is Windwalker-only.
- [x] Static Fists of Fury routine-retention audit: normal spell actions are rejected by `CanCastSpell` while any channel exists, `reach melee` is explicitly useless while channeling, and `set facing` does not move. No Monk-specific blanket lock is needed for routine rotation actions. Runtime must still verify whether emergency/formation/avoid-AoE movement interrupts Fists appropriately rather than pathologically.
- [ ] Runtime Energy/Chi cadence, Tigereye Brew name resolution, and Fists of Fury movement/channel validation.

### Build / runtime / upstream contribution

- [ ] Rebase feature branch cleanly onto current upstream/fork `master` when Git network access is available.
- [ ] Configure/build successfully with `PLAYERBOTS=1`.
- [ ] Resolve all Monk-related compiler/linker failures introduced by this branch.
- [ ] In-game Monk bot creation/login and party join/follow.
- [ ] In-game Brewmaster validation.
- [ ] In-game Mistweaver validation.
- [ ] In-game Windwalker validation.
- [ ] Final upstream Monk AI Issue/PR overlap search.
- [ ] Create/update an upstream Monk AI implementation issue after build gate passes.
- [ ] Satisfy maintainer-response / 24-hour coordination gate.
- [ ] Open upstream implementation PR with scope, verification, and known limitations.

## Bugs / issues discovered

### Fixed statically — Guard same-name spellbook override ambiguity

- **Symptom:** normal Guard `115295` and Glyph-of-Guard override `123402` have the same spell name. Generic PlayerBot `SpellIdValue` collects active same-name spells and, for unranked matches, repeatedly overwrites the selected ID while iterating from high to low. If both variants are active, the generic resolver can finish on `115295` even when `123402` is the active spellbook override.
- **Target evidence:** target core binds the Guard script to the two player variants; MoP-era spell data identifies `123402` as the ability replacing Guard after Glyph of Guard is active.
- **Fix:** Monk-only `GetGuardSpellId` prefers `HasActiveSpell(123402)`, then falls back to `HasActiveSpell(115295)`. Guard power preflight, legality check, and execution use that exact resolved ID. `isUseful` treats either Guard aura as already active. Global `SpellIdValue` remains unchanged to avoid unrelated class regressions.
- **Commits:** `f8b3f222c26c40a52bcdd74f2a8a3435a6516cdc`, `4db14f55c9eab94f2df01472c5c9023cd86c6451`.
- **Verification:** `Player::HasActiveSpell(uint32) const` API and source path verified; real compile and glyph/no-glyph runtime test pending.

### Fixed previously — Touch of Death scope regression

- **Symptom:** progress history said Touch of Death had been made Windwalker-only, but source had reintroduced it into `GenericMonkStrategy` while `DpsMonkStrategy` lacked the trigger.
- **Fix:** remove the generic trigger and restore it in `DpsMonkStrategy` at high priority.
- **Commits:** `763865ee94afdeb83f7a16c8ed9a913ed3ac5680`, `97c9d26be32274b0e4f6590c1b44d4f595659401`.
- **Verification:** source/factory key audit; compile/runtime pending.

### Fixed previously — Mistweaver magic Detox missing

- **Symptom:** generic Monk cure handled poison/disease only; Mistweaver never requested Detox for dispellable magic effects.
- **Target-core evidence:** `spell_monk_detox` for active spell `115450` calls `PreventDefaultEffect` on its magic dispel unless the caster has `SPELL_MONK_INTERNAL_MEDICINE = 115451`.
- **Fix:** add `DISPEL_MAGIC` self/party trigger paths, explicit `115451` aura gating, `detox magic on party` action, factory registrations, and Mistweaver-only strategy triggers.
- **Commits:** `d1b9e8462dc4e97639926dd3462b1f28a54dcf0f`, `84bcd8c8ba04cecc4295386cadc8025a44991f21`, `801f1523a11763ed78f9f4884edf49bafbee5533`, `30d50f91efd5f0f449144b05ad3a0a66ecc6ce60`, `78673123799aa45689feab84e45fdfd0b73ad8c0`, `7111311c612e8dca343872dcb3bfcba7d2baf898`.
- **Verification:** target-core/static factory verification complete; compile/runtime pending.

### Previously fixed statically — power-preflight gap

- **Symptom:** generic PlayerBot preflight can select an action while ignoring its current Mana/Energy/Chi cost and only fail during real cast.
- **Fix:** Monk-local `HasPowerForSpell` resolves DBC power type/cost and checks current power before existing action preflight; no hardcoded Energy/Chi/Mana costs and no global PlayerBot behavior change.
- **Status:** static evidence complete; compile/runtime pending.

### Previously fixed statically — legal Mistweaver follow-up heals blocked while channeling

- **Symptom:** generic `CanCastSpell` rejects all new casts while a channeled spell exists.
- **Target-core evidence:** Surging Mist and Enveloping Mist explicitly support direct casts during Soothing Mist and redirect to the current channel target.
- **Fix:** narrow Monk-only channel exception, with learned spell/cooldown/power/immunity/range safety checks and final legality left to real cast/`Spell::CheckCast`.
- **Status:** static evidence complete; compile/runtime pending.

### Narrowed in current run — Fists of Fury interruption risk

- Earlier static inspection correctly found that `PlayerbotAI::UpdateAI` resumes normal engine evaluation once a channel is no longer in `SPELL_STATE_PREPARING`, but that alone overstated the risk of routine rotation self-interruption.
- `PlayerbotAI::CanCastSpell` explicitly rejects ordinary casts whenever `CURRENT_CHANNELED_SPELL` exists, so Jab/Tiger Palm/Blackout Kick/Rising Sun Kick and other normal spell actions cannot replace a running Fists channel through the standard path.
- `ReachTargetAction::isUseful` explicitly returns false while channeling, so Generic Monk's `reach melee` response does not move the bot during Fists of Fury.
- `SetFacingTargetAction` only updates facing and does not issue movement.
- **Decision:** no Monk-specific blanket channel lock is justified. Runtime validation remains necessary for global safety movement (formation, avoid-AoE, forced movement), where interrupting Fists may be the correct behavior.

### Environment blocker — transient DNS

- Local Git probe in the current run again failed: `Could not resolve host: github.com`.
- This prevents a trustworthy local rebase and real `PLAYERBOTS=1` compile but does **not** indicate a source build failure.
- GitHub connector operations remain functional, so non-overlapping source auditing and safe commits continue.
- Do not mark build or runtime boxes complete until actual evidence exists.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI implementation issue/PR found in the current run.
- #413: bot command management only.
- #150: Tushui Monk NPC behavior only.

## Key references

- Architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`, `druid/`, `shaman/`, `paladin/`, `priest/`
- Context/default strategies: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- Module build discovery: `modules/CMakeLists.txt`
- Generic cast behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`
- Generic reach/channel movement guard: `modules/mod_playerbots/src/strategy/actions/ReachTargetActions.cpp`
- Generic movement/facing behavior: `modules/mod_playerbots/src/strategy/actions/MovementActions.cpp`
- PlayerBot spell-name resolver: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- PlayerBot real/preflight cast behavior: `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Cure framework: `modules/mod_playerbots/src/strategy/triggers/CureTriggers.h/.cpp`
- Target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- Spell ledger: `MONK_SPELL_AUDIT.md`

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and Monk AI overlap at the start of the next run.
2. Retry local Git network access once. If restored, perform a clean rebase onto `6f264eea...`; abort and record exact conflict paths if any conflict is ambiguous.
3. Immediately run a real `PLAYERBOTS=1` build after the rebase. Do not enable the `AiFactory::createAiObjectContext` Monk case before this build gate.
4. If Git/build access is still transiently unavailable, continue static compiler/API audit without broadening scope. The remaining static priority is to inspect any unresolved group-value/target assumptions that affect Renewing Mist/Uplift and then re-check the complete Monk file set for compile-signature mismatches.
5. After a successful build, fix compile/link issues, then enable `MonkAiObjectContext` construction and rebuild before any upstream issue/PR step.