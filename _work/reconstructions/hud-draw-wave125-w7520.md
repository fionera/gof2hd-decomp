# Hud::draw — wave 125 (w7520): dock1 local type tie-breaks

**Status:** code change + analysis, 2026-11-06 (wave 125). Worktree `wt/w7520`,
base `master 02513de33`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA
`0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint
**CLEAN**, drift **0**.

Official per-function score: **96.445573 -> 96.587843 %** (raw
`SequenceMatcher`), size **9332 B** both, normalised instruction streams
**3149 / 3153 -> 3149 / 3152**, diff hunks **88 -> 85**.

This note extends `hud-draw-wave123-w7314.md` (boost if/else, `Globals::status`
anchor, `short barW`); read that first.

---

## 0. TL;DR

Four dock1 local type tweaks, landed together, each measured against the
wave-124 HEAD:

```cpp
short total = ego->getDockTotalAmount();                                  // was int
unsigned short barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c); // was short
short top = this->field_0x3e2;                                            // was unsigned short
unsigned int barYOffset = Globals::layout->field_0x218;                   // was int
```

These are **allocator tie-breaks**, not necessarily faithful original types.
The faithful all-`int` dock form is 9328 B / 94.265 %.  The landed combination
keeps the body at the original **9332 B** and removes 3 diff hunks, raising the
raw score by +0.142 pp.

The second-bar `int top` / `int barYOffset` were left untouched; swapping the
second-bar `String` / `GetImage2DWidth` order still drops to 9328 B / 94.79 %.

---

## 1. Landed source edits

`src/game/ui/Hud.cpp`, dock1 block (current lines 1650, 1656, 1663, 1664):

| local | wave-124 | wave-125 |
|---|---|---|
| `total` | `int` | `short` |
| `barW` | `short` | `unsigned short` |
| `top` | `unsigned short` | `short` |
| `barYOffset` | `int` | `unsigned int` |

All four are in the first (docking-transfer) progress-bar block only.  The
second-bar block (`int halfW`, `String text`, `int barH`, `int barYOffset`,
`int top`) is unchanged.

---

## 2. Variant matrix (official raw `SequenceMatcher`, full link)

Measured against the wave-124 HEAD (96.445573 %, 9332 B, 3149/3153, 88 hunks):

| variant | size | insns | raw pct | hunks |
|---|---:|---:|---:|---:|
| **landed: `short total` + `unsigned short barW` + `short top` + `unsigned int barYOffset`** | **9332** | **3149/3152** | **96.587843** | **85** |
| `short total` only | 9336 | 3149/3154 | 94.37 | — |
| `unsigned short barW` only | 9328 | 3149/3152 | 94.57 | — |
| `short top` only | 9332 | 3149/3153 | 96.445573 | 88 |
| `unsigned int barYOffset` only | 9332 | 3149/3153 | 96.445573 | 88 |
| all-`int` dock (faithful) | 9328 | 3149/3146 | 94.265 | — |
| `int frameH` + `(short) frameH` cast | 9332 | 3149/3153 | 96.445573 | 88 |
| second-bar `String` before `halfW` | 9328 | 3149/3142 | 94.79 | — |
| gamma plain / `Globals::status` arg2 | 9312 | 3149/3148 | ~75.0 | — |

A local/random search over 18 type/order knobs found no combination better
than the landed 96.587843 %.

---

## 3. Remaining root causes (85 hunks)

All are allocator/register-allocation artefacts:

1. **Gamma block `Status` reload.**  Original keeps `&Globals::status` in `r6`
   and re-reads `[r6]` for the second call; ours spills `&Globals::status`
   immediately and reuses `r4`.  Same size, different opcode.  Plain
   `Globals::status` (no `st` local) collapses to 9312 B / ~75 %.
2. **Dock1 register swap.**  Original: `&Globals::Canvas` in `fp`, `textH` in
   `r9`, `barW` -> `r8`, `barH` -> `fp`.  Ours: `textH` in `fp`,
   `&Globals::Canvas` in `r6`, `barW` -> `fp`, `barH` -> `r8`.  The `sxth` /
   `ubfx` shapes of the `short` dock locals drive the tie-break.
3. **Second-bar `String` / `GetImage2DWidth` order.**  Original constructs the
   `String` before `GetImage2DWidth` (halfW); ours does halfW first.  Swapping
   in source drops to 9328 B / 94.79 %.
4. **Shield/armour `[sp,#108]` reloads.**  Original has extra
   `ldr.w r8,[sp,#108]` / `ldrd fp,r1,[sp,#116]` loads around the shield
   blocks; ours hoists/sinks them differently.
5. **Trailing EH-pad `b.n` offsets** (`0x242c..0x2444`) differ by one
   instruction slot.

---

## 4. Section deltas (wave 125)

| section | orig | ours | delta | differing bytes |
|---|---|---|---|---|
| `.text` | 1,259,464 | 1,259,464 | 0 | **85,912** (was 85,975) |
| `.ARM.exidx` | 28,528 | 28,528 | 0 | **0** (byte-identical) |
| `.ARM.extab` | 156,312 | 156,312 | 0 | 1,474 |
| `.rodata` | 269,159 | 269,159 | 0 | **0** (byte-identical) |
| `.rel.plt` | 29,392 | 29,392 | 0 | **0** (byte-identical) |
| `.note.gnu.build-id` | 36 | 36 | 0 | 20 |

File size exact (2,187,328 B); all 27 section headers present and exact.

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7520
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target lint          # CLEAN
cmake --build cmake-build-match --target drift -j6     # 0 fields
python3 tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7520`: `build.sh` (link + score), `score.py`
(raw ratio + cost + hunks), `adiff.py` (address-tagged normalised diff),
`localsearch.py` / `localsearch2.py` / `randsearch.py` / `randsearch2.py`
(type/order sweeps).  Pristine wave-124 HEAD copy: `/dev/shm/w7520/Hud.cpp.orig`;
landed variant: `/dev/shm/w7520/Hud.cpp.best4`.
