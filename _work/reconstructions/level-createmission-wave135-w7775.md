# w7775 (wave-135): `Level::createMission` — station-pirate escort loop,
# cached-pointer lever and its size cost

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7775`, based on master `bbffef482`.
Continues `level-createmission-wave134-w7755.md`, `…-w7756.md`, `…-wave132-w7710.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; linked `.text` `0xaffd8`, 17,172 B). Ghidra 12.1.2 headless
decompile of `_ZN5Level13createMissionEv` (`/dev/shm/w7775/gh_cm.md`) plus manual
Thumb disassembly of the original. Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   fn bytediff 5761
.text diff 85022   .ARM.extab 1416   .ARM.exidx 0
.rel.plt/.rodata/.data/.bss exact
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## New hypothesis tested: exact statement order / call-argument order

Ghidra's escort loop (gh_cm.md ~1460-1500) is:

```c
iVar14 = getRandomEnemyFighter(globals, 8);
uVar12 = createShip(this, 8, 0, iVar14, nullptr, true, false);
fVar56 = pirateBase.x;                       // vldr s16,[sp,#312]
*(...)[i] = uVar12;
iVar14 = nextInt(rnd, 20000); iVar36 = nextInt(rnd, 2);   // x pair
fVar57 = pirateBase.y;
iVar43 = nextInt(rnd, 20000); iVar53 = nextInt(rnd, 2);   // y pair
fVar60 = pirateBase.z;
iVar36 = nextInt(rnd, 20000);
fVar61 = (float)(sign*iVar14 + 10000);       // x offset
fVar59 = (float)(sign*iVar43 + 10000);       // y offset
iVar14 = nextInt(rnd, 2);
fVar55 = (float)(sign*iVar36 + 10000);       // z offset
uVar13 = (int)(fVar56 + fVar61);             // arg 1 (x)
uVar58 = (int)(fVar57 + fVar59);             // arg 2 (y)
piVar51 = (*enemies)[i];
uVar12 = (int)(fVar60 + fVar55);             // arg 3 (z)
(**(code**)(*piVar51 + 0x48))(piVar51, uVar13, uVar58, uVar12);
setToSleep((*enemies)[i]);
*(u8*)((*enemies)[i] + 0x12f) = 1;
```

The checked-in source already reproduces this exactly: `createShip` statement,
then the three-component `Vector pos = {…}` aggregate initialiser (whose
left-to-right argument evaluation interleaves `pirateBase.<comp>` loads with the
`nextInt` pairs), then `setPosition((int)pos.x,(int)pos.y,(int)pos.z)`, then
`setToSleep`, then the `field_0x12f` store.  The original reloads
`this->enemies` before *each* of the three operations, so the source does **not**
cache the element pointer.

The remaining loop residual is therefore pure register allocation:

* original keeps `this` in `sl` and hoists `movw fp,#10000` into the preheader,
  using `mla r0,r0,r6,fp` three times;
* ours does `mov fp,sl; movw sl,#10000` inside the body, `mla …,sl`, and
  `mov sl,fp` to restore — i.e. `this`/`10000` swap `sl`/`fp`.
* original preheader sums `escortsEnd = pirateEscorts + first + pirates`
  (`ldr [sp,#20]; ldr [sp,#112]; ldr [sp,#32]`); ours emits
  `(pirates + pirateEscorts) + first`.  The LLVM `Reassociate` pass sorts by
  rank (phi < fptosi < add), so this order is canonical and *all* source
  reorderings of `escortsEnd` are neutral (verified: 8 variants, all 5761,
  except the `first`-before-`pirates` forms which shrink to 17168).

## The cached-pointer lever (new, but not size-neutral)

Caching the element pointer in a statement-local changes the greedy register
allocator's global assignment and makes the **post-loop** `extraAliens` /
`pirateFortress` blocks match the original almost instruction-for-instruction
(their diff drops 837 -> 329 in `[0xb3e50,0xb4200)`).  The loop itself grows by
12 B (an extra `mov r0,r4` + `blx setToSleep` + `movs r5,#1` + `strb`).

```text
body after the createShip statement        fn bytediff   fn size
baseline (no cache)                             5761      17172
KIPlayer *e; e->setPosition; e->setToSleep;
   ((PlayerFighter*)(*this->enemies)[i])->f=1   5271      17184   (+12)
same, all three via e                           5278      17184   (+12)
same, all three via e, `unsigned n` (no volatile)5240     17180   (+8)
same, e->setPosition only                       5761      17172   (neutral)
same, e->setToSleep only                        5874      17180
same, field store only                          5883      17180
Array<KIPlayer*> *en = this->enemies; all via en 5278     17184   (+12)
inline setPosition + cached e                   5885      17176
inline setPosition + cached e + plain n         5896      17172   (worse)
```

None of the size-exact (`17172`) formulations improves on 5761.  Every
improving formulation is +8 or +12 B, which is a **section-size regression**:
`.text` grows 1259464 -> 1259496 (+32, alignment) and `.ARM.exidx` diff jumps
0 -> 3553 because every following function's exidx entry shifts.  That is why
the change was not landed.

## Why the size is hard to fix locally

Aligning original and ours instruction-by-instruction gives a cumulative
address delta (`orig - ours`) that oscillates: 0 at entry, -24 near `0xb1420`,
+8/+14/+18 around `0xb2e26`-`0xb3272`, -12 at `0xb3332`, 0 at `0xb34ea`,
+24 at `0xb3bb8` (escort loop), +12 at `0xb3dfe`, 0 at `0xb4194`, 0 at
`0xb4276`.  The function is a patchwork of ±2/±4/±8/±12-byte backend
permutations that happen to cancel to the exact 17172 B total; a local +12 in
the loop cannot be cancelled by any single local source change found.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7775/measure.py        # fn size/bytediff + section diffs
python3 /dev/shm/w7775/insdiff.py LO HI  # per-instruction orig/ours diff
cmake --build cmake-build-match --target check -j6
```

Scratch: `/dev/shm/w7775/{measure.py,insdiff.py,runs.py,cmpwin.py,run.py,
v1.json,v2.py,v3.py,v4.py,v5.py,v6.py,v7.py,v8.py,v9.py,v10.py,v11.py,v12.py,
Level.base.cpp,orig.asm,ours.asm,gh_cm.md,Level.ll}`.

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
.text diff      85022 -> 85022
.ARM.extab diff 1416   -> 1416
.ARM.exidx diff 0      -> 0
.rel.plt        exact  -> exact
```
