# w4235: `Level::createMission` (90.7 %, 17172 B) — exact spill weights, the alienGuns
# `Vector` lifetime, and why the epilogue spill is a global StackSlotColoring outcome

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4235`, branch `wave92-w4235`,
base `ae1984b1`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every variant below is neutral or a regression.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.7 %  size 17172 B  @0xaffd8
gate: byte 4498 / linked 4498, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not committed.

## Result in one line

The raid/guard spill swap is **not** a local source-order effect. Measured on this
base the two slots are `raid = 1.301489` and `guard = 1.193760` (margin
**0.107729**, not the 0.004199 quoted in the wave-92 handoff, which is the stale
wave-90 base number). The wave-91 `bool alienWave` fix leaves both weights and the
epilogue frequency bit-for-bit unchanged. The only source variants that flip the
slot (`(security==3) && raid`, `campaign>=32 && raid`) change the CFG and the
function size (+24 B / +8 B), so they are not landable. The faithful alienGuns
`Vector` lifetime (`turretPos` initialised after `setRotation`, sharing
`turretDir`'s stack slot) is identified from the original disassembly, but it
shrinks the function to 17144 B and drops the fuzzy to 88.4 %.

## 1. Exact StackSlotColoring weights on this base

Method: `-mllvm -print-machineinstrs` gives the `After Virtual Register Rewriter`
machine function (pre-coloring frame indices), and
`-mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv`
gives the machine BFI (537 blocks, entry `bb.0` = 1.0; the second and third
emitted sections are identical and are the ones StackSlotColoring sees).
`LiveIntervals::getSpillWeight(false,true,MBFI,MI)` adds the referencing block
frequency once per FI operand, so a slot's weight is the sum of the frequencies
of the blocks that reference it (each raid reference is in a distinct block, so
this equals the distinct-block sum here).

```text
  slot (pre-color fi)   weight      final sp   refs
  raid        fi#86     1.301489    sp+88      19 (blocks below)
  guard       fi#74     1.193760    sp+84       2 (bb.1 store, bb.519 load)
  localRace   fi#83     5.488776    (other)
  alienWave   fi#102    0.527731    sp+64
  bigAlienWave fi#101   0.353174    sp+56
  pirateFortress fi#98  0.325035    sp+60
```

raid reference blocks and frequencies (sum = 1.301489):

```text
  bb.23  0.078125   bb.30  0.117190   bb.35  0.156250   bb.44  0.156250
  bb.52  0.156250   bb.63  0.061035   bb.67  0.063477   bb.69  0.039673
  bb.71  0.024796   bb.89  0.009155   bb.99  0.078125   bb.100 0.058594
  bb.106 0.036621   bb.110 0.019531   bb.114 0.078125   bb.118 0.078125
  bb.138 0.048828   bb.189 0.031738   bb.258 0.009601
```

The original is the mirror image: `raid` at `[sp,#84]` (17 refs) and the guard at
`[sp,#88]` (2 refs). Ours: `raid` at `[sp,#88]` (17 refs), guard at `[sp,#84]`
(2 refs). The raid reference *addresses* are identical in both binaries
(`b04c0 b04d8 b0546 b05da b0680 b0730 b07a6 b0882 b090a b0922 b0964 b098c b09d8
b0a24 b0a4a b2a0a b3758`), and the whole raid computation `b04a0`–`b04d8` is
byte-identical apart from the `str` offset (`9015` orig vs `9016` ours).

### Guard weight = entry + epilogue

`guard = freq(bb.1) + freq(bb.519) = 0.625000 + 0.568760 = 1.193760`.
The epilogue (bb.519) predecessor decomposition on this base (raw successor
weights divided by 2^31; single-successor edges are 100 %):

```text
  bb.6    freq 10.000000  edge 3.125 %  contrib 0.312500
  bb.133  freq  0.029297  edge 100 %    contrib 0.029297
  bb.307  freq  0.067096  edge 100 %    contrib 0.067096
  bb.318  freq  0.014850  edge 100 %    contrib 0.014850
  bb.325  freq  0.128350  edge 3.75 %   contrib 0.004813
  bb.326  freq  0.018601  edge 50 %     contrib 0.009300
  bb.357  freq  0.009300  edge 100 %    contrib 0.009300
  bb.368  freq  0.022461  edge 100 %    contrib 0.022461
  bb.382/396/402/417/440/454/465/488 (0.011230 each, 100 %) = 0.089840
  bb.517  freq  0.009300  edge 100 %    contrib 0.009300
  total = 0.568759  (matches freq(bb.519) = 0.56876)
```

The largest term is the alien-orbit loop header `bb.6` (freq 10.0, exit
probability 4/128 = 3.125 %, LLVM 7's `LBH_TAKEN_WEIGHT=124 / LBH_NONTAKEN_WEIGHT=4`
loop heuristic), which is fixed by the loop shape. To make the guard win by
weight alone the epilogue would have to rise by **> 0.107729** (to > 0.676489),
or raid's mass would have to fall by the same amount. That is a whole-CFG effect,
not a statement-order effect.

## 2. `bool alienWave` does not move the weights (wave-91 negative result confirmed)

`bool alienWave = false;` reproduces the original's materialised
`(i != X) | !alienWave` at `b2e04`, but on this base:

```text
                         baseline        bool alienWave
  raid weight            1.301489        1.301489
  guard weight           1.193760        1.193760
  freq(bb.519)           0.568760        0.568760
  size                   17172 B         17156 B
  fuzzy                  90.7 %          89.2 %
  guard_sp               84              84
```

So the b2e04 CFG shape is not the upstream lever for the slot swap (it is still
a real source-shape discovery for the b2e04 region itself, but it is not
landable because it changes the size).

## 3. The alienGuns `Vector` lifetime (the `turretPos`/`turretDir` slot merge)

Source (`Level.cpp` ~1945–1975, inside `if (alienGuns != 0)`):

```cpp
Matrix rotation;
Vector v = {0.0f, 0.0f, 0.0f};
Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
int k = 0;
for (...) {
    ...
    const float *t = (bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets)[k];
    Vector turretPos;                       // default-constructed (zero stores)
    Vector turretDir = {t[3], t[4], t[5]};
    v = MatrixRotateVector(rotation, turretDir);
    gun->geometry->setRotation(v);
    turretPos = Vector{t[0], t[1], t[2]};   // operator= (out of line)
    v = MatrixRotateVector(rotation, turretPos);
    ...
}
```

Original disassembly (`b32c6`–`b3320`):

```text
  b32d4  add.w r0,r1,r9        ; t = table + k*24
  b32dc  ldr   r1,[r0,#8]      ; t[2]
  b32de  ldr   r2,[r0,#16]     ; t[4]
  b32e0  str   r1,[sp,#116]    ; save t[2]
  b32e2  str   r2,[sp,#204]    ; turretDir.y
  b32e6  ldr   r1,[r0,#12]     ; t[3]
  b32e8  ldr.w r8,[r0,#4]      ; t[1]
  b32ec  ldr   r0,[r0,#20]     ; t[5]
  b32ee  str   r1,[sp,#200]    ; turretDir.x
  b32f2  str   r0,[sp,#208]    ; turretDir.z
  b32f4  mov   r0,r5           ; &temp  (sp+212)
  b32f6  blx   MatrixRotateVector
  b3300  blx   Vector::operator=          ; v = temp
  b3308  blx   setRotation
  b330c  ldr.w r0,[sl,r9]      ; t[0]
  b3312  str.w r8,[sp,#204]    ; turretPos.y = t[1]
  b3316  str   r0,[sp,#200]    ; turretPos.x = t[0]   <-- SAME slot as turretDir
  b331a  ldr   r0,[sp,#116]    ; t[2]
  b331c  str   r0,[sp,#208]    ; turretPos.z = t[2]
  b3318  add   r2,sp,#200      ; &turretPos = &turretDir
  b3320  blx   MatrixRotateVector
```

So the original has:

```text
  turretDir  sp+200..208   (direct init from t[3],t[4],t[5])
  turretPos  sp+200..208   (direct init from t[0],t[1],t[2]; same slot, disjoint lifetime)
  temp       sp+212        (sret slot of both MatrixRotateVector calls)
  v          sp+372
  base       sp+224
  rotation   sp+312
  t[2] saved sp+116, t[1] kept in r8 across the first call
```

There is **no default-construction / zero-init of `turretPos`** in the original.
The faithful source is therefore direct initialisation declared after
`setRotation`:

```cpp
Vector turretDir = {t[3], t[4], t[5]};
v = MatrixRotateVector(rotation, turretDir);
gun->geometry->setRotation(v);
Vector turretPos = {t[0], t[1], t[2]};
v = MatrixRotateVector(rotation, turretPos);
```

Ours instead keeps the `Vector turretPos;` default construction (`strd r0,r0,
[sp,#212]; str r0,[sp,#220]`) because the later `turretPos = Vector{...}` goes
through the out-of-line `Vector::operator=`, so the zero stores are not dead.
That forces a second 12-byte slot and moves the call temp to `sp+188`.

Measured variants:

| variant | size | fuzzy | guard_sp | note |
|---|---:|---:|---:|---|
| **baseline** (`Vector turretPos;` then `operator=`) | **17172** | **90.7** | **84** | turretDir=sp+200, turretPos=sp+212, temp=sp+188 |
| `turretPos` direct-init after `setRotation` (variant B) | 17144 | 88.4 | 84 | turretDir=sp+212, turretPos=sp+200, temp=sp+200 |
| single `turretPos` reused for both vectors | 17178 (obj) | — | 84 | one slot, but size grows |
| `bool alienWave` | 17156 | 89.2 | 84 | b2e04 materialised, no weight change |

Neither direct-init shape reproduces the original's exact colouring
(`turretDir`/`turretPos` both at sp+200, temp at sp+212); the difference is again
which disjoint intervals StackSlotColoring decides to merge, i.e. a global
colouring outcome.

## 4. The epilogue slot swap is a global colouring outcome, not a weight comparison

This is the key new negative result. The wave-90/91 notes treated the swap as
"guard weight must exceed raid weight". That is necessary but **not sufficient**:
the colour a slot receives is decided by `StackSlotColoring::ColorSlot`
(`llvm/lib/CodeGen/StackSlotColoring.cpp`, LLVM 7):

```cpp
// intervals sorted by weight descending
Color = UsedColors[StackID].find_first();
while (Color != -1) { if (!OverlapWithAssignments(li, Color)) { Share = true; break; }
                      Color = UsedColors[StackID].find_next(Color); }
if (!Share) { Color = NextColors[StackID]; NextColors[StackID] = AllColors[StackID].find_next(Color); }
```

A high-weight interval that can *share* an existing low colour takes it; a
long-lived interval that cannot share takes the next free colour. Consequently
changing an unrelated interval can renumber raid/guard even when their own
weights do not change.

Direct evidence: `if ((security == 3) && raid)` (short-circuit `&&`) flips the
slot while raid's weight is still **1.223364 > guard 1.193760**:

```text
  raid&&sec variant: raid fi#86 weight 1.223364 (19 refs, bb.44 -> bb.45)
                     guard fi#74 weight 1.193760
                     freq(bb.519) 0.568760 (unchanged)
                     guard_sp 88  (FLIPPED), size 17196 B, fuzzy 87.1 %
```

The short-circuit adds a block (bb.44 becomes bb.45 with a different frequency),
which perturbs the interference/colour order of the whole function and ends with
the guard getting the lower colour despite the lower weight. This is why the
wave-90 "margin" arithmetic (0.004 or 0.108) cannot predict the flip.

## 5. Variants measured this pass (all reverted)

Object sizes below are from a direct `Level.cpp` compile; the linked `.so`
function is object size + 2 B of section padding. `guard_sp` is the `[sp,#N]` of
the stack-protector address spill.

| variant | obj size | linked | guard_sp | note |
|---|---:|---:|---:|---|
| **baseline** | **17170** | **17172** | **84** | |
| `int raid` | 17170 | 17172 | 84 | neutral |
| `bool raid = false;` | 17170 | 17172 | 84 | neutral |
| `bool raid = false;` + `if (!introSystem \|\| hardCore)` | 17170 | 17172 | 84 | neutral |
| `raiders` via `if (raid)` instead of `?:` | 17170 | 17172 | 84 | neutral |
| `raid & (security==3)` (commuted) | 17170 | 17172 | 84 | neutral |
| `(security==3) && raid` | 17194 | 17196 | **88** | flips, fuzzy 87.1 % |
| `campaign>=32 && raid && ...` | 17178 | 17180 | **88** | flips |
| decl order `pirateFortress, alienWave, bigAlienWave` | 17178 | 17180 | 84 | size changes |
| decl order `alienWave, pirateFortress, bigAlienWave` | 17162 | 17164 | 84 | size changes |
| `base` before `v` in alienGuns | 17170 | 17172 | 84 | neutral |
| `int k = 0` before `v` | 17170 | 17172 | 84 | neutral |
| `bool alienWave` | 17154 | 17156 | 84 | fuzzy 89.2 % |
| `turretPos` direct-init after `setRotation` | 17142 | 17144 | 84 | fuzzy 88.4 % |
| `bool alienWave` + direct-init | 17132 | 17134 | 84 | |

No size-preserving variant flips the guard slot.

## 6. What remains

1. **raid/guard swap** (~19 instructions, ~1.1 pp): a global colouring outcome.
   It can only be fixed by matching the whole function's IR (the alienGuns
   `Vector` lifetime, the b2e04 condition, the convoy switch, the EH pads), so
   that the interference graph and the colour order match. No local reordering
   works while keeping size 17172.
2. **alienGuns `Vector` lifetime** (~50 instructions): the faithful source is
   direct initialisation of `turretPos` after `setRotation` (sharing
   `turretDir`'s slot), but it currently shrinks the function by 28 B. A shape
   that keeps the original's `t[2]`-in-sp+116 / `t[1]`-in-r8 staging and the
   index-based `table + k*24` addressing is still missing.
3. **b2e04** (`bool alienWave`): correct shape, 16 B too small.
4. **convoy switch `b1408`**: unchanged from wave 90 — needs unequal `!prof`
   weights below the 66 % peel threshold.
5. **EH landing pads** (`b41f0`–`b42d0`): block order, not a different try/catch
   structure (`.ARM.extab` is identical, 148 call sites).

## Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# exact Level.cpp compile (NDK r18b clang 7.0.2, -Oz), see /dev/shm/w4235/cc.sh
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  ... -ffunction-sections -funwind-tables -c src/game/world/Level.cpp
# spill weights
clang++ ... -mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv
# pre-colouring frame indices / CFG edges
clang++ ... -mllvm -print-machineinstrs
# side-by-side disasm
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump <ndk-objdump> \
  --show _ZN5Level13createMissionEv
```
