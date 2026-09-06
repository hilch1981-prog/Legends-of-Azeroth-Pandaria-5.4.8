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
- [x] The upstream #425 change touches core stat/threat code and PlayerBot `AttackersValue` / `TankTargetValue`, but it does not modify any Monk feature-branch path.
- [ ] Feature branch rebase onto `6f264eea...` is pending. Before this progress-file commit, compare showed 39 commits ahead / 1 behind fork `master`, merge base `3ec151e16c7912b217838040ac1bb30c6f1fc84d`.
- [x] No unsafe force-update was used. A fresh local `git ls-remote` attempt still fails with `Could not resolve host: github.com`, so a trustworthy local rebase cannot be performed yet.
- [x] Upstream issues/PRs were searched again for Monk-related work. No dedicated overlapping Monk PlayerBot combat-AI implementation was found.
- [x] Issue #413 remains bot-command management only; issue #150 remains Tushui Monk NPC scripting only.

### Repository facts verified

- [x] `Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling; only `MonkAiObjectContext` construction remains commented.
- [x] `PlayerbotSpec.cpp` already maps Brewmaster/Windwalker/Mistweaver and their tank/healer roles.
- [x] `modules/CMakeLists.txt` recursively includes module `.cpp`/`.h` files; no per-file CMake list edit is required for `Classes/monk/`.
- [x] `RandomPlayerbotFactory.cpp` still has no concrete defect requiring modification and remains untouched.
- [x] `ValueContext` exposes repository-native `tank target`, and upstream `TankTargetValue` uses current ThreatManager data to select a tank target that needs aggro attention.
- [x] `SpellIdValue` resolves action names from the bot's actually learned active spells. Guard still needs DBC/runtime proof because target-core Guard binds two player-ability IDs (`123402`, `115295`) and the learned-spell variant must not be guessed.
- [x] `PartyMemberToHealValue` and `group members` are the local group-healing/party-enumeration primitives used by the Monk actions.
- [x] Target-core `119611 Renewing Mist` is caster-bound, creates the Uplift-allowing aura, jumps to units not already carrying the same caster's Renewing Mist, and prefers an injured jump target when available.
- [x] Target-core Uplift `116670` restricts its heal target set to units carrying Renewing Mist `119611` from the caster; Thunder Focus Tea controls the refresh effect while the heal set remains the caster's Renewing Mist targets.
- [x] Target-core Fists of Fury `113656` is implemented as an aura/channel-style periodic damage ability; real movement/channel interruption behavior still needs runtime validation.
- [x] Surging Mist `116694` and Enveloping Mist `124682` explicitly allow direct casts during Soothing Mist `115175` and redirect to the active Soothing Mist channel target.
- [x] `SpellInfo` provides build-local `GetPowerType` and `CalcPowerCost`; `SharedDefines.h` confirms MoP power types including Mana, Energy, and Chi.
- [x] Generic `PlayerbotAI::CanCastSpell(Unit*)` preflight constructs `Spell` with `TRIGGERED_IGNORE_POWER_AND_REAGENT_COST`, while the real `CastSpell` path later uses `TRIGGERED_NONE`. Therefore generic action preflight alone does not prove current Mana/Energy/Chi sufficiency.

### Implementation checklist

- [x] Audit strategy/action/trigger factory patterns for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat and non-combat/cure baselines.
- [x] Implement initial Monk actions, interrupts, cures, and party-heal wrappers.
- [x] Implement initial Monk triggers.
- [ ] Complete Brewmaster — Shuffle maintenance, Stagger purification, Elusive Brew stack gating, repository-native lost-aggro Provoke targeting, and DBC-backed power preflight for resource-sensitive actions are represented. Guard learned-spell variant plus build/runtime validation remain.
- [ ] Complete Mistweaver — Mana Tea stack gate, Soothing Mist channel-aware Surging/Enveloping, fresh-target Renewing Mist, Uplift coverage eligibility, and DBC-backed power preflight for major heals are represented. Build/runtime cadence tuning remains.
- [ ] Complete Windwalker — Rising Sun Kick/Fists of Fury baseline, exact `125195` 10-stack Tigereye Brew use, and DBC-backed Energy/Chi power preflight are represented. Live priority/cadence and Fists of Fury movement/channel validation remain.
- [x] Verify existing Monk specialization/role/name pipeline; no modification is justified by current evidence.
- [ ] Enable `MonkAiObjectContext` include and `CLASS_MONK` creation path in `Factory/AiFactory.cpp` only after a real `PLAYERBOTS=1` build passes.
- [x] Verify `RandomPlayerbotFactory.cpp` requires no change on current evidence.
- [ ] Finish MoP 5.4.8 spell/resource/range/aura/cooldown validation; static power-cost resolution now uses target-core DBC data, while Guard and runtime semantics remain.
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
- `c84e944400f9662342f7523b53e1fc8e5567b392` — document the PlayerBot power-preflight gap and Monk-local DBC-backed mitigation in `MONK_SPELL_AUDIT.md`.

## Bugs / issues discovered

### Fixed statically: high-priority Monk actions could pass preflight without enough power

- **Symptom:** a high-priority Energy/Chi/Mana action could be repeatedly selected because generic `CanCastSpell` ignored power/reagent cost during preflight, then fail later in the real cast path.
- **Repository evidence:** `PlayerbotAI::CanCastSpell(Unit*)` uses `TRIGGERED_IGNORE_POWER_AND_REAGENT_COST`; actual `PlayerbotAI::CastSpell` creates a normal `TRIGGERED_NONE` spell and calls the real cast check. `SpellInfo` exposes `GetPowerType` and `CalcPowerCost` against the current bot/build data.
- **Resolution:** a Monk-local `HasPowerForSpell` helper resolves the learned action spell and checks its DBC-derived current power cost before delegating to the existing action preflight. No Mana/Energy/Chi number is hardcoded and global PlayerBot behavior is unchanged.
- **Scope:** primary Monk combat spenders, Brewmaster resource actions/Guard, Touch of Death, Detox variants, and major Mistweaver heal/resource actions.
- **Verification:** declarations and definitions were re-read after the edits and match statically. Real compile/runtime validation remains mandatory.

### Fixed: Provoke used only `current target`

- **Symptom:** Brewmaster could request Provoke for the current target rather than the repository's best lost-aggro tank target.
- **Upstream evidence:** `ValueContext` registers `tank target`; upstream #425 modernized `TankTargetValue` against the current ThreatManager API.
- **Resolution:** `CastProvokeAction::GetTargetName()` now returns `tank target`, and the redundant-taunt guard checks that resolved target.
- **Verification:** static repository-path verification complete; runtime threat/taunt behavior still pending.

### Fixed statically: Renewing Mist could repeatedly prefer an already-covered heal target

- **Symptom:** generic `party member to heal` can remain the lowest-health member even when that unit already carries this Monk's `119611` Renewing Mist, wasting spread opportunities.
- **Target-core evidence:** Renewing Mist is caster-bound; its jump selector explicitly excludes units already carrying the same caster's `119611` HoT.
- **Resolution:** the action keeps the normal heal-priority target when uncovered; otherwise it selects the lowest-health valid group player without this Monk's `119611` HoT. If no fresh target exists, usefulness rejects the duplicate cast.
- **Verification:** static source evidence complete; compile/runtime proof pending.

### Fixed statically: Uplift could be considered without useful Renewing Mist coverage

- **Symptom:** the generic self-heal action wrapper had no awareness that target-core Uplift heals only the caster's Renewing Mist targets.
- **Target-core evidence:** Uplift's target filter retains only units with `119611` from the caster.
- **Resolution:** Uplift now requires at least two injured group players with caster-owned Renewing Mist before returning useful.
- **Verification:** static source evidence complete; exact in-game tuning remains pending.

### Fixed statically, runtime pending: legal Mistweaver follow-up heals were blocked by the generic channel gate

- `CastSpellAction::isPossible()` delegates to `PlayerbotAI::CanCastSpell`, whose generic channel guard blocks a normal new cast while a channel exists.
- Surging Mist and Enveloping Mist are target-core exceptions during Soothing Mist and are handled narrowly in Monk actions without changing the global PlayerBot cast gate.

### Previously fixed static issues retained

- distinct `melee aoe` strategy identity;
- Shuffle trigger key vs actual Shuffle aura lookup;
- Tigereye Brew exact stack aura instead of duration-dependent generic stack trigger;
- Elusive Brew / Mana Tea requests without accumulated stacks.

### Build/rebase execution environment unavailable in this run

- **Symptom:** a fresh local Git probe still fails with `Could not resolve host: github.com`.
- **Effect:** no trustworthy local rebase or `PLAYERBOTS=1` compile can be executed from this environment.
- **Mitigation:** upstream/fork state and source are verified through the GitHub connector; feature edits continue only on paths not touched by upstream #425. No force-push or fabricated rebase was used.
- **CI:** the fork workflow is configured for `push`, but GitHub Actions still reports zero workflow runs for `feature/monk-ai-object-context`, so there is no CI result to substitute for a real build.
- **Next:** retry clean local rebase/build when Git network access returns. Treat this as an environment limitation, not a source build failure.

### Runtime semantics still intentionally unverified

- Guard player-ability resolution between target-core IDs `123402` / `115295`.
- actual Provoke `tank target` selection under multi-attacker threat.
- Mistweaver channel and Renewing Mist/Uplift timing in a live group.
- Windwalker live Energy/Chi cadence and Fists of Fury movement/channel interruption.

## External overlap / coordination

- No dedicated overlapping Monk PlayerBot combat-AI issue/PR found during the current run.
- Issue #413 is command-management related and does not block this workstream.
- Issue #150 is NPC scripting related and does not block this workstream.

## Reference sources

- class architecture: `modules/mod_playerbots/src/strategy/Classes/warrior/`, `druid/`, `shaman/`, `paladin/`
- context integration: `modules/mod_playerbots/src/Factory/AiFactory.cpp`
- specialization helpers: `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp`
- action/channel behavior: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`, `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- target values: `modules/mod_playerbots/src/strategy/value/ValueContext.h`, `TankTargetValue.cpp`, `PartyMemberToHealValue.cpp`, `GroupValues.cpp`
- name-to-spell resolution: `modules/mod_playerbots/src/strategy/value/SpellIdValue.cpp`
- power-cost API: `src/server/game/Spells/SpellInfo.h`, `src/server/game/Miscellaneous/SharedDefines.h`
- target-core Monk behavior: `src/server/scripts/Spells/spell_monk.cpp`
- spell ledger: `MONK_SPELL_AUDIT.md`
- external donor code is behavioral reference only; do not copy it verbatim.

## Next action

1. Retry clean rebase of `feature/monk-ai-object-context` onto upstream/fork `master` `6f264eea...` as soon as real Git network access is available; do not force-push or fabricate conflict resolution.
2. Retry a real `PLAYERBOTS=1` build immediately after synchronization. Compiler/linker failures take priority over new feature expansion.
3. If build access remains unavailable, continue only repository-backed static work: resolve Guard only if learned-spell/DBC evidence identifies the correct player variant; otherwise keep it runtime-gated. Review remaining high-priority Monk actions for target/range/channel semantics without inventing costs.
4. Do not enable the `MonkAiObjectContext` include/`CLASS_MONK` case in `Factory/AiFactory.cpp` until the new source set passes a real build gate. After build success, enable integration in a separate commit, then perform runtime tests before opening the upstream coordination issue/PR.
