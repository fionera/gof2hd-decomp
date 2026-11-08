# w10209 (wave 156): `Level::createMission` — temp=r5 landed on wave-155 master;
# `GOF2_GOLD_PAD_CAP` wired cleanly; no `.text` pad growth

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10209`, based on master
`cf74f4ee6` (wave 155). Continues `level-createmission-wave155-w10107.md`,
`…-wave154-w10011.md`, `…-wave153-w9911.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP is
unavailable in this environment (`rea capabilities` -> `executable_missing`), so the NDK
`objdump` disassembly is the authority. Raw same-address diff is
`/dev/shm/w10209/fdiff.py`; instruction-index alignment is
`/dev/shm/w10209/align_diff.py` (worktree-local copies of the shared tools, with `OURS`
pointed at this worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
.text 0x1337c8 (exact)   .text diff 86,044   .ARM.extab 0x26298 diff 1,292
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

The wave-155 source already had the w9911 `v`-before-`rotation` declaration order and the
w10011 `Vector p2` second rotate; the first rotate was still the unnamed
`v = MatrixRotateVector(...)` and the alien loop still carried the non-original
`gun->field_0x70 = 0;` store.

## 1. Landed source (`src/game/world/Level.cpp`, alien-gun loop + raid prologue)

```diff
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
             gun->cargo = nullptr;
```

and the raid-prologue status read (the companion):

```diff
-        Status *status = *(Status *volatile *) &Globals::status;
+        Status *volatile status = Globals::status;
```

The first change is the only 17,172-B source form found that puts the first
`MatrixRotateVector` sret temp in **r5** (the original's `sp+212` slot).  The second is a
pure companion: it keeps the size at 17,172 B and improves the instruction-index
alignment by 14 (1,210 -> 1,196) with no raw change.  The original alien loop has no
`field_0x70` store and exactly one `cargo = nullptr`; dropping the probe is faithful.

## 2. `.text` pad: the landed shape does NOT grow it

On this wave-155 master the temp=r5 shape links at `.text 0x1337c8`, exactly the original
size.  w10107 measured a +0x20 pad on its wave-154 base; that does not reproduce here.
`GOF2_GOLD_PAD_CAP` is nevertheless wired (see §3) so a future shape that does cross the
boundary can be matched without a second link pass.

Verified both directions of the knob in this worktree:

```text
-DGOF2_GOLD_PAD_CAP=0x40 (default, stock gold)  .text 0x1337c8  (original)
-DGOF2_GOLD_PAD_CAP=0x20 (patched gold)         .text 0x1337a8  (-0x20)
```

## 3. `GOF2_GOLD_PAD_CAP` wired into `CMakeLists.txt`

`tools/patch_gold_pad.py` (wave 152) rewrites gold's hard-coded ARM stub-table padding cap
(`min(delta*0.02, 0x40)`).  It is now a cache variable:

```cmake
set(GOF2_GOLD_PAD_CAP "0x40" CACHE STRING "gold ARM stub-table auto-padding cap (hex)")
```

At the default `0x40` the stock `${GOF2_NDK_GOLD}` is used unchanged (no copy, no patch,
byte-identical link).  Any other value copies gold into the build dir, patches it, and
links with the copy; the patched linker is a build dependency.  The link command line is
unchanged (no new link flags).

## 4. Register map of the landed alien loop

```text
        temp1       temp2       this        gun   i          k     t     rotation    v
ORIG    r5(sp+212)  r5(sp+212)  r4(reload)  r6    fp         r9    sl    r4(sp+312)  r4(sp+372)
OURS    r5(sp+212)  r8(sp+200)  r5(reload)  r4    r4/sp+116  r9    r6    r8(sp+312)  fp(sp+372)
```

`temp1` matches (r5, `sp+212`).  `temp2` is still a second slot (`sp+200`, r8) because
the named `p`/`p2` pair splits stack colouring; the original reuses one `sp+212` slot for
both rotates and for the `setPosition` vector.  `this`/`gun`/`i`/`t` remain one rotation
off: the original keeps `t` in `sl` and `gun` in `r6`, ours keeps `t` in `r6` and `gun` in
`r4`, so `this` is displaced to `r5` and `i` is spilled to `sp+116`.

## 5. Companion sweep (all 17,172 B unless noted)

Every measurement below is on top of the landed temp=r5 shape; raw / align / size.

| variant | raw | align | size | temp 1/2 |
|---|---:|---:|---:|---|
| **landed: temp=r5 + `Status *volatile status`** | **6449** | **1196** | **17172** | **r5 / r8** |
| temp=r5 only, original cast status read | 6449 | 1210 | 17172 | r5 / r8 |
| `Status *status = Globals::status` (plain) | 6449 | 1210 | 17172 | r5 / r8 |
| `Status **status = &Globals::status` + `(*status)->…` | 6457 | 1198 | 17164 | r5 / r8 |
| `Status *&status = Globals::status` | 6457 | 1198 | 17164 | r5 / r8 |
| status read hoisted before the alien block (plain) | 6518 | 903 | 17184 | fp / fp |
| status read hoisted before the alien block (cast) | 6518 | 903 | 17184 | fp / fp |
| `__builtin_expect(alienGuns != 0, 1)` | 6469 | 1230 | 17172 | r5 / r8 |
| `(bigAlienWave \| (i < 0))` table select | 6513 | 1136 | 17168 | r5 / r8 |
| `(bigAlienWave ^ (i < 0))` / `- (i < 0)` / `+ (i >> 31)` | 6449 | 1196 | 17172 | r5 / r8 |
| `bigAlienWave ? … : …` (drops the `(i<0)` hack) | 6586 | 1246 | 17140 | r9 / fp |
| `v` before `rotation` | 6451 | 1209 | 17172 | r5 / r8 |
| `base` before `int k` | 6449 | 1196 | 17172 | r5 / r8 |
| `gun->cargo` before `gun->shipGroup` in the tail | 6449 | 1198 | 17172 | r5 / r8 |
| named `Vector pos` for `setPosition` | 6449 | 1196 | 17172 | r5 / r8 |
| `KIPlayer *gun = createStaticObject(...); enemies[i] = gun;` | 6335 | 1207 | 17172 | r5 / r8 |
| first rotate scoped `{ Vector p … }`, second unnamed | 5776 | 935 | 17168 | r9 / r9 |
| both rotates in scoped `Vector p` blocks | 5776 | 935 | 17168 | r9 / r9 |
| first `Vector p`, second unnamed | 6520 | 823 | 17184 | fp / r5 |
| first `Vector p`, second scoped `Vector p` | 6520 | 823 | 17184 | fp / r5 |
| reuse one `Vector p` for both rotates | 16589 | 3097 | 17312 | fp / r5 |

No companion recovers the raw metric while keeping temp1=r5 and size 17,172 B.  The
`pre_plain`/`pre_cast` hoist is the best alignment result (903) but is 12 B over the
original size (17,184 B) and moves both temps to `fp`; it is not landable without finding
the missing 12 B.  The `both_scoped_p` shape is 4 B short (17,168 B) and moves both temps
to `r9`.

## 6. Both orders (merge lesson)

The two landed edits are independent and commute in the final source.  Intermediate
measurements:

| order | shape | size | raw | align |
|---|---|---:|---:|---:|
| temp=r5 first | w10011 shape only | 17172 | 6449 | 1210 |
| companion first | baseline + `Status *volatile status` | 17176 | 5532 | 806 |
| **both (either order)** | **landed** | **17172** | **6449** | **1196** |

The companion alone grows `.text` to `0x1337e8` (gold stub-table pad +0x20); the combined
shape lands back at `0x1337c8`, so the default gold cap `0x40` is correct and no patch is
needed for this shape.

## 7. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6449   (+962)
                           align  774 -> 1196   (+422)
.text size             0x1337c8 -> 0x1337c8 (exact, no pad growth)
.text diff             86,044 -> 87,006  (+962)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,292 -> 1,304   (+12)
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.got     0 -> 0
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

## 8. Open questions

1. `temp2` must reuse `temp1`'s `sp+212` slot (and the `setPosition` vector must reuse it
   too).  The named `p`/`p2` pair splits the slot; scoped/unnamed forms merge it but move
   the temp out of r5.  A source form that both forces r5 and keeps one slot is still
   unknown.
2. The `(i < 0)` ternary hack is still present.  `bigAlienWave ? … : …` gives the
   original's `cbz` but shrinks the function to 17,140 B; the missing 32 B of genuine code
   is still unidentified.  The original builds the two input vectors with individual
   `ldr`/`str` pairs, while our `Vector{…}` braced init emits `ldmia`/`stmia`; that is a
   ~14 B difference and a likely part of the missing code.
3. The original's raid prologue keeps `&Globals::status` in r8 across
   `getRandomEnemyFighter` and derefs it afterwards; every source form tried either hoists
   the value (volatile/plain local) or loses 8 B (`Status **` / `Status *&`).  The
   `pre_plain` shape reaches align 903 but is 17,184 B.
