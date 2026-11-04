# MGame::OnUpdate — idle-contact `Status*` residual (wave 107, w5520)

Status: research pass, 2026-11-04. Worktree `wt/w5520`, branch `wave107-w5520`,
base `f857b5171`. **No source change landed**: the function is already at its
best measured shape (41 diff bytes, size exact, 99.9 %). This note extends
wave-97 (`mgame-onupdate-r5-cache-wave97-w4717.md`), wave-105
(`mgame-onupdate-wave105-w5312.md`) and wave-106
(`mgame-onupdate-wave106-w5408.md`) with the exact greedy-RA *decision path*
(which is sharper than the "static cost" framing of wave 106) and with a new
batch of measured source shapes.

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8 (offset 0 = 0x17c8d8).

## 1. Baseline (verified this session)

```
size 11440 11440  diffbytes 41  runs 1
0x17bb 0x17e4 41
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2767-2776`). Original vs ours:

```
original                                    ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                       17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx   getPlayingTime                 17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

The original has **four fresh `ldr.w rX,[r9]` loads and no cached `Status*`**;
size is neutral (4+4 = 4+2+2). `sl` is `&this->player` (set by
`ldr.w r0,[sl,#88]!` at 0x17cab2), `fp` is `this`.

## 2. Exact greedy-RA decision path (new this wave)

`-debug-only=regalloc` on the two faithful builds
(`/dev/shm/w5520/greedy_base_dbg.txt`, `greedy_nost_dbg.txt`) shows the
mechanism is **`tryAssign` vs `tryEvict`**, not a direct static-cost comparison:

* `%1053` (baseline) / `%1052` (no-`st`) is the long-lived
  `&Globals::status` GOT base (the `t2LDRi12 ... load 4 from got` value). It is
  a single vreg live across ~28 segments of the whole function.
* Greedy first assigns it r6, evicts, r10, evicts, **r9**, then it is evicted
  from r9 by `%2927` (a `COPY` of the `gLDRLIT_ga_pcrel @_ZL21gWingmenRadioMissions`
  literal, weight 5.64e-07 > the GOT base's 2.48e-07). The GOT base is requeued
  and ends in `selectOrSplit ... RS_Split`.
* `calculateRegionSplitCost` then splits it into a **global piece**
  (`%2938` baseline / `%2939` no-`st`, weight 2.62e-08, the later
  `[68808,102544)` bundles) and a **compact piece**
  (`%2931` baseline / `%2932` no-`st`, weight 8.70e-07, the hot region that
  contains the m23 block). Both pieces carry `hints: $r9`.
* The queue is size-ordered, so the **global piece is dequeued first** and
  takes r9 in both builds.
* Then the compact piece is dequeued:

  | build | what happens |
  |---|---|
  | baseline (`st`) | `tryAssign` finds **no free register** (r5 is occupied by `%128`/`st`, live `[42032,42608)` inside the compact region) -> `RS_Assign Cascade 0` -> `tryEvict` -> `should evict: %2938` (weight 2.6e-08) -> `evicting $r9 interference: Cascade 37` -> **compact piece gets r9**. |
  | no-`st` | `tryAssign` finds **r5 free** and returns it immediately (the hint r9 is occupied but the `missed hint` eviction path is not taken because the found register is not the hint) -> **compact piece gets r5**. |

So the `st` local is an **RA anchor that makes `tryAssign` fail**, which forces
the eviction path that hands the compact region r9. The static-cost table of
wave 106 (`$r5 static` 0.001645 vs 0.000456, `$r9 static` 0.000872 in both) is
the same decision seen through `addSplitConstraints`; the `tryAssign` view is
the one that explains why *any* r5 occupant inside the compact region is enough,
and why the occupant must be a real (non-optimised-away) vreg.

`$r9 static` is identical in both builds; only `$r5` changes (the `st` interval
is the only r5-assigned vreg that overlaps a compact-piece range in the
baseline). In the no-`st` build the other r5 values (`%1088 [43568,43776)`,
`%1151 [46416,46624)`, `%1255 [50512,50608)`, ...) all sit in the *gaps*
between the GOT base's live segments, so they do not block `tryAssign`.

## 3. Variants measured this wave

All measured with `link_so` + `/dev/shm/w5520/fdiff.py` (size, diff bytes,
runs). The baseline is the current source (`Status *st = Globals::status;`
used at `st->getPlayingTime()`).

| variant | size | diff | runs |
|---|---:|---:|---:|
| baseline `st` at getPlayingTime | **11440** | **41** | **1** |
| `st` declared before the inner `m_nScriptTime` if, used at m24 ccm | 11440 | 230 | 9 |
| same, used at m24 getMission | 11440 | 254 | 9 |
| same, used at m24 getPlayingTime | 11440 | 270 | 9 |
| same, used at m122 ccm | 11440 | 492 | 20 |
| `st` declared before the outer `elapsedTime` if, used at m24 ccm | 11392 | 8727 | 534 |
| `st` at m23 getMission only (no getPlayingTime use) | 11396 | 5129 | 240 |
| `st` at m23 getMission + getPlayingTime | 11436 | 4982 | 303 |
| `long long last = field;` before the if, `st` at getPlayingTime | 11436 | 4982 | 309 |
| `long long last = field;` before the if, no `st` | 11432 | 5040 | 270 |
| `long long last;` + comma-assign `last = field` in the condition, `st` at getPlayingTime | 11440 | 66 | 1 |
| `23 == ccm` / `3600000 < pt-field` / `>= 3600001` / `isMining()==0` / `isEmpty()==1` / fully parenthesised | 11440 | 41 | 1 |

The `long long last;` + comma variant is the closest non-baseline shape
(66 diff, 1 run): it keeps size and r9, but the field `ldrd r4,r8,[r0,#256]` is
issued *before* the `getPlayingTime` call instead of after it.

## 4. What is proven vs hypothesis

Proven:

* The whole residual is the m23 `Status*` cache decision; every other byte of
  the 11440-byte function matches.
* The original IR is the no-`st` shape (four distinct loads of
  `@_ZN7Globals6statusE`), as the native decompilation
  (`_work/oracles/.../native/game/MGame.c:4906`) also shows.
* The `st` anchor works by making `tryAssign` fail for the compact region
  (r5 occupied by `%128`), which forces `tryEvict` to evict the lower-weight
  global piece from r9. `$r9 static` is unchanged between builds; only `$r5`
  changes.
* Any `st` use that CSEs with the adjacent load (ccm or getMission) or that
  lives only across one call is not enough: the anchor only fires when `st` is
  loaded at the top (CSE with the ccm load) and used at `getPlayingTime`, i.e.
  live across the three intervening calls, which is why it has the highest
  weight of all tried shapes.
* All CSE-equivalent spellings of the baseline (`auto`, `const`, `&`,
  `(*st).`, `Status**`+`*sp`, `register`, casts) are byte-identical (41).
* All code-neutral rewrites of the m23 comparison (`23 ==`, swapped `>`,
  `>= 3600001`, `isMining() == 0`, `isEmpty() == 1`, extra parentheses) are
  byte-identical (41).

Hypothesis (not proven): the original source contains a code-neutral IR
difference that keeps a real vreg in r5 inside one of the compact-piece live
segments without emitting an instruction at the m23 block (e.g. a dead
live-through value). No such C++ construct was found; every construct that
survives optimisation either emits the `mov r0,r5` at the m23 block or
re-colours the whole function.

## 5. Structural state (unchanged)

No source change was made, so the ELF is untouched: 28 sections (rank table),
`.ARM.extab` still `0x26294` vs the original `0x26298` (4 B short; per-record
LevelScript +4, Hud +4, createShip -8, Generator -4). No extab/section delta
was introduced by this pass.

## 6. Gate (note-only pass)

`cmake --build cmake-build-match --target check -j6`:

```
compared 4524  avg 99.91%  byte_exact 4500  linked_exact 4500  fuzzy_perfect 4500
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500.

## 7. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5520/fdiff.py _ZN5MGame8OnUpdateEv        # size / diffbytes / runs
python3 /dev/shm/w5520/fdiff.py _ZN5MGame8OnUpdateEv -d     # side-by-side disasm
cmake --build cmake-build-match --target check -j6
```

Scratch drivers in `/dev/shm/w5520`: `fdiff.py`, `try.sh`, `var.py`,
`batch1..13.py`, `b15.py`, `MGame.orig.cpp`, the faithful LLVM-7.0.2 oracle
`/dev/shm/wf110/llvm702build/bin/llc`, and the regalloc dumps
`greedy_base_dbg.txt` / `greedy_nost_dbg.txt` / `ra_base.txt` / `ra_nost.txt`.
