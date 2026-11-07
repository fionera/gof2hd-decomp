# w8611 (wave-141): `Level::createMission` — alien `bool` spill anatomy,
# `guns` rematerialisation, and tail-loop store-order probes

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8611`, based on master
`e18e0c2a4` (wave 140). Continues `level-createmission-wave140-w8511.md`,
`…-wave139-w8411.md`, `…-wave138-w8311.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8611/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5763
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## NEW evidence 1 — `hasWanted` is `bool`, and its combined guard already matches

The friend loop guard (`0xb2a9e`) is the *same* branchless `&&` shape as the
alien guard:

```text
b2aa2 mov   r0, r1          ; r1 = i
b2aa4 cmp   r1, #0
b2aa6 str   r1, [sp, #104]  ; spill i
b2aa8 it    ne
b2aaa movne r0, #1          ; r0 = (i != 0)
b2aac eor.w r1, r2, #1      ; r1 = hasWanted ^ 1
b2ab0 orrs  r0, r1
b2ab2 bne   b2af0
```

`hasWanted` is already `bool` in the source, so clang emits the `eor #1`
branchless form. **This proves the `eor` is a `bool`-type effect, not a
different source construct.** The alien flags (`alienWave`, `bigAlienWave`,
`pirateFortress`) are therefore `bool` in the original too. Ours differs only
in the placement of the `i` spill (`str` before vs after `it ne`), a
scheduler tie-break, and in the register used (`r1` vs `r6`).

## NEW evidence 2 — the 16-byte `bool alienWave` deficit is a *spill* artifact

With `bool alienWave`, the alien guard matches the original exactly:

```text
subs.w r0, fp, r3
it     ne
movne  r0, #1
ldr    r1, [sp, #60]
eor.w  r1, r1, #1
orrs   r0, r1
bne.w  ...
```

but the function is 17,156 B (−16). The IR shows why: the three flags are SSA
`phi` values (`%1110` i32, `%1111` i1, `%1112` i32) and the "re-init" at
`0xb0f16` is the register allocator **spilling the phi results** at the join
block. With `bool alienWave` clang keeps the i1 phi in a register and drops its
spill; the original spills *all three* with word `str`:

```text
original b0f16: mov.w r0,#0 ; mov.w r1,#0 ; str r0,[sp,#60]
                mov.w r0,#0 ; str r1,[sp,#56] ; str r0,[sp,#64]   (18 B)
bool ours b0f14: movs r1,#0 ; str r1,[sp,#56]                       ( 4 B)
```

The remaining 2 B are the `mov r1,r0` the original keeps before `it ne`
(ours reuses `r0`). Declaration-order, explicit re-init, `(bool)`, `!!`,
`!=0`, `==1`, `==true`, `& 1`, and `bool isAlien = …` spellings all fail to
force the i1 spill (measured; see table below).

## NEW evidence 3 — `guns` is rematerialised in the original, not materialised

The original keeps `alienGuns` at `[sp,#52]` and `fortressGuns` at `[sp,#40]`
and adds them **inline** at each loop bound:

```text
b3758 ldr.w fp, [sp, #40]   ; fortressGuns (loaded before the raid guard)
b37a6 ldr.w sl, [sp, #52]   ; alienGuns
b37e6 add.w r5, sl, fp      ; guns = alienGuns + fortressGuns
b396c add.w r8, sl, fp      ; recomputed for the pirate loop
```

Our source has `int guns = alienGuns + fortressGuns;`, which clang materialises
once and spills, hoisting `guns + friendCount` above the raid guard. Removing
the `guns` local and inlining `alienGuns + fortressGuns` at every tail use
reproduces the original's `add sl, fp` shape but costs **+28 B** (17,200 B) —
the extra adds are exactly the rematerialisation the original pays for. The
original must therefore be `bool` flags *and* inline `guns`: the two effects
are ~size-neutral together. Measured combinations:

| variant | size | raw diff |
|---|---:|---:|
| int flags, `int guns` (HEAD) | 17172 | **5763** |
| bool alien, `int guns` | 17156 | 12394 |
| bool alien+big, `int guns` | 17164 | 12378 |
| bool alien, inline guns | 17192 | 12503 |
| bool alien+big, inline guns | 17200 | 12509 |
| bool all three, inline guns | 17320 | 16539 |
| bool alien+big + case-2 alias removal | 17172 | 11888 |

None beats HEAD: the bool variant's internal size distribution shifts the
whole tail (phase-shift starts at `0xb0ed8`), so even the size-matched 17172
variant scores 11,888.

## NEW evidence 4 — case-183 switch body order does not change block layout

All six body permutations that keep `default` last were measured. Only the
source orders beginning with `case 0` reproduce the original compare chain
(`cmp #2; cmp #1; cbnz`); the emitted block layout is always
`case2, case0, case1, default`, while the original is `case2, case1, case0,
default`. Body order `0,1,2` and `0,2,1` both give raw 5763; `2,*,*` gives
5766; `1,*,*` gives 5771. This confirms the wave-138/139 conclusion: the
`case0`/`case1` block swap is an MBP tie-break, not reachable from the switch
source.

## NEW evidence 5 — tail-loop store-order probes

* **case-2 convoy loop** (`0xb1930`): the original emits
  `str [r1,#80]` (cargo) then `strb [r1,#76]` (crateCaptured) from a single
  element load, and reuses the array base `r0` for the `setSpeed` element
  reload. Our `enemies` alias caches `this->enemies` in `r4` across the whole
  body. Removing the alias gives the right store order but adds an element
  reload (+8 B) and shifts the region. Swapping the two source stores is
  size-neutral but +1 raw (5764).
* **friend loop** (`0xb2aa2`): swapping `hasWanted && i == 0` to
  `i == 0 && hasWanted` is −8 B and shifts the tail (12,642); `(i==0) &&` and
  `!(i != 0)` are identical to the swap. HEAD's order is correct.
* **case-1 `objectivesA`** (`0xb20f6`): still tail-merged into the case-4
  tail store because `this` is in `r4` at both sites; the original's tail store
  uses `this` in `r8`. Unchanged (6-byte region).

## Conclusion

No source change this wave beats HEAD. The alien block is now fully explained:
the flags are `bool`, the `eor` is the `bool` branchless-`&&` lowering, the
"re-init" is a register-allocator spill of the three phi values, and `guns` is
rematerialised inline. Landing it requires the i1 phi spill to survive, which
the current clang 7.0.2 `-Oz` register allocator eliminates. Baseline
(17,172 B, raw 5,763) retained.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8611/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8611/buckets.py _ZN5Level13createMissionEv 0x100
python3 /dev/shm/w8611/cmp.py 0xb2d80 0xb2f80
python3 /dev/shm/w8611/idiff2.py 0xb0000 0xb4300
python3 /dev/shm/w8611/sizediff.py 0xb2d00 0xb3600
```

Scratch: `/dev/shm/w8611/{Level.base.cpp,fdiff.py,buckets.py,cmp.py,
idiff.py,idiff2.py,sizediff.py,measure.sh,sweep*.py,casesweep.py,
raidsweep.py,orig.asm,ours.asm,t/}`.
