# LevelScript::process — iOS-structure pass 4 (wave 186, w13004)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13004`, branch
`wave186-w13004`, base master `2d02567f6` (wave-185 harvest, raw **52,075**,
`.text` 74,402).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13004`.

Continuation of wave-183 w12704, wave-184 w12804 and wave-185 w12904 (iOS
1.1.4 structural oracle).  This pass re-diffs the iOS Mach-O implementation,
re-derives the complete `tbh` dispatch map of the Android original vs ours, and
isolates the one remaining state-machine structural divergence.  **No
size-exact, `.rel.plt`-identical raw win was found; `src/` is left at the base.**

## Baseline (verified on this worktree)

```
process raw diffbytes    52,075
process aligned insns    20,766
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,402
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

## iOS 1.1.4 oracle re-check

`_work/bins/ios_1.1.4_HD` is the stripped armv7 Mach-O; `llvm-objdump
--triple=thumbv7` disassembles it.  `LevelScript::process` is the Thumb
function at `0x134098` (93,948 B from `LC_FUNCTION_STARTS`).  Re-confirmed:

* The iOS source-layout dump (`source file list dump/gof2hd_1.1.19_ios.cpp`,
  `Classes/Game/LevelScript.cpp`) lists exactly the same non-`process` members
  as the Android dynsym.  **No per-state helpers exist in either build.**
* Mission-0: state-0 body is statement-for-statement identical (player
  `(18000,-12000,-40000)`, camera `(-12000,2000,-500)`,
  `MatrixSetTranslation`, three ships at `(-10000,500,0)`,
  `(-10000,-300,-1700)`, `(-10000,-200,2000)`, `setDirection`/`setVisible`
  loop, `forceUpdate(30,false)`).  iOS sets `m_nState = 1` *before* the
  position calls (`0x134188`), Android/ours set it after; w12904 measured the
  Android placement as the unique best (+4 raw for the iOS order).
* Mission-0 compare order is **4, 3, 5** and the switch is `subs #6; cmp #9`
  (states 6..15) in both iOS and Android.  Our source order 4,5,3 lets clang 7
  merge state 3 into the switch (`subs #3; cmp #12`); the faithful 4,3,5 order
  is size +8 and raw +2,890 (w12904), so it stays unlanded.
* Mission-1 intro is identical in emitted order (`forceUpdate(delta,true)`,
  `rotate(0,angle,angle)` with `angle = ((delta<<1)&~16) * 2^-16 * 2*pi`,
  `enemies[0]->getPosition()`, `setToSleep()`, depth clamp / `moveForward`,
  message 2 fade-out, `translate(delta*0.1,0,0)`).
* **No enum value differs** and no state is split/collapsed in the
  mission-0/1 state machine.

## Android `tbh` dispatch census (new evidence)

The Android original has 15 `tbh`; ours has 15.  Matching the dispatch ranges
(the `subs`/`cmp` immediately before each `tbh`) gives:

| # | original range | ours range | note |
|---:|---|---|---|
| 1 | 1..12 | 1..12 | shared active tail |
| 2 | 1..5 | 1..5 | mission-1 switch |
| 3 | r1 (computed) | r1 | |
| 4 | r1 (computed) | r1 | |
| 5 | 0..3 | 0..3 | mission 73 |
| 6 | **6..15** | **3..15** | mission-0 switch (state 3 merged) |
| 7 | 1..7 | 1..7 | |
| 8 | 0..3 | 0..3 | |
| 9 | 1..10 | 1..10 | |
| 10 | **2..5** | **0..5** | **mission 145 (state 0/1 split)** |
| 11 | 2..7 | 2..7 | |
| 12 | 1..10 | 1..10 | |
| 13 | 1..6 | 1..6 | |
| 14 | 1..10 | 1..10 | |
| 15 | 8..12 | 8..12 | |

Apart from the known mission-0 state-3 merge, **mission 145 is the only
remaining structural divergence**: the original splits the mission-145 state
machine into states 0/1 handled by compares and states 2..5 handled by a
`tbh` (`0x1393a2`: `cmp #1; beq state1; cbz r4, state0; b 0x13cacc`;
`0x13cacc`: `subs r0,r4,#2; cmp #3; tbh`), while our source has one
`switch (m_nState)` with cases 0..5 (`0x1392d0`: `cmp r4,#5; tbh`).

The original's state-0 body falls through to the state-1 body
(`0x1393fc m_nState=1` -> `0x1393fe`), and the state-1 body is the long
teardown ending at `0x13cac6`.  The original's state-0 body order is
`new Explosion(0)`, store `m_pExplosion`, `addFireStreaks()`,
`setScaling(12.0f)`, `m_nState = 1`; ours has `setScaling` before
`addFireStreaks`.

## Measured mission-145 variants (all on the 52,075 base body)

Every variant below keeps the same semantics; only the dispatch shape (and in
the last row the two-call order) changes.  All are measured with the exact
`compile_commands.json` command + gold link + GNU strip.

| variant | raw | size | `.rel.plt` |
|---|---:|---:|---|
| **base: single `switch` 0..5** | **52,075** | **57,232** | `289b2a06…` |
| `if (s==0){…} if (s==1){…} else switch 2..5` | 52,075 | 57,232 | identical (clang merges back to base) |
| nested `default: switch (s)` / `switch (s-2)` / `switch (local)` | 52,075 | 57,232 | identical (merged) |
| three `if`s (`if (s>=2) switch`) | 54,898 | 57,264 (+32) | diff |
| `if (s==0){…} if (s==1){…} else if (s>=2) switch` | 55,367 | 57,204 (-28) | diff |
| `if (s==0){…} if (__builtin_expect(s==1,1)){…} else switch` | 55,345 | 57,140 (-92) | diff |
| **two separate `switch`es (0..1, 2..5)** | **55,363** | **57,436 (+204)** | diff |
| two switches + `addFireStreaks` before `setScaling` | 55,363 | 57,436 (+204) | diff |
| two switches separated by a status getter call | 55,482 | 57,436 (+204) | diff |
| `volatile int state145 = m_nState; switch (state145)` | 54,843 | 57,216 (-16) | diff |

The faithful two-switch form is real (it reproduces the original's
`cmp #1; beq; cbz; b switch` + `subs #2; cmp #3; tbh` shape), but on our body
it costs **+204 B** and +3,288 raw bytes: the split changes the block layout of
the whole function and widens branches in later missions.  The base single
`switch` is the codegen local optimum.

The `addFireStreaks`/`setScaling` swap alone (matching the original's emitted
order) is raw **52,076** (+1), so the current order is better for our codegen.
All 6 permutations of the three `m_pExplosion`/`m_nState` statements after
`new Explosion(0)` were measured; none beats 52,075.

## Conclusion

* The iOS 1.1.4 re-diff confirms the wave-183/184/185 conclusions: no per-state
  helpers, no enum differences, no state split/collapse in mission 0/1.
* The complete `tbh` census isolates mission 145 as the one remaining
  state-machine structural divergence (states 0/1 split from 2..5).  It is a
  faithful structural fix but a net codegen loss on the current body, so it is
  **not landed**.
* No size-exact, `.rel.plt`-identical raw win was found.  `src/` is at the
  base.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13004
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52075 aligned= 20766 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52075 size=57232/57232 text=74402 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch files (`LevelScript.cpp.base`, `sweep145.py`, `orig.dis`, `ours.dis`,
`ios.dis`, `our_lines.json`, `objtest/`) live in
`/home/debian/GalaxyOnFire2/scratch/w13004/`; they are scratch, not tracked.
