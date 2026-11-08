# w8812 (wave-143): `Level::createMission` — raid `guns` rematerialisation,
# faithful tail loop, and the tail `rnd`/`ship` register map

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8812`, based on master
`bf083ab41` (wave 142). Continues `level-createmission-wave142-w8712.md`,
`…-wave141-w8611.md`, `…-wave139-w8411.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8812/fdiff.py` — note: the older `/dev/shm/fdiff.py`
points at the *main* repo build, not this worktree).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5756
.text diff 85,119   .ARM.extab 1,395   .rel.plt 0   .note.gnu.build-id 20
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## NEW evidence — raid `guns` is computed *after* the raid guard

The original raid block (`b3752`..`b37e2`) loads `Status` into r8
(`ldr r8,[sp,#108]`), `fortressGuns` into fp (`ldr fp,[sp,#40]`) and
`alienGuns` into sl (`ldr sl,[sp,#52]`), then computes `add r5, sl, fp`
**after** the `if (raid)` guard at `b37e6`, not before it. HEAD hoisted
`int guns = alienGuns + fortressGuns;` above the guard, so clang kept the sum
in a stack slot and reused it for the post-raid pirate loop.

Fix (variant **A**): declare `int guns` *inside* the `if (raid)` block and a
second `int guns` after it. This is the faithful source shape — the original
has two independent live ranges.

## NEW evidence — tail loop register map

Original tail (`b416e`..`b41ec`):

```text
b415c/b4168  Globals::rnd loaded once into fp
b416e..      this in r8, i in r9, Globals::globals in sl
b41c8        ldr r0,[r8,#248]; ldr r0,[r0]   ; size() re-read every iteration
             setToSleep on the just-stored element (reload)
             objectivesA store: str.w r5,[r8,#40]  (does not tail-merge with
             the case-1 store at b20f6 str r5,[r4,#40])
```

HEAD cached `enemies->size()` in a `volatile unsigned n` stack slot and kept
`this` in sl. Three faithful reshapes:

* **F** — `for (unsigned i = 0; i < this->enemies->size(); i++)` (bound re-read).
* **L** — `KIPlayer *ship = this->createShip(...); (*this->enemies)[i] = ship;
  ship->setToSleep();` (store the local, then sleep the local).
* **R** — `AERandom *rnd = Globals::rnd;` before the tail loop and
  `rnd->nextInt(...)` in it (the original's fp cache; a pure pointer cache,
  no RNG-order change).

## Measured variants (all faithful; no call/store/RNG-order change)

| variant | size | raw diff |
|---|---:|---:|
| HEAD | 17172 | 5756 |
| A | 17192 | 5639 |
| A+F | 17184 | 5620 |
| A+F+L | 17176 | **5514** |
| **A+F+L+R (landed)** | **17172** | **5597** |

`A+F+L` is the lowest raw diff but is +4 B. `R` restores exactly 17,172 B at
the cost of +83 raw bytes (the fp cache forces a different allocation than the
`Globals::rnd` reload form). The task requires keeping 17,172 B, so
`A+F+L+R` is landed. **METRIC-ONLY note:** `R` is a pure pointer cache and is
witnessed by the original (`fp`), so it is faithful, not a metric trick.

## Negative results this wave (don't re-run)

* Tail loop spellings: `unsigned n`/`int n`/alias/`volatile int n`/preinc/
  `int i`/`int i<(int)size` → 5762–5889; faithful (no volatile) is
  17,164 B / 5889; `volatile int n` == baseline 5756.
* Tail `ship` local alone (no A): 17,164 B / 5872 (volatile n) or 5874.
* `bool alienWave` (+A): 12,333; `bool bigAlienWave+alienWave`: 11,734;
  `bool pirateFortress`: 16,535.
* `guns` as `const`: same 5756; `volatile`: 16,631; fully inlined: 6,565 (+28 B).
* Case-183 switch → if/else chains (3 orders): all exactly 5756.
* Case-2 convoy: store swap 5757; removing the `enemies` alias 10,884 (+8 B).
* `Globals *g = Globals::globals;` in tail: 5,428 but 17,176 B (+4); with
  `rnd` local too: 5,616/17,176.
* `Level *self = this` in tail: 5,514/17,176.
* A+F+R (no `ship` local): 17,184 / 5,621; `ship` local + faithful
  `(*this->enemies)[i]->setToSleep()` reload: 17,184 / 5,621 (clang coalesces
  the local back into the reload form). `ship` declared before the loop:
  identical to A+F+L+R (5,597).
* Raid `guns` declared *after* the spawn-position `if` (at the join point
  `b37e6`) instead of at the top of the raid block: 17,184 / 6,554 — worse.
  The original's `add r5,sl,fp` at the join is not reachable by source
  placement alone; variant A's top-of-block form stays.

## Open blocker (unchanged)

Alien-gun loop (`b329a`..`b3394`) and pirate-fortress loop (`b33fe`..`b34dc`)
remain the largest hunks. The in-loop table select is
`ldr r0,[sp,#64]; cbz; ldr r1,[pc]; add r1,pc; b; ldr r1,[pc]; add r1,pc;
add r0,r1,r9; mov sl,r1` in the original; our `(bigAlienWave + (i < 0))`
anchor emits `cmp asr#31; it ne; movne` and forces the wrong register map
(`this` r5, `gun` r4 vs original `this` r4, `gun` r6). This is the wave-142
blocker; no new spelling tried this wave beat it.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
.text diff      85,119 -> 84,960   (-159)
.ARM.extab      1,395  -> 1,415    (+20, not gated; extra tail scope)
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw diff 5756 -> 5597
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8812/fdiff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w8812/{Level.base.cpp,Level.A.cpp,Level.AFL.cpp,
Level.rndlocal.cpp,fdiff.py,measure.sh,cum.py,align.py,ops.py,hunks.py,
hunkrank.py,sweep_*.py,orig.asm,ours.asm,o_raid.asm}`.
