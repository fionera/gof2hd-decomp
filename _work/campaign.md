# Byte-matching campaign log

Orchestrator session log. One entry per session; newest first. Resume from git log + this file.

## Session 2026-07-26n (waves 62–72 — exact near misses, global reloads, and typed locals)

Net from the wave-61 checkpoint: avg **77.99->78.02**, byte
**1162->1164 (+2)**, linked **2639->2649 (+10)**, imports **601 (=)**,
verify extra **35 (=)**, sodiff allowed extras **52 (=)**, and
stub/missing/wrong_type **0/0/0**. Thirteen source passes were linked,
directly verified, ratcheted, baseline-locked, and committed serially by the
controller. Disjoint internal GPT-5.6 Sol workers at medium/low reasoning
performed the Ghidra analysis and C++ authoring; no Claude Workflow was used.
Workers did not build, gate, ratchet, update reports/docs/baselines, stash, or
commit.

Landed passes, in order:

- `4e367225` `WantedWindow::getRelativeScrollStartPos`: **0.0->100% byte
  exact**, 48/48 bytes. The literal pool at original `0xe17d4` is
  `+0.0f`; byte **1162->1163**, linked **2639->2640**.
- `3eee8384` `Level::attackWanted`: **90.0->97.5%**, 80/80 bytes.
  Logical induction now starts at zero while physical enemy access uses
  `i+1`, recovering the original `subs r1,r6,#1`; rounded counts unchanged.
- `7187929b` `JNI_OnLoad`: **95.3->100% linked**, 64/64 bytes. Declaring
  `JNIEnv *env` before assigning `g_pVM` restored the original store/load
  schedule; linked **2640->2641**.
- `02ff27de` `ParticleSystemSprite::render`: **95.0->96.7%**, 240/240
  bytes. A typed `float m[15]` declared after setup restored the matrix
  literal/load order; rounded counts unchanged.
- `15dffbfa` `ApplicationManager::SoundResume(int)`: **88.9->100%
  linked**, 26/26 bytes. The binary-authentic grouping is
  `(soundResource && soundFxEnabled) || musicEnabled`; linked
  **2641->2642**.
- `81fd9e9e` `PlayerFixedObject::moveForward`: **88.7->100% linked**,
  172/172 bytes. Replacing the UB raw buffer/fake receiver with the real
  typed `Vector` sret restored the 16-byte frame; linked **2642->2643** and
  avg rounded to **78.0**.
- `0a12b127` `GameText::isNonArabicString`: **85.4->97.6%**, 92/92
  bytes. An integer bitwise accumulator and recovered row-update source order
  align every instruction except the final truth test; rounded globals
  unchanged.
- `f1c25e08` `ObjectGun::replaceGun`: **86.4->100% linked**, 56/56
  bytes. Holding `&Globals::Canvas` and dereferencing it at both calls
  reproduces the original global-slot reloads; linked **2643->2644**.
- `98612156` `WantedWindow::WantedWindow` C1/C2: each **85.2->100%
  linked**, 244/244 bytes. The constructor uses the canvas global-slot
  address and the original post-text-height store order; linked
  **2644->2646**, avg **78.0->78.01**.
- `5d9574c5` `HangarWindow::highlightItem`: **90.0->92.5%**, 100/100
  bytes. Reversing two source assignments emits the original
  selected-item/pending-flag store order; rounded globals unchanged.
- `5b166a51` `Station::stationHasHiddenBlueprint`: **82.8->100%
  linked**, 68/68 bytes. Viewing the recovered flag array through
  `unsigned char *` restores the original any-nonzero test; linked
  **2646->2647**.
- `8d2343d2` `PlayerFixedObject::setDeadButSelectable`: **84.4->100%
  linked**, 88/88 bytes. Both AAPCS32 64-bit animation-range endpoints are
  the Transform's `animationLength`; linked **2647->2648**, avg
  **78.01->78.02**.
- `6b2278db` `Quaternion::Inverse`: **84.2->100% byte exact**, 74/74
  bytes. Spelling each component as `-inv * component` preserves reciprocal
  `s8` as the first `vnmul` operand; byte **1163->1164**, linked
  **2648->2649**.

Verified source, ABI, and compiler facts:

- The original `ApplicationManager::SoundResume(int)` intentionally permits
  the music-enabled path to call through a null `soundResource`; the unusual
  Boolean grouping is direct binary evidence and matches its siblings.
- `PlayerFixedObject::moveForward` calls `AEGeometry::getPosition` with a
  `Vector` hidden return at `sp`, then assigns it to `this+0x2c`. The old
  `char[12]` and casted fake receiver were both UB and wrong.
- Original global access frequently retains the address stored in a GOT slot,
  not the global value. `ObjectGun::replaceGun` and the WantedWindow
  constructor both require `PaintCanvas ** = &Globals::Canvas` and a fresh
  dereference per call.
- `Station::stationHasHiddenBlueprint` reads five one-byte flags and treats
  every nonzero value as found. A `bool *` view enabled strict `==1`
  normalization and was semantically wrong; an unsigned-character
  object-representation view is defined and layout-neutral.
- `Transform::animationLength` is a signed 64-bit value at `+0xf8`.
  `SetAnimationRangeInTime(long long,long long)` receives it in both the
  aligned `r2:r3` first argument and the stack-passed second argument.
- Quaternion's target operand identity is source-tree-sensitive despite
  multiplication being commutative: `-(inv*x)` and `inv*-x` both commute to
  the wrong encoding, while `-inv*x` is byte exact.
- `MaterialCreate`'s remaining mismatch is definitive constructor ownership:
  original calls weak `MaterialC2`, while current code calls a strong C1/C2
  alias from the wrong TU. It is not a legal function-body fix.

Deferred and requeue findings:

- New self-contained notes:
  `_work/reconstructions/AEMath-VectorComparisons.md`,
  `FModSound-stopAllSoundFXEvents.md`, `GameText-isNonArabicString.md`,
  `HangarWindow-highlightItem.md`, `IParticleSystem-resetEmitterVelocity.md`,
  `Level-attackWanted.md`, `Level-collideStation.md`,
  `PaintCanvas-MaterialCreate.md`, `ParticleSystemSprite-render.md`,
  `Player-ResumeEngineSound.md`, `Player-resetGunDelay.md`,
  `PlayerFighter-cloak.md`, `ScrollTouchBox-touchIsInside.md`,
  `Sparks-update.md`, and `Status-getFreighterMissionStationBit.md`.
  Do not repeat the exhausted source shapes recorded there.
- `Status::getFreighterMissionStationBit` exhausted early-check/switch,
  ordered PHI, and conditional-expression shapes. LLVM canonicalizes all
  mappings and moves station 95 to the tail; requeue only with a real
  control/data dependency.
- `FModSound::stopAllSoundFXEvents` exhausted three one/two-index loop
  lifetimes. All retain `r5` as the persistent counter; do not force
  registers.
- `Level::collideStation` requires the by-value Vector words live before both
  null guards. Landmark snapshots, a success-only reload, and a typed Vector
  copy were all scalarized/sunk.
- GameText's landed residual is only `lsls r0,r4,#31` versus `cmp r4,#0`.
  `bool` or an integer low-bit condition recovers the test but swaps the
  character/accumulator registers, so the integer ordinary-truth form is the
  best legal partial.
- HangarWindow's landed residual is only the pre-null-guard `this`/item save.
  Explicit aliases and nested guards are codegen-neutral.
- ScrollTouchBox's synthetic locals, rejection, positive conjunction, and
  nested guards all compile to the same branchy lower-bound sequence instead
  of the original conditional `ldrle/cmple` IT block.
- Directly screened scheduler-only skips remain:
  `SpriteSystemRelease`, `AEGeometry::setLodChildTransform`,
  `MenuTouchWindow::setSkipButtonVisible`, `ParticleSystemMesh::render`,
  `PlayerFighter::collide`, and `Player::addGun`.

Exclusive work remains parked:

- `TextureConference::Init` stores correct results at wrong member offsets.
  `Hud::touchMove`, `PlayerEgo::setTurretPosition`, and
  `ListItemWindow::render` likewise expose verified field-offset drift.
- `Engine::ShaderSetInActive` has the wrong shader container pointer/Array
  level. `Engine::SetUVMatrix` has a suspicious global-indirection mismatch.
- JNI `setEnvironmentVariables` appears to map env/class through swapped
  global slots and must be audited as a cross-cutting global-provenance pass.
- Material constructor ownership belongs to original PaintCanvas.cpp, while
  the current out-of-line definition is in Material.cpp. Drain the fleet and
  gate all construction sites/dynsym parity before changing it.
- All previously recorded layout and TU-merge items remain exclusive; never
  combine them with an ordinary worker wave.

Tier position: tier 1 is exhausted (`stub_zero_size 0`). The fresh tier-2
sweep found no remaining cheap provable wrong-callee family; imports remain
**601**. Tier 3 same-size near misses remains active. Strong fresh candidates
not yet attempted are `HackingGame::solvableInNSteps` and
`ModStation::OnResume`; the JNI global mapping and all listed field/TU work
are exclusive. Current committed metrics are **4523 compared, avg 78.02%,
byte_exact 1164, linked_exact 2649, verify extra 35,
missing/wrong_type/stub_zero_size 0/0/0, imports 601**. No workers or stashes
are in flight. The worktree is clean except the pre-existing untracked
`.claude/`, which was not touched. Run only one orchestrator.

## Session 2026-07-26m (waves 58–61 — exact near misses and real virtual calls)

Net from the wave-57 checkpoint: avg **77.97->77.99**, byte
**1161->1162 (+1)**, linked **2630->2639 (+9)**, imports **601 (=)**,
verify extra **35 (=)**, sodiff allowed extras **52 (=)**, and
stub/missing/wrong_type **0/0/0**. Nine passes were directly verified,
ratcheted, baseline-locked, and committed serially by the controller.
Disjoint internal GPT-5.6 Sol workers at medium/low reasoning performed the
Ghidra analysis and C++ authoring; no Claude Workflow was used. Workers did
not build, gate, ratchet, update shared reports/docs/baselines, stash, or
commit.

Landed passes, in order:

- `96a18cfc` `ApplicationManager::OnTouchMove`: **94.3->97.7%** at
  104/104 bytes. Direct x/y formal parameters restored the independent
  scalar spills and the original 44 decoded instructions; rounded globals
  stayed at avg **77.97**, byte **1161**, linked **2630**, imports **601**.
- `7c811aa5` `Achievements::setMedals`: **90.9->100% linked** at
  56/56 bytes. A separate signed fill induction variable restored
  `cmp #45; blt`; byte **1161->1162** and linked **2630->2631**.
- `a41efb0d` `Mission::setTargetStation`: **90.0->100% linked** at
  104/104 bytes; linked **2631->2632**. Chaining the station lookup into
  the name assignment restored the canary/Galaxy GOT register allocation.
- `a273fa71` `RadioMessage::RadioMessage(int,int,int,int)` C1/C2:
  each **89.5->100% linked** at 48/48 bytes. Allocating and initializing
  through `targetIndices` restored the exact tail for both aliases; avg
  **77.97->77.98**, linked **2632->2634**.
- `10dea043` `LensFlare::~LensFlare` D1/D2: each **90.0->100% linked**
  at 22/22 bytes; linked **2634->2636**. The original calls scalar
  `_ZdlPv`, not array `_ZdaPv`, despite the constructor's array allocation.
- `846803b8` `PlayerAsteroid::~PlayerAsteroid` D1/D2: each
  **94.7->100% linked** at 52/52 bytes; linked **2636->2638**. Moving the
  `explosion = nullptr` store to the common tail repaired both aliases.
- `91b48b90`
  `HangarWindow::refreshCargoAvailabilityForBlueprints`:
  **93.0->98.6%** at 188/188 bytes. Snapshotting only the counters Array
  while reloading its data pointer per ingredient restored the loop body;
  rounded global counts were unchanged.
- `c569efc3` `LevelScript::resetCamera`: **90.7->100% linked** at
  124/124 bytes. A camera snapshot used only by `setTarget` keeps
  `m_pCamera +0x14` live across the second `Level::getPlayer`, while the
  two offset calls retain their original member reloads; avg
  **77.98->77.99**, linked **2638->2639**.
- `33734a5e` `PlayerStation::outerCollide(float,float,float)`:
  **90.9->92.4%** at 196/196 bytes. The loop now calls the verified
  `BoundingVolume::outerCollide` vtable slot **+0x0c**, not `collide` at
  **+0x08**; rounded global counts were unchanged.

Verified source, ABI, and compiler facts:

- RadioMessage fields `targetCount/targetIndices` are the adjacent
  **+0x18/+0x1c** pair; initializing through the member lets `-Oz` defer
  and coalesce their stores after the allocated element write.
- BoundingVolume address-point slots in the original are
  `getCollisionNormal/update/collide/outerCollide/project` at
  **+0x00/+0x04/+0x08/+0x0c/+0x10**. PlayerStation's float overload
  genuinely uses `outerCollide`.
- `LevelScript::m_pCamera` is **+0x14** and `PlayerEgo::geometry` is
  **+0x08**. The original snapshots the camera for only the first camera
  call and reloads it for both Vector-offset calls.
- LensFlare's original scalar-delete call is intentional binary evidence;
  do not “correct” it back to `delete[]` without a new target.

Deferred and requeue findings:

- New self-contained notes:
  `_work/reconstructions/ApplicationManager-OnTouchMove.md`,
  `Station-hasItem-hasShip.md`, `FileInterfaceAndroid-Seek.md`,
  `HackingGame-reInit.md`,
  `HangarWindow-refreshCargoAvailabilityForBlueprints.md`, and
  `PlayerStation-outerCollide.md`.
- `ApplicationManager::OnTouchMove` retains only the early
  `mov r8,touch` schedule. `Station::hasItem/hasShip` retain only the
  pre-null-guard `this` copies at **96.0%, 58/58**. `FileInterfaceAndroid::Seek`
  retains two pre-guard preservation copies at **93.9%, 80/80**. Natural
  aliases and guard variants were neutral; requeue only with authentic
  null-edge liveness.
- `HackingGame::reInit` remains **89.8%, 420/420**. Signed, unsigned, and
  post-decrement shuffle forms all canonicalized to the current negative
  trip counter; the later solver-call zero-register schedule remains tied
  to it.
- HangarWindow's landed body has only a three-instruction MachineSink
  schedule around the cargo null guard. A direct chained counter expression
  regressed the register plan and was rejected.
- PlayerStation's correct-callee body still eagerly preserves x/y/z in the
  original but sinks those copies across the AABB guards now. Two retries
  found no same-version source evidence for forcing those lifetimes.
- ImageFactory's checked residuals remain as previously documented:
  `createChar(bool,int)` has correct GOT/table relocation identities, and
  `loadChar(int*)` is a one-move MachineSink case.

Tier position: tier 1 remains exhausted (`stub_zero_size 0`). No fresh,
provable wrong-callee family was found in the tier-2 sweep; **601** imports
remain pinned, with larger PaintCanvas/MenuTouch/Hangar bodies requiring
dedicated work. Tier 3 same-size near misses remains active. Exclusive
layout/TU items from prior sessions remain parked; drain the fleet before
touching them. Current committed metrics are **4523 compared, avg 77.99%,
byte_exact 1162, linked_exact 2639, verify extra 35, missing/wrong_type/
stub_zero_size 0/0/0, imports 601**. No workers or stashes are in flight.
The worktree is clean except the pre-existing untracked `.claude/`, which
was not touched. Run only one orchestrator.

## Session 2026-07-26l (waves 55–57 — jump scenes, de-shims, and exact near misses)

Net from the wave-54 checkpoint: avg **77.90->77.97**, byte
**1161 (=)**, linked **2624->2630 (+6)**, imports **612->601 (-11)**,
verify extra **35 (=)**, sodiff allowed extras **52 (=)**, and
stub/missing/wrong_type **0/0/0**. Six passes were linked, directly
verified, ratcheted, baseline-locked, and committed serially by the
controller. Disjoint internal GPT-5.6 Sol workers at medium/low reasoning
performed the Ghidra analysis and C++ authoring; no Claude Workflow was used.
Workers did not build, gate, ratchet, update shared reports/docs/baselines, or
commit.

Landed passes, in order:

- `c9a078db` ParticleSystemManager constructor/render batch: both constructor
  aliases became **100% linked**, linked **2624->2628**, avg
  **77.90->77.93**, imports **612->607**. Five fake imports became the real
  `initSprites` and ParticleSystem mesh/sprite render overloads.
- `2fead9cc` `Layout::reload`: **52.6->61.5%**,
  896 -> 952 bytes against original 1064; avg **77.93->77.94**, imports
  **607->604**. Three invented TouchButton constructor imports became the
  verified real overloads.
- `59b3c283` MGame jump-scene batch:
  `startJumpScene` **54.6->83.0%** (832 -> 904, original 896) and
  `updateJumpScene` **41.0->76.5%** (680 -> 912, original 920);
  avg **77.94->77.95**, imports **604->602**. The latter is conclusively a
  `bool` function; the real 64-bit countdown, completion paths, camera,
  Vector, station-global, and sound calls were restored.
- `7089a8bf` `ListItemWindow::render`: **35.9->96.5%**,
  204 -> 172 bytes at the original 172-byte size; avg **77.95->77.96**,
  imports **602->601**. The fake tail is the real
  `PaintCanvas::DisableClip()` call.
- `ead56794` `ParticleSystemSprite::reset`: **94.3->100% linked** at
  92/92 bytes; linked **2628->2629**. Reversing two independent source
  assignments recovered the original final store order.
- `bf45e779` `Station::clone`: **93.3->100% linked** at 116/116 bytes;
  linked **2629->2630**, avg **77.96->77.97**. The by-value name is
  `String(name, false)`.

Verified source and ABI facts:

- `MGame::updateJumpScene` returns true only after either completed
  transition. The original caller tests `r0`, the J2ME mapping is `()Z`,
  and the callee establishes `r5=0/1` before returning it. The comparison
  `1700LL - currentTime < 0` produces the original subtract-with-borrow.
- `ListItemWindow::render` uses `SetColor(0xffffffff)`,
  `CameraGetLocal(this->camera)`, the Matrix assignment, `End3d`, and
  `DisableClip`. A long-lived `PaintCanvas **` holder reproduces the original
  register set and removes the fake canary frame.
- `Layout::reload` allocates 200-byte TouchButtons and calls the real String,
  image, and extended image constructors. Its remaining 84-byte original
  spill frame is fully mapped in `_work/reconstructions/Layout-reload.md`.
- The MGame jump-scene literals and paths are recorded in
  `_work/reconstructions/MGame-jump-scenes.md`; notably FOVs 300000/450000,
  post effect `0x01400002`, landmark index 1, direction scale 3000, and
  application module 2.

Deferred and requeue findings:

- New self-contained notes:
  `_work/reconstructions/SpaceLounge-OnRender3D.md`,
  `Sprite-setFrame.md`, `AEGeometry-setLodChildMeshes.md`, and
  `Station-setAgents.md`. Their body-only legal forms were exhausted or
  disproved; do not repeat them.
- `SpaceLounge::OnRender3D` is an exclusive header-layout item.
  `listVisible` is proven at original `+0x1c` across constructor, key, draw,
  update, and touch paths, while the current header places it at `+0x1f`.
  Testing `chatActive` merely to obtain `+0x1c` is forbidden.
- ListItemWindow's remaining `render` and `OnTouchBegin` markers share the
  exclusive rectangle-layout blocker: original x/y/width
  `+0x64/+0x68/+0x6c`, current `+0x5c/+0x60/+0x64`.
- Sprite's two residual `mul.w` operand encodings are backend-only after
  three natural source shapes. AEGeometry's two argument/self moves remain
  MachineSink scheduling at the positive-count guard. Station::setAgents
  likewise remains a one-move MachineSink residual; the only form that moved
  it grew 36 -> 42 bytes and was reverted.

Tier position: tier 1 remains exhausted (`stub_zero_size 0`). Tier 2 remains
active with **601** pinned imports; the remaining cheap wrong-callee sites
are increasingly concentrated in monster PaintCanvas/MenuTouch/Hangar
functions and must not be farmed wholesale. Tier 3 same-size near misses
remains active. Exclusive layout items now include SpaceLounge and
ListItemWindow; drain the fleet before either header pass. No workers or
stashes are in flight. The worktree is clean except the pre-existing
untracked `.claude/`, which was not touched. Run only one orchestrator.

## Session 2026-07-26k (waves 53–54 — shader recovery and wrong-callee sweep)

Net from the wave-52 checkpoint: avg **77.85->77.90**, byte
**1161 (=)**, linked **2622->2624 (+2)**, imports **619->612 (-7)**,
verify extra **35 (=)**, sodiff allowed extras **52 (=)**,
stub/missing/wrong_type **0/0/0**, parity clean. Seven passes were linked,
directly verified, ratcheted, baseline-locked, and committed serially by the
controller. Disjoint internal GPT-5.6 Sol workers at medium reasoning did the
Ghidra analysis and C++ authoring; workers did not build, gate, ratchet,
change shared reports/baselines/docs, or commit.

Landed passes, in order:

- `df529f2e` `BumpShaderParticle::Init`: **97.0->100% linked**,
  388/388 bytes; linked **2622->2623**. Restored six attributes, thirteen
  uniforms, the sibling field remap, and both exact inline GLSL blobs.
- `cf1fdbd1` `ListItemWindow::update`: **45.7->77.0%**,
  352/300 -> 352/352 bytes; avg **77.85->77.86**, imports **619->618**.
  Restored real Matrix-by-value rotation/scaling calls and addon transform.
- `5807863a` Explosion class batch:
  `render` **41.2->63.3%** (388/424 -> 388/368),
  `start(Vector,Vector)` **48.9->49.8%** (396/384 -> 396/380), and
  `update(int,TargetFollowCamera*)` retained **55.4%** at 440/440 with its
  real callee; imports **618->614**.
- `ffd8b928` CutScene batch:
  `initialize` **27.5->47.7%** (1088/936 -> 1088/1052) and
  `replacePlayerShip` **59.2->82.2%** (288/276 -> 288/264);
  avg **77.86->77.87**.
- `3ebb4319` MGame small wrong-callee batch:
  `OnResume` **34.3->100% linked** at 52/52 and
  `OnSuspend` **76.9->88.4%** (192/176 -> 192/196);
  linked **2623->2624**, avg **77.87->77.89**, imports **614->612**.
- `707856b7` `RocketGun::seekEnemy`: **40.2->73.2%**,
  332/356 -> 332/340 bytes, with the exact original 44-byte frame and
  three physical Vector slots.
- `dfc9a8d4` `MovingStars::update`: **22.1->49.5%**,
  1356/1068 -> 1356/1352 bytes; avg **77.89->77.90**. Removed all six
  invented integer-to-float calls in the method and restored the transform
  and respawn logic.

High-value verified facts:

- BumpShaderParticle's original interface is six attributes plus thirteen
  uniforms. The linked vertex blob is 1223 bytes with SHA-256
  `b3111eac32e811247b3a993fbd7af5755e3b0e81a501c6f69645061dfc0807e9`;
  the fragment blob is 1181 bytes with SHA-256
  `89dc30ae67ef2d4c5fc79861e53691b2cb6db643fef90c35655206c785bb6391`.
- `ListItemWindow::update` uses `shipTransform @ +0x8c`,
  `addonTransform @ +0x90`, and rotation `(0, previewAngle, 0)`. Its
  `OnTouchBegin` residual is an exclusive layout problem: original
  x/y/width are `+0x64/+0x68/+0x6c`, current named fields
  `+0x5c/+0x60/+0x64`.
- Explosion uses the real reference-return Matrix operations and
  `AEMath::VectorLength`. Keeping `PaintCanvas**` improved register
  allocation, but `start(Vector,Vector)` still places the Matrix hidden
  return at `sp+12` instead of original `sp+8`; two retries were exhausted.
- CutScene mode 2 uses perspective `0.92, 200, 200000`, random bounds
  20000/60000, rotation `-pi/4`, and enemy Y values -30/+50. Mode 23 uses
  `0.8, 200, 100000`, rotation `+pi/8`, and translation 300/700/3000.
  Mode 4 uses `1.2, 200, 48000`.
- `MGame::OnResume` is the real
  `FModSound::tryToStopMusicForBGMusic()` / `setVolume(1,float)` path;
  `OnSuspend` begins with `Globals::recordHandler->saveOptions()` and
  tail-calls `Hud::releaseAllKeys()`.
- `RocketGun::seekEnemy` calls exported `AEMath::VectorNormalize` three
  times. Original stack slots are enemy position `sp+4`, target/final
  velocity `sp+16`, normalized intermediates `sp+28`, and canary `sp+40`.
- MovingStars literal-pool constants are spawn base -500, depth 4500,
  bounds -70/500/-500/70, speed/velocity 1000, respawn depth 20000, and
  random bounds 20000/18000/500. Its remaining 168-vs-160 frame difference
  is backend stack-object ordering after two legal retries.

Deferred and requeue findings:

- New self-contained notes:
  `_work/reconstructions/ListItemWindow-OnTouchBegin.md`,
  `MGame-OnSuspend.md`, `RocketGun-seekEnemy.md`, and
  `MovingStars-update.md`. Do not repeat their exhausted body-only shapes.
- Requeue ListItemWindow only as a complete exclusive header-layout pass;
  blind padding before x/y/width would shift already verified later fields.
- Requeue `MGame::OnSuspend` only with exclusive reconciliation of
  `sizeof(MenuTouchWindow)` (original 0x240, current 0x23c) and the
  `PlayerEgo::isDead()` return declaration (original bool pass-through,
  current int-to-bool normalization).
- Requeue RocketGun's target-selection residual only in an exclusive
  KIPlayer layout pass: original enemy index is `+0x38`, current named
  `field_0x34` is `+0x34`. The 1908-byte `RocketGun::update` still owns
  three invented `VectorRotateToTarget` calls and should be decomposed by
  the controller rather than assigned as an ordinary worker item.
- MovingStars' remaining eight frame bytes are allocation order, not a
  missing second Matrix. Forcing placement with volatility, placement-new,
  or artificial uses is rejected.
- Likely next disjoint tier-2 candidates after fresh direct diffs:
  the MGame `startJumpScene`/`updateJumpScene` fake `FModSound_setProp`
  family, `Layout::reload` TouchButton constructor shims, and cautious
  per-function ParticleSystemManager cleanup. Keep monster methods excluded.

Tier position: tier 1 remains exhausted (`stub_zero_size 0`). Tier 2 remains
active with **612** pinned imports; tier 3 same-size near misses is also
active. No workers or stashes are in flight. The worktree is clean except
the pre-existing untracked `.claude/`, which was not touched. Resume by
re-triaging current fake imports and direct diffs. Run only one orchestrator.

## Session 2026-07-26j (waves 35–52 — de-shims, scheduling, and layout recovery)

Net from the wave-34 checkpoint: avg **77.64->77.85**, byte
**1160->1161 (+1)**, linked **2587->2622 (+35)**, imports
**653->619 (-34)**, verify extra **35 (=)**, sodiff allowed extras **52 (=)**,
stub/missing/wrong_type **0/0/0**, parity clean. All 46 landed passes were
independently linked, directly verified, ratcheted, baseline-locked, and
committed by the controller. The fleet used disjoint internal GPT-5.6 Sol
workers at medium/low reasoning; workers remained read-only and never built,
verified, ratcheted, changed baselines, or committed.

Landed passes, in order:

- Early exact and typed-call sweep:
  `22a17955` MenuTouchWindow::saveGame,
  `cbf3e938` Gun D1/D2,
  `7f5000c4` PlayerEgo::isDockedToMiningPlant,
  `54fc27ad` PlayerFixedObject::projectCollisionOnSurface,
  `137c2067` PlayerFixedObject::getProjectionVector,
  `736a7131` PaintCanvas::MeshCreate,
  `6a0da8b6` MissionsWindow C1/C2,
  `edcf13fc` PaintCanvas SetTexture/GetWidth/GetHeight,
  `c68cf5b0` MissionsWindow::OnTouchEnd,
  `7cba60f2` Player position family,
  `f73baa67` Layout::getPulseValue,
  `2146d23d` SpaceLounge::update, and
  `a778c7ca` PaintCanvas Begin2d/SetProjOrthoMatrix.
- Typed-return, random, and engine-call reconstruction:
  `be078782` IParticleSystem::calcEmitterVelocity,
  `54ede9ce` PlayerFighter::cloak,
  `b416027c` PlayerEgo::stopShooting,
  `97c6a501` MiningGame::render2D,
  `709d1ac4` ModStation::OnKeyPress,
  `d307159f` PlayerFighter::initPush,
  `84dd3a04` PlayerFighter C1/C2,
  `8d78e0bb` IParticleSystem::emit,
  `d1e8aaab` IParticleSystem::resetEmitterVelocity,
  `cf31ce9b` PaintCanvas::Vibrate,
  `fbad102b` AEGeometry::setDirection,
  `3301f9f5` TouchButton::init ABI meanings,
  `9f745633` PaintCanvas::DisableClip,
  `7c671754` TouchButton achievement tables,
  `f1e11e79` AEGeometry::moveForward,
  `7f072a35` Mission constructors,
  `b2d8d3b7` ObjectGun::update, and
  `721a29e7` FBOContainer::Release.
- Exclusive and literal/source-order wins:
  `e60cb9b7` MGame natural alignment,
  `e7914d3f` HangarWindow::demountItem,
  `c2b08469` Ship::clone,
  `06e182b4` Ship::adjustPrice,
  `e5e40246` Trail::changeType, and
  `75eb2f50` BoundingSphere layout.
- Final near-miss waves:
  `8e61308c` GameText C1/C2,
  `72fde514` setBaughtCredits,
  `651032e3` TouchButton C1/C2,
  `815e6ac7` both Route overloads and four C1/C2 aliases,
  `c4dba4c9` Ship::setCargo,
  `301fab33` Waypoint C1/C2,
  `06f3bf93` PaintCanvas::GetReverseString,
  `c8311c49` PaintCanvas real Mesh shader-animation overload, and
  `f0b3daaf` PlayerEgo D1/D2 reachable cleanup.

High-value verified facts:

- Direct global/member expressions recover pool scheduling. GameText must
  assign `GameText::currentLang`, not an invented volatile pointer; TouchButton
  must load `Globals::font` before `Globals::Canvas`.
- Store scheduling often runs opposite source order at `-Oz`. Reversing
  independent assignments made Mission C1/C2, both Route overloads, and
  Ship::setCargo exact. Route's `loop @ +0x04` is a real `bool`; the type
  change is layout-neutral.
- Waypoint writes only the low byte of inherited `KIPlayer::hasCargo @ +0x4c`.
  The established `reinterpret_cast<uint8_t &>` overlay preserved
  `sizeof(KIPlayer)==0x124`, `sizeof(Waypoint)==0x138`, and made both aliases
  exact.
- Literal pools proved Ship::clone's handling multiplier is **100.0f** and
  adjustPrice uses **0.0f**, **-0.01f**, and **+0.01f**. Trail's original
  source/pool clause order is red, green, default, case 5, yellow, case 8.
- MGame is naturally aligned without packing; its verified offsets and
  `sizeof==496` remain unchanged. BoundingSphere has real inheritance, radius
  at **+0x38**, three typed Vector regions, and `sizeof==0x48`.
- Normal `delete easeMatrix` removes PlayerEgo's redundant `+0x358` reload.
  D1/D2 are now **99.5%** and every reachable instruction is structural or
  link-only. The remaining four bytes are an unreachable catch/terminate
  landing call; attempts to recover it from exception specifications are
  documented rather than faked.
- `setBaughtCredits` now maps 100k/300k/1m/3m/10m to packs 1/2/3/4/5.
  The original endpoint bug in the reconstruction was semantic, not only a
  literal-pool ordering issue.

Deferred and requeue findings:

- Self-contained notes were added or expanded for StatusWindow, CheatHandler,
  ImageFactory, AEMath::MatrixGetInverse, PaintCanvas::CameraCreate and other
  near misses, the MGame sound loops, Ship MachineSink methods, TouchSlider,
  PlayerEgo D1/D2, and PaintCanvas residuals. Do not repeat their exhausted
  source shapes.
- MatrixGetInverse, CameraCreate, and MaterialCreate are C2/C1 ownership
  blockers. Original constructors are weak C2-only; current constructors are
  strong out-of-line C1/C2 aliases. Matrix also has different default values:
  original ones at indices 0/5/10/13/14 versus current 0/5/12/14. Requeue
  Matrix, Camera, and Material only as drained, atomic header/source ownership
  passes with dynamic-symbol parity.
- BumpShaderParticle::Init is a high-confidence exclusive pass. Original
  layout is six attributes plus thirteen uniforms, not five plus fourteen,
  and `ES2LoadProgram` receives two verified inline GLSL blobs. Exact offsets,
  literals, hashes, and the TU-wide field remap are in
  `_work/reconstructions/BumpShaderParticle-Init.md`.
- MachineSink walls now include the six Ship nullable-array helpers, MGame
  pause/resume sound loops, PaintCanvas TransformRemoveMesh and
  TransformGetTriCount, the remaining prefix of MeshChangeShaderAnimValue,
  ImageFactory::loadChar, CheatHandler::Update, and previously documented
  functions. TouchSlider::OnTouchMove is signed-expression reassociation, not
  a field-type bug.
- StatusWindow::update exhausted four legal source attempts. Optimized IR
  retains the recovered store order, but SelectionDAG reverses it;
  diagnostic-only `-mllvm -pre-RA-sched=source` proves the backend lever and
  is forbidden.
- CheatHandler remains an exclusive authoritative TU merge with CheatCode.
  ImageFactory::createChar(bool,int) remains a verifier delayed-PIC
  normalization limitation. Matrix/Camera/Material ownership and
  BumpShaderParticle layout work must not overlap a worker wave.
- The ModStation typed OnUpdate batch and non-Vibrate/non-DisableClip
  PaintCanvas tail batch were rejected after local or transient regressions;
  gate those siblings individually. StarMap's previously documented
  path/pathFinder and missionChangedFlag layout blockers remain queued.

Tier position: tier 1 remains exhausted (`stub_zero_size 0`). Tier 2 remains
active with **619** pinned imports; tier 3 same-size near misses is also
active. No workers are in flight. Resume by re-triaging current imports and
near misses, or drain the fleet for the fully specified BumpShaderParticle
exclusive pass. Run only one orchestrator.

## Session 2026-07-25i (waves 18–34 — typed calls, ABI repairs, and near misses)

Net from the wave-17 checkpoint: avg **77.48->77.64**, byte
**1159->1160 (+1)**, linked **2566->2587 (+21)**, imports
**684->653 (-31)**, verify extra **35 (=)**, sodiff allowed extras **52 (=)**,
stub/missing/wrong_type **0/0/0**, parity clean. All 31 landed source passes
were independently linked, directly verified, ratcheted, baseline-locked, and
committed by the controller. The fleet used disjoint internal GPT-5.6 Sol
workers at medium/low reasoning; workers never built, gated, ratcheted, or
committed.

Landed passes, in order:

- Wrong-callee/typed-export sweep:
  `0fc204cc` KIPlayer engine-sound calls, `492c1168` ResourceTexture String
  construction, `7bf8c8e8` LODManager camera calls, `aa337cd7` AMeshMerger
  canvas call, `ebcdea13` PlayerAsteroid transform call, `76d91335` Trail
  canvas calls, `d96167ba` MovingStars velocity conversion, `b650f038`
  HangarList BluePrint call, `6a8c9f26` StarSystem position call,
  `6f559390` Transform four-argument frustum call, `5922fa5d` PlayerGasCloud
  normalize call, `6fc88b7c` MeshMerger canvas call, `323ebc2e` PaintCanvas
  real GLES/Engine clears, `ca80a343` HangarWindow BluePrint call,
  `d383df1e` PlayerCreature base call, `648b6c0d` BeamGun canvas call, and
  `b78d01c0` ParticleSystemManager typed operations. Together with the
  adjacent AEGeometry reconstruction below, this phase reduced imports
  **684->662** and added two linked-exact functions.
- Reconstruction/source-shape passes:
  `d8cd8374` AEGeometry::setPosition became linked-exact;
  `700d023b` ModStation::OnRelease became linked-exact;
  `f287777c` ScrollTouchBox::OnTouchEnd added the sole new byte-exact match;
  `1f59a6f5` ParticleSystemMesh::reset became linked-exact;
  `dcfd55ac` Transform::Update reconstructed timing/bounds and became
  linked-exact; `1afe080f` String::ToUpperCase restored its receiver return
  and became linked-exact.
- `75e0df1b` PlayerEgo::shake used the binary literal-pool divisor
  **40000.0f**, becoming linked-exact.
- `76a02b61` AEGeometry typed matrix rotation/scaling calls made setRotation,
  rotate, and setScaling linked-exact; linked **2574->2577**, imports
  **662->660**.
- `95d3b8ee` ParticleSystemManager reconstructed add/update/init/render paths:
  six methods became linked-exact; linked **2577->2583**, avg
  **77.56->77.63**, imports **660->655**. Its backing flags at
  **+0x38/+0x60** are real `bool` fields; the type correction preserved all
  offsets and class size.
- `2392578f` ImageFactory::createChar(int) restored the unmangled `int *`
  return ABI and became linked-exact; `39fe8859` Hud::touchEnd restored the
  accumulator/index initialization order and became linked-exact.
- `3f91c964` Mission::visible and isVisible changed from `uint8_t` to `bool`
  at the unchanged **+0x74** offset (Mission size **120**), repairing
  RecordHandler::writeMission **98.8->99.4** without a rounded global change.
- `bc58bea8` SimpleRefractionShader::UpdateMeshData locally interpreted
  framebuffer dimensions as signed 32-bit values; **98.9->100% linked**.
- `209d8be0` PaintCanvas::GetScreenPosition restored the real Matrix-overload
  call and its unmangled `int` return ABI. The Vector wrapper became
  **97.6->100% linked**, the Matrix body improved **15.9->24.0**, and the two
  obsolete shim imports disappeared (**655->653**).

Verified source/ABI facts:

- Unmangled return types are a recurring high-value repair: String::ToUpperCase,
  ImageFactory::createChar(int), Mission::isVisible, and the PaintCanvas
  Matrix overload all required the binary return ABI, not casts around a wrong
  declaration.
- Clang `-Oz` may emit adjacent stores or zero initializations in the reverse
  of source order. The Hud and ParticleSystemMesh matches came from changing
  source declaration/store order, not adding barriers.
- The PaintCanvas Matrix overload genuinely consumes its Matrix reference:
  original ABI is hidden Vector sret in `r0`, Matrix in `r1`, source Vector in
  `r2`; every failure returns 0 through one result register and only the final
  in-bounds path returns 1.
- Literal-pool constants remain authoritative. The shake divisor and
  ScrollTouchBox damping value were derived from the Android binary, not the
  v1 logic oracle.

Deferred/requeue findings:

- Self-contained notes now exist for Item::combineItems (**99.2**),
  StatusWindow::update (**98.5**), Ship::setMods (**98.4**),
  Sparks::explode(int,int,int) (**97.8**), Ship::removeCargo(int,int)
  (**97.9**), and AbyssEngine::TransformRelease (**97.1**). These are
  scheduling-only after their legal local forms were exhausted; do not repeat
  the attempts recorded in `_work/reconstructions/`.
- String::ReplaceString (**99.1**), RecordHandler::writeAgent (**99.6**),
  MGame::dialogueEvent (**98.6**), PlayerFighter::setMissionCrate (**97.3**),
  ImageFactory::loadChar (**97.6**), and ImageFactory::createChar(bool,int)
  (**98.1**) retain MachineSink or relocated-pool-only residuals.
- CheatHandler remains an **exclusive TU-membership merge**: the original
  CheatCode.cpp owns both CheatCode and CheatHandler, causing original C2/D2
  calls where the split reconstruction emits C1/D1. Do not direct-call
  constructors/destructors; merge per `tu_mapping.md`, drain the fleet, and
  gate byte neutrality.
- PaintCanvas's 604-byte Matrix GetScreenPosition body is still only
  **24.0%** and retains several `gsp2` reconstruction shims. The real initial
  MatrixTransformVector and return paths are now established; continue it as
  a dedicated same-class reconstruction, never by reverting the typed wrapper.
  Its gravity multiplier pool literal is verified as
  **1.5707963705062866**; the current placeholder
  `g_gsp2_gravscale_8bfa8 = 0` remains to be replaced in that dedicated pass.
- StarMap layout blockers from the previous checkpoint remain exclusive:
  path/pathFinder original offsets **0xa0/0x50** versus current
  **0x94/0x44**, and missionChangedFlag original **0xdc** versus current
  **0xd8**.

All campaign-owned wave-20 through wave-33 stash candidates have now been
individually accepted, rejected, or documented; no source result exists only
in a stash. Tier 1 remains exhausted (`stub_zero_size 0`). Tier 2 remains
active with **653** pinned imports, followed by tier-3 same-size near misses.
No workers are in flight. Resume by re-triaging current imports/near misses;
perform the CheatHandler TU merge or StarMap layout work only under an
exclusive fleet drain.

## Session 2026-07-25h (wave 17 — GPT-5.6 Sol wrong-callee sweep)

Net from the wave-16D checkpoint: avg **77.34->77.48**, byte **1159 (=)**, linked
**2557->2566 (+9)**, imports **725->684 (-41)**, verify extra **36->35**, sodiff allowed
extras **53->52**, stub/missing/wrong_type **0/0/0**, parity clean, every landed class
RATCHET PASS + baseline locked. Goal lint: operator_call **431->415**, void_ptr **37->34**.
The fleet used three disjoint internal GPT-5.6 Sol workers (medium/low); only the controller
built, verified, ratcheted, updated baselines, and committed.

Landed passes, in order:

- `f5ef7408` MenuTouchWindow setup cleanup already present at session start:
  startValkyrie **47.5->84.1**, startSupernova **42.5->82.7**, imports **725->714**.
- `89069202` MenuTouchWindow real Galaxy/Array calls: loadPreviewRecords **86.9->88.5**,
  startValkyrie **84.1->84.5**, startSupernova **82.7->83.1**; imports **714->712**.
- `ccbfdc68` PaintCanvas typed matrix/perspective/mesh calls: CameraGetLocal,
  TransformGetLocal, and MeshCreate became **100% linked**; perspective overloads
  **85.4->92.7** and **84.6->92.3**; linked **2557->2560**, imports **712->705**.
- `4b525a86` ModStation class de-shim: OnResume **75.7->81.0**, OnRelease
  **73.5->99.4**, showCBSMessage **33.8->92.9**, checkPendingProducts **19.2->23.3**,
  C2 **31.8->32.2**, showDlcMenu neutral; imports **705->697**, verify extra
  **36->35**.
- `eec7e73c` ObjectGun typed canvas calls (narrowed after gating): C2 **35.1->44.3**,
  replaceGun **34.3->86.4**, render **21.0->22.7**; imports **697->692**.
- `77ff1064` Globals typed constructors/string helpers: getItemName,
  getRandomPlanetName, getRandomStation, replaceKeyBindingTokens, and
  getRandomSystemForDrinks became **100% linked**; getKeyBindingReplaceString
  **81.2->96.7**, init **65.4->80.5**; linked **2560->2565**, imports **692->685**,
  operator_call **429->416**.
- `2f3e0e9b` StarMap real state/globals/callees: D2 **73.4->94.2**,
  askForJumpIntoAlienWorld **63.2->90.9**, initLights **51.3->100% linked**,
  OnTouchMove **37.8->38.2**, depart **14.0->49.1**; linked **2565->2566**,
  imports **685->684**, void_ptr **37->34**, operator_call **416->415**.

Gate-driven rejects/requeue:

- MenuTouchWindow::addButton direct `new TouchButton(...)` called the right export but
  regressed **67.6->55.0** and grew **296->312B**. Reverted. Retry must retain the old
  allocation/source scheduling while reaching the real constructor; do not simply repeat
  the typed `new` expression.
- ObjectGun::update fully typed canvas/AEMath rewrite regressed **15.0->7.6** and grew
  **844->892B**. Reverted only that body; C2/replaceGun/render landed. Reconstruct update
  per call cluster, preserving its current sret scratch and `PaintCanvas **` load shape.
- PaintCanvas matrix retry learning: passing the local return buffer as the explicit
  MatrixIdentity argument doubled the frame to 128B and cratered both functions to ~22%.
  The matching form discards the by-value result while passing `identityMatrix` as the
  explicit `Matrix&`; this restored the 64B frame and made both functions linked-exact.
- StarMap layout blockers remain exclusive work: setStart observes original path/pathFinder
  offsets **0xa0/0x50** versus current **0x94/0x44**; missionChangedFlag is original
  **0xdc** versus current **0xd8**. OnTouchMove/depart retain broader layout/body residuals.
  Do not mix these with another worker wave.
- ModStation residuals: resetIdleCamForHangar needs coordinated Matrix-by-value return-slot
  reconstruction; checkPendingProducts still lacks the notice String pipeline; C2 still
  uses placeholder camera tables. PaintCanvas MeshSet2DMask/ReloadTextures/MeshGetTriCount/
  TransformAddChild are scheduling/source-shape work, not wrong callees. Globals
  getRandomEnemyFighter/playMusicAndFadeOutCurrent need literal-table recovery.

Verified source-shape facts:

- Always inspect per-function before/after rows even when the aggregate ratchet passes:
  the first PaintCanvas batch and ObjectGun batch both passed globally while hiding severe
  local regressions.
- StarMap::depart calls byte-exact `Status::jumpgateUsed()` only for its side effect, then
  tests `jump` and `jumpMapModeB`; no return-type/member-pointer cast is needed.
- Correct real callees can still worsen codegen because typed `new`, hidden sret arguments,
  receiver caching, and exception cleanup change frames/register allocation. Split those
  sites instead of accepting them for import-count gains.

Tier position: tier 1 remains exhausted (`stub_zero_size 0`). Tier 2 wrong-callee/de-shim
work remains active with **684** pinned imports; start the next session by re-triaging the
current import set and assigning disjoint classes/TUs. No workers are in flight. Tier 5
StarMap layout findings above are queued but must wait for an exclusive pass; drift remains
zero.

## Session 2026-07-25g (wave 16D — PlayerEgo ramps + PlayerFighter update + MTW de-shims)

Net: avg 77.33->77.34, byte 1159 (=), linked 2557 (=), imports 737->725 (-12, locked), extra 36,
parity 0/0, RATCHET PASS, lint CLEAN. Per-function net +55.6 vs HEAD (15 gains, 3 wobble regs
-0.5/-0.3/-0.1 in untouched TUs = GOT/pool shift from import-count change, accepted).
- PE author (stalled twice on stream watchdog; controller finished): inlined PE_pitchRampDelta /
  PE_yawRampDelta into down/up/right/left with pool-decoded constants (12.0 / 1.45 / 1.25 / 3.3 /
  20.0 / 25/6 cap 280); fixed down()/up() pre-existing bug ang->step (ft*delta) per binary; turret
  k1/k2/k3 pools (200, 1/4096, +-2pi); re-modeled PlayerEgo 0x378-0x390 (cloakMaterial1/2/3 now
  0x388/0x38c/0x390 matching binary stores; new field_0x378, field_0x384); checkForTurret guard
  cloak!=0 + rollGeometry/turretGeometry meshIds. up/down/right/left +9.3/+8.7/+7.5/+6.6,
  strafe +3.1, toggleCloaking +2.6.
- Controller cleanup of PE options reads: OptionsRecord gained flag_0x11 (split field_0x10[3]);
  float ramp settings read via the documented reinterpret_cast-on-named-slot precedent
  (field_0x14 / field_0x18[0]) through (OptionsRecord *) Globals::options — no raw
  Globals::options[0xNN] byte-offset reads in new code.
- PF author: state-8 docking + shooting-math (0xded30-0xdf040) authored inside update;
  PlayerFighter::update 16.1->16.9 only (structural divergence remains — future deep pass).
- MTW worker: 10 mechanical shims removed; addButton regressed -8.4 from `bpos = get()`
  reassignment (temp+copy kills shared sret slot). NEW IDIOM: original reuses ONE sret stack slot
  for consecutive by-value calls -> write two SCOPED direct-init locals (non-overlapping lifetimes);
  addButton 65.7->67.6 (above pre-wave). MTW::update +13.7.
- Worker-stall protocol: SendMessage resume worked once (agent fixed its own broken TU before
  stalling again); controller then took over remaining sites directly. Always save the diff to
  scratchpad before touching a stalled worker's tree.

## Session 2026-07-25f (wave 16C — psm/pss + MovingStars/_ae_ + FMOD/FModSound de-shims)

Net: avg 77.31->77.33, byte 1159 (=), linked 2556->2557 (+1), imports 776->737 (-39), extra 36,
parity 0, RATCHET PASS + locked, lint CLEAN (void_ptr 39->37 locked).
- Three parallel workers: 21 psm/pss shims (ParticleSystemMesh/Manager), 12 MovingStars_ + 8 _ae_ shims
  (3 mangling-load-bearing _ae_ externals kept), 6 FModSound_ externs + playWingmanRecruit_ch recovery
  (heap Agent + getDialogueSoundId(0x139) + play) in ModStation, and the FMOD_* family in FModSound.cpp
  now calling FMOD::EventSystem/Event directly.
- Ratchet initially FAILED on 7 NEW imports — all verified present in the ORIGINAL's dynsym
  (EventSystem::setLanguage/getEventBySystemID/setReverbProperties/getReverbPresetByIndex/
  set3DListenerAttributes/load, Event::set3DAttributes) -> deliberate --update-baseline add.
- Controller fixed 4 regressions post-merge instead of reverting TUs:
  (1) FModSound::stop(FMOD::Event*) 100->46 — worker's `(FModSound*)(uintptr_t)(unsigned)e->stop(false)`
  cast chain (inttoptr after call) killed the tail-call. Return type is NOT mangled: redeclared as
  `void`, plain `e->stop(false)` -> guarded tail-call restored, back to 100 (the +1 linked_exact).
  LEARNING: change unmangled return types instead of casting an FMOD_RESULT to a pointer.
  (2) MovingStars::translate 82.7->38.6 — worker cached `PaintCanvas *canvas = Globals::Canvas` (hoists
  the load; original reloads per use via `PaintCanvas **canvas = &Globals::Canvas`) and assigned sret
  returns through `*(Vector*)charbuf` (temp+copy). Pointer idiom + direct-init typed locals
  (`Vector pos = MatrixGetPosition(...)`) -> 88.5, above pre-wave.
  (3) MovingStars ctor 67.5->62.2 — worker DROPPED the `loc` base arg of MatrixSetTranslationFrom
  (semantic bug). Restored `Matrix mat = MatrixSetTranslation(*(Matrix*)TransformGetLocal(h),0,0,0)`
  -> 72.6, above pre-wave.
  (4) Player::StopEngineSound 71->64.5 after the stop() retype — original stores
  engineSoundPlaying(0x108) before engineEvent(0xf0); clang emitted them swapped. Swapping the SOURCE
  statement order made clang invert them back to the original order -> 71.0. LEARNING: clang -Oz may
  invert adjacent-store order; if bytes show swapped stores, swap the source statements.
- Accepted residuals: setParticle -3.3, updateEvent3DAttributes -2.7 (low-match fns mid-authoring),
  PlayerFighter::update -0.1 (noise). Net per-function delta vs HEAD: +82.8.
- FMOD worker ignored the drop-GOF2_MATCH correction (reported the Transform static_assert as its only
  ModStation failure); controller re-verified both TUs exit-0 WITHOUT the flag. Worker compile commands
  must NOT define GOF2_MATCH.

## Session 2026-07-25e (wave 16B — LensFlare/AppMgr/Engine/RB de-shims + PF_update_body merged into update)

Net: avg 77.3->77.31, byte 1159 (=), linked 2556 (=), imports 820->776 (-44), extra 36,
parity 0, RATCHET PASS + locked, lint CLEAN.
- PF merge landed: PF_update_body inlined into PlayerFighter::update via single `update_body:`
  semantic label (9 call+return sites -> goto; one goto-over-init fixed by hoisting
  `Array<Player*>* enemies = nullptr;`). update 5.1->16.2 (+11.1). Now ~1518 insns vs orig
  ~3639; remaining gap = unauthored state-8 docking approach + ded30-df040 shooting-math block.
- LensFlare de-shim (12 shims): disasm-verified mappings incl. imgHandle/imgWidth BOTH ->
  GetImage2DWidth (PLT 0x62d84), pushState/setBlend/drawFinal/restoreState block ->
  GetColor/SetColor/FillRectangle. LensFlare ctor 55.6->63.9, render2D +1.0.
- EaseInOut_advance_ou -> e->Increase((float)dt); e->GetValue() (ModStation). AppMgr shims (7)
  -> Globals::appManager->... in ModStation + HangarWindow. ModStation::OnUpdate -0.9 accepted.
- Engine_* shims (11) removed across all 5 shader TUs (DrawFBO/Blur/Bloom/GlowPP/PostBW);
  every RenderEffect up: DrawFBO 84.8->92.7, PostBW +6.1, Blur +5.3, Bloom +4.7/+3.8, Glow +4.6.
- RB_* (20 shims) RepairBeam fully de-shimmed with per-site disasm proofs (receiver kp->player;
  GetDirVector/GetUpVector scale -300/200; damage(1,false,-1); getAttribute 0x35 range/0x36 rate;
  sort arg 9 for shield check). update 16.2->22.5. Sound globals retyped int* -> FModSound**.
- NOTE: worker standalone compile cmd lacks -DGOF2_MATCH -> Transform.h static_assert noise in
  worker reports; authoritative cmake build was clean. Add -DGOF2_MATCH to future worker prompts
  or ignore that specific assert in reports.
Next: wave 16C (psm/pss 25, MovingStars_ 12 + _ae_ 6, FMOD_ 11 + FModSound_ 12); PE_ analyst
(PlayerEgo::update chunk map like notes_pf_update.md); _mtw_ mechanical subset (single owner);
PF state-8 + shooting-math authoring.

## Session 2026-07-25d (wave 16A — imports triage + mechanical de-shims + emit volatile-cache + PF chunks 9-16)

Net: avg 77.3, byte 1159 (=), linked 2556 (=), imports 838->820 (-18), extra 36, parity 0,
RATCHET PASS + locked, lint CLEAN.
- IMPORTS TRIAGE (notes_imports_triage.md in scratchpad): 650 fake shim imports total
  (838 - 207 orig - 5 real libc/GLES that just bind differently: __aeabi_d2lz, __aeabi_ldivmod,
  glFinish, posix_memalign, puts). Top groups: paintcanvas 108 (dsc_* HARD), _mtw_ 79 (mixed),
  PE_ 24 (HARD, PlayerEgo::update chunks), _psm_/_pss_ 25, Station_ 21, AEFile_ 17, RB_ 14,
  Ship_ 14, FModSound_ 12, LensFlare_ 12. Wave 16B plan: LensFlare_(12)+EaseInOut_(5)+
  ApplicationManager_(7)+Engine_(11)+RB_(14). Wave 16C: psm/pss(25)+MovingStars_(12)+_ae_(6)+
  FMOD_(11)+FModSound_(12).
- RecordHandler AEFile de-shim: all 22 AEFile_* shims -> AEFile::Read/Write typed calls.
  7 fns up (recordStoreWritePreview +8.0/+5.8, recordStoreWrite +6.6, loadResolutionValue +6.1,
  readMission +5.8, saveOptions +5.1); loadOptions -1.1 accepted (net strongly positive).
- ModStation de-shim: 23 Station_/Item_/Ship_/Mission_ wrapper shims removed, direct typed
  calls via (T*)(long)handle idiom. ~46 opaque extern shims deferred (need per-site disasm).
  LEARNING: StarMap/SpaceLounge Station_* are `// lint: void_ptr` ABI EXTERNALS (mangling must
  match lib), NOT wrapper shims — don't count them in de-shim worklists.
- IParticleSystem::emit volatile-pointer-cache: 31.0->35.9 (+4.9). Root cause of the old -56B
  frame gap: orig destroys the def base reg (mov sl,#0 @182086) after caching 24 def pointers +
  &random + uvp to stack; clang folds plain pointer locals to [base,#imm] and never spills.
  Fix: `T *volatile` locals reproduce the exact add/str cache block + reload-deref loops.
  3 proven residual blockers: volatile allocas can't share slots with regalloc scratch
  (stack coloring), sequential-by-decl slot order vs scattered regalloc slots, allocator
  lottery on cache reg (def in r4 vs orig sl). Frame now 0x150 vs orig 0x130 (overshoot).
- PlayerFighter chunks 9-16: author completed ALL 10 update states + post-state subsystems in a
  static PF_update_body(PlayerFighter*,int) helper (I made it file-static after it leaked as an
  export -> extra 37/allowlist FAIL; caught by ratchet). update UNCHANGED at 5.1 because the
  helper is a local symbol invisible to verify AND structurally wrong: orig update is ONE 10KB fn;
  the 7 call sites need merging into update as a structured shared tail. NEXT PF TASK: inline
  PF_update_body into update (single shared-continuation restructure), then re-verify.
- Controller quality fixes on PF: field_0x148 -> Player *targetPlayer (proven: DeepOpen
  PlayerFighter.java line 672 `target__ != null && !target__.hidden && dist < sightRange`;
  +0x5e = Player::field_5e = hidden; +0x69 = Player::pad_69). The author had misattributed the
  de9da +0x69 read to commandRoute (wrong: both reads go through [r4,#0x148] — Route is only
  0x18 bytes so +0x69 was impossible). field_0x1ec retyped int32_t->float (all 4 uses were
  float puns). field_0x128 = sightRange per Java (rename deferred).
- Noise: BuildResourceList/OnCreateApplication -0.4 each (GOT/alignment wobble from import
  count change).
Next: wave 16B mechanical de-shims (list above, disjoint TU ownership; MenuTouchWindow still
single-owner pending); PF_update_body inline-into-update restructure; PE_ (PlayerEgo::update)
analyst+author pipeline like PF; Ghidra PSM/PSS struct sync when MCP reconnects.

## Session 2026-07-25c (wave 15D — PaintCanvas dtor/RAR de-shim + PF clean successor, both landed)

Net: avg 77.28->77.29, byte 1159 (=), linked 2556 (=), imports 855->838 (-17), drift 0,
extra 36, parity 0, RATCHET PASS + locked, lint CLEAN.
- PaintCanvas dtor/RAR/RSSR fully de-shimmed: all 17 paintcanvas_ext_{dtor_*,rar_*,
  release_sprite_res} shims + 21 call sites -> real ops, each proven from D1/RAR disasm
  (typed `delete (ResourceTexture/Mesh/Transform*)payload` where dtor+_ZdlPv pairs exist;
  MeshRelease/ImageFontRelease/Image2DRelease/SpriteSystemRelease by name; placement
  `->~String()` on the String embedded at PCCubeTexView.pathField+0; `delete cam`/`delete mat`
  typed). ReleaseAllResources 32.4->40.9, D1/D2 10.3->13.2. Controller fixed the worker's 4
  banned `::operator delete` spellings -> plain typed deletes (`delete char* payload/cell`,
  `delete PODstruct* tex` — identical bare _ZdlPv codegen); operator_call stayed at baseline 23.
- PlayerFighter clean successor: ctor 25.6->34.6 (beats the rejected draft's 31.8), update
  2.2->5.1 with STRUCTURED chunks 5-8 (wingman dispatch, target validation/route-follow,
  maneuver timer RNG gates, enemy scan loop; binary range dd22c->dd620). 0 gotos, 0 void*,
  0 builtins, lint CLEAN. Rejected-draft mining + DeepOpen control flow worked as a method.
  Resume state in notes_pf_author2.md; remaining chunks 9-16 (death/loot/boost/docking/
  shooting/roll/landmark) are the big tail (update is 10376B).

Next: PF update chunks 9-16 successor; remaining ~838 fake imports (fleet skip reasons in
tasks/w08jt2rdn.output); IParticleSystem::emit -56B frame chase; Ghidra PSM/PSS sync when
MCP reconnects.

## Session 2026-07-25b (wave 15C — monster-function fleet: 2 analysts + 4 authors, 4/5 TUs landed)

Analyst→author pipeline on the low-pct monsters. Net: avg 77.27->77.28, byte 1159 (=),
linked 2556 (=), imports 859->855 (-4), drift 0, extra 36, sodiff extra-beyond-allow 0,
RATCHET PASS + baseline locked, lint CLEAN.
- PaintCanvas::SetBlendMode 68.5->88.4: the 9 `sbm_setlight` shims are glDepthMask (proved by
  decoding the ARM PLT veneer at 0x19b398 -> 0x5eec0 glDepthMask@plt); they tail-merge under
  -Oz into the 3 machine call-sites my callee census saw — census counts MACHINE sites, not
  source calls; don't refute a mapping on counts alone. lightenable/lightsetlight shims ->
  real `engine->LightEnable(bool)` / `LightSetLight(0x4000)`; setalpha/glenablecap ->
  `engine->GlEnable(..)`. `volatile` on the file-static flag storage stopped const-folding and
  restored the original prologue + TBB switch shape (lint-clean).
- Level::initParticleSystems 22.8->42.9: rodata tables decoded from orig (skyColors@0x1fc4d0,
  shipTypeMap@0x1fc3d0, 4 velocity tables@0x1fdba0), field_74/field_80 system routing,
  enableSystemEmit(false) bug fix, 520B frame reproduced with 8 named Matrix locals.
- IParticleSystem::emitManual 27.6->34.2 (branch-order fix; single reused velBuf[12] matching
  orig slot reuse; NEON preloads force vpush {d8-d15} exact). emit 25.6->31.0 (24-pointer
  pre-loop def cache; frame still -56B, ~14 cache entries don't spill — residual blocker).
- PSM::emitTrail 53.4->58.8: unnamed-temp chain `MatrixGetRight(..) * f` reuses the sret slot,
  frame 264->248 (=orig). CEILING: r4/r5 swap — LLVM spill weight (uses/live-range) gives r4 to
  the short-lived sret buffer over long-lived `this`; no legal C++14 source form flips it.
  Same class: initParticleSystems `this` lands in r8 vs orig r4. Both documented in notes_*.md.
- PlayerFighter REVERTED (not landed): author got ctor 25.6->31.8 and update 2.2->13.6 but the
  update body was a Ghidra-style dump — 174 `goto dda88`-style address labels, 9 void_ptr casts,
  1 __builtin_memcpy, byte-offset field reads. Lint FAILed it; quality constraints make it
  uncommittable. Raw files saved: scratchpad/PlayerFighter.{cpp,h}.pf-author-raw + analyst map
  notes_pf_update.md (16 chunks) + notes_pf_author.md. Successor must rebuild with real control
  flow + typed fields. Analyst found real bugs to keep: ctor setLoop(true) not false (0xdc656),
  missing EaseInOutMatrix docking path, 5-slot roll buffer at +0x29c..0x2b4.
- Fleet learnings: worker prompts must state the no-cmake/ninja ban MORE prominently (one worker
  rebuilt the shared .so mid-wave twice); authors must be told "goto labels named after asm
  addresses = automatic reject" explicitly; nudges via SendMessage un-stalled 2 of 4 authors.

Next: PF cleanup successor (from saved raw + chunk map); dtor/RAR typed-delete de-shim in
PaintCanvas (includes in /tmp/revert.patch hunk 1; typed `delete (ResourceTexture*)p`, never
::operator delete); remaining ~855 fake imports; IParticleSystem::emit -56B frame chase;
Ghidra PSM/PSS struct sync when MCP reconnects.

## Session 2026-07-25 (wave 15B pt.2 + 15C starters — fleet gate, PSS/PSM authored)

Gated the round-2 de-shim fleet (w08jt2rdn, 7/8 groups) + two SendMessage-resumed authors.
Net: avg 77.21->77.26, linked 2555->2556, byte 1159 (=), imports 938->873 (-65), drift 0,
extra 36, RATCHET PASS, lint CLEAN, baseline locked.
- PSS::updateAreaExitParticle 77.7->91.6 (author iteration: unnamed-temporary chaining
  shrank frame 88->72; natural pair order + machine tail-merge; ldmia/stmia via 3x uint32
  loads; `if (len<1.0f) return;` for the bmi shape). Residual: orig's unfolded
  `add r0,r5,#120; ldr r1,[r0,#0]` not reconstructible legally (CodeGenPrepare always
  folds when base is live) — 2 structural lines, documented in notes_pss.md.
- PSM::emitTrail 0->53.4 + updateSingleColor 64.7->68.1 (emitTrail authored + all 42
  g_ParticleSetData refs -> real extern ParticleSettingsRef::cur, defeating static-array
  CSE; header emitTrail de-static'd, mangling unchanged).
- PaintCanvas fleet keepers: DrawTransform 41.9->75.8, StopDraw2FBO ->100, TransformAddMeshId
  +10.1, GetLineArray +9.4, TransformCreate +8.6, ReloadTextures +5.7, FontCreate, BeginBG,
  DrawMesh, MeshChangeShaderAnimValue +2.9. De-shims to REAL fns kept: FindResource,
  TextureCreateFromFile/Intern, CollectAnimationData, GetAEChar, delete[], glEnable/
  glDisable/glBlendFunc/glDepthMask (all 4 are genuine orig imports).
- REVERTED: dtor/ReleaseAllResources rewrites (nonexistent AbyssEngine::Resource* types +
  banned `::operator delete`) -> HEAD shims; GL1 laundering (glScalef/glLoadMatrixf/
  glLoadIdentity/glMultMatrixf/glTexEnvi direct calls = fake imports); SetBlendMode case
  0x25 `sbm_setlight(1)`->`glDepthMask(1)` swap (orig calls a light fn there: LightEnable
  x5 + LightSetLight x4 = 9 == HEAD's 9 setlight shims; swap cost 45.5->27.6, reverted).
LEARNINGS: (1) verify.py writes report.json to --build-dir, NOT cwd — stale repo
report.json poisoned an hour of measurements; always `cp $BUILDDIR/report.json report.json`.
(2) TextureCreate 64.0->34.1 is verify GOT/PLT-layout wobble: objdump diff of both builds
is instruction-identical (only PLT addrs/labels). Accepted; do NOT chase pct on blx-heavy
mid-size fns without an objdump A/B first. (3) The original DEFINES glTexEnvi as a local
2-byte stub (0x6dad2, exported) — GLES2 build stubs GL1 texenv; we currently shim
sbm_texcombine; modeling a real stub `glTexEnvi` TU could kill the shim AND match the
export (needs sodiff_allow check). (4) HEAD-report contamination from mid-gate interleaving
makes new-vs-HEAD diffs lie — trust only same-methodology A/B rebuilds.
NEXT: 15C monsters (IParticleSystem::emit 25.6 2368B, emitManual 27.6, emitTrail iterate
53.4->, Level::initParticleSystems 22.8, PlayerFighter::update 2.2 10376B, PF ctor 25.6);
glTexEnvi stub experiment; remaining ~866 fake imports (fleet skip reasons in
tasks/w08jt2rdn.output); dtor/RAR legal de-shim wave (typed delete, real Resource types);
Ghidra PSM/PSS struct sync when MCP reconnects.

## Session 2026-07-24c (wave 15A — drift eliminated: 49 fields / 5 classes -> 0, exclusive)

Workflow wf_096681a9-c42: 6 read-only sonnet analysts (one per class), ASM evidence via
`tools/verify/verify.py --show` on the ORIGINAL .so (full dynsym survives stripping — no
Ghidra needed for layout ground truth; objdump path + verify invocation in build.ninja).
Controller applied per-class, gated each (link_so+drift+verify+ratchet), 4 commits:
- PlayerTurret: `char field_0x3e` was a dup of KIPlayer::field_0x3e (base) — removed.
- PlayerAsteroid: missing `float field_0x160` (ctor strd 0.0001f pairs it w/ hitFlashTimer).
- Camera: campaign note was STALE — already 0x5c since the Matrix 0x3c fix; PaintCanvas
  holds Array<Camera*> (no inline shear). Added static_asserts only.
- Hud: eventScrolls uchar->int (str.w 0x1d8); _hudrealign_0x1f0[4]->int (align-driven);
  0x45c/0x464 real fields carved out of realign blobs.
- GameData: 0x6c..0x77 re-modeled as an 11-byte uint8_t run — original's unaligned
  str.w[+0x6d]/str.w[+0x71]/strh[+0x75] are clang-MERGED byte stores (key idiom!).
  JniBridge zeroes each byte; clang re-merges identically. GameData C2/D2 -> 100.
- PlayerFighter (26 fields, multi-gap): +3 Vectors 0x17c/0x188/0x194, +int 0x1bc, +int
  0x1cc, field_0x254/0x255 moved BEFORE rollMatrix (roll() strh 0x254 right after
  easeBaseMatrix), +field_0x2bc/0x2c0 after easeMatrix ptr, REMOVED field_0x2c9/0x2cd
  (Ghidra artifacts of overlapping unaligned ctor zero-stores str.w[+0x2c9]/[+0x2cd]
  spanning cloakTimer..cloakActive — second key idiom). 16 fn gains (setBoostProb/
  setAIDisabled/setShootError/setRotate/setCloakingPossible ->100, cloak +8.9,
  setWingmanCommand +14.9); ctor -0.1 (accepted).
Net wave 15A: byte 1155->1159, linked 2548->2555, avg 77.14->77.21, drift 49->0 fields.
LEARNINGS: (1) unaligned wide stores in the original = merged byte fields, model as
uint8_t runs; (2) overlapping zero-stores create phantom Ghidra fields at odd offsets —
delete, don't model; (3) drift `name=` hex is ground truth, fix actual to match it;
(4) analysis-only fleet + controller-applied edits works well for layout (no worker
edit hazards); analyst prompts + results: scratchpad wave15_analysis.json.
NEXT: wave 15B PaintCanvas shim redo (~80 shims, 1030 refs, call sites only, NO wrapper
laundering; sscanf via deliberate --update-baseline); 15C monsters (IParticleSystem::emit
25.6 2368B, emitManual 27.6, PSM::emitTrail 0.0, PSS::updateAreaExitParticle 0.0,
Level::initParticleSystems 22.8, PlayerFighter::update 2.2 10376B monster,
PlayerFighterC2 25.6 1324B now unblocked by correct layout); Ghidra PSM/PSS struct
sync still pending MCP reconnect.

## Session 2026-07-24b (wave 14 pt.2 — shim-sweep fleet, 8 workers, 6.5/8 pools landed)

Workflow wf_8981afbe-775 (8 sonnet workers, disjoint TU pools, edit-only; orchestrator gated).
Net: linked 2546->2548 (+2), byte 1155 flat, avg 77.09->77.14, imports 1014->950 (-64),
extra 36 / stubs 0 / missing 0 held. 36 per-fn gains incl. Engine::ShaderInit +49.4,
LightEnable +30.5, LightSetGlobalSceneColorAmbient +27.0, PlayerFighter::setPosition ->100,
PSM::init ->100, startValkyrie/startSupernova +6, MenuTouchWindow update/draw shims gone.
REVERTED wholesale: PaintCanvas.cpp pool — worker built 215 `static inline` wrappers KEEPING
the fake shim names (void* params everywhere: void_ptr 15->345 lint FAIL, +5 operator_call,
direct ::operator new, sscanf wrapper). Import laundering, not call-site replacement; the
-183 imports it claimed were rejected. PaintCanvas remains the big shim pool (~26 decls,
paintcanvas_ext_gl_* map to real GLES imports — redo properly, call sites only).
REVERTED surgically (per-hunk splice from HEAD + shim-decl restore):
- FileInterfaceAndroid Read/Write: JniTable(env)->CallIntMethod direct dispatch regressed
  Write 47.8->12.9 (original uses the variadic JNI_Call*Method import shape). Keep shims.
- PSM setQuadEdge/setParticle(12-arg)/updateSingle: real Vector operators + MeshSetPoint
  members regressed them (-6.6/-6.5/-19.5) while the SAME substitution gained in
  reset/init/updateUsual/updateTrail/updateSingleColor — kept those; regressed three kept
  their _psm_* vector/matrix shims (re-added 12 decls).
- MGame::successCheck: `new DialogueWindow()` -9.0 there but +2.3 in gameOverCheck (kept);
  successCheck's 3 sites back on ::operator new(0x74)+DialogueWindow_ctor(...) shim.
Accepted small losses for import convergence: RepairBeam::update -1.8, PF setLevel -0.9 /
handleCloaking -0.7, MGame OnTouchBegin -0.1 (all files net-positive).
sscanf: real original import (nm confirms); adding it needs the deliberate --update-baseline
path if PaintCanvas redo calls it directly.
Ghidra hygiene: IParticleSystem struct rebuilt in DB to ground truth (112B, create_struct
wholesale — NOTE remove_struct_field on packed structs SHIFTS trailing fields, don't use;
rebuild via create_struct). PSM/PSS derived structs still stale — MCP server disconnected
mid-sync; redo when back.
Worker-reported blocked (MenuTouchWindow): _mtw_startSupernovaChallenge_impl 764B blob,
Status setMission/setShip/setStation + Galaxy_getStation register-chained args, FModSound
setVolume drops channel arg, _mtw_draw_* / _mtw_onTouchEnd_* opaque sub-functions,
Ship/Item makers via fn-pointer tables.

## Session 2026-07-24b (wave 14 pt.1 — IParticleSystem family layout re-model, exclusive)

4-worker Ghidra harvest (scratchpad particle_harvest.json) + orchestrator disasm of every family
ctor/dtor/init/reset/release. Net: byte 1147->1155 (+8), linked 2532->2546 (+14), avg
76.91->77.09, imports 1022->1014 (-4, AERandom/base-ctor shims gone), extra 36 / stubs 0 /
missing 0 / wrong_type 0 held. operator_call lint 449->434. Removes the old IParticleSystem
field_0x2c/0x30 drift entries.
Ground truth landed (headers have full static_assert maps):
- IParticleSystem sizeof 0x70 (was 116B wrong): flags u32 @0x34 (single field, not
  trailFlags/edgeFlags/flags2 splits), inline Array<ParticleSet> @0x38 {count,data,cap},
  real AbyssEngine::AERandom @0x10 (member object; C1 in ctor mem-init list reproduces the
  original call sequence AERandom C1 -> stores -> Array C2; fake AERandom_* shims deleted),
  resource/idOffset i32 @0x54/0x58 (PSM mesh id == PSS sprite handle, unified),
  particleVelocities/Ages/SetIds @0x64/0x68/0x6c. Vtable has NO virtual dtor; header-inline
  `~IParticleSystem() {}` emits the Weak D2 the original exports; members auto-destruct in
  the original's order (Array then AERandom).
- init retyped int->void (PSS init tail-calls reset with no r0 set; return types unmangled =
  parity-safe; killed a forced `return 0`).
- Flag decoding: rotateUVs = bit25 `(flags>>24)&0x2` (old &0x80 was wrong, fixed in
  emit/emitManual/PSS updateSingle); trail = bit15 `(int)(flags<<16)<0`; edge test bit19.
- PSMesh 0xa0: derived @0x70.. incl. never-accessed field_0x8c; PSSprite 0x78 (alloc
  `movs r0,#0x78` beats Ghidra DB 0x74): cachedPow @0x70 = Pow(0.7f,0.2f) (pool consts, old
  Pow(0,0) wrong) + never-written field_0x74.
- PSMesh dtor LESSON: literally-empty body lets clang skip the vtable store
  (CanSkipVTablePointerInitialization) -> 4B vs orig 20B; body `this->release();`
  (devirtualized, inlines to nothing) forces the store -> linked-exact. Same idiom as PSS dtor.
- `new Vector[n]()` (value-init aggregate) reproduces the orig alloc+memset pattern in PSS ctor.
- Family linked-exact gains: base D2, PSM D1/D2, PSS D1/D2, PSS release/init/getQuadCount/
  enable, PSM emit/finishCurrentTrailParticle/release/incId/startNewSection + more.
Drift gate: GameData(16)/PlayerAsteroid(3)/Hud(3)/PlayerTurret(1) entries are PRE-EXISTING at
HEAD (verified via stash) — not from this wave; queue them as layout items.
Still open in family (next: shim sweep + monsters): PSM/PSS updateSingle, setParticle,
interpolateColor, emit 25.6 (2368B), emitTrail stub (1716B), updateAreaExitParticle stub
(700B), PSM ctor 32.6, Manager ctors 62.9.

## Session 2026-07-24 (wave 13 — shim-sweep by class, 8 workers, 8/8 landed + 1 leftover)

Pools: diffs13 (84 fns, 8 disjoint class pools). Workflow wf_ef0eebba-80a (script had to embed
pool/worker data inline — the Workflow `args` param never reached the script; known bug, use
scriptPath relaunch with inlined data). Serial salvage via per-worker `git diff` snapshots
(scratchpad/snapshot_patches.sh) -> revert tree -> apply/gate/commit per item.
Net: byte 1143->1145 (+2), linked 2511->2527 (+16), avg 76.75->76.86, imports 1025->1018 (-7),
extra 36 / stubs 0 / missing 0 held.
- 18c28307 (pre-wave leftover) recordhandler3: writeAgent 88.1->99.6, writeMission 93.3->98.8;
  orchestrator fixed a void_ptr lint hit by dropping an unneeded (void*) cast (implicit conv).
- 9b1171cc paintcanvas4: strcmp shims -> String::Compare in GetMesh/GetTextureResourceId,
  Suspend cached-reload fix (-1 import).
- c5f34252 particles: PSMesh::render shims -> real PaintCanvas members 78.6->92.9, PSSprite
  D1/D2 + PSManager::releaseSprites ->100 (-5 imports). PSMesh dtor hunk REVERTED (70->22.2
  regression). Orchestrator retyped PaintCanvas::CameraGetLocal void*->float* (return types
  unmangled = parity-safe) instead of waiving the worker's void* local.
- a5c34a72 fileio: AEFile OPEN_READ=1/OPEN_WRITE=0 enum swap ->100 both (OpenWrite BYTE-exact),
  ReadSwitched void->uint32_t ->100, FIA::Output `return (char*)this` ->100, FileDelete
  String(name,false) ->100. FIA::Close hunk REVERTED (gMode statics path was closer).
- 543027c3 singles: readWanted AEFile_Read* shims -> real members 87.4->97.1.
- b6b1eb67 weapons: Gun::setLevel stores at 0x38 not 0x34 ->100, ObjectGun::setEnemies receiver
  is gun member [r0,#8] ->100 BYTE-exact, Gun D1/D2 92.7->95.1.
- d2b7a487 modstation2: autosave callDlcMenu tail ->100, OnResume float* global, showDlcMenu
  direct getPosition (-1 import, kept despite -4.3 pct: import convergence). REVERTED: worker
  added a param to the AEMath_MatrixSetTranslation shim decl = RENAMED the import -> ratchet
  hard-FAIL "new undefined import". Rule confirmed: never change a shim's signature, only
  replace call sites with the real callee. Also reverted an OnKeyRelease regression.
- 2979ee7c menutouch3: startGOF2 + setCutsceneMode ->100, setSkipButtonVisible 52.8->96.0 via
  xor-or button idiom `!((a^K)|b)`.
- f7e0c237 render_misc: MovingStars ctor createBillBoard args 61.0->67.5 (C1/C2).
NEW EXCLUSIVE-TIER LAYOUT ITEMS (worker-evidenced, orchestrator-only, Ghidra ground-truth first):
- PaintCanvas::identityMatrix ours 0xF4 vs orig 0xF8 — one of the three embedded matrices
  before it is 4B bigger in orig (not all embedded matrices may be 0x3c!). Blocks
  CameraGetLocal/TransformGetLocal.
- sizeof(Camera) ours 96 vs orig 92 — blocks CameraCreate (`movs r0,#92`).
- IParticleSystem ours 116B vs orig ~52B (!) — every ParticleSystemMesh field shifted +64
  (mesh 148 vs 84, positions 164 vs 100, edgeCount 212 vs 152...). Biggest single layout gap
  found so far; full offset list in wave13 particles worker notes (scratchpad wave13_results).
- Gun.h: `level` int at 0x34 is misnamed; real Level* slot is field_0x38 (setLevel now uses
  it; audit other `this->level` readers e.g. ignite). VecArray1 starts 0x14 not 0x10 —
  directionCount at 0x10 is actually VecArray0's capacity field; ctor/dtor alias via
  &velocities. Needs a Gun.h re-model.
- Radar.h drift (from wave 11) re-confirmed: getPlanetDockIndex needs planetDockIndex at
  +0x40 (ours +0x34; 12B missing between lockedGasCloud and players).
- MenuTouchWindow shim `_mtw_Status_setCredits(void*)` lacks the int arg orig passes (r1=0);
  fix startValkyrie/startSupernova by direct `(*statusHolder)->setCredits(0)`, NOT by editing
  the shim signature (see modstation2 lesson). Also Globals::ships is typed one indirection
  short (orig derefs 3 levels).
BLOCKED CLASSES (no source fix): register-scheduling walls (AEFile::Seek 93.9, FileExist,
TransformRemoveMesh/GetTriCount, MeshChangeShaderAnimValue, checkMedals, OnTouchMove,
MovingStars setRotation/rotate/getRotation/updateLod); GetScreenPosition needs Ghidra to
resolve paintcanvas_ext_getscreenpos_m's real callee; GetReverseString needs the wchar
String(in.data,false) form (semantic risk, deferred).
QUEUE for wave 14: exclusive layout pass FIRST (Camera 92B + PaintCanvas matrix region, then
IParticleSystem re-model, Gun.h, Radar.h — drain fleet, Ghidra ground truth, verify-gated);
then remaining fake-shim TUs (PaintCanvas ~219 fakes, PlayerEgo 37, PlayerFighter 17,
Globals 16, RepairBeam 16, Engine 14, StarMap 11, FModSound 11, MGame 10, HangarWindow 8);
19 true missing imports; monsters (Gun::shootAt 1788B placeholder, WantedWindow::draw).

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
