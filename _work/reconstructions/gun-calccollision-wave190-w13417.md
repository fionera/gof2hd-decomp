# `Gun::calcCharacterCollision` wave 190 (w13417): the residual is two 2-byte
# `add r4,sp,#104` remats (vs the shipped 4-byte `ldr.w fp,[sp,#40]` reloads) —
# no landed win

Worker: w13417. Branch `wave190-w13417`, base master `32785a58f`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13417`.

## 0. Result

**No `src/` change landed.** The committed body (wave-190 w13388/w13406) remains
the best measured shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.
Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `32785a58f`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on entry/exit unchanged: `check` green (byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.text` diff 72,943).

## 1. New evidence: the exact phase-shift root cause

The handoff (w13406) already knew the residual is the greedy-RA `i*3`/`&view`
r4/r5 swap.  This pass localises **where the 2-byte phase shift is born** and
shows it is *not* one shift but two, both 4B-vs-2B instruction-width choices:

```
orig 0x153196  ldr   r0, [sp,#36]        ; mine
orig 0x153198  ldr.w fp, [sp,#40]  4B    ; reload i  (sp+40)
ours 0x153198  add   r4, sp, #104  2B    ; remat &view (sp+104)
...
orig 0x153260  ldr   r0, [sp,#36]
orig 0x153262  ldr.w fp, [sp,#40]  4B    ; reload i
ours 0x153262  add   r4, sp, #104  2B    ; remat &view
```

`add r4,sp,#104` uses the 16-bit `ADD SP plus immediate` encoding (104 is a
multiple of 4 and < 256); `ldr.w fp,[sp,#40]` is a 32-bit load.  Each site is
therefore **2 bytes shorter in ours**, producing a cumulative `delta = -2`
(0x15319c..0x153266) and `-4` (0x153266..0x15327c) phase shift over ~192 bytes.
That shifted run is the single largest block of the 455 raw bytes.

`python3 ph.py orig.txt ours.txt` (scratch) prints the full delta map:

```
addr 0015319c/0015319a delta -2  ldr r6,[sp,#24]
addr 00153266/00153262 delta -4  ldr r6,[sp,#24]
addr 0015327c/0015327c delta +0  cmp r1,r0          <- compensated
addr 001533ca/001533c8 delta -2  mov r1,r0          <- view block
addr 001533d0/001533ce delta -2  blx memcpy
addr 001533f6/001533f6 delta +0  ldr r6,[sp,#24]
addr 00153442/0015344c delta +10 ldr r0,[r0,#4]     <- k-loop layout
addr 00153508/00153508 delta +0  ldr r2,[sp,#28]
```

So the raw metric decomposes as: ~192 B (two remat phase shifts) + ~146 B
(k-loop cold-block layout) + ~44 B (view block) + small branch-offset tails.
The aligned metric (93) is the "real" instruction delta.

### 1.1 Why the reload is not emitted

The shipped body reloads `i` at both sites because `fp` is free there (it held
`radius` only for the collision check) and the scheduler hoists the `i` load
ahead of the `mine` branch; `&targetDir` is hoisted alongside it.  In ours the
scheduler hoists the *cheaper* `&view` remat instead, so `i` stays in its spill
slot until 0x1531fa.  The two instructions are mutually exclusive at that slot.

### 1.2 Attempts to force a 4-byte instruction at 0x153198 (all failed)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| committed | **455** | **93** | 1984 | |
| `(void) this->lifetimes[i];` in asteroid block | 455 | 93 | 1984 | folded away |
| `if (mine && i < positions.count) continue;` | 1637 | 418 | 1980 | |
| `if (i == 0xFFFFFFFFu) continue;` | 455 | 93 | 1984 | folded (i<count) |
| `char viewbuf[60]; Matrix& view=*(Matrix*)viewbuf;` | 1130 | 131 | 1980 | |
| `char viewbuf[60]` + `__builtin_memcpy` | 455 | 93 | 1984 | identical |
| `viewbuf+1` / `viewbuf+2` (odd-aligned view) | 1861 | 387 | 1896 | |
| `Matrix *views[2]={&view,&view};` | 1726 | 178 | 2008 | frame 216 |
| `Matrix *views[2]` indexed by `i&1` | 1138 | 150 | 2004 | |
| `(e&1)?view:view` | 455 | 93 | 1984 | folded |
| `unsigned gi = geometries[i];` hoisted to body top | 1660 | 248 | 1976 | |

Making `&view` non-rematerialisable costs either a frame slot (+60 B, w13406's
`views[2]`) or an extra address add (+8 B), both of which shift every sp-relative
access and dominate the raw metric.

## 2. The size anchor is uniquely `expect(diff>0,0)`

A fresh sweep of **23 conditions × {0,1}** with the diff condition made plain
(`if (diff > 0)`) found **no** variant at 1984 B: every one lands 1880..1980.
A follow-up sweep of **all 676 pairs** from 13 conditions × {0,1}² with the diff
condition plain also found **zero** size-1984 variants.
Conversely, keeping `expect(diff>0,0)` and adding a second expect on any of the
23 conditions never moves `i*3` from r5 to r4 at 1984 B (the only r4 hits,
`isAster0`=1980 and `geometries1`=1956, are not size-exact).  So the 76-byte
size anchor and the `this=r9` allocation are both consequences of that single
`expect(diff>0,0)` cold block; they cannot be decoupled by moving the expect.

Notable cells:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| `expect(diff>0,1)` (== no expect) | 1887 | 281 | 1908 |
| `if (expect(diff<=0,1)) continue;` | 455 | 93 | 1984 |
| `if (!expect(diff>0,0)) continue;` | 457 | 84 | 1984 |
| diff-inside-if, `expect(magnitude>distance,0)` | 460 | 89 | 1984 |
| diff-inside-if + faithful C-D-E order | 463 | **57** | 1984 |
| `expect(isAster,0)` (i*3 = r4!) | 1813 | 376 | **1980** |
| `expect(isAster,0)` + `expect(empDamage>0,0)` | 1607 | 375 | 1984 |
| `expect(empDamage>0,0)` alone | 928 | 107 | 1984 |
| `expect(owner...,1)` | 533 | 112 | 1984 |
| `expect(impact,1)` / `expect(playerGun,1)` | 455 | 93 | 1984 |

The `expect(isAster,0)` result is the closest to the shipped allocation
(`i*3 -> r4`) but it moves `this` to `fp` and loses 4 B.

## 3. Literal-pool ordering is already exact

The whole tail from `0x1536d0` (asteroid/achievement block, literal pool
`0x1536dc..0x1536e6`, and the second achievement block) is **byte-identical**
between ours and the original, and the 48-instruction prologue
(`0x152f90..0x153012`, including the `s16/s18/s20/s22/s24/s26/s28` constant
materialisation order) is also byte-identical.  So neither the literal pool nor
the prologue is a lever for this function.

## 4. Other cells measured this pass (all size-exact unless stated)

All `link_so` + `tools/fn_diff.py`, `-j2`, `ulimit -v 12000000`.

- k-loop order matrix (diff out/in × ECD/CDE/CED/DCE/DEC/EDC): best still
  `out ECD` 455; `in CDE` 463/57.
- `expect` with 23 conditions × {0,1} added on top of the committed diff expect:
  `emp0/1` 928, `fall0/1` 455, `impact0` 1992, `impact1` 455,
  `isAster0` 1980 (i3=r4), `isAster1` 1968, `explode0` 455, `explode1` 1980,
  `isActive0` 455, `isActive1` 1980, `hp0` 1880, `hp1` 455, `owner0/1` 455,
  `playerGun0/1` 455, `ego0` 1956, `ego1` 1928, `geometries0` 1972,
  `geometries1` 1956, `isRocket0` 1988, `isRocket1` 455, `enemies0` 455,
  `enemies1` 1932.
- `expect` forms: `!!(diff>0)`, `(diff>0)!=0`, `0<diff`, `diff>=1`,
  `0-diff<0`, `(long)diff>0` all codegen-identical (455); `(unsigned)diff>0u`
  1047/1980.
- `int i` (cast or not) 455; `volatile unsigned i` 513.
- collision-check reorder y-z-x / z-x-y 455/102; `nr=(float)-radius` 1796/1968.
- `r` inline / `td` direct / `outer` inline / `ki` cache 455.
- `mine` check moved after `setBombForce` 456/101.
- view block before emp/owner block 1536 / 1298; `Matrix* views[2]` 1726.
- `unsigned gi = geometries[i]` hoisted 1660/1976.
- `lifetimes[i]`/`setHitVector` reorders 484/637/1333.
- k-loop condition `expect(k<size,1)` 1798/1948, `{0}` 1710/1944.

## 5. Conclusion

The residual is a genuine greedy-RA plateau with a now-exact decomposition:
two 2-byte `ADD SP` remats (192 B) + the k-loop cold-block layout (146 B) +
the view block (44 B).  Fixing it requires the shipped `i` spill/reload at
0x153198/0x153262, which in turn requires `&view` to be non-rematerialisable
without growing the 208-byte frame — every frame-neutral form tried either
folds away or costs >= 8 B and shifts the whole body.  The committed
**455 / 93 / 1984** body remains the best measured shape.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13417
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 7. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13417.md` only.
