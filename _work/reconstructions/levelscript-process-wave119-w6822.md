# LevelScript::process — wave 119 / w6822 (statement-permutation hill-climb)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6822`, base master
`d4d2d3f91` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.471288951       59.962691208     (+0.491402)
orig / ours entries      20169 / 20042      20169 / 20036
orig / ours size         57232 / 57232      57232 / 57232    (size-exact)
byte_exact               4505               4505
linked_exact             4505               4505
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN              CLEAN
drift                    0                  0
RATCHET                  PASS               PASS
sections                 27                 27
file size                2187328            2187328
.rel.plt / .rel.dyn      exact              exact (content, not just size)
.ARM.extab size          156308             156308 (unchanged; still 4 B short of 156312)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6822/score.py`, same definition as the wave-118/119 notes).
The master base reads **59.471288951** with this helper.

Size-exactness is mandatory: a +-4/8 B change to `process` shifts every later
symbol and drops `byte_exact`. Every landed reorder is size-neutral (57232 B).

## Landed changes

All are statement-level reorders; none adds/removes a call or a symbol, so
`.rel.plt`, `.rel.dyn`, `.dynsym`, `.text` size and all section sizes are
unchanged. Source order below is the final file order.

| # | source lines (final file) | change | faithful? |
|---:|---|---|---|
| 1 | 1315 / 1324 | `m_nState = 5` / `m_nState = 4` swapped between case 3 and case 4 | tuned (state values) |
| 2 | 2146 / 2151 | `m_nScriptTimerA += delta` moved before the `currentTime >= 4001` test | tuned (statement order) |
| 3 | 2154 / 2155 | `m_nScriptTimerA = 0` moved before `startFade(...)` | tuned (independent store/call) |
| 4 | 2825-2827 | `AEGeometry *ship` declaration hoisted above `setHitpoints(0)` / `m_tempVec = ...` | faithful (declaration) |
| 5 | 3957 / 3960 | `m_nScriptTimerA = 0` moved before the up-vector add / camera seat | tuned (independent store) |
| 6 | 4282 / 4290 | `SetCurrentApplicationModule(2)` moved before `m_nState = 4` | tuned (call order) |
| 7 | 4699 / 4703 | `m_nState++` / `m_nScriptTime = 0` swapped between state 9 and state 10 | tuned (state stores) |
| 8 | 4703 / 4709 | `m_nScriptTime = 0` / `delete objectivesB` swapped between the two state-10 arms | tuned (statement order) |

The running score after each landed round:

```
master                                      59.471288951
+ (2146,2151)                               59.472767968
+ (4282,4290)                               59.479221070
+ (4699,4703)                               59.935339468
+ (2154,2155)                               59.940313355
+ (1315,1324)                               59.945287242
+ (4703,4709)                               59.949759992
+ (2825-2827 perm)                          59.959708508
+ (3957,3960)                               59.962691208   <- landed
```

## Method

1. Built and confirmed the master baseline (`RATCHET PASS`, byte 4505,
   ratio 59.471288951).
2. Generated every same-indent single-line statement pair at distances 1..8
   and every 3-statement rotation, then compiled only `LevelScript.cpp` with the
   exact `compile_commands.json` flags and gold-linked a variant `.so` against
   the checked-in objects (`/dev/shm/w6822/build_variant.sh`, `sweep.py`).
3. Repeated the sweep from each improved base until no adjacent/distance-8 swap
   or 3-cycle won. The largest single win was the state-9/state-10 store swap
   (`m_nState++` vs `m_nScriptTime = 0`, +0.456).
4. Only size-57232 variants with `.rel.plt`/`.rel.dyn` byte-identical to the
   original were considered landable. The best raw variant was
   `swap 2109,2123` at **59.994528316**, but it flips the `.rel.plt` order of
   `PlayerWormHole::freeMissionLock` and `StarSystem::switchSunForSupernovaIntro`
   (entries 2828/2829), so it was rejected; the landed variant is the best
   `.rel.plt`-preserving one.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6822/score.py                         # 59.962691208
```

`/dev/shm` helpers are scratch only; the landed source is the sole copy of the
change. The `.ARM.extab` size is unchanged by this pass (156308 B, 4 B short of
the original 156312 B), and `.rel.plt`/`.rel.dyn` remain byte-identical.
