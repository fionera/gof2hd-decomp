# w6794: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# turret-loop register-allocation attack; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6794`, branch `wave119-w6794`.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(function `_ZN5Level13createMissionEv` at `0xaffd8`-`0xb42ec`, 17172 B).
This continues `level-createmission-wave119-w6782.md` and
`level-createmission-wave119-w6773.md`. `src/game/world/Level.cpp` is unchanged;
this note is the only commit.

## Baseline (pristine HEAD)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. Corrected frame-slot census (the w6782 note is right, this confirms it)

The first differing instruction in the whole function is the prologue spill at
`0xb0024`:

```text
orig  b0024: str.w fp, [sp, #88]      ; fp = &__stack_chk_guard  -> guard at 0x58
ours  b0024: str.w fp, [sp, #84]      ;                           -> guard at 0x54
orig  b04c0: str   r0, [sp, #84]      ; raid = false (line 1573)  -> raid  at 0x54
ours  b04c0: str   r0, [sp, #88]      ;                           -> raid  at 0x58
```

So the two 4-byte RA spill slots `&__stack_chk_guard` and `raid` are swapped
(17 refs each; every other `[sp,#N]` in the function already agrees).  A
simulated swap is worth roughly +0.31 pp, but the order is decided by the
greedy allocator's spill-slot creation order and could not be moved from the
source: `bool raid;` -> `int`/`char`/`unsigned raid` all keep the same swap and
score 92.128713 (the type change perturbs one other instruction).

## 2. First *length* divergence is the convoy k-chain, not the turret preheader

Walking both disassemblies instruction by instruction (objdump encoding width):

```text
first text   divergence: insn 28   0xb0024  str.w fp,[sp,#88] vs [sp,#84]
first length divergence: insn 1900 0xb1424  orig 2 B `ldr r1,[pc,#408]`
                                            ours 4 B `str.w sl,[sp,#376]`
first address offset   : insn 1901 0xb1426  delta -2 (ours 2 B ahead)
```

`0xb1424` is the `k == 2 / k == 1 / k == 0` convoy offset chain (source lines
2193-2205): the original descends (`cmp #2`, `cmp #1`, `cbnz`), ours ascends
(`cmp #0`, `cmp #1`, `cmp #2`).  The running size delta oscillates and returns
to 0, so this is a layout difference, not a size difference — but it is the
first place the instruction stream stops lining up, which is why the earlier
"first divergence at 0xb144e" note was close and the handoff's "0xb324e" claim
is not the first.

## 3. Where the score is actually lost: everything after the turret preheader

Sequence-ratio split at the alien-turret preheader (`0xb322e`):

```text
before 0xb322e : orig 4540 insns / ours 4543   96.73 % match   (~148 mismatches)
after  0xb322e : orig 1515 insns / ours 1522   78.96 % match   (~323 mismatches)
```

The turret loop itself (`0xb322e`-`0xb3398`) matches at only **56.6 %**; the
pirate-fortress loop (`0xb3398`-`0xb3752`) at 81.9 %.  Fixing the turret loop's
register assignment is worth about +0.9 pp, the whole tail about +5.3 pp.

## 4. Root cause of the turret-loop split (VRR MIR evidence)

From `-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level13createMissionEv`
(`IR Dump After Virtual Register Rewriter`, `%666` is `this`, a `%stack.75`
reload):

```text
bb.249 (preheader)
  63896B  renamable $r4 = t2LDRi12 %stack.75, 0      ; this -> r4
  63904B  renamable $r0 = t2LDRi12 renamable $r4, 248
  64080B  %4851 = t2LDRi12 %666, 352
  64096B  %4850 = t2LDRi12 %666, 356
  64112B  %4849 = t2LDRi12 killed renamable $r4, 360 ; r4 dies here
bb.252 (loop body)
  64680B  renamable $r5 = t2LDRi12 %stack.75, 0      ; this reloaded into r5
```

Ours therefore keeps `this` in **r5** across the loop and hoists both
`&rotation` (`%3966` -> `sl`) and `&temp` (`%3082` -> `r8`) into callee-saved
registers.  The original keeps `this` in **r4** from the preheader into the
loop body, rematerialises `&rotation` (`add r4, sp, #312`) inside the body and
reloads `this` only at the loop back edge:

```text
orig b3232: ldr r4,[sp,#124]     ; this, hoisted above `if (alienGuns)`
orig b32da: add r4,sp,#312       ; &rotation kills r4
orig b336e: ldr r4,[sp,#124]     ; this reloaded for the loop bound
ours b3260: ldr r4,[sp,#124]     ; this, inside the branch
ours b3280: ldr r5,[sp,#124]     ; this again, kept across the whole body
```

That single split rotates the whole downstream allocation
(`gun` r6->r4, `&v` r5->r8, `k*24` r9->fp, `i` fp->r9, table base sl->r6,
`t[1]` r8->sp+104).  The source-level declaration order, `Vector v` spelling,
`k` scope, `this` caching and body statement order were all swept (table
below) without flipping it.

## 5. The `(i < 0)` table-select anchor is load-bearing

The source currently reads

```cpp
const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
```

The `(i < 0)` term looks like a padding anchor but is the only thing that keeps
the select inside the loop (the original has an in-loop `cbz` at `0xb32c8`).
Removing it — or anchoring on `k`, or making `i` unsigned — lets LICM hoist the
select and collapses the score:

```text
no anchor / anchor on k / `unsigned i`   88.380543 %  17140 B  6054 insns
anchor `(i > 0)`                         90.901592 %  17180 B  6068 insns
anchor `(i == -1)`                       88.643299 %  17180 B  6070 insns
anchor `(bigAlienWave | (i < 0))`        92.244224 %  17172 B  6065 insns
anchor `(bigAlienWave - (i >> 31))`      92.293729 %  17172 B  6065 insns  (identical code)
anchor `(i <= -1)` / `(i > -1 ? 0 : 1)`  92.293729 %  17172 B  6065 insns  (identical code)
volatile bigAlienWave                    87.740444 %  17140 B  6058 insns
```

`k` does not work because `k` starts at 0 and only increments, so `k < 0` is
provably false and the select becomes loop-invariant.

## 6. Variant table (all measured this wave, baseline 92.293729 / 17172 / 6065)

| variant | % | size | insns |
|---|---:|---:|---:|
| turret body `r3,scale,rot,r0,pos,group,cargo` (HEAD) | **92.293729** | 17172 | 6065 |
| `r3,rot,r0,pos,scale,group,cargo` (fresh scaling) | 92.244224 | 17172 | 6065 |
| `r3,rot,r0,pos,scale,cargo,group` (fresh scaling) | 92.227723 | 17172 | 6065 |
| `r3,rot,r0,pos,scale,group,cargo` (cached `gun`) | 92.176927 | 17172 | 6063 |
| `r3,rot,r0,pos,scale,cargo,group` (cached `gun`) | 92.160423 | 17172 | 6063 |
| HEAD with `scale` moved first (`swap12`) | 92.277228 | 17172 | 6065 |
| HEAD with `group`/`cargo` swapped (`swap56`/`swap67`) | 92.277228 | 17172 | 6065 |
| HEAD with `rot`/`scale` swapped (`swap23`) | 92.293729 | 17172 | 6065 |
| HEAD with `pos`/`scale` swapped (`swap45`) | 90.781547 | 17164 | 6062 |
| HEAD with `r0`/`pos` swapped (`swap34`) | 90.489153 | 17188 | 6068 |
| `hard` before `raidType` | 92.277228 | 17172 | 6065 |
| `Matrix rotation = Matrix(1,0,0,0, 0,1,0,0, 0,0,1,0)` | 92.293729 | 17172 | 6065 |
| `Vector v(0,0,0)` / `Vector v;` / `k` in for-init / `unsigned k` | 92.293729 | 17172 | 6065 |
| `int`/`char`/`unsigned raid` | 92.128713 | 17172 | 6065 |
| `Vector base` before `Vector v` | 92.128713 | 17172 | 6065 |
| `Vector base` before `Matrix rotation` | 92.062706 | 17172 | 6065 |
| `if ((i == X) & alienWave)` (line 1904) | 75.689058 | 17172 | 6063 |
| `if (!(i != X \| !alienWave))` (line 1904) | 82.981182 | 17168 | 6061 |
| `if (!(i != X \|\| !alienWave))` (line 1904) | 92.293729 | 17172 | 6065 (identical IR) |
| `if ((i == X) & pirateFortress)` (line 1924) | 63.045811 | 17312 | 6060 |
| `if (!(i != X \| !pirateFortress))` (line 1924) | 88.577088 | 17172 | 6061 |
| `(*enemies)[i]->setPosition(...)` (fresh load) | 90.901592 | 17180 | 6068 |
| no cached `gun` at all | 88.558938 | 17204 | 6068 |
| no cached `gun`, faithful order | 88.537028 | 17212 | 6071 |

The original's `it ne; movne r0,#1; eor r1,#1; orrs r0,r1; bne` shape on both
`i == X && flag` tests (lines 1904 and 1924) is a *branch-folding* choice, not a
source shape: spelling it as a bitwise `&`/`|` in the source makes the score
much worse because the `then` block stops being the fall-through.

## 7. Conclusion

Every source-level knob that could plausibly move the turret-loop live ranges
was tried; HEAD is a local optimum at 92.293729 %.  The remaining ~7.7 % is
greedy-allocator colour choice (turret loop `this` in r4 vs r5, `&rotation`
rematerialised vs hoisted) plus the `raid`/`&__stack_chk_guard` slot order,
neither of which is reachable from the C++ source without changing semantics.
