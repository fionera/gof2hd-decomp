# Small tail wave 146 (w9215): Gun / Generator / SpaceLounge —
# LICM sret-address hoist, faithful-bonus frame object, and a corrected
# SpaceLounge clamp reading

Status: **no landed `src/` change.** Worktree `wt/w9215`, base `31297fadc`
(wave 145). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave145-w9020.md`, `near-miss-trio-wave145-w9019.md`,
`long-tail-wave144-w8919.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175** (unchanged).
`.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes)

| target | size | raw | first divergence |
|---|---:|---:|---|
| `Gun::calcCharacterCollision` | 1984 B | 463 | `0x15306c` `add.w r4` vs `r5` (i*3) |
| `Generator::createMission` | 1996 B | 445 | `0x976d4` bonus block |
| `SpaceLounge::drawLounge` | 1948 B | 175 | `0x170ebe` pad `r9` vs `fp` |

## 1. Gun — the r4/r5 swap is a **LICM/rematerialisation of the k-loop
##    `getPosition()` sret address**, not a source-local choice

New mechanism this wave.  The first divergence (`i*3` in `r4` vs `r5`) is
downstream of a 2-byte-shift run at `0x153198..0x1532c1`.  In the original the
scatter k-loop body recomputes the `Player::getPosition()` sret address each
iteration:

```
153442: ldr r0,[r0,#4]        ; enemies->data
153444: add r4,sp,#104        ; sret temp for getPosition  (inside the loop)
153446: ldr r8,[r0,r5,lsl#2]  ; enemy
```

HEAD hoists that `add r4,sp,#104` out of the k-loop into **both** predecessors
of the scatter block (`0x153198`, asteroid path, and `0x153260`, non-asteroid
path).  Because `r4` is then live across the whole scatter block, the inner
`i*3` index is forced into `r5` (original: `r4`), which rotates the
`playerPosition`/`operator-` 12 B/60 B StackSlotColoring pair
(`sp+44`/`sp+104` crossed) and shifts every branch target by 2 bytes.

New source probes (all reverted; all shrink the function, so none composes):

| variant | raw | size |
|---|---:|---:|
| HEAD | **463** | 1984 |
| `Player *other = (*this->enemies)[k]` in the k-loop | 1782 | 1896 |
| `Vector pos = enemy->getPosition(); pos -= position;` | 1772 | 1876 |
| both (`other` + local `pos`) | 1742 | 1864 |
| remove k-loop `__builtin_expect(distance < magnitude, 0)` | 1810 | 1908 |
| remove mine-branch `__builtin_expect` | 673 | 1984 |

`G1..G5` (named `enemyPos`, direct `this->targetDir`, `td` inside the scatter
block, `Vector diff = enemy->getPosition() - position`) are all 463 or shrink.
The hoist is the backend's rematerialisation of the stack address; no source
spelling tried prevents it while preserving the 1984 B size.

## 2. Generator — the faithful `bonus = rawBonus - rem` is 304 raw but the
##    extra frame object is the **agent spill**, not `rem`

Re-confirmed: faithful `bonus = rawBonus - rem` is **304 raw / 2000 B**
(frame 104) vs shipped algebraic `bonus -= rem*2` **445 raw / 1996 B**
(frame 96).  The original is 1996 B and uses the faithful operand form
(`subne r5,r6,r0`, `str r1,[sp,#32]` / `ldr r0,[sp,#32]`).

New this wave: the +8 frame is **one extra live 4-byte object** (MIR
`Frame Objects` after StackSlotColoring: base `fi#5..11` live, faithful
`fi#5..12` live).  The extra object is the **agent spill**, not `rem`: in the
faithful form `rem` stays in `r9` (`mov r9,r1` / `subne.w r5,r6,r9`) and
`agent` is reloaded from `[sp,#36]` at `0x9776a`; the original keeps `agent`
in `r9` and spills `rem` into the dead `status` slot `[sp,#32]`.  The frame
grows 96 -> 104 only because the last live 4-byte slot moves from `SP-152` to
`SP-156` (8-byte frame alignment).

New source probes (all reverted):

| variant | raw | size |
|---|---:|---:|
| HEAD algebraic | **445** | 1996 |
| faithful `bonus = rawBonus - rem` | **304** | **2000** |
| faithful + `int rem` at function scope / `int rawBonus` outer | 304 | 2000 |
| faithful + `Agent *a = agent` | 304 | 2000 |
| faithful + `short`/`unsigned`/`const`/`signed char rem` | 304 | 2000 |
| faithful + `volatile rem` | 702 | 2000 |
| faithful + `short bonus` | 363 | 2000 |
| faithful + `volatile short bonus` | 956 | **1996** |
| faithful + `int b = ...; bonus = b;` | 304 | 2000 |
| faithful + second `rawBonus % 50` in the if | 304 | 2000 |

The faithful form cannot be landed at 1996 B: a 4-byte size change shifts the
following functions and `check` drops byte_exact **4507 -> 4412**.  The
remaining lever is to make `rem` (not `agent`) spill; every type/scope
spelling still keeps `rem` in `r9`.

## 3. SpaceLounge — **correction**: the original clamp reuses `sl = pad*2`,
##    it does not sign-extend a `short`

The wave-145 note says the original right-edge clamp folds through a 16-bit
sign-extend.  The original disassembly (`0x1712d4..0x1712e2`) is:

```
1712d4: add.w r1, r9, r5      ; pad + x
1712d8: add r1, r6            ; + width
1712da: cmp r1, r0
1712dc: itt gt
1712de: addgt.w r1, r6, sl    ; width + sl   (sl = pad*2, set at 0x170ed2)
1712e2: subgt r5, r0, r1      ; x = (w - inset) - (width + pad*2)
```

The committed `short pad3` source is what produces the sign-extend in ours
(`movgt.w r1, fp, lsl #16; addgt.w r1, r6, r1, asr #15`), which is why the
2-byte-shift run exists.  Plain `width + pad * 2` does match the original
clamp instruction (`addgt.w r1, r6, sl`) but the allocator leaves `x` in `r9`
and `pad` in `fp` (original: `x` in `r5`, `pad` in `r9`), adding `mov r5,fp`
copies and giving **445** raw.  The real blocker is still the pad/x register
priority (wave-145 trace), not the clamp expression.

New probes (all reverted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`short pad3`) | **175** | 1948 |
| `short pad = Globals::layout->field_0x94` | 1484 | 1960 |
| `int pad = (short) Globals::layout->field_0x94` | 1484 | 1960 |
| `pad3 + pad3` instead of `pad3 * 2` | 497 | 1952 |
| `int pad2 = pad + pad` reused top/clamp | 445 | 1948 |
| plain `width + pad * 2` | 445 | 1948 |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9215
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9215/fdiff.py <mangled>                # raw same-address diff
```

Scratch (not committed), `/dev/shm/w9215/`: `fdiff.py`, `dr.py` (diff-run +
instruction report), `measure.sh`, `gun_sweep{,2,3}.py`, `gen_sweep{,2,3,4,5,6}.py`,
`sl_sweep.py`, `*.base.cpp`, `gen_paa*.txt` (MIR frame objects).

## 5. Files changed

`_work/reconstructions/small-tail-wave146-w9215.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
