# `.rodata` content diff map (wave87 / worker w3428)

Status: research note + one landed source fix (`Generator.cpp`).
Baseline at start of pass: **byte_exact 4182, linked_exact 4495**.
After the landed fix: **byte_exact 4188, linked_exact 4495** (no other gate movement).

All numbers below are reproducible from the commands in the last section. The original
binary is `../_work/bins/android_2.0.16_libgof2hdaa.so`; `.rodata` in both binaries is
VMA/file-offset `0x1c9e10`, size `0x41b67` (269,159 bytes), so a raw offset in `.rodata`
is the same VMA in both files.

---

## 1. Baseline and gate state

```
cmake --preset default                       # only if cmake-build-match is missing
cmake --build cmake-build-match --target check -j8
```

Before the landed fix:

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-allow 0 missing 0 allowed 0 imports 207
RATCHET PASS
```

After the landed `Generator.cpp` fix:

```
verify: byte 4188  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4182 -> 4188 (lock with --update-baseline)
RATCHET PASS
```

`python3 tools/ratchet.py --update-baseline` then locks the gain in
`tools/verify_baseline.json` (`byte_exact 4188, linked_exact 4495`); a subsequent
`check` passes with no `GAIN` line.

`lint` (separate target): `CLEAN ✓ (no new soft violations; hard invariants 0)`.
`drift` (separate target): `=== DRIFT: 0 field(s) across 0 class(es) ===`.
Every ELF section is still size- and base-exact (`sodiff` + `data: exact 843 of 843`).

---

## 2. How the map was produced

The shipped `.so` is stripped of `.symtab`, so a scratch relink was made with
`-Map` and without the final strip. The link command was taken verbatim from
`ninja -C cmake-build-match -t commands libgof2hdaa.so`, with only the output
path and `-Map` changed (no other flag touched):

```
# /tmp/w3428_relink_map.sh  (exact command used)
cd cmake-build-match && <ndk>/ld.gold -shared -soname libgof2hdaa.so \
  --build-id -z now --exclude-libs=libgcc.a \
  --section-ordering-file=cmake-build-match/section_order.txt \
  -Map=/tmp/w3428_rodata.map -o /tmp/w3428_rodata_unstripped.so \
  ...same objects/archives/libraries as the real link...
```

This produces:

* `/tmp/w3428_rodata_unstripped.so` — same layout, with `.symtab` for `nm -S`;
* `/tmp/w3428_rodata.map` — per-input-section address/size/object, used to attribute
  each differing byte to the owning TU.

`_work/reconstructions/rodata_content_diff_map.py` (committed) reproduces the raw
diff, word multiset/alignment and per-symbol exact-content check. The remaining
scratch helpers are under `/tmp`, not part of the repo: `w3428_map_rodata.py`
(per-object attribution), `w3428_extra_words.py` (which words are unique to one
side), `w3428_resolve_pcrel.py` (ARM `ldr [pc]` + `add pc` literal-pool resolver).

---

## 3. Raw diff map (after the landed fix)

```
.rodata base 0x1c9e10, size 0x41b67
differing bytes : 14,515 / 269,159   (5.39%)
differing runs  : 5,430
first difference: VMA 0x1fb8c8  (offset 0x31ab8)
last  difference: VMA 0x2037a8  (offset 0x39998)
```

Controlled A/B (same tree, `git stash push -- src/game/mission/Generator.cpp`,
rebuild + `check`, then `git stash pop` + rebuild + `check`): the pre-fix build
gave **14,640 bytes / 5,555 runs, first difference VMA `0x1fb590`** (the
`kKaamoStationItems`/`kAgentOfferItems` swap described in §5) and the same gate
`byte 4182 / linked 4495`. The fix removed 125 diff bytes and the whole
`0x1fb590..0x1fb810` region became byte-identical.

The diff is not spread over `.rodata`: it is concentrated in the game TUs
(`Level.cpp`, `Globals.cpp`, `StarSystem.cpp`, `Gun.cpp`, …). Top owners from
`/tmp/w3428_map_rodata.py`:

| bytes | runs | owning TU (map) |
|---:|---:|---|
| 4741 | 575 | `o102_Level.cpp.obj` |
| 4632 | 3246 | `o115_Globals.cpp.obj` |
| 824 | 228 | `o137_StarSystem.cpp.obj` |
| 495 | 149 | `o149_Gun.cpp.obj` |
| 374 | 125 | `o136_ListItemWindow.cpp.obj` |
| 329 | 126 | `o122_ImageFactory.cpp.obj` |
| 300 | 40 | `o108_ModStation.cpp.obj` |
| 272 | 87 | `o155_Achievements.cpp.obj` |
| 262 | 32 | `o101_Status.cpp.obj` |
| 253 | 106 | `o103_StarMap.cpp.obj` |
| 235 | 81 | `o091_Player.cpp.obj` |
| 212 | 70 | `o098_RepairBeam.cpp.obj` |
| 204 | 31 | `o168_ObjectGun.cpp.obj` |
| 169 | 148 | `o145_MissionsWindow.cpp.obj` |
| 169 | 60 | `o109_PlayerFighter.cpp.obj` |
| 95 | 14 | `** merge strings` (shared pool) |
| 83 | 63 | `o089_PlayerEgo.cpp.obj` |
| 77 | 58 | `o088_CutScene.cpp.obj` |
| 76 | 34 | `o132_MenuTouchWindow.cpp.obj` |
| 72 | 40 | `** merge constants` (shared pool) |

Remaining TUs each contribute < 70 bytes. The largest single run is 466 bytes at
`0x1fc640` (`Level.cpp`), followed by 227 bytes at `0x1fe3a0` (`Globals.cpp`) and
190 bytes at `0x1fc34c` (`Status.cpp`, the `kMissionCase` table — §6).

---

## 4. The diff is overwhelmingly *movement*, not wrong values

Word-multiset over the whole differing region
(`0x1fb8c8..0x2037ac`, 8,121 32-bit words):

| quantity | count |
|---|---:|
| words in region | 8,121 |
| words present in **both** binaries (multiset) | 8,064 |
| words only in original | **57** |
| words only in ours | **57** |

Pre-fix the same region was 8,327 words with 8,270 common and the same 57/57
unique split, i.e. the `Generator` fix removed 206 words of pure movement without
changing the unique-word counts.

`difflib.SequenceMatcher` alignment of the same region:

| opcode | words |
|---|---:|
| equal | 7,297 |
| delete (original only) | 560 |
| insert (ours only) | 714 |
| replace (unequal aligned pairs) | 45 |

So 8,064 − 7,297 = **767 words are byte-identical but moved**, and only 90 words
(45 replaced pairs) are aligned-but-different. The 57 only-original words are all
small duplicate-table values (`0`×24, `1`×14, `3`×6, `-1`×6, `2`×4, `8`×3): the
original kept extra copies of small tables that our compiler merged. The 57
only-ours words are 48 words of the `kMissionCase` byte table (§6), 8 words of a
duplicated `Level.cpp` mesh table (§7), and 1 word spanning the
`kApLit1`/`kApLit2` strings in `AutoPilotList.cpp`.

**Conclusion (proven):** the `.rodata` content is essentially correct. The
remaining 14,515 bytes are emission-order / duplicate-placement differences, not
wrong data — with the single exception of `kMissionCase`.

---

## 5. Landed fix: `Generator.cpp` duplicate 9-item arrays

### Evidence

Pre-fix `nm` on the scratch relink (old layout):

```
001fb590 r kKaamoStationItems
001fb5b8 r kShipRaces
001fb6b8 r kLootTypeChance
001fb6cc r kBlockedMissionStations
001fb79c r kAgentOfferItems
001fb7c0 r kAgentOfferShips
001fb7d8 r kAgentEnemyRaces
```

`Generator.cpp` declared three identical 9-int arrays
(`kAgentOfferItems`, `kNeverUnlockedItems`, `kaamoSpecialItems`) and placed
`kKaamoStationItems` first. The original `.rodata` at `0x1fb590` starts with
`200,220,208,213,216,228,229,230,231` (`kAgentOfferItems`), immediately followed by
`101,102,103,107,108,109,114,124,0,0` (`kKaamoStationItems`), then `kShipRaces`.

### Change

* moved `kAgentOfferItems` above `kBlockedMissionStations`;
* deleted `kNeverUnlockedItems` and `kaamoSpecialItems`, replacing both uses with
  `kAgentOfferItems` (the original compiler had merged the three identical arrays
  into one copy);
* added a comment explaining the merge.

### Result

Post-fix `nm -S`:

```
001fb590 r kAgentOfferItems
001fb5b4 r kKaamoStationItems
001fb5dc r kShipRaces
001fb6dc r kLootTypeChance
001fb6f0 r kBlockedMissionStations
001fb7c0 r kAgentOfferShips
001fb7d8 r kAgentEnemyRaces
```

`orig[0x1fb590:0x1fb810] == ours[0x1fb590:0x1fb810]` is **True** (640 bytes
byte-identical). Gate: **byte_exact 4182 → 4188, linked_exact 4495 unchanged**.
`Generator` function bodies are still not byte-exact in `report.json` (e.g.
`isKaamoSpecialItem` is 100% but not byte): their remaining diff is the moved
table addresses, so the 6-function gain comes from the corrected data layout
feeding other functions, not from a `Generator` body change.

---

## 6. `kMissionCase`: the only genuinely different content

`Status.cpp` declares `static const unsigned char kMissionCase[190]` at our
`0x1fc34c` (`nm -S`: size `0xbe`). Its 190 bytes do **not** occur anywhere in the
original `.rodata` (`bytes.find` → `-1`). The original at the same VMA is
`g_levelXPTable[4..]` (the table starts at `0x1fc33c`: `0, 7, 0x15, 0x2a, 0x46, …`).

Disassembly of the original `Status::missionCompleted` (`0xabf38`, Thumb,
`-M force-thumb`) shows the switch was compiled to **in-text `tbh` jump tables**,
not to a byte-table load:

```
abfd6  blx  Mission::getType()
abfda  sub.w r1, r0, #150
abfde  cmp   r1, #39
abfe2  tbh   [pc, r1, lsl #1]      ; 40-entry jump table
...
ac046  sub.w r1, r0, #8
ac04a  cmp   r1, #6
ac04e  tbh   [pc, r1, lsl #1]      ; 7-entry jump table
```

There is no `ldrb rX,[rY,rZ]` in the original function (the only `ldrb` is
`ldrb.w r0,[r6,#240]`). Our build, by contrast, loads the byte table:

```
abfda  ldr  r1,[pc,#844]   ; literal pool -> 0x1fc34c
abfdc  add  r1, pc
abfde  ldrb r0,[r1, r0]
```

**Proven:** the original `.rodata` has no `kMissionCase` copy; the original
compiler folded the lookup into `.text` jump tables. **Hypothesis:** the original
source switched on `m->getType()` directly (case labels 150..189 and 8..14),
while our reconstruction introduced the 190-byte lookup table. This is the top
follow-up for `Status.cpp`; it is a codegen/switch reconstruction, not a rodata
value fix. Note that `.rodata` is still size-exact, so the 190 bytes we spend on
`kMissionCase` are currently compensated by original duplicate small tables that
our compiler merged.

---

## 7. The other two "not byte-identical" symbols are placement artifacts

The first version of the per-symbol check (using the gap to the next symbol as the
size) flagged three symbols. Two of them are padding false positives, explained
here; using the real `nm -S` size (`rodata_content_diff_map.py`) leaves only
`kMissionCase`:

* `g_Hud_securityColors` (`0x203778`, `nm -S` size `0x30` = 48 bytes). Our 48-byte
  table occurs verbatim in the original at `0x1fde10` **and** at `0x203780`
  (the original has 8 bytes of `0xffffffff` alignment padding before the second
  copy, hence the shifted VMA). Content identical; placement differs by 8 bytes.
* `.L_ZZN5Level15createAsteroidsEvE9distances_1` (`0x1fc580`, `nm -S` size `0xc` =
  12 bytes). The 12 bytes `{40000,70000,90000}` occur verbatim in the original at
  `0x1fc540`. The script saw 16 bytes because it included 4 bytes of padding to the
  next symbol. Content identical; the three `createAsteroids` distance tables are
  shifted by `0x40` (ours `0x1fc560/570/580`, original `0x1fc520/530/540`).

So **194 differing symbols; with real `nm -S` sizes only `kMissionCase` has no
verbatim copy in the original `.rodata`** (the two padding artifacts above
vanish). Examples of moved-but-identical large tables (our VMA → original VMA): `gDialogueSoundPairs` `0x1feba0` → `0x1feb20` (12,032 B),
`g_cg_meshTable` `0x1fce70` → `0x1fc54c` (932 B), `g_cg_soundTable` `0x1fd214`
→ `0x1fbb50`/`0x1fbf80`/`0x1fc8f0` (932 B), `gSO_table` `0x202ec8` → `0x202ea8`
(600 B), `kStationLightColors` `0x202510` → `0x2024f0` (324 B), `gCFN_req`
`0x203154` → `0x203134` (540 B), `MUZZLE_MESHES` `0x2033b8` → `0x203398`
(932 B), `CAMPAIGN_GOAL_TEXT` `0x20287c` → `0x20285c` (660 B).

### Known duplicate-emission difference (not yet fixed)

`Level.cpp` has two identical 8-word mesh tables:

* `createSpace()`'s `jumpgateMeshes[32]` tail at our `0x1fc4c0` (the last 16
  `unsigned short` values are `14725,0,14726,0,…`, i.e. the same 8 ints);
* `createScene()`'s `kAgentMeshes[8]` at our `0x1fdd54`.

The original has **one** copy at `0x1fdc74`. This is the source of the 8 only-ours
words in §4. Fixing it means making the two tables share a declaration (as the
`Generator` fix did), but the original also emits `createSpace`'s statics at the
*end* of `Level.cpp`'s `.rodata` (`jumpgateMeshes` at original `0x1fdc54`) while
ours emits them first (`0x1fc4a0`), so the full `Level.cpp` layout is a larger
local-static ordering pass — left as follow-up.

---

## 8. Proven vs hypothesis

**Proven (commands above):**

1. `.rodata` is size- and base-exact (`0x1c9e10`, `0x41b67`).
2. 14,515 / 269,159 bytes differ (5.39%) in 5,430 runs.
3. Over the diff region, 8,064 of 8,121 words are common to both binaries; only
   57 words each are unique to one side. 767 common words are moved.
4. 194 symbols differ; 193 are byte-identical somewhere else in the original
   `.rodata`. Only `kMissionCase` has no verbatim copy.
5. The original `Status::missionCompleted` uses `tbh` jump tables and contains no
   byte-table load, so the original `.rodata` genuinely has no `kMissionCase`.
6. The `Generator.cpp` duplicate-array merge is real: it makes
   `0x1fb590..0x1fb810` byte-identical and raises `byte_exact` 4182 → 4188 with
   `linked_exact` unchanged at 4495.

**Hypothesis (consistent with the evidence, not yet proven):**

1. The remaining 14,515 bytes are entirely emission-order / duplicate-placement
   differences (per-TU declaration order and local-static emission order), except
   for the `kMissionCase` table.
2. The next lever is per-TU declaration order. `Generator.cpp` is now fixed; the
   next candidates are `Status.cpp` (`kMissionCase` / `g_levelXPTable` order),
   `Level.cpp` (local statics emitted in a different function order), and
   `Globals.cpp` (`gDialogueSoundPairs`, 12,032 B, shifted by `0x80`).
3. `kMissionCase` should be removable by reconstructing the original
   `switch (m->getType())` (case labels 150..189 and 8..14) instead of the
   lookup table; this would free 190 bytes of `.rodata` and should be checked
   against the original `tbh` targets before landing.

---

## 9. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3428

# 1. gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift

# 2. raw diff (both files have .rodata at file offset 0x1c9e10, size 0x41b67)
python3 - <<'PY'
orig=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
ours=open('cmake-build-match/libgof2hdaa.so','rb').read()
base=0x1c9e10; size=0x41b67
o=orig[base:base+size]; u=ours[base:base+size]
d=[i for i in range(size) if o[i]!=u[i]]
print(len(d), 'bytes', hex(base+d[0]), hex(base+d[-1]))
PY

# 3. scratch relink with -Map + .symtab
bash /tmp/w3428_relink_map.sh          # exact command in §2
nm -S -an /tmp/w3428_rodata_unstripped.so | grep -E 'kMissionCase|kAgentOfferItems|kKaamoStationItems'

# 4. per-object and per-symbol attribution
python3 _work/reconstructions/rodata_content_diff_map.py
python3 /tmp/w3428_map_rodata.py
```

`_work/reconstructions/rodata_content_diff_map.py` reproduces the raw diff, the
word multiset/alignment, and the per-symbol exact-content check. Scratch files
(the `-Map` relink, the per-object attribution script) are under `/tmp` only; the
only tracked changes from this pass are `src/game/mission/Generator.cpp`,
`tools/verify_baseline.json` (locked gain), this note, and the reproduction
script.
