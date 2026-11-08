# w10613 (wave 160): `Level::createMission` — non-alien mission arms audit
# + case-183 freighter `switch(k)` default-store order; raw 5,652 -> 5,659,
# aligned 709 -> 704, size 17,172 B

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10613`, based on master
`08af4633b` (wave 159).  Continues
`level-createmission-wave159-w10509.md`, `…-wave159-w10510.md`,
`…-wave158-w10410.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B).  Ghidra
MCP unavailable; NDK `objdump` is the authority.  Raw same-address diff:
`/dev/shm/w10613/fdiff.py`; instruction-index alignment:
`/dev/shm/w10613/align_diff.py` (worktree-local copies with `OURS` pointed at
this worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5652   align 709
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,722   .ARM.extab 1,256   .rel.plt 0 (byte-identical)
```

## 1. Mission-arm map (else branch `switch (mission->getType())`)

The original lowers the sparse switch as a TBH over `type-1` (range 0..14)
plus explicit `cmp #184` / `cmp #183`.  Decoded targets (TBH base `0xb02ac`):

| type | block | source |
|---|---|---|
| 3,5 | `0xb02d0` | cargo mission (byte-identical to ours) |
| 183 | `0xb10d2` | freighter convoy |
| 1 | `0xb14b2` | attackers + friends |
| 2 | `0xb16d4` | asteroid convoy |
| 4 | `0xb19c0` | hunter branch (`goto case4Else/case4Tail`) |
| 6 | `0xb1a00` | single freighter |
| 7 | `0xb1b20` | asteroid field |
| 9 | `0xb1c9c` | station defence |
| 10 | `0xb1db8` | station attack |
| 12 | `0xb2128` | agent escort |
| 15 | `0xb2368` | mining plant |
| 184 | `0xb2700` | two-station convoy |

The `isEmpty()` branch (freelance/random mission generation) is interleaved
in the address space; its post-alien tail (pirateFortress / raid / pirates /
stationPirates / extraAliens / convoy / blueprint) lives at
`0xb2e04`–`0xb40b2`.

## 2. Landed change (`src/game/world/Level.cpp`, case 183 `switch (k)`)

The case-183 freighter loop stores a `float offset[3]` per `k`.  The original
default (`k==3`) emits `offset[1]=6000`, `offset[0]=12000`, then shares the
`offset[2]=-45000` tail store with cases 0/1/2.  Our source wrote
`offset[2]; offset[0]; offset[1]`; an exhaustive sweep of all six store
permutations found `offset[1]; offset[2]; offset[0]` is the best aligned shape:

| default store order | raw | align | size |
|---|---:|---:|---:|
| **`2,0,1` (HEAD)** | **5652** | **709** | 17172 |
| `0,1,2` | 11165 | 858 | 17172 |
| `0,2,1` | 5658 | 709 | 17172 |
| `1,0,2` | 11345 | 825 | 17172 |
| **`1,2,0` (landed)** | **5659** | **704** | 17172 |
| `2,1,0` | 5655 | 706 | 17172 |

The landed order is the only one at 704; all six are size-exact.  The change
also reorders the literal pool so the pc-relative loads line up better with
the original (the aligned metric normalises branch targets but not literal
pool addresses).

## 3. Findings that did NOT land (measured, size-exact unless noted)

### 3.1 case-183 `switch(k)` body order is a PGO block-placement artefact

Original body layout: case0 `0xb1420`, **case1 `0xb142c`**, **case2 `0xb1438`**,
default `0xb1444`.  Ours: case0 `0xb1420`, **case2 `0xb142c`**,
**case1 `0xb1438`**, default `0xb1446` — the case1/case2 bodies are swapped.
The dispatch compares are identical (`cmp #2 / beq; cmp #1 / beq; cbnz /
default; case0`).  Reordering the `switch` cases in source (0,2,1) does not
change the emitted layout (raw 5652 / align 709, byte-identical codegen).
This is the same sample-PGO branch-weight signature documented in w10509 §5.

### 3.2 case-2 convoy loop store order / `enemies` cache

Original `0xb1930`: reloads `this->enemies` for every statement and stores
`cargo` (`+0x50`) before `crateCaptured` (`+0x4c`).  Ours keeps the
`Array<KIPlayer*> *enemies` local in `r4` and stores `crateCaptured` before
`cargo`.  Removing the local (+8 B, gate-fatal) and swapping the two source
stores (raw +1, align unchanged) were both measured and rejected.  The
`enemies` local is load-bearing for size (commit `43b884af2`).

### 3.3 All literals / thresholds are correct

An instruction-level immediate diff (`/dev/shm/w10613/immdiff.py`) over the
whole function reports **no non-pc-relative immediate mismatch**.  Spot
checks against the original:

* pirateFortress gate `rnd->nextInt(100) <= 29` (`cmp #29 / bgt`) — `<=`.
* alienWave gate `rnd->nextInt(100) <= 29` (twice) — `<=`.
* `bigAlienWave = mission > 103` (`cmp #103 / it gt`) — `>`.
* `pirateFortress = mission > 140` (`cmp #140 / it gt`) — `>`.
* `*bigShipsCount <= 0 || localRace != 0` (`cmp #0 / ble; bne`) — `<= 0`.
* station-race `rnd->nextInt(100) > 29` (`cmp #29 / it gt`) — `>`.
* `friendCount`/`field_164`/`field_168`/`hostileCount` ArraySetLength sum
  (`0xb2a50`–`0xb2a8c`) has the **same left-associative term order** as ours;
  only stack-slot numbers differ (the global 4-byte slot permutation, see §4).

### 3.4 `isEmpty()` tail after the alien block

The `pirateEscorts + stationPirates + blueprint + extraAliens + pirates +
convoy + fortressGuns + alienGuns + friendCount + field_164 + field_168 +
hostileCount` sum and the pirate-escort loop bound (`0xb3dda`) also use the
same association order; the differences are stack-slot permutation
(`sp+40/44/48` rotated) and register allocation (`fp`/`sl`/`r8`/`r9`).

## 4. Remaining residual (all RA / PGO / stack-slot)

* Global 4-byte stack-slot permutation: original spills `Globals::status` at
  `sp+88`, ours at `sp+84`; slots `40/44/48` are rotated.  The frame size is
  identical (`0x190`) and the set of used offsets is identical except
  `760/896` (orig) vs `768/856` (ours).  This accounts for the bulk of the
  `operand:ldr` (126) mismatches.
* The two `if (i == start && alienWave)` / `else if (... && pirateFortress)`
  guards at `0xb2e04`/`0xb2f2a` are branchless if-conversions in the original
  (`subs`/`it ne`/`movne`/`eor`/`orrs`) and short-circuit branches in ours.
  Sample-PGO signature; no source form reproduces it at `-Oz` (w10509 §5).
* The wanted-loop `if (hasWanted && i == 0)` guard (`0xb2aa2`) is likewise
  if-converted in the original.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5652 -> 5659   (+7)
                           align  709 -> 704    (-5)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,722 -> 85,729  (+7)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,256 -> 1,256
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

## 6. Open questions

1. The case-183 case1/case2 body swap and the `0xb2e04` if-conversions are
   sample-PGO artefacts.  Reproducing them would realign the whole tail and
   drop raw by thousands, but needs profile data or a clang flag (not allowed).
2. The 4-byte stack-slot permutation is the largest remaining non-PGO residual.
   A source shape that changes the alloca/first-use order of the `status`
   spill and the `sp+40/44/48` locals would remove most `operand:ldr`
   mismatches; not found this wave.
3. `offset[1]; offset[2]; offset[0]` is the best aligned default-store order
   found (704).  If a future wave changes the surrounding case-183 code, re-run
   the six-permutation sweep.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10613/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10613/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10613/secdiff.py
```

Scratch: `/dev/shm/w10613/{fdiff.py,align_diff.py,secdiff.py,regions.py,
immdiff.py,opdiff.py,sweep.py,sweep2.py,sweep3.py,Level.base.cpp}`.
