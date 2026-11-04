# Layout::Layout C1/C2 — REA block-tree evidence, shared-float round-trips,
# and a fresh exhaustive negative search (w5301, wave 105)

Worker **w5301**, branch `wave105-w5301`, base `835748c18`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

Continues `layout-layout-c1-c2-literal-pool-constants-wf102.md` (wave 102,
the landed 47.7724741448 state), `layout-layout-c1-c2-oracle-block-structure-wf301.md`
(wave 103), `layout-layout-c1-c2-ios-order-wf101.md` and
`layout-layout-c1-c2-pass11-w4802.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.77247414478918 %** (unrounded `fscore.py`, `autojunk=False`). Gate before
== after (`check` on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

This pass contributes one new piece of evidence (the **REA/Ghidra 12.1.2
decompilation of the actual Android 2.0.16 ctor**, not the wave-103
KiritoJPK oracle) and a set of exhaustive negative searches that close out
the remaining "move one statement / one if-block" hypothesis.

## 1. REA evidence: the actual source shape

Command (first import ~3 min):

```sh
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC \
  rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN6LayoutC1Ev --format json > /dev/shm/w5301/rea-layout-c1.json
```

The normalized pseudocode is saved at `/dev/shm/w5301/rea_layout_c1.c`
(1551 lines).  It agrees with the wave-103 oracle and adds the Android
2.0.16 constants.  The shape is:

```
cVar9 = Globals::iPadHD;
if (Globals::iPadHD == '\0') {
    if (Globals::n9 == 0) { ... per-field retina/default chains ... }
    else { ... n9 chains ... }
    /* 0x4 .. 0xa4 */
    ...
    *(int *)(this + 0x150) = (int)fVar33;
    if (cVar26 == '\0') {          /* iPadLarge */
      if (Globals::iPad == '\0') { ... }
      else { ... }
      fVar33 = (float)uVar13;      /* int -> float */
      *(int *)(this + 0x158) = (int)fVar33;   /* float -> int */
      ...
      fVar33 = (float)uVar15;
      *(int *)(this + 0x15c) = (int)fVar33;
      ...
    }
    fVar33 = (float)uVar13;
    *(int *)(this + 0x160) = (int)fVar33;
    ...
}
else {
    /* HD arm: one straight-line run of undefined4/undefined8 stores */
    *(undefined8 *)(this + 0x150) = 0x4600000005;
    *(undefined8 *)(this + 0x158) = 0xce00000046;
    ...
    fVar33 = 5.625;
}
LAB_000e21e2:
*(uint *)(this + 0x1e8) = uVar12;
*(int *)(this + 0x1e4) = (int)fVar33;
```

Two facts are directly visible and were not in the earlier notes in this
form:

1. **The source uses one shared `float fVar33`** for the non-HD per-field
   chains, and stores through `(int)fVar33`.  This is exactly why the
   original has the `vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32; vstr`
   round-trips at `0x158/0x15c/0x160/0x170` (and `0x1e4` in the common
   tail): the value is produced as an **int** in a branch, assigned to a
   **float** variable, then converted back to **int** for the store.
2. **The HD arm is a straight-line block** (`undefined4`/`undefined8`
   stores, `fVar33 = 5.625` for `0x1e4`), and the non-HD arm is the
   per-field branch tree.  This is the same tail-merged structure as the
   wave-103 oracle.

The current `Layout.cpp` is a flat ternary chain.  Clang folds
`(int)(float)(int-select)` back to an int select, so the round-trips do not
survive (verified below).

## 2. Exact remaining diff (current build)

Instruction counts and multiset delta (`verify.disassemble` + `reconcile`,
literal pools included):

| | total | code | `pool:` |
|---|---:|---:|---:|
| original | 2529 | 2491 | 38 |
| ours | 2499 | 2465 | 34 |

Top multiset deltas (ours − original):

```
 +72 str.w        -60 movs        +39 cmp.w       -20 cmp
 +22 beq.n        -17 strd        -15 add.w       -13 addw
 +12 it           -12 lsls        -11 cbz         -11 vst1.32
  +9 movne        -9 vld1.64       +8 b.n         +8 ldr.w
  -6 mov.w        +6 moveq.w       -6 vmov.f32     -5 beq.w
  -5 ldr          -5 ldrb          -5 ldrb.w       -4 clz
  +4 ite          -4 mov           -4 mvn.w        +4 orrs.w
  -4 pool         -4 vbsl          -4 vorr         +4 subs
  +3 adcs         +3 ands.w        -3 b.w          -3 bne.n
  -3 ldrd         +3 nop           +3 vbic.i16     +3 vceq.i16
  +3 vmov.u16     +2 add           -2 eor.w        +2 movwne
  +2 mvnne.w      -2 stmia         +2 str          +2 strb.w
  +2 subw         -2 vcvt.s32.f32  -2 vldr         -2 vstr
```

The dominant term is still **+72 `str.w` / −17 `strd`**: the original pairs
adjacent-address stores into `strd` far more often; ours emits two `str.w`.
The second term is **+39 `cmp.w` / −20 `cmp` / −11 `cbz`**: the original
keeps the flag bytes in low registers, ours in high registers.  Both are
downstream of the exact branch tree and the shared-float round-trips.

Window map (original address ranges, `difflib` match over normalized
instructions):

```
0xd1028 29/ 80=36.2%   0xd1928 26/ 79=32.9%
0xd1128 15/ 76=19.7%   0xd1a28 27/ 70=38.6%
0xd1228 42/ 81=51.9%   0xd1b28 15/ 73=20.5%
0xd1328 119/127=93.7%  0xd1c28 27/ 78=34.6%
0xd1428 98/125=78.4%   0xd1d28 32/ 96=33.3%
0xd1528 61/101=60.4%   0xd1e28 18/ 85=21.2%
0xd1628 67/ 97=69.1%   0xd1f28 17/ 99=17.2%
0xd1728 58/ 94=61.7%   0xd2028 12/ 76=15.8%
0xd1828 46/ 94=48.9%   0xd2128 24/ 77=31.2%
0xd2228 20/ 69=29.0%   0xd2728 31/ 88=35.2%
0xd2328 27/ 78=34.6%   0xd2828 82/123=66.7%
0xd2428 24/ 80=30.0%   0xd2928 40/126=31.7%
0xd2528 28/ 84=33.3%   0xd2a28 121/126=96.0%
0xd2628 45/ 91=49.5%   0xd2b28 50/ 79=63.3%
```

The worst windows are `0xd2028` (15.8 %, `0x170..0x18c` branch tree),
`0xd1f28` (17.2 %, `0x158..0x170` round-trips), `0xd1128` (19.7 %, HD
block) and `0xd1b28` (20.5 %, `0x14c..0x160`).  These are exactly the
regions the REA tree says are written as nested branches with `fVar33`.

## 3. What was tried this pass (all measured)

All sizes are real `link_so` sizes; all ratios are the official unrounded
`fscore.py` unless marked *fast* (`fastscore.py`, object-level, ~0.3 s,
ranks correctly but is ~0.08 pp optimistic).

### 3.1 Exhaustive single-statement moves (size-7072 filter)

For each of the six disputed HD/tail fields, moved the whole source unit to
every one of the 204 positions:

| field | result |
|---|---|
| `0x94`, `0x140`, `0x144`, `0x204`, `0x23c` (`blk16.b`), `0x2bc` | **no size-7072 position beats 47.7724** |
| `0x90`, `0x98` | no size-7072 position beats 47.7724 |

This is 8 × 204 = 1632 measured builds.  The previously reported
`0xac`↔`0x98` swap (6976 B / 48.1023) is still the best *size-changing*
single move; it is not landable.

### 3.2 Exhaustive pairwise swaps, first 50 units

All `C(50,2) = 1225` pairwise swaps of the first 50 source units: **no
size-7072 swap beats 47.7724**.

### 3.3 Exhaustive if/else-block moves

The body has 14 top-level units that are `if`/`else` blocks (units 36, 43,
93, 113, 115, 136, 147, 149, 152, 153, 164, 171, 177, 184).  Each was
moved to every one of the 204 positions: **2856 measured builds, no
size-7072 position beats 47.7724**.

### 3.4 Individual ternary→if/else conversion

For every one of the 189 simple-ternary units, replaced the unit with a
float-local `if/else` chain (`float f; if (...) f = ...; else ...;
this->field = (int)f;`) in place, keeping the rest of the body.  **No
size-7072 conversion beats 47.7724**; most are code-identical (clang folds
the float local back to a select).

### 3.5 Shared-float round-trip probes

The REA tree says the original uses a shared `float fVar33`.  Tried:

| variant | size | raw % |
|---|---:|---:|
| `float f;` at top + `f = ...; this->0x158 = (int)f;` for `0x158/0x15c/0x160/0x170` | 7072 | **47.7724741448** (code-identical) |
| same + `0x150/0x1e4` routed through `f` | 7072 | 42.6243 |
| per-field `float f` if/else for `0x158` | 7072 | 47.7725 (code-identical) |
| per-field `float f` if/else for `0x158`+`0x15c` | 7072 | 47.7725 (code-identical) |
| per-field `float f` if/else for `0x9c` | 7072 | 47.4448 |
| per-field `float f` if/else for `0x160` | 6968 | 47.7522 |
| per-field `float f` if/else for `0x1f4` | 6992 | 47.7636 |
| per-field `float f` if/else for `0x148` | 6968 | 47.6324 |

The flat ternary body lets clang fold `(int)(float)(select)` to an int
select; only a *real branch tree* (the REA shape) keeps the round-trips.
The shared-float probe shows the float variable alone is not enough.

### 3.6 The 6976-byte `0xac`/`0x98` body

The best size-changing body is the `0xac`↔`0x98` swap: **6976 B /
48.1022772673 %**.  It needs +96 B to land at 7072.  Tried:

* all 2^10 = 1024 subsets of the 10 known size-changing expression edits
  (`0x150` float, `0x158/0x15c/0x170` reorders, `0x160` all-float,
  `0x188` float, `0x194` float, `rewardBoxWidth=140`, `0x1f4` float,
  `0x9c` float) — **no subset lands at 7072 with >47.7724**;
* 2000 random single-unit moves from the 6976 body, size-7072 filter —
  **no move lands at 7072 with >47.7724**;
* moving the `0x8c..0xa4` region after `0x1e4`/`0x1e8`/`0x2f4` — all
  size-changing and worse (6984..6996 B, 21–41 %).

### 3.7 Other probes (all code-identical or worse)

* `field_0x1e8` as `(retina || iPadHD) ? 1 : (n9 ? 1 : 0)` — code-identical.
* `field_0xac`/`0xa8` as `(n9 || iPad) ? 61 : ...` — code-identical.
* `animSpeedScale`/`ringRadiusScale` as per-field float if/else, or
  `3.0f/2.0f` — code-identical.
* `field_0x9c` n9 arm as `(float)75.0` or a float-local — code-identical.
* Grouping `field_0x238_blk16.a/b/c/d` contiguously at every one of the
  204 positions — no size-7072 improvement.

## 4. Why the residue is (still) irreducible by local edits

The current body is a **flat ternary chain**.  Clang 7.0.2 `-Oz`:

1. hoists the `iPadHD` arm into one straight-line HD block (good: matches
   the original's HD block shape);
2. if-converts each non-HD per-field chain to a select (bad: the original
   keeps a real branch tree, so it has more `strd`/`cbz`/`movs` and the
   `(int)(float)` round-trips);
3. folds `(int)(float)(int-select)` to an int select (bad: the original's
   `fVar33` round-trips survive because `fVar33` is a shared float across
   branches).

Every local edit tried either (a) is folded away, or (b) changes the
function size away from 7072.  The 7072-byte constraint is what makes the
flat ternary hill order the metric optimum: the faithful REA tree compiles
to a *different* size (the wave-103 oracle block body was 6680 B; the iOS
order is 6936 B).

## 5. Next hypothesis

The only remaining route to byte-exactness is a **from-scratch
reconstruction of the REA tree**, not more hill-climbing:

1. Write the non-HD path as the actual nested `if/else` tree from
   `/dev/shm/w5301/rea_layout_c1.c` (or the tracked oracle at
   `_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Layout.c`),
   field by field, using one shared `float fVar33`.
2. Keep the HD arm as a straight-line block (the compiler will hoist it
   from the tree anyway).
3. Then tune the constants to the Android 2.0.16 values (the REA
   pseudocode already has them: e.g. `fVar33 = 5.625` for `0x1e4`,
   `0x158 = 0x46/0x64/0x32/...`, `0x9c = 0x46/0x4b/0x64/0x32`).
4. Expect a size near 7072; if it lands short, the missing bytes are the
   surviving `(int)(float)` round-trips (each ≈ 12 B) that the flat body
   folds away.

The current flat body should be kept as the metric baseline until that
tree is complete; do not land the iOS order or the 6976-byte body.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5301
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w5301/fscore.py                            # 47.7724741448, 7072 B
python3 /dev/shm/w5301/winmap.py                            # window match map
python3 /dev/shm/w5301/search_move.py                       # 6-field exhaustive moves
python3 /dev/shm/w5301/search_pair.py 0 50                  # pairwise swaps
python3 /dev/shm/w5301/search_unit.py                       # if/else-block moves
python3 /dev/shm/w5301/search_ifelse.py                     # per-unit if/else
python3 /dev/shm/w5301/subset_fast.py                       # 1024 subsets on 6976 body
python3 /dev/shm/w5301/rand6976.py 2000 7                   # random moves on 6976 body
```

Scratch (`/dev/shm/w5301`): `fscore.py` (official unrounded linked scorer),
`fastscore.py` (object-level triage), `winmap.py`, `search_move.py`,
`search_pair.py`, `search_unit.py`, `search_ifelse.py`, `subset_fast.py`,
`rand6976.py`, `rea_layout_c1.c` (REA normalized pseudocode),
`rea-layout-c1.json` (REA evidence record), `Layout.base.cpp` (pristine
ctor).
