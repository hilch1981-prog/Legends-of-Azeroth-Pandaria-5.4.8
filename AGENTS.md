# Legends-of-Azeroth Pandaria 5.4.8 — Agent Instructions

## Build

```
mkdir build && cd build
cmake .. -D<OPTION>=<VAL>
make -j$(nproc)
```

Default build type is `Release`. CMake disallows in-source builds.

**Key CMake options** (`cmake/options.cmake`):
- `SCRIPTS=1` — build core with custom scripts (default ON)
- `TOOLS=0` — map/vmap/mmap extractors (default OFF; enable for map tooling)
- `ELUNA=1` — Lua engine (default ON)
- `PLAYERBOTS=1` — player bot AI (default ON; see README quirks below)
- `COREPCH/SCRIPTPCH=1` — precompiled headers (default ON; set to 0 for faster incremental rebuilds)
- `BUILD_DEPLOY=1` — Unix only (default ON)

Debug builds: `cmake .. -DCMAKE_BUILD_TYPE=Debug`

## Requirements

- **Compiler**: GCC ≥ 13 or Clang ≥ 12 (Windows: MSVC ≥ 2019 v16.4)
- **CMake** ≥ 3.16 (README says 3.27.2)
- **MySQL** 5.7 or 8.0-8.1 (note: OpenSSL 3.2.0 not supported with MySQL ≥ 8.0.33)
- **Boost** ≥ 1.85 (Linux), ≥ 1.81 MSVC
- **OpenSSL** 1.1.1 or 3.0–3.1.1
- **C++20** (enforced via `CMAKE_CXX_STANDARD`)

## Architecture

This is a **WoW private server** (Mists of Pandaria 5.4.8 client patch).

- `src/server/shared/` — shared library (packets, networking, data stores, threading)
- `src/server/authserver/` — authentication server
- `src/server/worldserver/` — game world server
- `src/server/game/` — core game logic (entities, spells, quests, combat, maps, AI, etc.)
- `src/server/scripts/` — custom C++ game scripts, organized by region/expansion (Pandaria, EasternKingdoms, Kalimdor, etc.)
- `src/server/database/` — database record definitions
- `modules/` — pluggable module system (loaded via `ModulesLoader`)
- `mod_playerbots/` — AI player bot module (early stage)
- `src/tools/` — map_extractor, vmap4_extractor/assembler, mmaps_generator
- `dep/` — vendored dependencies (Boost, fmt, StormLib, g3dlite, MySQL client, OpenSSL, RecastNavigation, etc.)
- `sql/base/` — base DB schema (`auth.sql`, `characters.sql`, `world.sql`)
- `sql/updates/` — incremental DB migration SQL, organized by date
- `contrib/lua_scripts/` — live Eluna Lua scripts (deployed alongside binaries)

## SQL / Database

- Base schema lives in `sql/base/` — apply these first
- Incremental updates in `sql/updates/` — ordered by filename prefix (date-based)
- Three databases: `auth`, `characters`, `world`
- Playerbots module uses its own DB: `mop_playerbots` (configured in `worldserver.conf`)

## Eluna Lua Engine

When `ELUNA=1`, the server loads `contrib/lua_scripts/` at runtime.
- **Quirk**: `mod_playerbots` is **HIGHLY recommended to disable** when Eluna is enabled (conflicts/instability).

## Player Bots Quirks

To enable bots beyond building:
1. Import playerbots database
2. Copy `playerbots.conf` into your build directory
3. Must use **enUS** DBC files
4. Add to `worldserver.conf`:
   ```
   PlayerbotsDatabaseInfo = "127.0.0.1;3306;root;root;mop_playerbots"
   PlayerbotsDatabase.WorkerThreads = 1
   PlayerbotsDatabase.SynchThreads = 1
   Logger.playerbots = 3,Console Server
   ```
5. First startup is slow — let it load and randomize

## Code Style

- Sun/Oracle C++ conventions
- 4 spaces (no tabs)
- LF line endings only (`core.autocrlf=true`)
- No trailing whitespace
- Squash PR commits; keep PRs tested (compile + functional)
- See `doc/code_standards.md` for full details

## CI

- Linux GCC: `.github/workflows/linux_gcc.yml` — GCC 13, Ubuntu 24.04
- Windows: `.github/workflows/windows-build-release.yml` — VS 2022, Boost 1.87, OpenSSL 3.1.1
- SQL changes are ignored in CI (`paths-ignore: sql/**`)
- Travis CI (`build/` dir, `cmake .. -DSCRIPTS=1 -DTOOLS=1`) is legacy

## Monk PlayerBot AI contribution workstream

This repository has a dedicated long-running contribution task to implement Monk PlayerBot combat AI for MoP 5.4.8 (build 18414). The working branch is `feature/monk-ai-object-context`. The authoritative handoff/status file for this task is `MONK_AI_PROGRESS.md`.

### Session-start protocol

Every new work session, including scheduled/automated runs, must do the following before editing code:

1. Fetch both remotes (`git fetch upstream` and `git fetch origin`) and compare upstream `master`, fork `master`, and `feature/monk-ai-object-context`.
2. Search upstream Issues and Pull Requests for `monk`, `Monk AI`, and `MonkAiObjectContext`.
   - If another contributor has started materially overlapping work, read it first.
   - Continue only on non-overlapping work when integration is clearly safe.
   - If the work conflicts or duplicates the same implementation, stop code changes and record the overlap in `MONK_AI_PROGRESS.md`; never overwrite another contributor's work.
3. Read `MONK_AI_PROGRESS.md` and continue from the last verified checkpoint. Do not restart the implementation from scratch merely because this is a new session.
4. If upstream advanced, rebase the feature branch only when the rebase is clean or conflicts can be resolved with unambiguous repository evidence.
   - Never invent a merge resolution.
   - For ambiguous conflicts, abort the rebase, record the conflicting paths and upstream/head SHAs in `MONK_AI_PROGRESS.md`, and leave the branch in a clean pre-rebase state for human review.

### Scope and architecture

- Implement Monk under `modules/mod_playerbots/src/strategy/Classes/monk/` using the repository's existing Strategy / Trigger / Action / AiObjectContext architecture.
- Use existing class implementations (especially `warrior/` and `druid/`) as structural templates, but implement Monk semantics rather than mechanically copying class logic.
- Expected components include:
  - `MonkAiObjectContext.h/.cpp`
  - generic Monk combat and non-combat strategies
  - Brewmaster tank strategy
  - Mistweaver healer strategy
  - Windwalker DPS strategy
  - Monk actions
  - Monk triggers
  - required strategy/action/trigger factory registrations following existing class patterns
- `modules/mod_playerbots/src/Factory/AiFactory.cpp` currently has the `CLASS_MONK` context construction disabled. Enable the include/case only after `MonkAiObjectContext` exists and is registered correctly.
- Existing Monk specialization role/name handling in `AiFactory` must be preserved unless a concrete bug is proven. Verify the existing specialization pipeline before modifying it.
- Treat `Factory/RandomPlayerbotFactory.cpp` as out of scope by default. Do not change its Monk specialization/race logic unless build or runtime evidence demonstrates a specific defect required by this task.
- Validate spell IDs, effects, resource rules, ranges, auras, stance/spec restrictions, and cooldown assumptions against MoP 5.4.8 build 18414. Do not import IDs blindly from another expansion or branch.

### Reference-source rule

DigiD702/mod-playerbots Monk rotation code may be used as a behavioral reference (for example Monk rotation or healer priority ideas), but do not copy-paste its implementation. Translate only verified behavior into this repository's Strategy / Trigger / Action architecture and naming conventions. Repository-local patterns take precedence over donor structure.

### Build and bug-fix loop

1. Work in small, reviewable functional increments.
2. After each meaningful increment, configure/build with PlayerBots enabled (`PLAYERBOTS=1`) using the repository-supported toolchain.
3. On build/test failure:
   - record symptom, failing target/test, suspected or confirmed cause, and affected paths in `MONK_AI_PROGRESS.md`;
   - fix the issue;
   - rebuild/retest;
   - mark a checklist item complete only after verification.
4. Keep feature work and unrelated bug fixes separated. When practical, use a dedicated fix commit for defects discovered while implementing a feature.
5. Never hide a failing build, crash, infinite loop, invalid cast loop, or unverified spell assumption merely to make the progress checklist look complete.
6. In-game tests, when an executable test environment is available, should cover bot creation/login, party join/follow, each Monk specialization's combat rotation, target switching, resource starvation/recovery, and Mistweaver healing behavior.

### Progress-file requirements

Update `MONK_AI_PROGRESS.md` in every session that changes code or changes a verification conclusion. It must include:

- current upstream/fork/feature SHAs when synchronization status changes;
- checklist status for context, factories, all three specs, build, and runtime tests;
- discovered bugs with symptom, cause/status, and fixing commit when available;
- external PR/issue overlap found during the session;
- exact remaining next action, so a later scheduled run can continue deterministically.

Do not mark an item `[x]` based on intent or partial code; `[x]` means the implementation or verification named by that item is actually complete.

### Upstream contribution gate

- Do not open the upstream implementation PR until all three specs are implemented and a `PLAYERBOTS=1` build has passed.
- Before opening the PR, search upstream again for a Monk AI issue/PR.
- If no dedicated implementation issue exists, open one first and record its URL/number and creation timestamp in `MONK_AI_PROGRESS.md`.
- Give maintainers an opportunity to respond before the PR: a later scheduled run may proceed once either (a) a maintainer has responded with direction, or (b) at least 24 hours have elapsed since the issue was opened without conflicting guidance. Do not simulate waiting inside one run.
- If an appropriate issue already exists, report progress there rather than creating a duplicate.
- The PR description must state implementation scope, repository-local structural references, verification performed, and known limitations.
- Follow upstream branch protection and review requirements. Never force-push shared/upstream branches and never overwrite unrelated contributor changes.

### Explicit prohibitions

- Do not bypass or delete `MONK_AI_PROGRESS.md` to restart the task.
- Do not overwrite, silently supersede, or force-rebase another contributor's overlapping work.
- Do not open the implementation PR before the three-spec implementation and build gate are satisfied.
- Do not copy donor rotation code verbatim.
- Do not change unrelated PlayerBot systems merely to broaden the scope of the contribution.
