# LevelScript::process — wave 169 / w11405: tail + inlined-vs-called split

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B), size-exact.
Worktree `wt/w11405`, branch `wave169-w11405`, base master `ec3ceb07c`.
Scratch `/dev/shm/w11405/`.

Baseline on entry (rebuilt from the clean tree):

```
raw 54,015   aligned 17,788   size 57,232/57,232   insn 20,715/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. Inlined-vs-called split — re-confirmed, no inlining difference

`_work/original_layout/original_layout.json` (`tu_symbols[...]['Classes/Game/LevelScript.cpp']`)
contains only the `LevelScript` methods plus `ArrayAdd<Player*>`.  `resetCamera` is the
only same-TU callee and is a PLT call 51x on both sides; `ArrayAdd<Player*>` is inlined
on both.  The wave-166 histogram (245-name PLT callee set identical) holds on this base.
No declaration move / TU merge can change inlining.

The residual call-count deltas are *call-site shape*, not inlining: 14 sites are
`blx rN` (virtual/GOT) in the original and direct PLT calls in ours.  One of them is
concrete and was fixed (below): `TargetFollowCamera::setPosition(Vector const&)` vs
`setPosition(float,float,float)`.  After the fix the `TargetFollowCamera::setPosition`
call-site split is **exactly** the original's: 8x `fff` + 59x `Vector const&` (67 total).

## 2. Verified local fixes landed

### 2a. `m_pCamera->setPosition(m_tempVec)` (mission-1 case 12, original `0x13d434`)

Original:
```
13d42e ldr r0,[sl,#20]
13d432 mov r1,r5              ; r5 = &m_tempVec (this+0x28)
13d434 blx TargetFollowCamera::setPosition(Vector const&)
13d438 movs r0,#13
13d43a strd r6,r6,[sl,#144]
13d43e b.n 13d4ec
```
Ours had `setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z)` (the `fff` overload:
`add r3,fp,#40; ldmia r3,{r1,r2,r3}`).  Changing it to `setPosition(m_tempVec)` emits
the original's `add r5,fp,#40; mov r1,r5; blx setPosition(Vector)`.
Measured alone: raw 54,459 / aligned 17,783 / size 57,224 (-8).

### 2b. mission-78 case 12: `m_nState = 13;` *before* the return

Original (`0x13b8b2..0x13b8bc`):
```
13b8b2 movs r0,#13
13b8b4 ldrb r5,[sl,#17]
13b8b8 str  r0,[sl,#28]
13b8bc b.w  144f5a            ; shared flag epilogue
```
Our source had `return m_bCinematicBreak != 0;` followed by dead `m_nState = 13;`
(clang -Oz dropped the store).  Swapping the two statements emits the store live and
matches the original sequence exactly.  Measured alone: raw 54,821 / aligned 17,791 /
size 57,240 (+8).

**Combined (landed):** raw 54,113 / aligned 17,780 / size 57,232 (exact) / insn 20,715.
The two size deltas cancel exactly; aligned improves by 8; raw +98 (branch-displacement
churn from the -8/+8 pair, both blocks now match the original instruction-for-instruction).

Gate after: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN.

## 3. Dead mission-78 tail — measured again, still not landable

The tail (`if (state-5<=3) camera->translate(-delta*0.8,0,0); return flag;`) is inside
the `switch` braces at `LevelScript.cpp:1237` and unreachable.  Moving it outside makes
it live but is size-fatal on this base:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (tail dead) | 54,015 | 17,788 | 57,232 |
| tail outside, case-12 return kept (A) | 55,544 | 20,321 | 57,264 (+32) |
| A + case-12 store/return swap | 55,542 | 20,320 | 57,264 (+32) |
| tail outside + case-12 falls to tail + return (A2) | 55,443 | 20,867 | 57,352 (+120) |

Shape A is the cheapest (+32).  Its tail is emitted at `0x139770` (right after case 1)
and tail-merges its `translate` with mission-1's call at `0x137e7e`, so the block is 40 B
instead of the original 54 B; the +32 is branch relaxation elsewhere.  The original tail
sits at `0x13c88c`, after all case bodies.  Block placement only matches once the
mission-78 case bodies are the original size (ours are ~0x1200 B shorter in that cold
region, compensated elsewhere), so the tail remains blocked on the case-body reconstruction.

Useful side effect found while measuring: making the tail live *un-merges* the two
`m_nScriptTimerA = 0; m_nState = 6;` sites (mission-1 `isTriggered` and mission-78 case 5).
Today they share one block at `0x13b678` emitting two `str.w` (8 B); the original has two
separate `strd r0,r0,[this,#0x90]` (4 B each).  Shape A restores exactly 29 same-register
`strd ...#144` sites (original: 29; baseline ours: 27).  So the tail fix is also the
prerequisite for the timer-store codegen.

## 4. Rejected

* `m_nScriptTimerB = 0;` next to `m_nScriptTimerA = 0;` — wrong: `m_nScriptTimerA` is a
  64-bit field at 0x90, so `strd r0,r0,[this,#0x90]` already zeroes the whole field.
  Adding B zeroes 16 B and clang emits `vmov.i32 q8,#0` + `vst1.64` (12 B), a regression.
* `setPosition(*camPos)` (line 3819) — size -4, raw 54,070, aligned 17,791; not a win
  on this base and not verified against the original block, so left out.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=54113 aligned=17780 size=57232/57232 insn=20715/20830
```
