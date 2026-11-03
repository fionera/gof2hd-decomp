# `Gun::calcCharacterCollision` pass 6: the `$r4` copy hint on `this`, and the Ghidra-faithful `enemy` reuse (w4211)

Worker: w4211. Branch: `wave92-w4211`, base master `b379d00d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4211/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `b379d00d`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change. This pass adds three pieces of evidence that were not in the
wave-90/91 notes:

1. the *clang* machine-IR variable → vreg → physreg map for the faithful body
   (not the llc trace), and
2. a **priority-override experiment on the real LLVM-7 greedy allocator** which
   shows `this` is pinned to `r4` by a `$r4` copy hint, not by its priority:
   even forcing `this` to the lowest priority, or below four other ranges, it
   still evicts whoever holds `r4` and takes it.
3. the Ghidra-faithful **`enemy` reuse in the scatter loop** (HEAD uses a
   separate `Player *other`), which is a real source-shape difference and worth
   +7.0 oracle points on the faithful body, but still does not beat HEAD.

## 1. Ground truth recap (unchanged)

Shipped prologue / stack map (`0x152f90`, frame 208):

```
152f9c  sub  sp, #208
152f9e  mov  r9, r0                 ; this = r9
152fa6  str  r0, [sp, #12]          ; canary ptr
152faa  str  r0, [sp, #204]         ; canary
152fd4  str  r1, [sp, #32]          ; scatter
152fe4  str  r2, [sp, #20]          ; explosive
153000  mov  r6, r9
153006  str.w r2, [r6], #216        ; lastHitKIPlayer = 0; r6 = this+0xd8
15300c  str  r1, [sp, #16]          ; explodeOnContact
15300e  str  r5, [sp, #36]          ; mine
153010  str  r6, [sp, #24]          ; *** td spill ***
153018  str  r2, [sp, #28]          ; e
153074  str.w fp, [sp, #40]         ; i
```

Shipped register map: `this=r9`, `enemy=r8` (outer and scatter `other` are the
same value), `i*3=r4`, `i=fp` (+spill `[sp,#40]`), `mine=r5`, `&enemy->radius`
then `&position` in `sl`, `td` spilled `[sp,#24]` and reloaded into `r6` at
`15319c`, `153266`, `1533f6`.

## 2. New evidence A — clang machine IR for the faithful body

`src/game/weapons/Gun.cpp` with the faithful body (`Vector *td =
&this->targetDir;`, no `itemIndex` local, no volatile slot, no `hitFlag`),
compiled with the project command plus `-mllvm -print-after-all`:

```
16B    renamable $r4 = COPY $r0          ; this -> r4
1344B  renamable $r9 = COPY renamable $r4 ; td = this+0xd8 -> r9
...
DBG_VALUE $r4,  !"this"
DBG_VALUE $r9,  !"td"
DBG_VALUE $r11, !"enemy"
DBG_VALUE $r10, !"i"        (also $r2 in one block)
DBG_VALUE $r6,  !"mine"
```

Frame objects: `fi#0` = canary slot, `fi#10/fi#11` = the two 60-byte Matrix
temporaries, `fi#13..fi#21` = the spilled scalars. The clang allocation is
`this=r4, td=r9, enemy=r11, i=r10, mine=r6, i*3=fi#20` — exactly the w4010
table, now confirmed from the real clang pipeline rather than an llc replay.

## 3. New evidence B — the greedy-allocator enqueue order (faithful body)

`/home/debian/llvm7build/build/bin/llc -O2 -debug-only=regalloc
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` on the faithful IR
(the IR has a `variables:` field LLVM 7 does not parse; strip it with
`sed -E 's/, variables: ![0-9]+//'` first). The ENQUEUE lines for this function
(`vreg = reg - 2147483648`):

```
prio=3758112600 vreg=55  size=16216   this    hinted global
prio=3758108174 vreg=11  size=11790   td      hinted global
prio=3758108094 vreg=107 size=11710           hinted global
prio=3758108078 vreg=150 size=11694           hinted global
prio=3758107950 vreg=304 size=11566           hinted global
prio=3758107854 vreg=278 size=11470           hinted global
prio=3758107552 vreg=15  size=11168   enemy   hinted global
prio=3758106702 vreg=16  size=10318           hinted global
prio=3758097600 vreg=45  size=1216            hinted global
prio=3758096800 vreg=28  size=416             hinted global
--- non-hinted globals (bit 30 clear) ---
prio=2684371776 vreg=56  size=17216
prio=2684367196 vreg=6   size=12636
prio=2684367100 vreg=7   size=12540
prio=2684366366 vreg=14  size=11806   e
prio=2684366318 vreg=83  size=11758
prio=2684364976 vreg=17  size=10416
prio=2684364862 vreg=18  size=10302   i
prio=2684364402 vreg=99  size=9842    i*3
```

`Prio = (1<<29)+Size`, `|= (1<<31)` for a global range, `|= (1<<30)` when
`VRM->hasKnownPreference(Reg)` (LLVM 7 `RegAllocGreedy::enqueue`). `this` and
`td` both carry the hint bit, so `this` is first purely because it is bigger;
`i*3` (9842) is far down the queue and is the range that ends up on the stack
(`fi#20`).

## 4. New evidence C — lowering `this`'s priority does not move it off `r4`

The instrumented LLVM 7 tree (`/home/debian/llvm7build/llvm-7.0.0.src`) was
extended with two env hooks in `RAGreedy::enqueue`
(`FORCE_GUN_REG`/`FORCE_GUN_PRIO`, `NO_HINT_GUN`), rebuilt in ~5 s. Results on
the *faithful* IR:

| forced priority of `this` (%55) | `this` | `td` | `enemy` | `i` | `mine` | `i*3` |
|---|---|---|---|---|---|---|
| none (natural 3758112600) | **r4** | r9 | r11 | r10 | r6 | fi#20 |
| 2684370776 (= non-hinted value) | **r4** | r11 | fi | r10 | r5 | fi#20 |
| 0 (lowest) | **r4** | r8 | fi#21 | r11 | r5 | fi#19 |
| 3758107900 (below %304) | **r4** | r9 | r11 | r10 | r5 | fi#20 |
| 3758107800 | **r4** | r10 | r11 | r9 | r5 | fi#20 |
| force `td` to 4000000000 (highest) | r5 | r9 | r11 | r10 | r6 | fi#20 |
| force `i*3` (%99) to 4000000000 | **r10** | r11 | fi#18 | r9 | r4 | r6 |

The trace explains it: `this` carries the copy hints `$r0 $r0 $r0 $r4`, so when
it is finally selected it prefers `r4` and *evicts* the current holder:

```
selectOrSplit GPRnopc:%55 ...
hints: $r0 $r0 $r0 $r4
missed hint $r0
$r9 is available at cost 1
should evict: %11 ...            ; td
evicting $r4 interference: Cascade 2
unassigning %11 from $r4: R4
assigning %55 to $r4: R4 ...
```

So the residual is **not** "`this` is the biggest range" (the w4010 model). It
is that `this` has a hard `$r4` copy hint and will take `r4` from any spillable
range. The last row is the proof of the mechanism: promoting `i*3` above
`this` moves `this` all the way to `r10` (`mov r10, r0`) and puts `i*3` in a
callee-saved register. In the shipped binary `r4` is held by `i*3`, i.e. `i*3`
must be register-resident *and* allocated before `this` (or be unspillable),
and `td` must be the value that loses the last callee-saved register.

## 5. New evidence D — the Ghidra-faithful `enemy` reuse in the scatter loop

The w3909 note says "`this_01` (enemy) is reused as the scatter-loop `other`",
but the tree's HEAD body still declares `Player *other = (*this->enemies)[k];`.
The shipped scatter block really does overwrite `r8`:

```
153442  ldr  r0, [r0, #4]
153446  ldr.w r8, [r0, r5, lsl #2]   ; enemy/other = enemies[k]  (r8 = enemy!)
15344a  mov  r0, r4
15344c  mov  r1, r8
15344e  blx  Player::getPosition()
```

Replacing `other` with `enemy` in the faithful body is a real IR change: it
stops LICM from hoisting `&enemy->transform` / `&enemy->radius` out of the inner
loop (the faithful body emits `add.w r0,fp,#64; str r0,[sp,#36]` etc. before the
inner loop; the reuse variant computes them inside). Measurements (object-level
oracle, `autojunk=False`; HEAD oracle = 82.4):

| variant | pct | size | frame | `this` | `[sp,#24]` |
|---|---:|---:|---:|---|---|
| HEAD (3 anchors) | **82.4** | 1984 | 216 | r9 | r2 (`e`) |
| faithful (no anchors) | 57.3 | 1892 | 208 | r4 | — |
| faithful + `td` before null check | 58.3 | 1892 | 208 | r4 | — |
| faithful + `enemy` reuse in scatter | 64.3 | 1920 | 208 | r4 | r2 (`e`) |
| faithful + `enemy` reuse + docked `__builtin_expect` | 63.7 | 1924 | 208 | r4 | r2 (`e`) |
| faithful + `enemy` reuse + volatile `enemySlot` | 74.9 | 1968 | 216 | r9 | r2 (`e`) |
| HEAD + `enemy` reuse | 82.4 | 1984 | 216 | r9 | r2 (`e`) |

So the reuse is worth +7.0 on the faithful body and is the correct source shape,
but it does not dislodge `this` from `r4` and does not beat HEAD. Note also that
the original's `enemy` is `r8` while the reuse variant puts it in `r6` (or
`r11`), so the reuse alone is not the missing lever either.

## 6. Conclusion / remaining work

The faithful source shape is confirmed (local `Vector *td`, no `itemIndex`
local, `enemy` reused in the scatter loop). The 17.3% residual is a single
greedy-RA outcome, but the mechanism is now known to be the **`$r4` copy hint on
`this`**, not its live-range size: `this` evicts `td` (or whoever holds `r4`)
and takes `r4` even when forced to the lowest priority. To reproduce the shipped
`this=r9` / `td=[sp,#24]` / `i*3=r4` triple, the source/IR must make `i*3`
outrank `this` and be unspillable at `r4`, while `td` is the value that loses
the last callee-saved register. Every natural source-shape variant tried so far
(w3805/w3829/w3909/w4010/w4110 plus this pass's 184 combinations, including the
`enemy`-reuse family) either keeps `this=r4` or keeps `td` in a register.

## 7. Files changed

None (note only). Gate: byte 4498 / linked 4498, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`, lint CLEAN, drift 0.
