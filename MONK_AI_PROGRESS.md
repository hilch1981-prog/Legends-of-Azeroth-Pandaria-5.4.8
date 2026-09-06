# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-07)

### Synchronization / overlap check

- [x] Upstream, fork `master`, and feature branch checked at the start of the current run.
- [x] Upstream `master`: `06fb98107158ee5a513e673a42674693fb4db7a2` (`Mordenize Combat System part 1 (#426)`).
- [x] Fork `master` is at the same `06fb98107158ee5a513e673a42674693fb4db7a2` SHA.
- [ ] Feature rebase onto current `06fb9810...` remains pending. Compare immediately before the first progress update in this run reported **92 ahead / 2 behind**, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`; feature source/doc HEAD at that point was `467bfbd6092aea89d0b41340005f316019f37d9b`.
- [x] Upstream #426 was audited for PlayerBot overlap. It updates generic threat APIs (`getThreat` -> `GetThreat`) in `GenericTriggers.cpp`, `TankTargetValue.cpp`, `AttackersValue.cpp`, `DpsTargetValue.cpp`, and `ThreatValues.cpp` plus core combat/threat internals. The Monk feature does not directly call the removed lowercase threat API and continues to consume repository-native `tank target`, so no Monk source rewrite is currently required by #426.
- [x] No force update or invented rebase resolution was used.
- [x] Fresh upstream Issue/PR overlap search found no dedicated Monk PlayerBot combat-AI implementation. Historical Monk PRs found previously are unrelated to PlayerBot combat AI: #363 fixes Earth/Wind/Fire crashes, #168 adjusts Monk starting action bars, and #268 adjusts Pandaren action bars/stance behavior. Issue #413 is bot command management; issue #150 is Tushui Monk NPC scripting.

### Environment / validation state

- [ ] Real local Git/rebase is currently unavailable because the execution environment still cannot resolve `github.com` (`Could not resolve host: github.com`). The local Git network probe in this run failed with the same DNS error.
- [ ] Real local `PLAYERBOTS=1` compile is therefore still unavailable in this environment.
- [x] GitHub connector access is healthy and is used for source inspection/writes.
- [x] Fork-local validation PR #1 (`CI validation: Monk PlayerBot AI workstream`) remains open against fork `master` and is used only for repository-side validation signals. It is **not** the upstream contribution PR and must not be merged as a substitute for the contribution workflow.
- [ ] The current normalized PR #1 snapshot reports `mergeable: false`, but also still reports stale base SHA `6f264eea...` while fork `master` is actually `06fb9810...`; the direct compare API reports only the expected feature/master divergence. Treat this PR mergeability flag as stale/unresolved rather than proof of a source conflict; do not force-update either branch.
- [ ] GitHub Actions still has zero workflow runs for the feature branch. No CI result is being treated as a build substitute.
- [x] The repository's default GCC workflow is **not** a valid PlayerBot build gate: it configures with `cmake ../ -DTOOLS=1 -DELUNA=0 ...` and omits `-DPLAYERBOTS=1`, while `modules/CMakeLists.txt` removes `mod_playerbots` whenever `PLAYERBOTS` is false. Even a green default workflow cannot prove the Monk module compiles unless PlayerBots are explicitly enabled.
- [x] A fork-only validation branch `ci/monk-ai-playerbots` was created from feature commit `c07efd3e0347761bb6f0d2c64e115172618664ce`. Only that validation branch changes `.github/workflows/linux_gcc.yml`, adding `-DPLAYERBOTS=1`; validation commit `0c51fa9c3ecb7e3ae97f419296217bd208175096`. The feature branch/upstream contribution diff is not contaminated by this CI-only change.
- [ ] Immediately after the CI-only push, the branch still has zero workflow runs/status contexts. This is an infrastructure/Actions availability signal, not a compile result; re-check on a later run without treating absence as PASS or FAIL.

## Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] Existing Monk specialization/role pipeline is present and no current evidence justifies changing it.
- [x] `RandomPlayerbotFactory.cpp` has no concrete Monk defect requiring modification and remains untouched.
- [x] Module source discovery is recursive: `modules/CMakeLists.txt` gathers module `.cpp/.h` files recursively, so the new Monk files require no per-file CMake registration.
- [x] Existing class contexts/strategies provide the Strategy / Trigger / Action / AiObjectContext architecture used by the Monk implementation.
- [x] `ValueContext` provides repository-native `tank target`; current upstream `TankTargetValue` uses ThreatManager state.
- [x] Upstream #426 renamed the ThreatManager getter used by PlayerBot values to `GetThreat`. Monk Provoke does not call ThreatManager directly and is insulated behind `tank target`.
- [x] `PartyMemberToHealValue`, `party member to dispel`, and `group members` are repository-local healing/dispelling primitives.
- [x] `PartyMemberToHeal::Check` uses same-map, LOS, and `< healDistance * 2`; Monk Renewing Mist's fallback target scan mirrors that range/LOS envelope.
- [x] `GroupMembersValue` returns the full group member list and falls back to the bot itself when solo; Monk group scans therefore have a repository-native source and do not depend on an invented roster.
- [x] Generic `PlayerbotAI::CanCastSpell` preflight ignores power/reagent costs, while real `CastSpell` uses normal cast checks; Monk therefore uses class-local DBC-backed power preflight for resource-sensitive actions.
- [x] Target `SpellInfo` API exposes `GetPowerType(Unit const*, int32*)` and `CalcPowerCost(Unit const*, SpellSchoolMask, int32)`, matching the Monk helper's signatures.
- [x] `Player::HasActiveSpell(uint32) const` is a real target-core API, so Monk-local Guard and Mana Tea override resolvers are statically API-compatible.
- [x] `SpellIdValue` same-name unranked ordering can resolve the numerically lower ID; Monk Guard and Mana Tea avoid depending on that ambiguity.
- [x] `PlayerbotAI::GetAura(std::string, ...)` scans aura spell names, while `CastBuffSpellAction::isUseful()` delegates to that name-based aura lookup. Stack-consuming/channel abilities should not depend on buff-style "already has aura" usefulness semantics.
- [x] Target core explicitly implements normal Mana Tea `115294` as a channel/aura that consumes one `115867` stack on apply and another per periodic tick, and glyphed Mana Tea `123761` as an instant spell whose `CheckCast` requires at least two stacks.
- [x] Routine Fists of Fury retention is protected by generic channel checks: ordinary casts and `reach melee` do not replace a running channel; runtime is still required for global safety/formation movement.
- [x] Non-combat `nc` is loaded for Monk through the universal non-combat strategy path. Because there is no separate Monk case in `AddDefaultNonCombatStrategies`, Mistweaver-specific out-of-combat behavior must be specialization-gated inside `GenericMonkNonCombatStrategy`.
- [x] Target-core `spell_monk.cpp` explicitly identifies Combo Breaker: Tiger Palm as aura `118864` and Combo Breaker: Blackout Kick as aura `116768`; both are emitted by `spell_monk_combo_breaker`. MoP-era spell data corroborates Tiger Power as aura `125359`; live build-18414 runtime verification remains pending.
- [x] Target-core `spell_monk_fortifying_brew` is an AuraScript on `120954`. This proves `120954` is an aura/effect ID, **not** that it is the learned player cast ID. The PlayerBot action remains spell-name resolved and does not hardcode `120954`; MoP-era data points to player cast `115203`, which remains pending target DBC/runtime proof.
- [x] Target-core `spell_monk_muscle_memory` is wired to Jab variants and casts exact buff `139597` when the Monk knows passive `139598`; the Spinning Crane Kick damage script can also grant `139597` after three hits with the same passive check.

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
- [x] Fortifying Brew defensive trigger remains name-resolved; no unproven aura/effect ID is hardcoded as the player cast.
- [x] Guard explicitly prefers active `123402`, falls back to active `115295`, uses the resolved ID for power/cast checks/execution, and treats either Guard aura as active.
- [ ] Runtime/DBC prove Fortifying Brew learned cast ID and resulting aura.
- [ ] Runtime prove live Glyph of Guard spellbook override behavior and verify Guard with/without glyph.
- [ ] Runtime tanking/taunt/Guard/rotation validation.

### Mistweaver

- [x] Stance of the Wise Serpent maintenance in combat.
- [x] Soothing Mist baseline.
- [x] Surging Mist and Enveloping Mist narrow exception for legal casts during Soothing Mist channel, preserving the channel target.
- [x] Renewing Mist prefers an eligible group member without this Monk's caster-owned HoT `119611`.
- [x] Uplift requires useful caster-owned Renewing Mist coverage before spending Chi.
- [x] Mana Tea resolves active variants explicitly: prefer glyphed `123761` when active, otherwise `115294`; normal channel is allowed from one stack and glyphed cast retains the target-core two-stack minimum.
- [x] Life Cocoon / Revival baseline.
- [x] Heal-target out-of-range movement recovery using the same repository pattern as existing healers.
- [x] Magic Detox requires Internal Medicine `115451`; poison/disease Detox remains generic Monk utility.
- [x] Mistweaver non-combat support added in `GenericMonkNonCombatStrategy`: specialization-gated Wise Serpent stance, magic Detox, range recovery, and basic Soothing/Renewing/Surging/Enveloping party healing. Brewmaster/Windwalker do not receive these healer-only non-combat triggers. Commit `766a77e6467e9bead069041bbb045556209c6a83`.
- [x] Remove unconditional Mistweaver Tiger Palm default Chi spending: Jab is now the healthy-party melee fallback, and exact Muscle Memory aura `139597` triggers Tiger Palm at default-level relevance below healing actions. Source commits `85fc716466cf69f62d81349f2a54b2cc123bb58d`, `52f38ba9ac22b94693150ab30fae837619aa1e81`, `5a44a35a7f89daaf45fd3ddbc36efef1379f2d8e`, `40b8530758b097127ccc0f9029aa6ea0477f86f7`.
- [ ] Runtime combat/non-combat channel, healing cadence, Renewing Mist spread, Uplift, Mana Tea normal/glyphed behavior, Muscle Memory/Jab/Tiger Palm cadence, and magic-dispel validation.

### Windwalker

- [x] Stance of the Fierce Tiger maintenance.
- [x] Rising Sun Kick / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Tiger Palm removed from unconditional default filler; it is requested for missing Tiger Power (`125359`) or Combo Breaker: Tiger Palm (`118864`) so normal Chi can reach Blackout Kick/Fists/Rising Sun Kick instead of being consumed every default pass.
- [x] Combo Breaker free-spender support: Tiger Palm `118864` and Blackout Kick `116768` bypass only the conservative Monk power preflight while the exact proc aura is active, leaving the normal cast legality path intact. Blackout Kick now has an explicit high-priority proc trigger/factory key as well.
- [x] Fists of Fury baseline with DBC-backed power gate.
- [x] Tigereye Brew uses exact stack aura `125195` and requests active `116740` at 10 stacks.
- [x] Touch of Karma low-health defensive path.
- [x] Touch of Death is Windwalker-only.
- [x] Static Fists of Fury routine-retention audit complete; no blanket Monk-specific channel lock justified.
- [ ] Runtime Energy/Chi cadence, Tiger Power/Combo Breaker handling, Tigereye Brew name resolution, and Fists of Fury movement/channel validation.

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

### Fixed this run — Mana Tea variant resolution and stack gating were too generic

- **Symptom 1:** `CastManaTeaAction::isUseful()` required at least two `115867` stacks for every Mana Tea cast, so the normal `115294` channel could not be used with exactly one stack even though the target core consumes one stack immediately on aura application.
- **Symptom 2:** the action inherited generic same-name spell resolution despite target core defining a second active Mana Tea variant, glyphed `123761`; this has the same class of override ambiguity already proven for Guard.
- **Target-core evidence:** `// 115294 - Mana Tea` removes one `115867` stack on apply and one per periodic tick. `// 123761 - Mana Tea` has an explicit `CheckCast` requiring at least two `115867` stacks.
- **Repository evidence:** `Player::HasActiveSpell` is available; generic string-based casts use `SpellIdValue`; and buff-style usefulness performs a name-based aura lookup, which is the wrong semantic dependency for a stack-consuming regen action.
- **Fix:** add Monk-local `GetManaTeaSpellId`, prefer active `123761`, fall back to active `115294`, execute and preflight with the exact ID, permit normal Mana Tea from one stack, and retain the two-stack minimum only for the glyphed variant. The action no longer depends on `CastBuffSpellAction::isUseful()`.
- **Commit:** `15fa4919b7a995d0e164fb89841a8dbcabe7ff9c`; spell ledger updated by `467bfbd6092aea89d0b41340005f316019f37d9b`.
- **Verification:** target-core spell scripts and repository APIs were statically verified. Real compile plus live glyph/no-glyph spellbook behavior and stack consumption remain pending.

### Fixed previously — Mistweaver default Tiger Palm could starve healing Chi

- **Symptom:** `HealMonkStrategy::getDefaultActions()` kept `tiger palm` as an unconditional melee fallback after Soothing/Renewing Mist. When a Mistweaver entered melee with Chi available, that default action could spend Chi without a Muscle Memory proc and compete with Enveloping Mist/Uplift resource needs.
- **Target-core evidence:** `spell_monk_muscle_memory` is attached to Jab variants and casts exact proc aura `139597` when the Monk knows passive `139598`. The Spinning Crane Kick damage script can also grant `139597` after three hits under the same passive check.
- **Fix:** replace unconditional Mistweaver Tiger Palm with Jab as the default melee fallback; add exact `139597` `MuscleMemoryTrigger`, register it in `MonkAiObjectContext`, and request Tiger Palm only from that proc at `ACTION_DEFAULT + 0.25f`, below actual healing trigger relevance.
- **Commits:** `85fc716466cf69f62d81349f2a54b2cc123bb58d`, `52f38ba9ac22b94693150ab30fae837619aa1e81`, `5a44a35a7f89daaf45fd3ddbc36efef1379f2d8e`, `40b8530758b097127ccc0f9029aa6ea0477f86f7`; spell ledger updated by `6693f5d79d644db72aa06931f3d8db11c2deeacf`.
- **Verification:** target-core proc generation and factory/action key wiring verified statically. Real compile and live Jab -> Muscle Memory -> Tiger Palm cadence/mana interaction remain pending.

### Fixed previously — Combo Breaker free procs could be rejected by conservative Monk power preflight

- **Symptom:** the Windwalker strategy already reacted to Combo Breaker: Tiger Palm, but `CastTigerPalmAction::isPossible()` still required the normal DBC Chi cost to be available before the cast. Blackout Kick had the same preflight behavior and no explicit Combo Breaker trigger. If `CalcPowerCost` does not fold the proc aura into the preflight result, a valid free proc could be skipped at zero Chi.
- **Target-core evidence:** `spell_monk_combo_breaker` explicitly emits `118864` for Tiger Palm and `116768` for Blackout Kick. These are exact target-core proc auras, not donor-only IDs.
- **Fix:** while the corresponding exact proc aura is present, bypass only `HasPowerForSpell` for Tiger Palm/Blackout Kick and still run `CastMeleeSpellAction::isPossible()` plus the normal real-cast legality path. Add `combo breaker blackout kick` trigger/factory registration and prioritize the free Blackout Kick proc before the Tiger Palm proc.
- **Commits:** `606bf515d88f82dfa075083a296de8e5f07711ac`, `ea0c88d2479d4b1b03b5635b3b2893ea8002c590`, `a7e93cd4ca7693e9a6976186309b58ac604b8073`, `082910bb1af94171da70061f4a118f50f93b6736`, `954fd28cc811691fe52cdf536d860b2d09df5de5`.
- **Verification:** target-core proc IDs and proc-emission script verified statically; compile/live free-proc behavior remains pending.

### Fixed previously — Windwalker Tiger Palm default filler could starve Chi spenders

- **Symptom:** `DpsMonkStrategy::getDefaultActions()` placed `tiger palm` ahead of `blackout kick`. For Windwalker, Tiger Palm is a Chi spender; when Chi was available the default engine could repeatedly select Tiger Palm before Blackout Kick, degrading the intended Energy -> Chi -> spender cadence and potentially starving the stronger Chi spenders.
- **Cause:** the initial baseline treated Tiger Palm like a generic filler instead of a maintenance/proc action. That is acceptable for Brewmaster's different Tiger Palm semantics but not for Windwalker.
- **Fix:** remove Tiger Palm from the Windwalker default list; add exact-aura triggers for missing Tiger Power (`125359`) and Combo Breaker: Tiger Palm (`118864`); register both triggers in `MonkAiObjectContext`; preserve Brewmaster behavior unchanged.
- **Commits:** `a830ff938a5b40975a95a61fe45c2e56bdda081d`, `9d7c39f776b0d07a708d87d15dff009a8ee0af41`, `c241382116226ed641cc3bc7a38ee2afc43882a0`, `e2490be8485ea540cf3f9ae271c050508ed0c0c2`.
- **Verification:** factory/key/API static audit complete. Exact Combo Breaker aura is present in target-core Monk enums; Tiger Power ID is corroborated by MoP-era spell data. Build and live cadence remain pending.

### Corrected previously — Fortifying Brew `120954` was over-classified in the spell ledger

- **Symptom:** `MONK_SPELL_AUDIT.md` previously listed `120954` as the confirmed active Fortifying Brew cast because the target-core comment says `// 120954 - Fortifying Brew`.
- **Cause:** the referenced implementation is an `AuraScript`; the script establishes an aura/effect ID but does not establish the learned spellbook cast ID.
- **Fix:** move `120954` to confirmed related/internal IDs, keep the PlayerBot action name-resolved, and record MoP-era `115203` only as corroboration pending target DBC/runtime proof.
- **Commit:** `2dac31107560dbec3ae23933fffb753b4cd61e53`.
- **Verification:** target-core script type audited; no source behavior change required because Monk code never hardcoded `120954` for casting.

### Fixed previously — Mistweaver non-combat healer/Detox gap

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
- **Fix:** add magic self/party triggers, explicit `115451` gate, party action/factory registration, and Mistweaver combat wiring; non-combat wiring was added later.
- **Core commits:** `d1b9e8462dc4e97639926dd3462b1f28a54dcf0f`, `84bcd8c8ba04cecc4295386cadc8025a44991f21`, `801f1523a11763ed78f9f4884edf49bafbee5533`, `30d50f91efd5f0f449144b05ad3a0a66ecc6ce60`, `78673123799aa45689feab84e45fdfd0b73ad8c0`, `7111311c612e8dca343872dcb3bfcba7d2baf898`.

### Fixed previously — resource preflight gap

- Generic PlayerBot preflight can ignore current Mana/Energy/Chi. Monk-local `HasPowerForSpell` now uses DBC/core power type and calculated cost before resource-sensitive actions. No global PlayerBot behavior changed.

### Fixed previously — legal Mistweaver follow-up heals blocked while channeling

- Generic `CanCastSpell` rejects new casts during a channel, while target-core Surging/Enveloping explicitly support direct casts during Soothing Mist. A narrow Monk-only exception preserves the current channel target and leaves final legality to the real cast path.

### Narrowed statically — Fists of Fury interruption risk

- Ordinary casts are rejected while `CURRENT_CHANNELED_SPELL` exists, and `reach melee` is useless while channeling. `set facing` does not move. No blanket channel lock is added; runtime must verify global safety/formation/avoid-AoE movement behavior.

### Environment blocker — transient DNS / no valid PlayerBot CI evidence

- Local Git probe still fails with `Could not resolve host: github.com`.
- This prevents trustworthy local rebase and real local `PLAYERBOTS=1` compile but is not a source build failure.
- Fork-local validation PR #1 currently has no workflow run, and its normalized mergeability/base metadata is stale relative to current fork `master`.
- The repository's default GCC workflow does not set `PLAYERBOTS=1`; `modules/CMakeLists.txt` therefore excludes `mod_playerbots` in that workflow. A future green default GCC run must **not** be counted as the Monk build gate.
- A separate fork-only `ci/monk-ai-playerbots` branch now supplies the missing `-DPLAYERBOTS=1` flag without altering the feature branch. Its first push currently has no Actions run/status context, so CI availability itself remains the blocker.
- Do not mark build/runtime boxes complete until actual `PLAYERBOTS=1` compile/runtime evidence exists.

## External overlap / coordination

- Fresh searches found no dedicated overlapping Monk PlayerBot combat-AI implementation issue/PR in the current run.
- Historical Monk PRs #363, #168, and #268 are unrelated to this PlayerBot combat-AI workstream.
- #413: bot command management only.
- #150: Tushui Monk NPC behavior only.
- Fork PR #1 and `ci/monk-ai-playerbots` are validation-only and are not upstream coordination/contribution artifacts.

## Key references

- Architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`, `druid/`, `shaman/`, `paladin/`, `priest/`
- Context/default strategies: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- Module build discovery: `modules/CMakeLists.txt`
- Generic cast behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`
- Generic reach/channel movement guard: `modules/mod_playerbots/src/strategy/actions/ReachTargetActions.cpp`
- PlayerBot spell-name resolver: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- PlayerBot real/preflight cast behavior and name-based aura lookup: `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Healing target behavior: `modules/mod_playerbots/src/strategy/value/PartyMemberToHealValue.cpp`
- Group roster behavior: `modules/mod_playerbots/src/strategy/value/GroupValues.cpp`
- Cure framework: `modules/mod_playerbots/src/strategy/triggers/CureTriggers.h/.cpp`
- Target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- Spell ledger: `MONK_SPELL_AUDIT.md`

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and Monk AI overlap at the start of the next run.
2. Re-check `ci/monk-ai-playerbots` commit `0c51fa9c...` for an Actions run/status. If a real `PLAYERBOTS=1` build appears, inspect and fix every Monk compiler/linker failure immediately; if no run exists, do not infer a build result.
3. Retry local Git network access once. If restored, rebase cleanly onto current upstream/fork `master`; abort and record exact conflict paths if any conflict is ambiguous.
4. Configure a real build with `-DPLAYERBOTS=1` after rebase/network recovery. Do not rely on the repository's unmodified default GCC workflow because it excludes `mod_playerbots` without that flag.
5. After a pre-activation PlayerBot build passes, enable `MonkAiObjectContext` construction in `AiFactory.cpp`, rebuild again with `PLAYERBOTS=1`, and fix any activation-only failures.
6. After both PlayerBot builds pass, perform Brewmaster/Mistweaver/Windwalker runtime validation including Guard and Mana Tea glyph/no-glyph paths, Mistweaver Jab -> Muscle Memory `139597` -> Tiger Palm cadence, and Fists of Fury channel behavior before entering the upstream coordination gate.