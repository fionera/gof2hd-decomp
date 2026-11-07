# SpaceLounge::drawLounge — wave-125 EH / branch-weight / join-shape re-check (w7636)

Status: research note (2026-11-07). Worktree `wt/w7636`, base `4bbe88776`.
**No `src/` change landed**: the committed HEAD (`short pad3`, raw
**95.676047 %**, 1948 B) is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 728/729 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave124-w7613.md` (MIR priority/hint),
`-wave122-w7217.md` (pad weight), `-wave121-w7117.md` (RA order),
`-w5016.md` (branch weights), `-w103-oracle.md` (oracle), `-ra-split-w3838.md`.
This note supersedes nothing.

## 1. Residual (re-verified, unchanged)

Only the clamp / join `0x1712b2..0x171330` differs. Original vs HEAD:

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

Original clamp/join:

```text
1712b2 vmov r5, s18
1712bc sub.w r1, r5, r9          ; x - pad
1712c4 add.w r5, r0, r9          ; x = inset + pad
1712d4 add.w r1, r9, r5          ; pad + x
1712d8 add r1, r6                ; + width
1712dc itt gt
1712de addgt.w r1, r6, sl        ; width + pad2
1712e2 subgt r5, r0, r1          ; x = right - (width + pad2)
1712f4 vmov r2, s16
1712f8 mov r3, r5                ; x copy #1
1712fa add sl, r6                ; boxW
1712fc mov r1, r9                ; pad
1712fe subs r6, r3, r1           ; x - pad
171300 str.w r8, [sp, #12]       ; spill agent
171304 sub.w r5, r2, r9          ; top - pad
171308 mov r9, r3                ; x copy #2
17130a mov r8, r2
17130c strd sl, fp, [sp]
```

HEAD instead: `ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,r1,asr#15;
subgt.w r9,r0,r1`, `str.w r8,[sp,#12]` before the String ctor, `mov r5,fp`,
and no `mov r3,r5`/`mov r9,r3`.

## 2. EH landing-pad / LSDA re-check (new this wave)

The task hint suggested the EH landing-pad order. Decoded the per-function
`.ARM.extab` entry (both exidx entries resolve to `@0x1c7138`, same size
`0xa8`):

```text
va 0x1c7138   original   ours
18:           cc12       ce12     (+2)
30:           d201       d401     (+2)
3c:           d20e       d40e     (+2)
4c:           d60e       cc0e
64:           d608       d808     (+2)
68:           0bd4       0bd8     (+4)
70:           d62c       d826
74:           0ca6       0ca4
78:           0eca22     0eca24
88:           d612       d812
```

Every difference is a 2/4-byte address shift of a call-site range or landing
pad, not a different cleanup order or a different number of call sites. The
landing-pad *sequence* is identical (`~String(sp+80) -> ~String(sp+68) ->
~String(sp+56) -> ~String(sp+32) -> ~String(sp+44) -> ~String(sp+92) ->
~String(sp+104) -> _Unwind_Resume`); the extra `b.n` at 171484/17148e in ours
is the same blocks re-relaxed around the 2-byte-shorter join. **EH is not a
lever**: it is downstream of the clamp/join size.

## 3. Branch weights re-check (new this wave)

The build's per-file `-mllvm -likely-branch-weight=7 -mllvm
-unlikely-branch-weight=1` (CMakeLists.txt:94) was re-swept on the *faithful*
`pad2` form over 7:1, 2000:1, 20:12, 3:1, 1:1, 2:1, 4:1, 8:1, 16:1, 31:1,
100:1: every ratio gives `pad=fp pad2=sl x=r9 lh=fp` (729 insns, 1948 B,
~94.1). The single `__builtin_expect(agent->isKnown(), 1)` is a call-result
branch (IR `!prof 2000:1`), so the per-file 7:1 flag does not even reach it.
**Branch weights are not a lever.**

## 4. Exact MIR priority arithmetic (instrumented LLVM 7.0.2)

`RATRACE`/`FINALDBG` for the committed HEAD:

```text
ENQ %3  (x)     size=4288  prio=0xE0000000+4288  hint=$r3   (hinted)
ENQ %46 (top-pad) size=736 prio=0xE0000000+736   hint=$r3   (hinted)
ENQ %47 (boxW)  size=768   prio=0xE0000000+768   hint=$r3   (hinted)
ENQ %2  (pad)   size=6384  prio=0xA0000000+6384  no hint    (unhinted)
```

Allocation order (`selectOrSplit`):

```text
%3  x       -> $r5
%47 boxW    -> $r6
%46 top-pad -> $r5   (evicts x)
%3  x       -> $r9   (whole; r9 free)
%2  pad     -> $r11  (fp; evicts Canvas)
```

The target needs `pad -> r9` **between** `x -> r5` and `%46 -> r5`. With the
greedy priority `Prio = (1<<31)|(1<<29)+Size` (+`1<<30` if hinted), that
requires

```text
0xE0000000+736  <  Prio(pad)  <  0xE0000000+4288
```

i.e. pad must be **hinted with an interval size in (736, 4288)**. pad's
interval is `[496,6064B)[6752,7568)` = 6384, so it can only satisfy the upper
bound if its live range is shortened by ~2100 slots or x's interval is
lengthened past 6384. Neither is reachable from the source: pad is loaded in
the entry block (`ldr.w r9,[r6,#148]` at 170ebe) and its last use is the join
(`sub.w r5,r2,r9` at 171304), and x's `vmov r5,s18` is already sunk to the
clamp (`%386:spr = VTOSIZS` at MBB2, `%607 = COPY %386` at MBB39). The faithful form's
`FINALDBG` intervals are essentially identical (pad `[496,6064B)[6752,7552r)`
= 6368, x `[6784,11056r)` = 4220), so the same bound applies.

`COPYHINTDBG` confirms the only physical hint on x is `$r3 = COPY %607` at
the first `DrawString` (hint=69); pad has no COPY use anywhere
(`%2:rgpr = t2LDRi12 ...`).

## 5. Shapes measured this wave (~430 variants, all <= 95.676)

Harness: `/dev/shm/w7636/harness.py` (single-TU NDK r18b compile, exact
`.text._ZN11SpaceLounge10drawLoungeEv` bytes, `verify.disassemble` + call
normalisation, `autojunk=False`).

| variant | raw % | size | pad | x |
|---|---:|---:|---|---|
| **HEAD (`short pad3`)** | **95.6760** | **1948** | fp | r9 |
| faithful `pad2` (top/right/boxW) | 94.1015 | 1948 | fp | r9 |
| faithful + `right` local | 94.1015 | 1948 | fp | r9 |
| faithful + `inset` local | 94.1015 | 1948 | fp | r9 |
| faithful + boxX/boxY/boxW locals (both calls) | 94.1015 | 1948 | fp | r9 |
| inline `clampPlateX(...)` helper (faithful) | 95.6104 | 1948 | fp | r9 |
| lambda-inlined faithful clamp | 95.6104 | 1948 | fp | r9 |
| faithful + `boxY` local (both calls) | 92.4451 | 1944 | **r5** | r9 |
| HEAD hack + `boxY` local (both calls) | 93.2785 | 1948 | **r5** | r9 |
| `pad2` late / `top` late / `centerX` late | 71.4-94.1 | 1948-1976 | fp/r4 | r9/r6 |
| `pad3` forms (`(short)pad`, `&0xffff`, `ushort`) | 91.9-95.7 | 1948-1956 | fp | r9 |
| `top` via `pad3`/`(short)pad` | 88.7 | 1948 | fp | r9 |

The `boxY` local is the only new shape that moves pad out of `fp`, but it
moves it to `r5` (first free callee-saved) and shortens the body to 1944 B;
it does not reach `pad=r9, x=r5`. No variant reached the target colouring.

## 6. Conclusion

1. The EH landing-pad order and the branch weights are both downstream /
   neutral: the LSDA is structurally identical (2/4-byte address shifts only)
   and every branch-weight ratio gives the same `pad=fp, x=r9` allocation.
2. The residual is the single greedy-RA fact already pinned in wave-124:
   pad is an unhinted `0xA` interval of size 6384, x is a `$r3`-hinted `0xE`
   interval of size 4288, so x is selected first (`r5`), `%46` evicts it, and
   x re-selects whole into `r9` before pad is considered.
3. To land pad in `r9` the source would have to produce a pad interval of
   size 737..4287 (and hinted) or an x interval > 6384; neither is reachable
   from the shipped load/use positions. The committed HEAD
   (`short pad3`, **95.67604667124228 %**, 1948 B) remains the best shape.

## 7. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score
python3 /dev/shm/w7636/harness.py
# MIR weights / hints / live ranges
MIDBG=1 RA_TRACE=1 CLANG=/tmp/llvm_android_702b/build2/bin/clang++ \
  /dev/shm/w7636/compile.sh
# LSDA decode
python3 - <<'PY'  # see section 2
PY
```
