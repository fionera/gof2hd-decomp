# `Generator::createMission` — wave 119 (w6703): pad-store placement + reward-chain order

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **source change landed.**  The wave-118 shape (50.1 %) is replaced by a variant
that keeps the same `.text` size (1996 B), the same `.ARM.extab` section size
(`0x26294`, 4 B short of the original `0x26298`) and the same 27 sections, while the
fuzzy match rises **50.1 % -> 53.3 %**.  The landed LSDA record is 173 B / `tlen=152`
(one byte larger than the original 172/151; the section size is unchanged because the
record is 4-byte aligned, so the ratchet is unaffected).

Gate at the end of the pass (full `check`):

```
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.ARM.extab`
`0x26294` (unchanged), `.rodata` `0x41c27` (unchanged; the appended rank table).
`git status --short` empty after the commit.

## 1. The landed delta (vs the wave-118 shape)

```diff
     assignType:
+    padA = 0; (void) padA;
         type = pick;
     }
...
-    padA = 0; (void) padA;
-    int base = (int) ((float) difficulty / 10.0f * 5500.0f) + 1500;
     float reward = (galaxy->distance(...) / 1200.0f + 1.0f) *
-                   (float) base;
-    if (type == 7) { reward *= 0.7f; }
-    else if (type == 9) { reward *= 1.2f; }
-    else if (type == 8) { ... getMaxPrice ... }
-    else if (type == 11) { ... }
-    else if (type == 3) { reward *= 2.0f; }
-    else if (type == 5) { reward *= 2.0f; }
+                   (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);
+    if (type == 11) { ... }
+    else if (type == 5) { reward *= 2.0f; }
+    else if (type == 3) { reward *= 2.0f; }
+    else if (type == 8) { ... getMaxPrice ... }
+    else if (type == 7) { reward *= 0.7f; }
+    else if (type == 9) { reward *= 1.2f; }
```

Three independent knobs were searched: the position of the pre-existing `volatile
double padA` store, the `int base` split, and the order of the six reward if-else arms.
All three change the post-RA block layout, so they move the fuzzy score without changing
the function size.

## 2. Measurements

All variants below keep `usize == 1996` (the `.text` size must not move: a 4-byte
shortfall shifts every later function and drops `byte_exact` from 4503 to 2615) and
`rec == 173` / `.ARM.extab 0x26294`.

| variant | pct | usize | rec | extab |
|---|---:|---:|---:|---:|
| wave-118 landed (pad after loop, `int base`, chain 7,9,8,11,3,5) | 50.1 | 1996 | 172 | 0x26294 |
| pad after loop, no `int base`, chain 7,9,8,11,3,5 (`D_nobase`) | 52.2 | 1996 | 173 | 0x26294 |
| pad after loop, no `int base`, chain 9,7,8,11,3,5 | 52.4 | 1996 | 173 | 0x26294 |
| pad after loop, no `int base`, chain 5,3,8,11,7,9 | 52.7 | 1996 | 173 | 0x26294 |
| pad after `used[...] = true`, chain 5,3,8,11,7,9 (`Q1`) | 53.0 | 1996 | 173 | 0x26294 |
| **pad after `assignType:`, chain 11,5,3,8,7,9 (`Q9`, landed)** | **53.3** | **1996** | **173** | **0x26294** |

The full 720-permutation reward-chain sweep on the `Q1` pad position gives a maximum of
**53.3 %** at `(11,5,3,8,7,9)`, `(11,5,3,7,8,9)`, `(11,3,5,8,7,9)`; 53.5 % exists at
`(11,5,9,7,3,8)` etc. but at `usize 1992` (fails the size constraint).  The pad-store
position is a plateau: every position between the `used` block and the `assignType:`
label compiles to the same 53.3 % code.

## 3. Dead ends (measured, not landed)

| variant | pct | usize | rec | note |
|---|---:|---:|---:|---|
| faithful shape, no `volatile type` (`base83`) | **83.3** | 1996 | 168 | `.ARM.extab` shrinks to `0x2628c` (8 B regression); gate still PASSES (byte 4503) |
| `base83` without `padA` | 71.9 | 1984 | 168 | frame 96 (matches original) but 12 B short -> byte 2615 |
| `Q9` with `volatile float padA` | 55.2 | 1992 | 173 | 4 B short -> byte 2615 |
| `Q9` with `volatile int32_t type;` (no init) | 55.2 | 1992 | 173 | 4 B short -> byte 2615 |
| `Q9` with `volatile int padA` | 53.9 | 1992 | 173 | 4 B short |
| `Q9` with `volatile long long amount` | 52.9 | 2032 | 173 | 36 B long |
| `Q9` + `expect(getRoutes()==nullptr, 0)` | 50.8 | 2008 | 173 | |
| `Q9` + `expect(race<4,1)` | 46.1 | 2028 | 174 | |
| `Q9` + `expect(difficulty>=10,1)` | 52.2 | 1996 | 173 | no change |

The 83.3 % shape is the *faithful* one (`int32_t type`, no volatile spill); its only
defect is the 168-byte LSDA record (the `type` spill block is placed after the
`type = 8` arm instead of after the `type = 13` arm — a clang 7.0.2svn vs 7.0.1
`MachineBlockPlacement` divergence, see `generator-createmission-wave117-w6509.md` §5).
Landing it would undo wave 118's LSDA exactness, so it is reported but not landed.

## 4. Reproduction

```sh
cp /dev/shm/w6703/Q9.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4503
python3 /dev/shm/w6703/score.py                        # pct 53.3, usize 1996
python3 /dev/shm/w6703/lsda2.py cmake-build-match/libgof2hdaa.so \
    _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE   # rec 173, tlen 152, 26 rows
```

Scratch files: `/dev/shm/w6703/` (`Q9.cpp`, `Q1.cpp`, `DN8.cpp`, `D_nobase.cpp`,
`Generator.landed.cpp`, `Generator.base.cpp`, `score.py`, `lsda2.py`, `measure2.sh`,
`perm.py`).  Not part of the repo.
