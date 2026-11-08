# LevelScript::process — wave 154 / w10007: prologue `status` register probe
# (still r9, but the vtable load moves to r4; raw -1 / aligned -9 at 57,232 B)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10007`, base master
`4417db0b4` (wave 153, w9904 landed).  Scratch in `/dev/shm/w10007/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,328            55,327            (-1)
process aligned insns    17,185            17,176            (-9)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The landed edit is a **`Status *status` local used for the two dispatch
calls only**:

```diff
     Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    if (Globals::status->getCurrentCampaignMission() == 0) {
+    Status *status = Globals::status;
+    if (status->getCurrentCampaignMission() == 0) {
...
-    } else if (Globals::status->getCurrentCampaignMission() == 1) {
+    } else if (status->getCurrentCampaignMission() == 1) {
```

It is size-exact (57,232 B) and drops both metrics.  It does **not** put the
status pointer itself into `r4`; it puts the *vtable* pointer there:

```
original prologue                          landed prologue
137d5a ldr   r4,[r0,#0]   ; status->r4     137d5a ldr.w r9,[r0]   ; status->r9
137d5e ldr   r0,[r4,#0]   ; vtable->r0     137d5e ldr.w r4,[r9]   ; vtable->r4
137d62 blx   getMission                    137d62 mov   r0,r4
                                           137d66 blx   getMission
...
137d76 ldr   r0,[r4,#0]   ; vtable again   137d7a mov   r0,r4     ; vtable reused
137d78 blx   getMission                    137d7e blx   getMission
```

The compiler now CSEs the `status->vtable` load across the two dispatch
blocks (the local `status` makes the alias analysis trivially identical), so
the second `ldr` becomes a `mov`.  This is a small faithfulness regression
(the original loads the vtable twice) but it is the best size-exact variant
measured this wave.

## Why `status` still lands in r9 (MIR evidence)

`-mllvm -print-after=greedy` / `-print-after=virtregrewriter` on the base
source shows the callee-saved assignment order in the prologue:

```
%12232 (&this->m_pLevel) -> r4    (1 def + 4 uses, all in entry block)
%12275 (messages)        -> r5
%12085 (delta)           -> r6
%11678 (enemies)         -> r8
%11652 (Globals::status) -> r9    (1 def + 3 uses; 2 in entry, 1 in bb.X)
%796   (__stack_chk_guard ptr) -> sl
%11434 (this)            -> fp
```

`LiveIntervals::getSpillWeight` in the NDK clang 7.0.2 binary
(`0xd18760`, disassembled) is exactly
`(isDef + isUse) * blockFreq / entryFreq` — **no** division by live-range
size.  `&m_pLevel` therefore weighs 5 (its `t2LDR_PRE` is a def+use) and
`status` weighs ~3.x (its second use is in a lower-frequency block), so
`&m_pLevel` is allocated first and takes r4.  The original has the same
operand counts, so its r4 assignment must come from a *different global
allocation* (the original keeps `this` in `sl` and the canary in `fp`, while
ours keeps `this` in `fp` and the canary in `sl`), not from a local prologue
source shape.

## Sweep (all built with the real project flags; `.o` prologue inspected)

Every variant below kept `status` in r9 unless noted:

| variant | size | status reg |
|---|---:|---|
| base `messages,player,enemies` | 57232 | r9 |
| all 6 accessor permutations | 57232/57200/57220 | r9 |
| `Status *status` local, both dispatch calls (landed) | 57232 | r9 (vtable r4) |
| `Status *status` local, first call only | 57232 | r9 |
| `Status *status` local, second call only | 57232 | r9 |
| `Status *status` local before accessors | 57292 | (re-coloured) |
| `Status *status` local after accessors | 57232 | r9 |
| `Status &status = *Globals::status` | 57256 | (re-coloured) |
| `Level *level = m_pLevel` (all 3 accessors) | 57232 | r9 |
| `Level *level` (mixed m/level accessors) | 57232/57272 | r9 |
| `Status *status` local for ALL uses in process | 57032 | fp (re-colour) |
| `Status *status` local for all uses, declared after accessors | 57148 | (re-colour) |
| null-check `Globals::status != nullptr && ...` | 57428 | (re-colour) |
| duplicate `getCurrentCampaignMission()` in condition | 57456 | r9 |
| case-92 3-call `getPosition()` shape | 57240 | r9 |
| case-92 3-call + status local | 57300 | (re-colour) |

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10007/fdiff.py _ZN11LevelScript7processEi        # 55,327 / 57,232
python3 /dev/shm/w10007/align_diff.py _ZN11LevelScript7processEi  # 17,176
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## Open questions

1. `status` r9 vs r4 is a whole-function greedy-allocator outcome.  The
   original's `this`=sl / canary=fp swap is the remaining correlated delta;
   matching it (likely by completing the body so `this` spills more — the
   original reloads `this` from `[sp,#88]` 328 times vs our 157) should let
   `status` reuse r4 for free.
2. The landed variant CSEs the second vtable load.  A source shape that keeps
   two vtable loads *and* is size-exact would be more faithful; none was
   found this wave.
3. The case-125 inline body and the 0xea/0x108 completion-tail gap (w9904
   open questions) are untouched.
