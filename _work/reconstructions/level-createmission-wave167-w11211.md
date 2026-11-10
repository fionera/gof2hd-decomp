# w11211 (wave 167): `Level::createMission` — scope / temporary-lifetime /
# spill-slot angle (no raw win; 5,436 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11211`, base `d00057e6d`.
Continues `level-createmission-wave166-w11111.md` (frame map + phi/block/decl
sweep) and `level-createmission-wave164-w11010.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5436   align 767   insn 6204/6197
```

## 1. Spill-slot pair decoded exactly (confirms w11111, refines its labels)

Per-offset `[sp,#N]` reference *kinds* (store = slot definition site):

```text
        ORIGINAL                     OURS
sp+40   fortressGuns (st 0x2a86)    extraAliens   (st 0x0870)
sp+44   extraAliens  (st 0x0870)    stationPirates(st 0x06dc)
sp+48   stationPirates(st 0x06dc)   fortressGuns  (st 0x2a76)
sp+52   alienGuns    (st 0x2a8a)    alienGuns     (st 0x2a7a)
```

So the three-way slot order is a pure rotation:
orig (low->high) `fortressGuns, extraAliens, stationPirates`;
ours `extraAliens, stationPirates, fortressGuns`.  `alienGuns` is sp+52 in
both.  The *first-store* order is identical in both builds
(stationPirates 0x06dc < extraAliens 0x0870 < fortressGuns ~0x2a7x), so the
difference is the frame-object index (regalloc spill order), not source
order — this is why the w11111 declaration-order sweep was neutral.

`sp+84` = `raid` (17 refs) and `sp+88` = `__stack_chk_guard` (2 refs) are
byte-for-byte correct in both builds; nothing to move there.

## 2. `sp+92` is `guns`, not `end`

w11111 labelled `sp+92` as `end`.  In the original, every `[sp,#92]` use is
`guns` (`int guns = alienGuns + fortressGuns`): 0x39b0 store, 0x3aac/0x3e04/
0x3e8a loads in the `stationPirates` / `extraAliens` / `convoy` index sums.
`end` shares the slot (non-overlapping live range) but the original's
alien-gun loop bound is **recomputed**, not read from the slot:

```text
b3384 ldmia r2,{r0,r1,r2}      ; friendCount, field_164, field_168
b3386 ldr.w sl,[sp,#52]        ; alienGuns
b338a add.w r3,r0,sl
b338e add   r3,r1
b3390 add   r3,r2
b3392 cmp   fp,r3
b3394 blt
```

i.e. one comparison.  Our source's `(i < end) & (i < dup)` produces two
(`cmp; itttt lt; addlt x3; cmplt`) — 2 extra instructions.

## 3. Sweep (all `link_so -j2`, measured with `tools/fn_diff.py`)

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| HEAD (baseline) | 17172 | **5436** | **767** | best |
| alienWave `&&` -> `&` only | 17184 | 12411 | 2564 | +12 B |
| pirateFortress `&&` -> `&` only | 17192 | 13342 | 1369 | +20 B |
| both `&&` -> `&` | 17188 | 14538 | 1740 | +16 B |
| swap alienGuns/fortressGuns decl | 17172 | 5436 | 767 | neutral |
| hoist alienGuns/fortressGuns decl | 17172 | 5436 | 767 | neutral |
| convoy switch source order (0,2,1) | 17172 | 5436 | 767 | neutral |
| convoy switch source order (2,1,0) | 17172 | 5437 | **762** | -5 aligned, +1 raw |
| convoy switch source order (2,0,1) | 17172 | 5437 | 762 | +1 raw |
| convoy switch source order (1,0,2)/(1,2,0) | 17172 | 5442 | 769 | worse |
| convoy `default` stores y,x,z (orig pool order) | 17188 | 11346 | 805 | +16 B |
| drop `end`, loop `i < dup` only | 17172 | 5634 | **690** | -77 aligned, +198 raw |
| keep `end`, loop `i < end` only | 17156 | 6505 | 1395 | -16 B |
| swap `&` operands in alien-gun loop | 17172 | 5436 | 767 | neutral |
| alien-gun loop `&&` | 17280 | 16665 | 2231 | +108 B |
| `bool hard` -> `int` | 17172 | 5436 | 767 | neutral |
| `bool resetStation` -> `int` | 17172 | 5436 | 767 | neutral |
| `bool hasWanted` -> `int` | 17148 | 12589 | 855 | -24 B (matches w11111) |
| `bool nameLeader` -> `int` | 17180 | 13440 | 870 | +8 B |
| `bool siegedStation` -> `int` | 17352 | 16541 | 2635 | +180 B |
| `bool raid` -> `int` | 17172 | 5664 | 779 | +228 raw |
| `(security == 3) & raid` -> `&&` | 17196 | 14788 | 1015 | +24 B |
| `int extraFriend;` -> `= 0` | 17172 | 5436 | 767 | neutral |
| `pirateEscorts` -> `unsigned` | 17172 | 5437 | 768 | +1 |
| `alienPercent` -> `unsigned` | 17172 | 6031 | 797 | +595 |

## 4. Conclusion

* The frame is fully decoded: the only slot-order mismatch is the
  `fortressGuns/extraAliens/stationPirates` rotation, and it is set by the
  register allocator's spill order, not by any source declaration order we
  can reach (7 permutations tried, all neutral).
* The `(i < end) & (i < dup)` in the alien-gun loop is a deliberate size
  anchor: dropping the redundant comparison is size-exact and improves
  aligned by 77 (767 -> 690) but costs +198 raw, because the loop then keeps
  `alienGuns` in a register (`add.w r3,r0,r8`) instead of reloading it from
  `[sp,#52]` as the original does.
* The three big raw regions remain block-order artefacts
  (convoy switch 0x1440-0x1a25, alien-wave predicate 0x2e2c-0x3359, and the
  blueprint/alien index sums 0x377a-0x4311).  Source case reordering does not
  move them: clang canonicalises the switch to `case0, case2, case1,
  default` while the original is `case0, case1, case2, default`.
* Nothing landed; tree left clean at the baseline.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w11211/{orig.dis,ours.dis,regions.py,adiff.py,spmap.py,
spmap2.py,spone.py,rd.py,measure.sh,try.py,v1..v9.py,Level.cpp.base}`.
