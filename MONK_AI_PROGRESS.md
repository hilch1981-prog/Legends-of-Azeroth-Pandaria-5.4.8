# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Current state — 2026-09-07

### Synchronization / overlap

- [x] Upstream, fork `master`, and feature branch checked at the start of this run and upstream re-checked before handoff.
- [x] Upstream `master`: `06fb98107158ee5a513e673a42674693fb4db7a2` (`Mordenize Combat System part 1 (#426)`).
- [x] Fork `master`: same `06fb98107158ee5a513e673a42674693fb4db7a2`.
- [x] Feature branch was safely synchronized with current fork/upstream `master` through fork-local sync PR #3 (`master` -> `feature/monk-ai-object-context`) after GitHub reported the merge as clean. The merge commit is `5041141c2846d2da9d16e69aaf57e4cdb312f8c3`; no force-push or history rewrite was used.
- [x] Post-sync compare: feature **101 ahead / 0 behind**, with merge base equal to current `master` `06fb98107158ee5a513e673a42674693fb4db7a2` before this documentation commit.
- [x] Upstream #426 audited: it changes generic combat/threat APIs, but Monk Provoke uses repository-native `tank target` and does not directly call the renamed ThreatManager getter.
- [x] Fresh upstream Issue/PR searches found no materially overlapping Monk PlayerBot combat-AI implementation. Historical Monk PRs #363/#168/#268 and issues #413/#150 remain unrelated to this workstream.

### Validation environment

- [ ] Local Git and local compile remain blocked by environment connectivity (`Could not resolve host: github.com`; direct outbound connectivity also unavailable). Current source synchronization is no longer blocked because the master changes were merged through GitHub itself.
- [x] GitHub connector reads/writes are healthy.
- [x] Fork PR #1 remains validation-only and is **not** the upstream contribution PR. GitHub mergeability has fluctuated while head/base moved, so it is not treated as a compile or conflict gate.
- [ ] Feature branch still has zero GitHub Actions runs after synchronization.
- [x] Repository default GCC workflow is **not** a valid PlayerBot build gate: it omits `-DPLAYERBOTS=1`, while `modules/CMakeLists.txt` removes `mod_playerbots` when `PLAYERBOTS` is false.
- [x] Fork-only branch `ci/monk-ai-playerbots` was created from feature source. Validation commit `0c51fa9c3ecb7e3ae97f419296217bd208175096` changes only `.github/workflows/linux_gcc.yml` to add `-DPLAYERBOTS=1`; this change is intentionally absent from the feature branch/upstream contribution diff.
- [x] Fork-local PR #2 (`ci/monk-ai-playerbots` -> `master`) remains open specifically as build validation. It is mergeable, but it is build-validation-only, is **not** the upstream implementation PR, and must not be merged.
- [ ] PR #2 / `ci/monk-ai-playerbots` still has zero workflow runs/status contexts after the PR-open event. The fork reports no Actions run for that branch, so no CI PASS/FAIL is inferred.

## Architecture / implementation state

### Common

- [x] `modules/mod_playerbots/src/strategy/Classes/monk/` implemented.
- [x] `MonkAiObjectContext` Strategy/Trigger/Action factories implemented and keys statically cross-checked.
- [x] Generic combat, non-combat, AoE, and cure strategies implemented.
- [x] Monk-local DBC-derived Mana/Energy/Chi preflight implemented for resource-sensitive actions; global PlayerBot behavior unchanged.
- [x] Current upstream `SpellInfo.h` revalidates the exact APIs used by that preflight: `GetPowerType(Unit const*, int32*)` and `CalcPowerCost(Unit const*, SpellSchoolMask, int32)`.
- [x] Current generic action code revalidates target semantics: `CastHealingSpellAction` targets self (appropriate for Revival/Uplift), while party heals use `HealPartyMemberAction`/explicit target overrides.
- [x] Current generic `CastSpellAction::Execute` calls `PlayerbotAI::CastSpell` directly, so the narrow Surging/Enveloping-during-Soothing `isPossible()` exception is not immediately re-run through the generic channel rejection before execution; final legality still remains in the core cast path.
- [x] `MonkActions.cpp` directly includes `ObjectAccessor.h` for its `ObjectAccessor::FindPlayer` group lookups instead of relying on transitive includes. Source hardening commit: `c9346401e269971ed8d1434be1873116f0182e38`.
- [x] `RandomPlayerbotFactory.cpp` remains untouched because no concrete defect requires modification.
- [x] Module source discovery is recursive; no per-file CMake registration is required for the new Monk files.
- [ ] `MonkAiObjectContext` construction in `Factory/AiFactory.cpp` remains intentionally disabled until a real pre-activation `PLAYERBOTS=1` build passes.

### Brewmaster

- [x] Stance of the Sturdy Ox.
- [x] Keg Smash / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Shuffle maintenance through Blackout Kick.
- [x] Moderate/Heavy Stagger -> Purifying Brew.
- [x] Elusive Brew stack gate.
- [x] Provoke uses repository-native `tank target` and avoids redundant taunt.
- [x] Guard resolves active glyph override `123402` before normal `115295`, then uses the exact ID for legality/execution and recognizes either Guard aura as active.
- [x] Fortifying Brew remains spell-name resolved; target-core `120954` is only proven as an aura/script ID, not the learned player cast.
- [ ] Runtime/DBC Fortifying Brew cast-ID proof.
- [ ] Runtime Guard glyph/no-glyph and tanking validation.

### Mistweaver

- [x] Stance of the Wise Serpent in combat plus specialization-gated non-combat recovery.
- [x] Soothing Mist baseline.
- [x] Surging/Enveloping Mist narrow legal-cast exception during Soothing Mist while preserving the channel target.
- [x] Renewing Mist spreads to a valid group member without this Monk's caster-owned `119611` HoT.
- [x] Uplift requires useful caster-owned Renewing Mist coverage before spending Chi.
- [x] Life Cocoon / Revival baseline and heal-target range recovery.
- [x] Magic Detox requires Internal Medicine `115451`; poison/disease Detox remains generic Monk utility.
- [x] Healthy-party melee fallback is Jab; Tiger Palm is requested from exact Muscle Memory proc `139597`, avoiding unconditional healing-Chi spending.
- [x] Mana Tea same-name variants are explicit: active glyphed `123761` is preferred, otherwise normal `115294`; normal is useful from one stack while glyphed requires two.
- [x] Target-core normal `115294` Mana Tea sizes channel duration from current `115867` stacks and consumes one stack per periodic tick; glyphed `123761` requires/consumes two stacks. The one-stack normal threshold remains correct.
- [ ] Runtime normal/glyphed Mana Tea, healing cadence, Renewing Mist/Uplift, Muscle Memory cadence, and magic-dispel validation.

### Windwalker

- [x] Stance of the Fierce Tiger.
- [x] Rising Sun Kick / Blackout Kick / Jab baseline.
- [x] Tiger Palm is maintenance/proc-driven instead of unconditional filler: missing Tiger Power `125359` or Combo Breaker: Tiger Palm `118864`.
- [x] Combo Breaker Tiger Palm `118864` / Blackout Kick `116768` free procs bypass only conservative Monk power preflight.
- [x] Fists of Fury baseline with DBC power gate.
- [x] Tigereye Brew uses exact stack aura `125195` and requests active `116740` at 10 stacks.
- [x] Touch of Karma defensive path.
- [x] Touch of Death remains Windwalker-only.
- [x] Static Fists of Fury channel-retention audit found no basis for a blanket Monk channel lock.
- [ ] Runtime Energy/Chi cadence, Tiger Power/Combo Breaker, Tigereye Brew, and Fists of Fury movement/channel validation.

## Key fixes in the current implementation

- Safe master synchronization without rewriting feature history: fork-local PR #3, merge commit `5041141c2846d2da9d16e69aaf57e4cdb312f8c3`.
- Mana Tea explicit variant resolution and 1-stack normal / 2-stack glyph thresholds: `15fa4919b7a995d0e164fb89841a8dbcabe7ff9c`.
- Mana Tea spell evidence correction: `9313d49c15fb92d8e41ea7a48099712ac3ae1f59`; source-comment/direct-include hardening: `c9346401e269971ed8d1434be1873116f0182e38`.
- Mistweaver Jab -> Muscle Memory `139597` -> Tiger Palm flow: `85fc7164`, `52f38ba9`, `5a44a35a`, `40b85307`.
- Combo Breaker free-spender handling: `606bf515`, `ea0c88d2`, `a7e93cd4`, `082910bb`, `954fd28c`.
- Windwalker Tiger Palm maintenance/proc gating: `a830ff93`, `9d7c39f7`, `c2413821`, `e2490be8`.
- Guard explicit override resolution: `f8b3f222`, `4db14f55`.
- Mistweaver non-combat support: `766a77e6`.
- Mistweaver Internal Medicine-aware magic Detox: `d1b9e846`, `84bcd8c8`, `801f1523`, `30d50f91`, `78673123`, `7111311c`.
- Touch of Death restored to Windwalker-only scope: `763865ee`, `97c9d26b`.
- Fortifying Brew ledger correction (`120954` aura/script, not proven learned cast): `2dac3110`.

## Build / runtime / upstream gate

- [x] Feature branch contains current upstream/fork `master` without force-push/history rewrite.
- [ ] Pre-activation build with `-DPLAYERBOTS=1` passes.
- [ ] Enable Monk `AiObjectContext` construction in `AiFactory.cpp`.
- [ ] Post-activation build with `-DPLAYERBOTS=1` passes.
- [ ] In-game Monk creation/login and party join/follow.
- [ ] In-game Brewmaster validation.
- [ ] In-game Mistweaver validation.
- [ ] In-game Windwalker validation.
- [ ] Final upstream Monk AI overlap search.
- [ ] Create/update upstream Monk AI implementation issue only after build gate passes.
- [ ] Respect maintainer response / 24-hour coordination gate.
- [ ] Open upstream implementation PR only after implementation, build/runtime verification, and coordination gate are satisfied.

## Exact blocker

The current blocker is **build/runtime execution availability**, not synchronization and not a known source compiler error. The feature branch now contains current `master` and is 0 commits behind, but the local runtime still cannot resolve/reach `github.com`, so the full checkout/build environment cannot be refreshed or compiled here. The fork build-validation PR #2 still has no Actions run/status even though its isolated workflow change enables `-DPLAYERBOTS=1`. The default GCC workflow cannot be substituted because it excludes `mod_playerbots` when the flag is absent. No absent workflow is being reported as a PASS or FAIL.

## External coordination

- No dedicated overlapping upstream Monk PlayerBot combat-AI issue/PR found this run.
- Fork PR #1 is mergeability/diff validation only.
- Fork PR #2 and branch `ci/monk-ai-playerbots` are PlayerBot-build validation artifacts only; they are not upstream contribution artifacts and must not be merged.
- Fork PR #3 was synchronization-only and is merged; it is not an upstream contribution artifact.
- No upstream implementation issue/PR will be opened before the build gate.

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and upstream Monk AI Issue/PR overlap.
2. Re-check fork PR #2 / `ci/monk-ai-playerbots` for an Actions run/status; if a real `PLAYERBOTS=1` run appears, inspect every compile/link failure and fix Monk failures immediately.
3. Retry local Git network once. If restored, fetch the already-synchronized feature tree and run the real pre-activation build with `-DPLAYERBOTS=1`.
4. After the pre-activation build passes, enable Monk `AiObjectContext` construction in `AiFactory.cpp` and rebuild with `PLAYERBOTS=1`.
5. After both builds pass, execute the Brewmaster/Mistweaver/Windwalker runtime matrix.
6. Re-run the upstream Monk AI overlap search, create/update the appropriate upstream issue, respect the recorded maintainer-response/24-hour coordination gate, and only then open the upstream implementation PR.