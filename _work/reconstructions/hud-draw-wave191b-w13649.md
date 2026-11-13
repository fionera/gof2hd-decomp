# Hud::draw — damage-block frame-object sweep (wave 191b, w13649)

Worker `w13649`, branch `wave191b-w13649`, base master `4b2b1c10f`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13649`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw 19 /
aligned 6 / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |

## 0. Residual restated

Everything outside `0x164184-0x16419f` is byte-identical.  The shipped block is
*plain* (each store carries its own `movne`):

```
orig  164184 f011 0f24  tst.w  r1,#36
      164188 bf1c       itt    ne
      16418a f44f 7096  movne.w r0,#300
      16418e f8ca 04b8  strne.w r0,[sl,#1208]   ; field_0x4b8
      164192 f011 0f18  tst.w  r1,#24
      164196 bf1c       itt    ne
      164198 f44f 7096  movne.w r0,#300
      16419c f8ca 04b4  strne.w r0,[sl,#1204]   ; field_0x4b4
```

HEAD's unbraced anchor `if (hitFlags & 0x24) Status **volatile p =
&Globals::status; this->field_0x4b8 = 300;` hoists the shared `mov r0,#300`,
makes the `field_0x4b8` store unconditional and puts the anchor store in the
slot of the second `movne`.  Both blocks are 28 B, so the anchor is a
*substitution*, not an addition.

The original also has a **dead** `ldr.w r8,[sp,#108]` at `0x164176` (reload of
`&Globals::status`; r8 is overwritten by `Globals::w` at `0x1641e4` before any
use).  HEAD has the same reload, but the anchor store consumes it.

## 1. New evidence: frame objects (MIR `Frame Objects:` after PEI)

A `-mllvm -print-machineinstrs -mllvm -filter-print-funcs=...` dump of
`Hud.cpp` (scratch `mir2_base.txt` / `mir2_plain.txt`) gives the final frame
objects.  HEAD and the no-anchor build differ by exactly **one 4-byte stack
object**:

| build | frame | 12-byte String temps | 4-byte objects |
|---|---:|---:|---:|
| HEAD (anchor) | 224 | 9 | **16** |
| plain (no anchor) | 216 | 9 | **15** |

The anchor's `Status **volatile p` alloca is `fi#1: dead` (stack-coloured onto
another slot, DWARF location `sp+208`).  The frame growth is therefore **not**
the volatile alloca itself: it is the extra 4-byte *spill slot* that the
`&Globals::status` live range across the block creates (the slot at `sp+60`
that holds `Globals::w`/`Globals::h` in the 4th damage indicator).  This is why
a `volatile int pad = 0;` gives frame 224 but shifts every String temp down by
4 (its alloca sits at `sp+216`, above the String temps), whereas the anchor's
alloca is dead-coloured and the String temps stay at `sp+208`.

Consequence: the target is **plain block + the dead `ldr.w r8,[sp,#108]` +
frame 224**, i.e. HEAD's frame layout with the anchor store deleted.  That is
9,332 B exactly (the dead reload is the 4 B that plain, 9,328 B, lacks).

## 2. Measured this session

`size`/`frame`/`blockmatch` from the single-TU `Hud.cpp` object (`probe.py`);
`layout` compares the `[sp,#N]`/`add rX,sp,#N` slot set against the original
(`layout.py`).  `orig-only: []` means the spill map matches the original
exactly.  Linked numbers are `tools/fn_diff.py` on `libgof2hdaa.so`.

| variant | size | frame | block | layout |
|---|---:|---:|---|---|
| **HEAD** anchor in `&0x24` if | **9332** | 224 | no | identical |
| plain, no anchor | 9328 | 216 | **yes** | shifted |
| `{ Status **volatile p = &Globals::status; }` after `&0x18` | 9336 | 224 | **yes** | identical |
| `{ volatile int p = (int)&Globals::status; }` after `&0x18` | 9336 | 224 | **yes** | identical |
| `{ volatile short/char p = (int)&Globals::status; }` after `&0x18` | 9336 | 224 | **yes** | identical |
| `{ volatile int p = 0; }` at P0/P1/P2/P4 | 9332 | 224 | **yes** | shifted (−4) |
| `{ volatile int p = hitFlags; }` at P0/P1/P2/P4 | 9332 | 224 | **yes** | shifted (−4) |
| `{ volatile Status **p = 0; }` at P0/P1/P2/P4 | 9332 | 224 | **yes** | shifted (−4) |
| `{ volatile int p = Globals::w; }` after `&0x18` | 9268 | 224 | **yes** | identical |
| `{ volatile int p = (int)sideImage; }` after `&0x18` | 9348 | 224 | **yes** | identical |
| `{ Status **volatile p = &Globals::status; }` before `&1` | 9360 | 224 | **yes** | identical |
| `{ Status **volatile p = &Globals::status; }` before `sideImage` | 9356 | 224 | **yes** | identical |
| `Status *const p = Globals::status;` before gamma, `p->` in gamma | 9340 | 224 | **yes** | identical |
| `Status *const p = Globals::status;` at fn top, `p->` in gamma | 9336 | 224 | **yes** | near (sp+44) |
| `Status *const p` at top, `p->` in gamma **and** panel | 9332 | 224 | no (`fp` for `this`) | identical |
| `int w = Globals::w; int h = Globals::h;`, uses replaced in damage section | 9128 | 224 | **yes** | identical |
| `int w/h`, uses replaced in first damage block only | 9244 | 224 | **yes** | identical |
| `volatile int hitFlags = ego->field_0x20;` | 9340 | 224 | no (`r0`) | shifted |

Linked spot checks:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **19** | **6** | 9332 |
| plain | 7340 | 914 | 9328 |
| `{ Status **volatile p = &Globals::status; }` after `&0x18` | 7070 | 407 | 9336 |
| `Status *const p` before gamma, `p->` in gamma | 7784 | 597 | 9340 |
| `int w/h` all damage uses replaced | 8774 | 1325 | 9128 |

## 3. Why no win

Every construct that produces the plain block *and* `orig-only: []` pays at
least one 4-byte instruction for the `&Globals::status` liveness that the frame
needs:

* a `volatile` store (`str r8,[sp,#208]`) → 9,336 B;
* a `Status *const p = Globals::status` initialisation/spill → 9,336–9,340 B;
* `volatile int`/`Status**` zero/`hitFlags` stores → 9,332 B but their alloca
  sits at `sp+216`, shifting every String temp by 4 (`orig-only` non-empty).

The only 9,332 B / frame-224 / plain-block shape found (`volatile int p = 0` at
P0/P1/P2/P4) has the shifted spill map, so its raw is in the thousands.  The
original's dead `ldr.w r8,[sp,#108]` is an RA live-range-split artefact of the
`&Globals::status` live range (gamma block `0x164022` → mission panel
`0x164558`); no source construct found reproduces it without also emitting a
store or a load.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13649
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness: `scratch/w13649/` — `cc.sh` (single-TU compile with the
original flags), `probe.py` (size/frame/block bytes), `layout.py` (slot set vs
original), `blk.py` (block context), `odiff.py` (object instruction diff),
`linkmeasure.sh` (link + `fn_diff`), `sweep*.py` (the variant matrices above),
`mir2_base.txt` / `mir2_plain.txt` (MIR with `Frame Objects:`).
