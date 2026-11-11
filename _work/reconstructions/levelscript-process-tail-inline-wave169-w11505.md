# LevelScript::process — wave 169 / w11505: mission-78 tail + case-body shape

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11505`, branch `wave169-w11505`, base master
`00c403d5a`.  Scratch `/dev/shm/w11505/`.

Baseline on entry (rebuilt from the clean tree):

```
raw 53,970   aligned 17,818   size 57,232/57,232   insn 20,722/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. The original tail is shape B (post-switch), re-confirmed

Original `0x13c88c` is reached by the switch default (`bhi.w 0x13c88c` at
`0x138380`) *and* by every `break` (e.g. case 11 `b.w 13c88c` at `0x13be92`).
The tail is therefore the first statement **after** the switch, not a
`default:` label:

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
13c8be  b.w  144f5a          ; shared bool epilogue
```

i.e. shape B: `if (state>=5 && state<=8) translate(...); return flag;`.
`default:` is folded away by clang (it is statically dead for states 0/>12);
measured identical to HEAD.

## 2. Live-tail shapes on this base (full link, `tools/fn_diff.py`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (tail dead inside switch) | **53,970** | **17,818** | **57,232** |
| A: tail after switch, no return | 55,572 | 20,364 | 57,264 (+32) |
| B: tail after switch + `return flag` | 55,543 | 20,750 | 57,380 (+148) |
| B + `m_nState = 13;`/return swap | 55,542 | — | 57,380 (+148) |
| B, `__builtin_expect(...,0)` | 55,531 | 20,698 | 57,364 (+132) |
| B, `__builtin_expect(...,1)` | 55,541 | 20,750 | 57,380 (+148) |
| goto to label before final return | 55,896 | 24,670 | 57,536 (+304) |
| `default:` tail (folded) | 53,970 | 17,818 | 57,232 |

Equivalent spellings (`bool` return, `!!flag`, `(unsigned)(state-5)<=3`)
compile to the same binary as B.  Shape B flips the whole function's
allocation (`r9 = this`, `sl = guard`, delta at `[sp,#84]`, epilogue at
`0x144cec`) and inserts its 68-byte tail at `0x13c418`, not the original
`0x13c88c`: our mission-78 block is still ~0x474 shorter than the original's
(the original interleaves the mission-106 state-2 cold block, 0x9f6 B, between
case 11 and the tail; ours places it after case 11).  Hence the raw regression
is a block-placement shift, not a code-shape defect.  No size-exact,
raw-non-regressing live-tail form was found.

## 3. Inlined-vs-called split — re-confirmed

The 245-name PLT callee set is identical (wave-165/167 result holds).  The
tail's `TargetFollowCamera::translate(fff)` is a PLT call in both; it is not
inlined.  `LevelScript::resetCamera` is the only same-TU callee and is called
in both.  No TU move can change `process` inlining.

## 4. Landed: case-11 translate argument shape (size-exact, aligned win)

Original case 11 (`0x13bd2c`) computes the third `translate` argument as
`(float)(delta * 3)` — integer `add r0,r4,r4,lsl#1` then `vcvt` — while our
source had `(float) delta * 3.0f` (`vmov.f32 s0,#8` + `vmul`).  Changing it to
`(float) (delta * 3)` makes the 16-instruction sequence match the original
instruction-for-instruction (only `sl`/`fp` differ):

```
raw 53,970 -> 53,979   aligned 17,818 -> 17,780   size 57,232/57,232
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207, lint CLEAN
```

Raw is +9 (literal-pool placement moves case 11 from `0x13ba98` to
`0x13bad0`, shifting the already-divergent body); aligned improves by 38
instructions.  Kept as a structural win, honestly reported.

Related shape found but **not** landed (size-fatal): original case 2
(`0x13ba5c`) negates the integer delta before converting
(`negs; vmov; vcvt; vmul 0.5`), i.e. source `(float)(-delta) * 0.5f`, while
ours `-(float) delta * 0.5f` folds to `vcvt; vmul #-0.5`.  Measured alone:
raw 54,821 / aligned 17,756 / size 57,240 (+8); with the case-11 fix:
raw 54,930 / size 57,248 (+16).  It needs a -8/-16 companion elsewhere in
`process` and was left out.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=53979 aligned=17780 size=57232/57232
```
