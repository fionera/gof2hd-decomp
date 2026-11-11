# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 178 / w12213

Target: `src/game/world/Level.cpp`, function at `0xaffd8`, size **17172 B** (`0x4314`).
Branch `wave178-w12213`, base master `1b48f990f`.

## Result

**One size-exact code win landed.**

```
raw 5143 -> 4467   aligned 952 -> 952   size 17172 / 17172 (unchanged)
.text differing bytes  79,251 -> 78,575  (-676)
.ARM.extab differing   731 -> 700        (-31; size/base still 0x26298)
.ARM.exidx differing   0 -> 0
.rel.plt               byte-identical (cmp clean)
```

Gate after the change: `byte_exact 4507 / 4524`, `linked_exact 4507`, `size_exact 4524/4524`,
`data_exact 843/843`, imports **207**, extra exports **0**, missing **0**, lint **CLEAN**,
drift **0**, symbol parity **5370/5370**, `RATCHET PASS`.

## The change (two edits, one size-neutral pair)

### 1. Drop the `extraAliens` expect (`Level.cpp:2052`)

```cpp
-    if (__builtin_expect(extraAliens >= 1, 1)) { // lint: arm_builtin
+    if (extraAliens >= 1) {
```

This is the wave-178 lead: it changes clang's global block placement, fixes the
`raid = raidRoll < (security==0?90:…)` block order at `0xb04be`, and collapses the 249-byte
raw run at `0xb2f24`.  Alone it is **+4 B** (17176), because the tail then keeps the original's
code length and exposes an extra literal-pool word.

### 2. Call `nextInt` on `Globals::globals`, not `Globals::rnd`

```cpp
-    // Hoisted before the hunter-count block: the RNG pointer is loop-invariant,
-    // and this placement reproduces the original's literal-pool/RA shape.
-    AERandom *rnd = Globals::rnd;
     ...
-                this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length())), true, false);
+                this->playerRoute->getWaypoint(((AERandom *) Globals::globals)->nextInt(this->playerRoute->length())), true, false);
```

This removes the extra `Globals::rnd` GOT literal and restores the original's 6-word tail pool,
paying back the `+4` exactly.

## Why `Globals::globals` (the wave-176 note had this wrong)

The wave-176 note said the original keeps `&Globals::rnd` in `[sp,#120]`. It does not. Resolving
the original's PC-relative literals against `.rel.dyn`:

| site | literal | `lit + pc` | GOT symbol |
|---|---|---|---|
| `0xb2d08` (`ldr r1,[pc,#224]`) | `0xb2dec` = `0x15d500` | `+0xb2d18` = **`0x210218`** | `_ZN7Globals7globalsE` |
| `0xb415c` (`ldr r0,[pc,#392]`) | `0xb42e8` = `0x15c0ae` | `+0xb416a` = **`0x210218`** | `_ZN7Globals7globalsE` |

(`_ZN7Globals3rndE` is at GOT `0x2101f8`.) So the original hoists `&Globals::globals`:

```
b2d1c: str r1,[sp,#120]      ; sp+120 = &Globals::globals
...
b2d20: ldr r0,[sp,#120]
b2d24: ldr r0,[r0]           ; Globals::globals -> getRandomEnemyFighter(this)
...
b4168: ldr.w sl,[r0]         ; sl = &Globals::globals
b416e: ldr.w r0,[sl]         ; Globals::globals -> getRandomEnemyFighter(this)
...
b417a: ldr r0,[sp,#120]
b4180: ldr r6,[r0]           ; Globals::globals -> AERandom::nextInt(this, len)
```

The original really passes `Globals::globals` as the `AERandom::nextInt` receiver (a type-pun of
the `Globals*`; `Globals` has no `nextInt` member and does not derive from `AERandom` — its ctor
at `0xe31e0` calls no base ctor and zeroes offsets 0/4).  Reproducing that receiver is what kills
the extra literal.

## Measured variants (all real `link_so` + `fn_diff`)

| variant (on top of the `extraAliens` expect removal) | raw | aligned | size |
|---|---:|---:|---:|
| none | 4485 | 975 | 17176 (+4) |
| `AERandom *&rnd = Globals::rnd;` (semantically "correct" RNG) | 4664 | 979 | 17184 |
| `AERandom *&rnd = (AERandom *&) Globals::globals;` | 4467 | 952 | 17172 |
| `AERandom *&rnd = *(AERandom **) &Globals::globals;` | 4467 | 952 | 17172 |
| `AERandom *&rnd = reinterpret_cast<AERandom *&>(Globals::globals);` | 4467 | 952 | 17172 |
| `AERandom *rnd = (AERandom *) Globals::globals;` (top local) | 4586 | 958 | 17168 |
| `Globals::globals->getRandomEnemyFighter(8)` + direct cast nextInt (no local) | 4572 | 948 | 17172 |
| **landed: keep `Globals *globals` local, direct cast at the nextInt site** | **4467** | **952** | **17172** |

The landed shape is the semantically faithful one (the original uses `Globals::globals` at both
call sites) and it also happens to give the best raw count.  The reference-binding forms are
codegen-equivalent but express the same type-pun less directly.

## Residual (unchanged by this pass)

The 249-byte run at `0xb2f24` is now gone.  The remaining raw is dominated by the flag/stack-slot
rotation (`pirateFortress/alienWave/bigAlienWave` = `sp+56/60/64` original vs `sp+56/60/64` in a
different order) and the `extraAliens`/`stationPirates` slot swap documented in the wave-177 note;
those were re-measured and remain size-fatal or raw-negative.

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12213
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch tooling: `/home/debian/GalaxyOnFire2/scratch/w12213` (`batch.py`, `rawruns.py`,
`textdiff.py`).  Pristine source copy: `Level.base.cpp`; base build `base.so`.
