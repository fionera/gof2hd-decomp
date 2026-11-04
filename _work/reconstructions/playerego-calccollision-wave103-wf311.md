# PlayerEgo::calcCollision — wave 103 (wf311): the new KiritoJPK oracle
# confirms the *nested* source shape, but the allocator fixed point stands

**Status:** research note, 2026-11-04. Worktree `wt/wf311`, branch
`wave103-wf311`, base master `1d2b4398e`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note adds
the wave-103 oracle comparison and ~200 new measured builds (loop forms,
`__builtin_expect`, arm order, cold-block forms, anchor placements).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions, 87.6 %).

## TL;DR

The new oracle
`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/PlayerEgo.c`
(`calcCollision @0xab550`, 1058 B in the clang-3.8 `-O2` mod rebuild) renders
the body as the **fully nested** shape:

```cpp
if (objects != 0 && (!dockedFlag || dockingPointIndex - 1 > 2) && objects->count != 0) {
    Vector &pull = this->moveTarget;          // hoisted before the loop (this+0xec)
    unsigned i = 0;
    do {
        KIPlayer *object = objects->data[i];
        if (object != 0) {
            if (i == 0 && !status->inAlienOrbit()
                && VectorLength(this->dockOffsetVec) < 16000.0f && object->field_0x71)
                this->collidesWithStationFlag = 1;
            Vector surface = this->geometry->getPosition();
            if (object->outerCollide(surface)) {
                if (object->getType() == 16994 && object->isVisible()) {
                    /* wormhole arm */
                } else if (!object->stealFlagByte) {
                    /* visible / projectCollisionOnSurface arm */
                } else {
                    /* steal / getProjectionVector arm */
                }
            }
        }
        ++i;
    } while (i < objects->count);
}
```

That is the same nested shape pass 12 identified from the Android Ghidra
decompile (`body_NEST`, 64.1 % / 968 B). The oracle adds two details that
were not in the pass-10/11/12 notes: (a) `Vector &pull = this->moveTarget`
is hoisted before the loop, and (b) the loop is a `do … while` after an
explicit `count != 0` guard. Neither changes the Android codegen: the
compiler already hoists `&moveTarget`/`&moveDirection` to the preheader, and
the real binary is top-tested (`0x9f134 ldr r0,[r4,#0]; cmp fp,r0; bcs
0x9f452`), so the oracle's `do … while` is a Ghidra artifact.

The oracle therefore does **not** move the fixed point: the faithful nested
shape still scores 64.1 %, and the committed two-anchor shape still scores
87.6 %. The remaining gap is still the `dock`/`i` register pair plus the
latch position, exactly as pass 10/11/12 concluded.

## 1. Original binary facts (re-confirmed)

Manual Thumb disassembly of `android_2.0.16_libgof2hdaa.so`
`0x9f0cc`–`0x9f492`:

* preheader spills `&dockOffsetVec` once: `0x9f110 add.w r9,r5,#328;
  0x9f12a str.w r9,[sp,#20]`; reloaded at `0x9f362`, `0x9f392`, `0x9f44c`.
* top-tested loop: header `0x9f134`, latch `0x9f38c`
  (`add.w fp,fp,#1; b.n 0x9f134`). One `[r4,#0]` size load per iteration.
* register file `this=r5 objects=r4 object=r8 dock=r9 i=fp surface=r6`;
  canary pointer spilled to `[sp,#24]`; frame 112.
* two `cmp fp,#0`: `0x9f14a` (i==0 && !inAlienOrbit …) and `0x9f374`
  (i==0 && object->field_0x71, the collision-arm guard).
* collision-arm register shuffle is real:
  `0x9f30c mov sl,r5; 0x9f30e mov r5,r6; 0x9f310 add r6,sp,#96; …;
  0x9f326 mov r6,r5; 0x9f328 mov r5,sl`.
* literal pool: `16000.0f`, `0.2f`, `0.1f`, then the GOT slots for
  `__stack_chk_guard`, `Globals::status`, `Globals::sound`,
  `Globals::Canvas` ×2.

The oracle's field offsets differ by 4 in the `KIPlayer` head
(`stealFlagByte` at 0x38 there vs 0x3c here; `field_0x71` at 0x6d vs
0x71), so it is a slightly different source revision — logic/shape only,
never offsets.

## 2. Measured variants (official `tools/verify/verify.py`)

All bodies were spliced into `src/game/ship/PlayerEgo.cpp`, built with
`cmake --build cmake-build-match --target link_so -j8`, and scored with
`python3 tools/verify/verify.py … --show …`.

| variant | pct | size | note |
|---|---:|---:|---|
| **checked-in (A1+A2, no real guard)** | **87.6** | 968 | landed |
| oracle nested (`if (object != nullptr)` + `if (outerCollide)` + else-if) | 64.1 | 968 | frame 120, dock=sl, object=r9, surface=r8 |
| oracle nested + named `position` | 67.5 | 968 | frame 128, guard spilled |
| wrap `if (i < objects->size())` after outerCollide | 87.3 | 968 | latch at end, dock=fp, i=r9 |
| wrap + A2 (`size()==i` in wormhole) | 85.5 | 976 | dock=r9, i=fp, latch at end |
| wrap + A2 + real guard | 85.7 | 984 | |
| committed + real guard (A1+A2+G) | 85.0 | 976 | |
| committed + real guard, no A2 (A1+G) | 82.6 | 968 | dock=fp, i=r9 |
| committed + `do … while` + `count != 0` | 73.1 | 996 | |
| committed + `i != size` | 87.4 | 968 | |
| committed + `size() > i` | 87.1 | 968 | |
| committed + `__builtin_expect(object==nullptr,0)` | 81.2 | 964 | |
| committed + `__builtin_expect` on A1/A2 | 70.5 / 81.6 | 992 / 968 | |
| committed, collision arm before steal (else-if) | 87.6 | 968 | byte-identical to landed |
| committed, named `Vector position` for collision | 57.7 | 996 | |
| committed, `Vector collision` function-scope | 77.6 | 988 | |
| committed, `Vector collision` loop-top | 73.7 | 988 | |
| committed, `Vector zero` local | 58.3 | 980 | |
| committed, hoisted `Vector &dock`/`&pull`/`&drift` | 87.6 | 968 | identical code |
| committed, `objects->data_[i]` / `at(i)` / `count` | 87.6 | 968 | identical code |
| committed, surface block forms (`const&`, `bool hit`, `==0`, direct temp) | 87.6 | 968 | identical code |
| committed, A1/A2 spelling variants (`>=`, `!(<)`, `size-i<1`, `+0`) | 87.6 | 968 | identical code |
| committed, extra cold block `if (size()==0) continue;` + G | 85.0 | 976 | |
| committed, extra `(void)size();` cold use + G | 82.6 | 968 | no allocation change |

The full 40-cell A1/A2/G/extra-anchor matrix reproduces the pass-12 table:
the only 87.6 cells are A1+A2 with no real guard (and the no-op spelling
variants); every cell with the real guard is ≤85.0.

## 3. Why the oracle does not close the gap

The oracle's nested shape is the *real source*, but it produces the wrong
register file on this compiler:

| | score | frame | this | object | dock | i | surface | guard |
|---|---:|---:|---|---|---|---|---|---|
| original | 100 | 112 | r5 | r8 | r9 | fp | r6 | `[sp,#24]` |
| oracle nested | 64.1 | 120 | r5 | r9 | sl | fp | r8 | r6 (also stored `[sp,#24]`) |
| committed | 87.6 | 112 | r5 | r8 | r9 | fp | r6 | `[sp,#24]` |
| wrap + A2 | 85.5 | 112 | r5 | r8 | r9 | fp | r6 | `[sp,#24]` |

The nested shape's `Vector surface` is declared in the `if (object !=
nullptr)` scope, so its `lifetime.end` is at the end of the loop body and
stack colouring cannot share it with the `sp+32` velocity/Matrix scratch
(frame 120). The committed shape scopes `surface` to the
`{ … if (!outerCollide) continue; }` block, so it shares `sp+32` (frame
112) — but that shape needs the two redundant `objects->size()` anchors to
keep `dock=r9, i=fp`, and the anchors rotate the latch to the front.

The wrap+A2 shape is the closest *structure*: latch at `0x9f38c` like the
original, `dock=r9, i=fp, surface=r6`, canary `[sp,#24]`. It is 8 B too
long (976 vs 968) because the wrap (8 B) and A2 (6 B) are not in the
original, and it still misses the preheader `str.w r9,[sp,#20]` and the
collision-arm shuffle. Removing the wrap re-rotates the loop; removing A2
swaps `dock`/`i`. No source spelling found this wave gets both without the
extra block.

## 4. Proven vs. hypothesis

**Proven (measured):**

* The new oracle's `calcCollision` is the nested
  `if (object != nullptr) { if (outerCollide) { if/else-if/else } }` shape,
  with `Vector &pull = this->moveTarget` hoisted before the loop.
* The Android binary is top-tested and un-rotated (latch after
  `hitCamera`), with one preheader dock spill and the real collision-arm
  `i == 0 && field_0x71` guard.
* The committed two-anchor shape is still the best official score
  (87.6 % / 968 B); the oracle-faithful shape scores 64.1 % / 968 B.
* ~200 new builds (loop forms, `__builtin_expect`, arm order, cold-block
  forms, declaration placement, anchor spellings) do not beat 87.6.

**Hypothesis (not proven):**

* The original source is the oracle's nested shape, and the
  `dock`/`i` assignment plus the latch position are decided by
  `RAGreedy`/`MachineBlockPlacement` tie-breaks on this clang 7.0.2 `-Oz`
  build. The two attractors (right registers + rotated latch vs right
  latch + swapped registers) appear mutually exclusive from source alone.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf311
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch (not committed): `/dev/shm/wf311/` — `try.sh` (splice + build +
score), `score.py` (single-function official score), `mnem.py`
(mnemonic-only diff), `body_current.txt`, `body_oracle.txt`, and the
`v*/` variant directories.

## 6. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`. `RATCHET
PASS`.
