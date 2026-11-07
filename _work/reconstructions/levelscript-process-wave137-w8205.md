# LevelScript::process — wave 137 / w8205 (the 0.8f camera-creep constant)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `655e3bcc5`
(wave-136 head). Scratch helpers in `/dev/shm/w8205/` (not tracked): `floats2.py`
(float-literal multiset diff), `ints.py` (integer-immediate multiset diff),
`bigdiff.py` (whole-function difflib block map), `callset.py`, `mkours.sh`,
`score.py`, `try.sh`.

## Result

```
                         before (master)   after (landed)
raw difflib ratio        58.9139           58.9139   (instruction stream identical)
orig / ours insns        20169 / 20049     20169 / 20049
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The change is a single wrong float constant; it does not move any instruction, so the
fuzzy score and `.text` positional diff are unchanged. It fixes one `.text` pool word
semantically (the pool word lives at a different address in our build, so the
positional byte-diff cannot see it).

## The bug: `-delta * 0.3f` should be `-delta * 0.8f`

Source line 1239 (general mission state machine, shared tail of the 64–73 group):

```cpp
if (m_nState >= 5 && m_nState <= 8) {
    m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);   // was 0.3f
}
```

Original `0x13c88c` (reached from ~30 cold blocks in the 0x13b8xx–0x13be9x case-64/73
region, then `b.w 144f5a` = the epilogue):

```
13c88c  ldr.w r0,[sl,#28]        ; m_nState
13c890  subs  r0,#5
13c892  cmp   r0,#3
13c894  bhi.n 13c8ba             ; m_nState not in [5,8] -> skip
13c896  ldr   r0,[sp,#76]        ; delta
13c898  movs  r2,#0
13c89a  vldr  s2,[pc,#92]        ; pool 0x13c8f8 = 0x3f4ccccd = 0.8f
13c89e  movs  r3,#0
13c8a0  negs  r0,r0
13c8a2  vmov  s0,r0
13c8a6  vcvt.f32.s32 s0,s0
13c8aa  ldr.w r0,[sl,#20]        ; m_pCamera
13c8ae  vmul.f32 s0,s0,s2        ; -delta * 0.8f
13c8b2  vmov  r1,s0
13c8b6  blx   TargetFollowCamera::translate
13c8ba  ldrb.w r5,[sl,#17]       ; m_bCinematicBreak
13c8be  b.w   144f5a             ; epilogue
```

Found by diffing the multiset of float literals actually loaded by the function
(`floats2.py`, resolves every `vldr sN,[pc,#imm]` to the pool word in each binary and
decodes `vmov.f32` immediates from objdump's hex comment). Before the fix the original
had one more `0.8f` than ours and ours had one more `0.3f`; after the fix the *set* of
float literals is identical apart from pool-sharing (the original keeps two separate
`0.8f` pool words at `0x13c8f8` and `0x13e87c`; ours now shares one word between the
`[5,8]` tail and the case-102 state-1 translate, which is why the count is 2 vs 3).

## What else was tried this session (all measured, none landed)

### 1. Prologue getter order

The original calls the three prologue accessors in the order
`getMessages`, `getPlayer`, `getEnemies` (`0x137d3e/0x137d46/0x137d50`); our source
declared them `enemies`, `player`, `messages`. Reordering the declarations to match
makes the prologue **instruction-for-instruction identical** (same `mov r5,r0`,
`strd r4,r0,[sp,#80]`, `mov r8,r0`, `strd r5,r6,[sp,#72]`), but it also swaps the two
adjacent stack slots 72/76 (`messages`/`delta`), which changes every later reload:
**57396 B (+164), ratio 58.6915, insns 20031**. Reverted. A faithful landing needs a
way to keep `messages` in slot 72 while computing it first (slot pairing is chosen by
the stack-slot colouring pass, not by source order).

### 2. Re-applying the wave-136 w8106 patch set (handoff)

The handoff's uncommitted patch (case-102 `setTarget`/`translate` reorder, case-154
`m_dirVec` order, cached-`player` receivers, `resetGunDelay` on a fresh
`m_pLevel->getPlayer()`, `m_nState++` in case-145 state 5, `case 6:` removal) measures
**57224 B (-8), ratio 57.9535, insns 20046**. Restoring `case 6: m_nState = 2;` gives
**57236 B (+4), ratio 58.6176** — i.e. the case-6 arm costs 12 B (it adds a sixth
dispatch arm, not just 4 B of body). No +8 B faithful companion was found in the time
box, so the patch cannot be landed size-exact. All its individual edits were re-verified
against the original this session:

* case 102 state 0 `0x13a702` really does `setTarget(enemies[0]->geometry)` **before**
  the `getPosition()+Vector{9000,-7000,40000}` / `setPosition` pair;
* case 102 state 1 (`0x13e880`) really does start with
  `translate(delta*0.6, delta*0.8, -delta*2.5)` and then `messages[2]->isOver()`;
* case 145 state 5 teardown (`0x13f8fe`) really does end in `m_nState++`
  (`b.w 14500e` → `adds r0,#1; str r0,[r9,#28]`), so `m_nState++` must be **kept**;
* case 145's dispatch (`0x13cacc`: `subs r0,#2; cmp r0,#3; bhi`) handles states 2..5
  only, so `case 6: m_nState = 2;` is a genuine extra arm and must be **removed**.

### 3. The handoff's "case-145 state 5/6/7/8/9 re-split" is a case-154 misread

The 10-entry `tbh` at `0x13e8f0` (`m_nState-1`, `cmp #9`) belongs to **case 154**
(entry `0x13a76e`), not case 145. Its states map 1..10 to
`13e908, 142f34, 1430d6, 143138, 1432c2, 14330e, 14349c, 1434c0, 1434f2, 14354c`,
which is exactly the source's existing `m_nState == 1..10` chain (state 6 = 0.25-step
camera ride + `messages[6]`, state 7 = `isDocked()` guard, state 8 = 2001-timer →
`m_nState = 9`, state 9 = `messages[7]` + teardown, state 10 = `messages[8]` +
objective). Nothing to re-split there.

## Why the body is "~120 instructions short" — it is encoding width, not code

Raw instruction counts: original **13044 × 2 B + 7786 × 4 B**; ours
**12802 × 2 B + 7907 × 4 B** (both 57232 B). Ours has 242 fewer 16-bit and 121 more
32-bit instructions — exactly the 121-instruction deficit. Each 32-bit instruction in
ours replaces a *pair* of 16-bit instructions in the original (2×2 B → 1×4 B).

The dominant cause is register allocation of `this`:

| value        | original register | original stack slot | ours |
|--------------|-------------------|---------------------|------|
| `this`       | spilled           | `[sp,#88]` (309 loads) | `sl` (776 `[sl,...]` uses) |
| `delta`      | `sl`              | `[sp,#76]` (154)    | `[sp,#76]`/`[sp,#72]` |
| `enemies`    | `r8`              | `[sp,#96]` (161)    | `r8` |
| `messages`   | `fp`              | `[sp,#72]` (39)     | `[sp,#72]` (175) |
| `player`     | spilled           | `[sp,#84]` (152)    | `[sp,#84]` (146) |
| `Globals::status` | spilled      | `[sp,#92]` (43)     | `r9` (23) |

`ldr r0,[sp,#88]; ldr r0,[r0,#28]` is 2×16-bit (4 B, 2 insns) where ours emits
`ldr.w r0,[sl,#28]` (4 B, 1 insn). Forcing `this` out of `sl` would convert roughly
400 such uses and overshoot the target by ~300 instructions, so the mapping is not 1:1;
the allocator keeps `this` in `sl` because our source has fewer simultaneously-live
values than the original. This is the next structural lever, not a missing statement.

## Open questions

* The source's case-145 state-5 teardown contains
  `Globals::status->nextCampaignMission(true);` but the original block
  `0x13f832..0x13f8fe` does **not** call it (the `nextCampaignMission` at `0x13f92e`
  belongs to the block reached from `0x13d4d2`, a different mission). Removing it is a
  genuine (size-shrinking) fix; the whole-function `nextCampaignMission` call count is
  10/10, so some other call must be missing elsewhere to compensate.
* The original has 37 `TransformGetTransform` calls, ours 36. The unmatched one is in
  the `0x1411xx`/`0x1415xx` region (case 91/92): the original emits both
  `enemies[0]->geometry->transform` (`0x141164`) and `...->childTransform` (`0x141184`)
  in the `field_178 > 7 && m_nScriptTime <= 179999` if/else, plus a third at
  `0x14152c`. Our source has the first two; the third is unaccounted for.
* Landing the w8106 patch set needs a genuine +8 B statement (or a -4 B size pad that
  does not break `.rel.plt`).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8205/floats2.py                       # float-literal multiset diff
python3 /dev/shm/w8205/score.py                         # 58.9139, 57232 B
```
