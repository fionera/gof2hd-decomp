# Layout::Layout (`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`) — the `0x120`/`0x130`
# swap was a measurement regression; reverting it restores 47.664 % (w4501, wave 95)

Status: one-line source revert landed. Worktree `wt/w4501`, branch
`wave95-w4501`, base `4a88bd6e`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, size **7072 B**,
image base `0x10000`).

Continues `layout-layout-c1-c2-vstr-shapes-w4401.md` (wave 94),
`layout-layout-c1-c2-vstr-opacity-w4274.md`,
`layout-layout-c1-c2-midblock-vstr-w4267.md`.

## Result

| metric | before (HEAD `4a88bd6e`) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` raw ratio (unrounded) | 47.3161033797 | **47.6644802226** |
| displayed pct (verify rounds to 1 dp) | 47.3 | **47.7** |
| `usize` / `osize` | 7072 / 7072 | **7072 / 7072** |
| `byte_exact` | 4500 / 4524 | **4500 / 4524** |
| `linked_exact` | 4500 / 4524 | **4500 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

The landed edit is the inverse of commit `a0f2dcc2` ("move `0x120` before
`0x130`"): put `field_0x130` back before `field_0x120`. The `field_0x134`
if/else from `30752cad` is kept.

```cpp
    this->field_0x130 = Globals::iPadHD ? 78 : (Globals::n9 ? 60 : (Globals::iPadLarge ? 112 : (Globals::retinaDisplay ? 80 : (Globals::iPad ? 56 : 40))));
    this->field_0x120 = Globals::iPadHD ? 21 : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 28 : 15));
```

## 1. Why: the two "improvement" commits were measured one edit behind

The commit messages of `30752cad` and `a0f2dcc2` claim

```
30752cad  raw 47.6550 -> 47.6645   (field_0x134 if/else)
a0f2dcc2  raw 47.655  -> 47.664    (0x120 before 0x130)
```

Measured with the official verifier (`tools/verify/verify.py`'s own
`disassemble` + `reconcile` + `SequenceMatcher(autojunk=False)`, unrounded),
the three tree states give:

| tree state | `usize` | raw ratio |
|---|---:|---:|
| `f262bd4b` (before both commits) | 7072 | 47.6550079491 |
| `f262bd4b` + `field_0x134` if/else only | 7072 | **47.6644802226** |
| `f262bd4b` + `field_0x134` if/else + `0x120`/`0x130` swap (= HEAD) | 7072 | **47.3161033797** |

So the `field_0x134` if/else is a real +0.00947 pt improvement, and the
`0x120`/`0x130` swap is a **−0.34838 pt regression**. The reported
`47.664` for `a0f2dcc2` is exactly the *previous* state's number: each
measurement was taken against a stale `libgof2hdaa.so` (the edit had not
been relinked), so the swap was credited with the `field_0x134` gain.

The committed `report.json` at `69d0aa97` already contradicted its own commit
message: it records `Layout::Layout pct 47.3`, not `47.6645`.

This is a strict target-function improvement (raw ratio) with the function
size unchanged at 7072 B, so downstream `byte_exact` is untouched.

## 2. Re-measured order search (with a forced relink per variant)

`fscore.py` (below) always runs `cmake --build ... --target link_so` before
scoring, so it cannot reproduce the stale-build error. On the restored
47.6645 body:

* **Exhaustive adjacent-swap hill climb** over all 203 top-level constructor
  units (if/else chains parsed as single units): no swap with `usize == 7072`
  beats 47.6644802226. The best 7072 B neighbours are the swap back to
  `0x120`/`0x130` (47.655008) and several no-ops (identical codegen).
* **400 random single-unit moves** (move unit *i* to position *j*), accepting
  only `usize == 7072`: no improvement. The body is a strong local optimum.
* **400 random moves from the `0x98`/`0xac`-swapped body** (see §3) and
  **400 from the `0xb0`/`0xa0`-swapped body**: no `usize == 7072` result
  above 47.6645 (best 7072 hits were ≈42.5 %).

## 3. The two non-landable order leads (size-short)

Re-measured on the 47.6645 body:

| variant | `usize` | raw ratio |
|---|---:|---:|
| `0x98` before `0xac` (swap units 44/45) | 6984 | 47.9553161779 |
| `0x98` before `0xac` + `0x150` `(int)(float)` | **6992** | **48.0654168329** |
| `0x98` before `0xac` + `0x3f8` `(int)(float)` | 6992 | 47.9856402074 |
| `0xb0` after `0xa0` (swap units 47/48) | 7024 | 47.6910828025 |
| `0x98` before `0x9c` only | 6968 | 47.9233226837 |
| `0x94` moved after `0x90` | 6992 | 45.4690618762 |

All are **short of 7072 B**, so they cannot be landed without destroying the
downstream `byte_exact` count (a size change shifts every later function).
The `0x98`/`0xac` swap is the strongest lead: it raises the ratio by
+0.29 pt and is still 88 B short. Adding the only reproducible missing
`vstr` (`0x150`) recovers 8 B (6992 B / 48.065 %); the remaining 80 B are
the four opaque round-trips of §4.

## 4. The four opaque `(int)(float)` fields: all source forms still fold

Targets `0x9c`, `0x158`, `0x15c`, `0x170` (the `(int)(float)`/`(int)(float)int`
stores the original keeps and our flat ternaries fold). Measured on the
restored 47.6645 body and on the `0x98`/`0xac` body (forced relink each time):

| source form | `usize` / raw on 47.6645 body | `usize` / raw on 6984 body |
|---|---:|---:|
| `float f; if/else chain; f = retina ? hi : lo; field = (int)f;` | 7072 / 47.664 | 6984 / 47.955 |
| `int v; if/else chain; field = (int)(float)v;` | 7072 / 47.664 | 6984 / 47.955 |
| plain `if/else` assigning the field directly | 7072 / 47.664 | 6984 / 47.955 |
| combined `0x154`+`0x158` if/else chain (both fields per branch) | 7072 / **47.466** | 6976 / 47.914 |
| combined `0x154`+`0x158` with a shared `float f158` | 7072 / **47.466** | 6976 / 47.914 |
| `field = ...; field = (int)(float)field;` (member round-trip) | 7072 / 47.664 (folded away) | 6984 / 47.955 |
| `0x150` all-float ternary | 7080 / 47.218 | 6992 / 47.477 |
| `0x150` `(int)(float)(int ternary)` | 7088 / 47.657 | **6992 / 48.065** |
| `0x1e4` mixed `4 * 1.40625f : 6 : 8 : 4` | 7024 / 46.188 | 6976 / 46.086 |
| `0x9c` if/else -> flat ternary | 7072 / **47.536** | 6984 / 47.955 |

The `(int)(float)` round-trips of `0x158`/`0x15c`/`0x170` are still folded
by InstCombine in every form tried; the combined `0x154`+`0x158` chain even
*loses* 8 B (6976). This confirms the w4401 conclusion: the conversion is a
property of the original's whole-function branch/select split, not of the
expression text. The one reproducible form remains `0x150` (all-float or
`(int)(float)` of the int ternary), which emits the original's exact
`vmov.f32`/`vcvt.s32.f32`/`vstr` sequence.

## 5. Original def-region store order (new, for the next pass)

The literal-pool store-order extractor was corrected to strip objdump's
`; 0xNN` comments (the earlier `stores.py` runs silently dropped every
commented `add.w rX, r0, #imm` and therefore all `vst1` groups). The
original's default-path store order around the `0x8c`..`0xcc` block is:

```
8c 90 94 98 9c ac a8 a0 | d0 d8 e0 e8 fc 104 2b4 |
b0(group) c0(group) ec(group) 294(group) 2a4(group) |
118(group) 108(group) 128(group) 140(group) 138 ...
```

Our 47.6645 body emits:

```
8c 90 9c ac 98 98 ac a8 a0 | ec f0 f4 f8 2b4 |
b0(group) c0(group) 294(group) 2a4(group) dc(group) |
118(group) 108(group) 128(group) 23c 138 14c ...
```

i.e. `0x98` is after `0xac` (original: before `0x9c`), `0x94` is far away
(unit 178), and `0xec..0xf8` are four scalars in ours but one `vst1` group
in the original. That is the structural reason the `0x98`/`0xac` swap
recovers ratio but also changes the SLP grouping and loses 88 B.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target check -j8     # RATCHET PASS

# unrounded single-function score (forces a relink; cannot go stale)
python3 /dev/shm/w4501/fscore.py build

# original/ours store-group order (offset + instruction, address order)
python3 /dev/shm/w4501/stores.py    > /dev/shm/w4501/stores.txt   # original
python3 /dev/shm/w4501/ourdump.py   > /dev/shm/w4501/ours.txt     # ours

# adjacent-swap / random-move order searches
python3 /dev/shm/w4501/hill2.py
python3 /dev/shm/w4501/rmove.py 1 400
```

Scratch (`/dev/shm/w4501`): `fscore.py` (relink + unrounded ratio),
`region.py` (per-region match: hd/n9/def/tail), `uvary.py` (unit permutation
harness), `hill2.py`, `rmove*.py`, `try1..3.py`, `stores.py`, `ourdump.py`,
`Layout.base.cpp` (the 47.6645 body), `Layout.swap.cpp` (the 6984 B lead).

## 7. Remaining work

1. Recover the original's interleaved `0x150`..`0x168` branch chain so the
   four opaque `(int)(float)` round-trips survive. On the `0x98`/`0xac`
   body the missing stores are `0x9c` (`(int)float`, +8), `0x150`
   (`(int)float`, +8, proven), `0x158`/`0x15c`/`0x170`
   (`(int)(float)int`, +16 each) and `0x3f8` (`(int)(float)int`, +16);
   `0x1e4` is the seventh original-only `vstr`. Recovering them is exactly
   the 88 B the swap loses (6984 → 7072) and would make the 48.06 % variant
   landable.
2. Recover the `0xec..0xf8` `vst1` group (ours: four scalars) and the
   `0x140` group (ours: missing) — together they are the 88 B the
   `0x98`/`0xac` swap loses.
3. The `0x94` unit is at source position 178 but the original stores it
   between `0x90` and `0x98`; moving it there alone costs 80 B
   (6992 B / 45.47 %), so it must be moved together with the group fix.
