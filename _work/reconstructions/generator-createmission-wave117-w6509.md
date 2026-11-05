# `Generator::createMission` — wave 117 (w6509): the spill block is the whole delta

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **research note, no source change landed.**  The wave-114/115/116 shape
(`volatile double padA` + `volatile int amount`, `__builtin_expect(pick == 10, 1)`,
`__builtin_expect(pick == 12, 0)`, `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)`)
remains the unique best variant: **83.3 % official / 1996 B / extab 168 B**.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.ARM.extab` `0x26294` (4 B short of `0x26298`).
`git status --short` is empty after the note commit.

## 1. The whole remaining delta is one MBP decision

Disassembling the *linked* baseline against the original shows the two functions are
instruction-for-instruction identical except for the shared `type` spill block and the
`getRoutes() == nullptr` block.  Everything else is the same code shifted by `+0x12`
(18 B) after the null block:

```
orig                                      ours (baseline)
97342: blx Agent::getOffer                97342: blx Agent::getOffer
97348: bne.n 97356                        97348: bne.n 97356
9734a: ... getStation; movs r0,#8         9734a: ... getStation; movs r0,#8
97354: b.n 973f8        <-- spill block   97354: str r0,[sp,#36]   <-- spill here
...                                       ...
973f6: movs r0,#13                        973f4: movs r0,#13
973f8: str r0,[sp,#36]  <-- spill block   973f6: b.n 97354        <-- back to spill
973fa: blx Agent::getName                 973f8: <null block, nextInt(2)>
...                                       9740c: blx Agent::getName
97586: <null block, nextInt(2)>           ...
```

So the only thing that has to change is **where the `type` phi spill block is placed**:
after `bb.48` (the `type = 13` arm) in the original, after `bb.32` (the `type = 8` arm)
in ours.  The null block then follows the spill.

## 2. The MBP frequency model, with measured frequencies

`MachineBlockPlacement` (post-RA, `TargetPassConfig::addBlockPlacement()`, line 889)
places the spill block `M` after the first predecessor that claims it.  When `bb.32`
(the `type = 8` arm) considers `M`, `hasBetterLayoutPredecessor` rejects the claim iff

```
freq(bb.48) * HotProb >= freq(bb.32) * HotProb.getCompl()
freq(bb.48) >= freq(bb.32) / 4          (HotProb = StaticLikelyProb = 80 %)
```

Parsing the branch weights out of the `-print-machineinstrs=block-placement` MIR dump
and propagating them gives, for the baseline:

```
freq(bb.31) = 1.30208e-03   (after the mission-type loop; the 1/32 loop-exit heuristics dominate)
freq(bb.32) = 6.51042e-04   (type = 8 arm, 50 % of bb.31)
freq(bb.48) = 9.92914e-08   (type = 13 arm)
ratio       = 1.525e-04     threshold 0.25   ->  1640x short
```

The path to `bb.48` is
`0.5 (offer!=5) * 0.5 (type<15) * 0.9995 (mask) * 1/32 (while targetStation) *
 0.75 (race<4) * 2/3 (type==13) * 0.625 (routes!=null) * 1/32 (while !found)`.
The two loop-backedge heuristics (`BranchProbabilityInfo` LBH, 124:4 = 31:1) are the
dominant factors.  Even with both loop exits at 100 % the ratio is only 0.156, so a
plain source change cannot flip it: `freq(bb.32)` must also drop, i.e. the offer check
needs a branch-weight hint.

## 3. The record-172 family, and the `.rel.plt` blocker

`volatile int32_t type` + splitting the reward base into an `int base` temp
(`int base = (int)(difficulty/10.0f*5500.0f)+1500;` — which is exactly the original's
`9764e..9768a` shape) + `__builtin_expect(agent->getOffer() == 5, 0)` reproduces the
original's **172-byte record, `tlen=151`, 26 rows**, and makes `.ARM.extab` size-exact
at `0x26298`:

```
vol+r5+offer0   50.4 %  1996 B  rec 172  tlen 151  rows 26  extab 0x26298
```

(The row *contents* are a size coincidence, not the original's rows — the extra `lp=0`
row lands at a different offset — but the record size and therefore the section size
are exact.)

It **fails the ratchet**: `byte_exact 4501 -> 4485` (−16).  `volatile int32_t type`
reorders the TU's relocation stream, which changes gold's PLT assignment order
(`Output_data_plt_arm::add_entry` assigns `begin_offset + entry_count*plt_entry_size`
during the relocation scan).  Three `.rel.plt` entries are permuted:

```
base:  ... Item::getMaxPrice, SolarSystem::getName, Mission::setTargetSystemName
hit:   ... SolarSystem::getName, Mission::setTargetSystemName, Item::getMaxPrice
```

Those three PLT slots are referenced by 16 other functions (Hud::drawOrbitInformation,
BluePrint::getAutoCompletionPrice, ModStation::OnRender2D, NewsTicker::replaceTokens,
SpaceLounge x3, WantedWindow, RecordHandler x2, ListItemWindow, MenuTouchWindow,
Status::calcCargoPrices, Globals::getAgentMissionText, StarMap x2), so their bytes change.

The reorder happens because the volatile variant's MBP output places the reward chain's
`type == 8` block **after** the tail loop, so `Item::getMaxPrice` (0x97898) is emitted
after `SolarSystem::getName` (0x97844).  The original places `type == 8` **before**
`getName` (`0x97862` vs `0x9788e`), which is why the baseline's PLT order matches.

## 4. Measurements this pass

All on top of the pristine baseline unless stated.  `relplt_diff` counts differing
8-byte `.rel.plt` entries vs the baseline link.

| variant | pct | usize | tlen | rec | relplt_diff |
|---|---:|---:|---:|---:|---:|
| **baseline** | **83.3** | **1996** | **147** | **168** | **0** |
| `vol+r5+offer0` | 50.4 | 1996 | 151 | 172 | 3 |
| `vol+r5` | 51.1 | 2044 | 151 | 172 | 3 |
| `vol+r5+chain(3\|\|5)` | 51.3 | 2028 | 151 | 172 | 3 |
| `vol+r5+mask` | 51.9 | 2044 | 151 | 172 | 3 |
| `vol+r5+t15` | 51.4 | 2020 | 151 | 172 | 3 |
| `vol+r5+8first` | 51.5 | 2040 | 141 | 164 | 9 |
| `vol+r5+8second` | 51.2 | 2040 | 141 | 164 | 9 |
| `vol+r5+8last` | 50.5 | 1988 | 152 | 176 | 3 |
| `hit+tail B/D` | 49.9 | 2000 | 151 | 172 | 3 |
| `hit+tail name-temp / while-break / goto-end` | 50.4 | 1996 | 151 | 172 | 3 |
| `hit+tail early` | 44.8 | 2000 | 151 | 172 | 7 |
| `hit+sep8` | 47.5 | 1992 | 141 | 164 | 9 |
| `offer0` | 63.4 | 1972 | 147 | 168 | 0 |
| `offer0+wl0` | 60.4 | 1984 | 147 | 168 | 0 |
| `offer0+fl0` | 41.7 | 1996 | 146 | 168 | 0 |
| `offer0+wl0+fl0` | 44.3 | 2044 | 147 | 168 | 4 |
| `offer0+t15` | 47.0 | 1976 | 147 | 168 | 0 |
| `t15+wl0+fl0` | 45.4 | 2052 | 147 | 168 | 4 |
| `r5` alone | 77.5 | 1996 | 146 | 168 | 0 |

Every hint variant that lowers `freq(bb.32)` (or raises `freq(bb.48)`) also makes `type`
register-resident, which removes the spill block entirely, so the record never grows.

### 4.1 Source shapes that are byte-identical to the baseline

All of these compile to exactly the baseline object (83.3 % / 1996 B / 168):

```
getRoutes() == nullptr  ->  !getRoutes()                  (and ternary / local r)
if (race < 4 && type == 13)  ->  nested ifs, goto, early-exit, reversed if/else
while (targetStation == ...)  ->  for / while with index
while (!found)  ->  for(;!found;), while(i<size), goto out
if (agent->getOffer() == 5)  ->  != 5, switch(offer), offer5 flag + two ifs
swap the race<4 block and the targetStation loop in source
goto typeDone after the type = 8 arm
```

The compiler canonicalises them all back to the same IR.

### 4.2 Random search

120 random combinations of {`volatile type`, `int base` split, `offer0`/`offer1`,
`t15`, chain order (base/8-first/3||5), tail-loop shape (base/B/D)} produced **24
record-172 variants, all with `relplt_diff != 0`** (3, 9 or 17).  No non-volatile
variant produced `tlen = 151`.

## 5. Version note

The LLVM 7.0.1 debug build (`/dev/shm/wf213/llvm701build`) places the spill block after
`bb.48` for the pristine baseline (matching the original), while the NDK r18b clang
7.0.2svn build places it after `bb.32`.  `MachineBlockPlacement.cpp` is functionally
identical between the two trees (only `DEBUG` -> `LLVM_DEBUG` / `\brief` comment
differences), so the divergence must come from another pass in the Android 7.0.2svn
tree (or from the debug build's own codegen).  The 7.0.1 build is therefore not a
usable oracle for this function (its `createMission` is 2008 B, not 1996 B).

## 6. Reproduction

```sh
cp /dev/shm/w6509/Generator.base.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4501

# record-172 variant (fails the ratchet: byte 4485)
python3 /dev/shm/w6509/apply_hit.py
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6509/score.py                          # 50.4 % 1996 B
python3 /dev/shm/w6509/rowsz.py cmake-build-match/libgof2hdaa.so 0x971b8 | head -1
                                                         # tlen=151
```

Scratch files: `/dev/shm/w6509/` (`harness.py`, `runvar.py`, `v1.py`..`v15.py`,
`rand117.py`, `Generator.base.cpp`, `Generator.hit.cpp`, `mbp.log`, `mbp701*.log`,
`sbs.txt`).  Not part of the repo.
