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
- [x] Feature branch was cleanly based on that master and required no rebase before implementation.
- [x] Upstream Issues and open Pull Requests were searched for `monk`, `Monk AI`, and `MonkAiObjectContext`.
- [x] No dedicated overlapping Monk PlayerBot combat-AI implementation was found in the current run.
- [x] Upstream issue #413 mentions `.npcbot addclass monk`, but is a bot-command management issue rather than a Monk combat-AI implementation.
- [x] Upstream issue #150 concerns Tushui Monk NPC scripting and does not overlap PlayerBot Monk combat AI.

### Repository facts verified

- [x] `modules/mod_playerbots/src/Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling.
- [x] `AiFactory::createAiObjectContext` still has the `CLASS_MONK` / `MonkAiObjectContext` construction disabled by comments; this remains intentionally unmodified until the new context receives static/build validation.
- [x] `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp` already maps Brewmaster, Windwalker, and Mistweaver specializations and identifies Brewmaster as tank / Mistweaver as healer.
- [x] `modules/CMakeLists.txt` recursively globs module `.cpp`/`.h` files, so the new `Classes/monk/` files do not require a manual per-file CMake source-list edit.
- [x] `Factory/RandomPlayerbotFactory.cpp` already contains Monk/Pandaren race-pool handling; no concrete defect requiring a change was found, so it remains untouched.
- [x] Existing Warrior/Shaman/Druid/Paladin implementations were audited for AiObjectContext, strategy, action, trigger, role, cure, healing, AoE, and tank patterns.
- [x] `Strategy.h` confirms the action priorities used by this implementation, including `ACTION_INTERRUPT`, `ACTION_DISPEL`, and `ACTION_EMERGENCY`.
- [x] `GenericSpellActions.h`, `GenericTriggers.h`, and `CureTriggers.h` signatures were checked against the initial Monk action/trigger wrappers.
- [x] Shaman's `melee aoe` pattern was checked and used to preserve distinct strategy identity for Windwalker AoE rather than aliasing it to an object named only `aoe`.

### Implementation checklist

- [x] Audit exact strategy/action/trigger factory patterns needed for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` skeleton and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat strategy baseline.
- [x] Implement generic Monk non-combat/cure strategy baseline.
- [x] Implement initial Monk action set and party cure/heal action wrappers.
- [x] Implement initial Monk trigger set and cure/interrupt triggers.
- [ ] Complete Brewmaster (tank) strategy — initial baseline is committed; MoP 5.4.8 spell/resource/targeting validation and build/runtime refinement remain.
- [ ] Complete Mistweaver (healer) strategy — initial baseline is committed; channel/heal target/resource behavior and build/runtime refinement remain.
- [ ] Complete Windwalker (DPS) strategy — initial baseline is committed; Chi/Energy priority/cooldown validation and build/runtime refinement remain.
- [x] Verify existing Monk specialization/role/name pipeline; no modification currently justified.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` creation path in `Factory/AiFactory.cpp` after the new source set passes the next static/build gate.
- [x] Verify `Factory/RandomPlayerbotFactory.cpp` requires no change for this task based on current evidence.
- [ ] Validate every used spell name/ID/effect/range/resource/aura/cooldown against MoP 5.4.8 build 18414 — target-core audit started in `MONK_SPELL_AUDIT.md`; several active IDs and internal effects are confirmed, but unresolved cast/resource semantics remain.
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

## Implementation commits

- `033af43b312a4d5cf4e38b952ef02b0556138dc6` — `feat(playerbots): add initial Monk AI strategy framework`
  - Adds 16 files under `modules/mod_playerbots/src/strategy/Classes/monk/`.
  - Adds `MonkAiObjectContext`, generic combat/non-combat strategies, action/trigger factories, and initial Brewmaster/Mistweaver/Windwalker strategy baselines.
  - Does **not** enable the `AiFactory.cpp` `CLASS_MONK` creation path yet.
- `3ed310cf5a0524a15c5a8ca8e18f101815b8d99a` — `docs(playerbots): add Monk spell validation audit`
  - Adds `MONK_SPELL_AUDIT.md` using the target core's `src/server/scripts/Spells/spell_monk.cpp` as the primary evidence source.
  - Confirms active target-core IDs including Blackout Kick `100784`, Fortifying Brew `120954`, Provoke `118635`, Soothing Mist `115175`, Elusive Brew `115308`, Spinning Crane Kick `101546`, Renewing Mist `115151`, Mana Tea regeneration `115294`, Uplift `116670`, and Touch of Death `115080`.
  - Separates secondary/internal aura/effect IDs from still-unresolved active cast IDs so donor/modern IDs are not accidentally treated as verified 5.4.8 data.
- `70064249f9aacb922013495ae7e5857b82314e84` — `fix(playerbots): preserve Monk melee aoe strategy identity`
  - Adds a distinct `MeleeAoeMonkStrategy` whose `getName()` is `melee aoe` and maps the corresponding factory key to it, matching the repository's Shaman-style strategy identity pattern.

## Bugs / Issues discovered

### Fixed: `melee aoe` factory alias returned a strategy named only `aoe`

- **Symptom:** the initial Monk context mapped both `aoe` and `melee aoe` to `MonkAoeStrategy`, whose identity is `aoe`.
- **Scope / affected paths:** `GenericMonkStrategy.h`, `MonkAiObjectContext.cpp`.
- **Cause:** confirmed static architecture mismatch with repository-local strategy naming patterns.
- **Resolution:** fixed by introducing `MeleeAoeMonkStrategy` with its own `melee aoe` identity and factory creator.
- **Fix commit:** `70064249f9aacb922013495ae7e5857b82314e84`.
- **Verification:** compared against Shaman's separate `MeleeAoeShamanStrategy` / `melee aoe` factory pattern; compiler/runtime verification still pending.

### Build execution environment unavailable in this run

- **Symptom:** local execution environment could not resolve `github.com` while attempting to clone/fetch the repository for a real `PLAYERBOTS=1` compile.
- **Scope / affected paths:** verification environment only; not identified as a repository/source defect.
- **Cause:** transient execution-environment DNS/network limitation.
- **Resolution:** pending retry in a later run/environment.
- **Fix commit:** N/A.
- **Verification:** repository-side static architecture audit continued; actual compiler/linker verification is still required.
- **CI note:** the fork currently reports no Actions runs for `feature/monk-ai-object-context` despite a push-triggered GCC workflow being present, so GitHub CI did not provide a substitute compile in this run.

### Runtime semantics still intentionally unverified

- Brewmaster `provoke` currently uses the generic cast action and may need repository-native aggro-target selection refinement after build/runtime testing.
- Mistweaver healing is a safe initial Strategy/Action baseline, not yet a verified channel/Chi/Mana Tea implementation.
- Windwalker and Brewmaster priority lists do not yet claim complete Chi/Energy optimization.
- These are tracked as unfinished validation/refinement work, not declared completed behavior.

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
- Target-core Monk spell implementation: `src/server/scripts/Spells/spell_monk.cpp`
- Spell verification ledger: `MONK_SPELL_AUDIT.md`
- External behavioral reference only: DigiD702/mod-playerbots Monk rotation/healer logic. Do not copy implementation verbatim; translate only verified behavior into this repository's Strategy / Trigger / Action architecture.

## Next action

Retry a real `PLAYERBOTS=1` build as soon as an execution/CI environment can run it, while continuing target-core spell/semantic validation from `MONK_SPELL_AUDIT.md`. Fix compiler/linker issues first. After the new Monk source set passes that gate, enable the `MonkAiObjectContext` include and `CLASS_MONK` case in `Factory/AiFactory.cpp` in a separate commit. Then refine Brewmaster Stagger/Shuffle/Guard/taunt behavior, Mistweaver channel/Chi/Mana Tea/heal targeting, and Windwalker Energy/Chi/cooldown priorities before any upstream issue/PR is opened.
