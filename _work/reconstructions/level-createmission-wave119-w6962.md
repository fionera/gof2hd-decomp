# w6962: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-119
# nameLeader block-order / tail-latch / turret-anchor / && sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6962`, branch `wave119-w6962`,
based on current master (`438df77e7`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave119-w6946.md` (Ghidra re-decompile + anchor
sweep), `…-w6925.md` (stack-slot / tail / convoy / turret sweep) and
`…-w6918.md` (negated `bigShipsCount`).  `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26298 (exact this baseline)
```

The fuzzy ratio is `2*M/(6055+6065)`; one extra matching instruction is
`+0.0165 pp`, i.e. 92.3267 -> 92.3432.

## 1. The `nameLeader` block-order problem (new evidence)

This is the clearest structural residual.  The original isEmpty-branch tail is
(offsets relative to `0xaffd8`):

```text
0x40b2  ldr  r0,[sp,#28]        ; nameLeader
0x40b4  cmp  r0,#0
0x40b6  beq.w <epilogue>        ; if (!nameLeader) return
0x40ba  ... getText(1663) ...   ; name body
0x40da  b.w  <epilogue>         ; return
0x40de  <case4Else>             ; stationPosition -> new Route(coords,3)
0x411a  <case4Tail>             ; hunter tail
```

So the original has **two** branches to the epilogue (a `beq` for the false
path and a `b.w` after the name body) and `case4Else` is laid out *after* the
nameLeader block.  The current source reproduces the layout with a tuned
`else { return; }`:

```cpp
    if (nameLeader) {
        (*this->enemies)[0]->name = *Globals::gameText->getText(1663);
    } else {
        return;
    }
    }              // closes the inner `{` opened right after `if (mission->isEmpty()) {`
case4Else:
```

Because the true path falls through into `case4Else`, clang keeps the two
blocks adjacent in that order.  The price is that the `b.w <epilogue>` after
the name body is absent (the true path falls through instead), and the 4 bytes
are recovered elsewhere (the function is size-exact).

The faithful spelling is:

```cpp
    if (nameLeader) {
        (*this->enemies)[0]->name = *Globals::gameText->getText(1663);
    }
    return;
    }
case4Else:
```

Measured at **88.432004 % / 17152 B / 6056 insns**.  The reason is block
placement: with an unconditional `return` at the end of the inner block, clang's
MachineBlockPlacement puts `case4Else` *before* the nameLeader block (it becomes
the fallthrough from the switch's `case 4`), so the whole 0x40de-onward region
shifts by -0x14 and the literal pools move.  Every spelling that makes the
isEmpty branch unconditionally return reproduces this:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `else { return; }`** | **92.326733** | 17172 | 6065 |
| `if (nameLeader) { … } return;` (faithful) | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { … } else { return; } return;` | 88.432004 | 17152 | 6056 |
| `if (!nameLeader) return; name=…; return;` (guard) | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { name=…; return; } return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { name=…; goto done; } else return;` + `done: ;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { name=…; return; }` (true returns, false falls to case4Else) | 91.237624 | 17172 | 6065 |

The `91.237624` form is size-exact but keeps the wrong fallthrough (false path
falls into `case4Else`), so it loses more matches than the `else` form.

Putting `case4Else`/`case4Tail` inside the inner block (`{ … case4Else: … }`)
is not possible without a `goto` that bypasses the block's declarations
(`jump bypasses variable initialization`); wrapping the tail in a further
nested block does not help because the `goto` from the switch still enters the
outer scope.

Conclusion: the `else { return; }` trick is the best available source shape for
this block.  It is a deliberate semantic deviation (the true path falls through
to `case4Else`) kept purely for block placement, and it is worth ~3.9 pp.

## 2. `case4Tail` `fp = 1` vs `fp = &Globals::rnd`

Original tail preheader / loop (offsets):

```text
0x415c  ldr  r0,[pc]            ; &Globals::globals
0x415e  mov.w fp,#1             ; fp = hostile arg
0x4162  mov.w r9,#0             ; i = 0
0x4166  add  r0,pc
0x4168  ldr.w sl,[r0]           ; sl = Globals::globals
0x416c  b    <cond>
...
0x4198  strd r0,fp,[sp]         ; createShip(..., wp, true, false)
0x419c  movs r0,#0
0x419e  str  r0,[sp,#8]
```

Ours (baseline) instead keeps the `&Globals::rnd` GOT address in `fp` and
materialises the constant 1 each iteration:

```text
0x4156  ldr  r0,[pc]            ; &Globals::rnd
0x415a  ldr.w fp,[r0]           ; fp = &Globals::rnd
...
0x4196  str  r0,[sp,#0]
0x4198  movs r0,#1
0x419a  str  r0,[sp,#4]         ; true
0x419c  movs r0,#0
0x419e  str  r0,[sp,#8]         ; false
```

The faithful re-read of `this->enemies->size()` on the loop back-edge is
8 B shorter (17164) and keeps `fp = &Globals::rnd` too; the `volatile` latch
is what restores the size.  Variants measured this session:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 | 6065 |
| `const volatile unsigned n` | 92.326733 | 17172 | 6065 |
| `volatile size_t/unsigned long/int n` (prior waves) | 92.326733 | 17172 | 6065 |
| faithful `i < this->enemies->size()` (unsigned/int/while) | 91.341312 | 17164 | 6060 |
| `Level *self = this;` + faithful re-read | 91.341312 | 17164 | 6060 |
| `Level *self = this;` + volatile latch | 92.326733 | 17172 | 6065 |
| `Globals *globals = Globals::globals;` before tail | 92.187113 | 17172 | 6066 |
| `Route *route = this->playerRoute;` before tail | 92.136315 | 17172 | 6064 |
| `AERandom *volatile rnd = Globals::rnd;` | 92.161716 | 17172 | 6065 |
| `AERandom **rndp = &Globals::rnd;` | 92.326733 | 17172 | 6065 |
| `AERandom *rnd = Globals::rnd;` | 91.113128 | 17168 | 6064 |
| `const bool hostile = true;` | 92.326733 | 17172 | 6065 |
| `volatile bool hostile = true;` | 91.041082 | 17184 | 6067 |

No spelling moves the `1` into `fp` without also changing the loop back-edge
(or shrinking the function by 8 B).

## 3. Alien-turret table select (`0x32ee`, 13-instruction cluster)

Original is a real in-loop branch:

```text
0x32c6  ldr  r0,[sp,#64]        ; bigAlienWave
0x32c8  cbz  r0,<else>
0x32ca  ldr  r1,[pc]            ; &kAlienStationTurrets
0x32cc  add  r1,pc
0x32ce  b    <join>
0x32d0  ldr  r1,[pc]            ; &kAlienOutpostTurrets
0x32d2  add  r1,pc
0x32d4  add.w r0,r1,r9          ; t + k*24
```

The current source uses the `(bigAlienWave + (i < 0))` anchor to keep the
select in the loop, but clang if-converts the value comparison:

```text
cmp.w r1, r9, asr #31
it    ne
movne r6, r0
```

The anchor is required: a plain `if (bigAlienWave) t = A; else t = B;` (or a
ternary, or `init t = B; if (bigAlienWave) t = A;`) is hoisted out of the loop
by LICM and scores 88.413577 / 17140 B.  All of the following simplify to 0 and
hoist: `(i<0)-(i<0)`, `((i<0)&0)`, `(i<0)*0`, `(i<0)/2`, `((i>>31)&0)`.
`__builtin_expect` on the anchored condition does not stop if-conversion.
Row-pointer spellings (`const float *t = (bigAlienWave ? A : B)[k];`) were
measured in w6946 at 83.7-88.4 %.

## 4. `&&` at `0x2e2c` / `0x2f52` (branchless `orrs`)

Original emits a branchless form for both station-loop tests:

```text
subs.w r0, fp, r3        ; i - bound
mov    r1, r0
it     ne
movne  r1, #1            ; r1 = (i != bound)
ldr    r2,[sp,#60]       ; alienWave
eor.w  r2, r2, #1        ; !alienWave
orrs   r1, r2
bne.w  <else>
```

Ours short-circuits (`ldr flag; cmp; beq; cmp i,bound; bne`).  Source
spellings tried this session, all 92.326733 / 17172:

* `if (alienWave && i == bound)`
* `if (!(i != bound || !alienWave))` and the swapped De Morgan form
* `bool isAlienStation = i == bound && alienWave; if (isAlienStation)`
* the same for the `pirateFortress` test

Clang 7 canonicalises the `&&` and keeps the short-circuit at the top-level
`if`; the branchless `EvaluateExprAsBool` path is not reachable from these
spellings (matches w6946 §3.5).

## 5. Other clusters re-checked (unchanged)

* `0x397c` (12 insns) and `0x3c94` (7) — convoy / extraAliens loop induction
  variable and register roles.  `int first = guns + …` hoists were re-measured
  in w6946 (91.1 % / 17180 B) and remain worse.
* `0x3e02` (7) — `miningPlantIndex` sum stack-slot colours
  (`sp+48/92/20/44/32` vs ours `sp+20/40/44/32/112`).  RA, not source.
* `0x1444` (8) — case-183 k-chain tests `[2,1,0]` with bodies `[0,1,2]`.
  The MIR before MachineBlockPlacement already has the wrong body order for
  every source spelling tried (w6946 §3.6); MBP, not source.
* `raid` (`sp+84`) vs `&Globals::rnd` (`sp+88`) spill-slot swap (17 sites,
  ~+0.28 pp if isolated).  Moving the `bool raid;` declaration earlier
  (`before stationIndex` / `before raidRoll`) drops to 91.485149; the
  declaration must stay immediately before the `if (introSystem …)`.
  `int/char/unsigned char raid`, `bool raid = false`, `raid = 0`, and the
  `AERandom *rnd` hoists were re-checked in w6925/w6946 and do not move it.
* `0x2d4` — jump-table halfword entries shift with the downstream block
  placement; not independently fixable.

## 6. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~45 new variants were measured this session on top of the ~500 in the
preceding wave-119 notes; none beat the baseline.  The residual is compiler
register allocation, stack-slot colouring, if-conversion and
MachineBlockPlacement, not source shape.

## 7. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
                       -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6962/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w6962/align.py            # address-aligned diff -> align.txt
python3 /dev/shm/w6962/opaddr.py           # difflib clusters -> opaddr.txt
python3 /dev/shm/w6962/range.py LO HI      # side-by-side disasm
python3 /dev/shm/w6962/try.py variants.py  # apply/build/score/restore
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6962` (`Level.base.cpp` = pristine HEAD copy,
`rea.c` = Ghidra decompile, `align.txt` = address-aligned diff,
`opaddr.txt` = difflib clusters with original addresses, `vA.py`…`vX.py` =
variant batches for this session).
