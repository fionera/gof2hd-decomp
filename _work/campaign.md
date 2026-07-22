# Byte-matching campaign log

Orchestrator session log. One entry per session; newest first. Resume from git log + this file.

## Session 2026-07-23 (verifier-accuracy fixes + -fstack-protector-strong landed; no fleet)

Orchestrator-only session: the wave-11 flag experiment's RATCHET FAIL (linked -31) was
root-caused to TWO verifier bugs, both fixed against green gates, then the flag landed
as a strict win. Net: linked 2316->2500 (+184), avg 74.56->76.72 (+2.16), byte 1143
stable, imports 1028, extra 36/missing 0.
- d0b8696d verify fix 1 (blx align-down): Thumb->ARM blx computes from Align(PC,4); fn
  base = 2 (mod 4) made the naive base+t sum land 2 past the PLT entry -> false '|' on
  identical code, and #X-vs-#X spurious matches when both sides missed. +23 cleared,
  -4 formerly-masked TRUE mismatches exposed: AEGeometry::getMatrix/getPosition/
  getUpVector call our AEGeomCanvas::TransformGetLocal(uint,uint) shim where orig calls
  PaintCanvas::TransformGetLocal(uint); also PaintCanvas::StopDraw2FBO. 2316->2335.
- 12d5cb12 verify fix 2 (exact site-parity blx): align-down is wrong for call sites at
  offset+4 = 2 (mod 4) (true target is base+t+2, align-down lands -4). normalize() now
  gets the insn offset and rebuilds addr = t - Align(off+4,4) + Align(base+off+4,4).
  Validated vs ListItem::init PLT 0x73e0c. 2335->2398 (+63, ListItem/CheatCode family).
- 5a76c25b verify fix 3 (pool-line collapse): masked pool words decode as 1 Thumb32 or
  2 Thumb16 lines depending on the meaningless word value -> length mismatches on
  identical code (get_crc_table, switchSkyboxForIntro). One entry per 4-byte word now.
  2398->2466 (+68, zero losses). This + fix 1 explain the whole "GOT wobble" class.
- ad147cd4 toolchain: -fstack-protector-strong (NDK default since r14; 3 workers found
  orig canaries on String/ptr-array locals). With honest verifier: 2466->2500 (+34,
  zero exact losses), avg +1.76 — biggest single move of the campaign.
LESSONS: (1) ratchet.py does NOT re-run verify when only tools/ change (report newer
than .so) — force verify.py directly after tool edits. (2) Snapshot the baseline .so
(scratchpad/baseline_libgof2hdaa.so) before flag experiments so tool fixes can be
re-scored without rebuilds. (3) A red gate on instruction-identical diffs means the
TOOL is wrong — fix it against a green gate, never to turn red green.
QUEUE for wave 12: AEGeometry AEGeomCanvas-shim conversions (newly exposed);
canary-unblocked near-misses now at 90%+ (FModSound ctors 97.5, RecordHandler
writeMission 93.3/writeWanted 92.7/writeAgent 88.1, Radar::getPlanetDockIndex 92.9,
ApplicationManager OnTouchMove 94.3); AEFile ReadSwitched/collectFilesInPakFiles;
PaintCanvas::StopDraw2FBO.

## Session 2026-07-22 (wave 11 — mid-band 35-60%: 3 dead-worker retries + new classes, 8/8 landed)

Pool: diffs11. Workflow wf_e603dd9d-2df. paintcanvas2 stalled 6/6 but left landable edits
(+1 orchestrator fix). Net: byte 1141->1143, linked 2306->2316 (+10), avg 74.34->74.56,
imports 1037->1028 (-9).
- 774d5f2 radar: getPlanetDockIndex shim->getSystem()/getStations() 47.6->89.7, dtor
  double String-dtor removed. Radar.h layout drift documented (12-56B missing mid-struct:
  players 0x28 vs 0x34, lockLabel 0x168 vs 0x18c, scannerAvailable 0x175 vs 0x1ab) —
  NEW exclusive-tier layout item.
- cda5116a recordhandler2: writeMission AEFile::Write conversions 54.2->78.9 (-4 imports).
  7 fns canary-blocked.
- 8492e38f modstation: enterStation 36.0->100 linked.
- b88cad63 aefile: JACKPOT — file-statics migrated to the REAL AEFile class statics
  (fileInterface/pakFileEntryList/file are exported .bss syms in orig; our spurious
  4th "initialized" flag dropped). 13 fns -> 95-98%. Confirms: when orig exports a
  static, our file-static replica must become the class static.
- c17b22a3 hangarwindow: getCurrentItem + getRelativeScrollHeight ->100 BYTE-exact,
  demount/mount/highlight ->90-96.
- 074caa94 explosion: canvas/rnd = &Globals::* uninit-deref fix; Status D1/D2 ->100;
  triggered the largest ICF refold storm yet (160 changed / 73 wobble-dips, net +6 linked).
- 59589542 paintcanvas2 (stalled salvage): worker used true-import glClear but our curated
  third_party/gl/GLES/gl.h lacks it — orchestrator added GLES2/gl2.h include (Engine.cpp
  precedent). ClearDepth ->100, -4 imports.
- 954ef7c1 kiplayer_asteroid: PlayerAsteroid::push +2.4 only.
STANDOUT FINDING (3 independent workers): orig canaries String/ptr-array locals that
-fstack-protector never protects => orig likely -fstack-protector-strong. ~15+ fns
blocked on this. Flag experiment queued (orchestrator-only, ratchet-gated).

## Session 2026-07-22 (wave 10 — mid-band 35-60% cont., 8 workers, 5/8 left edits, 5/5 landed)

Pool: diffs10 (35-60 band, 67 fns). Workflow wf_94a96e14-19d. 3 workers died mid-response
(radar, recordhandler2, modstation) leaving NO tree edits; the other 5 all landed.
Net: byte 1140->1141, linked 2301->2306 (+5), avg 74.25->74.34, imports 1037 (unchanged —
fmodsound worker did NOT add the 7 true FMOD imports; those fns are canary/flag-blocked).
- e70162f0 fmodsound: resume(Event*) `sys = this->system` pre-condition local forces
  mov r2,r0 / movs r0,#0 early-init ->100 linked; pause s[2] buffer sp-offset fix 35.5->41.9.
  5 fns blocked: orig has stack canaries our -fstack-protector doesn't emit (ptr-array
  locals canaried in orig => orig likely -fstack-protector-strong). Flag-blocked class.
- a07e0f66 level: render — removed cached-element locals for double-fetch `(*arr)[i]->`
  pattern + unconditional renderSunStreak tail call ->100 linked; switchSkyboxForIntro ->100
  linked (source was already right; diff was vs stale bin); attackWanted 39.5->90.2,
  supernovaReversal 40.4->64.2. createSentryGuns: clang 7.0.2 reg-allocator defect
  (this not restored to r0 after __aeabi_uidiv) — unfixable from source, documented.
- aa070847 playerfixed: getPosition ->100 BYTE-exact. PlayerFighter untouched.
- 5fb4dacf particlesprite: render 44.0->69.7 (GlowPPShader C1/C2 -1.8 wobble, untouched TU).
- 7fbbaaa6 scrolltouch: ctor C1/C2 45.2->76.8, setText String(text,false) temps +10/+5;
  draw/drawTextBG hunks REGRESSED (drawBox 6-arg overload + drawWindow temp reshape were
  both wrong) — reverted those two only; Achievements::getValue wobbler flipped ->100 linked.
Learnings: (a) double-fetch `(*arr)[i]->f(); (*arr)[i]->g();` beats cached-element locals
for -Oz reg-alloc match in render loops; (b) `T *x = this->field;` before an early-out
check forces the orig's early default-return init pattern; (c) widening a stack out-buffer
(s[1]->s[2]) relocates it to sp+0 to match `mov r1,sp` shapes.

## Session 2026-07-22 (wave 9 — mid-band 35-60% breadth, 8 workers, 6/8 landed)

Pool: diffs9 (35-60 band). Workflow wf_6f0475b8-3f7. 3 workers stalled + 1 died, but ALL
stalled workers left good tree edits — serial salvage landed every non-empty patch.
Net: byte 1138->1140, linked 2287->2301 (+14), imports 1049->1037, avg 74.02->74.25.
- e267c22d mgame: uninit-global-deref inits (g_maneuverScale=&Globals::w, g_fmod, g_gameText)
  + handleAccelerometer shipField arg. maneuverTouchMove 28.6->100, maneuverTouchEnd ->100.
  OnKeyPress/OnKeyRelease blocked: orig is bare bx lr from a longlong-returning signature.
- 3133ffa1 layout_touchbutton: drawMask spurious SetColor removed, getPosition VFP order,
  touchedInside field_0x38. 2 ctor arg-order hunks REGRESSED and were reverted (worker's
  init() arg-trace was wrong for 2 of 4 ctor families, right for none that gated better).
- 2f3f86b4 appmgr: GetKeyState/GetActionState 64-bit (keyState|high pair) — both ->100
  byte-exact. All 18 other appmgr/FileRead fns blocked (reg-alloc/scheduling walls).
- d5679d94 status (stalled-worker salvage): orbit-check family ->84-100, setCampaignMission
  45.8->76.9. missionFailed 100->93.5 + wantedBoardAccessible -16 = TU coupling (sentinel
  revert did NOT recover them; kept net).
- ea44df85 dialoguewindow (stalled-worker salvage): C1/C2()->100, dtors ->93.3,
  nextPage ->84.8, draw ->58.2.
- 1e44bf3d paintcanvas (stalled-worker salvage + 2 orchestrator fixes): DrawTextLines(4-arg)
  40->100, SpriteSystemSet* ->100, -12 imports. Fix 1: worker's new TextureCreateFromFile
  fwd-decl tripped goal_lint void_ptr (+2) — converted the remaining tcg_ shim to the real
  call and same-line-waived the mangling-required void*s. Fix 2: GetLine — worker used
  default-ctor String tmp + assignments; reshaping to direct `*out = str.SubString(...)`
  temps went 27.4->43.0 (above the 35.4 start) AND net +4 more linked TU-wide.
- hud worker: NO edits, full layout dossier instead — Hud.h wrong in 4 independent regions
  (eventScroll block +4/+6, eventQueue region +28, autofire region +44, cargoFullFlag +1,
  drawEventString margins +80). QUEUED as exclusive layout item (12 fns blocked on it).
- aefile worker died leaving nothing; paintcanvas dump had 29 fns, capped worker at 12.

Learnings:
- Stalled-worker tree edits are consistently landable — ALWAYS snapshot before killing.
- Temp-object shape matters at -Oz: `T tmp; ... tmp = f(); *out = tmp;` vs `*out = f();`
  changed GetLine by +15pct and refolded ICF across the whole TU (+7 linked).
- goal_lint void_ptr is per-file counted on unwaived lines; mangling-required void* decls
  need the same-line `// lint: void_ptr (...)` waiver, not baseline edits.
- Layout-item queue additions: Hud.h (4 regions, dossier in wave-9 workflow output),
  MGame OnKeyPress/OnKeyRelease signature (bx lr vs longlong return).

## Session 2026-07-22 (wave 8 — mid-band 35-60% + shim continuation, 6 workers, 5/6 landed)

Pool: diffs8, MIN 35 / MAX 60 pct. Workflow wf_7787ab83-218 (resumable via scriptPath).
Net: byte 1138, linked 2288->2287 (transient wobble), imports 1053->1049, avg 73.93->74.02.
- f03ef9de playerego2: +1 net linked. endExplosion -> ((Explosion*)v)->reset(), hitCamera
  -> ->hit() — both found by decoding ARM interwork VENEERS to get the real blx target.
  dockToStream: setExhaustVisible(false) with NO null check; revive: setPosition(0,0,10000).
- dc638692 engine: SetUVMatrix 52.6->95.7, ShaderSetInActive 44.4->91.7, SetWorldView
  27.3->57.6, LightSetGlobalSceneColorAmbient 0x3f800000->1.0f (43.8->63.2). ORCHESTRATOR
  TUNE: reverted the same matrix.m hunk in SetModelMatrix only (it regressed there 50.3->45.6
  while helping the other two — same edit, opposite effect per fn; tune per-fn, not per-pattern).
- 432494a6 mission: Mission::clone String(src,false) 47.2->48.9.
- 2758e357 imagefactory_players: RNG globals `= &Globals::rnd` init un-elides UB-elided
  derefs (createChar 51.9->84.8, 66.0->96.4); gCreateChar2Table scalar -> const int[44]
  from orig .rodata@0x201c24; PlayerJunk g_PJ_sound/g_PJ_random init (update 59.5->67.9).
- a74d6255 menutouch2 (DEAD-WORKER SALVAGE — API connection died, tree edits gated fine
  after one fix): startGOF2 52.6->78.6, -4 imports. Worker called rh->readRecord(slot)
  against a void* header decl -> compile fail; fixed by retyping RecordHandler
  readRecord/recordStoreRead/recordStoreReadPreview to GameRecord* (return types are NOT
  mangled — free semantic win, dropped 4 casts). loadGame 50.8->43.6: conversion cost;
  tried restoring nested branch structure, identical score -> the dip is GOT/inline shift,
  not CFG. Requeued.
- paintcanvas3 STALLED all 6 attempts (no tree edits, no output) — 29 fns still in
  diffs8/paintcanvas3.txt, requeue with smaller assignment slices.

Learnings:
- Same source edit (matrix.m) can improve one fn and regress a sibling in the same TU;
  gate hunks per-fn when regcheck disagrees with itself.
- Dead workers' tree edits are worth gating: menutouch2 landed with one orchestrator fix.
- Return types are not part of Itanium mangling: retyping void* method returns to real
  types is always parity-safe and removes casts.
- Layout-item queue additions (exclusive-mode, from worker blocked reports): PlayerEgo.h
  (10 trivial accessors on wrong offsets; resetMovement needs 24B zero at 0x268),
  Engine struct (lightDiffuseShaded 764 vs 740, lightSpecularShaded 740 vs 728).

## Session 2026-07-22 (wave 7 — shim-import convergence, 7 workers, 7/7 patches landed)

STRATEGIC FINDING that drove this wave: the original has only 207 imports; we carried 1081,
of which 893 are FABRICATED shim externs (fake decls marked "lint: void_ptr (external
symbol...)"). Every fake shim call really targets an internal game function. Conversion
recipe (now proven at scale): read the orig side's bl/blx target symbol in the diff dump,
confirm the real callee is defined in our tree, rename the call — never touch logic.
Also 19 TRUE imports still missing: 7 FMOD Event/EventSystem methods (FModSound family),
__aeabi_l2d, unwind_cpp_pr0/pr1, __gnu_ldivmod_helper, fprintf/printf/sscanf/memcpy/
memset/memalign/mkstemp/raise. orig import list cached at scratchpad/orig_imports.txt.

7 workers on the pct>=60 pool of shim-heavy TUs (diffs7). All landed (one orchestrator fix):
byte 1137->1138, linked 2271->2287 (+16), imports 1081->1053 (-28), avg 73.78->73.93.
- 521ebcdd playerego: +7 linked (addGun x2 tail-call checkForTurret — the resetGunDelay
  guess was wrong; setVisible cascades setExhaustVisible; 7 fns at orig byte count).
- dc663718 paintcanvas2: +1 byte, -15 imports. ORCHESTRATOR FIX: worker made
  paintcanvas_g_flipv a TU-global to defeat const-folding -> exported a symbol the orig
  lacks -> sodiff extra-beyond-allow FAIL. Kept it static; MeshSetUv parked until the
  real global's identity is known. Mask2D writes field_0x20 not mask2dImage@0x2c.
  Cost: 5 wave-6 sprite/transform fns flipped linked->fuzzy — sole structural diff is
  the blx ALIAS CHOICE (gold ICF fold direction shifted when new template instantiations
  entered the TU). New wobble class alongside GOT reshuffle.
- 32f354ae recordhandler: notEnoughMemory linked via (int) cast (signed it-lt vs it-cc);
  writeAgent/writeWanted +16pct each (real AEFile::Write + NRVO). StarMap blocked:
  READ-ONLY header layout drift (systemPath 148 vs 160) -> layout-item queue.
- de3e5f42 starsystem_missions: +2 linked, -5 imports (typed ArrayReleaseClasses/dtor
  calls; Transform::Update((longlong)dt,bool) ABI slot fix; Mission D1/D2 ICF-wobbled out).
- 9dd13c28 menutouch: +3 linked, -7 imports (ChoiceWindow member calls; saveGame was
  dropping the slot arg).
- c40b3c81 cutscene: +3 linked (indexed loop shape in render3D; FogMode_1 in dtor).
  SpaceLounge blocked: READ-ONLY header field drift (listVisible 0x1f vs 0x1c) -> layout.
- 00434592 hud_player: +1 linked, -1 import (touchMove real y arg; setEnemies whole-array
  ArrayAdd + gun->setEnemies). Player near-misses mostly reg-scheduling walls.

Learnings:
- Shim conversion is now the highest-yield tier and ~860 fakes remain; richest TUs still:
  PaintCanvas (~470 decls left), MenuTouchWindow (~170), PlayerEgo (~60), below-60-pct fns
  everywhere (this wave only covered fns >=60).
- Import shrink causes GOT reshuffle AND gold-ICF fold-direction flips: a handful of
  linked fns wobble out each wave with only a blx-alias structural diff; they tend to
  come back. Judge waves on NET linked, never freeze on individual flips.
- Never introduce a namespace-scope global to defeat const-folding: it exports a symbol
  and fails parity. If the orig provably has a mutable global (GOT access), find its real
  identity (likely already among our 36 extra or the orig's dynsym data) first.
- Layout-item queue grew: StarMap.h (systemPath 148 vs 160), SpaceLounge.h (listVisible
  0x1f vs 0x1c) — both need exclusive-mode header fixes with drift tooling.

Queue after wave 7: (1) shim-conversion wave 8 on the <60-pct pool of the same TUs +
next shim-heavy TUs; (2) exclusive layout items StarMap.h + SpaceLounge.h; (3) FModSound
family to pull in the 7 missing FMOD imports; (4) shader-zoo TU pooling (exclusive);
(5) monsters (Gun::shootAt, WantedWindow::draw/selectWanted, convertStringFromArabic).

## Session 2026-07-22 (wave 6 — near-miss pool >=85%, 9 workers, 7/8 patches landed)

9 disjoint workers on the pct>=85 near-miss pool (diffs6 dumps, 123 fns; shader zoo excluded
as the exclusive TU-pooling item; String D0 excluded as unfixable). All 9 returned; 8 edited
files, wanted_status found all 12 fns blocked (ARM-mode-PLT-stub / register-scheduling causes)
and correctly edited nothing. Serial salvage: snapshot patches6/, revert, per-item gate.

Landed (net: byte 1136->1137, linked 2242->2271 (+29), avg 73.73->73.79, imports 1090->1081):
- 0bd604cb paintcanvas: 9 fake paintcanvas_ext_* shim imports -> real member calls; 12 fns
  linked. ORCHESTRATOR FIX REQUIRED: worker rewrote the 6-arg DrawTextLines logic (flag==0
  right-align via p5) into a copy of the 5-arg center logic — restored orig logic + member
  calls, then BOTH overloads linked. Import shrink locked deliberately (convergence).
  Collateral: Station isAttackedByAliens/visit/isDiscovered linked->fuzzy (GOT reshuffle
  from import removal; expected to return as imports converge).
- d214f64c string: dropped redundant length(0) member-init from 7 delegating-to-Set String
  ctors (+10 linked; orig's Set() zeros data/length itself, vtable strd covers data).
- 254eb5c4 agent: +6 linked (ctor/dtor pairs, getSystemName/getStationName via 2-arg
  String(src,false)). ODDITY: getMissionString same transform sits at 83.3, but reverting it
  alone flips the two GOOD getters to 83.3 (TU fold/ordering interplay) — kept full-patch
  state (net +2 linked vs partial revert). getMissionString requeued as a single.
- 5db3c3b4 geometry: AEGeomCanvas static wrappers -> direct PaintCanvas member calls
  (+4 linked, movs r3,#0 bool materialization restored).
- 07c13122 item: transactionBlueprint branch on fabricate (r1) not mode — BYTE-EXACT (+1).
- b734c794 level_station: Station::getName linked (+1), uncoverWanted 87.5->90.
- 6d4fb288 fighter_kiplayer: no linked flips but 4 real pct gains (KIPlayer dtor 93.8,
  setMissionCrate 97.4); avg +0.01.
Reverted: ship addMod compound-loop-condition attempt (86.3->83.0, regression).

Learnings:
- Fake-shim -> real-member-call conversion is the single richest near-miss vein: it fixes
  bytes AND shrinks the import set (double gain). Grep for remaining *_ext_* shim decls.
- Import-set changes reshuffle the GOT -> previously-linked fns in UNRELATED TUs can flip
  linked->fuzzy. Not a real regression; ratchet nets it; note in commit and move on.
- Same-shape sibling getters can be codegen-coupled through the TU (Agent getters): judge
  keep/revert on NET linked count, not per-fn.
- The dominant blocked cause this wave: -Oz defers callee-saved register moves until after
  an early cbz/cmp exit, original saves them before. Not controllable from source; a large
  standing class of ~95% fns (most of Ship) is parked on this compiler-scheduling wall.
- Ship D1/D2 needs __clang_call_terminate; worker's analysis: requires a throwing dtor in
  the destruction path (Item::~Item is implicitly noexcept) — same EH-cascade family as
  AEPak/AENormal, but the trigger must come from Item's TU (cross-file, orchestrator item).

Queue after wave 6: near-miss singles left: Agent::getMissionString (83.3, coupled),
Level::uncoverWanted (90), KIPlayer dtor/setShipGroup (93+), setMissionCrate (97.4),
Wanted/StatusWindow/WantedWindow all parked (PLT-stub-mode / scheduling). Next tiers:
remaining *_ext_* fake-shim conversion sweep (import convergence), shader-zoo TU pooling
(exclusive), monsters (Gun::shootAt, WantedWindow::draw/selectWanted, convertStringFromArabic),
hard singles from wave-5 list.

## Session 2026-07-22 (wave 5 — MCP-free, 12 workers, 9/10 patches landed)

12 disjoint workers on the hard-singles/retry pool (diffs5 dumps). Ship worker "stalled on all
6 attempts" per the harness but its partial tree edits gated cleanly and were the wave's best
item. scrolltouch + objective correctly landed nothing (all-blocked with analyses). Serial
salvage: snapshot patches5/ -> revert -> apply/gate/commit per item. Landed:
- Ship (dead-worker partial): +1 byte +7 linked — ctor pair, replaceCargo, removeAllCargo,
  addCargo x2, hasCloak exact; removeEquipment 19->96, getCargo 23->96, removeCargo 34->98.
- MiningGame up/down byte-exact (void-return, same as left/right); PlayerEgo callers switched
  to the comma-expression-returning-inputY idiom (mirrors wave-4 inputX shape).
- MineGun dtor pair linked-exact, ctor 68.6->93.3 (Gun base-ctor delegation).
- Standing::getStanding byte-exact; getMissionBonus 48.5->61.3 (vmax partially cracked).
- Achievements::applyNewMedals linked-exact; countMedals 43->70.5.
- GameText::getLanguage linked-exact; setLanguage role-swap landed metric-neutral (stays 14.4).
- FModSound (orchestrator-corrected): worker's init()/getPlayingProgress wins kept but its fake
  4-arg FMOD_Event_getInfo shim replaced with the REAL member import
  FMOD::Event::getInfo(int*,char**,FMOD_EVENT_INFO*); baseline now carries the original's true
  getCategory/getProjectByIndex/Event::getInfo imports instead of 3 fake shims (imports stay
  1090). init 68.8->89.3 (real category names {master,music,sfx,voice}). 11 of the worker's 14
  edits were codegen no-ops (canary array-scope moves did NOT change -Oz codegen) or regressions
  (freeAllEvents) — dropped.
- Route::update(fff) linked-exact; AutoPilotList draw 34->51, ctor 55->68 (no flag flips).
Reverted: PlayerWormHole (measurable no-op).
End state: **byte 1136, linked 2242, stubs 0, extra 36, imports 1090, avg 73.73.**

Learnings:
- Import convergence: when a worker's "new import" is IN the original's dynamic table, the fix
  is deliberate baseline convergence (fake shim -> real symbol), not a violation. Check with
  objdump -T before rejecting.
- The array-to-outer-scope canary trick does NOT work: -Oz sinks the alloca back; the canary
  needs a real >=8B byte array. FModSound canary fns remain open.
- scrolltouch worker's blocked analysis (plausible, unverified): orig compiler reuses the stmdb
  register-save area as String temp storage + canary on struct locals — i.e. setText/draw/ctor
  family may be UNFIXABLE with plain -fstack-protector clang 7.0.2. Treat as low-priority.
- "Stalled" workflow workers can still have landed good tree edits — always snapshot + gate a
  dead worker's diff before discarding.

Queue after wave 5 (hard singles remaining): FModSound updateAll/play/isChannelActive family +
updateEvent3DAttributes (19.3), Achievements::checkForNewMedal 7.3, MiningGame render2D 18.9,
GameText setLanguage 14.4 + ReadLangFile 26.7, PlayerJunk::update 59.5, PlayerWormHole::update
43.9, Route clone/getWaypoint/dtor (EH stub), Ship addMod 86.3 + refreshValue 45.2 + getUsedSlots
etc. mid-tier, Standing isEnemy/isFriend/getMissionBonus, ScrollTouchWindow (likely blocked),
Objective achieved/setAchievedText/dtor. Monsters (orchestrator-only): Gun::shootAt (1788B,
placeholder body), WantedWindow::draw/selectWanted, GameText::convertStringFromArabic.
Exclusive: shader-zoo TU-pooling, TU merges.

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
