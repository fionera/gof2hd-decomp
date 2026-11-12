# Radar::draw wave 191 (w13505) — asteroid-lock null test: ternary on the
# pointer *values* instead of on the field addresses; raw 6,059 -> 6,020

**Status:** landed, 2026-11-12. Worktree `wt/w13505`, branch `wave191b-w13505`,
base `master` `fc4e63303`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191-w13484.md`, `-w13489.md`, `-w13474.md`,
`radar-draw-wave190-w13493.md` (asteroid-lock tail, base 6,058).

## 0. The change

In the asteroid-lock tail (`Radar.cpp:1207`) the null test selected between the
two *field addresses* and then dereferenced:

```cpp
-                    if ((candidateWreck ? this->field_0x1c : this->lockedAsteroid) == nullptr) {
+                    if (candidateWreck ? this->field_0x1c == nullptr : this->lockedAsteroid == nullptr) {
                         this->field_0x10 = 0;
                         this->asteroidLockTimer = 0;
                     } else {
```

The two forms are semantically identical (`(c ? x : y) == nullptr` <=>
`c ? x == nullptr : y == nullptr`; all operands are pure pointer loads), but
the second makes each arm a *value* comparison, so clang emits the original's
conditional-load shape instead of a conditional move of the two precomputed
addresses:

```
ours before (10 B + 2 B cmov):        original / after (10 B):
  ldr  r1,[sp,#116]  ; &field_0x1c      cmp  r5,#0
  cmp  r5,#0                            ite  eq
  ldr  r0,[sp,#124]  ; &lockedAsteroid  ldreq r0,[sp,#124]  ; &lockedAsteroid
  it   ne                               ldrne r0,[sp,#116]  ; &field_0x1c
  movne r0,r1                           ldr  r0,[r0,#0]
  ldr  r0,[r0,#0]
```

The saved 2 bytes are exactly the 2-byte excess the tail carried, so the
following `12fe34` asteroid-timer block (already instruction-identical) snaps
back onto the original's addresses. That is why the raw win (-39) is much
larger than the 2 bytes removed: the whole `12fe34`-`12fe48` tail is now at the
original's addresses.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,059 | **6,020 (-39)** |
| instruction-index alignment | 2,027 | **2,024 (-3)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,801 | **71,762 (-39)** |
| `.ARM.extab` differing bytes | 582 | 582 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.note.gnu.build-id` | 20 | 20 |
| `.rel.plt` | byte-identical | **byte-identical** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

The function is already non-byte-exact, so `byte_exact` is unchanged; the gain
is 39 raw `.text` bytes.

## 1. Negative cells measured this pass (all reverted, size-exact unless noted)

Base 6,059 / 2,027 / 9,908. Scratch `/home/debian/GalaxyOnFire2/scratch/w13505/`
(`fast.py` 1 s compile+link harness, `m*.cpp` / `n*.cpp` / `t*.cpp` / `u*.cpp`).

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **ternary on values (landed)** | **6,020** | **2,024** | **9,908** |
| ternary on values + WT (faithful `asteroidLockTimer`) | 6,901 | — | **9,904** (fatal) |
| ternary on values + M3 (tractor reorder) | 6,022 | — | 9,908 |
| ternary on values + M3 + WT | 6,903 | — | 9,904 |
| inverted ternary `(!candidateWreck ? lockedAsteroid : field_0x1c) == nullptr` | 6,059 | 2,027 | 9,908 |
| local `KIPlayer **lock = candidateWreck ? ... ; if (*lock == nullptr)` | 6,059 | 2,027 | 9,908 |
| local `lock = !candidateWreck ? ...` | 6,059 | 2,027 | 9,908 |
| `*(candidateWreck ? &f : &l) == nullptr` (address deref form) | 6,059 | 2,027 | 9,908 |
| `candidateWreck ? f == nullptr : l == nullptr` with `== 0` | 6,020 | 2,024 | 9,908 |
| `navWreck` store: inverted ternary / local slot / value ternary | 6,059 | 2,027 | 9,908 |
| `navWreck` store: `navWreck ? f==nullptr : l==nullptr` (value form) | 6,060 | — | 9,908 |
| `navWreck` store: nested if/else / `&&`/`||` De Morgan | 6,060 | — | 9,908 |
| tractor guard `tractorMode2 && field_0x1c==nullptr && wreck` (M3) | 6,061 | 2,017 | 9,908 |
| tractor guard nested `if (tractorMode2) { if (field_0x1c==nullptr) { if (wreck) ... } }` | 6,061 | — | 9,908 |
| tractor guard `tractorMode2 && field_0x1c==nullptr` outer / `wreck` inner | 6,061 | — | 9,908 |
| tractor guard `wreck != 0` / `wreck == 1` / `!!wreck` / `(wreck & 1)` | 6,061 | — | 9,908 |
| tractor guard `field_0x1c==nullptr && tractorMode2 && wreck` | 6,060 | — | 9,908 |
| tractor guard `tractorMode2 && wreck && field_0x1c==nullptr` | 7,634 | — | 9,908 |
| tractor guard `(wreck & 1)` | 6,915 | — | 9,904 (fatal) |
| `__builtin_expect(tractorMode2, 0)` | 7,625 | — | 9,944 (fatal) |

The faithful `asteroidLockTimer = 0` tail (w13484 `WT`: `= 0` in the `best > -1`
block, `+= mode` in the else branch, `lockedAsteroid = nullptr`/`hasScanner`
removed) is still size-exact on its own (9,908) but raw 6,301; combined with the
value-ternary it becomes size-fatal (9,904), so it stays unlanded. The tractor
guard's branchy original shape (`cbz`/`cbnz`) is not reproducible from any
source spelling tried; clang always if-converts the last two operands.

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6020 aligned= 2024 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
