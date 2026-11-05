# w6768: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119:
# table-select / turret-order / `&&` / case-183 re-sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6768`, branch `wave119-w6768`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. Result

No source variant beat the baseline. Best measured score remains
**92.293729 % / 17172 B / 6065 insns**. `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

The difflib census is unchanged: 5593 of 6055 instructions match; the
largest non-equal blocks are 13 insns (alien-turret preheader), 12 (raid
loop bound), 10 (`&&` at 1904), 8 (case-183 default body), 7 (setScaling
receiver), 6 (pirateFortress loop bound). Everything else is 1-4 insn
register-allocation churn.

## 2. Alien-turret table select: the `(i < 0)` anchor is load-bearing

The original emits an in-loop **branch** (`b32c6: ldr r0,[sp,#64]; cbz;
ldr;add;b;ldr;add`).  The baseline source anchor `(bigAlienWave + (i < 0))
? kAlienStationTurrets : kAlienOutpostTurrets` makes the select
loop-variant, which keeps it in-loop and if-converts it
(`cmp.w r1,r9,asr #31; it ne; movne`).  Every spelling without the anchor
is hoisted out of the loop by LICM and shrinks the function by 32 B:

| table-select source | official % | size | insns |
|---|---:|---:|---:|
| **baseline anchor** | **92.293729** | 17172 | 6065 |
| plain ternary `bigAlienWave ? ...` | 88.380543 | 17140 | 6054 |
| `t = kAlienOutpostTurrets; if (bigAlienWave) t = kAlienStationTurrets;` | 88.380543 | 17140 | 6054 |
| `t = kAlienStationTurrets; if (!bigAlienWave) t = kAlienOutpostTurrets;` | 88.380543 | 17140 | 6054 |
| `if (bigAlienWave) ... else ...` | 88.380543 | 17140 | 6054 |
| `if (bigAlienWave != 0)` / `if (bigAlienWave == 0)` | 88.380543 | 17140 | 6054 |

No source form produced the original's real `cbz` branch while keeping the
select in-loop.

## 3. Alien-turret body: statement order (A=rotate1 B=setRotation
## C=rotate2 D=setPosition E=setScaling F=shipGroup G=cargo)

The original's disasm order is `A B C D E G F` with `setScaling` through the
cached `gun` (r6).  Our baseline is `A Ef B C D F G` (Ef = fresh
`(*enemies)[i]` receiver).  Re-measured this wave:

| order | official % | size | insns |
|---|---:|---:|---:|
| **baseline `A Ef B C D F G`** | **92.293729** | 17172 | 6065 |
| `A Ef B C D F G` with cargo before shipGroup | 92.277228 | 17172 | 6065 |
| `A B C D E F G` (faithful, fresh receiver) | 92.227723 | 17172 | 6065 |
| `A B C D E F G` (faithful, cached gun) | 92.160423 | 17172 | 6063 |
| `A B E C D F G` (fresh receiver) | 92.293729 | 17172 | 6065 |
| `A B E C D F G` (cached gun) | 90.906090 | 17164 | 6063 |
| `A E B C D F G` (cached gun) | 90.922594 | 17164 | 6063 |

The fresh-receiver hack is worth ~1.4 pp over the faithful cached-`gun`
form even though the original clearly uses `gun`; keeping it.

`PlayerTurret *gun` instead of `KIPlayer *gun` is score-identical
(92.293729 / 17172 / 6065).

## 4. `&&` at 1863 / 1904: clang 7 always short-circuits

The original materialises both operands and branches on the OR of the
negations, e.g. at `b2e04`:

```text
subs.w r0, fp, r3      ; i - X
mov    r1, r0
it ne
movne  r1, #1          ; r1 = (i != X)
ldr    r2, [sp, #60]   ; alienWave
eor.w  r2, r2, #1      ; !alienWave
orrs   r1, r2
bne.w  <else>          ; (i != X) | !alienWave
```

Ours emits two short-circuit branches (`cmp alienWave,#0; beq; cmp i,X;
bne`).  All spellings tried are score-identical unless noted:

| spelling (line 1904) | official % | size | insns |
|---|---:|---:|---:|
| baseline `i == X && alienWave` | **92.293729** | 17172 | 6065 |
| `i == X && alienWave != 0` | 92.293729 | 17172 | 6065 |
| `(i == X) & (alienWave != 0)` | 92.293729 | 17172 | 6065 |
| `alienWave && i == X` | 92.293729 | 17172 | 6065 |
| `!(i != X \|\| !alienWave)` | 92.293729 | 17172 | 6065 |
| `bool firstSlot = i == X; if (firstSlot && alienWave)` | 92.293729 | 17172 | 6065 |
| `i == X && alienWave == 1` | 92.062706 | 17172 | 6065 |
| `i == X ? alienWave : false` | 92.062706 | 17172 | 6065 |
| `(i == X) & alienWave` | 75.689058 | 17172 | 6063 |

Line 1863 `hasWanted && i == 0`:
`(i == 0) & hasWanted` and `hasWanted & (i == 0)` are score-identical
(92.293729); `i == 0 && hasWanted` (operands reversed) is 80.597508 /
17164 / 6062.  So the original's OR-of-negations is not reachable by any
`&&`/`&` spelling tried.

## 5. `if (alienGuns != 0)` spelling is irrelevant

`> 0`, `>= 1`, `0 != alienGuns`, `> 0u`, and plain `alienGuns` are all
92.293729 / 17172 / 6065 (byte-identical codegen).  The original's
`ldr r4,[sp,#124]` before the `cmp` (vs ours after) is a scheduler
decision, not a condition-shape decision.

## 6. Case-183 convoy default body: any shared tail vectorises

The original's four bodies all store `offset[1]; offset[0];` then fall
through to a shared `str [sp,#380]` (`offset[2]`).  Making the default body
`1,0,2` shares the tail but makes clang 7 merge the shared store into VFP:

```text
vmov.f32 s0, s16/s18/s20/s22     ; per body
vstr     s0, [sp, #380]          ; shared tail
```

| default body | official % | size | insns |
|---|---:|---:|---:|
| **baseline `1,2,0`** | **92.293729** | 17172 | 6065 |
| `1,0,2` | 89.653465 | 17188 | 6065 |
| `1,0,2` via `float oy/ox/oz` temporaries | 89.653465 | 17188 | 6065 |
| `1,0,2` with `+0.0f` / `-(-6000.0f)` | 89.653465 | 17188 | 6065 |
| `1,0,2` via `volatile float` temporaries | 89.717160 | 17196 | 6072 |

Any spelling that lets the default share the tail triggers the VFP merge;
none avoids it.

## 7. Convoy `cargo` / `crateCaptured` order

At `Level.cpp:2294` the original stores `cargo` (0x50) then
`crateCaptured` (0x4c); ours is the reverse.  Swapping the source lines is
score-identical (92.293729 / 17172 / 6065) — clang reorders either way.
Using fresh `(*enemies)[convoyEnd + j]` receivers is worse
(92.070303 / 17172 / 6064).

## 8. Declaration / induction-variable order

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `int k` before `Vector base` | 92.293729 | 17172 | 6065 |
| `int k` before `Vector v` | 92.293729 | 17172 | 6065 |
| `for (int i = ..., k = 0; ...)` | 92.293729 | 17172 | 6065 |
| `Vector base` declared first | 92.062706 | 17172 | 6065 |
| `Vector v` after `base` | 92.128713 | 17172 | 6065 |
| `k++` moved into the body | 88.685167 | 17156 | 6053 |

## 9. Confirmed root cause of the dominant residual (post-RA MIR)

`-mllvm -print-after-all`, Virtual Register Rewriter dump, turret block:

```text
; preheader (bb.249)
renamable $r4 = t2LDRi12 %stack.75, 0      ; this  (reload)
... $r4 used for getPosition and the loop-init loads ...
renamable $r9  = t2ADDrr ...               ; i
renamable $r11 = t2MOVi 0                  ; k*24

bb.250 (loop header):  liveins: $r0, $r1, $r2, $r6, $r9, $r11
                       ; 'this' (r4) is NOT live into the header

bb.252 (loop body):
renamable $r5 = t2LDRi12 %stack.75, 0      ; this  (second reload)
```

The greedy allocator splits `%666` (`this`) at the loop header, so the body
reloads into **r5** and `gun` takes **r4**.  The original keeps the
preheader's **r4** live across the header into the body and only reloads at
the body end (`b336e`), so `gun` gets **r6** and `&v` **r5**.  This single
split rotates the whole downstream allocation (`this` r4/r5, `gun` r6/r4,
`&v` r5/r8, `k*24` r9/fp, `i` fp/r9) and accounts for the raid-loop,
pirateFortress and later 1-4 insn churn.  The fresh `setScaling` receiver
extends our body live range past the second rotate (`this->enemies` at
`b32fe`), while the original's `setScaling` uses `gun`; but the faithful
cached-`gun` form measures lower (92.16 vs 92.29), so the hack stays.

No declaration order, statement order, type, or `&&` spelling tried this
wave moves the split point.

## 10. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6768/score.py                 # official %, size, insns
python3 /dev/shm/w6768/blocks.py                # difflib block census
python3 /dev/shm/w6768/showblock.py 4           # non-equal blocks
python3 /dev/shm/w6768/diffmap.py 4             # blocks -> source lines
python3 /dev/shm/w6768/sweep.py <variants.json> # apply/build/score sweep
cmake --build cmake-build-match --target check -j6
```

Scratch (not tracked): `/dev/shm/w6768/{score,blocks,showblock,diffmap,
sline,sweep}.py`, `Level.base.cpp` (== HEAD), `Level.s` (fresh `-S`),
`mir_isel.txt`, `ra_vrr.txt`, `ra_greedy.txt`, `pa.txt`, `rea.c`,
`orig.dis`, `ours_cur.dis`, `v_{t,ord,cg,and2,cargo,c183b,gtype,w,k,ag,
decl2,abecd}.json`.
