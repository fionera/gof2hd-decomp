# `Gun::calcCharacterCollision` wave 190 (w13449): the 226-byte region is a
# pure phase shift born in pre-RA `StackColoring` — no landed win

Worker: w13449. Branch `wave190-w13449`, base master `8d8f43bcd`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13449`.

## 0. Result

**No `src/` change landed.** The committed body (wave-190 w13388/w13406) remains
the best measured shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.
Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `8d8f43bcd`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on entry/exit: `check` green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370).

## 1. New evidence: the root cause is the pre-RA `StackColoring` merge

The previous notes localised the residual to "greedy RA materialises the
`&view` remat early". This pass found the *pass* that creates the shared value
and the exact frame objects involved, from `-mllvm -print-after-all`
(scratch `all.log`, 73 MB):

**Before `Merge disjoint stack slots` (`# *** IR Dump After Slot index
numbering ***`, all.log:819414) the frame has three separate objects:**

```
  fi#10: size=60, align=8, at location [SP]   ; Matrix view
  fi#11: size=60, align=4, at location [SP]   ; enemyPosition
  fi#12: size=12, align=4, at location [SP]   ; k-loop getPosition() sret temp
```

and two separate lifetime regions:

```
  14256B  LIFETIME_START %stack.10, debug-location !7545   ; view  (line 710)
  14992B  LIFETIME_END   %stack.10, debug-location !7560
  15968B  LIFETIME_START %stack.12, debug-location !7602   ; k-loop temp
  16176B  LIFETIME_END   %stack.12, debug-location !7605
```

**`# *** IR Dump After Merge disjoint stack slots ***` (all.log:821040) merges
fi#12 into fi#10**: the k-loop body's `t2ADDri` becomes `%308 = t2ADDri
%stack.10` (bb.81), the same frame index as the `view` block's `%275`.

**`# *** IR Dump After Early Machine Loop Invariant Code Motion ***`
(all.log:825831) then hoists `%308` from the k-loop body (bb.81) all the way
out to the e-loop preheader (bb.5)**, where it joins the other hoisted
rematerialisable constants (`%108 = t2ADDri %stack.3`, `%152 = t2ADDri
%stack.1`, `FCONSTS 8/80/96/112/120`, `t2MOVi16 20001/10000`, …). The
`view` block's own `%275` is CSE'd away at the same time.

`IsProfitableToHoist()` in LLVM 7 (`MachineLICM.cpp:1216`) is unconditional
for rematerialisable instructions — *"Rematerializable instructions should
always be hoisted since the register allocator can just pull them down again
when needed"* — so a merged frame-index `t2ADDri` can never be kept in the
loop by source shape alone.

Greedy RA then splits that long live range at the two blocks the value is
live through but not used in, and "pulls it down" there:

```
ours 0x153198  add   r4, sp, #104      ; 2B  dead remat (bb.43, asteroid check)
orig 0x153198  ldr.w fp, [sp, #40]     ; 4B  reload i  (sp+40)
...
ours 0x153260  add   r4, sp, #104      ; 2B  dead remat (bb.55, mine check)
orig 0x153262  ldr.w fp, [sp, #40]     ; 4B  reload i
```

Each site is 2 bytes shorter in ours, producing the cumulative `-2`
(0x15319c..0x153266) and `-4` (0x153266..0x15327c) phase shift that accounts
for ~226 of the 455 raw bytes.

## 2. The 226-byte region is a pure phase shift

`python3` byte compare of `0x15319a..0x153280` at shift 0/2/4:

| shift | matching bytes | differing bytes |
|---:|---:|---:|
| 0 | 2 | 33 |
| **2** | **58** | **13** |
| 4 | 5 | 32 |

At shift 2 the only real mismatches are the second remat site and the branch
displacements (which the assembler would recompute if the code were actually
2 bytes longer). So repairing the two `add r4,sp,#104` sites would collapse
the region from 226 raw bytes to roughly 10.

## 3. Experiments (all `link_so` + `tools/fn_diff.py`, `-j2`, `ulimit -v 12000000`)

### 3.1 Forcing the k-loop temp into its own stack object

Declaring the k-loop sret temp as a named `Vector` whose lifetime starts
before the `view` block does split the objects (the dead `add r4,sp,#104`
remats at the asteroid/mine checks disappear) — but it also grows the
208-byte frame to 224 and re-allocates the whole body (666 vs 660 insns), so
the raw metric collapses:

| variant | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| committed | **455** | **93** | 1984 | 208 |
| `Vector kpos;` at i-loop body top, `kpos = getPosition(); *td = kpos;` | 1742 | 348 | 1976 | 224 |
| `Vector kpos;` at function scope, same use | 1827 | 316 | 1972 | — |
| `enemyPosition = getPosition(); *td = enemyPosition;` (reuse fi#11) | 1739 | 202 | 1992 | — |
| `Vector kpos;` in k-loop body (lifetime inside the loop) | 455 | 93 | 1984 | 208 |
| `Vector kpos = getPosition(); *td = kpos;` (copy-elided) | 455 | 93 | 1984 | 208 |
| `Vector kpos; kpos = getPosition(); *td = kpos;` | 1768 | 359 | 1972 | — |
| `*td = kpos = getPosition();` | 1641 | 354 | 1976 | — |
| `Vector *kp = (char*)&kpos + (i & 0x80000000u); *td = *kp = getPosition();` | 1646 | 299 | 1976 | — |
| `Vector playerPosition;` at i-loop top (moves the first `%stack.10` use past bb.55) | 1560 | 326 | 1964 | 224 |

Every frame-neutral form either folds back to the committed `%stack.10`
(codegen-identical, 455) or grows the frame by >= 12 B and shifts every
sp-relative access.

### 3.2 Other cells measured this pass

All size-exact unless stated; none beats 455.

- `alignas(4) Matrix view` **455**; `alignas(16) Matrix view` 1859/1996.
- `Matrix view(*(Matrix*)…)` copy-ctor form **455**.
- `{ Matrix view = …; … }` extra brace scope **455**.
- `const Vector &playerPosition = …getPosition()` **455**;
  `Vector playerPosition(…getPosition())` **455**.
- `-mllvm -no-stack-coloring` 1870/341/**1960** (frame 304);
  `-mllvm -no-stack-coloring=false` 1859/245/**1996**.
- `-mllvm -disable-machine-licm` (both MachineLICM passes) 1833/**1928** —
  confirms the hoist is load-bearing for the current 1984 B size anchor.

## 4. Conclusion

The remaining 455 bytes are one greedy-RA decision (`i*3 -> r5`,
hoisted-`&%stack.10 -> r4`) whose root cause is now pinned to a named pass:
the pre-RA `StackColoring` merge of the 12-byte k-loop `getPosition()` sret
temp into the 60-byte `Matrix view` slot. Undoing the merge requires a
distinct stack object, and every frame-neutral way of asking for one either
folds away or grows the 208-byte frame and shifts the whole body.

Two levers remain, both now fully characterised:

1. a source shape in which clang's frontend reuses the *view* alloca for the
   k-loop sret temp (so `StackColoring` has nothing to merge) **without**
   extending the view's lifetime past the `impact` block — not found;
2. a 2-byte longer encoding at `0x153198`/`0x153260` (e.g. a high-register
   remat, which is `add.w`, 4 B) — the greedy allocator picks `r4` because
   nothing else is live there.

The committed **455 / 93 / 1984** body remains the best measured shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13449
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13449.md` only.
