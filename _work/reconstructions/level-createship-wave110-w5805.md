# Level::createShip — wave 110 (w5805): the 332-byte LSDA, and why +4 is the only usable delta

**Status:** investigation only — **no source change landed** (the wave-109 tuned
spawn at 74.4 % is still the best measured shape). This note records the
wave-110 measurements, the record-size arithmetic, and one new mechanism that
explains why *every* LSDA experiment except an exact +4 bytes fails the gate.

Target `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`).

## 0. Baseline (verified twice this session, `check` target)

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 0x1337c8  .ARM.exidx 0x6f70  .ARM.extab 0x26294 (4 B short of 0x26298)
createShip record 328 B / 54 rows / tt_enc 0x00 / cslen 299
```

## 1. The record-size arithmetic (new, and it constrains the whole search)

The `.ARM.extab` entry for createShip is `16-byte prefix + LSDA` (the prefix is
`prel31 __gxx_personality_v0` plus 12 fixed bytes; the 12 bytes are identical
between ours and the original and the prel31 only differs by the record's own
address, so the prefix never matters).  For the current shape
(`tt_enc = 0x00`, one catch-all action, one type-table entry):

```text
rec = 16 + 1(lp_enc) + 1(tt_enc) + 2(ttoff uleb) + 1(cs_enc)
        + 2(cslen uleb) + cslen + 2(action) + 4(type table)
    = 29 + cslen
```

`cslen` is the byte length of the call-site table.  Records are 4-byte aligned
(`EmitAlignment(2)`), so

```text
rec 328  <=>  cslen 299          (measured)
rec 332  <=>  cslen in [300,303]  <-- the target
rec 336  <=>  cslen in [304,307]
```

For the faithful `tt_enc = 0xff` shape (no action table, no type table) the
header is 21 bytes: `rec = 21 + cslen`, so 332 needs `cslen in [308,311]`
(the original's faithful table is `cslen = 315`, i.e. 336).

## 2. NEW mechanism: any extab delta other than +4 shifts later sections

`.ARM.extab` sits before `.data.rel.ro`, `.dynamic`, `.got` and `.rel.dyn`.
The current `.ARM.extab` is exactly **4 bytes short**, so an LSDA change of
`+4` makes the section exact and leaves every later address alone.  Any other
delta shifts `.data.rel.ro`/`.got`, which changes every `R_ARM_GOT_PREL`
literal-pool word in the image, and `byte_exact` collapses.  Measured
(full link, `bytecount.py`):

| variant | extab delta | `.ARM.extab` | byte_exact |
|---|---:|---:|---:|
| base | 0 | 156308 | **4500** |
| `try { jitterY = ... } catch (...)` | +12 | 156320 | 3428 |
| `try { player = new Player } catch (...)` | +8 | 156316 | 2363 |
| `try { fixed->setShipGroup } catch (...)` | +12 | 156320 | 3428 |
| `shipClass != 0` (empTime branch) | +4 | **156312** | 4485 (only because `.text` also shrank 4) |
| `catch (int)` | +4 | 156312 | 1775 (GOT slot moves) |

**Consequence:** the gate (`byte_exact >= 4500`) can only survive a createShip
change if the record becomes *exactly* 332 bytes (or stays 328).  A 336-byte
record needs a compensating −4 elsewhere (the wave-107 Hud route); anything
else is a hard ratchet failure.

## 3. Why +4 is the only reachable delta (row arithmetic)

Each call-site row is `uleb(start) uleb(len) uleb(landingpad) uleb(action)`.
In this function start/landing-pad are always ≥ 128, so

* cleanup row = 2 + 1 + 2 + 1 = **6 B**,
* gap row = 2 + 1 + 1 + 1 = **5 B** (6 B when `len >= 128`),
* the *first* gap row is `(0, 406, 0, 0)` = 1 + 2 + 1 + 1 = **5 B**.

Base table: 27 cleanup rows × 6 + 27 gap rows = 162 + 137 = **299**.

Adding one try-range at `[p, q)` (p < 128 so the new leading gap row is 4 B):

| case | rows | delta |
|---|---|---:|
| q < 128, throwing call before next range | 4 + 5 + 5 − 5 | +9 |
| q ≥ 128, throwing call before next range | 4 + 6 + 6 − 5 | +11 |
| q = next range start (no gap row) | 4 + 6 − 5 | +5 |
| q < 128 **and** no throwing call in [q,next) | 4 + 5 − 5 | **+4** |
| new range merges with the next row (same pad+action) | 4 + 6 − 5 − 6 | −1 |

The `+4` case needs a try-range that begins and ends below offset 128 **and**
is followed by a call-free region up to the next invoke — impossible here,
because the next invoke (`::operator new(sizeof(Player))`) is at offset 406 and
every byte between contains `nextInt`/`getLevel`/`gameWon` calls.

The only other `+4` is a **null-landing-pad try-range** (a `LandingPadInfo`
with `LandingPadBlock == nullptr`, the LLVM "nounwind case"): it emits the
leading 4-byte gap row and *no* call-site row, and if it ends exactly at 406
no trailing gap row either → exactly +4.  But `SelectionDAGBuilder::visitInvoke`
always calls `MF.addInvoke(FuncInfo.MBBMap[EHPadBB], ...)` with a non-null
block, and `MachineFunction::tidyLandingPads` *erases* null-label pads that
have a block.  No C++ statement reaches the null-block case.

Everything else is quantised to +5/+6/+9/+11 (measured below), i.e. 333/336/340.

## 4. Measured table (this session, full link, official verifier)

`pct` = official fuzzy, `text` = body bytes, `rec`/`cslen` from the object's
`.ARM.extab.text._ZN5Level10createShip…` section, `byte` = whole-.so
`byte_exact`.  Base row for comparison: **74.4 / 2764 / rec 328 / cslen 299 /
byte 4500**.

| variant | pct | text | rec | cslen | byte |
|---|---:|---:|---:|---:|---:|
| base (wave-109 tuned spawn) | **74.4** | 2764 | 328 | 299 | **4500** |
| `if (shipClass < 1)` for the fighter branch | 74.4 | 2764 | 328 | 299 | 4500 |
| `switch(type)` → if-chain | 74.4 | 2764 | 328 | 299 | 4500 |
| `mission>=49 && mission<=52 \|\| mission==56` | 74.4 | 2764 | 328 | 299 | 4500 |
| `int empTime = 0;` | 74.4 | 2764 | 328 | 299 | 4500 |
| `int empTime = 15000;` + no else | 74.4 | 2764 | 328 | 299 | 4500 |
| `int sy/sz` int temps before float y/z | 74.4 | 2764 | 328 | 299 | 4500 |
| `x = jitterZ+jitterX; x -= 20000;` | 74.4 | 2764 | 328 | 299 | 4500 |
| `try { delete fighter->cargo; } catch (...)` | 74.4 | 2764 | 328 | 299 | 4500 |
| ship decl before `new Player` | 74.4 | 2764 | 328 | 299 | 4500 |
| spawn after the try | 72.4 | 2764 | 328 | 299 | 4500 |
| `empTime = shipClass==1 ? 45000 : 15000` (hoisted) | 72.3 | 2764 | 328 | 299 | 4500 |
| **semantically correct spawn** (`wx+jx, wy+jy, wz+jz`) | 73.9 | 2764 | 328 | 299 | 4500 |
| `try { jitterY = nextInt } catch (...)` | 62.0 | 2764 | 340 | 308 | 3428 |
| `try { fixed->setShipGroup } catch (...)` (empty handler) | 65.8 | 2764 | 340 | 310 | 3428 |
| `try { lodManager->addObject(fixed->geometry) } catch` | 66.0 | 2764 | 340 | 310 | 3428 |
| `try { player = new Player } catch (...)` | 61.4 | 2788 | 336 | 305 | 2363 |
| `try { player = new Player } catch` + plain setEmpData | 61.1 | 2800 | **332** | 300 | 2372 |
| `try { fixed = ::operator new } catch (...)` | 61.0 | 2768 | 336 | 305 | 2363 |
| `try { new (fighter) PlayerFighter } catch` | 66.4 | 2772 | 340 | 310 | 2363 |
| `try { new (fixed) PlayerFixedObject } catch` | 66.1 | 2772 | 340 | 310 | 2363 |
| `try { ArraySetLength(11,...) } catch` | 66.3 | 2772 | 336 | 305 | 2363 |
| `try { fixed->setWreckedMeshId(0x4780) } catch` | 66.2 | 2772 | 336 | 305 | 2363 |
| `try { (*volumes)[0] = new BoundingAAB } catch` | 65.0 | 2784 | 336 | 305 | 2363 |
| whole fighter branch in one try | 45.0 | 2748 | 348 | 316 | 2827 |
| faithful (real `new` both, no try, switch) | 64.6 | 2768 | 336 | 315 | 2363 |
| faithful + `empTime` try | 66.3 | 2780 | 352 | 321 | 2363 |
| `catch (int)` | 66.3 | 2772 | **332** | 300 | 1775 |
| `catch (std::exception&)` | 66.3 | 2772 | **332** | 300 | 2372 |
| `if (shipClass != 0)` (empTime branch) | 39.3 | 2760 | **332** | 300 | 4485 |
| `if (shipClass != 0)` + `else if (shipClass != 0)` | 35.1 | 2700 | **332** | 300 | 2514 |
| `if (shipClass != 0)` + a trailing call | 42.8 | 2772 | **332** | 300 | 2372 |

Every `rec == 332` shape found has `cslen == 300` (the +1 comes from a gap
row's length crossing 128 after a large code-layout change), and every one of
them also changes `.text` (2760 / 2700 / 2772 / 2800), so `byte_exact` drops.

## 5. The `+1` route (closest miss)

`if (shipClass != 0)` for the empTime branch is the only cheap source of
`cslen = 300`: it moves the `if (shipClass == 0)` block, growing the gap after
the `setEmpData` invoke from 84 to 240 bytes, which costs one extra ULEB byte.
It also shrinks `.text` by 4 (2760).  Adding 4 bytes back at the tail is not
possible from source: `return ship ? (PlayerFixedObject*)ship : nullptr;`,
`if (ship == nullptr) return nullptr;` and `if (ship) ship->setLevel(this);`
are all optimised to the same code or change the size by more than 4.

## 6. Proven vs hypothesis

**Proven (measured):**
1. `rec = align4(29 + cslen)` for the `tt_enc = 0x00` shape; the 332 target is
   exactly `cslen in [300,303]`.
2. Any `.ARM.extab` delta other than +4 shifts `.got` and collapses
   `byte_exact` (4500 → 2363…3428).  A 336-byte createShip needs a −4
   compensation elsewhere.
3. Row deltas from adding a try-range are quantised: +5 (q = next range),
   +6 (one extra `new` invoke), +9/+11 (new gap rows), −1 (merge).
4. The only `+4` shapes need either a try-range fully below offset 128 with a
   call-free tail (impossible: the next invoke is at 406) or a null-landing-pad
   range (unreachable from C++ in LLVM 7).
5. `catch (int)` / `catch (std::exception&)` do produce `cslen = 300` → 332,
   but the LSDA type-table `R_ARM_TARGET2` allocates the typeinfo GOT slot at
   Level.cpp's link position (0x210364 / 0x210368) and shifts ~413 later GOT
   slots → `byte_exact` 1775 / 2372.
6. The wave-109 tuned spawn (74.4) is a stable local optimum: `switch`→if,
   condition rewrites, `empTime` initialisation, int-temp spawns and the
   semantically correct spawn all measure 74.4 or lower.

**Hypothesis (not proven):**
* A 332-byte record at `text = 2764` and fuzzy ≥ 74.4 does not exist for
  clang 7 `-Oz`; the structural fix is the wave-107 route (faithful 336-byte
  createShip **plus** a −4 Hud::draw dock reorder), which trades createShip's
  fuzzy (74.4 → ~64.6–66.4) for an exact `.ARM.extab`.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5805
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

**Fast per-variant loop (≈2.4 s/variant, used for every row in §4).**
Compile only `Level.cpp` with the exact command from
`cmake-build-match/compile_commands.json` and read the per-function sections
`-ffunction-sections` gives it:

```sh
clang++ <Level.cpp flags from compile_commands.json> -o /tmp/L.o -c src/game/world/Level.cpp
arm-linux-androideabi-objdump -h /tmp/L.o | grep -E \
  '\.text\._ZN5Level10createShip|\.ARM\.extab\.text\._ZN5Level10createShip'
```

`.text.…createShip` = body size (must stay `0xacc` = 2764);
`.ARM.extab.text.…createShip` = record size (328 today, 332 wanted).  A full
`link_so` is only 2.2 s, so `pct`/`byte_exact` are cheap to confirm with
`tools/verify/verify.py --show` and `bytecount`-style section comparison.

Scratch harnesses (this pass, `/dev/shm/w5805/fast/`): `fast.py` (compile-only
size probe), `full.py` (write → `link_so` → pct/extab/byte), `rows.py` (LSDA
row decoder for the object file), `batchA..M.py`, `grid.py` (80-way
`shipClass` condition grid).
