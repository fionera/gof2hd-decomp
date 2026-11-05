# `Generator::createMission` — wave 119 (w6780): LSDA / 172 B record audit, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B; original `.ARM.extab` LSDA record **172 B**, ours **168 B**).

Status: **research note, no source change landed.**  The wave-119/w6752 shape stays at
**83.6 % (raw 83.6088)**, 1996 B, frame 112, `.ARM.extab` section `0x26294` (ours) vs
`0x26298` (original).  Every source variant tried this pass is equal or worse.

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B
after:          83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B  (no change)
```

Full `check` at HEAD (re-run this pass):

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.ARM.exidx` `0x6f70`
(exact), `.ARM.extab` `0x26294` (4 B short; `createShip` is now exact, so this is the
only remaining `Generator` extab delta).

## 1. Exact LSDA structure (both records parsed)

The `.ARM.extab` record for `createMission` is at `0x1a78c4` (ours and original), and is
`personality word (4) + unwind opcodes (12) + LSDA`.  The unwind opcodes are byte-identical
(`c9 4f 97 02 f0 80 00 83 b0 b0 b0 ab`).  The LSDA header is `ff ff 01 <uleb cslen>`
(LPStart omit, TType omit, call-site encoding uleb128); `cslen >= 128`, so the length is a
2-byte uleb and the record size is `21 + cslen` padded to 4.

| | original | ours |
|---|---:|---:|
| call-site table length (`cslen`) | **151** (`0x97`) | **147** (`0x93`) |
| call-site entries | **26** | **25** |
| record size | **172** (`0xac`) | **168** (`0xa8`) |

The original's extra entry is a **pad=0 gap**:

```
orig [7]  start=0x3c8  len=0x20  pad=0x0   -> 0x97580..0x975a0
ours [0]  start=0x0    len=0x25e pad=0x0   -> 0x971b8..0x97416  (absorbs the null block)
```

The gap covers the cold `getRoutes() == nullptr` block at `+0x3ce` (`0x97586`), which the
original lays out **after the `type == 15` switch arm** (between the type-15 try-range and
the type-0 try-range).  Ours lays the null block out at `0x973f8`, immediately before
`getName` (`0x9740c`), i.e. inside the initial gap, so it creates no separate entry.
The gap entry is 5 bytes (`start 2 + len 1 + pad0 1 + action0 1`); adding it to ours
would give `cslen = 152`, i.e. `21 + 152 = 173` bytes, padded to **176 B** — not 172.
Exact 172 needs `cslen` in 148..151; the original's 151 comes from its different
try-range partition, not from simply appending that entry to our table.

To restore the record the null block must be laid out **between two try-ranges**, which
in turn requires the shared `type` store block (`%177`, the join of the offer/else arms)
to be chained with the **else** arm (as in the original) rather than with the offer arm
(ours).  In ours the else arm ends with `b.n 0x97354` (store in the offer arm); the
original's else arm falls through to `str r0,[sp,#36]` at `0x973f8`.

## 2. Measurements

All variants are 1996 B unless noted; `cslen`/`n` are from the parsed LSDA.

### 2.1 Branch-weight hints around the null block

Single `__builtin_expect` on each branch feeding the join, and combinations with the
cold-null hint (`F0`).  None changes `cslen` (all `147/25`):

| variant | raw % | usize | cslen/n |
|---|---:|---:|---:|
| **HEAD** | **83.6088** | **1996** | **147/25** |
| `expect(offer==5,0)` | 63.8947 | 1956 | 147/25 |
| `expect(offer==5,1)` | 40.0278 | 2020 | 147/25 |
| `expect(type<15,0)` | 39.1667 | 2020 | 147/25 |
| `expect(type<15,1)` | 65.5602 | 1960 | 147/25 |
| `expect(race<4,0/1)` | 83.6088 | 1996 | 147/25 |
| `expect(type==13,0)` | 44.3677 | 2036 | 147/25 |
| `expect(type==13,1)` | 83.6088 | 1996 | 147/25 |
| `expect(routes==nullptr,0)` (F0) | 63.8122 | 1980 | 147/25 |
| `expect(routes==nullptr,1)` | 47.9613 | 2036 | 147/25 |
| `expect(!found,0/1)` | 47.4412 / 55.6246 | 2036 / 2008 | 147/25 |
| `expect(offer==5,0)` + F0 | 60.5263 | 1968 | 147/25 |
| `expect(type<15,1)` + F0 | 61.1765 | 1968 | 147/25 |
| `expect(campaign,0/1)` | 70.5559 / 67.7686 | 2004 / 1996 | 147/25 |
| `expect(mask!=0,0)` | 73.3287 | 1996 | 147/25 |

The cold-null hint alone (`F0`) does **not** move the null block out of the initial gap:
its address stays `0x973fe` (right before `getName`).  With `expect(offer==5,0)+F0` the
null block moves to `0x978de` (after the switch), but that is inside the **final** pad=0
gap entry, so still no new entry.

### 2.2 Structural rewrites

| variant | raw % | usize | cslen/n |
|---|---:|---:|---:|
| invert `offer != 5` + swap arms (3 spellings) | 83.6088 | 1996 | 147/25 |
| `goto afterTypeBlock` / `switch(offer)` / two `if`s / `bool isOffer5` | 83.6088 | 1996 | 147/25 |
| `!routes` / `routes==0` / bool `noRoutes` before `getName` | 83.6088 | 1996 | 147/25 |
| swap mask-check and race block | 61.9640 | 1960 | 147/25 |
| `goto nullRoutes` after the found loop | 79.8621 | 1992 | 147/25 |
| null block after the switch (flag) | 73.8209 | 2024 | 142/24 |
| null block after the switch + `getName` before offer | 45.1613 | 2048 | 171/29 |
| `getName` before offer (AB1) | 46.1645 | 2032 | 183/31 |
| `getName` before type-12 check (AB2) | 47.7383 | 2000 | 183/31 |
| `getName` after the switch | 52.5507 | 1948 | 105/18 |
| `getName` before the race block (bool restructure) | 46.4111 | 1988 | 170/29 |
| `type==12` block moved after offer / into else | 67.9167 / 52.6904 | 1972 / 1944 | 147/25 |
| `int race` / `int selected` / `guard=998` | 83.4711 | 1996 | 147/25 |
| `type & ~4 == 8` / `int i` / `int found` | 83.3333 | 1996 | 147/25 |
| empty `default:` in the type switch | 83.6088 | 1996 | 147/25 |

Moving `getName` changes the invoke structure and therefore the LSDA (99–183 bytes) but
always costs 20+ points of match.

### 2.3 Empty-destructor guard trick (adds a try-range without a call)

A local `struct NoopGuard { ~NoopGuard() {} } guard;` around a statement makes the calls
in its scope `invoke`s (new landing pad) while the destructor emits no code.  It reliably
adds **one 6-byte try-range entry** (and often shrinks a 0x80-length entry by 1), i.e.
`cslen = 152/153` → a **176 B** record, not 172:

| guard placement | raw % | usize | cslen/n |
|---|---:|---:|---:|
| around `level = getLevel()` | 81.7056 | 2000 | 152/26 |
| around the reward expression (split at `getLevel`) | 81.9807 | 2000 | 152/26 |
| around `getStation` in the reward chain | 83.6088 | 1996 | 147/25 (merged) |
| around `getTecLevel` / `count=nextInt(15)` | 81.2930 | 2000 | 153/26 |
| around `costs = ...nextInt(...)` / `getRandomName` | 79.0345 | 1992 | 153/26 |
| around the null block's `nextInt` | 74.3290 | 1996 | 158/27 |
| around `getRace` / `generateStationIndex` (plain region) | 74.9828 / 42.8966 | 1992 / 2048 | 156 / 158 |
| around `getOffer` / type-12 `getStation` | 80.8803 / 81.2930 | 2000 | 159/27 |
| around `getName` | 75.1540 | 2020 | 159/27 |
| around the initial `generateStationIndex` | 73.5172 | 1992 | 152/26 |
| around `new Mission` / `getAsteroidProbabilities` | 83.6088 | 1996 | 147/25 (merged) |

The new entry is always 6 bytes (`start>0x7f` = 2, `len` = 1, landing pad `>0x7f` = 2,
action 0 = 1); the only 2-byte length in the base table is the 0x80 reward try-range, so
splitting it saves at most 1 byte.  Net is therefore +5 (`152`) or +6 (`153`), never the
+1..+4 needed for `cslen` in 148..151.  A `try/catch` variant adds a type table and is
much larger (`0x262a8`..`0x262ac`, 2008–2012 B).

## 3. Conclusion

The 4-byte `.ARM.extab` delta is a **block-placement consequence**, not a source spelling:

1. the original chains the shared `type` store block (`%177`) with the **else** arm, so the
   else arm falls through to the store and the hot chain continues through `getName` and
   the switch; the cold null block is then placed after the `type == 15` arm, between two
   try-ranges, creating the 5-byte pad=0 gap entry (`cslen 151`);
2. ours chains `%177` with the **offer** arm (`str r0,[sp,#36]` inline at `0x97354`), the
   else arm branches back to it, the chain ends at the found loop, and the null block is
   placed immediately before `getName` — inside the initial gap.

Every source-level lever tried (branch hints, arm inversions, gotos, switches, flags,
`getName` placement, guards) either leaves `cslen = 147` or overshoots to 152/153.
A byte-exact 172 B record needs the original's `%177` chain selection; no source form
found this pass reproduces it without a large match regression.

## 4. Reproduction

```sh
git checkout -- src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
python3 /dev/shm/w6780/score.py                        # raw 83.6088, 1996 B
```

Scratch: `/dev/shm/w6780/` (`Generator.base.cpp`, `score.py`, `tscore.py`,
`batch1.py`..`batch41.py`, `flagtest.sh`, `MBP.cpp`).  Not part of the repo.
