# MGame::OnTouchEnd (8,080 B) — w13819 (wave 191b): shared dispatch tail /
# high-numbered touch cases — note-only pass

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8,080 B).
Branch `wave191b-w13819`, base master `6b680e716`.
Worker w13819. Slice: the shared dispatch tail + the high-numbered touch cases
(100/110/120/126/127/134/144/155/161/162) and the `hudMenuOpen` weapon-slot /
engine-sound dispatch.  Deliberately did **not** re-do the cinematic-steer
`case 0`, the case-0 store tail, or the cargo / instant-action reorder
(w13774 / w13798 / w13805).

**No size-exact raw/aligned win was found.  `src/` is unchanged.**

## 0. Baseline (pristine tree, rebuilt this pass)

```
_ZN5MGame10OnTouchEndEiiPv   raw=3665  aligned=545  size=8080/8080  insn=2865/2865
```

Section deltas vs the original (`scratch/w13819/secdiff.py`):

| section | delta |
|---|---:|
| `.text` | 76,743 |
| `.ARM.extab` | 625 |
| `.note.gnu.build-id` | 20 |
| `.ARM.exidx` / `.rodata` / `.data` / `.rel.plt` | 0 (identical) |

## 1. New evidence this pass

### 1.1 The `hudMenuOpen` flag-dispatch chain is byte-identical

`17ad4c..17ada6` (orig) and the same addresses in ours are **byte-identical**:
`ldr r0,[fp,#0xf8]` then `tst #0x200` / `lsls r1,r0,#20` / `bmi.w` … with the
same shift amounts (`#20,#21,#18,#17,#16,#15,#14,#13,#12,#11,#19`) and the same
`str.w r8,[sp,#28]` interleave.  Only the `bmi.w` **targets** differ.  So the
`hudTouchFlags` bit → `weaponSlot` / `wingmanCommand` value mapping is already
correct; the whole residual in this area is target-block placement.

Dispatch targets (orig → ours):

| flag | orig target | orig value | ours target | ours value |
|---|---|---|---|---|
| 0x2000 | 17b1e4 | `movs r6,#0` | 17b0b6 | `movs r6,#0` |
| 0x4000 | 17b51c | `movs r6,#1` | 17b594 | `movs r6,#1` |
| 0x8000 | 17b704 | `movs r6,#2` | 17b7a2 | `movs r6,#2` |
| 0x10000 | 17b9ca | `movs r6,#3` | 17ba7a | `movs r6,#3` |
| 0x20000 | 17bc1e | `mov r8,#1` | 17bc36 | `mov r8,#1` |
| 0x40000 | 17bcfa | `mov r8,#3` | 17bdb4 | `mov r8,#3` |
| 0x80000 | 17be2e | `mov r8,#2` | 17bea4 | `mov r8,#2` |
| 0x100000 | 17beca | toggle `field_f8`, `r8=0` | 17bec8 | same |

All four weapon-slot bodies are the `movs r6,#N; b.n <shared body>` fan-in in
both binaries; only the shared body's address differs (orig `17b9cc`, ours
`17ba7c`).

### 1.2 The two adjacent cold blocks are already in the right relative order

In **both** binaries the `weaponSlot = 0` dispatch block is immediately followed
by the `cmp r2,#0` (engine-sound) block:

- orig: `17b1e4 movs r6,#0 / 17b1e6 b.n 17b9cc` then
  `17b1e8 cmp r2,#0 / 17b1ea beq.w 17b5a0 / 17b1ee blx PlayEngineSound …`
- ours: `17b0b6 movs r6,#0 / 17b0b8 b.w 17ba7c` then
  `17b0bc cmp r2,#0 / 17b0be beq.w 17b618 / 17b0c2 blx PlayEngineSound …`

So the *relative* order of these cold blocks is right; only their absolute
position differs (orig places them after the 74-clause body region / before the
42 clause; ours places them straight after the `ResumeEngineSound` block).
The `ResumeEngineSound` block itself is at `17af06` in **both** binaries
(`blx PlayerEgo::ResumeEngineSound` / `getEnemies` / loop
`KIPlayer::ResumeEngineSound`), byte-identical through `17af30` except the
final `b.n` target.

### 1.3 Original campaign-chain source order is 65, 81, 74, 95

Read off the false-target chain (each clause's `beq`/`bne` goes to the *next*
clause):

```
17b556 (74 test)  beq.w 17b694   -> 95 test
17b69a (95 test)  cbz  r0,17b708 -> 96 test
17b71e (96 test)  bne.w 17b93c   -> 100 test
17b7xx (126)      ...            -> 127 test at 17bc24
```

so the original's clause order is `65, 81, 74, 95, 96, 100, 110, 120, 126,
127, 134, 144, 155, 161, 162`, while ours is `95, 65, 81, 74, 96(+127), 100,
…`.  Moving the 95 clause to the end of the 65/81/74 group is therefore the
*source-correct* order — but it measures **worse** (see §2), because the
literal-pool islands shift with it.

### 1.4 `dockToAsteroid` merge site

Both binaries tail-merge the `isDockingToAsteroid` and `isDockingToStream`
`player->dockToAsteroid(...)` calls into one `blx`; they differ only in which
site is the merge target:

- orig: merge target `17bcf4` (`blx dockToAsteroid; b.n 17bb50`), reached from
  `17bcf0 ldr r2,[fp,#128]; mov r0,r8` (asteroid) and
  `17bf56 ldr r2,[fp,#128]; ldr r0,[fp,#88]; ldr r1,[r2,#36]; b.n 17bcf4`
  (stream, tail jump).
- ours: merge target `17bf5e` (stream site, fallthrough), asteroid site jumps
  `17bcf6 b.n 17bf5e`.

## 2. Measured table (this worker; all size-exact unless marked)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **baseline (pristine)** | **3,665** | **545** | **8,080** |
| 95 clause after 65 | 3,755 | 767 | 8,080 |
| 95 clause after 81 | 3,759 | 769 | 8,080 |
| 95 clause after 74 (source-correct order) | 3,751 | 799 | 8,080 |
| cinematic switch `case 1` before `case 0` | 3,665 | 545 | 8,080 (identical) |
| 96/127 split, single-call clauses, 127 after 126 | 4,847 | 950 | 8,148 (fatal) |
| 96/127 split + 95 after 65 | 4,837 | 1,020 | 8,148 (fatal) |
| 96/127 split + 95 after 81 | 4,839 | 1,022 | 8,148 (fatal) |
| 96/127 split + 95 after 74 | 4,831 | 1,052 | 8,148 (fatal) |
| engine-sound: `soundsMuted` branch first | 6,062 | 869 | 8,084 (fatal) |
| weapon-slot `if (weaponSlot < 0) {wingman} else {weapon}` | 3,665 | 545 | 8,080 (identical) |
| `!isMining() && (hudTouchFlags & 4)` operand swap | 3,681 | 551 | 8,080 |
| `!isMining() && (hudTouchFlags & 0x80)` operand swap | 3,681 | 551 | 8,080 |
| wingman block before the `hudTouchFlags == 0` block | 5,568 | 871 | 8,028 (fatal) |
| `if (!this->hudMenuOpen)` inverted if/else | 4,491 | 635 | 8,088 (fatal) |
| `__builtin_expect(isMining(), 0)` | 3,904 | 642 | 8,080 |
| `__builtin_expect(isMining(), 1)` | 4,015 | 788 | 8,060 (fatal) |
| `__builtin_expect(weaponSlot >= 0, 1)` | 3,665 | 545 | 8,080 (identical) |
| `__builtin_expect(weaponSlot >= 0, 0)` | 3,665 | 545 | 8,080 (identical) |

Notes:
- `weaponSlot >= 0` is already the profitable branch in clang's view, so both
  `__builtin_expect` directions and the `weaponSlot < 0` inversion are exact
  no-ops (byte-identical output).  No lever there.
- The 96/127 split costs **+68 B** in the cheapest spelling (two single-call
  clauses, body duplicated, 127 clause placed after 126).  Clang 7.0.2 `-Oz`
  does **not** tail-merge the two identical bodies at this size, so the
  original's `17bc36 beq.w 17b722` backward re-entry into the 96 body is not
  reachable from a duplicated-body source.  Since the function is size-exact at
  8,080, the split is gate-fatal unless ~68 B are recovered elsewhere in the
  same TU.
- The source-correct 95-clause order (§1.3) is size-exact but *raises* aligned
  from 545 to 799: the moved clause drags the literal-pool islands with it, so
  the aligned metric gets worse even though the clause order matches.

## 3. Residual clusters (unchanged from w13805)

| n | kind | O block | U block | what |
|---:|---|---|---|---|
| 63 | insert | `O[1609:1609]@17b374` | `U[1610:1673]@17b37a` | cargo-loop literal-pool island |
| 46 | replace | `O[1926:1972]@17b6ec` | `U[1963:1965]@17b758` | 95-clause tail / 96/127 merge |
| 31 | insert | `O[2280:2280]@17bac6` | `U[2254:2285]@17ba7a` | weapon-slot body placement |
| 25 | replace | `O[2196:2221]@17b9d8` | `U[2186:2195]@17b9c0` | weapon-slot body placement |
| 22 | replace | `O[1629:1651]@17b3b4` | `U[1693:1694]@17b456` | cargo literal pool |
| 21 | replace | `O[1470:1491]@17b1e2` | `U[1491:1492]@17b21e` | engine-sound block placement |
| 16 | replace | `O[1369:1370]@17b0c2` | `U[1369:1385]@17b0c6` | engine-sound block placement |
| 14 | delete | `O[2123:2137]@17b910` | `U[2128:2128]@17b922` | `isMining()` cold block before 100 body |
| 13 | replace | `O[1869:1882]@17b64e` | `U[1914:1918]@17b6cc` | cinematic switch test order |
| 11 | replace | `O[2023:2034]@17b7e4` | `U[2025:2035]@17b7e8` | literal-pool island |
| 10 | replace | `O[1257:1259]@17af82` | `U[1257:1267]@17af82` | `isInstantActionMission` before `isCampaignMission` |
| 7 | delete | `O[2417:2424]@17bc2a` | `U[2425:2425]@17bc42` | standalone `==127` clause |
| 4 | replace | `O[2486:2490]@17bcf4` | `U[2487:2488]@17bcf6` | shared `dockToAsteroid` `blx` placement |

## 4. Conclusion / recommendation

Every remaining cluster in this slice is **machine-block / literal-pool
placement**, not statement order: §1.1 and §1.2 show the dispatch tests, the
flag→value mapping, the shared weapon-slot body and the two adjacent cold
blocks already agree with the original; only addresses differ.  The one
source-order difference that is provably real (95 after 74, §1.3) is
size-exact but measures worse, and the one CFG difference that is provably real
(96/127 split, §1.2 of w13805) costs +68 B.

The next productive angle is a TU-level / link-order change that shifts the
literal-pool and machine-block placement globally, or a `-mllvm
-print-machineinstrs` style block-order dump of the original's layout (needs
the original's IR, which we do not have) — not another local statement reorder
in `OnTouchEnd`.
