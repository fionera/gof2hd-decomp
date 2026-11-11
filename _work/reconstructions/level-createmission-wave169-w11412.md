# w11412 (wave 169): `Level::createMission` — local-scope / temporary-lifetime
# sweep, convoy-switch tail re-check (no raw win; 5,435 / 767 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11412`, branch
`wave169-w11412`, base master `ec3ceb07c`.  Continues
`level-createmission-wave168-w11311.md` (guard bool sweep),
`level-createmission-wave168-w11312.md` (convoy block layout) and
`level-createmission-wave166-w11111.md` (frame map).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Convoy `switch (k)` — the original is a 4-way GPR tail, re-derived

Raw byte diff regions (36 merged, 5,435 bytes): the five big ones are
`0x1441..0x1a26` (1,509, convoy switch), `0x2e2c..0x335a` (1,326, guard +
station loop), `0x3376..0x3512` (412, alien-gun loop), `0x3797..0x3c96`
(1,279, raid block), `0x3e02..0x4312` (1,296, blueprint/nameLeader).  The
other 31 are 1-27 byte sp-slot operand deltas.

Original switch bodies (literal pool at `0xb15bc`):

```text
case0 b1420: ldr -3000  -> [376] (=offset[1]); ldr 20000 -> [372] (=offset[0]); ldr -50000 -> tail
case1 b142c: ldr -7000  -> [376];              ldr 10000 -> [372];              ldr -30000 -> tail
case2 b1438: ldr 25000  -> [372]; str.w sl(0) -> [376];                          ldr -36000 -> tail
default b1444: ldr 6000 -> [376];              ldr 12000 -> [372];              ldr -45000 -> tail
tail  b144e: str r1,[sp,#380] (=offset[2])   <-- shared by ALL FOUR cases (GPR)
```

i.e. every case has machine store order `offset[1], offset[0], offset[2]`
and the merged tail is `str r1` (GPR).  Ours (HEAD, default source order
`2,0,1`) has the default body as machine `offset[0], offset[2], offset[1]`
so only the three real cases share the tail and `case1` falls through it
(the known +2 shift).

## 2. Default store-order re-sweep (all `link_so -j2`, `fn_diff`)

| default source order | size | raw | aligned | note |
|---|---:|---:|---:|---|
| `2,0,1` (HEAD) | 17172 | **5435** | **767** | 3-way tail, case1 falls through |
| `2,1,0` | 17172 | 5438 | 764 | machine `1,2,0`, no tail merge |
| `0,2,1` | 17172 | 5441 | 767 | machine `2,0,1`, no tail merge |
| `1,2,0` | 17172 | 5442 | 762 | machine `2,1,0`, no tail merge |
| `1,0,2` | 17188 | 11346 | 805 | 4-way tail but value in `s0` (`vmov`+`vstr`), +16 B |
| `0,1,2` | 17164 | 11330 | 838 | `SwitchToLookupTable` fires (`vldr s4,[r1,#-40]`), -8 B |

New detail: with `1,0,2` the four `offset[2]` constants are hoisted into
`s16/s18/s20/s22` (`vldr s16,[pc]` …) before the loop and the tail is
`vstr s0,[sp,#380]`; the original keeps them as GPR `ldr r1,[pc]` and
`str r1`.  So the 4-way merge is reachable but the register class is not.

`case 3:` + `default:` variants (to perturb the lowering) also measured:
`case3`/`default` body `1,0,2` -> 17188 / 11346 / 805; `case3` body then
empty `default` -> 17188 / 11350 / **665** (best aligned seen, but +16 B);
body `0,1,2` -> 17164 / 11330 / 838 (lookup table).  None size-exact.

## 3. `int offset[3]` (integer stores) — size-exact but raw +16

Replacing `float offset[3]` with `int offset[3]` and storing the float bit
patterns as hex ints gives **17172 / 5451 / 788** (size-exact).  The
switch then uses `ldr r1`/`str r1` (GPR) but the compiler strength-reduces
some constants to `add.w r1,r1,#0x224000` (25000.0f) and `#0x1f4000`
(12000.0f), so the bodies still do not match.  Not a win.

`Vector offset;` with `offset.x/.y/.z` and `setPosition(offset)`:
17176 / 10732 / 801 (default-ctor zero stores survive, +4 B).

## 4. Scope / lifetime / declaration sweep (all neutral or worse)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD | 17172 | **5435** | **767** |
| remove the extra `{` after `if (mission->isEmpty())` | does not compile (goto bypasses init) | | |
| `stationPirates` `unsigned` -> `int` | 17172 | 5435 | 767 |
| brace the `extraAliens` if-body | 17172 | 5435 | 767 |
| `bool raid = false; if (!(intro && !hard)) raid = ...` | 17172 | 5435 | 767 |
| `bool raid; if (...) { } else { }` (braced) | 17172 | 5435 | 767 |
| `bool raid = ...; if (intro && !hard) raid = false` | 17208 | 16409 | 1764 |
| friend loop `unsigned i` | 17172 | 5437 | 767 |
| friend loop `unsigned i` + station loop `unsigned` | 17172 | 5437 | 767 |
| station loop `unsigned i` | 17172 | 5436 | 767 |
| jumper loop `unsigned i` | 17172 | 5438 | 767 |
| `i == 0 && hasWanted` (operand swap) | 17164 | 12476 | 1499 |
| `i > wingmen \|\| !hasWanted` (operand swap) | 17168 | 16024 | 2289 |
| `int i` hoisted before friend loop | 17172 | 5435 | 767 |
| `Status *status = Globals::status` (drop volatile) | 17172 | 5443 | 770 |
| `int hard = status->hardCoreMode()` | 17172 | 5435 | 767 |
| `raidType` declared before `status` | 17172 | 5447 | 767 |
| `hard` declared before `raidType` | 17172 | 5447 | 764 |
| move `int guns` after the `raidRace==8` block | 17172 | 6376 | 1362 |
| `bool alienWave` (w11311 re-check) | 17156 | 12403 | 818 |
| `bool alienWave` + `bool pirateFortress` | 17168 | 15965 | 3334 |
| all three wave flags `bool` | 17180 | 15695 | 2033 |

## 5. Root cause of the remaining single-byte deltas (sp-slot rotation)

The 31 small regions are sp-slot operands.  The rotation is confirmed
again: original `sp+40 fortressGuns, sp+44 extraAliens, sp+48 stationPirates`;
ours `sp+40 extraAliens, sp+44 stationPirates, sp+48 fortressGuns`
(`alienGuns` sp+52 in both).  First-store order is identical in both
builds (`stationPirates` 0x6dc < `extraAliens` 0x870 < `fortressGuns`
0x2a7x), so the mismatch is set by StackSlotColoring interference/colour
order, not by any declaration order reachable from source.  `sp+84` =
`raid` (17 refs) and `sp+88` = `__stack_chk_guard` (2 refs) are exact in
both; the pair is not the cause of any residual delta.

## 6. Conclusion

No size-exact shape reduced raw below 5,435.  The convoy switch's 4-way
GPR tail is the single biggest prize (~1,500 raw bytes) but the only
source orders that produce the 4-way merge either change the size
(`1,0,2` +16, `0,1,2` -8) or pick FP registers for the merged value.
The remaining deltas are the known block-order/scheduling residue plus
the `fortressGuns/extraAliens/stationPirates` colour rotation.

Nothing landed: tree left clean at the baseline (5,435 / 767 / 17,172 B),
`check` RATCHET PASS (byte 4507, linked 4507, size 4524/4524, data 843,
imports 207, lint CLEAN, parity 5370/5370).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11412/{orig.dis,ours.dis,regions.py,ctx.py,adiff.py,
adiff.txt,setdef.py,var.py,var2.py,var3.py,var4.py,var5.py,var6.py,var7.py,
var8.py,var9.py,var10.py,var11.py,measure.sh,Level.cpp.base}`.
