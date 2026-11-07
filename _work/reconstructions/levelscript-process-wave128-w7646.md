# LevelScript::process — wave 128 / w7646 (high-case body order hypothesis + case-135 cargo-delete order)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `fee528cbe`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.076102365      60.102467171     (+0.026364806)
official verify pct      60.1              60.1             (same, rounded)
orig / ours insns        20169 / 20040     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54542             54577            (+35)
whole .text diffbytes    85475             85510            (+35)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.ARM.exidx/.rel.plt/.rodata/.data/.bss   0 / 0 / 0 / 0 / 0
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams.

## Hypothesis tested: "high case bodies laid out in source order"

The wave-127 open question was whether the original lays the high switch-case bodies out in
*source* order while clang uses ascending case-value order. **It does not — the body order is
already identical in both binaries.**

Decoded from the two range `tbh` tables (`0x138a8a` cases 131..145, `0x138c3e` cases 64..73) the
original body order by address is:

```
main compare chain:  158, 29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16
tbh 131..145:        131, 135, 145, 142, 144, 139
tbh 64..73:          64, 69, 73, 67, 70
```

Our build produces **exactly the same order** for all three groups (verified by decoding our own
`tbh` tables at `0x138a5e` / `0x138c16` and the compare chain at `0x1387da`).  Neither order is
ascending case value, and neither is our source order: clang's `MachineBlockPlacement` picks it
from the CFG, and both compilations pick the same permutation. So source-level case reordering is
not a lever here.

A small clang-7.0.2 experiment confirms the mechanism: for a dense `tbb` switch the body order is
identical for ascending and shuffled source order; for a sparse compare-chain switch the body
order *does* follow source order (8-case probe: shuffled `40,10,70,20,50,30,80,60` produced body
order `40,10,20,30,80,50,60,70`, ascending source produced `10,80,20,30,40,50,60,70`). The
LevelScript dispatch is a mixed tbh+compare-chain switch whose emitted order already matches.

`__builtin_expect` probes on the active-tail guard and the switch guard were also tried and both
grew the function (57336 / 57332 B) with a large fuzzy drop (49.8 / 44.8), so they were reverted.

## Landed change 1 — case 135 revive loop: `delete cargo; cargo = nullptr;`

Real fidelity/behaviour bug in the **revive loop** (case 135, states >= 1 — the "once the timer
elapses, revive every dead group-8 escort" loop at source line ~4157).  Our source had the two
statements in the wrong order:

```diff
-                        (*enemies)[i]->cargo = nullptr;
-                        delete (*enemies)[i]->cargo;
+                        delete (*enemies)[i]->cargo;
+                        (*enemies)[i]->cargo = nullptr;
```

Because `cargo` was nulled first, the `delete` was always `delete nullptr` (dead).  The original
`0x1392b4..0x1392d8` is:

```
1392b4  ldr.w r0,[r8,#4]
1392b8  ldr.w r1,[r0,sl,lsl #2]     ; enemy
1392bc  ldr.w r0,[r1,#80]!          ; r0 = cargo, r1 = &cargo
1392c0  cbz   r0, 0x1392d6
1392c2  blx   0x60204               ; operator delete
1392c6  blx   0x5eb48               ; ~Array<...>
1392ca  ldr.w r0,[r8,#4]
1392ce  ldr.w r0,[r0,sl,lsl #2]
1392d2  add.w r1,r0,#80
1392d6  movs  r0,#0
1392d8  str   r0,[r1]               ; cargo = nullptr
```

(The *other* case-135 loop — the state-0 "name the three docking ports" loop — already had the
correct order in our source.)  After the fix our block is instruction-for-instruction identical
(modulo the cached-enemy register): `ldr [r1,#80]!; cbz; blx delete; blx ~Array; reload;
add #80; movs #0; str`.  The fix alone is **+8 B** (57240).

## Landed change 2 — case 125 state 1: `setName` before `setActive`

Size-neutralising companion (already known faithful from wave 127).  Original
`0x13b486..0x13b516`:

```
13b48e  ldr.w r8,[r0,fp,lsl #2]     ; port = enemies[i]
...
13b4d2  blx   0x63fa8               ; port->setName(getText(3212) + " " + i)
...                                  ; ~String x4
13b4f6  blx   0x632f4               ; enemies[i]->setActive(true)
13b50e  strb.w r1,[r0,#116]         ; enemies[i]->field_0x74 = 0
```

i.e. `setName`, `~String`, `setActive`, `field_0x74 = 0` — our source had `setActive` first.
This is **-8 B** on its own and exactly compensates the case-135 fix, leaving the function at
57232 B.

## Experiments measured this pass (not landed; all reverted)

Baseline: raw 60.076102365, process diffbytes 54542, size 57232.

| variant | raw % | size | process diffbytes |
|---|---:|---:|---:|
| **landed (case-135 delete order + case-125 setName order)** | **60.102467** | **57232** | **54577** |
| case-135 delete order only | 59.358369 | 57240 | — |
| case-125 setName order only | 59.342900 | 57224 | — |
| case-135 delete + `(*enemies)[i]->setVisible` (drop getEnemies re-fetch) | 60.055713 | 57232 | 54562 |
| case-135 delete + case-157 state-9 `m_pGeometry2->setVisible(false)` | 59.374767 | 57236 | — |
| landed + remove case-40 bogus `m_nScriptTime = 0` | 60.100972 | 57232 | 54586 |
| `__builtin_expect(m_bStartSequence != 0, 1)` | 49.844407 | 57336 | 54842 |
| `__builtin_expect(<switch guard>, 1)` | 44.824496 | 57332 | 55399 |

The landed pair is the only size-exact variant that improved the fuzzy score.  It costs +35
`.text` bytes because the case-125 reorder shifts the register/scheduling of the surrounding
block; the case-135 fix is a genuine semantic correction regardless.

## Remaining root cause

Unchanged: global `MachineBlockPlacement` permutation.  The case-body *order* matches; the
difference is the intra-body block order and the register map of each case (e.g. case 135 uses
`r4` for `this` in the original and `r5` in ours).  A concrete next lead: the original keeps the
case-125 state-0 body hot at `0x139f9c` and moves the state-1 body cold to `0x13b43e`; ours keeps
both inline.  Making the first two state tests an `else if` chain flips the polarity
(`cmp r1,#0; beq state0`) and grows the function to 57248, so the original's exact CFG shape is
still not reproduced.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7646/score.py                         # 60.102467171
```

Scratch helpers (`score.py`, `vd.py`, `blocks.py`, `bodies.py`, `topblocks.py`, `prologue.py`,
`objdump_fn.py`, `try.sh`) live in `/dev/shm/w7646/` and are not part of the tree.
