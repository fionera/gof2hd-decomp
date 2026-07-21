# Byte-matching campaign log

Orchestrator session log. One entry per session; newest first. Resume from git log + this file.

## Session 2026-07-21 (exclusive: Gun.h layout fix)

Gun.h 0x38-0x48 span fixed (dac2ac0e): NOT a +4 drift — a 3-field intra-span reorder.
ASM-derived from orig ctor @0x151f20 (two new[] stores at 0x3c/0x40) + dtor @0x152134
(delete[] [0x3c] null-checked then [0x40] unchecked = our source order) + strd pair 0x44/0x48.
New order: field_0x38(int, ctor-zeroed), lifetimes@0x3c, hitFlags@0x40, initialLifetime@0x44,
fireDelay@0x48. No size change; static_asserts added. Follow-up 1d9b2f78: ctor zeroes
field_0x38 like orig (no metric delta, semantic alignment).
Result: linked 2226 -> 2227 (MineGun::render linked-exact), Gun D1/D2 -> 92.9,
refillGunDelay -> 90.9. No regressions. Full derivation: _work/reconstructions/Gun-layout.md.

QUEUE CORRECTIONS (verified against source, 2026-07-21):
- AEMath::Matrix 0x3c re-size + Transform re-layout: ALREADY DONE (e0d971d4, 2026-07-06;
  Matrix.h is float m[15] with sizeof==0x3c assert; no unlinked Transform:: rows remain).
  The "Matrix mis-size gates Gun::shoot" note was STALE.
- Gun::shootAt is a PLACEHOLDER body (usize 18 vs orig 1788) — it's an unimplemented monster
  fn needing real reconstruction (DeepOpen oracle + orig disasm), orchestrator-only.
  Gun::shoot (orig 88B) forwards to it and will follow once shootAt exists.
End state: **byte 1132, linked 2227, stubs 0, extra 36, imports 1090, avg 73.53.**

## Session 2026-07-21 (wave 4 — MCP-free, 8 workers, all landed)

Pre-wave: FModSound::disableReverb cracked by orchestrator — orig memcpys 80B from rodata
@0x1fb540 which decodes EXACTLY as FMOD_PRESET_OFF (FMOD Ex 4.44 fmod.h); all-zero placeholder
folded to __aeabi_memclr. Fix: non-zero function-static struct + unsigned char buf (canary) ->
linked-exact; __aeabi_memcpy8 import added (present in original's set, 1090->1091).

Wave 4: 8 disjoint workers from fresh diffs4 dumps (dumper now persisted:
scratchpad/dump_diffs.py). Zero deaths, 8/8 items gated PASS, one hunk-revert (Route::getWaypoint
placement-new rewrite regressed 2pts; kept update+ctor wins). Landed highlights:
- PlayerWormHole get/setPosition -> 100 (declare fy before fx / store Y before X: LLVM batches
  VFP converts in Y-first order).
- AEPakFile/AENormalFile::Release byte-exact (fileInterface->Close() vtable call before delete);
  the throwing virtual call cascades the SJLJ terminate stub into the dtors (32->36B, ->96.8).
- CheatHandler::AddCheatCode 59.5->98.8 (String::operator[](int) call form fixes regalloc).
- MiningGame left/right byte-exact (void return kills the vmov r0,s0 materialization —
  same trick as Route::update(fff) 50.3->97.8); spurious MiningGame_sqrt import removed
  via globals->sqrt() (imports 1091->1090).
- Achievements ctor/dtor pairs linked-exact; initCheckEquipmentAndWeapons 44.8->81.6.
- AutoPilotList D1/D2 + getTargetString linked-exact; MineGun dtor/ctor +18/+15.
- BluePrint ctor linked-exact (init order); addItem 35.1->50.9 (early Globals::status load
  reproduces 9-reg prologue).
End state: **byte 1132, linked 2226, stubs 0, extra 36, imports 1090, avg 73.52.**

New cross-cutting learnings:
- void-return discovery: when orig never materializes r0/s0 at return and ours does, the decomp
  return type is wrong; fixing it frees a callee-saved VFP reg and can jump 40+ pct.
- Gun.h FIELD DRIFT found: gun->hitFlags is 0x40 in orig, 0x3c in our Gun.h — blocks MineGun
  render/update (and likely other Gun users). Add to the tier-5 drifted-fields exclusive pass.
- Recurring 4B tail: missing EH terminate stub in dtors (C++14 implicit-noexcept String dtor vs
  original's throwing-dtor EH pads). The files-worker workaround (a genuinely throwing virtual
  call in a sibling method) fixed AEPak/AENormal dtors; Objective/Route dtors still blocked on it.
- Ship::addMod: orig uses the CLZ equality trick (subs/clz/lsrs #5) = source shape
  `int += (a == b)`; untried, good next retry.
- Canary-unknown-trigger family (PlayerJunk::update, MineGun::update, AutoPilotList ctor/draw):
  orig has canary, plain -fstack-protector needs a byte-array >=8 local we haven't found yet —
  suspect a String/temp with inline char storage in the original source.
- Worker claims of "PLT-address-diff-only" at <100 pct (Wanted D1/D2, Objective setAchievedText,
  PlayerJunk render pre-fix) are USUALLY WRONG — verify masks true PLT displacement diffs; if
  it shows "|" the callee or code shape differs. Treat such claims as unverified.

Queue after wave 4: per-class tier (4) essentially exhausted — remaining are shader-zoo
(TU-pooling, exclusive), drifted fields (exclusive, now incl. Gun.h hitFlags), TU merges
(exclusive), monster fns (orchestrator-only), and hard singles (GameText::setLanguage rework,
Standing::getMissionBonus vmax, ScrollTouchWindow::setText, FModSound::updateAll 31.0,
Achievements::checkForNewMedal 7.3, MiningGame::render2D 18.9, AutoPilotList::draw 34.3,
PlayerWormHole::update 43.9, Route clone/getWaypoint/2-arg ctor).

## Session 2026-07-21 (wave 3 — MCP-free, clean run)

4 workers, NO Ghidra (explicitly forbidden in prompts after the wave-2 crash loop) — diff dumps
only. Zero worker deaths. Landed serially:
- FModSound: FMOD vtable-dispatch static wrappers replaced 5 spurious PLT imports (1095->1090 —
  original dispatches FMOD Event/Category calls through vtables inline, not PLT);
  promptMusicCue/setMusicParamValue/setVolume linked-exact; downPitch int8_t (strb). +3 byte.
- ScrollTouchWindow partial: split the worker patch — kept drawTextBG/ctor hunks, REVERTED the
  draw() hunk (regressed 43->39.6 in gate). Hunk-splitting a mixed patch works well.
- Standing/ListItem retry: ListItem copy-ctor + dtor pairs linked-exact (+4 linked, +2 byte).
- trades patch: measurable NO-OP (all 4 targets identical pct) — reverted, not landed.
End state: **byte 1127, linked 2205, stubs 0, extra 36, imports 1090, avg 73.38.**

Blocked items with actionable analysis (orchestrator or next wave):
- FModSound::disableReverb: orig memcpys an 80-byte non-zero reverb-properties struct from
  rodata — extract the bytes from the original .so literal pool (objdump -s) and inline them.
- GameText::setLanguage(s,i): orig prologue proves the original SOURCE had (langId, stringCount)
  parameter ROLES swapped relative to our decl usage — needs GameText.h+cpp co-owned rework.
- Standing::getMissionBonus: orig uses vmax.f32 — clang only emits it under fast-math; likely
  needs the original's expression shape (not std::max) or is TU-flag related. Deferred.
- ScrollTouchWindow::setText x2: orig saves/compares a two-level global deref (canary-like
  pattern around String assignment) — understand before retrying.
- "Scheduling artifact" claims (enableReverb, resume, pauseAllPlaying, IsCategoryEnabled...):
  worker gave up on register-allocation deltas; a fresh worker with different idiom attempts may
  still crack these — do not treat as proven-impossible.

## Session 2026-07-21 (wave 2 — salvaged)

Wave 2 launched as an 8-worker Workflow (Ship, FModSound, Wanted+WantedWindow, GameText,
LodMeshMerger, ScrollTouchWindow, BluePrint+Objective, Standing/ListItem retry). The **Ghidra MCP
connection flapped** ("MCP error -32000: Connection closed") and every MCP call during a drop
KILLED the calling worker; the workflow crash-retried ~6 generations per item, burning tokens with
one clean completion (wantedfam). Stopped the workflow, snapshotted per-item patches from the tree,
and gated them serially. Landed:
- 48e19218 Wanted/WantedWindow (completed worker): lastSeen/currentLocation field swap to binary
  offsets 68/76 -> +4 byte-exact accessors; byte 1116->1120, linked 2190->2192. Trade: Wanted D1/D2
  linked->90.9 (field-order-sensitive dtor) REQUEUED.
- Ship (partial): hasEquipment/hasCargo 29.5->96.9, hasSecondaryWeapons linked; 2192->2193.
  Trade: addMod 90.6->86.3 REQUEUED.
- BluePrint/Objective (partial): +2 byte (1122), +5 linked (2198). No regressions.
- LodMeshMerger (partial): init/update pct only.
- 75e5e32e GameText (partial): getLanguage ->85.7; linked 2198->2200. Trade: setLanguage
  19.1->14.4 (half-rewritten when worker died) REQUEUED.
End state: **byte 1122, linked 2200, stubs 0, extra 36, imports 1095, avg 73.32.**

Untouched (workers died before editing) -> wave 3: FModSound (29 fns), ScrollTouchWindow (8),
Standing/ListItem retry. Plus requeued trades above and wave-1 retry queue.

INFRA learnings (binding for future sessions):
- If the Ghidra MCP is flapping/disconnected, do NOT run Workflow workers that call it — they die
  mid-call and crash-loop. Either get the user to reconnect (bridge itself was alive on :8089;
  the session-side client was the broken half) or run workers MCP-free on the pre-dumped
  side-by-side diffs (they contain BOTH sides' full disassembly — enough for most matching).
- macOS backgrounds/App-Naps background-task processes AND sleeps between orchestrator turns:
  a ratchet that takes ~7 min foreground ran >5 h backgrounded. ALWAYS gate via foreground Bash
  wrapped in caffeinate -dims; never via run_in_background.

## Session 2026-07-21 (wave 1)

Queue position: **tier 1 (stubs) COMPLETE — stub_zero_size 12 → 0.** Tier 3/4 in progress.

Landed (each its own ratchet-gated commit, baseline locked):
- PlayerEgo::stopBoost, MTitle (2 stubs + 4 fns), Standing, ListItem (+24 linked via
  init()-inlining root-cause fix), BeamGun/SpriteGun (5 stubs), ImageFactory (reload stub + 10 fns),
  MeshMerger (last 3 stubs; memset→__aeabi_memclr idiom, plain-delete for scalar cell free).
- **verify metric v2.1** (91a8ddd0): PIC `ldr rX,[pc]`+`add rX,pc` GOT-delta pool words are now
  always masked. Root cause of phantom linked_exact flips (±2-5) on untouched functions at every
  layout shift — a delta accidentally equal to a function entry was compared as a named function
  pointer. Masking-only: +20 linked recovered at the flip.
- Counts: byte_exact 1115→1116, linked_exact 2125→2190, stubs 12→0, avg 72.33→73.13.

Fleet learnings (apply to future waves):
- Worker prompts must ban `operator new/delete` spelling (goal_lint operator_call is soft-ratcheted
  per file) and hand-declared `__aeabi_*` (mangles to a C++ import → ratchet import-set FAIL).
  Use `memset`/plain `delete` and let clang emit the aeabi calls.
- Workers syntax-check with compile_commands.json + -fsyntax-only; all 7 did this successfully.
- Serial gating flow: split worker edits into per-item patches by file ownership; revert tree;
  apply→ratchet→(--update-baseline + commit)|revert per item. Worked cleanly.
- Root report.json is only synced by the cmake verify target; ratchet.py writes
  cmake-build-match/verify/report.json — `cp` it to root in each commit.
- Wave-1 residual retry queue (unlinked, has fresh bodies now): Standing isEnemy/isFriend/
  getStanding/getMissionBonus; ListItem init/D1/checkCredits/checkSlot/checkSort/copy-ctor;
  MTitle OnKeyPress(50%)/OnKeyRelease(50%)/OnRender2D(64.9); MeshMerger init/setMesh/
  transformMesh/big-ctor/dtor-body; ImageFactory createChar/drawItem family; BeamGun ctor/update.
  MeshMerger::transformMesh: our body is functionally WRONG (2-arg shim passing matrix as vector;
  original iterates positions/normals with a 3-arg MatrixTransformVector form) — needs the real
  3-arg engine call modeled without inventing a new import.
- Shader-zoo near-misses (Init/UpdateMeshData family, 99%+): many were the GOT-delta noise and
  are now linked after v2.1. Remainder differ by one extra literal-pool word (string/TU pooling)
  — TU-membership work (exclusive mode), not per-function fixes.

Next: wave 2 = tier-4 cheap class batches (Ship, FModSound, Wanted+WantedWindow, GameText,
LodMeshMerger, ScrollTouchWindow, BluePrint+Objective) + a retry worker on wave-1 residuals.
Re-triage from fresh report.json first (metric v2.1 moved the near-miss set).
