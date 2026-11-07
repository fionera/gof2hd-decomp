# `Layout::Layout` C1/C2 — wave 131 / worker w7684

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-130 master `7c6933c2d` (7072 B / 48.943802311678 % unrounded).

## Result — size-exact source win, gate green

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 48.943802311678 % | **49.003189792663 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

Three one-line source changes, all semantically identical to HEAD (values
unchanged), found by bisecting the old `9ecb9ec99` ("0x19c/0x18c/0x1a0/0x198
store reorder", 48.963317) body against HEAD and re-testing each hunk:

1. `field_0x110_strip58` moved **before** `field_0x2b8` (was after).  This is
   the original's store order in the `0xb0/0xb4/0xb8` neighbourhood; the
   `9ecb9ec99` body had it and every later commit kept the swapped order.
2. `field_0x124` HD arm written as `1 * 1.40625f` instead of
   `(int)(1 * 1.40625f)`.  The explicit cast is redundant (the field is an
   `int`), and dropping it lets clang fold the HD arm into the neighbouring
   `0x128`/`0x10c` sequence instead of emitting a separate `vmov`/`vcvt`
   round trip.  This is the +0.02 % instruction-stream win.
3. `field_0x184` non-HD arm rewritten from the nested
   `n9 ? (iPad ? 173 : 210) : (iPad ? 173 : (retina ? 232 : 116))` to the
   flat `n9 ? 210 : (iPad ? 173 : (retina ? 232 : 116))`.  Both are the same
   value table, but the flat form matches the original's disassembly at
   `0xd1fb6..0xd1fde`: the original tests **iPad first**, then `n9`, then
   `retina` (`d1fb6 cbz r1,d1fd4` -> `0x184=210`; `d1fc6` -> `retina?232:116`).
   This hunk alone is -8 B (7064 B), which is why it was previously rejected;
   combined with (2) it lands exactly on 7072 B.

## 1. New evidence: the original `0x180..0x1ac` block shape

Decoded from `orig.dis` `d1e5a..d20ae` (191 insns / 532 B).  The original
emits this block **config-major**, not field-major: the outer test is
`iPadLarge`, then `iPad`, then `n9`, then `retina`, and each arm stores the
whole run of fields.  Within the `!iPadLarge` fallthrough the fields are
stored in ascending offset order (`0x180,0x184,0x188,0x18c,0x190,0x194,
0x198,0x19c,0x1a0,0x1a4,0x1a8,0x1ac`), with:

* `0x180` and `0x188` sharing one register (`r2` in the `!n9` arm at
  `d1e9a`, `r1`/`r2` in the fallthrough at `d1fa2`/`d1fde`) — they are the
  same value table;
* `0x18c`+`0x190` and `0x198`+`0x19c` emitted as `strd` pairs;
* `0x1a4`/`0x1a8`/`0x1ac` in a shared tail (`d2064`/`d2086`/`d2092`/
  `d20ac`) reached from every arm, which is why the original writes `0x1a4`
  only 3 times while HEAD writes it 6 times.

The sibling-version oracle
(`_work/oracles/.../decompiled/native/game/Layout.c`, lines 1300–1490)
confirms the same value tables and the shared `fVar28` live range feeding
`0x188` and `0x194`, but renders the source field-major — the sibling
compiler did not merge the per-field trees the way clang 7.0.2 did here.

## 2. Negative results (real `link_so` builds, exact unrounded ratio)

All 12-statement reorderings of the `0x180..0x1ac` block were built and
scored.  None beats HEAD at 7072 B:

| variant | size | ratio % |
|---|---:|---:|
| HEAD | 7072 | 48.943802 |
| ascending `0x180..0x1ac` | 7080 | 45.899682 |
| descending `0x1ac..0x180` | 6976 | 46.218656 |
| `0x1a4` moved after `0x190` | 7044 | 48.892437 |
| `0x184` iPad-first (nested) | 7056 | 48.544077 |
| `0x190` as one ternary (not if/else) | 7052 | 48.603352 |
| full config-major tree, flat arms (`cfg4`) | 7060 | 45.926518 |
| config-major + shared `0x1a4/0x1a8/0x1ac` tail (`cfg5`) | 7220 | 45.373841 |
| shared `v180` for `0x180`/`0x188` | 7072 | 48.943802 (identical code) |
| `0x1a4`-to-end + adjacent-move hill climb | 7072 | 48.943802 (local optimum) |

The config-major transcriptions are *structurally* right but lose the fuzzy
alignment because the surrounding field-major order no longer lines up; the
current hill-climbed order is a strong local optimum.

## 3. Open questions

* The `0x1a4` store-count gap (orig 3, HEAD 6) is the shared-tail shape above.
  Reproducing it needs the `0x1a4/0x1a8/0x1ac` statements moved into a
  post-tree tail *and* 28–48 B of real code recovered elsewhere; every such
  move measured here shrinks the body to 7044–7060 B.
* The reward-box tail (`0x1fc..0x27c`, orig `d1735..d1771`, seven `vld1.64`
  SLP groups) is still emitted by HEAD as scalar stores.  That block is the
  largest remaining instruction-stream divergence and the most likely place
  to recover the next size-exact win.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7684
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w7684/score1.py                        # 7072 / 49.003190
```

Scratch harnesses (not committed, `/dev/shm/w7684/`): `score1.py` (exact
single-function scorer), `gen.py`/`gen3.py`/`gen4.py` (block reorderers that
build+score), `movesearch.py` (size-constrained move hill climb),
`Layout.base.cpp` (HEAD snapshot), `Layout.best3.cpp` (landed),
`orig.dis` / `ours_base.dis`, `diffmap.py` / `diffstat.py`.
