# `Layout::Layout` C1/C2 — wave 126 / worker w7622

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-125 final master `4bbe88776` (48.943802311678 % unrounded).

## Result — no source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802311678 % | **48.943802311678 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

No `src/` change landed; the worktree is clean apart from this note.

## 1. The "one fewer callee-saved register" is one extra *local-stack* word

The brief's hint is exactly measurable in the prologue/epilogue:

| | original | HEAD |
|---|---|---|
| first push | `push {r4,r5,r6,r7,lr}` | same |
| fp setup | `add r7,sp,#12` | same |
| second push | `stmdb sp!, {r6,r7,r8,r9,sl,fp}` = **6 words** | `stmdb sp!, {r5,r6,r7,r8,r9,sl,fp}` = **7 words** |
| epilogue | `ldmia.w sp!, {r2,r3,r8,r9,sl,fp}` = 6 | `ldmia.w sp!, {r1,r2,r3,r8,r9,sl,fp}` = 7 |

The extra word is not a callee-saved register: the real saved set is
`r4,r5,r6,r8,r9,r10,r11` in **both**.  The trailing registers in the second
push are the *folded local stack frame* (`tryFoldSPUpdateIntoPushPop` in
LLVM 7 `ARMFrameLowering::emitPrologue`): `NumBytes` is 8 in the original and
12 in HEAD.  Confirmed by the stack-slot census:

| slot | original | HEAD |
|---|---|---|
| `[sp]` | `strd fp,sl` @`d1570`, reloaded @`d16a4/d16d2/d1710` | `strd lr,sl` @`d1af2`, reloaded @`d1d68/d1df6` |
| `[sp,#4]` | `str r1` @`d1880` (`&Globals::iPad`), reloaded `r6` @`d1ac2/d1cb8/d2802` | `str.w sl` @`d1568`, reloaded @`d16ae/d16e8/d1e1a` |
| `[sp,#8]` | — | `str r3` @`d198c`, reloaded `r9` @`d211e/d2182/d218c` |

So HEAD spills one extra 4-byte value (`r3` at `d198c`, live until `d211e`).
Eliminating that spill would make the 24-byte prologue/epilogue byte-identical
and shift every stack offset by +4; it is a Greedy-RA live-range outcome, not
a source-type issue.  No source edit tested below changed it.

## 2. HD block: first 9 SLP groups match; divergence is a one-field window shift

Per-block difflib ratios (raw instruction keys, orig/ours boundaries):

| block | orig insn | ours insn | ratio |
|---|---:|---:|---:|
| prologue | 9 | 9 | 88.9 % |
| HD (`d1042..d126c` / `..d1278`) | 164 | 169 | 37.2 % |
| n9 check | 6 | 6 | 100 % |
| n9 (`d127e..d1570` / `d128a..d1568`) | 342 | 331 | 84.4 % |
| common1 (`d1570..d21e2` / `d1568..d229e`) | 1059 | 1102 | 43.9 % |
| final (`d21e2..d2800` / `d229e..d28ba`) | 493 | 509 | 44.7 % |
| cold+pool | 464 | 373 | 73.6 % |

The HD block's `vst1.32` destination sequence is identical for the first nine
groups (`0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c`), then shifts by one
field:

| # | original dest (values) | HEAD dest (values) |
|---:|---|---|
| 10 | `0xc0` `[9,11,5,253]` | `0xb4` `[20,25,11,9]` |
| 11 | `0xb0` `[4,20,25,11]` | `0xc4` `[11,5,253,21]` |
| 12 | `0xec` `[56,112,281,562]` | `0xec` `[56,112,281,562]` |
| 13 | `0x294` `[281,118,16,16]` | `0x294` `[281,118,16,16]` |
| 14 | `0x2a4` `[154,140,147,42]` | `0x2a4` `[154,140,147,42]` |
| 15 | `0x108` `[-2,1,2,1]` | `0xdc` `[15.0,1.5,12.0,1.5]` |
| 16 | `0x118` `[9,35,21,1]` | `0x118` `[9,35,21,1]` |
| 17 | `0x128` `[1,21,78,151]` | `0x108` `[-2,1,2,1]` |
| 18 | `0x140` `[15,18,2,189]` | `0x148` `[2,189,5,70]` |
| 19 | `0x150` `[5,70,70,206]` | `0x158` `[70,206,68,0]` |
| 20 | `0x160` `[68,0,35,184]` | `0x168` `[35,184,174,84]` |
| 21 | `0x170` `[174,84,84,217]` | `0x180` `[23,163,23,244]` |
| 22 | `0x180` `[23,163,23,244]` | `0x190` `[-6,8,16,90]` |
| 23 | `0x190` `[-6,8,16,90]` | `0x1a0` `[18,22,22,243]` |
| 24 | `0x1a0` `[18,22,22,243]` | `0x1b0` `[18,42,46,1]` |
| 25 | `0x1b0` `[12,14,112,42]` | `0x1c0` `[92,26,40,50]` |
| 26 | `0x1c0` `[92,26,40,50]` | `0x1d0` `[7,17,5,14]` |
| 27 | `0x1d0` `[7,17,5,14]` | — |

The original groups `0xb0..0xbc` and `0xc0..0xcc`; HEAD groups `0xb4..0xc0`
and `0xc4..0xd0`.  The source order is `0xb0,0xcc,0xb8,0xb4,0xc4,0xbc,0xc0,0xc8`
(lines 71/75/77/78/82/83/84/86).  The SLP vectorizer is scheduling-driven, so
attempting to fix the window by reordering the eight source lines does **not**
change the emitted groups and only shrinks the body (see §4).

## 3. Literal-pool evidence (re-confirmed)

* HEAD end pool starts at `0xd28d0` with `0x43160000` (150.0f) + SLP integers;
  the original end pool starts at `0xd2854` with
  `0.325f, 0.65f, 100.0f, 50.0f` then GOT deltas then
  `281.25f, 400.0f, 200.0f, 150.0f`.
* HEAD materialises `0.325f/0.65f` in a `.text` island at `d1928/d192c`
  (original: end pool `d2854/d2858`), and materialises `82.0f/41.0f` in the
  end pool where the original stores the int chain `82/41` via
  `vmov/vcvt/vstr` (`d19de` `vstr s0,[r0,#0xa8]`).  This is the one genuine
  source-type mismatch: `field_0xa8` is an int chain in the original.
  The faithful shape (`int v = ...82:41; field_0xac = v; field_0xa8 = v;`)
  measures **7016 B / 48.930642 %** — same score, 56 B short.
* HEAD's extra `[sp,#8]` spill (see §1) is the only pool/frame difference
  not explained by SLP grouping.

## 4. Measured variants (all real `link_so` builds, exact unrounded ratio)

Scorer: `score.py` = `tools/verify/verify.py` `Resolver` + `reconcile` +
`difflib(autojunk=False)` on `_ZN6LayoutC1Ev`; reproduces the full-link number
bit-for-bit.

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| 8-field `0xb0..0xcc` ascending, contiguous | 7016 | 46.767757 |
| 8-field descending, contiguous | 7008 | 39.263558 |
| 8-field order `c0,c4,c8,cc,b0,b4,b8,bc` | 7056 | 44.400159 |
| `0xb4` moved before `0xb8` | 7080 | 48.815449 |
| `0xb4` moved after `0xb0` | 7080 | 48.556065 |
| `0xc4` moved after `0xc0` | 6968 | 44.359695 |
| `0xcc` moved after `0xc8` | 7064 | 43.608124 |
| `0x2c0` moved before `0x2c4` | 7068 | 45.383848 |
| `0x2c0..0x2d0` ascending after `0x78` | 6976 | 41.862332 |
| `0x2c0..0x2d0` descending after `0x78` | 7032 | 41.501897 |
| `field_0xa8` int chain (variant C) | 7016 | 48.930642 |
| `field_0xa8` int chain (variant A) | 7024 | 48.701558 |
| drop `volatile` on `rd` | 7064 | 48.853440 |
| swap `retinaDisplay || n9` | 7072 | 48.943802 (no-op) |
| 300 random single-line swaps (all `this->` lines) | — | best 7072 hit = 48.943802 |
| 120 random permutations of the 8 `0xb0..0xcc` lines | — | best 7072 hit = 48.943802 |

No variant both stays at 7072 B and beats the baseline.  The 7072-B wall is
hard: every grouping/order change that reproduces the original's SLP windows
shrinks the body by 8–104 B, and the fuzzy metric is maximised by the current
field-major statement order.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7622
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7622/score.py                          # 7072 / 48.943802311678
```

Scratch harnesses (not committed, `/dev/shm/w7622/`): `score.py`
(exact single-function scorer), `tryvar.py` (build+score a line list),
`slp.py` (SLP group/destination tracer), `block2.py` (per-block ratios),
`hddiff.py` (HD-block instruction diff), `rand.py` (random swaps),
`perm8.py` (8-field permutations), `dump.py` (pool word dump),
`Layout.base.cpp` (HEAD source snapshot), `orig.dis`, `ours.dis`.
