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
- [ ] Feature rebase onto `6f264eea...` remains pending. Current compare after the non-combat Mistweaver fix reports **69 ahead / 1 behind**, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`; feature HEAD before this progress commit is `766a77e6467e9bead069041bbb045556209c6a83`.
- [x] Upstream #425 does not modify any Monk feature path; its ThreatManager/TankTarget changes remain compatible with Monk Provoke's repository-native `tank target` selection.
- [x] No force update or invented rebase resolution was used.
- [x] Fresh upstream Issue/PR overlap search found no dedicated Monk PlayerBot combat-AI implementation. Issue #413 is bot command management; issue #150 is Tushui Monk NPC scripting.

### Environment / validation state

- [ ] Real local Git/rebase is currently unavailable because the execution environment still cannot resolve `github.com` (`Could not resolve host: github.com`). A fresh Git network probe in this run failed with the same DNS error.
- [ ] Real local `PLAYERBOTS=1` compile is therefore still unavailable in this environment.
- [x] GitHub connector access is healthy and is used for source inspection/writes.
- [x] Fork-local validation PR #1 (`CI validation: Monk PlayerBot AI workstream`) was opened against fork `master` solely to obtain repository-native CI/build evidence. It is **not** the upstream contribution PR and must not be merged as a substitute for the contribution workflow.
- [ ] GitHub Actions has not produced a workflow run/status for the validation PR/head yet. No CI result is being treated as a build substitute.

## Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] Existing Monk specialization/role pipeline is present and no current evidence justifies changing it.
- [x] `RandomPlayerbotFactory.cpp` has no concrete Monk defect requiring modification and remains untouched.
- [x] Module source discovery is recursive: `modules/CMakeLists.txt` gathers module `.cpp/.h` files recursively, so the new Monk files require no per-file CMake registration.
- [x] Existing class contexts/strategies provide the Strategy / Trigger / Action / AiObjectContext architecture used by the Monk implementation.
- [x] `ValueContext` provides repository-native `tank target`; current upstream `TankTargetValue` uses ThreatManager state.
- [x] `PartyMemberToHealValue`, `party member to dispel`, and `group members` are repository-local healing/dispelling primitives.
- [x] `PartyMemberToHeal::Check` uses same-map, LOS, and `< healDistance * 2`; Monk Renewing Mist's fallback target scan mirrors that range/LOS envelope.
- [x] Generic `PlayerbotAI::CanCastSpell` preflight ignores power/reagent costs, while real `CastSpell` uses normal cast checks; Monk therefore uses class-local DBC-backed power preflight for resource-sensitive actions.
- [x] Target `SpellInfo` API exposes `GetPowerType(Unit const*, int32*)` and `CalcPowerCost(Unit const*, SpellSchoolMask, int32)`, matching the Monk helper's signatures.
- [x] `Player::HasActiveSpell(uint32) const` is a real target-core API, so the Guard override resolver's `HasActiveSpell(123402/115295)` calls are statically API-compatible.
- [x] `SpellIdValue` same-name unranked ordering can resolve the numerically lower ID; Monk Guard avoids depending on that ambiguity.
- [x] Routine Fists of Fury retention is protected by generic channel checks: ordinary casts and `reach melee` do not replace a running channel; runtime is still required for global safety/formation movement.
- [x] Non-combat `nc` is loaded for Monk through the universal non-combat strategy path. Because there is no separate Monk case in `AddDefaultNonCombatStrategies`, Mistweaver-specific out-of-combat behavior must be specialization-gated inside `GenericMonkNonCombatStrategy`.

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
- [x] Guard explicitly prefers active `123402`, falls back to active `115295`, uses the resolved ID for power/cast checks/execution, and treats either Guard aura as active.
- [ ] Runtime prove live Glyph of Guard spellbook override behavior and verify Guard with/without glyph.
- [ ] Runtime tanking/taunt/Guard/rotation validation.

### Mistweaver

- [x] Stance of the Wise Serpent maintenance in combat.
- [x] Soothing Mist baseline.
- [x] Surging Mist and Enveloping Mist narrow exception for legal casts during Soothing Mist channel, preserving the channel target.
- [x] Renewing Mist prefers an eligible group member without this Monk's caster-owned HoT `119611`.
- [x] Uplift requires useful caster-owned Renewing Mist coverage before spending Chi.
- [x] Mana Tea stack gate.
- [x] Life Cocoon / Revival baseline.
- [x] Heal-target out-of-range movement recovery using the same repository pattern as existing healers.
- [x] Magic Detox requires Internal Medicine `115451`; poison/disease Detox remains generic Monk utility.
- [x] Mistweaver non-combat support added in `GenericMonkNonCombatStrategy`: specialization-gated Wise Serpent stance, magic Detox, range recovery, and basic Soothing/Renewing/Surging/Enveloping party healing. Brewmaster/Windwalker do not receive these healer-only non-combat triggers. Commit `766a77e6467e9bead069041bbb045556209c6a83`.
- [ ] Runtime combat/non-combat channel, healing cadence, Renewing Mist spread, Uplift, Mana Tea, and magic-dispel validation.

### Windwalker

- [x] Stance of the Fierce Tiger maintenance.
- [x] Rising Sun Kick / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Fists of Fury baseline with DBC-backed power gate.
- [x] Tigereye Brew uses exact stack aura `125195` and requests active `116740` at 10 stacks.
- [x] Touch of Karma low-health defensive path.
- [x] Touch of Death is Windwalker-only.
- [x] Static Fists of Fury routine-retention audit complete; no blanket Monk-specific channel lock justified.
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

### Fixed this run — Mistweaver non-combat healer/Detox gap

- **Symptom:** Monk `nc` was universal but contained only poison/disease Detox. Mistweaver therefore had no out-of-combat Wise Serpent maintenance, no magic Detox, no party-heal range recovery, and no class-specific healing responses even though existing healer classes use their non-combat strategy for party recovery.
- **Cause:** `AiFactory::AddDefaultNonCombatStrategies` has no dedicated Monk branch; all Monk specs fall through to the universal `nc` strategy. Mistweaver-only utility had previously been added only to the combat `heal` strategy.
- **Fix:** keep generic poison/disease Detox for all Monks, then specialization-gate Mistweaver-only Wise Serpent, magic Detox, reach-heal-target, and conservative basic party healing inside `GenericMonkNonCombatStrategy`.
- **Commit:** `766a77e6467e9bead069041bbb045556209c6a83`.
- **Verification:** static factory/action/trigger/API audit complete; compile/runtime pending.

### Fixed statically — Guard same-name spellbook override ambiguity

- **Symptom:** normal Guard `115295` and override `123402` share a spell name; generic `SpellIdValue` can settle on the lower unranked ID.
- **Fix:** Monk-only `GetGuardSpellId` prefers `HasActiveSpell(123402)`, then `115295`; exact ID is used for power/cast/execution. Global resolver remains unchanged.
- **Commits:** `f8b3f222c26c40a52bcdd74f2a8a3435a6516cdc`, `4db14f55c9eab94f2df01472c5c9023cd86c6451`.
- **Verification:** API/static source verified; real compile and glyph/no-glyph runtime pending.

### Fixed previously — Touch of Death scope regression

- **Symptom:** Touch of Death had reappeared in generic Monk strategy.
- **Fix:** remove generic trigger and restore Windwalker-only high-priority trigger.
- **Commits:** `763865ee94afdeb83f7a16c8ed9a913ed3ac5680`, `97c9d26be32274b0e4f6590c1b44d4f595659401`.

### Fixed previously — Mistweaver magic Detox missing

- **Symptom:** no magic dispel path despite target-core Detox support with Internal Medicine.
- **Target-core evidence:** active Detox `115450` prevents its magic-dispel effect unless caster has `115451`.
- **Fix:** add magic self/party triggers, explicit `115451` gate, party action/factory registration, and Mistweaver combat wiring; non-combat wiring was added in the current run.
- **Core commits:** `d1b9e8462dc4e97639926dd3462b1f28a54dcf0f`, `84bcd8c8ba04cecc4295386cadc8025a44991f21`, `801f1523a11763ed78f9f4884edf49bafbee5533`, `30d50f91efd5f0f449144b05ad3a0a66ecc6ce60`, `78673123799aa45689feab84e45fdfd0b73ad8c0`, `7111311c612e8dca343872dcb3bfcba7d2baf898`.

### Fixed previously — resource preflight gap

- Generic PlayerBot preflight can ignore current Mana/Energy/Chi. Monk-local `HasPowerForSpell` now uses DBC/core power type and calculated cost before resource-sensitive actions. No global PlayerBot behavior changed.

### Fixed previously — legal Mistweaver follow-up heals blocked while channeling

- Generic `CanCastSpell` rejects new casts during a channel, while target-core Surging/Enveloping explicitly support direct casts during Soothing Mist. A narrow Monk-only exception preserves the current channel target and leaves final legality to the real cast path.

### Narrowed statically — Fists of Fury interruption risk

- Ordinary casts are rejected while `CURRENT_CHANNELED_SPELL` exists, and `reach melee` is useless while channeling. `set facing` does not move. No blanket channel lock is added; runtime must verify global safety/formation/avoid-AoE movement behavior.

### Environment blocker — transient DNS / CI unavailable

- Local Git probe still fails with `Could not resolve host: github.com`.
- This prevents trustworthy local rebase and real local `PLAYERBOTS=1` compile but is not a source build failure.
- Fork-local validation PR #1 exists to request repository-native CI evidence, but no workflow run/status has appeared yet.
- Do not mark build/runtime boxes complete until actual evidence exists.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI implementation issue/PR found in the current run.
- #413: bot command management only.
- #150: Tushui Monk NPC behavior only.
- Fork PR #1 is validation-only and is not an upstream coordination/contribution PR.

## Key references

- Architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`, `druid/`, `shaman/`, `paladin/`, `priest/`
- Context/default strategies: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- Module build discovery: `modules/CMakeLists.txt`
- Generic cast behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`
- Generic reach/channel movement guard: `modules/mod_playerbots/src/strategy/actions/ReachTargetActions.cpp`
- PlayerBot spell-name resolver: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- PlayerBot real/preflight cast behavior: `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Healing target behavior: `modules/mod_playerbots/src/strategy/value/PartyMemberToHealValue.cpp`
- Cure framework: `modules/mod_playerbots/src/strategy/triggers/CureTriggers.h/.cpp`
- Target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- Spell ledger: `MONK_SPELL_AUDIT.md`

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and Monk AI overlap at the start of the next run.
2. Check fork validation PR #1 for a GCC workflow run/status. If a run appears, inspect build jobs/logs and fix any Monk compiler/linker failures immediately.
3. Retry local Git network access once. If restored, rebase cleanly onto `6f264eea...`; abort and record exact conflict paths if any conflict is ambiguous.
4. Run a real `PLAYERBOTS=1` build after rebase/network recovery. Do not enable the `AiFactory::createAiObjectContext` Monk case before a real build gate passes.
5. If build access remains transiently unavailable, continue narrow static compiler/API review only; do not broaden feature scope or open the upstream contribution issue/PR.
6. After a successful build, fix compile/link issues, enable `MonkAiObjectContext` construction, rebuild, then proceed to runtime validation and the upstream coordination gate.
