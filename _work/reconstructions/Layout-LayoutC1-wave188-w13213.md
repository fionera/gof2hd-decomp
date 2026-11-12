# `Layout::Layout` C1/C2 — wave 188 / worker w13213

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave188-w13213`, base master `ce16f45f1` (wave-187 harvest, raw **5,353**).

**Result: seven size-exact raw wins landed — 5,353 -> 6,116 (+763), size 7,072/7,072.**

| metric | base | p1..p6 | p7 |
|---|---:|---:|---:|
| raw same-address bytes | 5,353 | 5,980 | **6,116** |
| aligned (difflib) | 2,681 | 3,349 | 3,860 |
| size | 7,072/7,072 | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,521/2,552 | 2,523/2,552 | 2,513/2,552 |

Per-pass raw: base 5,353 -> p1 5,431 (0x190) -> p2 5,944 (0x114 + 0x74
int-typing) -> p3 5,949 (footerButtonOffset round-trip) -> p4 5,961 (0x108 +
0x1f4) -> p5 5,968 (0x210/0x214 strd) -> p6 5,980 (0x2fc/0x300 strd).

## 1. New capability: a corrected ARM/VFP emulator oracle

The wave-187 handoff's emulator (`scratch/emulate_orig3.py`) reported 18 "real"
field diffs, but most were emulator bugs.  A corrected emulator
(`scratch/emulate_orig4.py`) runs the original ctor under Unicorn and now agrees
with the Ghidra decompile on every field except four.  Fixes applied:

* `vldr sN,[rM]` / `[rM,#imm]` register-indirect loads (only `[pc,#imm]` was
  modelled before; the four `[rM]` loads silently left stale VFP registers).
* `vmov.f32 sN,#imm` uses the disassembler's `; 0x........` decoded constant,
  not the ARM modified-immediate encoding (`#36` means `0x41200000` = 10.0,
  not 36.0).
* `vmoveq/ne.f32` were being swallowed by the plain-`vmov` block and executed
  unconditionally; they now honour CPSR Z.
* `vcvt.s32.f32` / `vcvt.f32.s32` now store raw int bits / float bits rather
  than reinterpreting through `float`.
* `vshl.s32 q,q,q` register-form shift (sign-extended low byte per lane) and
  `vst1.32 {dN[k]},[...]` single-lane stores.

GOT mapping (verified against `.rel.dyn`): `0x21032c` iPad, `0x21038c` iPadHD,
`0x2103e4` retinaDisplay, `0x2103e8` n9, `0x2103ec` iPadLargePossible,
`0x2103f0` iPadLarge.

## 2. True semantic diffs of HEAD vs the original (only four)

`compare_values4.py` compares all 32 flag combinations against a host build of
HEAD.  The corrected oracle leaves exactly four real value differences:

| offset | original value | HEAD |
|---|---|---|
| `0x108` | `n9 ? -1 : -2` (HD also -2) | default `0` |
| `0x114` | `HD ? 1 : n9 ? -1 : -2` | default `1` |
| `0x190` | `HD ? -6 : iPadLarge ? -8 : n9 ? -4 : iPad ? -4 : retina ? -2 : -1` | retina tested before iPad |
| `0x1f4` | `HD ? 22 : n9 ? 1 : iPadLarge ? -8 : 0` | retina/iPad tree, no iPadLarge |

Everything else (including the whole `0xdc/0xe0/0xe4/e8` SLP block, `0x8c`,
`0x9c`, `0x2f8`, `0x3f8`, `0x3e0`) already matches.  In particular the
wave-187 claim that `0x8c`/`0x9c`/`0xdc..0xe8` are wrong was an artifact of
the broken emulator; the Ghidra decompile confirms HEAD.

## 3. The landed win: `field_0x190` branch order

Original disassembly computes `0x190` as `HD -> -6`, `iPadLarge -> -8`,
`n9 -> -4`, then `iPad -> -4`, then `retina -> -2`, else `-1`.  HEAD had the
`retinaDisplay` and `iPad` branches swapped, so `(retina=1, iPad=1)` stored
`-2` instead of `-4`.  Swapping the two `else if` arms:

```c
} else if (Globals::iPad) {
    this->field_0x190 = -4;
} else if (Globals::retinaDisplay) {
    this->field_0x190 = -2;
} else {
```

Measured with `tools/fn_diff.py`:

| variant | raw | size |
|---|---:|---:|
| base | 5,353 | 7,072 |
| **swapped iPad/retina arms (landed)** | **5,431** | **7,072** |

## 4. Pass 2: `0x114` default plus an int-typing compensation

`0x114` is `HD ? 1 : n9 ? -1 : -2` (HEAD had `retina ? -1 : 1`).  Fixing it
alone grows the function by 8 B:

| variant (on top of pass 1) | raw | size |
|---|---:|---:|
| `0x114` default `1 -> -2` | 5,582 | 7,080 (+8) |

Searching for a compensating -8 B shape change, wrapping the `iPadHD` arm of
`field_0x74` in `(int)(...)` shrinks the function by exactly 8 B *and* is
value-identical (the field is `int`, so `(int)(115 * 1.40625f)` == the implicit
conversion of the float ternary).  The combined pass-2 change:

```c
this->field_0x114_strip5c = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? -1 : -2);
this->field_0x74 = Globals::iPadHD ? (int)(115 * 1.40625f) : (Globals::n9 ? 207 : (Globals::retinaDisplay ? 230 : 115));
```

| variant | raw | size |
|---|---:|---:|
| pass 1 | 5,431 | 7,072 |
| **pass 2 (landed)** | **5,944** | **7,072** |

The `(int)` wrap alone (with `0x114` fixed) measured 5,944/7,072; the same wrap
without `0x114` was also size-exact but lower raw.

## 5. Pass 3: `footerButtonOffset` round-trip

A fresh `(int)(float)(...)` wrap sweep over the whole ctor on top of pass 2
(240 variants) found one further size-exact improvement: wrapping the
`footerButtonOffset` ternary in `(int)(float)(...)` (a value-identical no-op,
since the field is `int` and the values are 3..6) moved raw 5,944 -> **5,949**
at size 7,072.  All other 211 size-exact wraps were neutral (5,944).

## 6. Pass 4: the two remaining semantic fixes, paired

`0x108` and `0x1f4` are the last two real value diffs.  Individually they are
size-fatal, but together they are size-exact on top of pass 3:

| variant (on top of pass 3) | raw | size |
|---|---:|---:|
| `0x108` default `0 -> -2` | 5,990 | 7,056 (-16) |
| `0x1f4` -> `HD?22 : n9?1 : iPadLarge?-8 : 0` | 5,921 | 7,076 (+4) |
| **both (landed)** | **5,961** | **7,072** |

With pass 4 the ctor is now **value-identical to the original on all 32 flag
combinations** (the corrected oracle reports zero real field diffs).

## 7. Pass 5: `strd` adjacency for `0x210`/`0x214`

A `strd`-pair adjacency sweep (move the assignment of one member of an original
`strd` pair next to the other) on top of pass 4 found one size-exact win: moving
`field_0x214` immediately after `field_0x210` (either order) gives raw
5,961 -> **5,968** at size 7,072.  The pair is stored as `strd` in the original
but as two scalar `str.w` in HEAD before the move.

## 8. Pass 6: `strd` adjacency for `0x2fc`/`0x300`

Re-running the `strd`-pair sweep on pass 5 found three more size-exact moves;
the best was pairing `choiceWindowTitleHeight` (0x300) with `field_0x2fc`
(0x2fc), which enables the original's `strd` store: raw 5,968 -> **5,980** at
size 7,072.  (`0x2f4`/`0x2f8` gave 5,973, `0x30c`/`0x310` gave 5,969.)

## 9. Pass 7: partial store-order reorder + `0x1dc` int-typing

Sorting the 181 movable simple assignments by the original's store sequence
(`storeorder.store_sequence('orig.dis')`) and applying only the **first 16**
entries of that permutation (the rest left in base order) gives raw 6,004 at
size 7,056 (-16).  Wrapping `field_0x1dc`'s `10 * 1.40625f` arm in `(int)`
(value-neutral: the field is `int`) restores +16 B and lands the pair:

| variant (on top of pass 6) | raw | size |
|---|---:|---:|
| store-order prefix k=16 | 6,004 | 7,056 (-16) |
| **k=16 + `(int)(10 * 1.40625f)` on 0x1dc (landed)** | **6,116** | **7,072** |

The full store-order permutation (k=181) reaches raw 6,307 but shrinks the
function to 6,828 B, so it cannot land without a large compensating growth.

## 10. Other fixes measured but not landed (size-fatal)

| fix | raw | size |
|---|---:|---:|
| `rewardBoxY2` `(int)(float)` round-trip (on pass 3) | 5,922 | 7,072 (neutral raw) |

The `rewardBoxY2` round-trip is semantically right (the original does store
`(int)(float)((rewardBoxY + 17) << retina)`) but does not improve raw on this
body.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13213
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=6116 aligned=3860 size=7072/7072 insn=2513/2552
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13213/`
(`emulate_orig4.py`, `dump_orig4.py`, `compare_values4.py`, `tryfix.py`,
`tryfix2.py`, `sweep_intcast2.py`, `partial2.py`, `storeorder.py`,
`measure.py`).
