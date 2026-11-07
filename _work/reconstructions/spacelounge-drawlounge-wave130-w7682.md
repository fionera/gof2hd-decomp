# SpaceLounge::drawLounge — wave 130 (w7682): faithful-clamp score recheck + weight-order window (no source win)

Status: research note (2026-11-07). Worktree `wt/w7682`, base `003a9623a`.
**No `src/` change landed**: the committed HEAD (`short pad3`, raw **95.676047 %**, 1948 B) is
still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave129-w7657.md` (raw-vreg knobs),
`-wave124-w7613.md` (MIR priority/hint), `SpaceLounge-drawLounge-wave123-w7315.md`
(weight-order impossibility), `-wave122-w7217.md`, `-wave121-w7117.md`.
This note supersedes nothing.

## 1. Residual (unchanged)

Only the clamp / join `0x1712b2..0x171330` differs:

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

The original right arm is the faithful 8-byte `itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`.
HEAD uses the `short pad3` sign-extend workaround (`ittt gt; movgt.w r1,fp,lsl#16;
addgt.w r1,r6,r1,asr#15; subgt.w r9,r0,r1`).

## 2. NEW: the faithful clamp scores 94.0 with the *current* branch-weight flags

The wave-129 note measured the faithful clamp (`width + pad * 2`, no `pad3`) at 94.0. This wave
re-measured it through the official `verify.py` scorer on the current build (which carries
`-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` for this TU) and confirms
**raw 94.0, 1948 B**. The faithful build's near block is:

```text
57a vmov r9, s18        ; x -> r9            (target r5)
584 sub.w r1, r9, fp    ; x - pad            (target r5 - r9)
58c mov r5, fp          ; r5 = pad
58e add.w r9, r0, fp    ; x = inset + pad    (target r5 = r0 + r9)
5a0 add.w r1, fp, r9    ; pad + x            (target r9 + r5)
5aa addgt.w r1, r6, sl  ; width + pad*2      (target: same!)
5ae subgt.w r9, r0, r1  ; x = ...            (target r5)
5b2 str.w r8, [sp,#12]  ; agent spill        (target: same)
5b8 ldr.w fp, [r4,#48]  ; lineHeight -> fp   (target: same)
```

So the faithful clamp already has the **correct right arm and the correct join shape**
(`lineHeight -> fp`, agent spilled at the join). Its only residual is the pad/x colouring
(pad=fp vs r9, x=r9 whole vs r5->r3->r9). The HEAD trades the right arm (3 instructions) for a
different colouring that happens to score higher under the verifier's sequence metric.

## 3. NEW: the `agent > pad > x` weight window (re-confirmed with instrumented LLVM)

The wave-123 note pinned the target to `agent.weight > pad.weight > x.weight` (so lineHeight
evicts pad from fp, then pad evicts x from r9). Instrumented `FINALDBG` weights on the faithful
source (current flags):

```text
x         2.946140e-04   size 4220   hinted $r3
agent     2.267318e-04   size 8768   hinted
pad       2.077779e-04   size 6368   unhinted
lineHeight 5.978897e-04  size 384
```

The MIR order is `x > agent > pad`, so the target needs x lowered below pad **and** pad kept
below agent. Sweeps this wave:

| knob | effect | result |
|---|---|---|
| `VRW=2147483650:F` (pad weight) | F 1.42..2.00 gives pad=r9, x=r5->r2->r9 | lineHeight always r8, agent spill early; 1944 B |
| `XWFACTOR=0.5..1.0` (x weight) | x<pad at F<=0.7 | pad=r8, x=r2 (whole allocation rotates); never pad=r9 |
| branch weights on faithful {1:1..2000:1, 1:7..1:100} | no ratio gives pad=r9 | best 0.9514 sequence ratio = 7:1 faithful |
| `volatile` pad load (`*(const volatile int*)&layout->field_0x94`) | pad weight 2.078e-4 -> 2.088e-4 | unchanged allocation, 59 diff insns |
| `pad + pad` / `2*pad` / `pad*2+pad-pad` in the right arm | weight use count changes | 36-40 diff insns, no pad=r9 |

The sharp transition is still `pad.weight > x.weight` (2.078e-4 * 1.42 = 2.95e-4 > 2.946e-4),
but every build that crosses it also crosses `pad > agent`, so lineHeight takes r8 instead of fp.

## 4. Other source shapes measured this wave (all <= HEAD)

| variant | raw % / diff | size | note |
|---|---:|---:|---|
| **HEAD (`short pad3`)** | **95.676** | 1948 | committed |
| faithful `width + pad*2` | 94.0 | 1948 | right arm + join exact, pad/x swapped |
| faithful + `volatile` pad load | 92.6 | 1948 | weight barely moves |
| faithful + `pad + pad` right arm | 94.6 | 1948 | right arm re-associates |
| faithful + branch-weight sweep | 94.0 (best) | 1948 | 2:1/4:1/8:1/1:2 give 1944 B |
| HEAD + `__builtin_expect` clamp arms | 91-92 | 1952 | adds branch layout |

## 5. Conclusion

The residual is the same greedy-RA colouring fact, now with two new pieces of evidence:
(1) the faithful clamp with the current flags is 94.0 and *does* reproduce the original right arm
and join shape, so the HEAD's `pad3` workaround is buying the pad/x colouring at the cost of the
right arm; (2) the target colouring requires the weight ordering `agent > pad > x`, and every
source/knob shape that puts pad above x also puts pad above agent, so lineHeight evicts pad
instead of agent and lands in r8. No source-level shape found this wave reaches pad=r9 *and*
keeps lineHeight in fp. The committed HEAD (`short pad3`, **95.67604667124228 %**, 1948 B)
remains the best shape.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7682
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# faithful clamp: replace the pad3 block in SpaceLounge.cpp with
#   int x = centerX;
#   if (x - pad < layout->buttonInsetX) x = layout->buttonInsetX + pad;
#   else if (pad + x + width > Globals::w - layout->buttonInsetX)
#       x = Globals::w - layout->buttonInsetX - (width + pad * 2);
# instrumented weights: CLANG=/tmp/llvm_android_702b/build2/bin/clang++ ... -c SpaceLounge.cpp
# raw-vreg knobs (raw = 0x80000000|index): pad = 2147483650, x = 2147484255
VRW=2147483650:2.0 XWFACTOR=0.7 CLANG=/tmp/llvm_android_702b/build2/bin/clang++ \
  OUT=/dev/shm/w7682/dl/SpaceLounge.o /dev/shm/w7682/dl/compile.sh
```
