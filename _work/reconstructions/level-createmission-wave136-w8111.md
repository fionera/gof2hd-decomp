# w8111 (wave-136): `Level::createMission` — station-pirate escort loop,
# pass-pipeline trace of the `10000` remat (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8111`, based on master `2e51d765d`.
Continues `level-createmission-wave135-w7775.md`, `…-wave134-w7755.md`,
`…-wave132-w7710.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; linked `.text` `0xaffd8`, 17,172 B). Ghidra MCP was unavailable
this session (`executable_missing`), so the original disassembly is the NDK
`objdump` of the original `.so`. Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   rawfn bytediff 5761
.text diff 84960   .ARM.extab 1404   .ARM.exidx 0
.rodata/.data/.bss/.rel.plt exact
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
report row pct 92.2, 6055 orig / 6064 ours insns
```

## Hypothesis tested: station-pirate escort loop source order / call-argument order

The source (`src/game/world/Level.cpp` ~2043-2053) is:

```cpp
int escortsEnd = pirateEscorts + first + pirates;
for (int i = first + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
     i < escortsEnd + this->friendCount + this->field_164 + this->field_168 + this->hostileCount; i++) {
    (*this->enemies)[i] = this->createShip(8, 0, Globals::globals->getRandomEnemyFighter(8), nullptr, true, false);
    Vector pos = {pirateBase.x + (float) (Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000),
                  pirateBase.y + (float) (Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000),
                  pirateBase.z + (float) (Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000)};
    (*this->enemies)[i]->setPosition((int) pos.x, (int) pos.y, (int) pos.z);
    (*this->enemies)[i]->setToSleep();
    ((PlayerFighter *) (*this->enemies)[i])->field_0x12f = 1;
}
```

The original body (`0xb3bb8`-`0xb3cd8`) is instruction-for-instruction the same
as ours **except** for the `10000` constant:

```text
original preheader:                       ours preheader:
b3ba2 ldr  r6,[sp,#20]   ; pirateEscorts  b3b8e ldr  r6,[sp,#32]   ; pirates
b3ba4 movw fp,#10000     ; hoisted        b3b90 ldr  r5,[sp,#20]   ; pirateEscorts
b3ba8 ldr  r5,[sp,#112]  ; first          b3b92 add  r6,r5
b3baa add  r6,r5                          b3b94 ldr  r5,[sp,#104]  ; first
b3bac ldr  r5,[sp,#32]   ; pirates        b3b96 add  r6,r5
b3bae add  r6,r5                          b3b98 str  r6,[sp,#92]
b3bb0 str  r6,[sp,#104]                   b3b9a ldrd r9,r5,[sp,#96]
b3bb2 ldrd r9,r5,[sp,#96]                 b3b9e b    b3cbc
b3bb6 b    b3ccc

original body (three times):              ours body (three times):
b3c18 mla  r0,r0,r6,fp                    b3c06 mla  r0,r0,r6,sl
                                          (plus b3bfe mov fp,sl / b3c00 movw sl,#10000
                                           and b3c52 mov sl,fp)
```

Everything else in the loop is identical: `createShip(8,0,getRandomEnemyFighter(8),
nullptr,true,false)`, the interleaved `vldr pirateBase.{x,y,z}` / `nextInt(20000)` /
`nextInt(2)` pairs, the `vcvt.s32.f32`→`vcvt.f32.s32` round-trip of
`setPosition((int)pos.x,(int)pos.y,(int)pos.z)` (the callee takes three `float`s),
`setToSleep()`, and the `strb #1,[...,#303]` (`field_0x12f`). The original also
reloads `this->enemies` before each of the three operations, so the source must
not cache the element pointer.

The residual is therefore **not** statement order. It is the `10000`
materialisation: original keeps it in `fp`; ours rematerialises it into `sl` and
spills `this` to `fp` for the duration (2 extra instructions, 8 bytes).

## New evidence: exact pass-pipeline trace of the remat

`-mllvm -print-after-all` on the current `Level.cpp` (scratch `pa.txt`, 130 MB)
shows the constant is hoisted correctly and then **rematerialised by Greedy RA**:

1. **Early Machine Loop Invariant Code Motion** — hoists it to the preheader
   (`bb.239`, `%ir-block.2781`), together with `8/0/1/20000/2/-1`:

   ```text
   %3497:rgpr = t2MOVi 8
   %3499:rgpr = t2MOVi 0
   %3501:rgpr = t2MOVi 1
   %3511:rgpr = t2MOVi16 20000
   %3514:rgpr = t2MOVi 2
   %3521:rgpr = t2MOVi -1
   %3523:rgpr = t2MOVi16 10000
   t2B %bb.243
   ```

2. **Greedy Register Allocator** — splits `%3523`'s live range at the loop body
   and rematerialises it (`%5417`), saving `this` (`%5420`) to a new vreg `%5419`:

   ```text
   80260B  %5419:gpr = COPY %5420:gprnopc      ; this -> fp
   80264B  %5417:rgpr = t2MOVi16 10000
   80272B  %3524:rgpr = t2MLA %3522, %3518, %5417
   ...
   80712B  %5420:gprnopc = COPY %5419:gpr      ; fp -> this
   ```

3. **Virtual Register Rewriter** — the preheader no longer has any `movw 10000`;
   the body has `renamable $r11 = COPY $r10` / `renamable $r10 = t2MOVi16 10000`
   / `renamable $r0 = t2MLA …, $r10` (r10 = sl, r11 = fp).

The post-RA MachineLICM (`1304079`) and PostRA Machine Sink do **not** re-hoist
it, because `r10` is live-in as `this`. So the difference is a Greedy-RA
tie-break between the two callee-saved registers, not a source-order issue.

## Variants measured this session (fresh `link_so` each)

All of the following are **neutral at 5761 / 17172** (no source win):

| variant | rawfn diff | size |
|---|---:|---:|
| baseline | 5761 | 17172 |
| `int first = stationPirates + guns;` (swap order) | 5761 | 17172 |
| `unsigned escortsEnd = …` | 5761 | 17172 |
| `const int escortsEnd = …` | 5761 | 17172 |
| `escortsEnd = pirateEscorts + first + (int)pirates` | 5761 | 17172 |
| `escortsEnd = (int)((unsigned)pirateEscorts + (unsigned)first + (unsigned)pirates)` | 5761 | 17172 |
| `escortsEnd = pirateEscorts + pirates + first` | 5761 | 17172 |
| `escortsEnd = first + pirateEscorts + pirates` | 5761 | 17172 |
| `escortsEnd = pirateEscorts + pirates; escortsEnd += first;` | 5761 | 17172 |
| `escortsEnd = first; escortsEnd += pirateEscorts + pirates;` | 5761 | 17172 |
| `escortsEnd = pirateEscorts + (guns + stationPirates) + pirates` | 5761 | 17172 |
| `Vector pos = Vector(…)` explicit temporary | 5761 | 17172 |
| `const int base = 10000;` inside loop | 5761 | 17172 |
| `int base = 10000;` before loop | 5761 | 17172 |
| `10000u` / `10000L` | 5761 | 17172 |
| `int base = 10000;` + `base` in all three components | 5761 | 17172 |
| `Vector pirateBase;` (default ctor) | 5761 | 17172 |
| `Vector pirateBase(0.0f,0.0f,0.0f)` | 5761 | 17172 |
| `int first` before `Vector pirateBase` | 5761 | 17172 |
| explicit `(float)(int)` at `setPosition` | 5761 | 17172 |
| `(float)(int)` on each `pos` component at declaration | 5761 | 17172 |

Regressions (not landed):

| variant | rawfn diff | size |
|---|---:|---:|
| swap `setToSleep()` and `field_0x12f = 1` | 5928 | 17160 |
| `escortsEnd = pirateEscorts + guns + stationPirates + pirates` | 6034 | 17168 |
| `pos` components cast to `(float)(int)` *before* the aggregate (changes type) | 5909 | 17196 |

The compiler canonicalises every `escortsEnd` permutation through `Reassociate`
(loads rank 1 sort before the `first` add rank 2), so the preheader order is
`pirates + pirateEscorts + first` regardless of source spelling.

## Compiler-flag probes (diagnostic, not landable)

`-mllvm -hoist-cheap-insts`, `-disable-machine-licm`,
`-disable-postra-machine-licm`, `-avoid-speculation`,
`-regalloc-csr-first-time-cost=0/100`, `-spp-rematerialization-threshold=0/100`,
`-enable-deferred-spilling`, `-split-spill-mode=size` all leave the escort
loop's `10000` rematerialised (only `-disable-machine-licm` moves it to a
different register, still in the body). The remat is robust to every RA knob
tried; the original's `fp` assignment is not reachable from source.

## Conclusion

The station-pirate escort loop's statement order and call-site argument order
are already exact. The remaining 2-instruction / 8-byte delta is a Greedy-RA
rematerialisation tie-break: early MachineLICM hoists `movw 10000` to the
preheader, then Greedy RA rematerialises it into `sl` and spills `this` to `fp`.
No size-neutral source formulation found (20+ variants above). Source left
pristine; gate unchanged.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8111/measure.py          # rawfn diff + section diffs
python3 /dev/shm/w8111/runvar.py varsN.json
cmake --build cmake-build-match --target check -j6
```

Scratch: `/dev/shm/w8111/{measure.py,runvar.py,disloop.py,cc_base.sh,
vars1.json,vars2.json,vars3.json,pa.txt,orig_escort.txt,ours_escort.txt,
aligned.txt,Level.pristine.cpp}`.

## Gate counts (unchanged; source reverted)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      84960 -> 84960
.ARM.extab diff 1404  -> 1404
.ARM.exidx diff 0     -> 0
.rel.plt        exact -> exact
```
