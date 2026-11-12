# `Layout::Layout` C1/C2 — wave 189 / worker w13314

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave189-w13314`, base master `51d865ebe` (wave-188 harvest, raw **5,257**).

**Landed: one semantic fix, size-exact, raw-neutral, aligned/insn -2.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,257** | **5,257** (neutral) |
| aligned (difflib) | 2,728 | **2,726** (-2) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,522/2,552 | **2,520/2,552** (-2) |
| whole-`.text` diff | 74,039 | 74,039 |
| `.ARM.extab` diff | 639 | 639 |
| build-id diff | 20 | 20 |

Gate after the change: `RATCHET PASS` — byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0,
extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  `.rel.plt` unchanged (secdiff reports no other section delta).

## 1. The wave-188 "four semantic diffs" are mostly oracle false positives

The wave-188 w13213 note (`Layout-LayoutC1-wave188-w13213.md`) reports four
value diffs from a corrected Unicorn oracle.  Re-checking each against the
checked-in Ghidra decompile
(`_work/reconstructions/layout-orig-ghidra-decompile.txt`) and the original
literal pools shows **only `0x190` is a real source-level bug**.

### `0x108` / `0x114` — base is already correct

The original stores `0x108..0x114` as one 16-byte SLP group.  The literal
pools (ELF VMAs; Ghidra address = ELF + `0x10000`) are:

| branch | literal | `0x108,0x10c,0x110,0x114` |
|---|---|---|
| HD | `0xd1488` | `-2, 1, 2, 1` |
| n9 (`cVar43 != 0`) | `0xd28f0` | `-1, 2, 1, -1` |
| !n9, retina | `0xd2960` | `-2, 2, 1, -1` |
| !n9, !retina | `0xd2970` | `0, 1, 2, 1` |

The !n9 group is `VectorBitwiseSelect(mask(retina), {0,1,2,1},
{-2,2,1,-1})` (decompile lines 566-599 / store at 651-652).  HEAD's
`0x108 = HD ? -2 : (n9 ? -1 : (retina ? -2 : 0))` and
`0x114 = HD ? 1.40625f : (n9 ? -1 : (retina ? -1 : 1))` reproduce exactly
those four vectors.  The oracle's `0x108 = n9 ? -1 : -2` (all non-n9 = -2)
and `0x114 = n9 ? -1 : -2` are inconsistent with the two-way select: neither
`0x114` vector contains `-2`.

### `0x1f4` — base is already correct

Original `strd r2, fp, [r0, #496]` at `0xd2610` stores `0x1f0` and `0x1f4`.
`fp` is `22` in the HD arm (`0xd255a`).  In the non-HD arm
(`0xd2566..0xd2592`) the code computes

```
r3 = retina ? (iPad ? 32 : 4) : (iPad ? 16 : 2)
vcvt.f32.s32 / vcvt.s32.f32   ; (int)(float) round trip
fp = r3
```

which is exactly HEAD's
`HD ? 16*1.40625f : (retina ? (iPad ? 32 : 4) : (iPad ? 16 : 2))`
(the `16*1.40625f` HD arm truncates to 22).  Ghidra line 1575
(`*(int *)(this+500) = iVar64`) confirms it.  The oracle's
`HD ? 22 : n9 ? 1 : iPadLarge ? -8 : 0` is not this field.

### `0x190` — real bug: iPad must win over retina

The original decompile (lines ~962-990) is

```
bVar42 = (iPad == 0);
if (bVar42) { uVar50 = retina ? -2 : -1; store 0x190 = uVar50; }
else        { store 0x190 = -4; }
```

with `iPadLarge -> -8` and `HD -> -6` in their own arms.  So the value is
`HD ? -6 : iPadLarge ? -8 : n9 ? -4 : iPad ? -4 : retina ? -2 : -1`:
**iPad has priority over retina** when both are set.  HEAD tested retina
first, giving `-2` for an iPad-with-retina device.

## 2. The landed shape

The faithful source shape is not a flat `else if (retina) -2; else if (iPad)
-4;` chain (that is wave-188's 190a, raw 5,335).  It is the nested form that
mirrors the original's `!iPad ? (retina ? -2 : -1) : -4`:

```cpp
    } else if (Globals::retinaDisplay) {
        this->field_0x190 = Globals::iPad ? -4 : -2;
    } else {
        this->field_0x190 = Globals::iPad ? -4 : -1;
    }
```

This is value-identical to 190a but compiles to the same 7,072 B body as the
base with raw 5,257, aligned 2,726 (base 2,728) and 2,520 instructions
(base 2,522).  Seven other shapes were measured:

| shape | raw | size |
|---|---:|---:|
| flat chain, iPad before retina (190a) | 5,335 | 7,072 |
| merge last two arms into one ternary | 5,382 | 7,072 |
| merge n9/else into one ternary | 5,401 | 7,072 |
| `retina && !iPad` guard | 5,393 | 7,096 |
| nested if inside retina arm | 5,345 | 7,088 |
| **landed nested ternary (f)** | **5,257** | **7,072** |
| base (wrong order) | 5,257 | 7,072 |

## 3. Searches on the wave-188 "all-four corrected" body (no win)

Applying all four wave-188 fixes gives raw **6,301**, size **7,040** (-32).
Exhaustive 2-opt (20,103 valid swaps) best: proxy 5,844 / size 7,072; K=4
best 6,053; moves 1..20 best 6,052.  None is close to 5,257.

Applying only the real `0x190` fix in the flat-chain shape (190a) gives
raw 5,335 / size 7,072.  Exhaustive 2-opt best 5,305 (proxy 5,239), K=4 and
moves best 5,235 proxy (5,301 linked) — still 44 B worse than base.

The landed nested shape (f) is a local optimum: exhaustive 2-opt, K=4, and
moves 1..20 all return the identity at proxy 5,191 (= linked 5,257).  A
2,500-candidate random 2..4-swap sample was uniformly worse (best 5,485).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13314
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5257 aligned=2726 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13314/`
(`units.py`, `batch_score.py`, `sweep.py`, `sweep2.py`, `shapes.py`,
`shapes2.py`, `shapes190.py`, `shapes190b.py`, `secdiff.py`, `prefix.h`,
`ctor_body*.txt`, `*opt*.json`).
