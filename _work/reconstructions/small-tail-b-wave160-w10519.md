# Small tail B wave 160 (w10519): SpaceLounge clamp-colouring sweep,
# Generator exact-block landing-pad blocker, `.ARM.extab` second record

Status: **note only, no `src/` change**.  Worktree `wt/w10519`, base master
`cfde1f5f2` (wave 158).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10519`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  All 27 section sizes unchanged;
file size 2,187,328 B unchanged.

Metrics: raw same-address diff bytes (`/dev/shm/w10519/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10519/align_diff.py`).

| fn | before raw/aligned/size | best measured this wave |
|---|---:|---:|
| `SpaceLounge::drawLounge` | 175 / 41 / 1948 | 558 / 138 / **1952** (not size-exact) |
| `Generator::createMission` | 162 / 64 / 1996 | 86 / 31 / **2000** (exact block, not size-exact) |
| `.ARM.extab` total | 1284 | 1284 (unchanged) |

## 1. SpaceLounge — the exact clamp is reachable, but the frame grows

The wave-158/159 conclusion stands: every `pad * 2` clamp shape keeps
`pad -> fp`, `x -> r9` (445/54 with the faithful right arm).  The `boxW`
local flips `pad -> r9`, `x -> r5` but grows the frame 120 -> 128.

**New this wave**: computing `boxW` from the raw field instead of `pad`

```cpp
int boxW = width + Globals::layout->field_0x94 * 2;
```

with the faithful clamp

```cpp
x = Globals::w - layout->buttonInsetX - (width + pad * 2);
```

reaches **the exact original clamp** at 1948+4 = **1952 B**:

```
1712c0: sub.w r1, r5, r9      ; x - pad      (pad=r9, x=r5 -- target)
1712c4: cmp   r1, r0
1712c6: bge.n 1712ce
1712c8: add.w r5, r0, r9      ; x = inset + pad
...
1712e0: itt   gt
1712e2: addgt.w r1, r6, sl    ; width + pad*2 (sl -- target)
1712e6: subgt r5, r0, r1      ; x = (w-inset) - ...
```

The +4 is the join: the field reload needs `r8`
(`ldr.w r8, [r4, #148]` at `0x1712ee`), so `lineHeight` takes `r8`, `top`
is spilled to `[sp,#20]`, and the frame is 128 instead of 120.  The clamp
itself is instruction-identical to the original.  Measured 558/138/1952.

Variants tried this wave (all real TU compile + gold link):

| shape | raw | aligned | size | pad |
|---|---:|---:|---:|---|
| HEAD | **175** | **41** | **1948** | fp |
| faithful clamp, no local | 445 | 54 | 1948 | fp |
| `boxW = width + pad*2` (both uses) | 1833 | 241 | 1968 | r9 |
| `boxW = width + field*2`, faithful clamp | 558 | 138 | **1952** | r9 |
| `boxW = field*2 + width`, faithful clamp | 558 | 138 | 1952 | r9 |
| `boxW = width + (field << 1)`, faithful clamp | 558 | 138 | 1952 | r9 |
| `boxW = width + field*2`, HEAD clamp | 1849 | 231 | 1976 | r9 |
| `boxW = field*2 + width`, HEAD clamp | 1849 | 231 | 1976 | r9 |
| `pad2 = pad + pad` + faithful clamp | 445 | 54 | 1948 | fp |
| `padCopy = pad` + `boxW = width + padCopy*2` | 1833 | 241 | 1968 | r9 |
| `padCopy = field` + `boxW = width + padCopy*2` | 558 | 138 | 1952 | r9 |
| `boxW` used only in drawBox (rect recomputes) | 1833 | 258 | 1968 | r5 |
| `boxW` used only in DrawRectangle | 1833 | 258 | 1968 | r5 |
| `boxW` const/short | 1833/1845 | 258/240 | 1968 | r5/fp |
| `lineHeight` field directly (no local) | 558 | 138 | 1952 | r9 |
| `top` after boxW | 1827 | 179 | 1964 | r6 |
| `float topF` / `short top` + field boxW | 558 | 138 | 1952 | r9 |
| `int top = (int)bottomRight.y - pad*2` | 1846 | 234 | 1964 | r9 |
| `y` local for `top + field_0x2c0` | 1662 | 246 | 1964 | r5 |
| `bool known` local | 1274 | 182 | 1956 | r5 |
| `pad` after `centerX` | 254 | 72 | 1948 | fp |
| `centerX` via `(topLeft.x+bottomRight.x)*0.5f` | 182 | 46 | 1948 | fp |
| `const int pad` | 175 | 41 | 1948 | fp |
| `top` via `pad*2` / float / int math | 175/175/1825 | 41/41/163 | 1948/1948/1968 | fp |

**Open question**: avoid the `top` spill in the field-`boxW` shape.  If the
frame stayed 120 the function would be 1944 and the clamp would be exact;
the missing 4 B are the join's `ldr.w r8` + top spill.  No source spelling
tried this wave both keeps `pad=r9` and avoids the spill.

## 2. Generator — the exact bonus block is 2000 B, blocked by one landing pad

The wave-159 block (`remv` + `volatile rem` + `rv`) still reproduces the
original bonus block exactly:

```
9770c: adds r5, r1, r6
9770e: str r1, [sp, #44]     ; original: [sp,#32]
97710: movs r1, #50
97712: mov r0, r5
97714: blx __aeabi_idivmod
97718: ldr r0, [sp, #44]     ; original: [sp,#32]
9771a: cmp r1, #0
9771c: it ne
9771e: subne r5, r6, r0
97720: str r5, [sp, #16]
```

The only instruction difference in the block is the spill slot `[sp,#44]`
vs the original `[sp,#32]`.  Measured **86 raw / 31 aligned / 2000 B**.

The +4 B is the EH landing-pad selector chain: our build emits 13 `b.n`
stubs + 5 handlers (18 landing pads), the original 12 + 5 (17).  The extra
distinct landing pad is `0x97934` (a stub to the common `sp+80` handler);
the LSDA row count is 26 in both.  The extra stub comes from the
`volatile int rem` getting a dedicated stack slot at `[sp,#44]` (the
original's natural `rem` spill reuses `[sp,#32]`, the dead
`Globals::status` slot), which changes the String-at-`sp+44` cleanup
region and the landing-pad ordering.

Variants tried this wave, all 2000 B with 86/31 (the block is stable):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`volatile rem` + `bonus -= rem*2`) | 162 | 64 | 1996 |
| wave-159 `remv`+`volatile rem`+`rv` | **86** | **31** | 2000 |
| `rv` before `bonus =` | 86 | 31 | 2000 |
| `volatile int rem; rem = remv;` | 86 | 31 | 2000 |
| `const volatile int rem` | 86 | 31 | 2000 |
| `volatile int *rem = &remv` | 86 | 31 | 2000 |
| `volatile int rema[1]` / `[2]` | 86 | 31 | 2000 |
| `volatile long rem` / `short remv` | 86 | 31 | 2000 |
| explicit `else { bonus = bonus; }` | 86 | 31 | 2000 |
| `rv` as `long` | 86 | 31 | 2000 |
| `bonus = remv + rawBonus` / `-rv + rawBonus` | 86 | 31 | 2000 |
| extra scope around the block | 86 | 31 | 2000 |
| `rem`/`rv` in inner scopes | 86 | 31 | 2000 |
| `volatile rem` at function scope (`remI`) | 845 | 172 | 2000 |
| `volatile int rem = rawBonus % 50` (no `remv`) | 609 | 57 | 2000 |
| `volatile remv` + `bonus -= remv*2` (= HEAD) | 162 | 64 | 1996 |
| `__builtin_expect((type\|4)==12, 0)` + w159 | 1098 | 106 | 1996 |
| `__builtin_expect((type\|4)==12, 1)` + w159 | 1799 | 176 | 1996 |
| no `Item *it` temp (wave-158 shape) | 300 | **59** | 1996 |
| `Item *it` temp (HEAD) | 162 | 64 | 1996 |

**Open question**: get `rem` into `[sp,#32]` (the natural spill slot) while
keeping the exact block.  Every `volatile` spelling gets a dedicated
`[sp,#44]` slot; every non-volatile spelling keeps `rem` in `r9` and spills
`agent` (frame 104, 2000 B).  The `Item *it` temp is a raw win (300 -> 162)
but costs +5 aligned; no shape measured this wave gives 162/59.

## 3. `.ARM.extab` per-record map; second-largest record

`python3 _work/reconstructions/extab_record_map.py` on the landed build:

```
  diff function                                                         k   olen    len orow urow
   676 _ZN12HangarWindow6renderEv                                       0    948    948  156  156
   182 _ZN11LevelScript7processEi                                       0    348    348   46   46
   159 _ZN5Level13createMissionEv                                       0    956    956  148  148
    87 _ZN5Radar4drawEP6PlayerP3Hudi                                    0    132    132   19   19
    77 _ZN5Level10createShipEiiiP8Waypointbb                            0    336    336   57   57
    37 _ZN3Hud4drawExxP9PlayerEgobjj                                    0    476    476   77   77
    30 _ZN5MGame10OnTouchEndEiiPv                                       0    280    280   44   44
    14 _ZN11SpaceLounge10drawLoungeEv                                   0    168    168   25   25
    13 _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE      0    172    172   26   26
     7 _ZN5RadarC1EP5Level                                              0    256    256   39   39
     2 _ZN10ModStationC1Ev                                              0    112    112   15   15
TOTAL diff bytes 1284 in 11 records
```

The second-largest record is `LevelScript::process` (182 B, 348-byte
record, 46 rows both).  It is **not fixable at source level in this wave**:
row 0 already diverges (`start=0 len=5832` original vs `len=5654` ours),
i.e. our first cleanup landing pad is 178 B earlier.  The original's first
cleanup call is `Explosion::Explosion` at `0x1393e0` (case 145
`new Explosion(0)`); ours is `Player::shoot` at `0x1392de`.  The whole
57 KB function is one of the large non-byte-exact residuals, so the LSDA
cannot match until the code layout does.

The next record, `Level::createShip` (77 B), is much closer: all 57 rows
match in structure and only row 0 is off by 2 B (`406` vs `404`), but the
register colouring (`r5/r6`, `r9/fp/sl`) and the pre-cleanup code layout
still differ, so the landing-pad offsets do not line up either.

SpaceLounge (14 B) and Generator (13 B) records are tied to their
functions' remaining code differences above.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10519
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10519/fdiff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10519/align_diff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10519/fdiff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 /dev/shm/w10519/align_diff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 _work/reconstructions/extab_record_map.py
```

Scratch (not committed), `/dev/shm/w10519/`: `fdiff.py`, `align_diff.py`,
`showdiff.py`, `sweep_sl.py`, `sweep_gen.py`, `sweep2.py`, `lsda_gen.py`,
`lsda_ls.py`, `lsda_cs.py`, `v1.py`..`s12.py`, `g1.py`..`g17.py`,
`SpaceLounge.base.cpp`, `Generator.base.cpp`.

## 5. Files changed

* `_work/reconstructions/small-tail-b-wave160-w10519.md` (this note).
