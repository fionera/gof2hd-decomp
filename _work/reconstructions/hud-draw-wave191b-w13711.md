# Hud::draw — wave 191b (w13711): plain-block / `&Globals::status`-liveness re-probe (no win)

Worker `w13711`, branch `wave191b-w13711`, base master `d4e8f032d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13711`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw **19** /
aligned **6** / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.extab` for this TU is **0x1dc (476 B)** in every candidate measured
below (HEAD, plain, and all insert variants) — no extab growth.

| target | symbol | VMA | size | raw/aligned | insn |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9,332 | **19 / 6** | 3226/3226 |

## 0. Residual restated (unchanged)

Only the `hitFlags & 0x24` / `& 0x18` damage block differs
(`0x164184-0x16419f`, 19 bytes in 2 groups):

```
orig  164184 tst.w  r1,#36         f011 0f24
      164188 itt    ne             bf1c
      16418a movne.w r0,#300       f44f 7096
      16418e strne.w r0,[sl,#1208] f8ca 04b8
      164192 tst.w  r1,#24         f011 0f18
      164196 itt    ne             bf1c
      164198 movne.w r0,#300       f44f 7096
      16419c strne.w r0,[sl,#1204] f8ca 04b4
```

HEAD's unbraced anchor (`if (hitFlags & 0x24) Status **volatile p =
&Globals::status; this->field_0x4b8 = 300;`) hoists the shared `mov r0,#300`
into the join, makes the `field_0x4b8` store unconditional and spends the
freed 4 B on the anchor's `strne r8,[sp,#208]`.  Both blocks are 28 B, so the
anchor is a *substitution*, not an addition.

## 1. The exact arithmetic of the target (re-confirmed this session)

HEAD is byte-identical to the original everywhere except the 28-byte block.
`plain` (anchor removed) is **9,328 B**, i.e. exactly **4 B shorter** than
HEAD.  The missing 4 B is the single dead `ldr.w r8,[sp,#108]` at
`0x164176` (reload of `&Globals::status`; `r8` is overwritten by
`Globals::Canvas` at `0x1641e4` before any use).

Therefore:

> **plain block + that one dead reload = the original, byte for byte.**

The target is *not* "plain + frame growth": the frame is a consequence of the
same live range.  HEAD's frame-224 / `#108` spill map is already the
original's; the anchor only supplies the use that keeps `&Globals::status`
live at `0x164176`.  Any construct that creates the liveness must emit
**zero** bytes before `0x164184`, or the whole 0x176-byte tail shifts.

## 2. Measured this session (object-level, `screen.build`/`search.build`)

`size`/`frame`/`block`/`reload` from the single-TU `Hud.cpp` object;
`proxy` is `odiff.diff` (relocation-masked same-address byte diff — an upper
bound on linked raw; HEAD's own proxy is 0).  `reload` = the exact
`ldr.w r8,[sp,#108]` 14 B before the block.  Linked numbers are
`tools/fn_diff.py` on `libgof2hdaa.so`.

| variant | size | frame | block | reload | proxy |
|---|---:|---:|---|---:|---:|
| **HEAD** anchor in `&0x24` if | **9332** | 224 | rotated | yes | **19** |
| plain (no anchor) | 9328 | 216 | **EXACT** | no | 5843 |
| `(void)*(Status *volatile*)&Globals::status;` after `field_0x4b8 -= dt;` | **9332** | **224** | **EXACT** | **yes** | **318** |
| same, after `GetImage2DHeight(frontImage)` | 9332 | 224 | EXACT | yes | 330 |
| `volatile int pad = (int)(intptr_t)&Globals::status;` after `field_0x4b8 -= dt;` | 9332 | 224 | EXACT | yes | 318 |
| `volatile unsigned pad = (unsigned)(uintptr_t)&Globals::status;` there | 9332 | 224 | EXACT | yes | 318 |
| `(void)*(volatile Status * volatile *)&Globals::status;` there | 9332 | 224 | EXACT | yes | 318 |
| `volatile short/char pad = (short/char)(intptr_t)&Globals::status;` there | 9336 | 224 | EXACT | yes | 5223 |
| `Status *volatile pad = Globals::status;` there | 9336 | 224 | EXACT | yes | 5223 |
| `(void)*(volatile long long/double/u64 *)&Globals::status;` there | 9336 | 224 | EXACT | yes | 5223 |
| `__builtin_prefetch(&Globals::status);` there | 9336 | 224 | EXACT | yes | 5223 |
| `__builtin_prefetch(Globals::status);` there | 9336 | 224 | EXACT | yes | 5248 |
| `(void)Globals::status->getMission();` there | 9336 | 224 | EXACT | yes | 5244 |
| `volatile int pad = hitFlags;` there | 9636 | 224 | other | no | 6442 |
| `volatile int pad = 0;` there | 9332 | 216 | EXACT | no | 2477 |
| `volatile short/char pad = 0;` there | 9336 | 216 | EXACT | no | 5483 |
| `(void)Globals::status;` / `if (Globals::status) {}` / `(void)&Globals::status;` there | 9328 | 216 | EXACT | no | 5843 |
| `(void)Globals::status->field_178;` there | 9328 | 216 | EXACT | no | 5843 |
| volatile read before the mission panel | 9328 | 216 | EXACT | no | 5843 |
| volatile read before `SetColor(0xffffffff)` (damage tail) | 9332 | 216 | EXACT | no | 2473 |
| volatile read at end of function (last statement) | 9336 | 216 | EXACT | no | 5848 |
| `volatile int pad = (int)(intptr_t)&Globals::status;` at end of function | 9336 | 216 | EXACT | no | 5846 |
| `Status *st = Globals::status;` before `field_0x4ac` guard, `st->` in panel | 9240 | 224 | other | no | 6679 |
| `Status *volatile st = …` there, `st->` in panel | 9280 | 224 | EXACT | no | 5742 |
| `Status *st = Globals::status;` before panel, `st->` in panel | 9264 | 216 | EXACT | no | 6962 |
| `Status **p = &Globals::status;` before panel, `(*p)->` in panel | 9328 | 216 | EXACT | no | 5843 |
| `Status **volatile p = …` before panel, `(*p)->` in panel | 9292 | 224 | EXACT | no | 6614 |
| `Status *volatile st = Globals::status;` at fn top, `st->` in panel | 9220 | 224 | EXACT | yes | 7147 |
| anchor before `&1` / `&2` / `sideImage` / `frontImage` | 9356–9360 | 224 | EXACT | no | 6456–6476 |
| anchor before `&0x24` / `&0x18` | 9336 | 224 | other | no | 5666–5676 |
| anchor at top of damage section / before gamma / in rocket block | 9272–9368 | 216–224 | EXACT | no | 6471–7194 |
| `volatile int pad = 0;` before `&1`/`&2`/`sideImage`/`frontImage` | 9332–9336 | 224 | EXACT | no | 2337–5493 |
| `volatile int pad = 0;` at fn top | 9292 | 224 | EXACT | no | 7145 |
| in-if `ittt` read: `{field_0x4b8 = 300; (void)*(Status*volatile*)&status;}` | 9336 | 224 | other | no | 5631 |
| read after the whole `&0x24` block (before `&0x18`) | 9336 | 224 | other | no | 5626 |

Only the two `field_0x4b8` positions reproduce **size 9332 + frame 224 +
exact block + exact reload**; a full statement-boundary sweep of the damage
section (`sweep_read.py`, every `;`/`{`/`}` line from the damage comment to
the end of the function) finds exactly those two (318 / 330).

### 2a. Why the 318 variant cannot be driven to 0

The 2-byte volatile read is emitted as `ldr r0,[r6,#0]` at `0x16438c`
(`r6 = &Globals::status`, still live from the `0x16436e` reload).  It is
absorbed by the 2-byte literal-pool alignment `nop` at `0x164502`, so the
size stays 9,332 — but every instruction between `0x16438c` and `0x164502`
shifts by 2, and the five forward branches that span the insertion
(`0x1641fe`, `0x164568`, `0x16457a`, `0x16458e`, `0x164768`) change their
offsets.  That is the 318-byte proxy (linked **385 / 33**, first measured in
`hud-draw-wave191b-w13695.md` §2a).

The read must stay *inside* the damage section to create the extra
`&Globals::status` spill slot: the same read placed after the damage section
(before the mission panel, before `SetColor(0xffffffff)`, or at the end of
the function) leaves the frame at **216** and loses the `0x164176` reload.
Every 4-byte spelling of the read/store costs 9,336 B.

## 3. Why no win

`plain + dead reload` is the whole target, and the reload only exists if
`&Globals::status` is live at `0x164176`.  Every source construct that
creates that liveness and keeps the frame at 224 emits at least a 2-byte
instruction, and the only place that 2-byte instruction can be absorbed
(`0x164502`) is at the far end of the function, where the liveness is no
longer needed (frame drops to 216).  HEAD's anchor is still the cheapest
spelling: it reuses the plain block's spare 4 bytes and keeps raw at 19.

No variant measured this session beats HEAD's 19; the best plain-block shape
is 318 (linked 385/33).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13711
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness `scratch/w13711/`: `cc.sh` (single-TU compile with the
original flags), `measure.py`, `odiff.py` (relocation-masked object diff),
`screen.py` / `search.py` (variant builders), `sweep_read.py` (statement
sweep), `vhead.mir.txt` / `vplain.mir.txt` (`-mllvm -print-machineinstrs
-filter-print-funcs` dumps with final `Frame Objects:`).
