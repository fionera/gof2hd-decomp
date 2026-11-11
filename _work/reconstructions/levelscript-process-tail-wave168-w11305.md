# LevelScript::process — wave 168 / w11305: mission-78 dead tail, size-neutral search

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11305`, branch `wave168-w11305`, base master
`631c26f0b` (wave-167 harvest).  Scratch `/dev/shm/w11305/`.

Baseline on entry (rebuilt from the clean tree):

```
raw 54,031   aligned 17,764   size 57,232/57,232   insn 20,714/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. The tail (re-confirmed)

Original `0x13c88c` (switch default `bhi.w 0x13c88c` at `0x138380` + every
substate `break`; 21 branches in all):

```
13c88c  ldr  r0,[sl,#28]     ; m_nState
13c890  subs r0,#5
13c892  cmp  r0,#3
13c894  bhi  13c8ba
13c896  ldr  r0,[sp,#76]     ; delta
13c898  movs r2,#0
13c89a  vldr s2,[pc]         ; 0.8f
13c89e  movs r3,#0
13c8a0  negs r0,r0
13c8a2  vmov s0,r0
13c8a6  vcvt.f32.s32
13c8aa  ldr  r0,[sl,#20]     ; m_pCamera
13c8ae  vmul.f32
13c8b2  vmov r1,s0
13c8b6  blx  TargetFollowCamera::translate(fff)
13c8ba  ldrb r5,[sl,#17]     ; m_bCinematicBreak
13c8be  b.w  144f5a          ; shared bool epilogue (cmp r5,#0; it ne; movne r5,#1)
```

54 B.  The epilogue at `0x144f56` is shared with the post-switch mission tail
(`ldrb r5,[r9,#17]` at `0x144f56`, fall through to `0x144f5a`); the mission-78
tail loads the flag itself (`sl`) and branches straight to `0x144f5a`.

Our source has the same tail **inside** the `switch (m_nState)` braces after
`case 11` (`src/game/world/LevelScript.cpp:1239`), where it is unreachable and
clang drops it (no line info).  The mission-78 arm therefore falls through to
the shared "active" tail at `0x137f76` instead of returning.

## 2. Shapes measured (full link, `tools/fn_diff.py`)

All shapes keep `switch` bodies untouched; only the tail/return placement and
the position of the mission-78 `else if` change.

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| HEAD (tail dead) | **54,031** | **17,764** | **57,232** | 20,714 |
| B: tail after switch + `return m_bCinematicBreak != 0;` | 55,449 | 20,722 | 57,368 (+136) | 20,748 |
| B minus case-12 `return` (fall through to tail) | 55,411 | 20,643 | 57,344 (+112) | 20,743 |
| B, return only (no translate) | 55,398 | 20,633 | 57,296 (+64) | 20,723 |
| B, return only + case-12 `return` removed | 55,394 | 20,632 | 57,280 (+48) | 20,722 |
| A: tail after switch, no `return` | 55,496 | 20,294 | 57,252 (+20) | 20,712 |
| **M: mission-78 block moved to last `else if` + full tail** | **55,194** | **22,484** | **57,268 (+36)** | 20,753 |
| M, return only (no translate) | 55,130 | 22,468 | 57,236 (+4) | 20,730 |
| M, translate only (no `return`) | 55,557 | 20,114 | 57,328 (+96) | 20,778 |

Equivalent spellings of the tail condition/return (`(unsigned)(state-5)<=3`,
`!!flag`, `flag ? 1 : 0`, `bool r = ...; return r;`, `if/else`, guard form,
pointer copy of `this`) all compile to the **same** binary as B (55,449 /
57,368).  A `do { ... break; } while (0)` wrapper and a `goto` to a label before
the final return are both 57,376 / 57,392 (+144/+160).

### Why B costs +136

Adding a `return` at the end of the mission-78 block flips the whole
function's register allocation.  HEAD: `fp = this`, `r4 = &m_pLevel`,
`sl = &__stack_chk_guard`, `r9 = status`.  B: `r9 = this`, `sl = this+24`,
`fp = enemies`, guard address spilled to `[sp,#72]`, status to `[sp,#92]`.
The prologue alone changes 6 instructions.  The case-12 `return`
(`0x13b658` in HEAD: `ldrb r5,[fp,#17]; b.w 0x144c6a`) is merged into the new
after-switch return block, so the epilogue's flag load/normalisation is
duplicated at the tail (`ldrb; cmp; it; movne; ldr sl; b.w` = 16 B vs the
original's 8 B) and the epilogue's `this` register (`r9`) has to be live on
the new edge.

Measured decomposition: return only +64, translate only +20 (A), both +136
(non-linear: the return removes A's tail-merge of the `translate` call with
mission-1's call at `0x137e7e`).

### Why M is cheaper (+36)

Moving the mission-78 `else if` to the end of the dispatch chain (just before
the final `else { /* mission>=2 */ }`) lets clang place the tail at
`0x145840`, *after* the epilogue (`0x14448c`).  Both the `bhi` and the
translate's continuation become a single backward `b.w` to the epilogue's
`ldrb r5,[r9,#17]`; the normalisation is **not** duplicated.  Allocation
stays close to HEAD (`r9 = this`, `r4 = &m_pLevel`, `sl = &guard`,
`r8 = delta`).  The tail block itself is 58 B (original 54 B); the remaining
+32 is branch relaxation/displacement elsewhere.

M is still not size-neutral (+36), and it reorders the mission-id compares
(`78` after `95/99/109/119/126/133/160/161`), so it is not the original's
dispatch order either.

## 3. Declaration-order sweep (24 permutations, shape B)

All 24 orders of `messages, player, enemies, status` measure **57,368 B**
(+136); raw range 55,449..55,460, aligned 20,718..20,734.  Best raw is the
current order (`messages, player, enemies, status`, 55,449).  This confirms
wave-167 w11205's sweep on the new base; no order compensates the flip.

## 4. Conclusion

* No size-neutral form of the live tail was found.  The closest shapes are
  M return-only at **+4 B** (57,236, but the translate is missing) and
  M full tail at **+36 B** (57,268).
* Every shape that makes the tail live is also a raw **regression**
  (54,031 -> 55,130..55,557).  The reason is structural: our mission-78
  `switch` case bodies are much shorter than the original's
  (ours end at `0x13b660`, original at `0x13c864`, ~0x1200 B short), so the
  tail cannot land at the original's instruction index (`0x13c88c`) and the
  insertion shifts the whole remainder of the function.  The tail only
  becomes a raw win once the missing case-body code is reconstructed.
* The original's tail is **not** unreachable in the original: states 5/6/7
  (and 8 when its timer gate fails) leave `m_nState` in [5,8] on the `break`
  path, so the translate block executes.  It is only unreachable in *our*
  source because it is written after the last `case` label but inside the
  switch braces.
* Nothing was landed; the tree is clean at the entry baseline.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=54031 aligned=17764 size=57232/57232 insn=20714/20830
```
