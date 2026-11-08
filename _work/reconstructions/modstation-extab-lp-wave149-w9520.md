# ModStation ctor + SpaceLounge::drawLounge — wave 149 (w9520): LP order decoded,
# the pre-RA CMOV copy is the extab tie, and the drawLounge pad/x colouring

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9520`, base
`fece77a64` (wave 148). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. Body sizes unchanged: `drawLounge` 1,948 B, `ModStation` 1,008 B.

Targets and unchanged metrics (raw same-address diff / instruction-index
alignment / extab bytes):

| fn | bodyraw | aligned | extab |
|---|---:|---:|---:|
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | 42 | 14 |
| `ModStation::ModStation` (`0xd52d0`) | 43 | 12 | 2 |

## 1. ModStation: the literal-pool order is the **MIR LDRLIT order**

The original pool (`0xd56b0..0xd56bc`) is

```
gHangarCamPitch  gHangarCamYawIPad  gHangarCamYaw  Globals::Canvas
```

and ours is

```
gHangarCamPitch  gHangarCamYaw      gHangarCamYawIPad  Globals::Canvas
```

(confirmed by resolving the `add rX,pc` targets: `Pitch=0x1fe030`,
`YawIPad=0x1fe058`, `Yaw=0x1fe080` in both binaries; only the two middle
literals swap).

The pool is emitted in the order of the `tLDRLIT_ga_pcrel` pseudos in the
pre-RA MIR.  `-mllvm -print-machineinstrs` for `_ZN10ModStationC2Ev` gives,
in `bb.15` (the near-key block):

```
%115 = tLDRLIT_ga_pcrel @_ZL15gHangarCamPitch
%117 = tLDRLIT_ga_pcrel @_ZL13gHangarCamYaw
%165 = tLDRLIT_ga_pcrel @_ZL17gHangarCamYawIPad
...
%121 = COPY %165              ; false arm = YawIPad
%121 = t2MOVCCr %121, %117, 0 ; true arm  = Yaw  (it eq)
```

The IR select is `select (icmp eq 0), Yaw, YawIPad` (true = `Yaw`).
The **legalized DAG** order is actually `Pitch, YawIPad, Yaw`
(`t70 Pitch`, `t72 YawIPad`, `t74 Yaw`), but `DoInstructionSelection` walks
the topological list backwards and emits the CMOV's *true* operand (`Yaw`)
before its *false* operand (`YawIPad`), so the MIR/pool order becomes
`Pitch, Yaw, YawIPad`.  The original needs the false operand emitted first;
that is the reverse of the current `CMOV(false,true)` emission and is a
selection/scheduling tie, not a source-order fact.

## 2. ModStation: the "one pre-RA copy" is the CMOV false-arm init

`%121 = COPY %165` is the only pre-RA copy in the block.  It becomes
`mov r0,r3` (`r3 = YawIPad`).  The extab row difference is its position
relative to the `cmp`:

```
orig:  mov r0,r3 ; cmp r1,#0 ; str r3 ... ; it eq ; moveq r0,r4
ours:  cmp r1,#0 ; mov r0,r3 ; str r3 ... ; it eq ; moveq r0,r4
```

The original's protected range starts at the `ldr.w r3,[r0,r8,lsl#2]` (the
yaw load) because the COPY is already before the CMP; ours starts at the
COPY, producing the `(390,30,922)` vs `(398,22,922)` row.  The scheduler
places the COPY after the CMP in every source shape tested; the CMP depends
on the `ldrb` of `Globals::iPad` and is ready first.

Source shapes tested this wave (all compile to the same MIR as the base):
`iPad == 0 ? Yaw : YawIPad`, `!iPad ? Yaw : YawIPad`, `iPad != 0 ? ...`,
`bool ipad = iPad != 0`, `if/else`, `switch`, `(iPad & 1) ? ...`,
swap `pitchTable`/`yawTable` declarations, compute `pitchBits` before
`yawTable`.  The only shape that flips the pool is
`(signed char)iPad > 0 ? YawIPad : Yaw` (wave 132), which changes the
condition to `it gt` and mirrors the roles (1,000 B).  Dropping `volatile`
from the yaw load (wave 132's lever) flips the pool and keeps the roles but
re-schedules the `Yaw` LDRLIT/add pair; measured this wave it is **raw 44,
aligned 18** (vs base 43/12), i.e. worse on both metrics.

## 3. SpaceLounge::drawLounge: the residual is pad/x colouring + right arm

The 42-instruction residual is two coupled facts:

* `pad` is loaded into `r9` in the original and into `fp` in ours
  (`ldr.w r9,[r6,#148]` vs `ldr.w fp,[r6,#148]`); `x` (centerX) is
  `r5 -> r3 -> r9` in the original and `r9` whole in ours.
* the right-edge clamp arm is `itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`
  in the original (2 instructions, reusing `sl = pad*2`) but
  `itttt gt; movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6;
  addgt.w r9,r1,r0` in ours (4 instructions).

The `pad + pad` form in HEAD is the best of the two: the faithful clamp
(`width + pad * 2`, or an explicit `int pad2 = pad + pad` used in the
clamp) reproduces the original right arm exactly but re-colours the tail:
it adds two `mov r5,fp` (moving pad out of `fp`) and drops the join
`mov r5,fp`, so every address after the join shifts by 2 bytes.  Measured:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **HEAD `pad + pad`** | **176** | **42** | 1948 |
| `width + pad * 2` / `pad2` variable | 445 | 54 | 1948 |
| `int totalWidth = width + pad + pad` | 176 | 42 | 1948 (same MIR) |
| `w - inset - width - pad - pad` | 464 | 64 | 1944 |
| `bottomRight.y - pad - pad` | 1825 | 164 | 1968 |
| move `Layout *layout` to the top | 1832 | 276 | 1972 |
| merge `centerX`/`x`, swap top/centerX order | 176 | 42 | 1948 |

The `totalWidth` variant is CSE'd back to the same IR; the IR for the clamp
is already `%174 = mul i32 %45,-2; %175 = sub %174,%159; %176 = add %175,%171`
(`-2*pad - width + (w-inset)`), so InstCombine has reassociated away the
`width + pad*2` grouping before ISel.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9520
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 /dev/shm/w9520/fdiff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w9520/align_diff.py _ZN11SpaceLounge10drawLoungeEv
python3 _work/reconstructions/extab_census.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

MIR dump used for the ModStation decode (not committed):

```sh
clang++ ... -mllvm -print-machineinstrs -c src/game/menu/ModStation.cpp
# then look at "After Instruction Selection" bb.15 and the
# "Legalized selection DAG" of bb.12 'invoke.cont47'
```

Scratch (not committed), `/dev/shm/w9520/`: `fdiff.py`, `align_diff.py`,
`ms/` (ModStation variant compiler + `mi.txt`), `sl_orig.txt`, `sl_ours.txt`,
`sl_faith.txt`.

## 5. Open questions

1. A source shape that makes the CMOV false operand (`YawIPad`) be emitted
   before the true operand (`Yaw`) while keeping `it eq` and the roles.
   The DAG order is already `Pitch, YawIPad, Yaw`; only the reverse
   topological emission order swaps it.
2. A source shape that schedules the `COPY %165` before the `CMP` so the
   ModStation extab row matches.  The CMP depends on the `ldrb` and is
   ready first in every shape tested.
3. A source shape that gives `pad` a higher RA weight than `x` without
   also exceeding `agent`/`lineHeight` (the wave-130 `agent > pad > x`
   window), so pad lands in `r9` and x in `r5` while keeping the
   `pad + pad` right arm.
