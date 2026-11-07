# `Layout::Layout` C1/C2 — wave 137 / worker w8210
## `0xdc` strd-vs-vst1 and `0x140..0x1d0` tail-group boundary: disassembly census + source-shape sweep (no landed change)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-135 master `0e7adc2a7` (the current HEAD; wave-136 only added docs).

## Result — no source win landed; baseline re-confirmed

| metric | HEAD | best candidate |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | 7072 (several, none better) |
| target-function differing bytes | **5,757** | 5,757 (tie only) |
| unrounded `difflib` fuzzy | **43.6 %** | 43.8 % (at size 7088) |
| `.text` size | `0x1337c8` | unchanged |
| byte_exact / linked_exact | 4507 / 4507 | 4507 / 4507 |

`cmake --build cmake-build-match --target check -j6` → **RATCHET PASS**
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN).

> Note: the task brief quoted 44.8 % fuzzy; the *current* HEAD built with the current
> `tools/verify/verify.py` scores **43.6 %** (the repo-root `report.json` saying 45.0 is stale).
> 43.6 % is the figure this note uses as the no-regression floor.

## 1. Ground truth — full HD-arm store census (manual objdump, base-register tracked)

The w8108 note said "ours emits a `vst1` at `0xdc` where the original emits four `vstr`".
That is wrong for the HD arm. The original HD arm stores the `0xdc..0xe8` float quadruple as
**two `strd`** (integer doubleword stores of the constant bit patterns), not four `vstr`:

```
orig HD arm, d11c0..d11d4:
  d11c0  stmia.w fp, {r3, r6, r8}     ; fp = r0+0xd0  -> 0xd0,0xd4,0xd8
  d11c8  strd    r2, r4, [r0, #220]   ; 0xdc = 14.0625f (r2=*(d14bc)), 0xe0 = 1.40625f (r4=*(d1444))
  d11cc  movs    r2, #5
  d11ce  strd    r1, r4, [r0, #228]   ; 0xe4 = 28.125f  (r1=*(d14b8)), 0xe8 = 1.40625f (r4)
```
Ours (HD arm, object offset `0x15a`):
```
  15a  add.w  r3, r0, #220 ; 0xdc
  15e  vst1.32 {d2-d3}, [r3]          ; one 16-byte store of a 4-word literal-pool vector
```
So the residual is **two `strd` vs one `vst1`** (ours is 1 instruction / 8 bytes shorter here).
The four `vstr` at `0xdc..0xe8` in the original are in the *common* arm (`d1bf0..d1c04`), and
ours already matches those (`c96/c9e/ca2/ca6` in the object).

## 2. Ground truth — `0x140..0x1d0` tail groups

Original HD arm vectorizes the whole 40-int run `0x140..0x1dc` as ten 16-byte `vst1` groups:

```
0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0   (orig)
```
Ours HD arm groups (object offsets `0x1ce..0x23c`):

```
0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x1d0 0x1c0        (ours)
```
The first three are shifted **+8**: ours starts at `0x148`, the original at `0x140`.
Root cause: in our source `field_0x140` (line 295) and `field_0x144` (line 285) are emitted in
the *common* arm, not the HD arm (`str 0x144` at `0x15a6/0x161a/0x1674`, `str 0x140` at
`0x16bc` in the object), so the HD arm's vectorizable run begins at `0x148`. The original
source evidently kept those two assignments in the device-specific section, so its run begins
at `0x140`.

## 3. Source-shape sweep (every entry is a real `link_so` build, symbol size checked)

`osize` is always 7072; `usize` is our symbol size (7072 required for a landing).

| variant | change | usize | diffbytes | fuzzy |
|---|---|---:|---:|---:|
| HEAD | — | **7072** | **5757** | **43.6** |
| B1 | move `0x140`,`0x144` before `0x14c` | 7032 | 5943 | 42.1 |
| B2 | make `0x140,0x144,0x148,0x14c` contiguous ascending | 7008 | 5972 | 37.0 |
| B4 | move `0x140`,`0x144` before `0x148` | 7032 | 5966 | 42.1 |
| D1 | move only `0x140` before `0x14c` | 7048 | 5968 | 42.0 |
| D2 | move only `0x144` before `0x14c` | 7056 | 5962 | 42.4 |
| D3 | move only `0x140` before `0x148` | 7056 | 5992 | 42.1 |
| D4 | move only `0x144` before `0x148` | 7056 | 5956 | 42.4 |
| D5 | move both before `0x13c` | 7032 | 5944 | 42.1 |
| C1 | make `0xdc,0xe0,0xe4,0xe8` contiguous ascending | 7032 | 5896 | 43.3 |
| E1 | pair `(0xdc,0xe0)` adjacent, `(0xe4,0xe8)` left scattered | 7080 | 5836 | 43.7 |
| E2 | pair `(0xe4,0xe8)` adjacent, `(0xdc,0xe0)` left scattered | 7072 | 5851 | 43.3 |
| S1 | ascending sort of every statement with offset in `0xb8..0x118` | 7016 | 5846 | 43.3 |
| S2 | ascending sort of the contiguous source block `field_0xb8 .. field_0x118` | 6996 | 5982 | 40.3 |
| T1 | full ascending sort of the whole `0x140..0x1dc` tail | 7072 | 5953 | 21.8 |
| P1C | `0xdc` permutation (grew +16) + `0x1c0` group move | 7104 | 5994 | 41.7 |

Additional sweep: all **24 permutations** of the four `0xdc` statements placed *contiguously*
give `usize = 7032` (diff 5897–5914); all **24 permutations** that keep the four statements at
their original scattered source positions give `usize = 7072` for 8 of them (diff 5757, fuzzy
43.6 — codegen-identical to HEAD) and `usize = 7080/7088` for the rest (diff 5827–5829, fuzzy
43.6/43.8). None produces the original's `strd` pair; the contiguous forms all become one
`vst1` and shrink the body.

## 4. Conclusions

* Moving `0x140/0x144` into the device section **does** flip the HD-arm group start from
  `0x148` to `0x140`, but it removes two scalar stores from the common arm and lets the HD arm
  vectorize a group, shrinking the body by 16–64 B (7032/7008). Every such variant also scores
  worse on raw bytes and fuzzy. The missing bytes are the known "common arm too big / tail too
  small" residual, not in this group.
* The `0xdc` group cannot be flipped to `strd` by source reordering alone: contiguous placement
  always SLP-vectorizes to one `vst1` (and shrinks 40 B), scattered placement keeps the `vst1`
  at `0xdc` regardless of order. The original's `strd` pairs require the SLP vectorizer to
  decline the 4-wide group while the backend still merges pairs — not reachable from member
  order in this TU with clang 7.0.2 `-Oz`.
* The flat body is a strong local optimum: 24 `0xdc` permutations, 5 `0x140/0x144` moves and
  three ascending sorts all regress. HEAD is kept; no source change landed.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8210
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8210/score.py                            # 7072/7072, 5757 diff bytes
python3 /dev/shm/w8210/fuzzy.py                            # 43.6 %
```

Scratch harnesses (not committed, `/dev/shm/w8210/`): `score.py` (symbol-size + raw diff via
`Elf32`), `fuzzy.py` (verify.py normalize/reconcile/difflib for one symbol), `stores.py` /
`stores2.py` (base-register-tracked `vst1`/`vstr`/`str`/`strd`/`stmia` census), `gen.py` /
`batch*.py` / `perm.py` / `perm2.py` / `sortrange.py` / `sortblock.py` (source-reorder
harnesses), `Layout.base.cpp` (pristine wave-135 body).

## 6. Open questions

* The original's `strd`-pair form for `0xdc..0xe8` needs a source shape that defeats SLP but
  keeps the two 64-bit merges. The only untested axis is making the four values non-constant
  in the HD arm (e.g. routing one through a local), which changes the literal-pool order.
* The `0x140` group flip is understood but not free: it needs a compensating +40 B elsewhere in
  the common arm (the wave-135 "common arm 168 B too big / tail 186 B too small" residual).
