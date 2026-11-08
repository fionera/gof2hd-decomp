# `Layout::Layout` C1/C2 — wave 145 / worker w9008
## Reproduced the wave-144 `retina`-local idiom exactly (6952 B / 5662), then found a 7068 B / 5675 variant. The last 4 B (one literal-pool word) could not be recovered; baseline 7072 / 5704 kept.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-144 master `eb13ada9b`
(target-function raw same-address diff **5,704**, `.text` differing bytes 85,994).

## 1. The wave-144 variant, reproduced exactly

Wave 144 reported `unsigned char retina` for "the first four statements" = **6952 B, diff 5662,
0x vceq, 5x clz**.  That wording was ambiguous; the exact source is:

* declare `unsigned char retina = Globals::retinaDisplay;` **immediately before
  `this->field_0xb0`** (not at the top of the ctor), and
* replace `Globals::retinaDisplay` with `retina` in the **first four consecutive assignment
  statements starting at `field_0xb0`** — i.e. `field_0xb0`, `field_0xa0`, `centerYBase`,
  `marqueeWidth` (they are consecutive in source even though non-adjacent in the struct).

Measured: **6952 B, diff 5662, 5x clz, 0x vceq** — exactly the wave-144 numbers.
(Declaring the local at the top of the ctor instead gives 6820 B / 5814: the whole early
arm then shares the compare and vectorises, which is *not* what the original does.)

## 2. Better size-negative variants (new)

Starting from the 6952 variant, a raw-diff move-to-position climb over single-line `this->`
assignments found:

| variant | size | raw diff | clz | vbsl |
|---|---:|---:|---:|---:|
| HEAD (wave-144) | **7072** | **5704** | 5 | 7 |
| wave-144 local (above) | 6952 | 5662 | 5 | 9 |
| **best candidate** (`LayoutC1-w9008-candidate.cpp`) | **7068** | **5675** | 5 | 10 |
| best + `*(volatile unsigned char*)&field_0x285 = 2;` | 7076 | 5689 | 5 | 10 |

The best candidate is committed alongside this note as `LayoutC1-w9008-candidate.cpp`
(full ctor body, drop-in replacement).  It differs from HEAD by the `retina` local plus
~20 statement moves; the four `retina` uses in that variant are `field_0x238_blk16.b`,
`field_0x120`, `field_0x114_strip5c`, `field_0x108` (the climb moved them there).

## 3. The irreducible 4-byte gap

The original is 7072 = 0x1ba0; the best candidate is 7068 = 0x1b9c.  The missing 4 bytes are
**one literal-pool word** (or one 32-bit instruction).  Evidence:

* The original's final literal island (`d2a48..d2bc8`) has ~30 more words than the candidate's;
  the extra words are the retina/default select constants that only the original's duplicated
  shared-tail arms reference.
* Every padding trick tried lands on a multiple of 8 away from 7072: a volatile byte write adds
  8 B (7076), a volatile word read adds 8 B, `if (retina==7) …` before a store adds 16 B (7084),
  a `switch`/`?:` no-op is folded away.  Single-statement moves from 7068 give 7068 or 7076,
  never 7072.
* The original has 11 `vbsl` groups; the candidate has 10.  The missing group is the
  `0x1d0..0x1dc` select (original `d214c`).  Forcing it by making those four statements
  contiguous gives 7076 / 5789 (too big and worse diff), so it cannot be used as the 4-byte filler.

## 4. Faithful-order check (analysis only)

`_work/reconstructions/layout_ctor/gen.py` + `lay_orig.json` (the original's per-flag store
table) regenerates the whole ctor in the true iOS/default source order.  Measured:
flat faithful order **6488 B / 5895**; nested iPadHD/n9 blocks **6408-6476 B / 5645-5749**.
All are far below 7072, confirming the original's size comes from the duplicated shared-tail
arms (the `vbsl` select groups), not from the store order.

## 5. Not landed

`src/game/ui/Layout.cpp` is unchanged (baseline 7072 / 5704) because the best candidate is
4 B short and any size change shifts every later function and drops `byte_exact`.
The candidate is committed as a source file so a follow-up can add the missing literal word /
`0x1d0` select group without re-deriving it.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9008
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9008/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
# candidate: cp _work/reconstructions/LayoutC1-w9008-candidate.cpp src/game/ui/Layout.cpp
#            -> 7068/7072, diff 5675
```

Scratch (`/dev/shm/w9008/`): `Layout.base.cpp`, `Layout.best.cpp`, `Layout.volw3.cpp`,
`sweep4.py` (the exact 4-statement window sweep), `climb*.py`, `combo*.py`, `adduse.py`.

## 7. Open questions

* Find the one extra literal-pool word (or 32-bit instruction) that takes the 7068 candidate to
  7072 without changing the 5675 diff.  The original's `0x1d0..0x1dc` `vbsl` group (`d214c`) is
  the most likely source, but making it contiguous costs 8 B, not 4.
* The candidate's four `retina` uses are a metric-only choice (the climb moved them off the
  first four statements); the faithful source would use the local for the `0xb0..0xb4` group.
