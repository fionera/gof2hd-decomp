# w12011 (wave 176): `Level::createMission` — cleanup-only LSDA is already
# the right shape; landing a 6-byte raw win from the `rnd` hoist placement

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w12011`, branch
`wave176-w12011`, base `master b21088985` (wave 175).  Target
`_ZN5Level13createMissionEv` (`src/game/world/Level.cpp`, 17,172 B, VMA
`0xaffd8`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

## 1. Result

| metric | before (base b21088985) | after |
|---|---:|---:|
| raw same-address diff | **5,285** | **5,279** |
| instruction-index alignment | 958 | 959 |
| size | 17,172 / 17,172 | 17,172 / 17,172 |
| insns (orig/ours) | 6197 / 6212 | 6197 / 6212 |
| `.text` differing bytes | 80,166 | **80,160** |
| `.ARM.extab` size/base | 156,312 exact | 156,312 exact |
| `.ARM.extab` differing bytes | 746 | 747 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |
| `.rel.dyn` / `.rodata` / `.data` / `.got` / `.bss` | exact | exact |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.  `drift` 0.
Symbol parity 5370/5370.

## 2. Landed source shape

The only change is moving the loop-invariant RNG hoist from after
`ArraySetLength` to before the hunter-count computation:

```cpp
    // Hoisted before the hunter-count block: the RNG pointer is loop-invariant,
    // and this placement reproduces the original's literal-pool/RA shape.
    AERandom *rnd = Globals::rnd;
    float hunters = (float) ((int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f) + 2);
    int hunterCount = (int) (hunters + (*(float *) &Globals::options[44] - 0.5f) * hunters);
    this->enemies = new Array<KIPlayer *>();
    ArraySetLength(hunterCount, *this->enemies);
    Globals *globals = Globals::globals;
    Array<KIPlayer *> *enemies = this->enemies;
    for (unsigned i = 0; i < enemies->size(); i++) {
        KIPlayer *ship = this->createShip(
                8, 0, globals->getRandomEnemyFighter(8),
                this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length())), true, false);
        (*enemies)[i] = ship;
        ship->setToSleep();
    }
```

`Globals::rnd` is not modified by `getDifficulty()` or the `Globals::options`
read, so the hoist is semantics-preserving.  The codegen effect is a
literal-pool/RA shape change: the function stays size-exact, the tail
landing-pad block stays at the same address, and the same-address raw
diff drops by 6 bytes.

## 3. Exception / landing-pad findings

The original `createMission` LSDA is **already a cleanup-only record**, not a
`try`/`catch` record:

```text
orig:  reclen 956, personality 0x5eb60, uw 3w, lp 0xff, tt 0xff,
       cs_enc 0x1, cs_len 934, 148 rows
ours:  reclen 956, personality 0x5eb60, uw 3w, lp 0xff, tt 0xff,
       cs_enc 0x1, cs_len 932, 148 rows   (base was cs_len 933)
```

Both records are 16 B header + 320 B LSDA-ish body padded to 956 B; the
call-site row structure is the same (148 rows, same start/length sequence
except for the known 2-byte body-layout shifts around rows 37/38 and
49/50/53).  The remaining `.ARM.extab` diff is entirely **landing-pad
offsets**: every original LP is `ours + 6`, `ours + 10`, or `ours + 18`
depending on the thunk group, i.e. the cleanup thunk block is 6 bytes later
in the original because the main body is 6 bytes longer there.  There is no
missing EH scope and no wrong personality/type-table.

The 1-byte `.ARM.extab` diff increase (746 -> 747) is the ULEB encoding of
`cs_len` changing 933 -> 932; the record is still padded to 956 B, so the
section size/base and `.ARM.exidx` are unchanged.

### `try`/`catch` is size-fatal

Wrapping the tail loop body (or just the `createShip` call) in

```cpp
try { ... } catch (...) { __builtin_trap(); }
```

changes the record to `ttype 0x00` (catch-all type table), grows the
function to 17,184 B, and grows `.ARM.extab` to `0x262ac`:

| shape | size | raw | `.ARM.extab` |
|---|---:|---:|---:|
| base (cleanup-only) | 17,172 | 5,285 | `0x26298` |
| `try` around loop body | 17,184 | 13,321 | `0x262ac` |
| `try` around `createShip` call | 17,184 | 13,525 | `0x262ac` |
| natural loop + `try` around `new Array` | 17,196 | 5,254 | `0x262a8` |
| natural loop + `try` around `new Objective` | 17,192 | 5,351 | `0x262a8` |

So the faithful `new`-cleanup-only record is the right shape; a
`try`/`catch` cannot match the original LSDA without changing the section
size.

## 4. The natural (un-hoisted) loop is a near-win, but +4 B

The original's tail loop uses `Globals::rnd` inline and reloads
`&Globals::rnd` from `[sp,#120]` (stored at `b0276`), so its tail literal
pool has 6 words.  Our natural un-hoisted loop matches the loop body but
lets clang rematerialise the GOT address, adding a 7th tail literal:

| shape | size | raw | aligned | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base (`rnd`/`globals`/`enemies` hoisted) | 17,172 | 5,285 | 958 | `0x26298` |
| natural loop (no hoists) | 17,176 | 5,130 | 955 | `0x26298` |
| `globals` only hoisted | 17,176 | 5,129 | 958 | `0x26298` |
| landed: `rnd` hoisted before `hunters` | **17,172** | **5,279** | 959 | `0x26298` |

The +4 B is exactly the extra `Globals::rnd` GOT literal in the tail pool
(7 words vs the original's 6).  The original avoids it by keeping
`&Globals::rnd` in `[sp,#120]` across the function; clang 7.0.2
rematerialises it from a literal in our source shape.  Forcing the reload
(early `AERandom *rnd` live across the function, `AERandom **rndp`,
`volatile`, address-taken locals, `hunterCount` type/slot changes) either
keeps the literal or blows up the register allocation (17,248-17,364 B).
The landed `rnd`-before-`hunters` placement is the best size-exact
compromise found.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12011
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12011/`
(`Level_base.cpp`, `sweep_loop.py`, `sweep_hoistpos.py`, `sweep_full.py`,
`sweep_noe.py`, `sweep_try.py`, `sweep_natural_try.py`, `sweep_hc2.py`,
`sweep_known.py`, `sweep_switch2.py`, plus saved `base.so`).
