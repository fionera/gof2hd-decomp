# w10410 (wave 158): `Level::createMission` — KVBR alien-block declaration
# order (this reload into r4 at b3232); raw 5,487 -> 5,485, aligned 774 -> 795

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10410`, based on master
`9e4795206` (wave 157). Continues `level-createmission-wave156-w10310.md`,
`…-wave157-w10309.md`, `…-wave154-w10012.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10410/fdiff.py`; instruction-index alignment is
`/dev/shm/w10410/align_diff.py` (worktree-local copies, `OURS` pointed at this
worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,529   .ARM.extab 1,292   .rel.plt 0 (byte-identical)
```

## 1. Landed change (`src/game/world/Level.cpp`, alien-gun block)

Reordered the four alien-block declarations from `rotation, v, k, base`
(`RVKB`) to `k, v, base, rotation` (`KVBR`):

```diff
     if (alienGuns != 0) {
-        Matrix rotation;
-        Vector v = {0.0f, 0.0f, 0.0f};
         int k = 0;
+        Vector v = {0.0f, 0.0f, 0.0f};
         Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
+        Matrix rotation;
```

This reproduces the original's preheader `ldr r4,[sp,#124]` (this reload)
*before* the rotation identity init, i.e. at `0xb3234` vs the original
`0xb3232` (2-byte shift).  It is the first source shape that moves the
preheader `this` reload into `r4`; the loop body is still `this=fp`.

## 2. Register map (alien loop)

```text
        this        i            gun   k*24   t     v/sret  rotation/p2  temp
ORIG    r4(reload)  fp           r6    r9     sl    r5      r4 (reused)  r8
KVBR    fp          r4(sp+116)   r4    r6     r5    r9      sl           r8
BASE    fp          r4(sp+116)   r4    r6     r5    r9      sl           r8
```

The original keeps `i` in `fp`, rematerialises `this` from `sp+124` at the
loop entry (`0xb3232`) and tail (`0xb336e`), and reuses `r4` for
`&rotation`/`&p2`.  Ours keeps `this` in `fp` across the whole loop, spills
`i` to `sp+116`, and gives `&rotation` its own register (`sl`).  `gun`,
`k*24`, `t`, `v`, `temp` are a cyclic permutation of the target.

Why: ours has 8 values competing for the 7 callee-saved registers
(`r4,r5,r6,r8,r9,sl,fp`); the allocator spills the loop-carried `i`.  The
original has 7 values (this, i, gun, k, t, v, temp) and rematerialises `this`
and the rotation address into `r4`, so nothing spills.  Both `this` and `i`
have 4 uses in the loop body; the tie is broken by the rematerialisation of
`this` (a stack load) in the original.

## 3. Variants measured this wave (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5487 / 774`).  `this/i` is the
register map of the alien loop.

| variant | raw | align | size | this/i |
|---|---:|---:|---:|---|
| **HEAD (RVKB)** | **5487** | **774** | 17172 | fp / r4(spill) |
| **landed: KVBR** | **5485** | **795** | 17172 | fp / r4(spill) |
| KVRB | 5485 | 792 | 17172 | fp / r4(spill) |
| KVBR + `Level *self = this` (block) | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Level *self = this` (loop) | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Level *self` used in raid | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Status *&statusref = Globals::status` | 5493 | 798 | 17172 | fp / r4(spill) |
| KVBR + `Status **statusp = &Globals::status` | 5493 | 798 | 17172 | fp / r4(spill) |
| KVBR + `int fg = fortressGuns` (raid+alien) | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `int fg` before `fortressGuns` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `int *counts = &this->friendCount` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `int n = alienGuns` bound local | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `const int n` bound local | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `(i<0)+(i<0)` hack | 5674 | 818 | 17172 | fp / r4(sp+120) |
| KVBR + `(i<0)*2` hack | 5653 | 800 | 17172 | fp / r4(spill) |
| KVBR + `(i<0)\|(i<1)` hack | 5803 | 842 | 17180 | — |
| KVBR + `k++, i++` / `i+=1,k+=1` / `++i,++k` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `gun->cargo` before `field_0x70` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `__builtin_expect(alienGuns!=0,1)` | 5683 | 775 | 17172 | r5 / r4(spill) |
| KVBR + `__builtin_expect` on loop cond | 15853 | 2310 | 17196 | — |
| KVBR + `__builtin_expect` on table select | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + table select as `if/else` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `unsigned i` | 6594 | 1272 | 17140 | — |
| KVBR + named `Vector p` first rotate | 6593 | 1252 | 17196 | — |
| KVBR + both rotates unnamed | 6582 | 1226 | 17168 | — |
| KVBR + `KIPlayer *gun = createStaticObject()` | 6240 | 822 | 17164 | — |
| KVBR + drop early `cargo` | 6451 | 809 | 17164 | — |
| KVBR + drop `field_0x70` | 6247 | 734 | 17164 | — |
| KVBR + drop both early stores | 5821 | 870 | 17180 | — |
| `enemies` local before loop | 6492 | 1217 | 17164 | r5 / r4(spill) |
| `int *counts` bound only (RVKB) | 5487 | 774 | 17172 | fp / r4(spill) |

Key findings:

1. **No source shape found this wave puts `this` in `r4` in the loop body.**
   Every 17,172-B shape keeps `this` in a callee-saved register (`fp` or `r5`)
   and spills `i` (or `sl`).  The allocator's choice is a tie-break between
   `this` (4 loop uses, rematerialisable stack load) and `i` (4 loop uses,
   loop-carried phi); the original spills `this`, ours spills `i`.
2. KVBR is the only size-exact variant that moves the preheader `this` reload
   into `r4` (matching `0xb3232`).  It is a 2-byte raw win but a 21-instruction
   aligned regression (`extra-orig:ldr` 19 -> 22, `extra-ours:ldr` 16 -> 17):
   the moved reload creates extra loads around the rotation init.
3. `Level *self`, `Status *&`, `Status **`, `int fg`, `int n`, `int *counts`
   are all canonicalised away (identical codegen).  Adding uses of `i` (the
   `(i<0)` hack variants) does not flip the allocation.
4. The `__builtin_expect(alienGuns!=0,1)` hint moves `this` to `r5` (one step
   toward the target's low register) but costs +198 raw / -20 aligned.
5. `drop field_0x70` reaches aligned 734 but is 8 B short (17,164) and raw
   6,247; the missing 8 B of genuine code is still unidentified.

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 5485   (-2)
                           align  774 -> 795    (+21)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,529 -> 85,527  (-2)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,292 -> 1,292
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.ARM.exidx 0 -> 0
.note.gnu.build-id         20 -> 20
file size             2,187,328 -> 2,187,328 (exact)
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

`check` is green.

## 5. Open questions

1. `this` must be spilled/rematerialised in the alien loop so `r4` is free for
   `&rotation`/`&p2` and `i` can take `fp`.  The RA tie-break favours `this`
   because its loop interval is one long value; the original splits it into a
   short preheader interval and a short loop-tail interval.  A source shape
   that forces the split (e.g. a genuine second load of `this`) is unknown.
2. The missing 32 B of genuine code (removing the `(i<0)` hack shrinks the
   function) is still unidentified; `drop field_0x70` at 17,164 is 8 B short.
3. The original's `ldmia` bound loads and branch (`cbz`) table select are
   reproduced by ours only after the post-RA LDM formation; the pre-RA choice
   is not source-controllable from the forms tried.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10410/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10410/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10410/secdiff.py
```

Scratch: `/dev/shm/w10410/{fdiff.py,align_diff.py,secdiff.py,gen.py,gen2.py,
batch.py,batch2.py,variants.py,sweep.py,Level.orig.cpp,alien_base.txt,
body_*.txt,mir.txt}`.
