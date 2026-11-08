# w8712 (wave-142): `Level::createMission` — alien-gun turret cache anatomy,
# in-loop table select, and the case-183 `default` store order

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8712`, based on master
`1d10d0694` (wave 141). Continues `level-createmission-wave141-w8611.md`,
`…-wave140-w8511.md`, `…-wave139-w8411.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8712/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5763
.text diff 84,567   .ARM.extab 1,395   .rel.plt 0   .note.gnu.build-id 20
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## Landed change — case-183 `default` store order

The `default:` arm of the convoy-offset switch stores `offset[1]`, `offset[0]`,
`offset[2]` in the original (`b1446`, `b144a`, `b144e`). All six source
permutations were measured; the store scheduling is *not* the source order, so
the source order that reproduces the original's literal-pool ordering is
`offset[2], offset[0], offset[1]`:

| source order | size | raw diff |
|---|---:|---:|
| `1,2,0` (HEAD) | 17172 | 5763 |
| `0,2,1` | 17172 | 5762 |
| `2,1,0` | 17172 | 5759 |
| **`2,0,1` (landed)** | 17172 | **5756** |
| `1,0,2` | 17188 | 10892 (size shift) |
| `0,1,2` | 17164 | 11391 (size shift) |

Source case-block order (`0,1,2` vs `0,2,1`) makes no difference once the
default order is `2,0,1` (both 5756), so the wave-139 `0,1,2` order is kept.

## NEW evidence — alien-gun loop: `gun` is cached, `this`/`alienGuns` reloaded

The original alien-gun loop (`b329a`..`b3394`, **254 B / 91 insns**) caches the
new turret in **r6** and reuses it for `setScaling`
(`b3366 ldr r1,[pc,#584]; b3368 mov r0,r6; b336a blx setScaling`). Our source
deliberately reloads `(*this->enemies)[i]` for `setScaling` (a wave-13x
codegen anchor). Removing that reload and the `(i < 0)` anchor gives the
"clean" form (`/dev/shm/w8712/v1.asm`): **224 B / 79 insns**, function 17,140 B
(raw 6,613 — the −32 B phase-shifts the whole tail).

The 30-byte loop deficit is fully accounted for by the **in-loop table select**:

```text
original b32c6: ldr r0,[sp,#64]      ; bigAlienWave, reloaded every iteration
         b32c8: cbz r0, b32d0
         b32ca: ldr r1,[pc,#732]     ; kAlienStationTurrets
         b32cc: add r1, pc
         b32ce: b b32d4
         b32d0: ldr r1,[pc,#728]     ; kAlienOutpostTurrets
         b32d2: add r1, pc
         b32d4: add r0, r1, r9       ; t = base + k*24
         b32d8: mov sl, r1
```

`bigAlienWave` is *never* register-resident in the original: every use is a
`ldr [sp,#64]` (`b2a20` alienGuns, `b2e1a` big-wave branch, `b32c6` table
select). A plain `bigAlienWave ? A : B` is LICM-hoisted by clang 7.0.2 even
inside the loop, so the address materialisation moves out of the loop and the
loop loses the 24 B select + 4 B `add base,k*24` + the `mov sl,r1` (2 B).

Measured attempts to keep the select in-loop:

| variant | size | raw diff | select in loop? |
|---|---:|---:|---|
| HEAD (reload + `(i<0)` anchor) | 17172 | 5763 | yes, but `cmp asr#31` form |
| cached gun + plain select | 17140 | 6613 | no (hoisted) |
| cached gun + `if/else` | 17140 | 6613 | no (hoisted) |
| cached gun + `(i<0)` anchor | 17172 | 5913 | yes, `cmp asr#31` form |
| cached gun + `bool bigAlienWave` | 17124 | 12515 | no |
| cached gun + `*(volatile int*)&bigAlienWave` | 17132 | 12952 | load yes, select if-converted |
| cached gun + `volatile int bigAlienWave` | 17132 | 12955 | no |

The `(i<0)` anchor is the only spelling that keeps the select in the loop at
the right size, but it emits `cmp.w r1, r9, asr #31; it ne; movne` instead of
the original's `ldr; cbz` + two `ldr [pc]` arms, and it forces `gun` into r4
and `this` into r5 (original: `this` r4, `gun` r6, `i` fp, `k*24` r9, table
base sl). The register map is the blocker: the original keeps `this` in its
home register r4 and re-loads it in the loop tail (`b336e ldr r4,[sp,#124]`),
plus reloads `alienGuns` (`b3386 ldr sl,[sp,#52]`); the clean form holds both
in callee-saved registers and is 30 B shorter.

## Other negative results this wave

* `guns` inlined in the raid loop only / raid+pirate / +`first` (on top of the
  cached-gun form): no change to the 5,763 baseline; combined with the cached
  gun it is still 17,140 B.
* Pirate-fortress loop already matches the original's store order
  (`cargo` then `shipGroup`) and caches `gun` in r4; only the table base
  differs (`t+5`/`t` in ours vs `t+3`/`t` in the original, a 6-byte addressing
  tie-break).
* Tail loop: the original re-reads `this->enemies->size()` every iteration and
  keeps `this` in r8 (`b41c8 ldr r0,[r8,#248]; ldr r0,[r0,#0]`), so the
  `objectivesA` tail store is `str.w r5,[r8,#40]` and does not tail-merge with
  the case-1 store. The faithful tail (`i < this->enemies->size()`) is
  17,164 B / 5,896 and puts `this` in sl, not r8 — still the wave-140 result.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      84,567 -> 84,560
.ARM.extab      1,395  -> 1,395
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw diff 5763 -> 5756
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8712/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8712/buckets.py _ZN5Level13createMissionEv 0x400
python3 /dev/shm/w8712/alignr.py orig.asm ours.asm b1400 b1470 b1400 b1470
```

Scratch: `/dev/shm/w8712/{Level.base.cpp,fdiff.py,buckets.py,alignr.py,
dis.py,tt.py,sweep1..9.py,apply201.py,orig.asm,ours.asm,v1.asm,ac.asm,
d201.asm}`.
