# Radar::draw wave 146 (w9207) — per-window diff table; worst windows are shift, not local code

**Status:** analysis note, 2026-11-08. Worktree `wt/w9207`, based on `master`
(`31297fadc`, wave 145). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Original 9,908 B; ours 9,908 B. Baseline raw same-address diff **8,373 B**
(unchanged from wave 145). `.text` 85,651 differing bytes; `.ARM.extab` 1,370
(section); `.rel.plt` 0. Gate `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave145-w9009.md`, `radar-draw-wave145-w9010.md`,
`radar-draw-wave143-w8807.md`, `radar-draw-wave143-w8808.md`.

## 1. Mission and answer

Mission: build a per-window (256 B) diff table for the 9,908-byte body, identify
the 3 worst windows, and reconstruct only those from the original disassembly;
re-check the LSDA rows (the record differs by 86 bytes).

**Answer: the three worst raw windows are `0x1400` (253), `0x1500` (252) and
`0x0900` (252). They are not locally wrong code — they are the same instruction
streams as the original, displaced by a global branch-layout shift (+20, −54,
−58 B respectively). The residual diff in those windows is therefore a *shift
artefact*; reconstructing the windows in isolation cannot lower raw. The true
defects are the block-placement deltas upstream (section 3).**

## 2. Per-window (256 B) raw same-address diff table

`off` is relative to `0x12e050`. `raw` = differing bytes in that 256-B window.

| win | off | raw | | win | off | raw | | win | off | raw |
|---:|---:|---:|---|---:|---:|---:|---|---:|---:|---:|
| 0 | 0x0000 | 18 | | 13 | 0x0d00 | 248 | | 26 | 0x1a00 | 235 |
| 1 | 0x0100 | 4 | | 14 | 0x0e00 | 251 | | 27 | 0x1b00 | 247 |
| 2 | 0x0200 | 15 | | 15 | 0x0f00 | 239 | | 28 | 0x1c00 | 233 |
| 3 | 0x0300 | 199 | | 16 | 0x1000 | 236 | | 29 | 0x1d00 | 247 |
| 4 | 0x0400 | 248 | | 17 | 0x1100 | 247 | | 30 | 0x1e00 | 197 |
| 5 | 0x0500 | 249 | | 18 | 0x1200 | 236 | | 31 | 0x1f00 | 249 |
| 6 | 0x0600 | 246 | | 19 | 0x1300 | 228 | | 32 | 0x2000 | 243 |
| 7 | 0x0700 | 239 | | 20 | **0x1400** | **253** | | 33 | 0x2100 | 250 |
| 8 | 0x0800 | 249 | | 21 | **0x1500** | **252** | | 34 | 0x2200 | 251 |
| 9 | **0x0900** | **252** | | 22 | 0x1600 | 250 | | 35 | 0x2300 | 242 |
| 10 | 0x0a00 | 250 | | 23 | 0x1700 | 251 | | 36 | 0x2400 | 243 |
| 11 | 0x0b00 | 250 | | 24 | 0x1800 | 78 | | 37 | 0x2500 | 228 |
| 12 | 0x0c00 | 246 | | 25 | 0x1900 | 149 | | 38 | 0x2600 | 125 |

**Worst three: `0x1400` (253), `0x1500` (252), `0x0900` (252).**
Next: `0x2200` (251), `0x1700`/`0x0e00` (251), `0x2100`/`0x1600`/`0x0b00`/`0x0a00` (250).

An instruction-level structural diff (normalising branch targets) ranks the
windows `0x1100` (94/94 instructions differ), `0x1300` (93/93), `0x1500`
(89/89), `0x1400` (88/88), `0x1600` (84/84). Those are the gas-cloud /
asteroid preheader region; they look 100% different only because the whole
stream is displaced there.

## 3. Where the shift comes from (call-aligned delta curve)

Aligning the two functions by their 344/345 `bl`/`blx` call sequences gives an
exact delta at every call site. The delta is piecewise constant and resyncs at
four places (`0x12f8c0`, `0x12fb0e`, `0x13069c`, end). The large jumps are:

| orig addr | delta | cause (source-level) |
|---|---:|---|
| `0x12e368` | 0 | prologue identical except frame 296 vs 304 and `fp`/`sl` role swap |
| `0x12e3aa` | +4 | our second `otherLock` guard uses `itt eq` + `ldreq/cmpeq` and re-checks `field_0x1c` in the cold block; original uses a plain `cbnz` chain and checks `lockedStation` only |
| `0x12e4b0` | −46 | **campaign target block is inline in the original (`0x12e474`) but cold in ours (`0x12e5ca`)** — the `__builtin_expect(...,0)` on the outer campaign condition (wave 143 w8808) sinks the whole body; the original sinks only the 116/120/125 `else-if` chain |
| `0x12e8de` | +24 | landmark loop is rotated: original bound-checks at the top (`0x12e7d4`), ours at the bottom (`0x12eb0e`) |
| `0x12ea3e` | −54 | `i==0` String/lockLabel block order |
| `0x12ec2e` | −80 | enemy-loop entry |
| `0x12ef48` | −120 → −34 | **enemy-loop off-screen tail emits two `DrawImage2D` calls (ours `0x12eed0` and `0x12ef24`); the original computes one `image` and shares one call at `0x12ef38`** — call census: ours 13 `DrawImage2D`, original 12 |
| `0x12f3a8` | +6 | gas-cloud guard |
| `0x12f460` | −54 | **`navWreck` block is inline in the original (`0x12f3de`) but out-of-line in ours (`0x12f87e`)** — ours branches *to* the body (`beq 0x12f858`), original falls through into it (`bne 0x12f40e`) |
| `0x12f8c0` | 0 | **resync** (gas-cloud lock tail) |
| `0x12faae` | +22 | asteroid loop entry (wave-145 w9009 known +22) |
| `0x12fb0e` | 0 | **resync** (asteroid loop) |
| `0x12ff08` | +52 | music tail: ours has a separate `FModSound::play` for the 146/147 branch (ours 8 play, original 7) |
| `0x130186` | −272 | **music tail block order**: original has one shared `play(event)` at `0x130698`; ours duplicates the whole 136/140/141/142/145/148/152 tail |
| `0x13069c` | 0 | **resync** (music tail end) |

So the raw metric is dominated by two large, independently fixable source
shapes: the **enemy-loop single-`DrawImage2D` tail merge** (≈86 B of shift at
`0x12ef48`) and the **music-tail single `play(event)`** (≈272 B of shift at
`0x130186`). The three worst windows sit between the `0x12f460` and `0x12f8c0`
jumps.

## 4. Worst-window reconstruction (original disassembly)

### 0x1400 / 0x1500 (`0x12f450`–`0x12f650`) — gas-cloud spark loop

The original and ours are instruction-identical here; only the displacement
differs. Original spark loop (from `0x12f488`):

```
ldr.w r0,[fp,#4] ; ldr.w r1,[r0,r8,lsl#2] ; mov r0,r6 ; blx getPosition
add r3,sp,#172 ; mov r0,r4 ; ldmia r3,{r1,r2,r3} ; blx update(Vector)
ldrb.w r0,[r4,#284] ; cmp r0,#0 ; beq 0x12f594
... inSight bounds (crosshairPos +/- field_0x124) ...
vcmpe.f32 s22,s20 ; movs r0,#0 ; vmrs ; it ls ; movls r0,#1
ldr r1,[sp,#148] ; and.w r2,r1,r0 ; mov r0,sl ; mov r1,r8 ; blx setSparkInSight
ldr.w r9,[sp,#144] ; add r6,sp,#172 ; ldr r5,[sp,#152] ; b 0x12f59e
0x12f594: mov r0,sl ; mov r1,r8 ; movs r2,#0 ; blx setSparkInSight
0x12f59e: add.w r8,r8,#1 ; ldr.w r0,[fp] ; cmp r8,r0 ; bcc.w 0x12f488
```

Ours (`0x12f452`–`0x12f56e`) is the same stream with `sp` offsets +8 and
`[sp,#148]`→`[sp,#160]`, `[sp,#144]`→`[sp,#148]`, `[sp,#152]`→`[sp,#156]`.
No local source edit can improve these windows while the `0x12f460` shift
remains.

### 0x0900 (`0x12e950`) — landmark `i<=1` / `lockLabel` block

Original `0x12e922`–`0x12e9e4` and ours `0x12e938`–`0x12e9f2` are the same
instructions in a different schedule: original does `ldrd r3,r6,[r4,#252]`
before `cmp sl,#0`/`it eq`/`moveq r9,#556`; ours hoists the `cmp`/`it` first.
The `calcDistance` → `String::operator=` → `~String` → `DrawString` sequence is
identical (original `lockLabel` at `[sp,#104]`, ours `[sp,#112]`). The +20 B
displacement comes from the `0x12e8de`/`0x12ea3e` landmark-loop rotation, not
from this block.

## 5. LSDA / `.ARM.extab` record

`readelf -u` gives the same exidx entry for both: `0x12e050: @0x1c203c`,
personality `0x5eb60`, next record `0x1c20c0` → **record size 132 B in both**.
Content differs by **exactly 86 bytes** (as stated in the mission). The first
16 bytes (personality/type-table header) are identical; the divergence starts
at record offset `0x10` (`ffff0170` vs `ffff016f`) and is confined to the
call-site table rows — i.e. it is a consequence of the body/cleanup-region
layout, not an independent defect. Any body fix must re-check these rows; a
size change would break the size-exact `.ARM.extab` (156,312 B) and the
byte-identical `.ARM.exidx`.

## 6. New hypotheses tried this wave (all measured, none landed)

Baseline raw **8,373** / size 9,908 / frame 304 (original frame 296).

| variant | usize | frame | raw |
|---|---:|---:|---:|
| baseline | 9908 | 304 | **8373** |
| drop `__builtin_expect(...,0)` on campaign condition | 9892 | 304 | 9275 |
| `bool otherLock` (instead of `int`) | 9756 | — | 9126 |
| `bool landmarkHit` | 9912 | — | 8711 |
| `bool notOtherLock` | 9908 | 304 | 9138 |
| `bool notTurretMode` | 9836 | — | 9541 |
| drop `notOtherLock` (inline `!otherLock`) | 9908 | 304 | 9138 |
| drop `notTurretMode` (inline `!turretMode`) | 9836 | — | 9541 |
| **`isTarget = stations->data()[i] == campaignMission->getTargetStation()`** (witnessed operand order) | 9908 | **296** | 8682 |
| `isTarget` swap on freelance block | 9976 | 304 | 8772 |
| gas-cloud guard `!(A \|\| B \|\| turretMode \|\| otherLock)` | 9908 | 304 | 8373 (identical) |
| gas-cloud guard De Morgan `!A && !B && !turretMode && !otherLock` | 9908 | 304 | 8373 (identical) |
| gas-cloud guard `__builtin_expect(...,1)` | 9944 | 304 | 8778 |
| enemy-loop single shared `DrawImage2D` (w9010 tail merge) | 9880 | — | 8795 |

Notes:
* Removing the campaign `expect` makes the block hot (matching the original's
  inline placement) but costs 16 B and raw jumps to 9,275; a +16 B companion
  would be needed and the net raw is unknown.
* The `isTarget` operand swap is the only variant that reproduces the
  original's **frame 296** (it forces `campaign` back into `sl`), but raw
  regresses to 8,682. It is a faithfulness candidate for the coordinator, not a
  metric win.
* The enemy-loop merge removes the extra `DrawImage2D` (call census 13→12,
  matching the original) but is 28 B short and raw 8,795 on this base.

## 7. Open questions (for the next wave)

1. The campaign `expect(...,0)` sinks the *whole* body; the original sinks only
   the 116/120/125 chain. A source shape that keeps the outer body inline at
   9,908 B is the largest single win in `0x12e4b0`–`0x12e8de` (−46 B of shift).
2. The enemy-loop single-`DrawImage2D` tail merge (w9010) needs a +28 B
   faithful companion; the original's shared call is at `0x12ef38`.
3. The music tail must be one shared `play(event)` (original `0x130698`);
   ours duplicates the whole tail (−272 B of shift at `0x130186`).
4. The `navWreck` block is inline in the original (`0x12f3de`); ours is
   out-of-line (`0x12f87e`). This is the sole cause of the `0x12f460` −54 B
   shift and therefore of the two worst windows.

## 8. Gate (after, source unchanged)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
