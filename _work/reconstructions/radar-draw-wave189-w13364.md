# Radar::draw wave 189 (w13364) — `camZ` hoisted out of the gas-cloud loop;
# raw 6,308 -> 6,301 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13364`, branch `wave189-w13364`, base `master`
`30caae150`. Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13331.md`, `-w13335.md`, `-w13342.md`, `-w13346.md` and
`-w13356.md`. This pass deliberately avoided the planet-loop / register-skeleton cells
(w13331/w13335/w13342/w13346) and the landmark/asteroid ternary cells (w13356), and worked
the gas-cloud loop instead.

## 0. The win

One source edit in the gas-cloud loop header:

```cpp
     if (this->gasClouds != nullptr) {
         bool gasHit = false;
+        const auto camZ = this->cameraZ;
         for (uint32_t i = 0; i < this->gasClouds->size(); ++i) {
-            auto camZ = this->cameraZ;
             PlayerGasCloud *cloud = (PlayerGasCloud *) this->gasClouds->data()[i];
```

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,308** | **6,301** |
| instruction-index alignment | 1,858 | 1,876 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,554 | **73,547** |
| `.ARM.extab` differing bytes | 610 | 610 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** (29,392 B, diff 0) |
| `.rodata` | exact | exact |
| build-id differing bytes | 20 | 20 |
| total differing bytes | 74,184 | **74,177** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370. The function was already non-byte-exact, so
`byte_exact` is unchanged; the gain is in the raw residual only.

## 1. What the edit actually changes

The win is not in the gas-cloud body; it is in the **prologue of the gas-cloud loop**.
Hoisting the `cameraZ` load makes clang materialise it as `vldr s16,[r4,#580]` at
`12f43c`, next to the two literal-pool `vldr`s, instead of leaving the loop body's
`vldr s20,[r4,#580]` at `12f456`. The base's 24-byte mismatch run at
`12f442-12f45a` splits into 12 + 6 bytes, and one further stray byte at `12f822`
disappears:

```
base   12f442-12f45a  len=24      (24 differing bytes)
after  12f442-12f44e  len=12
       12f450-12f456  len=6       (18 differing bytes)
```

`cameraZ` is assigned once at `Radar.cpp:367` (`this->cameraZ = this->transform.m[11]`)
and is not written anywhere in the gas-cloud loop, so the hoist is semantically
equivalent to the per-iteration read; the original binary also reloads it at the
`calcDistance` call site, which is why the base's in-loop spelling was plausible but
byte-worse.

## 2. Cells measured this pass (all reverted except the landed one)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13364/`).

### 2.1 Gas-cloud sparks in-sight block (the handed-off lead)

`int top` (both bounds) is the original's arithmetic (the decompiler shows
`iVar10 = (int)(crosshairPos.y - size)` and `iVar15 = (int)(crosshairPos.x - size)`), but
every spelling is raw-worse because the block sits +12 B from the original:

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base (`float top`, `(int) top` twice) | **6,308** | 1,858 | 9,908 |
| `int top` first, `int left` second | 6,319 | 1,840 | 9,908 |
| `int left` first, `int top` second | 6,320 | 1,837 | 9,908 |
| `int top` + `Vector cross = crosshairPos` | 6,319 | 1,840 | 9,908 |
| `int top` + `left < screenX` operand order | 6,319 | 1,840 | 9,908 |
| `int top` + negated `<=` comparisons | 8,274 | 1,886 | **9,904** |

### 2.2 Landmark crosshair block

The original computes `left` as an `int` too (`iVar51 = (int)(crosshairPos.x - size)`),
and the decompiler's operand order is `left < screenX`, then the y pair, then
`screenX < left + 2*size`. The base uses `float left`; making it `int` is size-exact but
raw-worse:

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base (`float left`) | **6,308** | 1,858 | 9,908 |
| `int left` | 6,311 | 1,848 | 9,908 |
| `int left` + decompiler operand order | 6,313 | 1,805 | 9,908 |
| `float left` + decompiler operand order | 6,314 | 1,808 | 9,908 |
| `int left`, `top` declared first | 6,311 | 1,848 | 9,908 |

### 2.3 Gas-cloud loop counter / `camZ` placement

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,308 | 1,858 | 9,908 |
| **`const auto camZ` before the loop (landed)** | **6,301** | 1,876 | **9,908** |
| `float camZ` before the loop | 6,301 | 1,876 | 9,908 |
| `auto camZ` before the `gasClouds != nullptr` guard | 6,301 | 1,876 | 9,908 |
| `auto camZ` at function scope | 7,103 | 1,937 | 9,908 |
| loop counter `int`/`unsigned`/`size_t`/`int32_t` | 6,308 | 1,858 | 9,908 (byte-identical) |
| loop counter declared before the `for` | 6,308 | 1,858 | 9,908 (byte-identical) |
| `volatile` loop counter | 8,286 | 1,977 | **9,900** |
| `gasCount` limit local | 8,356 | 2,329 | **9,920** |
| `camZ` + `int left` landmark | 6,304 | 1,866 | 9,908 |
| `camZ` + `int top` gas sparks | 6,312 | 1,858 | 9,908 |

### 2.4 Stack-slot / register-allocation probes (all neutral or worse)

The residual `turretMode`/`planetHit`/`inCenter` slot swap (`turretMode` is `sp+0x98`
in ours vs `sp+0x94` in the original; `planetHit`/`inCenter` is `sp+0x94` vs `sp+0x98`)
is **not** reachable from source spelling: `int`/`unsigned`/`long`/`int32_t`/
`uint32_t` `inCenter`, declaration-order permutations, hoisting `inCenter` or
`planetHit` to function scope, and `bool turretMode != 0` / `(bool)` casts are all
byte-identical to the base. `unsigned char`/`short`/`int8_t` `turretMode` collapse to
the `int` body (9,238 / 9,856). The `alwaysEnemySeen` slot (`sp+0x5c` ours vs `sp+0x60`
original) is likewise fixed by the same campaign-skeleton allocation.

`alwaysEnemySeen`/`alwaysEnemy` declaration permutations, `uint8_t`/`char`/
`unsigned char` `alwaysEnemySeen`, eliminating `alwaysEnemy`, and assigning
`alwaysEnemySeen` in-loop are all neutral or worse (8,490-8,997; several +4 B).

## 3. Where the residual still lives (unchanged)

The 46-byte mismatch run at `12f79e-12f7cc` is the gas-cloud loop counter save/restore:
ours keeps `i` in `fp` (`mov fp,r5` at `12f620`, `mov r5,fp` at `12f74e`/`12f79e`),
the original spills it (`str r5,[sp,#152]` at `12f624`, `ldr r5,[sp,#152]` at
`12f7ca`). That is the same campaign-in-`sl`/zero-in-`fp` register skeleton pinned by
w13331/w13335/w13342: in the original `fp` is the planet-loop counter, in ours `sl` is,
so the greedy allocator hands `fp` to the gas-cloud counter instead of spilling it.
Fixing it needs the campaign skeleton, which is net raw-worse on every size-exact body
found so far.

The rest of the residual is the planet-loop latch/block placement (`12e3ce-12e7aa`),
the enemy loop (`12ef6d-12f7cc`), the wreck x/y block (`12fc4c-12fca0`, 12 B shorter
than the original because clang CSE-hoists `screenX`), and the music tail
(`130264+`, `r8` vs `r9` for `FModSound*` and `sp+0x60` vs `sp+0x5c` for
`alwaysEnemySeen`).

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6301 aligned= 1876 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13364/`: `Radar.base.cpp` (6,308 base),
`v_camz_outside.cpp` / `v_camz_out_const.cpp` (6,301), `v_lm_*`, `v_gc_*`, `v_ic_*`,
`v_tm_*`, `v_ph_*`, `v_*_flip*`, `rawruns.py`, `heat.py`, `shift.py`, `adiff.py`,
`radar_orig.c` (Ghidra decompile at `0x13e050`), `ghidra_scripts/DecompRadar7.java`.
