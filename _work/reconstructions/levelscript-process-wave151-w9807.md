# LevelScript::process — wave 151 / w9807: the mission-0 `(*enemies)[i]`
# loop is the status-r4 lever, but it costs +132 B

Status: **research note, no landed source change**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9807`, base master `9d491d46a` (wave 151).  Scratch in
`/dev/shm/w9807/`.

## Result (HEAD, unchanged)

```
process raw diffbytes    55,320
process aligned insns    17,244
orig / ours size         57232 / 57232   (size-exact)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
RATCHET                  PASS
```

## 1. The prologue difference is exactly the `&Globals::status` register

Original vs ours, byte-for-byte through `strd r5,r6,[sp,#72]`:

```
orig  137d26 mov sl, r0          ours  137d26 mov fp, r0        ; this
orig  137d30 ldr.w fp, [r0]      ours  137d30 ldr.w sl, [r0]    ; &__stack_chk_guard
orig  137d5a ldr r4, [r0, #0]    ours  137d5a ldr.w r9, [r0]    ; &Globals::status
orig  137d5c ldr r0, [r4, #0]    ours  137d5e ldr.w r0, [r9]
orig  137d64 str.w sl, [sp,#88]  ours  137d68 str.w fp, [sp,#88]
orig  137d68 strd r4, r8,[sp,#92] ours 137d6c strd r9, r8,[sp,#92]
```

`this`/canary are swapped (`sl`/`fp`) and `&Globals::status` is `r9` instead of
`r4`.  The two 32-bit `ldr.w` (4 B each) vs the two 16-bit `ldr` (2 B each) are
the +4 B.  `this`/canary swap is size-neutral.

The allocator is deterministic given the MIR, and the prologue MIR is
identical — so the difference is driven by the **body** codegen.  The allocator
(`RAGreedy`, LLVM 7.0.2) is a priority queue: global ranges by live-interval
size (longest first), local ranges by position, then `tryAssign` picks the
first free physreg in `RegisterClassInfo` order `r4,r5,r6,r8,r9,r10,r11`
(volatile regs first, then CSR order).  `&Globals::status` (interval
`0x137d56..0x137d68`, spilled at the entry block end) is a *global* range; it
gets whatever low register the larger prologue ranges (`this`, `delta`,
`messages`, `enemies`, `&m_pLevel`) leave free.  In the original `this` ends up
in `r10` and `r4` is free -> status `r4`; in ours `this` takes `r11`, so
`r10`/`r9` are consumed and status lands in `r9`.

## 2. The NEW lever: the mission-0 `(*enemies)[i]` loop shape

The mission-0 state-0 loop currently keeps a `KIPlayer **enemyData` local:

```cpp
for (int i = 0; i < 3; ++i) {
    KIPlayer **enemyData = enemies->data_;
    enemyData[i]->geometry->setDirection(Vector{1,0,0}, Vector{0,1,0});
    enemyData[i]->geometry->setVisible(true);
}
```

The original loop (disasm `0x137f34..0x137f66`) reloads `enemies->data_`
(`ldr.w r0,[r9,#4]`) **for both calls**; `enemies` itself is hoisted into `r9`
before the loop (`ldr.w r9,[sp,#96]` at `0x137f24`).  That is the
`(*enemies)[i]` source shape, not the `enemyData` local:

```cpp
for (int i = 0; i < 3; ++i) {
    (*enemies)[i]->geometry->setDirection(Vector{1,0,0}, Vector{0,1,0});
    (*enemies)[i]->geometry->setVisible(true);
}
```

**This shape flips `&Globals::status` to `r4`.**  Prologue becomes:

```
137d26 mov r9, r0             ; this -> r9
137d30 ldr.w sl, [r0]         ; canary -> sl
137d5a ldr r4, [r0, #0]       ; &Globals::status -> r4   (MATCH)
137d5c ldr r0, [r4, #0]       ; (MATCH)
137d68 strd r9, r4, [sp,#80]  ; this, player
137d6c str.w r8, [sp,#88]     ; enemies
137d70 stmia.w r0, {r5,r6,fp} ; messages, delta, player
```

but the whole function re-colours and grows to **57,364 B (+132)**, so
`byte_exact` would drop to 2741 (every following function shifts).  The
`setVisible` operand is the lever: changing only `setDirection` to
`(*enemies)[i]` (keeping `enemyData[i]` for `setVisible`) is byte-identical to
HEAD; changing only `setVisible` (or both) gives the +132 flip.

## 3. Measured variants (all built + linked on the wave-151 base)

| variant | size | raw | aligned | status reg |
|---|---:|---:|---:|---|
| HEAD `messages,player,enemies` | 57232 | 55,320 | 17,244 | r9 |
| `messages,enemies,player` | 57232 | 55,332 | **17,237** | r9 |
| `player,messages,enemies` | 57232 | 55,332 | 17,240 | r9 |
| `player,enemies,messages` | 57200 | 55,167 | 17,261 | r9 |
| `enemies,messages,player` | 57228 | 54,600 | 17,619 | r9 |
| `enemies,player,messages` | 57228 | 54,599 | 17,625 | r9 |
| `Status *status = Globals::status;` + first `if` uses `status` | 57232 | 55,320 | 17,244 | r9 |
| `Status *const status` / `auto *status` / `Status &status` | 57232 | 55,320 | 17,244 | r9 |
| `(void) Globals::status;` before the first `if` | 57232 | 55,320 | 17,244 | r9 |
| `int mission = Globals::status->getCurrentCampaignMission();` before first `if` | 57232 | 55,320 | 17,244 | r9 |
| `Status *status` + `int mission = status->...` before first `if` | 57232 | 55,320 | 17,244 | r9 |
| loop `setVisible` swapped before `setDirection` | 57232 | **55,318** | 17,252 | r9 |
| loop `enemyData` hoisted outside the loop | 57232 | 55,322 | 17,258 | r9 |
| loop `KIPlayer *e = (*enemies)[i]` | 57224 | 55,089 | 17,259 | r9 |
| **loop `(*enemies)[i]` for both calls** | **57364** | 54,971 | 21,858 | **r4** |
| loop `(*enemies)[i]` for `setVisible` only | 57364 | 54,971 | 21,858 | **r4** |
| loop `enemies->data_[i]` for both | 57364 | 54,971 | 21,858 | **r4** |
| `(*enemies)[i]` both + `messages,enemies,player` | 57364 | — | — | r4 |
| `(*enemies)[i]` both + `enemies,messages,player` | 57340 | — | — | r4 |
| `Status *status` used for the first N (N=1..97) `Globals::status->` | 57144..57328 | — | — | r4 only for N>=~30 |

The `status`-local sweep: N=1 is 57232 but the prologue becomes `this->r9`
(`mov r9,r0`, status stored straight to `[sp,#92]`); N=3 is 57236; N=10 is
57328; N=30 is 57308 with status `r4`; N=all (97) is 57144 with status `r4`.
No N lands at 57232 with status `r4` and the original stack layout.

## 4. Conclusion / open question

The faithful mission-0 loop shape (`(*enemies)[i]`) is the only source lever
found that reproduces the original's `&Globals::status`-in-`r4` prologue, but
it is +132 B on the current base, so it cannot be landed alone: it needs a
-132 B companion *and* the prologue stack layout still differs (`this`->`r9`,
`player`->`fp`, `&m_pLevel`->`[sp+64]` instead of `[sp+80]`).  The +132 is a
whole-function re-colour, not a local loop cost (the loop itself is only ~8 B
longer).  Reaching 57,232 B with status `r4` therefore requires matching the
body codegen first; the loop shape is the right starting point.

Size-exact variants that improve one metric trade the other: `m,e,p` is
aligned **17,237** (raw 55,332) and swapping the loop's `setVisible` before
`setDirection` is raw **55,318** (aligned 17,252).  Neither was landed because
the mission target (status `r4` at 57,232 B) was not reached and both are
faithfulness regressions.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9807/fdiff.py _ZN11LevelScript7processEi        # 55,320 / 57,232
python3 /dev/shm/w9807/align_diff.py _ZN11LevelScript7processEi  # 17,244
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```
