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
- [x] Feature branch is now 27 commits ahead / 0 behind fork `master` before this progress-file update.
- [x] Upstream Issues and Pull Requests were searched again for `monk`, `Monk AI`, and `MonkAiObjectContext`.
- [x] No dedicated overlapping Monk PlayerBot combat-AI implementation was found in this run.
- [x] Upstream issue #413 is bot-command management and does not overlap Monk combat AI.
- [x] Upstream issue #150 is Tushui Monk NPC scripting and does not overlap PlayerBot Monk combat AI.

### Repository facts verified

- [x] `modules/mod_playerbots/src/Factory/AiFactory.cpp` already contains Monk role/spec-name/default-strategy handling.
- [x] `AiFactory::createAiObjectContext` still has the `CLASS_MONK` / `MonkAiObjectContext` construction disabled by comments; this remains intentionally unmodified until the new context receives a real build gate.
- [x] `modules/mod_playerbots/src/AI/PlayerbotSpec.cpp` already maps Brewmaster, Windwalker, and Mistweaver specializations and identifies Brewmaster as tank / Mistweaver as healer.
- [x] `modules/CMakeLists.txt` recursively globs module `.cpp`/`.h` files, so the new `Classes/monk/` files do not require a manual per-file CMake source-list edit.
- [x] `Factory/RandomPlayerbotFactory.cpp` already contains Monk/Pandaren race-pool handling; no concrete defect requiring a change was found, so it remains untouched.
- [x] Existing Warrior/Shaman/Druid/Paladin implementations were audited for AiObjectContext, strategy, action, trigger, role, cure, healing, AoE, and tank patterns.
- [x] Warrior `CastTauntAction` usefulness logic was used as the repository-local semantic reference for avoiding redundant Provoke casts.
- [x] `GenericSpellActions`, `GenericTriggers`, `CureTriggers`, `SpellCastUsefulValue`, and `PartyMemberToHealValue` were audited against the Monk wrappers.
- [x] `SpellCastUsefulValue` does not block normal spell usefulness just because a channel exists.
- [x] `CastSpellAction::isPossible()` delegates to `PlayerbotAI::CanCastSpell`, and `PlayerbotAI::CanCastSpell` rejects every new cast while `CURRENT_CHANNELED_SPELL` is non-null before Monk-specific spell scripts can apply exceptions.
- [x] `PlayerbotAI::CastSpell` itself prepares the spell and runs core `Spell::CheckCast`; it does not contain the same blanket pre-channel rejection used by `CanCastSpell`.
- [x] Target-core `spell_monk.cpp` explicitly gives Surging Mist `116694` and Enveloping Mist `124682` `TRIGGERED_CAST_DIRECTLY` when Soothing Mist `115175` is the active channel, and redirects their effective heal target to the Soothing Mist channel target.
- [x] `PartyMemberToHealValue` remains the repository-local priority selector for group healing.
- [x] Target-core `spell_monk.cpp` confirms Elusive Brew stacks `128939`, active Elusive Brew `115308`, Mana Tea stacks `115867`, active Mana Tea regen `115294`, and glyphed Mana Tea's two-stack cast requirement.
- [x] Shaman's `melee aoe` pattern was used to preserve distinct strategy identity for Windwalker AoE.

### Implementation checklist

- [x] Audit exact strategy/action/trigger factory patterns needed for a new class.
- [x] Create `modules/mod_playerbots/src/strategy/Classes/monk/` source set.
- [x] Implement `MonkAiObjectContext` skeleton and local strategy/action/trigger registrations.
- [x] Implement generic Monk combat strategy baseline.
- [x] Implement generic Monk non-combat/cure strategy baseline.
- [x] Implement initial Monk action set and party cure/heal action wrappers.
- [x] Implement initial Monk trigger set and cure/interrupt triggers.
- [ ] Complete Brewmaster (tank) strategy — Stagger severity purification, Shuffle maintenance, conservative Elusive Brew stack gating, and redundant-Provoke suppression are represented; Guard variant resolution, stronger lost-aggro target selection, and build/runtime validation remain.
- [ ] Complete Mistweaver (healer) strategy — baseline, Mana Tea stack guard, and narrow Soothing Mist channel-aware Surging/Enveloping actions are committed. Renewing Mist/Uplift decision quality plus build/runtime validation remain.
- [ ] Complete Windwalker (DPS) strategy — active Tigereye Brew action and exact `125195` 10-stack consumption trigger are implemented; Energy/Chi priority, Fists of Fury runtime channel behavior, and build/runtime refinement remain.
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

- `033af43b312a4d5cf4e38b952ef02b0556138dc6` — initial Monk AiObjectContext, strategies, actions/triggers, and three-spec baselines.
- `70064249f9aacb922013495ae7e5857b82314e84` — distinct `MeleeAoeMonkStrategy` identity.
- `9c205f8f34fb164d78da067e75d0037ee90f088a` / `54079cc5406d053091f7717109560d3490ff64e8` — Brewmaster Shuffle/Stagger behavior and Shuffle lookup fix.
- `9da0a566bbb295f9e36b5d5ae11b0fb4cd79ace5` / `83222eef1d1847b98acaa7c7ff81cd5c8688586d` / `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0` — Windwalker Tigereye Brew implementation and duration-safe exact stack check.
- `4b60c5d03c1795be073b12104830b7350ce13092` / `1d5e654f8d8c768073c785d4cb718ab57ed7ff45` — Elusive Brew and Mana Tea action-level stack gates.
- `5b003d6e489661e184b20967ba7982eed3577633` / `adc15302fbefa8bfe163dd212060b7a93d5d558e` — Provoke usefulness refinement matching the existing Warrior taunt semantic guard.
- `9f27e46002f776e5af72ce54787515474cc6c391` / `b27d7fe8bf25976a97d09638634850f8fa588324` — stack-check cleanup and explicit `SpellAuras.h` dependency.
- `a61dc31a2ceab27e81224640843500d06bd3c254` — declares channel-aware target/possible/useful hooks for Surging Mist and Enveloping Mist party actions.
- `00fb9d358a0bce15033a62ce594d931a3cad999f` — implements narrow Soothing Mist channel support without changing global PlayerBot casting behavior.
- `60eb4a8e7a383dacaee7aca643bbab7dd9d441f8` — adds DBC-backed power-cost validation to the channel-aware Mistweaver path before deferring final validation to core spell casting.

## Bugs / Issues discovered

### Fixed: `melee aoe` factory alias returned a strategy named only `aoe`

- **Symptom:** initial Monk context mapped both `aoe` and `melee aoe` to an object whose identity was only `aoe`.
- **Resolution:** distinct `MeleeAoeMonkStrategy`.
- **Fix commit:** `70064249f9aacb922013495ae7e5857b82314e84`.

### Fixed: Shuffle semantic trigger key could replace the aura lookup key

- **Symptom:** a semantic `no shuffle` trigger name would look for an aura literally named `no shuffle`.
- **Resolution:** factory key remains `no shuffle`, internal aura key remains `shuffle`.
- **Fix commit:** `54079cc5406d053091f7717109560d3490ff64e8`.

### Fixed: generic aura-stack helper imposed an unverified duration requirement on Tigereye Brew stacks

- **Symptom:** `HasAuraStackTrigger` forces `checkDuration=true` and could reject a valid stack aura.
- **Resolution:** Windwalker checks exact target-core aura ID `125195` and `GetStackAmount() >= 10` directly.
- **Fix commit:** `d2ca0d111e332fd68c3066776b8f9fb3e2f444b0`.

### Fixed: low-health/low-mana brew requests could occur without usable stacks

- **Symptom:** Elusive Brew and Mana Tea could be requested with no usable accumulated stacks.
- **Resolution:** Elusive Brew requires at least five confirmed `128939` stacks; Mana Tea requires at least two confirmed `115867` stacks.
- **Verification:** target-core static evidence complete; compiler/runtime verification pending.

### Fixed: Provoke could be requested when target already attacked the Brewmaster

- **Symptom:** generic current-target Provoke had no redundant-taunt guard.
- **Resolution:** Provoke now declines when the current target already targets the bot.
- **Remaining:** best lost-aggro attacker selection still needs a stronger target-value path/runtime validation.

### Fixed statically, runtime verification pending: generic PlayerBot channel gate blocked legal Mistweaver follow-up heals

- **Symptom:** `CastSpellAction::isPossible()` calls `PlayerbotAI::CanCastSpell`, which rejects any cast when `CURRENT_CHANNELED_SPELL` exists. This prevented Surging Mist and Enveloping Mist from reaching target-core Monk scripts while Soothing Mist was channeling.
- **Target-core evidence:** both Surging Mist `116694` and Enveloping Mist `124682` explicitly detect Soothing Mist `115175`, add `TRIGGERED_CAST_DIRECTLY`, and resolve the heal to the Soothing Mist channel target.
- **Resolution:** only the two Monk actions bypass the generic pre-channel rejection while a confirmed Soothing Mist channel exists. They bind to the actual channel target, reject the action if the repository's current `party member to heal` priority points at a different unit, verify learned spell/cooldown/control/immunity/power/basic target validity, then defer final cast validation to the existing `PlayerbotAI::CastSpell` / core `Spell::CheckCast` path.
- **Scope protection:** global `PlayerbotAI::CanCastSpell` was not changed, so other classes/spells retain existing channel behavior.
- **Commits:** `a61dc31a2ceab27e81224640843500d06bd3c254`, `00fb9d358a0bce15033a62ce594d931a3cad999f`, `60eb4a8e7a383dacaee7aca643bbab7dd9d441f8`.
- **Verification:** source-path evidence complete; real compile/runtime proof still required.

### Build execution environment unavailable in this run

- **Symptom:** a fresh `git ls-remote https://github.com/hilch1981-prog/Legends-of-Azeroth-Pandaria-5.4.8.git refs/heads/feature/monk-ai-object-context` retry still failed with `Could not resolve host: github.com`.
- **Scope:** execution environment only; GitHub connector reads/writes remained healthy.
- **Cause:** transient local DNS/network limitation.
- **Resolution:** retry a real build on a later run; do not classify this as a source/build failure.
- **CI note:** GitHub Actions API still reports zero workflow runs for this feature branch, so there is no CI build result to substitute for the missing local compile.

### Runtime semantics still intentionally unverified

- Guard has two target-core player-ability IDs (`123402`, `115295`); correct PlayerBot name resolution must be proven in DBC/runtime rather than guessed.
- Mistweaver channel-aware action logic is now implemented statically but needs compiler and in-game proof; Renewing Mist/Uplift target-selection quality remains to refine.
- Provoke still relies on `current target` rather than a proven best lost-aggro attacker value.
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
- Generic spell action/channel gate: `modules/mod_playerbots/src/strategy/actions/GenericSpellActions.cpp`, `modules/mod_playerbots/src/AI/PlayerbotAI.cpp`
- Generic spell usefulness: `modules/mod_playerbots/src/strategy/value/SpellCastUsefulValue.cpp`
- Party healing target selection: `modules/mod_playerbots/src/strategy/value/PartyMemberToHealValue.cpp`
- Target-core Monk spell implementation: `src/server/scripts/Spells/spell_monk.cpp`
- Spell verification ledger: `MONK_SPELL_AUDIT.md`
- External behavioral reference only: DigiD702/mod-playerbots Monk rotation/healer logic. Do not copy implementation verbatim.

## Next action

Retry a real `PLAYERBOTS=1` build first; compiler/linker failures take priority over more feature expansion. If the execution environment still cannot reach GitHub, continue only repository-backed work: audit Windwalker Fists of Fury resource/channel behavior and Brewmaster Guard/lost-aggro targeting without guessing DBC mappings. Do not enable the `MonkAiObjectContext` include/`CLASS_MONK` case in `Factory/AiFactory.cpp` until the new source set passes a real build gate. After build success, enable that integration in a separate commit and then perform runtime tests before opening any upstream contribution issue/PR.
