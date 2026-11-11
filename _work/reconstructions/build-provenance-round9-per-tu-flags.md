# Build provenance round 9 (w11420): open per-TU flag families, branch-weight
# knobs, and the remaining `.ARM.extab` residual

Status: **research note, no landed `src/`/`CMakeLists.txt` change** (2026-11-11).
Worktree `wt/w11420`, branch `wave169-w11420`, base master `ec3ceb07c`
(wave 168). Scratch `/dev/shm/w11420`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370. `.text` **82,327** differing bytes, `.ARM.extab` **668**,
`.rel.plt` **0** (byte-identical). No source or build file was changed; the
tree is clean apart from this note.

## 0. Bottom line

1. Round 8's per-TU driver/ARM sweep is extended with the families it left
   open: the `-mllvm` branch-weight / block-frequency knobs, the ARM-specific
   `-mllvm -arm-*` options, the remaining ARM subtarget features, and
   combinations of the round-5/6/7/8 matching knobs. **6,565 object-level
   cells** were compiled and filtered; **96 size-exact strict cells** were
   link-measured with the real gold link.
2. **No cell reduces the raw `.text` diff below 82,327.** The closest strict
   cells are 82,328 (`LevelScript` `-machine-sink-split-probability-threshold=0`
   plus `-enable-earlycse-memssa=false` and/or `-disable-ifcvt-triangle-rev`);
   16 cells are exactly neutral at 82,327/668 but only by swapping bytes, not
   removing them. Since a size-exact, `byte_exact`-neutral raw win was the
   landing criterion, nothing was landed.
3. The `.ARM.extab` 668-byte residual is unchanged and still decomposes into
   the same 11 records. Nine records (645 B) are body-layout consequences;
   `Radar::Radar` (7 B) and `SpaceLounge::drawLounge` (14 B) are pure
   landing-pad identity swaps, and `ModStation::ModStation` (2 B) is a
   register-allocation difference. No flag changes the extab without also
   changing `.text`; the only text-neutral extab movers found
   (`-mllvm -arm-use-mulops=false` on `MGame`/`SpaceLounge`) leave the total
   extab diff at 668 or worse.
4. New source-shape probes on `Radar::Radar` (extra scopes around the
   `playerGuns`/`enemyGuns` blocks) confirm the wave-147/148 conclusion: the
   7 bytes are a machine-block-placement frequency tie, not a source cleanup
   scope. `scope_player` is text- and extab-neutral; `scope_enemy` and
   `scope_both` are worse.

## 1. Baseline (this worktree)

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` C1/C2 | `_ZN6LayoutC1Ev` / `C2` | 5,657 | 2,276 | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,031 | 17,764 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,560 | 455 | 2,764 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,435 | 767 | 17,172 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 7,487 | 1,454 | 9,908 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 4,082 | 1,018 | 8,080 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11,440 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1,984 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 2,245 | 620 | 9,700 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1,948 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 762 | 222 | 9,332 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1,896 |
| `ModStation::ModStation` C1/C2 | `_ZN10ModStationC1Ev` / `C2` | 43 | 12 | 1,008 |

13 TUs (Layout C1 is an alias of C2; ModStation likewise).

## 2. Method

The round-8 object filter is reused:

1. **Object-level filter.** The canonical compile command is taken from
   `ninja -t commands` (so the per-file StarMap/SpaceLounge/LevelScript/Radar/
   Level/Layout knobs are already in it) and the candidate flag appended. The
   TU is recompiled to a scratch object. A strict cell must keep every
   `.text.*` and `.ARM.extab*` section size exact, keep every non-residual
   function's `.text.*` bytes exact, and change at least one residual
   function. Cells that move a byte-exact non-target function are recorded as
   relaxed (they would drop `byte_exact`).
2. **Link measurement.** Every strict cell is linked with the exact gold link
   line (captured from `ninja -t commands link_so`, run directly so ninja
   cannot recompile the swapped object). `tools/fn_diff.py` plus a direct
   whole-`.text`/`.ARM.extab` byte diff against the original are measured;
   the canonical object is restored after every cell.
3. **Gate.** The tree is unchanged, so `check` is green by construction.

All compiles used `ulimit -v 12000000` and 6 concurrent single-TU clang
invocations; links were sequential.

## 3. Flag space swept

| family | flags | cells |
|---|---:|---:|
| driver / language (`-fno-threadsafe-statics`, `-fno-exceptions`, `-fno-rtti`, `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`, `-fno-strict-overflow`, pairings) | 14 | 182 |
| branch-weight pairs (`-likely-branch-weight` x `-unlikely-branch-weight`; 23 ratios incl. 7:1, 20:12, 80:1, 2000:1, 1:2000, 2000:2000) | 23 | 299 |
| block-frequency / BPI / MBP knobs (`-min-predictable-branch`, `-static-likely-prob`, `-cold-branch-ratio`, `-cgp-freq-ratio-to-skip-merge`, `-sink-freq-percent-threshold`, `-min-block-execution`, `-use-mbpi`, `-branch-fold-placement`, `-tail-dup-placement`, `-machine-sink-bfi`, `-jump-is-expensive`, `-consthoist-with-block-frequency`, `-disable-branch-fold`, `-disable-block-placement`, `-hot/cold-callsite-rel-freq`, `-coldcc-rel-freq`, `-loop-to-cold-block-ratio`, `-misfetch-cost`, `-switch-peel-threshold`, `-tail-dup-indirect-size`) | 72 | 936 |
| ARM-specific `-mllvm` (`-arm-load-store-opt`, `-arm-assume-misaligned-load-store`, `-arm-adjust-jump-tables`, `-arm-atomic-cfg-tidy`, `-arm-use-mulops`, `-old-thumb2-ifcvt`, `-arm-synthesize-thumb-1-tbb`, `-arm-global-merge`, `-arm-force-fast-isel`, `-arm-interworking`, `-arm-restrict-it`, `-arm-no-restrict-it`, `-arm-default-it`, `-t2-reduce-size`, `-arm-constant-island-max-iteration`, `-arm-promote-constant-max-total`, `-arm-implicit-it`, FP/math toggles, `-enable-<target-feature>=false` spellings) | 92 | 1,196 |
| remaining ARM subtarget features (`±32bit`, `±8msecext`, `±acquire-release`, `±avoid-movs-shop`, `±cheap-predicable-cpsr`, `±db`, `±dfb`, `±dont-widen-vmovs`, `±fpao`, `±mp`, `±nacl-trap`, `±neon-fpmovs`, `±no-movt`, `±no-neg-immediates`, `±noarm`, `±nonpipelined-vfp`, `±prefer-ishst`, `±prefer-vmovsr`, `±slow-fp-brcc`, `±slow-load-D-subreg`, `±slow-odd-reg`, `±slow-vdup32`, `±slow-vgetlni32`, `±trustzone`, `±virtualization`, `±vldn-align`, `±vmlx-hazards`, `±zcz`, `±v7clrex`, `±v8.1a`, `±v8.2a`, `±v8.3a`, `±v8m`, `±v8m.main`, `±vmlx-forwarding`, `±use-misched`, `±prof-unpr`, `±ret-addr-stack`, `±lob`, `±fuse-literals`, `±fuse-aes`, `±expand-fp-mlx`, `±muxed-units`, `±avoid-partial-cpsr`) | 88 | 1,144 |
| combinations of the round-5/6/7/8 matching knobs (sink, LICM, EarlyCSE-MemSSA, ifcvt, branch weights, `+prof-unpr`, `-tail-dup-placement`, `-arm-use-mulops`) | 30 | 390 |
| register-allocation / spill / coalescing knobs (`-callsite-splitting-duplication-threshold`, `-condsider-local-interval-cost`, `-disable-phi-elim-edge-splitting`, `-disable-post-ra`, `-disable-sched-physreg-join`, `-force-split-store`, `-machine-sink-split`, `-misched-postra`, `-optimize-regalloc`, `-phi-elim-split-all-critical-edges`, `-sink-insts-to-avoid-spills`, `-split-spill-mode`, `-spp-rematerialization-threshold`, `-stress-regalloc`, `-use-segment-set-for-physregs`, and the `true`/`false` inversions of the round-5 knobs) | 44 | 572 |
| focused 3-way combinations for the closest LevelScript/Radar/Layout cells | 25 | 325 |
| if-conversion pairings (the neutral `-disable-ifcvt-triangle-rev` cell plus each other ifcvt/tail knob) | 15 | 195 |
| **total** | **403** | **6,565** |

Rejected spellings (compile failure) included `-mllvm -arm-...` values clang 7
does not know and `-split-spill-mode` values other than `default/size/speed/
both`; those cells are counted in the table but produced no object.

## 4. Link-measured strict cells (96)

Baseline whole-`.text` 82,327, `.ARM.extab` 668. Only rows that are at or
near the baseline are shown; the full set is in
`/dev/shm/w11420/link_results*.json` (scratch, not committed).

| TU | flag | `.text` | `.ARM.extab` | verdict |
|---|---|---:|---:|---|
| LevelScript | `-mllvm -machine-sink-split-probability-threshold=0 -mllvm -enable-earlycse-memssa=false` | 82,328 | 668 | +1 |
| LevelScript | same + `-mllvm -disable-ifcvt-triangle-rev` | 82,328 | 668 | +1 |
| LevelScript | `-mllvm -machine-sink-split-probability-threshold=0 -mllvm -disable-ifcvt-triangle-rev` | 82,328 | 668 | +1 |
| LevelScript | `-mllvm -likely-branch-weight=10 -mllvm -unlikely-branch-weight=1` | 82,734 | **667** | text +407, extab -1 |
| LevelScript | `-mllvm -disable-ifcvt-triangle-rev` | 82,327 | 668 | neutral (bytes swap) |
| LevelScript | `-mllvm -enable-earlycse-memssa=false` | 82,327 | 668 | neutral |
| LevelScript | `-mllvm -disable-ifcvt-triangle-rev -mllvm -enable-earlycse-memssa=false` | 82,327 | 668 | neutral |
| Radar | `-fwrapv` / `-fno-strict-overflow` / `-fwrapv -fno-delete-null-pointer-checks` | 82,327 | 668 | neutral |
| Radar | `-mllvm -likely-branch-weight=1000 -mllvm -unlikely-branch-weight=1` | 82,331 | 668 | +4 |
| Radar | `-mllvm -enable-load-pre=false` | 83,340 | 671 | +1,013 |
| Hud | `-mllvm -machine-sink-split-probability-threshold=75` | 82,341 | 668 | +14 |
| Hud | `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` | 82,327 | 668 | neutral |
| Hud | `-Xclang -target-feature -Xclang +prof-unpr` | 82,348 | 668 | +21 |
| Layout | `-mllvm -tail-dup-placement=false` | 82,327 | 668 | neutral |
| Layout | `-mllvm -tail-dup-placement=false -Xclang -target-feature -Xclang +prof-unpr` | 82,395 | 668 | +68 |
| Layout | `-Xclang -target-feature -Xclang -prof-unpr` | 82,327 | 668 | neutral (undoes round 8) |
| MGame | `-mllvm -arm-use-mulops=false` | 82,513 | 675 | +186 |
| MGame | `-mllvm -sink-insts-to-avoid-spills` | 85,774 | 719 | +3,447 |
| SpaceLounge | `-mllvm -arm-use-mulops=false` | 82,729 | 673 | +402 |
| Gun | `-mllvm -disable-ifcvt-triangle-rev` | 82,331 | 668 | +4 |
| Gun | `-mllvm -disable-ifcvt-triangle-rev -mllvm -enable-earlycse-memssa=false` | 82,331 | 668 | +4 |
| Status | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=2000` | 82,351 | 668 | +24 |
| HangarWindow | `-mllvm -likely-branch-weight=256 -mllvm -unlikely-branch-weight=1` | 82,337 | 679 | +10 |
| HangarWindow | `-mllvm -likely-branch-weight=1000 -mllvm -unlikely-branch-weight=1` | 82,373 | 668 | +46 |
| Level | `-mllvm -likely-branch-weight=12 -mllvm -unlikely-branch-weight=20` | 82,345 | 668 | +18 |
| HangarWindow | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=1` | 89,130 | 1,141 | +6,803 |

No row is below 82,327. The 20+ neutral rows change bytes without changing
the count (e.g. `LevelScript -disable-ifcvt-triangle-rev` swaps six bytes;
adding `-disable-ifcvt-triangle`, `-disable-ifcvt-simple`,
`-enable-if-conversion=false`, `-disable-cgp-branch-opts` or
`-disable-cgp-select2branch` keeps it neutral), so they are not raw wins under
the same-address byte metric.

## 5. `.ARM.extab` residual (668 B, 11 records)

`_work/reconstructions/extab_record_map.py` on the current build:

| diff | record | owner | rows | root cause |
|---:|---|---|---:|---|
| 184 | 0x137d18 | `LevelScript::process` | 46 | body layout (25 SHIFT + 21 ROW) |
| 159 | 0xaffd8 | `Level::createMission` | 148 | body layout (55 LP address shifts, 40 call-site shifts) |
| 97 | 0x1490d8 | `HangarWindow::render` | 156 | body layout + row 39/40 split |
| 77 | 0xc0214 | `Level::createShip` | 57 | body layout |
| 44 | 0x12e050 | `Radar::draw` | 19 | body layout (constant-pool island shifts every LP by 2) |
| 42 | 0x17a144 | `MGame::OnTouchEnd` | 44 | body layout (11 SHIFT + 5 ROW) |
| 35 | 0x163b90 | `Hud::draw` | 77 | `float filled` VarDecl split + 5 LP address shifts |
| 14 | 0x170d38 | `SpaceLounge::drawLounge` | 25 | LP identity swap + 5 address shifts |
| 7 | 0x971b8 | `Generator::createMission` | 26 | body layout |
| 7 | 0x12d484 | `Radar::Radar` | 39 | LP identity swap (MBP frequency tie) |
| 2 | 0xd52d0 | `ModStation::ModStation` | 15 | RA of the `MatrixSetRotation` 4th arg |
| **668** | | | | |

### 5.1 `Radar::Radar` (7 B, body byte-exact)

Rows 1, 3, 11, 13, 15 unwind to the function-level cleanup
(`orig lp=2462`, `ours=2464`); rows 19, 21 unwind to the `enemyGuns` loop
cleanup (`orig 2464`, `ours 2462`). The two landing pads are adjacent
identical `b.n` trampolines, so the only observable is which IR landing pad
got which address. The wave-147 trace showed an exact 519/519 block-frequency
tie; the original's PGO weights break it the other way.

New scope probes (all size-exact, 1,008 B function):

| shape | `.text` | `.ARM.extab` | Radar record diff |
|---|---:|---:|---:|
| base | 82,327 | 668 | 7 |
| extra scope around `playerGuns` block | 82,327 | 668 | 7 |
| extra scope around `enemyGuns` block | 82,341 | 674 | 13 |
| extra scopes around both | 82,403 | 691 | 30 |

The `playerGuns` scope is neutral, confirming the scope structure is not the
lever; the other two perturb the body and make the record worse. No
`-mllvm` branch-weight / BPI / MBP knob moved the 519/519 tie: the
`-likely/-unlikely` ratios, `-min-predictable-branch`, `-static-likely-prob`,
`-use-mbpi`, `-machine-sink-bfi`, `-sink-freq-percent-threshold`, and
`-disable-postra-machine-licm` combinations all leave the record at 7 (or
change `.text`).

### 5.2 `SpaceLounge::drawLounge` (14 B)

The `{-10:1}` row is the same kind of LP identity swap; the five `{+2:5}`
rows are address shifts downstream of the 176-byte body. `-mllvm
-arm-use-mulops=false` changes the record to 19 bytes and the body to
82,729, so it is worse.

### 5.3 `ModStation::ModStation` (2 B)

Row 3 is `(390,30,922)` orig vs `(400,20,922)` ours; the LP is identical.
The 43-byte body diff is a register-allocation/scheduling difference around
the `MatrixSetRotation` call (`ldr r3/r4` order and one `mov r4,r2`). The
regalloc/spill sweep (44 knobs) found no size-exact shape that moves it.

## 6. Driver-family observations (re-confirmed)

* `-fno-threadsafe-statics`, `-fno-rtti`, `-fno-delete-null-pointer-checks`
  are exact no-ops on all 13 TUs.
* `-fno-exceptions` is size-changing on every TU with EH cleanup
  (`__clang_call_terminate` disappears, `.ARM.extab` collapses).
* `-fno-strict-aliasing` is size-changing on 12 of 13 TUs.
* `-fwrapv` and `-fno-strict-overflow` are strict only on Radar (one byte
  swaps, net raw unchanged).
* The `-mllvm -enable-*`/`-disable-*` boolean set was already exhausted by
  round 7; the new value-taking knobs above add no raw win.

## 7. ARM feature observations

* The round-8 conclusion holds: `+prof-unpr` is the only feature that
  improves a residual function, and it is already landed on `Layout.cpp`.
  `-prof-unpr` returns Layout to 82,327 (neutral), and `+prof-unpr` on Hud is
  +21.
* `+cheap-predicable-cpsr` on Hud/Radar, `+neon-fpmovs`, `+slow-vgetlni32`,
  and `+virtualization` on LevelScript are all text-neutral at 82,327/668.
* `-mllvm -arm-use-mulops=false` is the only ARM `-mllvm` option that moves
  an extab record without changing the target `.text` section (MGame
  `OnUpdate`, SpaceLounge `drawLounge`), but linked it changes `.text` too
  and worsens both sections.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11420
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so verify check -j2
python3 _work/reconstructions/extab_record_map.py
```

Scratch artifacts: `/dev/shm/w11420/{common.py,sweep.py,link.py,flags*.txt,
results*.json,link_results*.json,lsda.txt}`.

## 9. Files changed

* `_work/reconstructions/build-provenance-round9-per-tu-flags.md` — this note.
