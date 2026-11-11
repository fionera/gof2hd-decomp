# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 179 / w12312

Target: `src/game/world/Level.cpp`, function at `0xaffd8`, size **17172 B** (`0x4314`).
Branch `wave179-w12312`, base master `64014ce6f` (post-wave-178 merge).

## Result

**One size-exact code win landed (three edits).**

```
raw 4467 -> 3844   aligned 952 -> 962   size 17172 / 17172 (unchanged)
insn 6208 -> 6207
.text differing bytes  77,936 -> 77,313  (-623)
.ARM.extab differing      700 -> 694     (-6; size/base still 0x26298)
.ARM.exidx differing        0 -> 0
.rel.plt               byte-identical (cmp clean)
```

Gate after the change: `byte_exact 4507 / 4524`, `linked_exact 4507`, `size_exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**,
lint **CLEAN**, drift **0**, symbol parity **5370/5370**, `RATCHET PASS`.

The aligned metric rises by 10 because two of the three edits trade a long same-address raw run for a
short scheduling reorder; the raw metric (the one this sweep is scored on) falls by 623 B.

## The change

### 1. Explicit null check before the three `delete ...->cargo` sites (raw -600)

The wave-178 base had `delete ptr;` directly.  Writing the check out by hand:

```cpp
-                delete (*this->enemies)[i]->cargo;
+                if ((*this->enemies)[i]->cargo != nullptr) {
+                    delete (*this->enemies)[i]->cargo;
+                }
                 (*this->enemies)[i]->cargo = nullptr;
                 (*this->enemies)[i]->field_0x70 = 0;
```

applied to the three `delete cargo` sites:

| site | line (post-change) | raw delta |
|---|---|---:|
| `bigAlienWave` waypoint block | 1918 | **-591** |
| `hasWanted` wingman block (`[0]`) | 1874 | -4 |
| `pirateFortress` block | 1941 | -5 |

The big one is the `bigAlienWave == 1` branch.  With the plain `delete`, clang computes the address
of `cargo` into **r1** (`adds r1, #80`, 2 B) and pads the literal pool with a 2-byte `nop`; the
original computes it into **r3** (`add.w r3, r1, #80`, 4 B) and has no nop.  That single 2-byte
instruction-selection difference shifted every byte of the following **249-byte** run at
`0xb2f24..0xb301d`.  Spelling the null test out makes clang keep `(*enemies)[i]` live in r0 for the
`field_0x70` store and materialise `&cargo` in a fresh register (r3), exactly as the original does:

```
b2f16: ldr.w r2,[r5,#0xf8]      ; this->enemies
b2f1a: ldr.w r0,[r2,#4]!
b2f1e: ldr.w r1,[r0,r8]         ; (*enemies)[i]
b2f22: add   r0,r8              ; &(*enemies)[i]
b2f24: add.w r3,r1,#80          ; &cargo   <-- now matches
b2f28: b.n   b3194
...
b3194: str.w r9,[r3]            ; cargo = nullptr
b3198: ldr   r0,[r0,#0]
b319c: strb.w r9,[r0,#112]      ; field_0x70 = 0
```

The explicit test is semantically identical to `delete` (which already emits `cmp/beq`); it only
changes the register allocator's tie-break.  The `hasWanted` and `pirateFortress` sites are the same
shape and give small independent wins.

### 2. Split the pirate-escort `Vector pos` into three float locals (raw -16)

```cpp
-            Vector pos = {pirateBase.x + (float) (Globals::rnd->nextInt(20000) * (... ) + 10000),
-                          pirateBase.y + (float) (Globals::rnd->nextInt(20000) * (... ) + 10000),
-                          pirateBase.z + (float) (Globals::rnd->nextInt(20000) * (... ) + 10000)};
+            float ox = (float) (Globals::rnd->nextInt(20000) * (... ) + 10000);
+            float oy = (float) (Globals::rnd->nextInt(20000) * (... ) + 10000);
+            float oz = (float) (Globals::rnd->nextInt(20000) * (... ) + 10000);
+            Vector pos = {pirateBase.x + ox, pirateBase.y + oy, pirateBase.z + oz};
```

This shortens the `0xb3b74` run from 152 to 124 B (the `movw fp,#10000` vs
`mov fp,sl; movw sl,#10000` register swap is still there, but the surrounding schedule changes).
Semantics are unchanged: each component is still `pirateBase.{x,y,z} + (float)(int_expr)`.

### 3. Move the case-2 `Vector asteroidPos` declaration after `coords` (raw -7)

```cpp
-                    Vector asteroidPos = {0.0f, 0.0f, 0.0f};
                     int coords[6] = {...};
+                    Vector asteroidPos = {0.0f, 0.0f, 0.0f};
```

`asteroidPos` is only assigned/used in the second convoy loop; moving the zero-init after the
`coords` initialiser matches the original's store schedule and shortens the `0xb1688` run.
Semantics are unchanged (it is dead until the second loop).

## Negative results (all real `link_so` + `fn_diff` builds)

The sweep re-ran the wave-178 bool/type/order matrix on top of the null-check win; the base remains
the local optimum for every one:

- bool declaration order: BAP 3844, BPA 6685, ABP 8803, APB 10392 (size -8), PBA 11966 (+8),
  PAB 11848 (+8).
- all-`int` flags 5688 (+4); `raid` `int` 5344 (-4); `hasWanted` `int` 12539 (-24);
  `siegedStation` `int` 16460 (+136); `convoy` `bool` 15524 (+8); `nameLeader` `int` 13461 (+8).
- `unsigned` for `stationIndex`/`localRace`/`raidRace`/`extraFriend`/`convoy`/`alienGuns`/
  `stationRace`/`alienRace`/`race`/`x`/`k`/`raidType`/`guns`/`first`/`escortsEnd`/`start`/`slot`/
  `index`/`hunterCount`: all codegen-identical to base (3844).  `raidRoll`/`pirates`/
  `pirateEscorts`/`extraAliens`/`fortressGuns`/`level`/`count`: 3845-3848.  `security` 4492.
  `alienPercent` 5062.
- `hunters` `double` 4636 (+4); `alienChance` `double` 13565 (+8); `base` `double` 16032 (+24).
- `float` literal `10000.0f` in the escort position: 6131 (+48).  Separate `int ox/oy/oz`:
  codegen-identical.  `int off = 10000;` outside the loop: codegen-identical.
- `t = (bigAlienWave + (i < 0)) ? ...` alternatives: plain ternary 5485 (-12), `||` 5099 (-4),
  `!= 0` 5485 (-12), `+ (i<0) != 0` / `if` form: identical to base.
- `(security == 3) & raid` -> `&&` 14755 (+24); reversed operands identical.
- extraAliens loop: inline `start` 4660 (-8), `start` unsigned 3845, `pirates`-first bound 4658
  (-12), `base` local 4948 (-8).
- case4Tail: dropping the `Array<KIPlayer*> *enemies` local 3999 (-4), dropping the
  `Globals *globals` local 3949, `AERandom *rnd` local 4014 (-8); `int i` identical.
- case-2 `enemies` local placement: direct `this->enemies` 10699 (+8), local only for the calls
  5569, mixed 5569.
- `Globals::status`/`rnd`/`globals`/`gameText` hoisted to function-top locals: 16711 / 16457 /
  15560 / build-fail (all far worse) — the original really does use the globals directly.
- `mission == nullptr` -> `!mission`, `freelance != nullptr` -> `freelance`, etc.: all identical.
- `raid`/`extraFriend` initialisers: identical.

## Residual (unchanged)

The remaining raw is dominated by scheduling/register-allocation differences that are not
source-controllable from this sweep:

- `0xb1958` (148 B): the case-2 `Array<KIPlayer*> *enemies` local is cached in r4 across the
  `setSpeed`/`setRotationSpeed` virtual calls; the original reloads `this->enemies` from r5 each
  time.  Removing the local costs +8 B.
- `0xb189f`/`0xb1742`/`0xb1688` (135/111/105 B): `createShip(..., true, false)` argument-store
  scheduling and `enemyRoute` register caching.
- `0xb328a` (125 B): flag stack-slot rotation (`bigAlienWave` at `sp+56` vs the original `sp+64`)
  feeding the `kAlienStationTurrets`/`kAlienOutpostTurrets` select.
- `0xb3b74` (124 B): `movw fp,#10000` preheader hoist vs `mov fp,sl; movw sl,#10000` in-loop.

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12312
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch tooling: `/home/debian/GalaxyOnFire2/scratch/w12312` (`batch.py`, `measure.py`,
`rawruns.py`, `diffdetail.py`, `cmp.py`, `textdiff.py`).  Pristine base copy: `Level.base.cpp`;
landed copy: `Level.best.cpp`; base build `base.so`.
