# MGame::OnTouchEnd (8080 B) — w6107 (wave 113): instruction multiset is
# already exact; the residual 21 % is pure block placement

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `a979ace18`, branch
`wave113-w6107`.

## Result

**No gate-safe improvement found.**  HEAD stays at **8080 B /
78.9189 % / `.rel.plt` diff 0 / RATCHET PASS** (byte 4500, linked 4500,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
27 sections).  This note records one new, decisive measurement — the
function is *instruction-for-instruction identical* to the original once
branch targets and PC-relative pool offsets are normalised — plus the MIR
evidence for exactly which block placement differs and why the source
cannot move it.

## 1. New finding: the instruction multiset is already exact

Normalising the reconciled disassembly (replace every `b*`/`cbz`/`cbnz`
operand with `BR`, every `[pc, #N]` with `[pc,#X]`) and comparing
`collections.Counter` of the two 2775-entry streams leaves **six
entries**, all already known:

| original | HEAD |
|---|---|
| `cmp.w r9, #0` | `cmp.w r9, #4294967295` |
| `lsls r0, r0, #31` | `lsls r0, r0, #24` |
| `movs r0, #0` | `movs r1, #0` |
| `strb.w r0, [fp, #214]` | `strb.w r1, [fp, #214]` |
| `strb.w r0, [fp, #93]` | `strb.w r1, [fp, #93]` |
| (one extra `BR`) | (one extra `nop`) |

i.e. the mining-loop test, `bool` vs `char refinedAny`, and one
register-allocation difference in the weaponSlot body.  **There is no
missing or extra statement anywhere in the function.**  Consequently the
whole 21 % fuzzy gap is `SequenceMatcher` mis-alignment caused by block
ordering: every `b*` displacement and every `ldr rX,[pc,#N]` literal-pool
offset is different because the blocks sit at different addresses.

Corollary: any source edit that does not move blocks cannot raise the
score, and any edit that does move blocks can only be judged empirically.

## 2. MIR evidence: exactly one block placement differs

`-mllvm -print-machineinstrs` on the current source
(`/dev/shm/w6107/mir2.txt`, section at line 765329, "After Branch
Probability Basic Block Placement") gives the final layout.  The relevant
blocks (layout index → block):

```
idx 202  bb.342  $r0,$r2 = t2LDRDi8 $r11,116 ; $r1 = t2MOVi 1      (0x200 arm)
idx 209  bb.344  closeHudMenu ; ... useCloak                       (0x800 arm)
idx 224  bb.346  $r0,$r2 = t2LDRDi8 $r11,116 ; $r1 = t2MOVi 2 ; b bb.343  (0x400 arm)
idx 202  bb.343  tBL initHudMenu ; t2B bb.354                     (merged tail)
idx 349  bb.353  resumeSounds ; pauseOpen=0 ; hudMenuOpen=0 ; closeHudMenu  (weaponSlot body end)
idx 350  bb.354  ldr [r11,#248] ; cmp 0 ; bcc bb.358              (hudTouchFlags==0 test)
idx 351  bb.355  isAutoPilot ...                                  (function tail)
idx 352  bb.356  epilogue
idx 353  bb.357  __stack_chk_fail
idx 354  bb.358  closeHudMenu ; ... resetGunDelay                 (hud0 body)
```

* `bb.342` (the `0x200` arm, `movs r1,#1`) **falls through** into the
  merged `blx initHudMenu` block `bb.343`.
* `bb.346` (the `0x400` arm, `movs r1,#2`) **branches** to `bb.343`.
* `bb.353` (weaponSlot body end) **falls through** into the test `bb.354`.

The original is the mirror image:

```
17adc0  0x200 arm  ldrd ; movs r1,#1 ; b.n 17afde      <- branches
17afd8  0x400 arm  ldrd ; movs r1,#2                   <- falls through
17afde  blx initHudMenu
17afe2  test (ldr [fp,#248] ; cbz)
17afe8  tail (isAutoPilot)
...
17b9ca  weaponSlot=3 arm ; ... 17ba24 b.w 17afe8       <- branches to the tail
```

so the original's chain is
`[0x400 arm][blx][test][tail][epilogue][stack_chk_fail][hud0 body]`,
while HEAD's is
`[weaponSlot body][test][tail][epilogue][stack_chk_fail][hud0 body]`
with the merged `blx` stranded at idx 202 (0x17adcc).

This is *one* difference: which predecessor of the test block owns the
fall-through, and which of the two identical `initHudMenu` arms hosts the
merged tail.

## 3. Why the source cannot move it (BranchFolding mechanics)

The merge is `BranchFolder::CreateCommonTailOnlyBlock`
(`lib/CodeGen/BranchFolding.cpp:784`, called from
`MachineBlockPlacement` with `AfterBlockPlacement=true`).  It picks
`PredBB` (the layout predecessor of the test) if that block is in
`SameTails`; otherwise it takes the last block with the smallest
`EstimateRuntime(begin, TailStartPos)`.  Both `initHudMenu` arms have a
2-instruction prefix (`ldrd` + `movs`), so the tie-break is decided purely
by `SameTails` order, and `PredBB` is the weaponSlot body (not in
`SameTails`) — hence the `0x200` arm wins.

The test's fall-through is decided earlier, by
`MachineBlockPlacement::hasBetterLayoutPredecessor`
(`MachineBlockPlacement.cpp:1276`): a candidate edge `BB→Succ` is
rejected when any other predecessor `Pred` satisfies
`freq(Pred→Succ) * HotProb >= freq(BB→Succ) * (1-HotProb)` with
`HotProb = 80 %`.  The tail block has eight predecessors (all the early
`return` paths plus the weaponSlot body), one of which is hot enough to
reject `test→tail`.

Both heuristics are insensitive to C++ statement order; they only react to
CFG shape and edge frequencies.  Every CFG-shape change that makes the
weaponSlot body jump straight to the tail (`nested`, `hud0_ws`, `inv_ws`,
…) reproduces the original *edges* but re-lays the blocks as
`[weaponSlot body][tail]` with the test hoisted to 0x17aeb2, and costs
+4 B (8084) because the block order needs one more 32-bit branch plus a
`nop`.

## 4. Measurements (all on top of HEAD)

| variant | size | fuzzy (4 dp) | note |
|---|---:|---:|---|
| **HEAD** | **8080** | **78.9189** | gate-safe best |
| `nested` (hud0 check inside the wingman `else`) | 8084 | 76.6847 | original CFG, +4 B |
| `inv_ws` (`if (weaponSlot < 0) {wingman; hud0} else {ws}`) | 8084 | 76.6847 | same code as `nested` |
| `hud0_guard` (`if (hud != nullptr) closeHudMenu()` in hud0) | 8084 | 77.1974 | pre-`eb7439e53` shape |
| `ws_store_order` (`pauseOpen=0; hudMenuOpen=0; resumeSounds(); close`) | 8080 | 78.8829 | matches original store order, layout shifts |
| `ws_close_first` | 8080 | 78.8829 | |
| `khador_1st` / `khador_2nd` | 8092 / 8104 | 73.9287 / 74.1274 | |
| `khador_b400` / `khador_after400` | 8104 | 75.8683 / 75.8323 | |
| arm permutations `ABC` (HEAD) / `ACB` / `BAC` / `BCA` / `CAB` / `CBA` | 8080 / 8064 / 8080 / 8080 / 8064 / 8080 | 78.9189 / 71.9798 / 78.5225 / 78.5225 / 71.9077 / 78.8108 | |
| `hud0_before_ws` | 8048 | 72.9666 | |
| `hud0_after_decl` / `hud0_first` | 8080 | 76.0000 | |
| `swap4_80` | 8080 | 78.4505 | |
| `boolref` (`bool refinedAny`) | 8076 | 76.3518 | |
| `boolref+ne0` | 8072 | 70.6328 | |
| `m_ne0` / `m_plain` (`amount != 0` / `if (amount)`) | 8072 | 70.9782 | up-counter flip |
| `m_lt0` | 8084 | 71.9870 | |
| `strsplit` (split the `String` concatenation) | 8040 | 68.5590 | |
| `s127` (faithful separate `== 127` case) | 8100 | 75.1620 | |
| `disp` (pre-`ca43091c` campaign-first dispatch + hoisted `campaign`) | 8076 | 76.3295 | |
| `disp_b` (campaign-first dispatch, per-call fetch) | 8080 | 78.0461 | |
| `disp_c` (HEAD dispatch, hoisted `campaign`) | 8076 | 77.1310 | |
| `s127+disp` / `s127+disp_b` / `s127+disp_c` | 8096 / 8100 / 8096 | 74.3022 / 74.2888 / 75.1035 | |

Note: the wave-112 note quotes `disp` = 8088 / 79.0848 and
`s127+disp` = 8080 / 79.9063.  Those numbers do **not** reproduce from
the current master base (the exact `disp` spelling was not recorded in
that note); the three reconstructions tried here all land lower.  If a
future pass wants to chase the 79.9 % variant it must first pin down the
exact `disp` text.

## 5. Gate / section state (unchanged)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8 (size and
base exact), `.ARM.extab` 0x26298 (exact), `.rel.plt` diff 0, imports
207, lint CLEAN, drift 0, `RATCHET PASS` (byte 4500 / linked 4500,
avg 99.92, size 4524/4524, data 843/843).

```bash
cd /home/debian/GalaxyOnFire2/wt/w6107
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6107/score2.py                     # 8080 / 78.9189
```

Scratch (not tracked): `/dev/shm/w6107/` (`score2.py` prints the 4-dp
ratio, `multi.py` prints the normalised instruction multiset diff,
`try2.py`/`rep.py` rebuild one variant, `mir2.txt` is the current
`-print-machineinstrs` dump, `cc.sh` is the exact NDK clang++ command).
