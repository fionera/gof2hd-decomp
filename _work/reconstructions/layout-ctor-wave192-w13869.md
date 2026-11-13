# Layout::Layout C1/C2 — wave 192 (w13869)

Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`, `src/game/ui/Layout.cpp`, size 7,072 B.

Baseline on entry: raw **5,585**, aligned **2,005**, size 7,072/7,072, insn 2,516/2,552,
`.text`/`.ARM.extab`/`.ARM.exidx`/`.rodata`/`.rel.plt` deltas all 0.

## Landed: dedup `field_0x184` store

The source assigned `field_0x184` twice in a row (identical expression).  The original writes it
only once, in body C: the store is `strd`-shaped and appears once in the `d1d00..d2200` block.
Deleting the second copy:

| | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| before | 5,585 | 2,005 | 7,072 | 2,516 |
| after  | 5,614 | 1,993 | 7,072 | 2,518 |

`.text` delta stays 0, `.ARM.extab` 594, `.rel.plt` byte-identical.  Raw rises (branch-offset
shift) but the aligned drop is 12 slots.

## Open findings carried forward

* Original has two independent `Globals::iPadHD` tests: `d103a` (body A, cached in `sl`) and
  `d1276` (body B, cached in `fp`).  Do not merge the two source blocks (lines 14–23 and line 130).
* `footerTextInset` (src line 23) is a real duplicate in the original: body B writes
  `0x154/0x158 = 100/100` via `strd r1,r1,[r0,#340]`.  Deleting it shrinks size to 7,040 → dead end.
* Original stores `0x158/0x15c/0x160/0x168/0x170` via `vstr s0` float round-trips
  (`vcvt.f32.s32` / `vcvt.s32.f32`); ours emits `strd`/`str.w`.  Wrapping the RHS in
  `(int)(float)(int)` is folded away by clang 7 at -Oz for all five fields (measured: 0x158
  5,645/2,077; 0x15c 6,004/2,237 size 7,080 ✗; 0x160 no-op; 0x170 no-op; 0x174 5,660/2,095
  size 7,036 ✗).
* Original body-A vst1 run covers `0x128,0x140,0x150,0x160,0x170,0x180,0x190,0x1a0,0x1b0,0x1c0,0x1d0`
  (16 B each, literal pools `d14c8..`); ours defers `0x140/0x1c0/0x1d0` to body C and emits
  `0xdc` in body A instead.  Naive line moves of those three assignments all shrink size
  (6,960 / 6,984 / 6,984 / 6,848 / 6,944 ✗).
