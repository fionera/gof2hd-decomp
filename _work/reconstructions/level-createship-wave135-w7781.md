# `Level::createShip` — wave 135 (w7781): the untried non-EH scheduling
# boundaries (VLA / `__builtin_alloca` between the coordinates and
# `setEmpData`, `try` around `new Player`) all lose; the exact 336 B LSDA
# and 2764 B survive only in the plain body

Status: **note-only** (no source change landed; gate stays byte 4506 /
linked 4506, `.ARM.extab` 156,312 B exact).  Worktree `wt/w7781`, base
master `bbffef482` (wave-134 snapshot).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Thumb entry `0xc0215`, 1007 insns).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.

| | baseline |
|---|---:|
| function score | **70.6 %** (1007 vs 1005 normalised insns) |
| function size | 2764 B |
| `.ARM.extab` | 156,312 B (exact; 336 B / 57-row / ttype 0xff record) |

## 0. Summary (new results this pass)

This pass closed the two explicitly-untried items from the w7737/w7761 open
lists.  Both fail to keep the 2764 B / exact-LSDA constraints.

1. **A VLA (SP-defining instruction) between the coordinate block and
   `setEmpData` does not pay.**  `TargetInstrInfo::isSchedulingBoundary`
   returns true for any instruction that defines the stack pointer, so a
   dynamic `alloca`/VLA is a scheduling barrier exactly like `EH_LABEL` —
   but it costs real code and a dynamic frame:
   | variant | score | usize | insns |
   |---|---:|---:|---:|
   | `volatile int vn = hostile ? 1 : 2; int vla[vn]; vla[0] = x;` before `setEmpData` | 52.5 | **2848** | 1007/1018 |
   | `int vla[hostile ? 1 : 2]; vla[0] = x;` before `setEmpData` | 70.6 | 2764 | 1007/1005 |
   The second form is folded away completely (the array is dead after
   `vla[0] = x`), so it is byte-identical to master — the VLA only survives
   when its size is genuinely runtime-opaque (`volatile`), and then it adds
   84 B.  There is no middle ground at -Oz.

2. **`try` around `new Player` (already an invoke) instead of around
   `setEmpData` re-measured with the exact forms** (the w7761 note listed
   only `catch (...) { return nullptr; }`; the other catch bodies are new):
   | variant | score | usize | extab |
   |---|---:|---:|---:|
   | `try { player = new Player(...); } catch (...) { return nullptr; }` | 62.1 | 2788 | 156,328 |
   | `try { ... } catch (...) { throw; }` | 39.8 | 2796 | 156,340 |
   | `try { ... } catch (...) { }` | 59.5 | 2808 | 156,328 |
   | `try { ... } catch (...) { player = nullptr; }` | 59.4 | 2808 | 156,328 |
   Every one grows `.ARM.extab` by 16-28 B (an extra landing pad row) and
   none keeps 2764 B, so none can land on size grounds.

3. **The original really computes all three ints before `setEmpData` and
   converts at the ctor call sites.**  Confirmed again from the original
   disasm `0xc03c8..0xc03ee`:
   ```
   c03c8 ldr r2,[sp,#48]  ; wy      c03d2 ldr r3,[sp,#40]  ; wx
   c03ca ldr r3,[sp,#32]  ; jitterY c03d4 ldr r4,[sp,#28]  ; jitterX
   c03cc ldr r0,[sp,#44]  ; wz      c03d6 add r0,r1        ; wz+jitterZ
   c03ce ldr r1,[sp,#36]  ; jitterZ c03d8 movw r1,#20000
   c03d0 add r2,r3        ; wy+jitterY
   c03dc sub.w fp,r0,r1   ; z -> fp   c03e0 add r3,r4      ; wx+jitterX
   c03e2 subs r4,r2,r1    ; y -> r4   c03e4 sub.w r8,r3,r1 ; x -> r8
   c03e8 mov r0,r5 / c03ea mov r1,sl / c03ec mov r2,r9
   c03ee blx Player::setEmpData(int,int)
   ```
   The float conversions are `vcvt.f32.s32` at `0xc040c..0xc0422` (capital
   branch) and `0xc06c0..0xc06d8` (fighter branch).  Master sinks y and z
   below the call (`0xc03c0..0xc03e0`) and re-derives x from the spilled
   slots — the live-range change that moves Greedy RA off `type -> fp`,
   `shipClass -> r5/sl`.
   Also re-confirmed: the original has **no** `Globals::status == nullptr`
   guard — the first instruction after `setEmpData` is `mov r9,#0; cmp
   r6,#0` (i.e. `ship = nullptr; if (shipClass == 0)`).

4. **Dropping the (unfaithful) status guard does not recover 2764 B.**  The
   original has no `Globals::status == nullptr` guard, so it was removed in
   five shapes; none lands on 2764 B:
   | variant (guard removed) | score | usize |
   |---|---:|---:|
   | plain `float z,y; int x;` | 63.6 | 2748 |
   | all-`int` x/y/z | 59.6 | 2776 |
   | `Vector pos` + `int x`/`float y,z` | 55.6 | 2776 |
   | coords before `new Player` | 60.8 | 2776 |
   | `__sync_synchronize()` before `setEmpData` | 63.2 | 2752 |
   So the original's extra 16 B is *not* the guard and is not one of the
   obvious coordinate spellings; it is still unidentified.

## 1. Why the remaining gap is pure RA

The prologue is where the two diverge (first difference at instruction 5):
```
original: str r1,[sp,#60] (race)  mov fp,r3 (type)  mov r5,r2 (shipClass)
master  : str r2,[sp,#60] (shipClass)  str r1,[sp,#52] (race)  mov sl,r3 (type)
```
Both are 2764 B; master spends 16 B on the (unfaithful) status guard that
the original spends elsewhere.  The only shape known to reproduce the good
allocation is the w7693 `placement new + try` body (73.4 %), which forces a
328 B / 54-row LSDA and breaks `.ARM.exidx`; the only shape known to keep
the exact 336 B / 57-row cleanup-only LSDA is the plain body (70.6 %).

## 2. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7781/score.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (not committed): `/dev/shm/w7781/try_cs.py` (the variant harness:
patches the `Player *player = ...` … `if (Globals::status == nullptr)` block,
builds, scores, restores), `score.py`, `relink.sh`.

## 3. Open items for the next wave

1. The 16 B the original spends instead of the status guard is still
   unidentified.  Candidates: a genuinely faithful extra local (the original
   uses `[sp,#52]` for the spilled `type` and `[sp,#56]` for the spilled
   `this`, while master's `[sp,#52]` holds `race`), or a second
   `getLevel()`/`getCurrentCampaignMission()` call somewhere.
2. A non-`EH_LABEL` scheduling boundary that survives `-Oz` *and* costs
   nothing: a VLA is the only SP-defining instruction available in plain
   C++, and it always costs >= 20 B; `DBG_LABEL`/`CFI_INSTRUCTION`/
   `GC_LABEL` are not reachable from C++.
