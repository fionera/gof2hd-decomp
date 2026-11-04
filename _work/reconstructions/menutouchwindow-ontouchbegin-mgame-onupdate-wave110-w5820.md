# MenuTouchWindow::OnTouchBegin + MGame::OnUpdate — wave 110 (w5820)

Status: research pass, 2026-11-04. Worktree `wt/w5820`, branch `wave110-w5820`,
base `390f58052`. **No source change landed**: both functions are already at
their best measured shape. This note extends
`menutouchwindow-ontouchbegin-wave109-w5720.md` (which itself extends
wave-106) and `mgame-onupdate-wave108-w5620.md` (which extends waves
97/105/106/107) with a new batch of measured source shapes, focused on
*forcing the greedy-RA split* without adding an instruction.

Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`; real VMA = Ghidra − 0x10000).

| symbol | size orig/ours | fuzzy | diff regions |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **98.1** | 12 |
| `_ZN5MGame8OnUpdateEv` | 11440 / 11440 | **99.9** | 2 |

Gate before and after (pristine source restored): `RATCHET PASS`,
byte_exact 4500 / linked_exact 4500 / size 4524 / data 843 / imports 207 /
lint `CLEAN ✓` / drift 0 / avg 99.92.

Structural state unchanged (wave-108): **28 sections** (orig 27; the 192-byte
`Status::missionCompleted` rank table lives in a `.jcr` section in the
`.rodata..fini_array` gap), `.ARM.extab` `0x26298` orig vs `0x26294` ours
(4 B short), file size `2187328` orig vs `2187372` ours (+44). `.text`,
`.rodata`, `.data`, `.bss`, `.ARM.exidx` are size- and base-exact.

## 1. MGame::OnUpdate — the residual (unchanged)

The whole 2-region residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2767-2776`). Original vs baseline:

```
original (0x17e08c..)                 ours baseline
ldr.w r9,[r0]    ; &Globals::status   ldr.w r9,[r0]
ldr.w r0,[r9]                         ldr.w r5,[r9]   ; st
blx   getCurrentCampaignMission       mov   r0,r5
cmp   r0,#23                          blx   getCurrentCampaignMission
bne                                   cmp   r0,#23
ldr.w r0,[sl]                         bne
blx   isMining                        ldr.w r0,[sl]
cmp   r0,#0                           blx   isMining
bne                                   cmp   r0,#0
ldr.w r0,[r9]                         bne
blx   getMission                      ldr.w r0,[r9]
blx   isEmpty                         blx   getMission
cmp   r0,#0                           blx   isEmpty
beq                                   cmp   r0,#0
ldr.w r0,[r9]                         beq
blx   getPlayingTime                  mov   r0,r5
ldr.w r2,[r9]                         blx   getPlayingTime
                                      ldr.w r2,[r9]
```

The original IR is the *no-`st`* shape (four distinct `ldr [r9]` loads). The
baseline `Status *st = Globals::status;` used at `getPlayingTime()` is an RA
anchor: it occupies `r5` in the compact region, makes `tryAssign` fail and
forces `&Globals::status` into `r9` (wave-107 §2). Without it the compact
piece gets `r5` and every `Globals::status` load in the whole if-chain
shrinks from 4 B to 2 B (11392 B, 87.2 %, 336 regions).

### 1.1 New measured shapes this wave

All measured with `link_so` + `measure2.py` (size, fuzzy, diff regions).
`st` is `Status *st = Globals::status;` unless noted.

| variant | size | fuzzy | regions |
|---|---|---:|---:|
| **baseline `st` at `getPlayingTime`** | **11440** | **99.9** | **2** |
| `st` at `ccm` only | 11392 | 87.2 | 336 |
| `st` at `ccm` + `getPlayingTime` | 11440 | 99.9 | 2 |
| `st` at `getPlayingTime` + `field` | 11436 | 92.7 | 239 |
| `st` at `getMission` + `getPlayingTime` | 11436 | 92.5 | 246 |
| `st` at `field` only | 11436 | 92.5 | 247 |
| `st` at `getMission` only | 11396 | 87.1 | 334 |
| no `st` (all-direct) | 11392 | 87.2 | 336 |
| `(st, Globals::status)->getPlayingTime()` | 11392 | 87.2 | 336 |
| `((void)st, Globals::status)->getPlayingTime()` | 11392 | 87.2 | 336 |
| `(st, (Status *)Globals::status)->getPlayingTime()` | 11392 | 87.2 | 336 |
| `(st ? Globals::status : Globals::status)->getPlayingTime()` | 11392 | 87.2 | 336 |
| `(st, Globals::status)->getCurrentCampaignMission()` | 11440 | 99.9 | 2 |
| `st` at `ccm` via comma, play via `st` | 11440 | 99.9 | 2 |
| `st` at `ccm` via ternary, play via `st` | 11440 | 99.9 | 2 |
| `Status *const st` | 11440 | 99.9 | 2 |
| `auto st` | 11440 | 99.9 | 2 |
| `int mission = ccm;` then `st` at play | 11440 | 99.9 | 2 |
| `st` then `int mission = ccm;` then play via `st` | 11440 | 99.9 | 2 |
| `bool mining = isMining();` + `st` at play | 11432 | 92.3 | 246 |
| `bool empty = isEmpty();` + `st` at play | 11432 | 92.2 | 250 |
| `long long last = field;` before the if, no `st` | 11392 | 60.1 | 727 |
| `long long last;` + `pt - (last = field)`, no `st` | 11392 | 87.2 | 336 |
| `long long &last = field;` + `st` at play | 11396 | 87.1 | 335 |
| `Status *&status = Globals::status;` alias, all sites | 11392 | 87.2 | 336 |
| `Status **status = &Globals::status;` alias, all sites | 11392 | 87.2 | 336 |
| `Status &status = *Globals::status;` (object ref) | 11432 | 92.4 | 246 |
| `st` assigned at `ccm` (comma), used at play | 11440 | 99.9 | 2 |
| `st` assigned after `isEmpty`, used at play+field | 11392 | 87.1 | 336 |
| `st` assigned at `ccm`, used at field only | 11436 | 92.5 | 247 |
| `st` assigned at `getMission`, used at play | 11396 | 87.1 | 334 |
| `st` assigned at `getMission`, used at play+field | 11392 | 87.1 | 336 |
| `bool idle = (whole condition);` + `st` at play | 11440 | 99.9 | 2 |
| fully parenthesised `&&` chain + `st` at play | 11440 | 99.9 | 2 |
| `3600000LL` / `60LL*60*1000` + `st` at play | 11440 | 99.9 | 2 |
| `reinterpret_cast<Status *>(Globals::status)` at ccm | 11440 | 99.9 | 2 |
| `*(Status *volatile *)&Globals::status` at ccm | 11440 | 99.0 | 38 |
| `st` used at m24 `ccm` (declared before inner if) | 11440 | 99.9 | 2 (diff moves to m24) |
| `st` used at m24 `getMission` | 11440 | 99.9 | 2 (diff moves to m24) |
| `st` used at m24 `getPlayingTime` | 11440 | 99.9 | 2 (diff moves to m24) |
| `st` used at m122 `ccm` | 11440 | 99.3 | 26 |
| `st` used at `inAlienOrbit` | 11440 | 99.3 | 26 |
| `st` used in the m23 body field write | 11524 | 81.7 | — |

**Conclusion.** Every shape that keeps the m23 loads fresh *and* `r9` for the
GOT base costs at least one 2-byte `mov` somewhere in the compact region. The
only byte-neutral placement is the current baseline. The volatile-ccm shape
does force a genuinely fresh 4-byte load at `ccm` (`ldr r5,[r9]` / `ldr
r0,[r9]` / `blx`) but re-colours the whole function (38 regions).

### 1.2 Why the natural (no-`st`) IR cannot be recovered

`-mllvm -print-after-all` on the no-`st` build (`/dev/shm/w5820/mg_nat_mir.txt`,
4.23 M lines) confirms the pre-RA shape is the original's (four distinct
`t2LDRi12 %1052, 0` loads at 42032B/42352B/42592B/42704B) but the post-RA
`Greedy Register Allocator` dump assigns the compact split piece `%2932` to
`$r5`:

```
42016B  renamable $r5 = t2LDRi12 killed renamable $r0, 0   ; GOT entry
42032B  renamable $r0 = t2LDRi12 renamable $r5, 0          ; Globals::status
```

The original has the same value in `$r9`. The greedy decision is the
`tryAssign`/`tryEvict` path documented in wave-107 §2: with `r5` free,
`tryAssign` returns it before the `$r9` hint is honoured. A code-neutral
change elsewhere in the function that raises the compact region's `$r5` cost
was not found.

## 2. OnTouchBegin — the residual (unchanged)

12 diff regions in two groups:

* **case 3** (`case 1/2` list block, 2 regions): original
  `ldr.w ip,[sl,#340]` / `add r3,ip`; ours `ldr.w r4,[sl,#340]` /
  `add r3,r4`. `r4` is dead in both at that instruction — a pure greedy
  tie-break for the short-lived `this->listRowGap` load.
* **case 15** (10 regions): the scrollbar block. Original:

```
ldr.w r0,[sl,#312]  heapBufB
ldr.w r1,[sl,#480]  field_0x1e0
ldr   r2,[sp,#16]   &Globals::layout   (rematerialised)
ldr.w r1,[r0,r1,lsl#2]
ldr   r2,[r2,#0]    Globals::layout
ldr.w r0,[fp]       Globals::Canvas
ldr   r5,[r2,#12]   leftMargin
ldr   r6,[r2,#32]   top
blx   GetImage2DHeight
adds  r1,r6,r5      top+leftMargin
```

Ours baseline caches the `Layout` value in `r6` and uses it for `top`
(`ldr r4,[r6,#32]`), so it saves one 4-byte `ldr` (1892 B all-direct) but
also re-colours the `GetImage2DWidth` setup block (heapBufB/field_0x1e0/
layout/`Globals::w` rotate through `r2/r3/r6/r4` instead of
`r3/r6/r2/r5`).

### 2.1 New measured shapes this wave

`H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`;
`Td/Tc = Globals::layout->field_0x20_top` / `layout.field_0x20_top`;
`Ld/Lc = Globals::layout->field_0xc_leftMargin` / `layout.field_0xc_leftMargin`;
`Id/Ic = Globals::layout->buttonInsetX` / `layout.buttonInsetX`.

| variant | size | fuzzy | regions |
|---|---:|---:|---:|
| **baseline `Ic + (Tc + (Ld + H))`** | **1896** | **98.1** | **12** |
| `Id + (Tc + (Ld + H))` | 1896 | 98.1 | 12 |
| `Ic + (Td + (Lc + H))` | 1896 | 98.0 | 12 |
| `Ic + (Tc + (Lc + H))` (all-cached) | 1892 | 90.9 | 59 |
| `Id + (Td + (Ld + H))` (all-direct) | 1892 | 90.3 | 59 |
| `Layout *layout = Globals::layout` all sites | 1892 | 90.9 | 59 |
| `Layout *layout` + direct leftMargin | 1896 | 98.1 | 12 |
| `Layout *layout` used at inset only | 1892 | 90.3 | 59 |
| `Layout *layout` used at top only | 1896 | 98.1 | 12 |
| `PaintCanvas *canvas = Globals::Canvas` + all-direct | 1892 | 90.5 | 57 |
| `PaintCanvas *canvas` + `Layout &layout` | 1896 | 97.6 | 12 |
| `PaintCanvas *&canvas` + all-direct | 1892 | 90.3 | 59 |
| `PaintCanvas &canvas = *Globals::Canvas` + all-direct | 1892 | 90.5 | 57 |
| `PaintCanvas &canvas` + `Layout &layout` | 1896 | 97.6 | 12 |
| `const Layout &layout` | 1896 | 98.1 | 12 |
| `volatile Layout &layout` | 1888 | 89.7 | 59 |
| `Layout *volatile layout` | 1928 | 76.2 | 116 |
| `bool xhit = (x > ...); if (xhit && ...)` all-direct | 1892 | 90.3 | 59 |
| `int right = ...; if (x > right && ...)` all-direct | 1892 | 90.3 | 59 |
| `int bottom = ...; if (... && y < bottom)` all-direct | 1896 | 90.0 | 52 |
| local used at inset + third block | 1888 | 90.1 | 57 |
| `Lc + Td + H` (left-assoc) all-direct | 1892 | 90.3 | 59 |

**Conclusion.** The case-15 residual is exactly "original rematerialises
`&Globals::layout` and uses the fresh value for both `leftMargin` and `top`;
ours caches the value in `r6`". The all-direct source shape has the correct
pre-RA IR (wave-109 §1.1) but `RAGreedy` keeps `%508` (`&Globals::layout`)
in `fp` and gives `&Globals::Canvas` `r5`, so the function is 4 B shorter
(1892). The baseline's `Layout &layout` local is what makes the same compiler
rematerialise the address (it occupies a callee-saved register) at the cost of
the `r6` value cache.

## 3. What is proven vs hypothesis

Proven (this wave):

* MGame's 2-region residual is exactly the m23 `Status*` cache decision; the
  no-`st` pre-RA MIR is the original's shape, and the compact split piece
  `%2932` is assigned `$r5` by greedy (post-RA dump), not `$r9`.
* Every `st` placement that leaves the m23 loads fresh and keeps `r9` costs at
  least one 2-byte `mov`; the baseline is the unique byte-neutral placement.
* OnTouchBegin's case-15 residual is the `r6` value cache; the all-direct
  shape has the right pre-RA IR but greedy keeps `&Globals::layout` in `fp`.
* The case-3 `ip`/`r4` difference is a pure allocator tie-break (`r4` is dead
  in both binaries at that instruction).

Hypothesis (not proven): the original source for both functions is the
all-direct / no-`st` shape, and a code-neutral IR difference elsewhere in the
function (vreg numbering / live-range order) flipped the 7.0.2 greedy
decision. No C++ spelling producing that difference without adding an
instruction was found in this wave either.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5820
TMPDIR=/dev/shm/w5820/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5820/tmp cmake --build cmake-build-match --target check -j6

# per-variant measurement (size / fuzzy / opcode regions):
python3 /dev/shm/w5820/measure.py  _ZN5MGame8OnUpdateEv
python3 /dev/shm/w5820/measure2.py _ZN5MGame8OnUpdateEv   # prints each diff region
python3 /dev/shm/w5820/measure.py  _ZN15MenuTouchWindow12OnTouchBeginEiiPv
python3 /dev/shm/w5820/measure2.py _ZN15MenuTouchWindow12OnTouchBeginEiiPv

# MIR dumps:
#   /dev/shm/w5820/mg_nat_mir.txt   -print-after-all on the no-st MGame shape
#   /dev/shm/w5820/mg_nat_greedy.txt  post-RA greedy dump (compact piece -> $r5)
#   /dev/shm/w5820/mg_nat_live.txt    pre-RA MIR (four fresh loads)
```

Scratch drivers in `/dev/shm/w5820`: `measure.py`, `measure2.py`,
`sweep_mg.py`, `sweep2.py`..`sweep9.py` (MGame), `sweep_mt.py`,
`sweep_mt2.py`..`sweep_mt8.py` (OnTouchBegin).

Gate after (pristine source): `RATCHET PASS`, byte 4500 / linked 4500 /
size 4524 / data 843 / imports 207 / lint `CLEAN ✓` / drift 0 / avg 99.92.
Before -> after: byte 4500 -> 4500, linked 4500 -> 4500.
