# `Level::createShip` — wave 126 (w7604): the cleanup exception-pointer live range is the RA root cause

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `bf365a964` (wave-125 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7604`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The checked-in body stays
the best at **70.5000 %**, 2764 B, 1006 insns, exact 336 B / 57-row / `ttype
0xff` cleanup-only LSDA.  New finding: the real-`new` exception pointer is a
PHI that the register allocator keeps in **r6 across the whole function**,
because the landing-pad blocks are interleaved in the pre-RA MIR order.  That
is the register that the original uses for `shipClass`'s third piece.

## Result

```
                                   original        HEAD (bf365a964)
createShip fuzzy (official)        —               70.5000
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. The real root cause: the cleanup exception pointer

The four cleanup pointers (`Player` r5, `PlayerFixedObject` r9,
`Array<BoundingVolume*>` fp, `PlayerFighter` sl) are not the only long-lived
callee-saved values.  The **exception pointer PHI** is.

`-mllvm -print-machineinstrs` on the HEAD body shows the common resume block
(`bb.91`, `%ir-block.533`) as a PHI of the exception pointer from every
landing pad:

```
bb.91 (%ir-block.533):
; predecessors: %bb.14, %bb.32, %bb.33, %bb.83, ... %bb.21
  %125:gpr = PHI %31:gpr, %bb.14, %45:gpr, %bb.32, ...
  $r0 = COPY %125:gpr
  tBL @_Unwind_Resume
```

and the first landing pad (`bb.14`, `%ir-block.113`, the `new Player`
cleanup) is emitted **early**, right after the `new Player` invoke:

```
bb.14 (%ir-block.113, landing-pad):      ; MIR byte offset 4336
  %235:gpr = COPY killed $r1
  %234:gpr = COPY killed $r0
  $r0 = COPY %27:gpr
  tBL @_ZdlPv
  %32:gpr = COPY %235:gpr
  %31:gpr = COPY %234:gpr
  t2B %bb.91
```

`bb.91` is at MIR byte offset 26304.  So the PHI lives from 4368 to 26352 —
almost the whole function.  The register allocator therefore gives it a
callee-saved register (**r6**) and it stays there.  In the final code the
exception pointer is moved to r6 at every landing pad:

```
c0bbe  mov r6, r0        ; landing pad (r9 cleanup)
c0bc6  mov r6, r0        ; landing pad (fp cleanup)
c0bcc  mov r6, r0        ; landing pad (r5 cleanup)
c0bd4  mov r0, r6
c0bd6  blx _Unwind_Resume
```

The original instead moves the exception pointer to **r8** at every landing
pad, and r8 is free there because `z` died at `c041e`:

```
c0bba  mov r8, r0        ; landing pad (sl cleanup)
c0bc0  mov r8, r0        ; landing pad (fp cleanup)
c0bc6  mov r8, r0        ; landing pad (r9 cleanup)
c0bcc  mov r8, r0        ; landing pad (r5 cleanup)
c0bd4  mov r0, r8
c0bd6  blx _Unwind_Resume
```

r6 in the original is instead the `shipClass` third piece (`c02b4`..`c03fc`)
and then the BoundingAAB zero constant (`c04a2`..).  Freeing r6 is exactly
what lets `shipClass` stay in registers (r5 -> sl -> r6) and `type` stay in
fp, which is the whole prologue difference.

## 2. Why the original's exception pointer is short-lived

The original's landing pads are **in reverse allocation order and all at the
end of the function** (`c0b8a`..`c0bd6`, after the main `pop` at `c0b88`):

```
c0b8a..c0bb8  23 x b c0bcc   ; BoundingAAB cleanups -> player cleanup
c0bba         mov r8,r0; mov r0,sl; b c0bd0   ; fighter cleanup
c0bc0         mov r8,r0; mov r0,fp; b c0bd0   ; volumes cleanup
c0bc6         mov r8,r0; mov r0,r9; b c0bd0   ; fixed cleanup
c0bcc         mov r8,r0; mov r0,r5            ; player cleanup
c0bd0         blx operator delete
c0bd4         mov r0,r8
c0bd6         blx _Unwind_Resume
```

Reverse allocation order (BoundingAABs last allocated first, player first
allocated last) means the cleanups are popped at the **end of the function**,
so the landing-pad blocks are emitted at the end and the exception pointer
lives only across the adjacent cleanup block.

HEAD's landing pads are in **allocation order** (player first, then fighter,
fixed, volumes, BoundingAABs): each `new` cleanup is popped at the end of its
own full-expression, so the first landing pad (`new Player`) is emitted
early, and its PHI spans to the common resume.  The final code layout is the
same as the original (landing pads at `c0b8e`..`c0bd6`), but the RA sees the
interleaved pre-RA MIR order.

## 3. What was tried this pass

All rows keep the exact 336 B / 57-row / `ttype 0xff` LSDA unless noted.

| variant | % | size | insns | note |
|---|---:|---:|---:|---|
| **HEAD (wave-124 body)** | **70.5** | 2764 | 1007/1006 | best |
| int x, int y, int z (faithful coords), guard | 61.5 | 2772 | 1007/998 | |
| int xyz + two-call MIN clamps, no guard | 60.8 | 2792 | 1007/1009 | |
| float x, float y, float z | 57.5 | 2776 | 1007/1003 | |
| `int x,y,z` hoisted to top, assigned later | 61.5 | 2772 | 1007/998 | |
| `int cls = shipClass` copy (top / after switch) | 70.5 | 2764 | 1007/1006 | no change |
| `ship` / `player` hoisted to top | 70.5 | 2764 | 1007/1006 | no change |
| guard moved after branch / before Player / before setEmpData | 63.4 / 31.9 / 63.1 | 2760/2776/2776 | | worse |
| all 6 x/y/z declaration orders (int x, float y, float z) | 70.3–70.5 | 2764 | | zyx/xzy/zxy tie at 70.5 |
| all 8 int/float combinations of x,y,z | 57.5–70.3 | 2764–2788 | | no win |
| `try { ... } catch (...) { }` around the body | 41.9 | 2796 | 1007/1027 | moves landing pads to end, **ttype 0x00**, +72 B extab |
| `try { ... } catch (...) { throw; }` | 23.7 | 2808 | 1007/1031 | **ttype 0x00**, +88 B extab |
| `try { ... } catch (...) { __builtin_unreachable(); }` | 42.3 | 2792 | 1007/1026 | **ttype 0x00**, +72 B extab |

The `try` variants confirm the mechanism: they move the landing pads to the
end and the prologue becomes `str r1,[sp,#56]; mov r4,r0; ... mov r9,r3;
mov fp,r2` (both `type` and `shipClass` kept in callee-saved registers), but
the LSDA ttype changes from `0xff` to `0x00` (catch-all type table) and the
catch handler adds ~20 instructions, so the fuzzy score collapses.

## 4. Remaining lever

A source shape that makes clang pop all four `new` cleanups at the end of the
function **without introducing a catch-all type table** would move the
landing pads to the end, free r6, and let `shipClass`/`type` occupy r5/sl/fp
like the original.  A plain `try`/`catch` cannot do it (ttype 0x00).  The
original's reverse-order landing pads prove the cleanup scope is the whole
function, but no C++ source spelling tried so far reproduces that with a
cleanup-only (`ttype 0xff`) LSDA.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

MIR (landing-pad order and PHI live range):

```
<clang++> ... -c src/game/world/Level.cpp \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7604_*` (one-function scorer,
variant generator, LSDA parser).
