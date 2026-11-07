# `Layout::Layout` C1/C2 — wave 132 / worker w7703
## Literal implementation of the w7685 SLP store-group census (no byte-level win)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-131 master `70e451ffe` (7072 B / 48.943802311678 % unrounded,
`.text` differing bytes **85,379**, target-function differing bytes **5,836**).

## Result — no landable source win (note only)

| metric | before (HEAD) | after (best variant C) |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 48.943802311678 % | **42.837218569436 %** |
| target-function differing bytes | 5,836 | **5,929** |
| `.text` differing bytes | 85,379 | **85,472** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS` for the variant.
No `src/` change landed: every variant that matches more SLP groups also increases the
target-function byte diff, so the pristine wave-131 body is kept.

## 1. What was implemented (the w7685 census, literally)

The w7685 census says the original's HD-arm `vst1` groups are

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c | 0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118 |
0x128 | 0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

while ours is

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c | 0xb4 0xc4 0x294 0x2a4 0xdc |
0x118 0x108 0x128 | 0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

The three first-run differences are a one-field window shift: the original pairs
`(0xb0,0xb4)`, `(0xc0,0xc4)`, `(0xec,0xf0)`; ours pairs `(0xb4,0xb8)`, `(0xc4,0xc8)`,
`(0xdc,0xe0)`. The fix is to make the first field of each original pair source-adjacent
to the second:

* move `this->field_0xb0` (base line 71) to immediately before `this->field_0xb4` (line 78);
* move `this->field_0xf0` (base line 85) to immediately before
  `this->ringNearMidThreshold` (line 100, offset `0xec`).

Moving `this->field_0xc0` (line 84) before `this->field_0xc4` (line 82) is a no-op on its
own (the compiled output is byte-identical), because the scheduler already sinks `0xc0`
into the common arm; it only takes effect once the `0xb0` move has changed the schedule.

**This variant ("C") does exactly what the hypothesis asks:** the HD-arm `vst1`
destinations become

```
... 0xb0 0xc0 0x294 0xec 0x2a4 0x118 0x108 0x128 ...
```

i.e. `0xb0`, `0xc0` and `0xec` now match the original's groups exactly (only the
`0x140/0x150/0x160/0x170` vs `0x148/0x158/0x168` block and the intra-run order of
`0x294`/`0xec` and `0x108`/`0x118` remain). The body stays **7072 B** and the branch
tree is unchanged (136 branches, same mnemonic histogram as HEAD).

## 2. Why it is not a byte-level win

The group match is real but the literal-pool placement and register allocation diverge:

| variant | size | ratio % | target byte diff | `.text` diff | same-addr instrs |
|---|---:|---:|---:|---:|---:|
| **HEAD** | **7072** | **48.943802** | **5,836** | **85,379** | 337 |
| A: move `0xb0` only | 7072 | 44.479872 | 5,862 | 85,405 | 343 |
| **C: move `0xb0`+`0xf0`** | **7072** | **42.837219** | **5,929** | **85,472** | **362** |
| C + move `0x144` before `0x140` | 7072 | 42.518430 | — | — | 361 |
| C + move `0x150` before `0x148` | 7056 | 42.751745 | — | — | 366 |
| C + move `0x160` before `0x168` | 7064 | 41.138988 | — | — | 353 |

"same-addr instrs" = instructions whose mnemonic+operands are identical at the *same*
address (a stricter, byte-oriented proxy than difflib). Variant C improves that count
337 → 362, mostly in the HD arm (`[0xd1028,0xd1528)`: 122 → 143), but the difflib
alignment drops (1,228 → 1,075 equal instructions) and the actual differing-byte count
rises by 93. The `add r1, pc, #imm` operands of the now-matching stores still point at
different literal-pool slots, so the bytes do not converge.

## 3. Full ascending / paired reorder shrinks the body

The original source order (from the committed
`_work/reconstructions/layout-orig-ghidra-decompile.txt`, the shipped binary's own
decompile) assigns adjacent fields as 8-byte chunks: `*(longlong *)(this+0xb0)`,
`*(longlong *)(this+0xb8)`, `*(longlong *)(this+0xc0)`, `*(longlong *)(this+0xc8)`,
`*(longlong *)(this+0xec)`, `*(longlong *)(this+0xf4)`, ... i.e. pairs
`(N,N+4)` for N in `{0xb0,0xb8,0xc0,0xc8,0xec,0xf4,0x294,0x29c,0x2a4,0x2ac,0x108,0x110,
0x118,0x120,...}`. Our source pairs `(N+4,N+8)` instead.

Reordering the whole `0x8c..0x2b8` region into the original's pair order (without adding
the original's duplicated stores) gives:

| variant | size | ratio % |
|---|---:|---:|
| HEAD | **7072** | **48.943802** |
| `0x8c..0x2b8` sorted ascending | 6980 | 40.686079 |
| `0x8c..0x2b8` in the decompile's pair order | 6936 | 36.790371 |

Pairing adjacent fields lets clang emit more 8-byte stores, so the body *shrinks* by
92–136 B. This is the same conclusion as wave 131: matching the groups costs bytes, and
the original's extra ~64 B of outlined cold tails / duplicated common stores must be
supplied to get back to 7072. That is the branch-tree work, deliberately not re-run here.

## 4. Non-HD arm already matches

For completeness: the non-HD arm's `vst1` groups are already
`0xb0 0xc0 0xec 0x294 0x2a4 0x118 0x108 0x128` in both binaries (the original also has a
`0x140` group there; ours has a `0xdc` group instead). The whole residual is in the HD
arm, which is why the `0xb0`/`0xf0` source moves were the right target.

## 5. Conclusion / open question

The w7685 census is correct and the source moves that reproduce it are identified
(`field_0xb0` before `field_0xb4`, `field_0xf0` before `ringNearMidThreshold`). They make
the HD-arm groups match and keep the 7072-B size, but the byte diff rises because the
literal-pool slots and register allocation diverge. The next step is to pair the moves
with the original's duplicated common-arm stores (the decompile's repeated
`0x150/0x154/0x160/0x164/0x170/0x174` and `0x140/0x144` writes) so the group fix is paid
for by real code rather than by a scheduler side effect.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7703
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > ours.dis
python3 /dev/shm/w7703/stores.py orig.dis ours.dis   # base-register-tracked store census
python3 /dev/shm/w7703/score.py                        # 7072 / 48.943802
```

Scratch harnesses (not committed, `/dev/shm/w7703/`): `score.py` (exact single-function
scorer), `subscore.py` (first-N-instruction ratio), `sameaddr.py` / `sameaddr_range.py`
(address-aligned instruction match), `stores.py` (base-register-tracked store census),
`branches.py` (branch histogram), `try.py` / `batch*.py` (source-move harnesses),
`base.cpp` (pristine wave-131 constructor), `base.so` / `C.so` (link outputs).
