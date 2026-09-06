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
- [ ] Feature rebase onto `6f264eea...` remains pending. After the current Guard source commits and before this progress commit, compare reports **65 ahead / 1 behind**, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`.
- [x] Upstream #425 does not modify any Monk feature path; its ThreatManager/TankTarget changes remain compatible with Monk Provoke's repository-native `tank target` selection.
- [x] No force update or invented rebase resolution was used.
- [x] Fresh upstream Issue/PR overlap search found no dedicated Monk PlayerBot combat-AI implementation. Issue #413 is bot command management; issue #150 is Tushui Monk NPC scripting.

### Environment state

- [ ] Real local Git/rebase is currently unavailable because the execution environment still cannot resolve `github.com` (`Could not resolve host: github.com`).
- [ ] Real `PLAYERBOTS=1` compile is therefore still unavailable in this environment.
- [x] GitHub connector access is healthy and is used for source inspection/writes.
- [x] Fork workflow `.github/workflows/linux_gcc.yml` is configured for pushes, but GitHub Actions has not provided a usable feature-branch build result; no CI result is being treated as a build substitute.

## Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] Existing Monk specialization/role pipeline is present and no current evidence justifies changing it.
- [x] `RandomPlayerbotFactory.cpp` has no concrete Monk defect requiring modification and remains untouched.
- [x] Module source discovery is recursive; `Classes/monk/` does not require a per-file CMake list entry.
- [x] Existing class contexts/strategies provide the Strategy / Trigger / Action / AiObjectContext architecture used by the Monk implementation.
- [x] `ValueContext` provides repository-native `tank target`; current upstream `TankTargetValue` uses ThreatManager state.
- [x] `PartyMemberToHealValue`, `party member to dispel`, and `group members` are the repository-local primitives used for Monk healing/dispels.
- [x] Generic `PlayerbotAI::CanCastSpell` preflight ignores power/reagent costs, while real `CastSpell` uses normal cast checks; Monk therefore requires its local DBC-backed power preflight to avoid repeated underfunded high-priority actions.
- [x] `SpellIdValue` resolves spell names only from active learned, non-passive spellbook entries. For multiple same-name unranked entries it iterates IDs in reverse order while repeatedly assigning `castSpellId`, so the numerically lowest matching ID wins. Monk Guard now avoids depending on this ambiguous same-name ordering.

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
- [x] Static channel control audit: `PlayerbotAI::UpdateAI` only waits early for a current spell in `SPELL_STATE_PREPARING`; a running channeled spell can therefore continue into normal engine evaluation. No Monk-specific no-op/channel lock has been added yet because doing so could incorrectly suppress emergency movement/defensive behavior without runtime evidence.
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

### Fixed statically in current run — Guard same-name spellbook override ambiguity

- **Symptom:** normal Guard `115295` and Glyph-of-Guard override `123402` have the same spell name. Generic PlayerBot `SpellIdValue` collects active same-name spells and, for unranked matches, repeatedly overwrites the selected ID while iterating from high to low. If both variants are active, the generic resolver can finish on `115295` even when `123402` is the active spellbook override.
- **Target evidence:** target core binds the Guard script to the two player variants; MoP-era spell data identifies `123402` as the ability replacing Guard after Glyph of Guard is active.
- **Fix:** Monk-only `GetGuardSpellId` prefers `HasActiveSpell(123402)`, then falls back to `HasActiveSpell(115295)`. Guard power preflight, legality check, and execution use that exact resolved ID. `isUseful` treats either Guard aura as already active. Global `SpellIdValue` remains unchanged to avoid unrelated class regressions.
- **Commits:** `f8b3f222c26c40a52bcdd74f2a8a3435a6516cdc`, `4db14f55c9eab94f2df01472c5c9023cd86c6451`.
- **Verification:** source/API verification complete; real compile and glyph/no-glyph runtime test pending.

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

### Intentionally unresolved — Fists of Fury channel retention policy

- Static inspection confirms PlayerBot AI can continue normal engine evaluation while a channeled spell is running once it is past `SPELL_STATE_PREPARING`.
- For Fists of Fury this means movement or other eligible actions may be able to interrupt the channel, but a blanket channel-lock action would also be capable of suppressing legitimate emergency movement/defensives.
- **Decision:** keep current Fists of Fury cast implementation and require runtime evidence before adding Monk-specific channel retention/cancellation behavior.

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
- Generic cast behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`
- PlayerBot spell-name resolver: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- PlayerBot real/preflight cast behavior: `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Cure framework: `modules/mod_playerbots/src/strategy/triggers/CureTriggers.h/.cpp`
- Target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- Spell ledger: `MONK_SPELL_AUDIT.md`

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and Monk AI overlap at the start of the next run.
2. Retry local Git network access once. If restored, perform a clean rebase onto `6f264eea...`; abort and record exact conflict paths if any conflict is ambiguous.
3. Immediately run a real `PLAYERBOTS=1` build after the rebase. Do not enable the `AiFactory::createAiObjectContext` Monk case before this build gate.
4. If Git/build access is still transiently unavailable, continue static review for compiler/API mismatches and spec-specific behavior gaps without broadening scope. Prioritize Guard override API compatibility and Fists of Fury channel behavior evidence.
5. After a successful build, fix compile/link issues, then enable `MonkAiObjectContext` construction and rebuild before any upstream issue/PR step.
