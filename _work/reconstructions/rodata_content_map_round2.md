# `.rodata` content map, round 2 (w3438)

Status: research note + one landed source reorder. Branch `wave87-w3438`, based on
master `8bf6a675` (`docs: lock byte_exact 4249 (Hud raceBadge alignment)`).
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

## TL;DR

The current `.rodata` is still size- and base-exact (`0x41b67 @ 0x1c9e10`), but
**13,308 of 269,159 bytes differ** (4.94%, 5,144 runs).  The first differing byte
is now VMA `0x1fc390`, the 190-byte `kMissionCase` table in `Status.cpp`.

One strict improvement was landed: moving `g_levelXPTable` and
`Status::checkForLevelUp` before the `kMissionOrder`/`kMissionCase` declarations.
It makes `0x1fc330..0x1fc390` byte-identical to the original and flips
`Status::checkForLevelUp` to byte-exact:

| | byte_exact | linked_exact | `.rodata` | first `.rodata` diff | Status diff bytes |
|---|---:|---:|---:|---:|---:|
| before | 4249 | 4496 | `0x41b67` | `0x1fc33d` | 256 |
| after  | **4250** | **4496** | `0x41b67` | `0x1fc390` | 224 |

No other function changed: `Status::checkForLevelUp` is the only
`byte False -> True` row.  `lint` is `CLEAN ✓`, `drift` is `0 field(s) across
0 class(es)`, and every ELF section size/base is unchanged.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

Baseline at `8bf6a675`:

```
verify: byte 4249  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (both binaries):

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

## 2. Method: attribute bytes to the owning TU

The shipped `.so` is stripped, so the input-section owner is recovered from a
scratch gold relink with `-Map` and without the final `strip`:

```
# take the exact ld.gold command from the build
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3438_link_cmd.txt

# replace the output path with
#   -Map=/tmp/w3438_rodata.map -o /tmp/w3438_rodata_unstripped.so
# and drop the trailing '&& ... arm-linux-androideabi-strip --strip-all'
# (the scratch script used for this note is /tmp/w3438_relink_map.sh)
```

Then:

```
python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3438_rodata_unstripped.so \
  --map /tmp/w3438_rodata.map

python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3438_rodata.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so
```

`rodata_content_map_round2.py` (committed with this note) parses the `-Map`
input-section table, counts differing bytes per input section, and searches the
original `.rodata` for the section's exact bytes.  `NONE` means the section's
content itself differs, not merely its address.

## 3. Current map (after the landed fix)

```
differing bytes 13308 / 269159 (4.94%)
runs 5144  first VMA 0x1fc390  last VMA 0x20377f
region 0x1fc390..0x203780  words 7420
common 7363  only-orig 57  only-ours 57
align: equal 6600  delete 556  insert 710  replace 45 (unequal 45); moved 763
differing symbols 175; no verbatim copy in original 1
  NOT-EXACT _ZL12kMissionCase  ours 0x1fc390 size 190
```

The 57/57 unique-word split is unchanged from the previous round: the remaining
bytes are overwhelmingly movement, not wrong values.  Only `kMissionCase` has no
verbatim copy anywhere in the original `.rodata`.

Top owners (full table is printed by the script):

| differing bytes | section size | owner | ours | first original occurrence |
|---:|---:|---|---:|---:|
| 4684 | 6132 | `o102_Level.cpp.obj` | `0x1fc490` | NONE |
| 4519 | 14288 | `o115_Globals.cpp.obj` | `0x1fe310` | `0x1fe2a0` |
| 808 | 1272 | `o137_StarSystem.cpp.obj` | `0x202270` | `0x202260` |
| 454 | 1532 | `o149_Gun.cpp.obj` | `0x202b14` | `0x202b04` |
| 343 | 800 | `o122_ImageFactory.cpp.obj` | `0x201af4` | NONE |
| 314 | 804 | `o136_ListItemWindow.cpp.obj` | `0x201f4c` | `0x201f34` |
| 302 | 413 | `o108_ModStation.cpp.obj` | `0x1fdfd0` | `0x1fdf40` |
| 268 | 540 | `o155_Achievements.cpp.obj` | `0x203144` | `0x203134` |
| 247 | 556 | `o103_StarMap.cpp.obj` | `0x1fdda4` | NONE |
| 224 | 340 | `o101_Status.cpp.obj` | `0x1fc330` | NONE |
| 167 | 320 | `o109_PlayerFighter.cpp.obj` | `0x1fe170` | `0x1fe100` |
| 156 | 943 | `o168_ObjectGun.cpp.obj` | `0x2033a8` | `0x203398` |
| 156 | 656 | `o145_MissionsWindow.cpp.obj` | `0x202868` | `0x202858` |
| 96 | 128 | `** merge strings` | `0x1fdc84` | NONE |
| 67 | 160 | `** merge constants` | `0x1fdd04` | NONE |

The `0x1fc390` blocker is structural: the original has no 190-byte table there.
It has the two `Status::getGammaRayDamagePerSecond` switch tables at
`0x1fc390`/`0x1fc3b0`, and `Level.cpp`'s plain `.rodata` starts at `0x1fc3d0`.
Our `kMissionCase` occupies `0x1fc390..0x1fc44e`, so `Level.cpp` starts at
`0x1fc490` and every later section is displaced.

## 4. Landed fix: `g_levelXPTable` / `checkForLevelUp` order

### Evidence

Original `Status::missionCompleted` (`0xabf38`) uses two in-text `tbh` jump
tables and contains no byte-table load:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe2: tbh   [pc, r1, lsl #1]      ; 40-entry table, types 150..189
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04e: tbh   [pc, r1, lsl #1]      ; 7-entry table, types 8..14
```

The original `.rodata` at `0x1fc330` is:

```
0x1fc330  kMedalCampaignMissions (12 bytes)
0x1fc33c  g_levelXPTable (0x54 bytes)
0x1fc390  Status::getGammaRayDamagePerSecond switch tables
0x1fc3d0  Level.cpp .rodata
```

Our baseline emitted `kMissionCase` at `0x1fc33c` and `g_levelXPTable` at
`0x1fc3fc`, so the first 0x60 bytes of the original Status block were wrong even
though the table contents themselves were correct.

### Change

In `src/game/mission/Status.cpp`, the `g_levelXPTable` declaration and the
`Status::checkForLevelUp` definition were moved above the
`kMissionOrder`/`kMissionCase` declarations.  This is a pure source-order move:
no values, signatures, or section flags changed.

### Measured A/B

Both variants are full `cmake --build cmake-build-match --target check -j8`
builds of the same tree.

| variant | byte | linked | `.rodata` | first diff | Status diff bytes |
|---|---:|---:|---:|---:|---:|
| baseline | 4249 | 4496 | `0x41b67` | `0x1fc33d` | 256 |
| moved `g_levelXPTable`+`checkForLevelUp` | **4250** | **4496** | `0x41b67` | `0x1fc390` | 224 |

`0x1fc330..0x1fc390` is byte-identical after the move; the only changed report
row is `_ZN6Status15checkForLevelUpEv` (`byte False -> True`, `pct 100.0`).

## 5. Controlled negative experiments

### 5.1 Direct `switch (m->getType())` (remove `kMissionCase`)

The original `tbh` tables decode to the following type-to-case mapping (the
inverse of our `kMissionCase` table):

```
150->170 151->174 152->150 154->152 155->151 156->153 157->154 159->155
160->158 161->162 162->166 163->167 165->184 166->189 167->156 168->157
169->160 171->163 172->164 173->165 174->168 176->169 177->171 182->172
```

Replacing `switch (kMissionCase[m->getType()])` with `switch (m->getType())`,
relabelling the cases with that mapping, and deleting the 190-byte table gives:

```
verify: byte 2942  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
.rodata 0x41aa7 (-0xc0)
```

The drop is a layout effect, not a codegen failure: removing the table makes
`.rodata` 0xc0 bytes smaller, so every later input is 0xc0 lower than the
original layout and the total size is wrong.  The original's Level block is not
simply our Level block shifted by -0xc0 either (its content order also differs).
The direct-switch reconstruction is therefore correct in principle but cannot be
landed alone; it needs the missing 0xc0 bytes of Level/duplicate-table content
recovered in the same change.

### 5.2 Link-order moves

The original `.rodata` is a witness for the original link order, but moving
individual objects is not a free win because it also changes PLT/dynsym order and
alignment padding.  Relinks of the same objects with only the response-file order
changed:

| variant | byte | linked | `.rodata` |
|---|---:|---:|---:|
| baseline | 4249 | 4496 | `0x41b67` |
| `MenuTouchWindow` after `ListItemWindow` | 4174 | 4496 | `0x41b67` |
| `LensFlare` after `AutoPilotList` | 4169 | 4496 | `0x41b77` (+0x10) |
| both moves | 4114 | 4496 | `0x41b77` (+0x10) |

The rodata shift is real (the original `LensFlare` content sits at
`0x201ee4`, ours at `0x201e34`), but the PLT/order penalty is larger.  Not
landed.

## 6. Proven vs hypothesis

Proven (commands above):

1. `.rodata` is size- and base-exact; 13,308/269,159 bytes differ after the
   landed fix, in 5,144 runs.
2. The first difference is `kMissionCase` at `0x1fc390`; the original has no
   verbatim copy of that 190-byte table.
3. `0x1fc330..0x1fc390` is now byte-identical to the original, and
   `Status::checkForLevelUp` is byte-exact.
4. The landed move is lossless: byte 4249 -> 4250, linked 4496 unchanged,
   `.rodata` size/base unchanged, lint clean, drift 0.
5. Direct-switch removal of `kMissionCase` alone shrinks `.rodata` by 0xc0 and
   drops byte_exact to 2942; it needs the missing Level content in the same
   change.
6. Moving `MenuTouchWindow`/`LensFlare` in the link order changes `.rodata`
   placement but loses byte_exact (4174/4169/4114 vs 4249).

Hypothesis (consistent with the evidence, not yet proven):

1. The remaining 13,308 bytes are still movement/duplicate placement, not wrong
   values; the 57/57 unique-word split is unchanged.
2. The next high-value change is to make the original's 190-byte
   `kMissionOrder`-style table live in `.bss` (or otherwise remove
   `kMissionCase` from `.rodata`) **and** recover the 0xc0 bytes of original
   Level/duplicate-table content in the same pass.  That should move `Level.cpp`
   from `0x1fc490` to `0x1fc3d0` and align the `Level`/`Globals` blocks.
3. `Globals.cpp` is currently a pure `-0x70` shift (ours `0x1fe310`, original
   `0x1fe2a0`); it should follow automatically once the Status/Level front is
   fixed, because its section content is otherwise verbatim in the original.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3438

cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# scratch -Map relink (see §2)
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3438_link_cmd.txt
# edit the command: -Map=/tmp/w3438_rodata.map -o /tmp/w3438_rodata_unstripped.so
# and remove the trailing strip; then run it.
python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3438_rodata_unstripped.so \
  --map /tmp/w3438_rodata.map
python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3438_rodata.map

# lock the landed gain
python3 tools/ratchet.py --build-dir cmake-build-match --update-baseline
```
