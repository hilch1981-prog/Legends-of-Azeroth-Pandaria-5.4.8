# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-06)

### Synchronization / overlap check

- [x] Fork and upstream `master` checked before starting this workstream.
- [x] Upstream `master` SHA at bootstrap: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (`Modernize TreatManager (#423)`).
- [x] Fork `master` SHA at bootstrap: `3ec151e16c7912b217838040ac1bb30c6f1fc84d` (identical to upstream at bootstrap).
- [x] Dedicated upstream search performed for `monk`, `Monk AI`, and `MonkAiObjectContext`.
- [x] No dedicated Monk PlayerBot AI implementation issue/PR was found at bootstrap.
- [x] Upstream issue #413 mentions `.npcbot addclass monk`, but it concerns bot command management and is not treated as an overlapping Monk combat-AI implementation.

### Repository facts verified

- [x] `modules/mod_playerbots/src/Factory/AiFactory.cpp` contains Monk role/spec-name handling.
- [x] `AiFactory::createAiObjectContext` currently has the `CLASS_MONK` / `MonkAiObjectContext` construction disabled by comments.
- [x] Existing class strategy directories exist under `modules/mod_playerbots/src/strategy/Classes/` and provide the structural pattern for the new Monk directory.
- [x] Root `AGENTS.md` now contains the persistent Monk AI workflow, conflict-handling rules, build/test gate, and upstream contribution gate.

### Implementation checklist

- [ ] Audit exact strategy/action/trigger factory patterns needed for a new class.
- [ ] Create `modules/mod_playerbots/src/strategy/Classes/monk/`.
- [ ] Implement `MonkAiObjectContext` skeleton and registrations.
- [ ] Implement generic Monk combat strategy.
- [ ] Implement generic Monk non-combat strategy.
- [ ] Implement Monk actions.
- [ ] Implement Monk triggers.
- [ ] Implement Brewmaster (tank) strategy.
- [ ] Implement Mistweaver (healer) strategy.
- [ ] Implement Windwalker (DPS) strategy.
- [ ] Verify existing Monk specialization/role/name pipeline; modify only if concrete evidence requires it.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` creation path in `Factory/AiFactory.cpp` after the context is present.
- [ ] Verify `Factory/RandomPlayerbotFactory.cpp` requires no changes; keep it unchanged unless a specific defect is proven.
- [ ] Validate all used spell IDs/effects/ranges/resources/auras/cooldowns for MoP 5.4.8 build 18414.
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

## Bugs / Issues discovered

### None recorded yet

For each new item, use:

- **Symptom:**
- **Scope / affected paths:**
- **Cause:** confirmed / suspected
- **Resolution:** fixed / pending / blocked
- **Fix commit:** `<sha>` if available
- **Verification:** build/test performed after fix

## External overlap / coordination

- No dedicated overlapping Monk combat-AI implementation issue or PR found during bootstrap search on 2026-09-06.
- Issue #413 is command-management related and does not currently block this workstream.

If overlap appears later, record the PR/issue number, overlapping paths/features, and the decision (continue non-overlap / integrate / stop for review) here before editing overlapping code.

## Reference sources

- Repository-local class architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`
- Multi-role/spec structural reference: `modules/mod_playerbots/src/strategy/Classes/druid/`
- Context integration: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- External behavioral reference only: DigiD702/mod-playerbots Monk rotation/healer logic. Do not copy implementation verbatim; translate verified behavior into this repository's Strategy / Trigger / Action architecture.

## Next action

Audit one existing single-role class plus `druid/` to identify the exact required AiObjectContext factories, strategy naming, action registration, trigger registration, and build inclusion pattern. Then create the smallest compilable `MonkAiObjectContext`/factory skeleton before implementing any rotation logic.
