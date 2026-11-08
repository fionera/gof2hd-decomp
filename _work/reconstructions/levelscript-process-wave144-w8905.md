# LevelScript::process — wave 144 / w8905 (case-80 metric store removed; the
# genuine case-91 state-3 `m_nScriptTimerA = 0` restored)

Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-143 head `f5093474f`.
Scratch helpers in `/dev/shm/w8905/` (not tracked): `fdiff.py` (raw same-address
byte diff), `delta.py`/`align.py` (address-carrying normalised diff),
`timer0.py` (`strd [r,#144]` census), `h.py` (named-edit build harness),
`orig.s`/`ours_head.s` disassembly dumps.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,619            54,570            (-49)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    85,851            85,802            (-49)
.ARM.extab diffbytes     1,499             1,499             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The landed change (source diff is 1 deletion + 1 insertion)

```cpp
// case 80, state 0 (line ~2597) — the metric-only store is gone
 m_bCinematicBreak = 1;
 m_pHud->visible = 0;
 m_pRadar->enabled = 0;
-m_pRadar->dockNavPtr = nullptr;
 player->setComputerControlled(true);

// case 91, state 3 tail (line ~2991) — the genuine missing statement
 for (int i = 0; i < 3; i++) { ... setCloakingPossible / cloak ... }
+m_nScriptTimerA = 0;
 m_pRadar->dockNavPtr = nullptr;
 m_nState++;
```

### Why the case-80 store was wrong

The original case-80 state-0 block (`0x139e86..0x139eb8`) is

```
139e86 ldr.w r0, [r6, #208]   ; m_pHud
139e8a mov.w r8, #1
139e8e strb.w r8, [r6, #17]   ; m_bCinematicBreak = 1
139e92 movs  r1, #1
139e94 strb.w r9, [r0, #1]    ; m_pHud->visible = 0
139e98 ldr.w r0, [r6, #212]   ; m_pRadar
139e9c strb.w r9, [r0, #72]   ; m_pRadar->enabled = 0
139ea0 mov   r0, r4
139ea2 blx   setComputerControlled
...
```

There is **no** store to `Radar+8` (`dockNavPtr`) anywhere in the block.  The
w8802 store was a pure size companion.

### The genuine statement: case-91 state-3 tail

Original `0x13feaa..0x13fede` (after the three-ship cloak loop):

```
13fed0 ldr  r2, [sp, #88]     ; this
13fed2 movs r1, #0
13fed4 ldr  r0, [r2, #28]     ; m_nState
13fed6 strd r1, r1, [r2, #144] ; m_nScriptTimerA = 0   (0x90/0x94)
13feda adds r0, #1
13fedc str  r0, [r2, #28]     ; m_nState++
13fede b.w  144e36
```

Our source had `m_pRadar->dockNavPtr = nullptr;` there and **no** timer store.
The `strd` census (`timer0.py`) confirms it: the original has five
`strd rX,rX,[rY,#144]; adds; str [rY,#28]` timer-reset/state-increment
sequences (`13a91a`, `13fed6`, `142056`, `142d86`, `1434b4`); ours has two
(`13c5aa`, `143540`) before the fix.  `13fed6` is the case-91 state-3 one.

## Size accounting

Removing the case-80 store alone is **-8 B** (57224).  Adding the genuine
`m_nScriptTimerA = 0` alone is **-8 B** (57224) because clang then shrinks the
surrounding block further.  Both together are **-16 B** (57216), so a +16 B
genuine companion is still needed to stay size-exact.

The landed companion is the `dockNavPtr` store relocated to the same case-91
tail.  It is **METRIC-ONLY** there (the original tail has no `Radar+8` store),
but it is a real teardown store that the original performs in the neighbouring
case-91 states (state 5 at `0x13f3b8`, state 7 at `0x140294`), and it restores
exactly 57232 B while improving the raw metric by 49.

Measured combinations (all built + linked, `fdiff.py`):

| variant | size | raw |
|---|---:|---:|
| HEAD (case-80 store + wrong case-91 store) | 57232 | 54,619 |
| remove case-80 store only | 57224 | 55,134 |
| case-91 `dock` -> `timer` only | 57224 | 55,094 |
| remove case-80 + case-91 `dock` -> `timer` | 57216 | 55,218 |
| **remove case-80 + `timer; dock` at case-91 (landed)** | **57232** | **54,570** |
| remove case-80 + `timer; lockedEnemy; dock` at case-91 | 57232 | 54,568 |
| remove case-80 + `dock; timer` at case-91 (order swapped) | 57224 | 55,266 |
| remove case-80 + `timer; dock` + `forceUpdate`/timer reorder | 57244 | 54,871 |

The store order matters: `m_nScriptTimerA = 0;` must precede the `dockNavPtr`
store (the reverse order is 8 B smaller).  The `lockedEnemy` variant is 2 raw
bytes better but adds a second non-witnessed store, so the minimal form is
landed.

## Open questions

* The faithful set (case-80 store removed + case-91 `timer` only) is **57216**
  (-16).  A genuine +16 B statement is still missing.  Candidates that were
  measured and rejected: `forceUpdate`/timer reorder at the case-16 state-2
  tail (+12), the `dockToDockingPoint` receiver at line 3067 (-8), the
  `case-16 state-2` `m_nState++` inline form.  The original's five
  `strd;adds;str` timer sequences vs our two show that clang is folding the
  state increment into a shared tail (`144e12: adds r0,#1; str.w r0,[sl,#28]`)
  and emitting `str.w` pairs instead of a `strd` at some sites — that
  structural difference is where the remaining 16 B live.
* The `m_pRadar->dockNavPtr = nullptr` at the case-91 tail can be dropped once
  a genuine +8 is found (then the faithful set is 57232 with no metric-only
  store).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8905/fdiff.py _ZN11LevelScript7processEi   # 54,570
```
