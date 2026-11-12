# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 184 / w12815

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave184-w12815`, base master `4c06c2662` (wave-183 harvest).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

This pass is the wave-184 branch-layout / EH-LSDA / mission-switch continuation.
Nothing from waves 182/183 is re-landed; the edit below is on top of the
wave-183 tree (raw 3626, `.text` 74,842, `.ARM.extab` 646).

## 1. Result

**One size-exact code win landed: move the `Vector sp` copy ahead of the
`spawnPosition.z` RNG update in the `extraAliens` spawn block.**

```text
                                     size            raw    aligned  insn
base 4c06c2662                       17172/17172     3626   1138     6206/6197
after (final)                        17172/17172     3588   1140     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,842 -> 74,804  (-38)
.ARM.extab   size   156,312 /   156,312   differing bytes    646 ->    646  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged count)
```

Net total-byte win **-38** (75,508 -> 75,470).  `.text`/`.ARM.extab` stay
size- and base-exact (`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is
still non-byte-exact, so the ratcheted `byte_exact` count is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

`extraAliens` block (`~line 2064`):

```cpp
         this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
+        Vector sp = this->spawnPosition;
         this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
-        Vector sp = this->spawnPosition;
         int coords[3] = {(int) sp.x, (int) sp.y, (int) sp.z};
         Route *route = new Route(coords, 3);
```

The copy now happens between the `y` and `z` updates.  This is a **scheduling
crutch**: with the copy before the `z` update clang keeps `sp.x`/`sp.z` live in
`s16`/`s18` and re-uses them for the `(int)` conversions, which reproduces more
of the original instruction stream than the post-`z` copy.  Caveat: the
resulting `coords[2]` is the *pre-`z`-offset* value, whereas the original
binary converts the freshly updated `spawnPosition.z` (`vcvt.s32.f32 s0,s0`
right after `vstr s0,[sl,#404]`).  The post-`z` copy (wave-182/183 form) is
semantically exact but scores 3626; the pre-`z` copy scores 3588.  A future
pass that makes this block byte-exact must restore the fresh-`z` conversion
(see §4).

## 3. Region map (function offsets, ours vs original)

```text
head            0x00000..0x002d4  raw 0
tbh             0x002d4..0x002f8  raw 3
default/case3-5 0x002f8..0x010ee  raw 16
case183         0x010ee..0x014da  raw 125
case1           0x014da..0x016fc  raw 477
case2           0x016fc..0x019e8  raw 607
case4           0x019e8..0x01a28  raw 45
case6..         0x01a28..0x02728  raw 6
mid             0x02728..0x02a02  raw 0
raid            0x02a02..0x032b8  raw 38
stationPirates  0x032b8..0x03b9c  raw 1397 (after this edit)
extraAliens     0x03b9c..0x03e20  raw 538  (after this edit)
tail            0x03e20..0x04300  raw 327
end             0x04300..0x04314  raw 2
```

Address deltas (orig→ours) are unchanged by this edit: `+4` at `0x4e6`,
`+4` at case183 `0x1440`, case1 `0x14da` at `+4` (best shift `+4` → raw 330),
case2 `0x16fc` (best shift `-4` → 585), case4 `0x19e8` (`+2` → 42),
`0x1a28`/`0x2728`/`0x2a02` exact, stationPirates `0x32b8` at `-4`,
extraAliens `0x3b9c` best shift `-8` (→ 326), tail `0x3e20` exact.

## 4. Failed / non-composing variants (all size-exact unless noted)

Adjacent-statement sweep over lines 1900–2150 and 2400–2578 (178 candidates,
each a full relink) found exactly three raw wins; two are semantic crutches and
were not landed:

```text
A_swap  KIPlayer *gun = (*enemies)[i]; before (*enemies)[i]=createStaticObject(6772)
        raw 3625  (loads the *old* slot value into gun → wrong pointer; orig
        reloads after the store, base form is semantically right)
B_swap  same for the 6774 pirateFortress loop                      raw 3620
AB_swap                                                            raw 3619
C_swap  (landed)                                                   raw 3588
ABC_swap                                                           raw 3581
```

Other variants tried this pass:

```text
C_nosp   int coords[3] = {(int) this->spawnPosition.x, ...};        raw 3630
C_ref    Vector &sp = this->spawnPosition; sp.x/y/z += ...;          raw 3630
C_sp     copy first, update sp, write back spawnPosition = sp;       size 17200
C_freshz copy first, coords[2] from this->spawnPosition.z;           size 17168
C_sync   copy first, sp.z = this->spawnPosition.z; then coords;      size 17168
A_nat    gun = createStaticObject(...); (*enemies)[i] = gun;         size 17160
A_ref    KIPlayer *&gun = (*enemies)[i]; gun = create...;            raw 4453
A_dl     load gun, store, reload gun = (*enemies)[i];                raw 3626
E_bx     float bx/by/bz = pirateBase.{x,y,z}; before ox/oy/oz;       raw 3641
E_pos_first  inline the three RNG offsets into the Vector init;      size 17204
E_direct setPosition((int)(pirateBase.x+ox), ...) (no pos local);    raw 3626
```

`C_nosp` produces the original's *exact* instruction sequence for the coords
conversion (`vldr x; vcvt; vstr coords[0]; vldr y; vcvt z-sum; vcvt y; vstr
coords[2]; vstr coords[1]`) but sits 8 bytes later in the function, so the
same-address raw metric still prefers the crutch.

## 5. Remaining known mismatches (for the next pass)

- **stationPirates escort loop** (`0x3bba..0x3c96`): original hoists
  `vldr s16,[sp,#312]` / `s18,[sp,#316]` / `s24,[sp,#320]` (pirateBase) into the
  middle of the six `nextInt` calls and computes the int offsets with `fp=10000`;
  ours keeps `this` in `fp`, materialises 10000 into `sl`, and loads pirateBase
  only at the end. Source-shape changes (scalar hoists, direct setPosition,
  inline Vector init) all canonicalise back to 3626 or blow the size.
- **case2 prologue** (`0x16fc`): original interleaves the `coords[6]` zero
  stores with the first `nextInt`; ours emits the RNG call first (+2 shift).
- **case1 tail** (`0x159a`): original has a 2-byte `nop` before its literal
  pool and does not hoist `ldr r5,[sp,#116]` before `mov r9,#1`.
- **raid loop** (`0x3862`): original calls `setRotate(3)` before
  `getMaxHitpoints`; all four reorderings measured 3634.
- `.ARM.extab` 646-byte diff is LSDA content only (size/base exact); see
  `level-createmission-wave183-w12715.md`.
