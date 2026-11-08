# LevelScript::process — wave 161 / w10702: literal-pool bug (b) + `data_` guard

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10702`, base master
`01c7f92d6` (wave 160).  Scratch `/dev/shm/w10702/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,048            55,013            (-35)
process aligned insns    17,049            17,308            (+259)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,518            85,483            (-35)
.ARM.extab diffbytes     1,258             1,259             (+1)
.rel.plt                 0                 0                 BYTE-IDENTICAL
.rodata / .data / .bss   exact             exact
.ARM.exidx               exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## Source changes (2)

### 1. Mission-89 case 4 (literal-pool bug (b))

`src/game/world/LevelScript.cpp`, mission-89 `switch (m_nState)` `case 4:`.
The original at `0x13b3ba` calls `Level::getStarSystem()` **twice** (once
for `scaleSunDuringSupernovaIntro`, once for `updateSupernova`), has **no**
`m_nState = 4` no-op store in the fade block, and its shared tail at
`0x13df5a` is

```
13df5a: ldr   r5,[sp,#76]        ; delta
13df5c: adds  r0,r0,r5           ; m_nScriptTimerA += delta (64-bit)
13df5e: adc   r1,r1,r5,asr #31
13df62: strd  r0,r1,[r4]
13df66: subs.w r0,r0,#8000
13df6a: sbcs.w r0,r1,#0
13df6e: blt.n 13df76
13df70: movs  r0,#5
13df72: str.w r0,[sl,#28]        ; m_nState = 5
13df76: movs  r5,#1
13df78: b.w   144f60             ; return 1
```

so the source is

```c++
m_pLevel->getStarSystem()->scaleSunDuringSupernovaIntro(delta);
m_pLevel->getStarSystem()->updateSupernova(delta);
if (m_nScriptTimerA <= 7000 && m_nScriptTimerA + delta > 7000) {
    Globals::layout->startFade(true, 255, delta + 1000);
}
m_nScriptTimerA += delta;
if (m_nScriptTimerA >= 8000) {
    m_nState = 5;
}
```

The double `getStarSystem()` call is required: the old local
`StarSystem *stars4` let clang CSE the two calls.  This edit alone is
**+8 B** (57,240).

### 2. Mission-0 state-0 loop `data_` guard (the compensating -8)

`src/game/world/LevelScript.cpp:637` (mission-0 state-0 ship-parking loop).
The original loop at `0x137f34` has no `enemies->data_` null check; the
defensive `if (!enemies->data_) continue;` is **8 B**.  Dropping it is
**-8 B**, so the two edits together are exactly size-neutral (57,232 B).
It is also semantically faithful (the array is never null at this point).

## Literal-pool bug (a): measured, NOT landed (breaks `.rel.plt`)

w10603's bug (a): the `if (m_nState >= 5 && m_nState <= 8)
m_pCamera->translate(-delta * 0.8f, 0, 0)` block is currently *inside* the
mission-78 switch after the last `case`; clang proves it unreachable and
deletes it (our build has one `0x3f4ccccd` load, the original two).  The
original's block is the switch's break/default target at `0x13c88c`
(`bhi.w 13c88c` at `0x138380`), i.e. it is **after** the closing brace.

Moving it out does restore the second 0.8f load, but it reorders the whole
57 KB function (clang's global block placement) and **changes the `.rel.plt`
slot order**: the mission-89 branch is emitted before mission-0, so
`RadioMessage::isTriggered` moves after `PlayerFixedObject::moveForward`
etc.  That shifts 7 unrelated functions' PLT call addresses and drops
`byte_exact` 4507 -> 4500.  Measured variants (all size 57,232):

| variant | raw | aligned | `.rel.plt` order | byte_exact |
|---|---:|---:|---|---:|
| base | 55,048 | 17,049 | identical | 4507 |
| block moved out (no hint) | 55,482 | 19,734 | 62 lines differ | 4500 |
| block moved out + `expect(cond,1)` | 55,330 | 19,621 | 46 lines differ | 4500 |
| block moved out + `expect(cond,0)` | 55,394 | 19,483 | 46 lines differ | 4500 |
| above + mission-89 fix + double `getStarSystem()` | 55,330 | 19,621 | 46 lines differ | 4500 |
| above + drop `data_` guard | 55,327 | — | 46 lines differ | 4500 |

The `__builtin_expect(...,1)` form is the semantically right direction (the
block is the fall-through/hot path in the original), and it is *also* what
makes the function size come back to 57,232 when the mission-89 fix and the
double `getStarSystem()` call are present.  It is left out of the landed
commit only because of the `.rel.plt`/`byte_exact` regression.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10702/fdiff.py _ZN11LevelScript7processEi        # 55,013 / 57,232
python3 /dev/shm/w10702/align_diff.py _ZN11LevelScript7processEi  # 17,308
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. Land bug (a) without perturbing `.rel.plt`: the block move must be made
   without flipping the mission-0/mission-89 emission order.  A source
   reorder or a branch-weight hint that keeps `isTriggered` first-referenced
   before `PlayerFixedObject::moveForward` is the missing lever.
2. The extra `Globals::sound` / `Globals::layout` loads (one each, w10603)
   and the 13-entry state `tbh` at ours `0x138a08` are still open.
