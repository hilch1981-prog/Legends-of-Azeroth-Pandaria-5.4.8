# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Current state — 2026-09-07

### Synchronization / overlap

- [x] Upstream, fork `master`, and feature branch checked at the start of this run.
- [x] Upstream `master`: `06fb98107158ee5a513e673a42674693fb4db7a2` (`Mordenize Combat System part 1 (#426)`).
- [x] Fork `master`: same `06fb98107158ee5a513e673a42674693fb4db7a2`.
- [ ] Feature branch is currently **94 commits ahead / 2 behind** fork `master`; merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`. Rebase remains pending because local Git cannot currently resolve `github.com`.
- [x] Upstream #426 audited: it changes generic combat/threat APIs, but Monk Provoke consumes repository-native `tank target` and does not directly call the renamed threat getter.
- [x] Fresh upstream Issue/PR searches for Monk PlayerBot AI found no materially overlapping implementation work.
- [x] Historical Monk PRs #363, #168, #268 and issues #413/#150 remain unrelated to this combat-AI implementation.
- [x] No force update or invented conflict resolution used.

### Validation environment

- [ ] Local Git/rebase and local compile remain blocked by transient DNS: `Could not resolve host: github.com`.
- [x] GitHub connector reads/writes are healthy.
- [x] Fork PR #1 remains validation-only and is **not** the upstream contribution PR. After its metadata was refreshed this run, GitHub reports `mergeable: true` for current feature head `266bc6bd96d05d2e71258c3c5d1df4b3fb375866`.
- [ ] Feature branch still has zero GitHub Actions runs.
- [x] Repository default GCC CI is not a valid PlayerBot build gate: it omits `-DPLAYERBOTS=1`, and `modules/CMakeLists.txt` removes `mod_playerbots` when `PLAYERBOTS` is false.
- [x] Created fork-only validation branch `ci/monk-ai-playerbots` from feature source. Validation commit `0c51fa9c3ecb7e3ae97f419296217bd208175096` changes only `.github/workflows/linux_gcc.yml` to add `-DPLAYERBOTS=1`; this change is not on the feature branch and does not contaminate the upstream contribution diff.
- [ ] `ci/monk-ai-playerbots` currently has zero workflow runs/status contexts. This is infrastructure availability, not a PASS or FAIL build result.

## Repository facts verified

- [x] `Factory/AiFactory.cpp` already has Monk spec/role/default-strategy handling; only `MonkAiObjectContext` construction remains commented until the real build gate passes.
- [x] `RandomPlayerbotFactory.cpp` has no demonstrated Monk defect and remains untouched.
- [x] `modules/CMakeLists.txt` recursively gathers module `.cpp/.h` files, so new Monk files need no per-file CMake registration.
- [x] `Player::HasActiveSpell(uint32)` exists and is used only in Monk-local override resolution.
- [x] Generic `SpellIdValue` can resolve the numerically lower unranked same-name spell; Guard and Mana Tea now avoid that ambiguity locally.
- [x] Generic `PlayerbotAI::CanCastSpell` ignores resource cost during preflight; Monk-local resource-sensitive actions use target-core `SpellInfo::GetPowerType` + `CalcPowerCost` before the normal real-cast path.
- [x] `PlayerbotAI::GetAura(std::string, ...)` is name-based; buff-style usefulness therefore should not control stack-consuming Mana Tea availability.
- [x] Generic channel checks prevent routine casts/reach-melee from replacing a running Fists of Fury channel; runtime is still required for safety/formation movement.
- [x] Healing/group/tank target values used by the Monk implementation are repository-native primitives.

## Implementation checklist

### Common / architecture

- [x] `modules/mod_playerbots/src/strategy/Classes/monk/` created.
- [x] `MonkAiObjectContext` strategy/action/trigger factories implemented.
- [x] Generic Monk combat strategy implemented.
- [x] Generic Monk non-combat strategy implemented.
- [x] Generic poison/disease Detox cure strategy implemented.
- [x] Monk action wrappers and DBC-derived power preflight implemented.
- [x] Monk trigger/factory keys statically cross-checked.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` construction in `Factory/AiFactory.cpp` only after a real `PLAYERBOTS=1` pre-activation build passes.

### Brewmaster

- [x] Sturdy Ox stance.
- [x] Keg Smash / Tiger Palm / Blackout Kick / Jab baseline.
- [x] Shuffle maintenance.
- [x] Moderate/Heavy Stagger -> Purifying Brew.
- [x] Elusive Brew stack gate.
- [x] Provoke uses repository-native `tank target` and avoids redundant taunt.
- [x] Guard explicitly resolves active `123402` before `115295`, then uses the exact ID for legality/execution and treats either aura as active.
- [x] Fortifying Brew remains name-resolved because target-core `120954` is proven as an aura/script ID, not yet as the learned player cast.
- [ ] Runtime/DBC prove Fortifying Brew learned cast ID.
- [ ] Runtime Guard glyph/no-glyph behavior.
- [ ] Runtime tanking/rotation validation.

### Mistweaver

- [x] Wise Serpent stance in combat and specialization-gated non-combat recovery.
- [x] Soothing Mist baseline.
- [x] Surging/Enveloping Mist narrow legal-cast exception during Soothing Mist channel, preserving the channel target.
- [x] Renewing Mist spreads to a valid group member without this Monk's `119611` HoT.
- [x] Uplift requires useful caster-owned Renewing Mist coverage.
- [x] Mana Tea explicit variant resolution and correct stack gates: prefer active glyphed `123761`, otherwise `115294`; normal channel may use one stack, glyphed variant requires two.
- [x] Life Cocoon / Revival baseline.
- [x] Heal-target range recovery.
- [x] Magic Detox gated by Internal Medicine `115451`; poison/disease Detox remains generic Monk utility.
- [x] Healthy-party melee fallback is Jab; Tiger Palm is requested from exact Muscle Memory proc aura `139597` instead of being an unconditional Chi spender.
- [ ] Runtime combat/non-combat healing, Renewing Mist/Uplift, Mana Tea glyph/no-glyph, Muscle Memory cadence, and magic dispel validation.

### Windwalker

- [x] Fierce Tiger stance.
- [x] Rising Sun Kick / Blackout Kick / Jab baseline.
- [x] Tiger Palm removed from unconditional default filler; requested for missing Tiger Power `125359` or Combo Breaker: Tiger Palm `118864`.
- [x] Combo Breaker free Tiger Palm `118864` / Blackout Kick `116768` bypass only conservative Monk power preflight.
- [x] Fists of Fury baseline with DBC power gate.
- [x] Tigereye Brew uses exact stack aura `125195` and requests active `116740` at 10 stacks.
- [x] Touch of Karma defensive path.
- [x] Touch of Death kept Windwalker-only.
- [x] Static Fists of Fury channel-retention audit complete; no blanket channel lock added.
- [ ] Runtime Energy/Chi cadence, Tiger Power/Combo Breaker, Tigereye Brew, and Fists of Fury movement/channel validation.

## Target-core spell facts added/confirmed this run

- [x] Normal Mana Tea `115294` is an AuraScript/channel that removes one `115867` stack on aura apply and another per periodic tick.
- [x] Glyphed Mana Tea `123761` has an explicit target-core `CheckCast` requiring at least two `115867` stacks.
- [x] These two active Mana Tea variants are now recorded in `MONK_SPELL_AUDIT.md`; runtime still must prove how this fork exposes the glyph override in the live spellbook.

## Fixed issues ledger

| Status | Issue | Fix / commits |
| --- | --- | --- |
| fixed this run | Mana Tea normal channel was incorrectly gated at 2 stacks, and same-name glyphed `123761` could be ambiguously resolved | Monk-local variant resolver + exact-ID execute/preflight + 1-stack normal / 2-stack glyph thresholds: `15fa4919b7a995d0e164fb89841a8dbcabe7ff9c`; audit `467bfbd6092aea89d0b41340005f316019f37d9b` |
| fixed | Mistweaver unconditional Tiger Palm could starve healing Chi | Jab fallback + Muscle Memory `139597` trigger: `85fc7164`, `52f38ba9`, `5a44a35a`, `40b85307`; audit `6693f5d7` |
| fixed | Combo Breaker free procs could fail Monk power preflight | exact proc bypass + Blackout Kick proc trigger: `606bf515`, `ea0c88d2`, `a7e93cd4`, `082910bb`, `954fd28c` |
| fixed | Windwalker Tiger Palm default filler could starve stronger Chi spenders | maintenance/proc-only Tiger Palm: `a830ff93`, `9d7c39f7`, `c2413821`, `e2490be8` |
| corrected | Fortifying Brew `120954` over-classified as learned cast | ledger correction, keep action name-resolved: `2dac3110` |
| fixed | Mistweaver non-combat healer/magic-Detox gap | specialization-gated non-combat support: `766a77e6` |
| fixed statically | Guard same-name spellbook override ambiguity | explicit `123402` -> `115295` resolver: `f8b3f222`, `4db14f55` |
| fixed | Touch of Death leaked into generic Monk strategy | restored Windwalker-only scope: `763865ee`, `97c9d26b` |
| fixed | Mistweaver magic Detox missing | Internal Medicine-aware triggers/actions/factory wiring: `d1b9e846`, `84bcd8c8`, `801f1523`, `30d50f91`, `78673123`, `7111311c` |
| fixed | Generic PlayerBot preflight could ignore Monk Mana/Energy/Chi | Monk-local DBC-backed power preflight; global behavior unchanged |
| fixed | Generic channel guard blocked legal Surging/Enveloping during Soothing Mist | narrow Monk-only follow-up-heal path; final legality stays with core cast checks |
| narrowed | Fists of Fury interruption concern | routine casts/reach-melee statically shown not to replace channel; runtime global movement remains pending |

## Build / runtime / upstream gate

- [ ] Rebase feature branch onto current upstream/fork `master` when Git network access returns.
- [ ] Pre-activation `PLAYERBOTS=1` build passes.
- [ ] Enable Monk `AiObjectContext` construction in `AiFactory.cpp`.
- [ ] Post-activation `PLAYERBOTS=1` rebuild passes.
- [ ] In-game Monk creation/login, party join/follow.
- [ ] In-game Brewmaster validation.
- [ ] In-game Mistweaver validation.
- [ ] In-game Windwalker validation.
- [ ] Final upstream Monk AI overlap search.
- [ ] Create/update upstream Monk AI implementation issue only after build gate passes.
- [ ] Respect maintainer response / 24-hour coordination gate.
- [ ] Open upstream implementation PR only after implementation + build/runtime/coordination requirements are satisfied.

## Environment blocker

The exact current blocker is **build execution availability**, not a known source compiler error. Local Git/DNS cannot reach GitHub, and fork Actions has not produced a run even on the isolated `ci/monk-ai-playerbots` branch whose workflow explicitly sets `-DPLAYERBOTS=1`. No green/default workflow is being misreported as a PlayerBot build.

## External coordination

- No dedicated overlapping upstream Monk PlayerBot AI issue/PR found this run.
- Fork PR #1 and branch `ci/monk-ai-playerbots` are validation-only artifacts and are not upstream contribution artifacts.
- No upstream implementation issue/PR will be opened before the build gate.

## Next deterministic action

1. Re-check upstream/fork/feature SHAs and upstream Monk AI issue/PR overlap.
2. Re-check `ci/monk-ai-playerbots` commit `0c51fa9c...` for an Actions run/status; if a real `PLAYERBOTS=1` run exists, inspect all compile/link output and fix Monk failures immediately.
3. Retry local Git network once. If restored, rebase cleanly onto current `master`; abort and record exact paths for any ambiguous conflict.
4. Run the real pre-activation build with `-DPLAYERBOTS=1`.
5. After that build passes, enable Monk `AiObjectContext` construction in `AiFactory.cpp` and rebuild with `PLAYERBOTS=1`.
6. Only after both builds pass, perform the three-spec runtime matrix, then enter the upstream issue/24-hour/PR coordination gate.