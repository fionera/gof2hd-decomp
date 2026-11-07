# w8112 (wave-136): `Level::createMission` — call-order proof, branchless `&&`, tail register-map study (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8112`, based on master `2e51d765d`.
Continues `level-createmission-wave135-w7776.md`, `…-wave134-w7756.md`,
`…-wave133-w7733.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17172 B).
Manual Thumb disassembly (objdump) plus the linked `.so`'s own line table
(`objdump --dwarf=decodedline` on `Level.cpp.obj`, section
`.text._ZN5Level13createMissionEv`, address = linked addr − `0xaffd8`).
Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.1 % (report row)   size 17172/17172 B
.text diff_bytes 84960 (whole section)
.ARM.extab diff 1404   .ARM.exidx/.rodata/.data/.bss/.rel.plt exact
.note.gnu.build-id diff 20
raw fn differing bytes 5761 (same-offset byte diff inside the 17172-byte symbol)
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

`raw fn differing bytes` (positional byte diff of the symbol body) is the A/B
metric: it is exactly the part of the section diff that lives inside this
function.

## New method: the call sequence is already correct except for two statements

Extracting every `bl`/`blx <symbol>` in order from both disassemblies gives
**677 calls in each**, and a `difflib` alignment over the symbol names shows
exactly **two** reorderings in the whole 17172-byte function:

```text
1. PlayerTurret::setScaling
   original b336a  (after gun->setPosition)
   ours     b330a  (between the first rotate and geometry->setRotation)
2. Globals::getRandomEnemyFighter
   original b4174  (before the waypoint index; argument order of the one-liner)
   ours     b4188  (after the waypoint index; the `int idx` temp evaluates it first)
```

Every other byte of the residual is register allocation / instruction
scheduling, not statement order.  This bounds what a source edit can still win:
only these two statements can be moved.

## New hypothesis 1: the friend-loop `&&` conditions are branchless in the original

The original compiles both

```cpp
if (i == this->friendCount + this->field_164 && alienWave) { … }
else if (i == this->friendCount + this->field_164 && pirateFortress) { … }
```

**branchlessly** (if-converted):

```text
b2e04: subs.w r0, fp, r3        ; fp = i, r3 = friendCount+field_164
b2e08: mov    r1, r0
b2e0a: it     ne
b2e0c: movne  r1, #1            ; r1 = (i != X)
b2e0e: ldr    r2, [sp, #60]     ; alienWave
b2e10: eor.w  r2, r2, #1        ; !alienWave
b2e14: orrs   r1, r2            ; (i != X) | !alienWave
b2e16: bne.w  b2f2a             ; if (!(i==X && alienWave)) goto else-if
```

and the same shape again at `b2f2a` for `pirateFortress`.  Ours emits the
short-circuit form:

```text
b2e04: ldr r0,[sp,#60] ; cmp r0,#0 ; beq else-if
b2e0c: cmp fp,r3       ; bne else-if
```

This is a real codegen difference, but it is not steerable from the source
under clang 7.0.2.  Measured (fresh `link_so` each):

| spelling | raw fn bytes | size |
|---|---:|---:|
| **checked-in `&&` (short-circuit)** | **5761** | **17172** |
| `(i == X) & alienWave` (both conditions) | 14726 | 17196 |
| `(i == X) & alienWave` (first only) | 12882 | 17172 |
| `(i == X) & (flag != 0)` (both) | 16542 | 17312 |
| `!(i != X || !flag)` (both) | 5761 | 17172 |
| `!!(i == X && flag)` (both) | 5761 | 17172 |
| `(i == X) ? flag : 0` (both) | 5870 | 17172 |
| `bool leader = i == X; leader && flag` (both) | 5761 | 17172 |
| `flag && i == X` (both, swapped) | 5761 | 17172 |

`&` (bitwise) produces a *different* branchless shape (it keeps `(i==X)` and
`ands`, not the negated `(i!=X) | !flag`), so it does not reproduce the
original and is much worse.  The if-conversion is a backend profitability
decision that no spelling tested flips.

## New hypothesis 2: the one-liner tail reproduces the original register map

The original tail loop (`b416e..b41d0`) uses

```text
this          = r8   (reloaded at case4Else: ldr.w r8,[sp,#124] @ b40b6)
i             = r9
Globals::globals = sl
&Globals::rnd = [sp,#120]  (spilled; re-read every iteration)
true          = fp   (mov.w fp,#1 hoisted; strd r0,fp,[sp])
loop condition: ldr.w r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc
```

The checked-in `int idx` formulation gives `this=r9, i=r8, globals=sl, rnd=r4`
(i.e. `this`/`i` swapped).  Reverting to the **one-liner**

```cpp
(*this->enemies)[i] = this->createShip(
        8, 0, Globals::globals->getRandomEnemyFighter(8),
        this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
```

gives exactly `this=r8, i=r9, globals=sl` — the original's map — and also
restores the original's evaluation order (`getRandomEnemyFighter` first).  The
only remaining tail difference is that `&Globals::rnd` then lives in `fp`
instead of `[sp,#120]`, so `true` is materialised as `movs r0,#1; str r0,[sp,#4]`
instead of the original's `strd r0,fp,[sp]`.  That costs **2 bytes**:
`raw fn 5761 -> 5763`, size still 17172.  The checked-in `int idx` shape is
therefore kept (it is 2 bytes closer even though its register map is swapped).

Making `true`/`false` locals (`bool hostile = true; bool friendly = false;`,
`const bool`, `int`) is neutral (5763) — clang coalesces them back to literals.

## New hypothesis 3: the faithful loop condition flips `this` back to `sl`

Writing the loop condition as `i < this->enemies->size()` (re-read every
iteration, exactly like the original) instead of the `volatile unsigned n`
latch gives:

```text
this = sl, i = r9, Globals::globals = r8, &Globals::rnd = fp
loop condition: ldr.w r0,[sl,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc   (10 B, matches)
size 17164   raw fn 5896
```

The condition bytes are now identical to the original, but the function loses
the `case4Else` `this` reload (`ldr.w r8,[sp,#124]`, 4 B) and 4 more bytes
elsewhere, i.e. it is **8 bytes short**.  The `volatile unsigned n` latch is
the cheapest size-exact way to hold 17172, and it is what forces `this` into
`r8`.  Combining the faithful condition with a faithful turret order is still
17164.

## Other variants measured this session (fresh `link_so` each)

| variant | raw fn bytes | size |
|---|---:|---:|
| **baseline (`int idx` + `volatile n`)** | **5761** | **17172** |
| one-liner tail (original eval order + register map) | 5763 | 17172 |
| one-liner + `bool hostile/friendly` locals | 5763 | 17172 |
| `int type` temp then one-liner | 5763 | 17172 |
| `unsigned type` temp then one-liner | 5763 | 17172 |
| `int type; int idx;` | 5880 | 17172 |
| faithful `this->enemies->size()` condition | 5896 | 17164 |
| faithful condition + faithful turret order | 6046 | 17164 |
| faithful condition + fresh-receiver turret order | 5997 | 17164 |
| non-volatile `unsigned n` latch + one-liner | 5769 | 17172 |
| `Level *self`/`*volatile self` alias in case4Else+tail | coalesced / jump-crosses-init | — |
| turret: faithful order, `gun` receiver, cargo→shipGroup | 5911 | 17172 |
| turret: faithful order, fresh receiver, cargo→shipGroup | 5862 | 17172 |
| turret: checked-in order, fresh receiver, cargo→shipGroup | 5763 | 17172 |

The turret numbers confirm wave 135: the checked-in mid-loop `setScaling`
through a fresh `(*enemies)[i]` load with `shipGroup` before `cargo` is the
cheapest shape (5761); the faithful order is 150 bytes worse.

## Conclusion (no source win this wave)

The residual is now fully characterised:

1. **Statement order**: exactly two statements are out of order (turret
   `setScaling`, tail `getRandomEnemyFighter`).  Both faithful reorderings are
   *worse* under clang 7.0.2 because they disturb the register allocation.
2. **Tail register map**: the one-liner reproduces `this=r8 / i=r9 /
   globals=sl`, but the `true`-in-`fp` / `&rnd`-spilled combination cannot be
   reached from source; the `int idx` shape is 2 bytes closer overall.
3. **Branchless `&&`** in the friend loop is a clang if-conversion decision;
   `&`, `!(…)`, `!!(…)`, ternaries and swapped operands are neutral or worse.
4. **`raid`@84 vs saved-`fp`@88** stack-slot swap (first byte difference at
   `b0024`) and the **case-183 body layout 0,1,2 vs 0,2,1** (literal-pool
   reorder) remain backend decisions, unchanged from waves 133–135.

The checked-in source is locally optimal; no change is landed.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8112/rawfn.py      # positional byte diff inside the symbol
python3 /dev/shm/w8112/runs.py       # contiguous differing-byte runs
python3 /dev/shm/w8112/linemap2.py   # addr->source line from the .obj line table
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w8112/{Level.base.cpp,rawfn.py,measure.sh,runs.py,
linemap2.py,variant.py,t1.py..t9.py,orig.asm,ours.asm}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      84960 -> 84960
.ARM.extab diff  1404 -> 1404
```
