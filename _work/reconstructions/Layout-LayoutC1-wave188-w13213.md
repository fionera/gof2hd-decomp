# `Layout::Layout` C1/C2 — wave 188 / worker w13213

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave188-w13213`, base master `ce16f45f1` (wave-187 harvest, raw **5,353**).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes (differing) | **5,353** |
| aligned (difflib) | 2,681 |
| size | **7,072/7,072** |
| instructions | 2,521/2,552 |

Gate after the (empty) pass: `RATCHET PASS` — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370; whole-`.text` differing bytes **74,223**
(unchanged).

## 1. Corrected ARM/VFP emulator oracle (new capability)

The wave-187 handoff's emulator (`scratch/emulate_orig3.py`) reported 18 "real"
field diffs, but most were emulator bugs.  A corrected emulator
(`scratch/emulate_orig4.py`) runs the original ctor under Unicorn and now agrees
with the Ghidra decompile on every field except four.  Bugs fixed:

* `vldr sN,[rM]` / `[rM,#imm]` register-indirect loads (only `[pc,#imm]` was
  modelled before; the four `[rM]` loads silently left stale VFP registers, so
  `0x8c`, `0x9c`, `0xdc..0xe8`, `0x2f8`, `0x3f8` all read garbage).
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

## 3. Every faithful semantic fix is a raw *loss* on this body

`raw` is the count of **differing** bytes (lower is better; the wave-187
`5,354 -> 5,353` win was a decrease).  Each of the four true fixes, applied
alone to the base, raises raw and/or changes size:

| fix | raw | size |
|---|---:|---:|
| `0x190` iPad/retina order swap | 5,431 (+78) | 7,072 |
| `0x108` default `0 -> -2` | 5,467 (+114) | 7,064 (-8) |
| `0x114` default `1 -> -2` | 5,579 (+226) | 7,080 (+8) |
| `0x1f4` -> `HD?22 : n9?1 : iPadLarge?-8 : 0` | 5,458 (+105) | 7,092 (+20) |

So the base is a byte-matching local optimum that deliberately keeps four
wrong field values.  Pairs were also tried (`0x108+0x114` = 5,599/7,064) but
none is size-exact with raw < 5,353.

## 4. Searches re-run on the base (all confirm the local optimum)

* **Adjacent-unit swap hill climb** (204 units, every `i,i+1` swap, size-exact
  only): best raw **5,353** (neutral); no swap below 5,353.
* **`(int)(N * 1.40625f)` int-typing** of the `iPadHD` arm of every simple
  assignment (95 variants): 66 size-exact, all raw 5,353.
* **`(int)(float)(...)` round-trip** of every simple assignment RHS
  (240 variants): 213 size-exact, all raw 5,353.
* **`(float)(...)` wrap** of every RHS (246 variants): no size-exact variant
  below 5,353.
* **`strd`-pair adjacency** for every original `strd` pair (56 variants): all
  size-exact variants are >= 5,353.
* **Store-order reorder** of the 181 movable assignments: raw 6,307 at size
  6,828 (size-fatal, and raw much worse).

## 5. Store-type map (why the body cannot easily improve)

Restricted to the ctor range, the original's last store is `vstr` for
`0x9c, 0xa8, 0x150, 0x158, 0x160, 0x168, 0x170, 0x1a8, 0x1e4, 0x3f8`, while
HEAD uses `str.w`/`strd`; conversely HEAD uses `vstr` where the original uses
`str` at `0x20, 0x1f4, 0x204`.  The original also emits 62 `strd` / 86 `vld1` /
81 `vst1` vs HEAD's 41 / 76 / 71 — i.e. the original is more SLP/vector-store
heavy.  Forcing the missing `strd` pairs or the `(int)(float)` round-trips
(`rewardBoxY2`, `0x160`) either leaves raw unchanged or raises it and changes
size, so none can land.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13213
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5353 aligned=2681 size=7072/7072 insn=2521/2552
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13213/`
(`emulate_orig4.py`, `dump_orig4.py`, `compare_values4.py`, `swapmin.py`,
`alts.py`, `measure.py`, `units.py`, `storeorder.py`).
