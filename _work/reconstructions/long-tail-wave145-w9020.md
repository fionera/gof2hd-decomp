# Long-tail wave 145 (w9020): Gun / Generator / SpaceLounge + `.ARM.extab` 1,370

Status: **no landed `src/` change** (every size-exact candidate was neutral or
worse). Worktree `wt/w9020`, base `eb13ada9b` (wave 144). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175** (unchanged).
`.ARM.extab` **1,370** at exact 156,312 B, `.text` 0x1337c8, `.ARM.exidx`
byte-identical.

## 0. `.ARM.extab` 1,370 bytes, by owner (re-audited at HEAD)

`exidx_raw_audit.py` + `extab_lsda_decode.py` on the pristine build:

| extab diff | reclen | exidx `fn` | owner | body raw |
|---:|---:|---|---|---:|
| **762** | 948 | 0x1490d8 | `HangarWindow::render` | 1731 |
| **158** | 956 | 0xaffd8 | `Level::createMission` | 5592 |
| **150** | 348 | 0x137d18 | `LevelScript::process` | 54570 |
| **93** | 336 | 0xc0214 | `Level::createShip` | 1978 |
| **86** | 132 | 0x12e050 | `Radar::draw` | 8471 |
| **37** | 476 | 0x163b90 | `Hud::draw` | 1325 |
| **37** | 280 | 0x17a144 | `MGame::OnTouchEnd` | 4399 |
| **24** | 172 | 0x971b8 | `Generator::createMission` | 445 |
| **14** | 168 | 0x170d38 | `SpaceLounge::drawLounge` | 175 |
| **7** | 256 | 0x12d484 | `Radar::Radar` | **0** |
| **2** | 112 | 0xd52d0 | `ModStation::ModStation` | 11 |
| **1,370** | | | | |

All 11 records are equal-length; only row boundaries / landing-pad offsets
differ. `Radar::Radar` (7) and `ModStation` (2) are pure label/layout ties on
byte-exact-ish bodies. The other nine are downstream of their body layouts.

## 1. `HangarWindow::render`: borderX fixes rows 19/20 but costs +4 section bytes

The w8818 borderX local is still the right source shape and still measured:

```cpp
int borderX = this->hintOffsetX +
              (textX + 5 + Globals::layout->buttonInsetX +
               (int) (rate * (float) (this->progressBarWidth - 4)));
Globals::Canvas->DrawImage2D(this->progressBarBorderImage, borderX,
                             y + 2 + Globals::layout->field_0x70_rowHeight / 2, 17, 20);
```

| variant | fn size | `.text` | `.ARM.extab` | extab diff | raw |
|---|---:|---|---:|---:|---:|
| base | 9700 | 0x1337c8 | 156312 | 762 | 1731 |
| **borderX** | 9700 | 0x1337c8 | **156316** | **687** | **1698** |
| iconX at loop top | **9660** | 0x1337a8 | 156308 | 784 | 8579 |
| borderX + iconX at loop top | **9664** | 0x1337ac | **156312** | — | 8571 |

The record has 156 rows at base and 157 with borderX: borderX correctly splits
the blueprint-progress row 19 into the original `(1530,144,9410)` +
`(1712,46,9358)`, but our free-credits `iconX`/`iconY` VarDecl split still adds
the extra row the original does not have. Merging it (`int iconX; int iconY;`
at the loop top, assign in the `if`) restores 156 rows / exact 156,312 B but
costs **-36..-40 B of body** (whole-function RA, not an alloca), so the
following functions shift and `byte_exact` collapses. BorderX alone is +4
section bytes, which moves `.rodata` to 0x1c9e20 and breaks the PC-relative
literal pool (`check` 4507 -> ~2939, as w8920 measured).

**Open lead:** a size-neutral free-credits merge. The original keeps the
constant 17 in `fp` across the `DrawImage2D(tabIcons[count], ...)` call
(`str.w fp,[sp]` at 0x1afc); ours rematerialises `movs r3,#17; str r3,[sp]`.
The original free-credits block is 210 B, ours ~170 B; the 40 B is a
whole-function RA difference, not missing source.

## 2. `SpaceLounge::drawLounge`: the pad/x swap is the **priority hint bit**

New mechanism (w8919 only measured the greedy spill weights). The allocator
queue is ordered by `Prio = 0x80000000 | (0x40000000 if hint) | 0x20000000 |
size` (`RAGreedy::enqueue`), **not** by spill weight. The trace of the
committed source:

```
ENQ %608 size=4220 prio=3758100604   ; x, hint=0,$r3  (0xE0000000+4220)
ENQ %646 size=6088 prio=2684360648   ; pad, hint=0,$noreg (0xA0000000+6088)
select %608 -> $r5                   ; x gets r5 first (hint bit)
select %46  (top-pad) w=6.25e-4      ; evicts x from r5 (x w=2.95e-4)
select %608 -> $r9                   ; x re-enqueued, takes r9
select %2   (pad) -> $r11            ; r9 taken by x, r10 by %381(pad*2)
```

The hint comes from `$r3 = COPY %608` (the `DrawString(..., x, ...)` argument),
so x carries `knownpref=1` and is selected before pad regardless of size. The
original has pad=r9 and x=r5->r9 (x is *split* at the drawBox call: `mov r3,r5;
... mov r9,r3`). That needs x to keep r5, i.e. `%46` must not evict it.

Probes with the instrumented clang (`/tmp/llvm_android_702b/build2`, `RA_TRACE`,
`VRW`, `PRIOVREG`, `NOHINT`, `NOEVICT`):

| probe | result |
|---|---|
| `VRW=x:4` (x weight 1.18e-3 > %46) | x -> r5, but %46 -> r10, %381 -> r9, pad -> r11 |
| `VRW=%46:0.1` | x -> r5, %46 -> r9, pad -> r11 |
| `PRIOVREG=pad` (pad hint bit) | pad -> r5 (low reg preferred), x -> r6 |
| `NOHINT=x` | pad's interval is rematerialised from `stack.17`, pad -> r4 |
| `NOEVICT=x` | x -> r5, %46 -> r9, pad -> r10 |

No source spelling tried reaches `pad=r9, x=r5->r9` at 1948 B. The remaining
175 raw bytes are exactly the pad/x register swap plus the `mov r5,fp` pad
copy that becomes unnecessary once pad is in r9. Source variants re-measured
this wave (all 1948 B unless noted): plain `width+pad*2` 445, `-width-pad-pad`
175, `short pad3` 175, `unsigned pad` 177, `const pad` 175, `x` before strings
295, `top` before `centerX` 175, `topY` local 1828/1964, `left/topY/boxW`
locals 175, `centerX` in clamp 175, `left+pad` in DrawString 175.

## 3. `Generator::createMission`: faithful bonus form is 304 raw but 2000 B

Re-confirmed: the faithful `bonus = rawBonus - rem` form is **304 raw /
2000 B** (frame 104) vs the shipped algebraic `bonus -= rem*2` **445 raw /
1996 B** (frame 96). The original's `str r1,[sp,#32]` / `ldr r0,[sp,#32]`
spills `rem`; ours keeps `rem` in a register and evicts `agent`. Hoisting
`int *imageParts = agent->getImageParts();` before the bonus block makes agent
dead across `__aeabi_idivmod` and reaches 1996 B but **1174 raw**. New
negatives this wave (all 1996/2000 B): `volatile rem` 702/2000, `const rem`
304/2000, `rawBonus % 50` recomputed 304/2000, `if (bonus % 50)` truthy
304/2000, base+imageParts 1127/2008, base+agentRace 1151/1988,
base+status-local 445/1996.

## 4. `Gun::calcCharacterCollision`: no new win

The crossed 12-byte/60-byte StackSlotColoring pairing is unchanged; the first
divergence is still `0x15306c` (`add.w r4,fp,fp,lsl#1` vs `r5`). New probes
(declaration orders, `left`/`topY` locals) are neutral or worse; the 463 raw
bytes are the frame-object colouring, not a local source shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9020
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w9020/fdiff.py <mangled>                 # raw same-address diff
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not committed), `/dev/shm/w9020/`: `fdiff.py`, `dranges.py`,
`dump.py`, `cc_sl.sh` (instrumented clang), `cc_sl_real.sh`, `measure_sl.sh`,
`linkcmd.sh`, `sweep_sl*.py`, `sweep_gen*.py`, `sweep_hw*.py`, `sweep_ms*.py`,
`SL.base.cpp`, `Gen.base.cpp`, `HW.base.cpp`, `MS.base.cpp`, `sl_trace.txt`,
`vrw4.txt`, `prio*.txt`, `noev.txt`, `nohint.txt`.

## 6. Files changed

`_work/reconstructions/long-tail-wave145-w9020.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
