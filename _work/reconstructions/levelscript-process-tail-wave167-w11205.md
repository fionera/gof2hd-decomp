# LevelScript::process — wave 167 / w11205: mission-78 dead tail reconstruction

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11205`, branch `wave167-w11205`, base master
`d00057e6d` (wave 166, includes w11103's expect sweep).  Scratch
`/dev/shm/w11205/`.

Baseline on entry (rebuilt from the clean tree):

```
raw 54,095   aligned 17,621   size 57,232/57,232   insn 20,715/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
lint CLEAN / drift 0 / parity 5370
```

## 1. Re-verified histogram (independent parser, full function range)

Parsed `objdump -d` over `0x137d18..0x145ca8` (the full 0xdf90 body), counting
`bl`/`blx` to `@plt` targets:

| metric | original | ours |
|---|---:|---:|
| PLT calls | 2,355 | 2,362 |
| unique PLT callee names | **245** | **245** |
| set difference | **none** | **none** |

This confirms the wave-166 note: the callee *set* is identical; no callee is
inlined in the original that we call.  `LevelScript::resetCamera` (the only
same-TU callee) is a PLT call in both.  TU membership is therefore **not** a
lever for `process` inlining.

`ArrayAdd<Player*>` is **called**, not inlined: both bodies call
`_Z8ArrayAddIP6PlayerEvT_R5ArrayIS2_E` twice (original `0x140bfe`/`0x140c0c`,
ours the same addresses).  The wave-166 note's parenthetical "(no call in
either body)" is wrong, but the conclusion (identical callee set) stands.
The symbol itself is at `0xa2bbe` in both, so moving its explicit
instantiation from `ArrayInstantiations.cpp` into `LevelScript.cpp` (its
original TU) would not change `.text` placement or any gate count.

## 2. The mission-78 (0x4e) shared tail

Original `0x13c88c` (reached by the switch default `bhi.w 0x13c88c` at
`0x138380` and by every substate `break`):

```
13c88c  ldr   r0,[sl,#28]      ; m_nState
13c890  subs  r0,#5
13c892  cmp   r0,#3
13c894  bhi   13c8ba
13c896  ldr   r0,[sp,#76]      ; delta
13c898  movs  r2,#0
13c89a  vldr  s2,[pc]          ; 0.8f @0x13c8f8
13c89e  movs  r3,#0
13c8a0  negs  r0,r0
13c8a2  vmov  s0,r0
13c8a6  vcvt.f32.s32
13c8aa  ldr   r0,[sl,#20]      ; m_pCamera
13c8ae  vmul.f32
13c8b2  vmov  r1,s0
13c8b6  blx   TargetFollowCamera::translate(fff)
13c8ba  ldrb  r5,[sl,#17]      ; m_bCinematicBreak
13c8be  b.w   144f5a           ; shared bool epilogue (cmp/it/movne r5, return r5)
```

So the faithful source is the tail **after** the switch, followed by
`return m_bCinematicBreak != 0;`.  Our source has the `if` *inside* the switch
(after `case 11`'s closing brace, `LevelScript.cpp:1239`), so clang drops it as
unreachable and the mission-78 arm falls through to the post-switch mission
tail (`0x144e36` in the original / `0x144b82` in ours) instead of the epilogue.

### Measured shapes (full link, `tools/fn_diff.py`)

| shape | raw | aligned | size | register file |
|---|---:|---:|---:|---|
| HEAD (tail dead) | **54,095** | **17,621** | **57,232** | `fp` = this, `sl` = Globals |
| A: tail moved outside switch, no return | 55,486 | 20,170 | 57,252 (+20) | `fp` = this, `sl` = Globals |
| B: tail outside + `return m_bCinematicBreak != 0;` | 55,433 | 20,628 | 57,372 (+140) | `r9` = this, `sl` = this |

The wave-166 note measured A as size-exact (57,232) and B as +132 on its
pre-w11103 base.  On the current base A is +20 and B is +140.

### Why B is +140, not just the 56-byte tail

* B adds its own `translate` call and the `return`; the original also has its
  own tail translate, so the *local* code is faithful (B's tail block is
  68 B vs the original's 56 B; the extra 12 B are the inline
  `cmp/it/movne r5` bool normalization and an `ldr sl,[sp,#72]`).
* B flips the whole function's register allocation: the prologue becomes
  `mov r9,r0` instead of `mov fp,r0`, and the Globals pointer moves from `sl`
  to the stack.  That global change is most of the +140.
* A keeps the baseline allocation (`mov fp,r0`) and reuses the shared
  translate at `0x137e7e` (which sets `r0=m_pCamera`, `r1=s0`, `r2=r3=0`), so
  it is cheap but semantically wrong: it falls into the mission-0/1 active
  tail (`0x137f7c`) instead of returning.

### Declaration-order sweep (24 permutations)

Swept all 24 orders of the four prologue locals
(`messages, player, enemies, status`) with shape B.  **Every** order is
57,372 B (+140); best raw 55,433 (current order `messages,player,enemies,status`
and `enemies,player,messages,status` at 55,434).  No order compensates the
+140.

## 3. Conclusion

* The 245-name PLT callee set is identical; no same-TU declaration move can
  change `process` inlining.  `ArrayAdd<Player*>` is called twice in both and
  already sits at the same address (`0xa2bbe`).
* The mission-78 tail reconstruction is structurally clear (shape B), but it
  is **size-fatal on its own**: +140 B.  Landing it needs a -140 B companion
  elsewhere in the same function (the whole function is one `.text` section,
  so the -140 must come from `process` itself).  No such companion was found
  in this pass; the 24-way declaration-order sweep cannot supply it.
* Nothing was landed; the tree is clean at the entry baseline.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=54095 aligned=17621 size=57232/57232 insn=20715/20830
```
