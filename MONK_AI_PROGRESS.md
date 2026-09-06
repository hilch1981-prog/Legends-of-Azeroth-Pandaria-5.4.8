# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-06)

### Synchronization / overlap check

- [x] Upstream and fork `master` checked at the start of the current implementation run.
- [x] Upstream `master`: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (`Modernize TreatManager (#423)`).
- [x] Fork `master`: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (identical to upstream for this run).
- [x] Feature branch required no rebase before this run; latest code/audit head before this handoff update was `abfcbbff0da64d51e709cfef564eeeb62074d0ef`.
- [x] Upstream Issues and Pull Requests were searched for `monk`, `Monk AI`, and `MonkAiObjectContext`.
- [x] No dedicated overlapping Monk PlayerBot combat-AI implementation was found in the current run.
- [x] Upstream issue #413 mentions `.npcbot addclass monk`, but is a bot-command management issue rather than a Monk combat-AI implementation.
- [x] Upstream issue #150 concerns Tushui Monk NPC scripting and does not overlap PlayerBot Monk combat AI.

### Repository facts verified

- [x] `modules/mod_playerbots/src/Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling.
- [x] `AiFactory::createAiObjectContext` still has the `CLASS_MONK` / `MonkAiObjectContext` construction disabled by comments; this remains intentionally unmodified until the new context receives a real build gate.
- [x] `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp` already maps Brewmaster, Windwalker, and Mistweaver specializations and identifies Brewmaster as tank / Mistweaver as healer.
- [x] `modules/CMakeLists.txt` recursively globs module `.cpp`/`.h` files, so the new `Classes/monk/` files do not require a manual per-file CMake source-list edit.
- [x] `Factory/RandomPlayerbotFactory.cpp` already contains Monk/Pandaren race-pool handling; no concrete defect requiring a change was found, so it remains untouched.
- [x] Existing Warrior/Shaman/Druid/Paladin implementations were audited for AiObjectContext, strategy, action, trigger, role, cure, healing, AoE, and tank patterns.
- [x] Monk AiObjectContext factory attachment was rechecked against Druid's strategy/action/trigger context pattern and is structurally consistent.
- [x] `Strategy.h` confirms the action priorities used by this implementation, including `ACTION_INTERRUPT`, `ACTION_DISPEL`, and `ACTION_EMERGENCY`.
- [x] `GenericSpellActions.h`, `GenericTriggers.h`, and `CureTriggers.h` signatures were checked against the Monk action/trigger wrappers.
- [x] `GenericTriggers` provides `HasAuraStackTrigger`, but its implementation calls `PlayerbotAI::GetAura(..., checkDuration=true, checkStack)`. The Monk Tigereye Brew logic therefore does not depend on it until build-18414 DBC duration semantics are known.
- [x] `PlayerbotAI::GetAura` name lookup and stack filtering behavior were audited directly; exact-ID lookup is safer for the confirmed Tigereye Brew stack aura `125195`.
- [x] Shaman's `melee aoe` pattern was checked and used to preserve distinct strategy identity for Windwalker AoE rather than aliasing it to an object named only `aoe`.

### Implementation checklist

- [x] Audit exact strategy/action/trigger factory patterns needed for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` skeleton and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat strategy baseline.
- [x] Implement generic Monk non-combat/cure strategy baseline.
- [x] Implement initial Monk action set and party cure/heal action wrappers.
- [x] Implement initial Monk trigger set and cure/interrupt triggers.
- [ ] Complete Brewmaster (tank) strategy — Stagger severity purification and Shuffle maintenance are now represented; Guard variant resolution, Elusive Brew stack policy, Provoke target selection, and build/runtime validation remain.
- [ ] Complete Mistweaver (healer) strategy — initial baseline is committed; channel-aware Soothing/Surging/Enveloping behavior, Renewing Mist/Uplift targeting, Mana Tea stack policy, and build/runtime refinement remain.
- [ ] Complete Windwalker (DPS) strategy — active Tigereye Brew action and exact `125195` 10-stack consumption trigger are now implemented; Energy/Chi priority, channel/movement safety for Fists of Fury, and build/runtime refinement remain.
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
- `3ed310cf5a0524a15c5a8ca8e18f101815b8d99a` — `docs(playerbots): add Monk spell validation audit`
  - Starts the target-core spell evidence ledger. Later audit commits correct ambiguous helper-vs-active IDs; `MONK_SPELL_AUDIT.md` is the authoritative current mapping.
- `70064249f9aacb922013495ae7e5857b82314e84` — `fix(playerbots): preserve Monk melee aoe strategy identity`
  - Adds a distinct `MeleeAoeMonkStrategy` matching repository-local strategy identity conventions.
- `9c205f8f34fb164d78da067e75d0037ee90f088a` — `feat(playerbots): add Brewmaster stagger management`
  - Adds Shuffle maintenance and moderate/heavy Stagger purification behavior.
- `54079cc5406d053091f7717109560d3490ff64e8` — `fix(playerbots): keep Shuffle aura lookup key intact`
  - Corrects the semantic factory key vs aura-name lookup distinction for `no shuffle`.
- `9da0a566bbb295f9e36b5d5ae11b0fb4cd79ace5` — `feat(playerbots): register Tigereye Brew action`
  - Adds a Windwalker Tigereye Brew buff action and local action-factory registration.
- `83222eef1d1847b98acaa7c7ff81cd5c8688586d` — `feat(playerbots): consume Tigereye Brew at ten stacks`
  - Adds the Windwalker 10-stack decision and action priority based on target-core consumption behavior.
- `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0` — `fix(playerbots): make Tigereye Brew stack check duration-safe`
  - Replaces the generic duration-filtering stack helper with an exact target-core stack aura ID `125195` check.
- `fad91d80065b02838036248cabc6e325ebedad90` and `abfcbbff0da64d51e709cfef564eeeb62074d0ef` — spell-audit updates
  - Confirm Revival `115310`, Tigereye Brew active `116740`, stacks `125195`, Guard's dual player-ability IDs `123402/115295`, and record the duration-safe PlayerBot stack strategy.

## Bugs / Issues discovered

### Fixed: `melee aoe` factory alias returned a strategy named only `aoe`

- **Symptom:** the initial Monk context mapped both `aoe` and `melee aoe` to `MonkAoeStrategy`, whose identity is `aoe`.
- **Scope / affected paths:** `GenericMonkStrategy.h`, `MonkAiObjectContext.cpp`.
- **Cause:** confirmed static architecture mismatch with repository-local strategy naming patterns.
- **Resolution:** fixed by introducing `MeleeAoeMonkStrategy` with its own `melee aoe` identity and factory creator.
- **Fix commit:** `70064249f9aacb922013495ae7e5857b82314e84`.
- **Verification:** compared against Shaman's separate `MeleeAoeShamanStrategy` / `melee aoe` factory pattern; compiler/runtime verification still pending.

### Fixed: Shuffle semantic trigger key could replace the aura lookup key

- **Symptom:** a semantic `no shuffle` trigger name would make `HasNoAuraTrigger::IsActive()` look for an aura literally named `no shuffle`.
- **Cause:** generic trigger implementation uses its internal `getName()` as the aura lookup key.
- **Resolution:** factory key remains `no shuffle`, internal `HasNoAuraTrigger` name remains `shuffle`.
- **Fix commit:** `54079cc5406d053091f7717109560d3490ff64e8`.

### Fixed: generic aura-stack helper imposed an unverified duration requirement on Tigereye Brew stacks

- **Symptom:** `HasAuraStackTrigger::IsActive()` calls `GetAura` with `checkDuration=true`; this could reject a valid 10-stack aura if build-18414 DBC represents the stack aura with permanent duration.
- **Scope / affected paths:** `MonkTriggers.h/.cpp`; Windwalker Tigereye Brew trigger.
- **Cause:** confirmed repository helper behavior; target DBC duration for `125195` is still unverified.
- **Resolution:** check exact target-core stack aura ID `125195` directly and require `GetStackAmount() >= 10`, avoiding the unverified duration filter.
- **Fix commit:** `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0`.
- **Verification:** static source path verified against target-core Tigereye Brew implementation; compile/runtime verification pending.

### Build execution environment unavailable in this run

- **Symptom:** the local execution environment could not resolve `github.com` while attempting to clone/fetch the repository for a real `PLAYERBOTS=1` compile.
- **Scope / affected paths:** verification environment only; not identified as a repository/source defect.
- **Cause:** transient execution-environment DNS/network limitation.
- **Resolution:** pending retry in a later run/environment.
- **Fix commit:** N/A.
- **Verification:** repository-side static architecture/spell audit continued; actual compiler/linker verification is still required.
- **CI note:** the fork still reports no Actions runs for `feature/monk-ai-object-context` even though `.github/workflows/linux_gcc.yml` is push-triggered and configures the default PlayerBots-enabled build. GitHub CI therefore did not provide a substitute compile in this run.

### Runtime semantics still intentionally unverified

- Brewmaster `provoke` currently uses the generic current-target cast action and may need repository-native aggro-target selection refinement.
- Guard has two target-core player-ability IDs (`123402`, `115295`); the correct PlayerBot spell-name resolution must be proven in DBC/runtime rather than guessed.
- Mistweaver healing is still a Strategy/Action baseline rather than a verified Soothing Mist channel-aware implementation.
- Windwalker's 10-stack Tigereye Brew decision is statically implemented, but Energy/Chi starvation behavior and Fists of Fury channel safety remain runtime work.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI implementation issue or open PR was found during the current 2026-09-06 run.
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

Retry a real `PLAYERBOTS=1` build as soon as an execution/CI environment can run it; compiler/linker failures take priority over further expansion. In parallel, continue static target-core refinement without enabling the global `AiFactory` Monk case prematurely: (1) make Mistweaver healing channel-aware and define a target-core-backed Mana Tea stack policy, (2) resolve Brewmaster Guard spell-name/variant selection and Provoke aggro-target semantics, and (3) audit Windwalker Energy/Chi and Fists of Fury channel/movement safety. After the new Monk source set passes the real build gate, enable the `MonkAiObjectContext` include and `CLASS_MONK` case in `Factory/AiFactory.cpp` in a separate commit, then proceed to runtime tests before opening any upstream contribution issue/PR.
