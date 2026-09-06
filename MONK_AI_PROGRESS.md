# Monk PlayerBot AI — Progress Handoff

> Working branch: `feature/monk-ai-object-context`
>
> Target upstream: `Legends-of-Azeroth/Legends-of-Azeroth-Pandaria-5.4.8`
>
> Target client/core: Mists of Pandaria 5.4.8, build 18414

## Progress (last updated: 2026-09-06)

### Synchronization / overlap check

- [x] Upstream, fork `master`, and the feature branch were checked at the start of this run.
- [x] Upstream `master` remains `6f264eea5ac21c4315e529e91554f8c0dd45b232` (`[Core/StatSystem] Modernize Statesystem (#425)`).
- [x] Fork `master` remains synchronized at the same `6f264eea5ac21c4315e529e91554f8c0dd45b232` SHA.
- [x] Upstream #425 does not modify any Monk feature-branch path; its `TankTargetValue` update is compatible with the Monk Provoke target selection added here.
- [ ] Feature branch rebase onto `6f264eea...` is pending. Immediately before this progress commit, compare showed 44 commits ahead / 1 behind fork `master`, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`.
- [x] No unsafe force-update was used. Fresh local Git probes still fail with `Could not resolve host: github.com`, so a trustworthy local rebase cannot be performed yet.
- [x] Upstream issues/PRs were searched again for Monk-related work. No dedicated overlapping Monk PlayerBot combat-AI implementation was found.
- [x] Issue #413 remains bot-command management only; issue #150 remains Tushui Monk NPC scripting only.

### Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] `PlayerbotSpec.cpp` already maps Brewmaster/Windwalker/Mistweaver and their tank/healer roles.
- [x] `modules/CMakeLists.txt` recursively includes module `.cpp`/`.h` files; no per-file CMake list edit is required for `Classes/monk/`.
- [x] `RandomPlayerbotFactory.cpp` still has no concrete defect requiring modification and remains untouched.
- [x] `ValueContext` exposes repository-native `tank target`, and upstream `TankTargetValue` uses current ThreatManager data to select a tank target that needs aggro attention.
- [x] `PartyMemberToHealValue` and `group members` are the repository-local group-healing/party-enumeration primitives used by Monk actions.
- [x] Existing Priest and Shaman healer strategies both use `party member to heal out of spell range` -> `reach party member to heal`; Mistweaver now follows the same movement pattern.
- [x] All Monk-local strategy, trigger, and action keys referenced by `AiFactory` and the three specialization strategies were cross-checked against `MonkAiObjectContext`; no missing or misspelled Monk-local factory key was found.
- [x] `SpellInfo` provides build-local `GetPowerType` and `CalcPowerCost`; `SharedDefines.h` confirms normal power types including Mana, Energy, and Chi.
- [x] Generic `PlayerbotAI::CanCastSpell(Unit*)` preflight constructs `Spell` with `TRIGGERED_IGNORE_POWER_AND_REAGENT_COST`, while the real `CastSpell` path later uses `TRIGGERED_NONE`; generic preflight alone therefore does not prove current Mana/Energy/Chi sufficiency.
- [x] Target-core Guard script is registered for player ability variants `115295` and `123402`, and the core defines `SPELL_MONK_GLYPH_OF_GUARD = 123401`. MoP-era references corroborate `115295` as normal Guard and `123402` as the Glyph-of-Guard spellbook override, but PlayerBot override/learned-spell resolution still must be proven before hardcoding either ID.

### Implementation checklist

- [x] Audit strategy/action/trigger factory patterns for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat and non-combat/cure baselines.
- [x] Implement initial Monk actions, interrupts, cures, and party-heal wrappers.
- [x] Implement initial Monk triggers.
- [ ] Complete Brewmaster — Shuffle maintenance, Stagger purification, Elusive Brew stack gating, repository-native lost-aggro Provoke targeting, and DBC-backed power preflight are represented. Guard override resolution plus build/runtime validation remain.
- [ ] Complete Mistweaver — Mana Tea stack gate, Soothing Mist channel-aware Surging/Enveloping, fresh-target Renewing Mist, Uplift coverage eligibility, DBC-backed power preflight, and out-of-range heal-target movement are represented. Build/runtime cadence tuning remains.
- [ ] Complete Windwalker — Rising Sun Kick/Fists of Fury baseline, exact `125195` 10-stack Tigereye Brew use, DBC-backed Energy/Chi preflight, and Windwalker-only Touch of Death are represented. Live priority/cadence and Fists of Fury movement/channel validation remain.
- [x] Verify existing Monk specialization/role/name pipeline; no modification is justified by current evidence.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` creation path in `Factory/AiFactory.cpp` only after a real `PLAYERBOTS=1` build passes.
- [x] Verify `RandomPlayerbotFactory.cpp` requires no change on current evidence.
- [ ] Finish MoP 5.4.8 spell/resource/range/aura/cooldown validation; static power-cost and most active spell mappings are covered, while Guard override semantics and runtime behavior remain.
- [ ] Configure/build successfully with `PLAYERBOTS=1`.
- [ ] Resolve all Monk-related compiler/linker failures introduced by this branch.
- [ ] In-game: Monk bot creation/login and party join/follow.
- [ ] In-game: Brewmaster rotation/survival/taunt/Guard.
- [ ] In-game: Mistweaver healing/channel/Renewing Mist/Uplift/Mana Tea.
- [ ] In-game: Windwalker Energy/Chi/Fists of Fury/Tigereye Brew.
- [ ] Final upstream Monk AI overlap search immediately before contribution.
- [ ] Upstream implementation issue created or an appropriate existing issue updated.
- [ ] Maintainer-response / 24-hour issue gate satisfied.
- [ ] Upstream implementation PR opened with scope, verification, and known limitations.

## Recent implementation / audit commits

- `033af43b312a4d5cf4e38b952ef02b0556138dc6` — initial Monk AiObjectContext, actions/triggers, and three-spec baselines.
- `70064249f9aacb922013495ae7e5857b82314e84` — distinct `MeleeAoeMonkStrategy` identity.
- `9c205f8f34fb164d78da067e75d0037ee90f088a` / `54079cc5406d053091f7717109560d3490ff64e8` — Brewmaster Shuffle/Stagger behavior and Shuffle lookup fix.
- `9da0a566bbb295f9e36b5d5ae11b0fb4cd79ace5` / `83222eef1d1847b98acaa7c7ff81cd5c8688586d` / `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0` — Tigereye Brew action and exact 10-stack trigger.
- `4b60c5d03c1795be073b12104830b7350ce13092` / `1d5e654f8d8c768073c785d4cb718ab57ed7ff45` — Elusive Brew and Mana Tea stack gates.
- `a61dc31a2ceab27e81224640843500d06bd3c254` / `00fb9d358a0bce15033a62ce594d931a3cad999f` / `60eb4a8e7a383dacaee7aca643bbab7dd9d441f8` — channel-aware Surging/Enveloping Mist.
- `459534172aaec35e334617e228fbfef2712a87ad` / `690494bf3fdc1aa04b5ef7da8ad596defb7df43d` — Provoke uses repository-native `tank target`; Uplift action-level eligibility support.
- `fa5e35059d08f14988eb165337870115ef09a73f` / `bb782dbaee36d686be1bb47d7bb73049b6be95e3` — fresh-target Renewing Mist and caster-owned Uplift coverage gating.
- `2f6ecf7818087a0c1eba8e5211dfc30d06fb2e90` / `d68a7774a589584da2104b8a84b20d5bf207e243` — initial DBC-backed power preflight for Monk combat/resource actions.
- `ff2fc3a1cb94fdcf08c557dfd9bd8a386f90d7a5` / `30a215e4a6e84fd0abbd2f68d3c8f5b24ed458f8` — extend DBC power preflight to high-priority heals, cures, Guard, Touch of Death, and utility/resource spenders.
- `d70607254cf76db328ad432e1389a277ca1496cc` — harden Monk power preflight for `POWER_HEALTH` and unexpected special power types.
- `e3495c323c270258dca63af223d3141933a0138d` — remove generic/duplicate Touch of Death trigger so the high-priority execute remains Windwalker-only.
- `931ec9803f39af55216a4a6bb8dc23de43ef089c` — add repository-standard heal-target range recovery to Mistweaver.
- `d938494fee03ea7e374afe84c3194e17ebdad457` — refine Guard, factory-key, healer-range, and power-preflight evidence in `MONK_SPELL_AUDIT.md`.

## Bugs / issues discovered

### Fixed statically: high-priority Monk actions could pass preflight without enough power

- **Symptom:** a high-priority Energy/Chi/Mana action could be repeatedly selected because generic `CanCastSpell` ignored power/reagent cost during preflight, then fail later in the real cast path.
- **Repository evidence:** `PlayerbotAI::CanCastSpell(Unit*)` uses `TRIGGERED_IGNORE_POWER_AND_REAGENT_COST`; actual `PlayerbotAI::CastSpell` creates a normal `TRIGGERED_NONE` spell and calls the real cast check. `SpellInfo` exposes `GetPowerType` and `CalcPowerCost` against current bot/build data.
- **Resolution:** Monk-local `HasPowerForSpell` checks the resolved spell's DBC-derived current power cost before the existing action preflight. No Mana/Energy/Chi cost is hardcoded and global PlayerBot behavior is unchanged. Health-cost and unexpected power types now fail safely.
- **Verification:** declarations/definitions and the underlying SpellInfo power-selection implementation were re-read statically. Real compile/runtime validation remains mandatory.

### Fixed statically: Touch of Death was generic and duplicated for Windwalker

- **Symptom:** `GenericMonkStrategy` registered Touch of Death for all Monk specs while `DpsMonkStrategy` registered the same trigger again, allowing Brewmaster/Mistweaver to request a high-priority offensive resource action and duplicating Windwalker registration.
- **Resolution:** remove Touch of Death from the generic strategy and retain it only in `DpsMonkStrategy`.
- **Verification:** Monk strategy/action/trigger factory keys were re-audited after the change; no orphaned reference was introduced.

### Fixed statically: Mistweaver lacked heal-target range recovery

- **Symptom:** Mistweaver healing actions could remain unavailable when `party member to heal` was outside spell range without a class strategy trigger to close range.
- **Repository evidence:** both Priest and Restoration Shaman healer strategies use `party member to heal out of spell range` -> `reach party member to heal`.
- **Resolution:** add the same repository-standard movement trigger to `HealMonkStrategy` at critical-heal movement priority.
- **Verification:** uses global trigger/action contexts already present in the repository; no Monk-local factory registration is required.

### Fixed: Provoke used only `current target`

- **Symptom:** Brewmaster could request Provoke for the current target rather than the repository's best lost-aggro tank target.
- **Resolution:** `CastProvokeAction::GetTargetName()` returns `tank target`, and the redundant-taunt guard checks that resolved target.
- **Verification:** static repository-path verification complete; runtime threat/taunt behavior pending.

### Fixed statically: Renewing Mist and Uplift target semantics

- Renewing Mist now prefers an eligible group member not already carrying this Monk's caster-owned `119611` HoT.
- Uplift now requires at least two injured group players with caster-owned Renewing Mist before it is considered useful.
- Target-core script evidence supports both restrictions; runtime cadence remains pending.

### Fixed statically, runtime pending: legal Mistweaver follow-up heals were blocked by the generic channel gate

- Generic `PlayerbotAI::CanCastSpell` rejects a new cast while channeling.
- Target-core Surging Mist and Enveloping Mist explicitly allow direct casts during Soothing Mist and redirect to its current channel target.
- Monk actions now implement that narrow exception without changing global PlayerBot casting behavior.

### Guard mapping narrowed but intentionally not hardcoded

- Target core binds `spell_monk_guard` to player ability variants `115295` and `123402` and defines Glyph of Guard aura `123401`.
- MoP-era spell data identifies `123402` as the spellbook override when Glyph of Guard is active, with `115295` as the normal Guard.
- `PlayerbotAI::CanCastSpell(uint32, ...)` ordinarily requires `bot->HasSpell(spellId)`. Until this core's glyph override exposure to `HasSpell`/`SpellIdValue` is proven, forcing one numeric ID could regress glyphed or unglyphed bots. Current name-based Guard action is therefore retained.

### Build/rebase execution environment unavailable in this run

- **Symptom:** local Git/network probes still fail DNS resolution for GitHub; a second Git host probe also failed DNS, so this is broader execution-environment DNS failure rather than a repository URL error.
- **Effect:** no trustworthy local rebase or `PLAYERBOTS=1` compile can be executed from this environment.
- **Mitigation:** upstream/fork state and source are verified through the GitHub connector; feature edits continue only on paths not touched by upstream #425. No force-push or fabricated rebase was used.
- **CI:** the fork workflow is configured for `push`, but GitHub Actions still reports zero workflow runs for `feature/monk-ai-object-context`, so there is no CI result to substitute for a real build.
- **Next:** retry clean local rebase/build when Git network access returns. Treat this as an environment limitation, not a source build failure.

### Runtime semantics still intentionally unverified

- Guard override-spell exposure and actual base/glyphed cast selection.
- actual Provoke `tank target` selection under multi-attacker threat.
- Mistweaver channel, range recovery, Renewing Mist/Uplift timing, and Mana Tea cadence in a live group.
- Windwalker live Energy/Chi cadence and Fists of Fury movement/channel interruption.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI issue/PR found during the current run.
- Issue #413 is command-management related and does not block this workstream.
- Issue #150 is NPC scripting related and does not block this workstream.

## Reference sources

- class architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`, `druid/`, `shaman/`, `paladin/`, `priest/`
- context integration: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- specialization helpers: `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp`
- action/channel behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`, `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- target values: `modules/mod_playerbots/src/strategy/value/ValueContext.h`, `TankTargetValue.cpp`, `PartyMemberToHealValue.cpp`, `GroupValues.cpp`
- name-to-spell resolution: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- power-cost API: `src/server/game/Spells/SpellInfo.h`, `src/server/game/Spells/SpellInfo.cpp`, `src/server/game/Miscellaneous/SharedDefines.h`
- target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- spell ledger: `MONK_SPELL_AUDIT.md`
- external donor code is behavioral reference only; do not copy it verbatim.

## Next action

1. Retry a clean rebase of `feature/monk-ai-object-context` onto upstream/fork `master` `6f264eea...` as soon as real Git network access is available; do not force-push or fabricate conflict resolution.
2. Run a real `PLAYERBOTS=1` build immediately after synchronization. Compiler/linker failures take priority over new feature expansion.
3. If build access remains unavailable, continue repository-backed static work only: trace Guard spellbook override handling if possible, and audit remaining target/range/channel semantics without inventing numeric costs or runtime results.
4. Do not enable the `MonkAiObjectContext` include/`CLASS_MONK` case in `Factory/AiFactory.cpp` until the new source set passes a real build gate. After build success, enable integration in a separate commit, then perform runtime tests before opening the upstream coordination issue/PR.
