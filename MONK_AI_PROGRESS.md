# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-06)

### Synchronization / overlap check

- [x] Upstream and fork `master` checked at the start of the current run.
- [x] Upstream `master`: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (`Modernize TreatManager (#423)`).
- [x] Fork `master`: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (identical to upstream for this run).
- [x] Feature branch is based on the current upstream/fork master and required no rebase in this run.
- [x] Feature branch was 17 commits ahead / 0 behind before this run's new commits.
- [x] Upstream Issues and Pull Requests were searched again for `monk`, `Monk AI`, and `MonkAiObjectContext`.
- [x] No dedicated overlapping Monk PlayerBot combat-AI implementation was found in this run.
- [x] Upstream issue #413 mentions `.npcbot addclass monk`, but is a bot-command management issue rather than a Monk combat-AI implementation.
- [x] Upstream issue #150 concerns Tushui Monk NPC scripting and does not overlap PlayerBot Monk combat AI.

### Repository facts verified

- [x] `modules/mod_playerbots/src/Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling.
- [x] `AiFactory::createAiObjectContext` still has the `CLASS_MONK` / `MonkAiObjectContext` construction disabled by comments; this remains intentionally unmodified until the new context receives a real build gate.
- [x] `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp` already maps Brewmaster, Windwalker, and Mistweaver specializations and identifies Brewmaster as tank / Mistweaver as healer.
- [x] `modules/CMakeLists.txt` recursively globs module `.cpp`/`.h` files, so the new `Classes/monk/` files do not require a manual per-file CMake source-list edit.
- [x] `Factory/RandomPlayerbotFactory.cpp` already contains Monk/Pandaren race-pool handling; no concrete defect requiring a change was found, so it remains untouched.
- [x] Existing Warrior/Shaman/Druid/Paladin implementations were audited for AiObjectContext, strategy, action, trigger, role, cure, healing, AoE, and tank patterns.
- [x] Warrior `CastTauntAction` usefulness logic was rechecked and used as the repository-local semantic reference for avoiding redundant Provoke casts.
- [x] `Strategy.h` confirms the action priorities used by this implementation, including `ACTION_INTERRUPT`, `ACTION_DISPEL`, and `ACTION_EMERGENCY`.
- [x] `GenericSpellActions.h`, `GenericTriggers.h`, and `CureTriggers.h` signatures were checked against the Monk action/trigger wrappers.
- [x] `GenericTriggers` provides `HasAuraStackTrigger`, but its implementation calls `PlayerbotAI::GetAura(..., checkDuration=true, checkStack)`. The Monk Tigereye Brew logic therefore does not depend on it until build-18414 DBC duration semantics are known.
- [x] Target-core `spell_monk.cpp` confirms Elusive Brew stacks `128939`, active Elusive Brew `115308`, Mana Tea stacks `115867`, and active Mana Tea regen `115294`.
- [x] Target-core glyphed Mana Tea (`123761`) explicitly requires at least two Mana Tea stacks before cast.
- [x] Shaman's `melee aoe` pattern was checked and used to preserve distinct strategy identity for Windwalker AoE rather than aliasing it to an object named only `aoe`.

### Implementation checklist

- [x] Audit exact strategy/action/trigger factory patterns needed for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` skeleton and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat strategy baseline.
- [x] Implement generic Monk non-combat/cure strategy baseline.
- [x] Implement initial Monk action set and party cure/heal action wrappers.
- [x] Implement initial Monk trigger set and cure/interrupt triggers.
- [ ] Complete Brewmaster (tank) strategy — Stagger severity purification, Shuffle maintenance, a conservative Elusive Brew stack gate, and redundant-Provoke suppression are represented; Guard variant resolution, stronger lost-aggro target selection, and build/runtime validation remain.
- [ ] Complete Mistweaver (healer) strategy — initial baseline and a target-core-backed Mana Tea stack guard are committed; channel-aware Soothing/Surging/Enveloping behavior, Renewing Mist/Uplift targeting, and build/runtime refinement remain.
- [ ] Complete Windwalker (DPS) strategy — active Tigereye Brew action and exact `125195` 10-stack consumption trigger are implemented; Energy/Chi priority, channel/movement safety for Fists of Fury, and build/runtime refinement remain.
- [x] Verify existing Monk specialization/role/name pipeline; no modification currently justified.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` creation path in `Factory/AiFactory.cpp` only after the new source set passes a real `PLAYERBOTS=1` build gate.
- [x] Verify `Factory/RandomPlayerbotFactory.cpp` requires no change for this task based on current evidence.
- [ ] Validate every used spell name/ID/effect/range/resource/aura/cooldown against MoP 5.4.8 build 18414 — target-core audit is substantially expanded in `MONK_SPELL_AUDIT.md`, but DBC/runtime semantics remain for several actions.
- [ ] Configure/build successfully with `PLAYERBOTS=1`.
- [ ] Resolve all Monk-related compiler/linker failures introduced by this branch.
- [ ] In-game: Monk bot can be created/logged in and join/follow a party.
- [ ] In-game: Brewmaster rotation and survival behavior verified.
- [ ] In-game: Mistweaver healing/targeting/resource behavior verified.
- [ ] In-game: Windwalker rotation/resource behavior verified.
- [ ] Final upstream Monk AI issue/PR overlap search performed immediately before contribution.
- [ ] Upstream contribution issue created or existing issue updated.
- [ ] Maintainer-response / 24-hour issue gate satisfied.
- [ ] Upstream implementation PR opened with scope, verification, and known limitations documented.

## Recent implementation / audit commits

- `033af43b312a4d5cf4e38b952ef02b0556138dc6` — `feat(playerbots): add initial Monk AI strategy framework`
  - Adds the initial Monk AiObjectContext, generic/non-combat strategies, action/trigger factories, and Brewmaster/Mistweaver/Windwalker baselines.
- `70064249f9aacb922013495ae7e5857b82314e84` — `fix(playerbots): preserve Monk melee aoe strategy identity`
  - Adds a distinct `MeleeAoeMonkStrategy` matching repository-local strategy identity conventions.
- `9c205f8f34fb164d78da067e75d0037ee90f088a` — `feat(playerbots): add Brewmaster stagger management`
  - Adds Shuffle maintenance and moderate/heavy Stagger purification behavior.
- `54079cc5406d053091f7717109560d3490ff64e8` — `fix(playerbots): keep Shuffle aura lookup key intact`
  - Corrects the semantic factory key vs aura-name lookup distinction for `no shuffle`.
- `9da0a566bbb295f9e36b5d5ae11b0fb4cd79ace5` / `83222eef1d1847b98acaa7c7ff81cd5c8688586d` / `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0` — Windwalker Tigereye Brew implementation and duration-safe stack check.
- `4b60c5d03c1795be073b12104830b7350ce13092` / `1d5e654f8d8c768073c785d4cb718ab57ed7ff45` — Brew action stack gating.
  - `CastElusiveBrewAction` now requires at least five confirmed `128939` stacks before it is useful, avoiding a low-health loop that repeatedly requests Elusive Brew with no accumulated duration.
  - `CastManaTeaAction` now requires at least two confirmed `115867` stacks. This is safe for the target-core glyphed Mana Tea path, whose cast check explicitly requires two stacks, while still preventing zero-stack requests on the normal low-mana trigger.
- `5b003d6e489661e184b20967ba7982eed3577633` / `adc15302fbefa8bfe163dd212060b7a93d5d558e` — Provoke usefulness refinement.
  - Keeps Provoke as a normal ranged `CastSpellAction`, but mirrors the repository's Warrior taunt usefulness condition by refusing to cast when the current target already targets the bot.
- `9f27e46002f776e5af72ce54787515474cc6c391` — header-light cleanup for the new stack checks.

## Bugs / Issues discovered

### Fixed: `melee aoe` factory alias returned a strategy named only `aoe`

- **Symptom:** the initial Monk context mapped both `aoe` and `melee aoe` to `MonkAoeStrategy`, whose identity is `aoe`.
- **Resolution:** fixed with distinct `MeleeAoeMonkStrategy` identity.
- **Fix commit:** `70064249f9aacb922013495ae7e5857b82314e84`.

### Fixed: Shuffle semantic trigger key could replace the aura lookup key

- **Symptom:** a semantic `no shuffle` trigger name would make `HasNoAuraTrigger::IsActive()` look for an aura literally named `no shuffle`.
- **Resolution:** factory key remains `no shuffle`, internal `HasNoAuraTrigger` name remains `shuffle`.
- **Fix commit:** `54079cc5406d053091f7717109560d3490ff64e8`.

### Fixed: generic aura-stack helper imposed an unverified duration requirement on Tigereye Brew stacks

- **Symptom:** `HasAuraStackTrigger::IsActive()` forces `checkDuration=true` and could reject a valid permanent/driver stack aura.
- **Resolution:** Windwalker checks exact target-core stack aura ID `125195` directly and requires `GetStackAmount() >= 10`.
- **Fix commit:** `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0`.

### Fixed: low-health brew requests could occur without usable stacks

- **Symptom:** Brewmaster low-health logic requested Elusive Brew even with no `128939` stacks, and Mistweaver low-mana logic requested Mana Tea without proving `115867` stacks.
- **Cause:** generic health/mana triggers knew nothing about Monk brew stack resources.
- **Resolution:** action-level usefulness checks now require Elusive Brew >=5 stacks and Mana Tea >=2 stacks. The latter is directly compatible with the target-core glyphed Mana Tea cast gate.
- **Fix commits:** `4b60c5d03c1795be073b12104830b7350ce13092`, `1d5e654f8d8c768073c785d4cb718ab57ed7ff45`.
- **Verification:** static source evidence complete; compiler/runtime verification pending.

### Fixed: Provoke could be requested when target already attacked the Brewmaster

- **Symptom:** the generic current-target Provoke action had no taunt usefulness filter of its own.
- **Resolution:** it now follows the existing Warrior taunt semantic check and declines when the current target already targets the bot.
- **Fix commits:** `5b003d6e489661e184b20967ba7982eed3577633`, `adc15302fbefa8bfe163dd212060b7a93d5d558e`.
- **Remaining:** selecting the best lost-aggro attacker instead of relying on `current target` still requires deeper runtime/value-context validation.

### Build execution environment unavailable in this run

- **Symptom:** `git ls-remote https://github.com/hilch1981-prog/Legends-of-Azeroth-Pandaria-5.4.8.git refs/heads/feature/monk-ai-object-context` failed with `Could not resolve host: github.com`.
- **Scope:** execution environment only; GitHub connector access and repository writes remained healthy.
- **Cause:** transient local DNS/network limitation.
- **Resolution:** retry on the next run; do not classify this as a source/build failure.
- **CI note:** GitHub Actions API still reports zero workflow runs for this feature branch even though `.github/workflows/linux_gcc.yml` is push-triggered. The fork therefore still has no CI build result to substitute for a local compile.

### Runtime semantics still intentionally unverified

- Guard has two target-core player-ability IDs (`123402`, `115295`); the correct PlayerBot spell-name resolution must be proven in DBC/runtime rather than guessed.
- Mistweaver healing remains a Strategy/Action baseline rather than a verified Soothing Mist channel-aware implementation.
- Provoke now avoids redundant taunts but still relies on `current target` rather than a proven lost-aggro attacker value.
- Windwalker's 10-stack Tigereye Brew decision is statically implemented, but Energy/Chi starvation behavior and Fists of Fury channel safety remain runtime work.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI implementation issue or open PR found during the current 2026-09-06 run.
- Issue #413 is command-management related and does not block this workstream.
- Issue #150 is NPC scripting related and does not block this workstream.

If overlap appears later, record the PR/issue number, overlapping paths/features, and the decision (continue non-overlap / integrate / stop for review) here before editing overlapping code.

## Reference sources

- Repository-local single-role architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`
- Multi-role/spec structural reference: `modules/mod_playerbots/src/strategy/Classes/druid/`
- Healing/cure structural reference: `modules/mod_playerbots/src/strategy/Classes/shaman/` and `paladin/`
- Context integration: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- Specialization helpers: `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp`
- PlayerBot aura lookup semantics: `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Generic stack-trigger semantics: `modules/mod_playerbots/src/strategy/triggers/GenericTriggers.cpp`
- Target-core Monk spell implementation: `src/server/scripts/Spells/spell_monk.cpp`
- Spell verification ledger: `MONK_SPELL_AUDIT.md`
- External behavioral reference only: DigiD702/mod-playerbots Monk rotation/healer logic. Do not copy implementation verbatim; translate only verified behavior into this repository's Strategy / Trigger / Action architecture.

## Next action

Retry a real `PLAYERBOTS=1` build first; compiler/linker failures take priority over feature expansion. If the execution environment is still unable to reach GitHub, continue only changes backed by target-core/repository-local evidence: (1) make Mistweaver healing channel-aware without breaking generic party target selection, (2) resolve Guard's learned-spell variant through DBC/runtime evidence instead of hardcoding an ID, and (3) audit Windwalker Fists of Fury movement/channel behavior plus Energy/Chi gating. After the new Monk source set passes a real build gate, enable the `MonkAiObjectContext` include and `CLASS_MONK` case in `Factory/AiFactory.cpp` in a separate commit, then proceed to runtime tests before opening any upstream contribution issue/PR.
