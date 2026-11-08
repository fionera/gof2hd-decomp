# LevelScript::process — wave 162 / w10805: the 0.8f tail block and `.rel.plt`

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10805`, base master
`29248c1d0` (wave 161).  Scratch `/dev/shm/w10805/`.

## Baseline (HEAD, gate green)

```
process raw diffbytes    55,013
process aligned insns    17,308
orig / ours size         57232 / 57232
.rel.plt                 byte-identical (0 reloc diffs)
byte_exact               4507 / 4524
linked_exact             4507 / 4524
```

## What is missing

The original has **three** `0.8f` (0x3f4ccccd) literal-pool words inside the
function (`0x13c8f8`, `0x13e87c`, `0x145c78`); ours has **two**
(`0x13a950`, `0x145c88`).  The missing load is the mission-0x4e post-switch
tail at `0x13c88c`:

```
13c88c: ldr.w r0,[sl,#28]     ; m_nState
13c890: subs  r0,#5
13c892: cmp   r0,#3
13c894: bhi.n 13c8ba
13c896: ldr   r0,[sp,#76]     ; delta
13c898: movs  r2,#0
13c89a: vldr  s2, 0.8f
13c89e: movs  r3,#0
13c8a0: negs  r0,r0
13c8a2: vmov  s0,r0
13c8a6: vcvt.f32.s32 s0,s0
13c8aa: ldr.w r0,[sl,#20]     ; m_pCamera
13c8ae: vmul.f32 s0,s0,s2
13c8b2: vmov  r1,s0
13c8b6: blx   TargetFollowCamera::translate
13c8ba: ldrb.w r5,[sl,#17]    ; m_bCinematicBreak
```

`0x13c88c` is the switch's `bhi` (default) target **and** the common break
target of every `case` (`subs #1; cmp #11; bhi 13c88c` at `0x13837c`), i.e.
the block is the *post-switch* source:

```c++
switch (m_nState) { ... }
if (m_nState >= 5 && m_nState <= 8) {
    m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);
}
```

Our source currently has that `if` **inside** the switch after `case 11`
(line 1239), where every case `break`s, so clang proves it unreachable and
deletes it.  The `if` is a 46-byte block (condition + translate).

## Measured variants (all on this tree)

`raw` = same-address diff bytes, `aligned` = instruction-index non-matches,
`reloc` = number of differing `.rel.plt`/`.rel.dyn` entries vs the original.

| variant | raw | aligned | size | reloc | notes |
|---|---:|---:|---:|---:|---|
| baseline (if inside switch) | 55,013 | 17,308 | 57,232 | **0** | block deleted |
| move out, no hint | 55,467 | 19,970 | 57,216 | 16 | -16 B |
| move out + `expect(cond,1)` | 55,327 | 19,619 | **57,232** | 25 | block byte-shape matches |
| move out + `expect(cond,1)` + 89↔106 swap | **54,943** | 19,446 | 57,264 | 25 | best raw, +32 B |
| move out + `expect(cond,1)` + state-5 block before `<=2` | 55,365 | 21,691 | 57,256 | 33 | isTriggered first |
| move out + `expect(cond,1)` + mission-26 branch early | 55,184 | 21,196 | 57,256 | 31 | isTriggered first |
| move out + `expect(cond,1)` + 26 & 89 branches early | 55,537 | 22,611 | 57,332 | 34 | |
| `case 8` fallthrough (reachable, condition kept) | 55,474 | 19,900 | 57,240 | 33 | block tail-merged (36 B) |
| `case 8` fallthrough, unconditional translate | 55,466 | 19,894 | 57,224 | 33 | -8 B |

Key facts:

* `expect(cond,1)` is the only shape that both restores the 46-byte block
  (own `blx translate`, same instruction multiset/order as the original) and
  keeps the size at exactly 57,232 B.  But it rotates `.rel.plt`
  (25 entries) and `check` drops `byte_exact 4507 -> 4500`.
* Without the hint the block is 36 B (tail-merged `b.w` into the mission-1
  `translate`) and the function is 57,216 B.

## Why `.rel.plt` rotates

The PLT slots are created in first-reference-address order inside this
function.  The original's first `RadioMessage::isTriggered` is the
**mission-26** branch at `0x1382fe` (not mission-0 state 5), and the first
`PlayerFixedObject::moveForward` is mission-89 at `0x13865a`:

```
orig:  isTriggered 0x1382fe < moveForward 0x13865a < setFreeLookMode 0x138d70 < getSpeed 0x138ffa
base:  isTriggered 0x1384e4 < moveForward 0x138638 < setFreeLookMode 0x138a02   (matches)
out_e1:moveForward 0x1383ee < setFreeLookMode 0x138588 < getSpeed ... < isTriggered 0x1387fe
```

Adding the post-switch block changes clang's **global** block linearization:
mission-89 is hoisted before the mission-0 state machine, and the
mission-0 state-5 `isTriggered` block lands after it.  Moving the mission-26
branch early fixes the `isTriggered`/`moveForward` pair but then
`setFreeLookMode` (mission-0 state 4) lands before `moveForward`; the
original has `moveForward` before `setFreeLookMode`.  This is the same
PGO/branch-weight linearization problem documented for
`Status::missionCompleted`; the original's `isTriggered` block is split out
to a cold address (`0x1382f4`) while the `getSpeed` half stays at
`0x138ffa`, which our non-PGO clang does not reproduce.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10805/fdiff.py _ZN11LevelScript7processEi        # 55,013 / 57,232
python3 /dev/shm/w10805/align_diff.py _ZN11LevelScript7processEi  # 17,308
python3 /dev/shm/w10805/poolscan.py 0.8 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

## Open questions for the next wave

1. Reproduce the original's split of the mission-0 state-5 block: the
   `if (messages[14]->isTriggered())` half is cold at `0x1382f4`, the
   `getSpeed` half is at `0x138ffa`.  A hint/statement shape that forces
   this split would fix the `isTriggered`/`moveForward`/`setFreeLookMode`
   PLT order while keeping the 0.8f block.
2. The `expect(cond,1)` block shape is otherwise exact; only the linearization
   is wrong.  A source reorder that keeps `moveForward` before
   `setFreeLookMode` (e.g. keeping mission-0 state 4 after mission-89) is
   the missing lever.
3. `Globals::sound` / `Globals::layout` still each have one extra load
   (w10603) — not re-audited here.
