# `HangarWindow::render` — pass 7 (w4243): revert the 34e EH regression, lazy-load tie analysis

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave92-w4243`, base master `b379d00d`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones). This pass lands a **source + doc revert of
commit `34e76bbe`** and adds the lazy-load tie analysis requested for this target. No new
anchors, no new `volatile`/padding.

## TL;DR

- **`34e76bbe` was a regression.** It changed the `render` EH sink from `pad0` to a
  `volatile padS` to make `.ARM.extab` size-exact, but dropped the function from
  **80.6 % to 78.8 %** (raw `0.8064427312775331` -> `0.7878871300757054`). The task's
  stated current state (80.6 %, `.ARM.extab 0x26290 (-8)`) is the pre-34e source.
- This pass reverts the `src/game/ui/HangarWindow.cpp` hunk and the matching
  `docs/IDENTICALITY.md` text. After the revert: **80.6 %, 9700 B, 3626/3638 insns,
  `.ARM.extab 0x26290 (-8)`**, gate unchanged.
- **New evidence on the entry-block tie.** The original's `&Globals::layout` GOT load is
  lazy at `0x149136` and `sl` is `&layout`; the baseline hoists the load to `0x1490f6` and
  caches the value in `r4`. Removing the top `Layout *layout = Globals::layout;` local
  moves the load to `0x149136` but flips the roles to `this -> sl`, `&layout -> fp`,
  frame 384 (61.6 %). Caching three `Globals::gameText` uses restores `this -> fp` and
  frame 376 at `0x149136`, but `&layout` lands in `r9` (67.5 %). A hybrid
  (pre-34e body, lazy `drawBG`, three cached `gameText` uses) reproduces the first 22
  prologue instructions **byte-for-byte** (same literal offsets) with `&layout -> r5`,
  but is 12 B short (9688 B, 70.2 %). No shape beats 80.6 %.
- The missing `.ARM.extab` row is the cleanup split at `0x149656`/`0x149660`: the original
  has two 4-byte rows with landing pads `0x14b5a2`/`0x14b64c`; ours has one 18-byte row
  with `0x14b64a`. A dead non-volatile `int padS` at the item-kind test adds the row
  (record 948 B, `.ARM.extab 0x26298`) but costs 1.5 points (79.1 %) because the extra
  landing-pad thunk shifts every later landing-pad branch target. The trade is documented
  below; the revert keeps the higher function score and accepts `-8`.

## Baseline / gate

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331, insns 3626/3638
sections: .text 0x1337c8, .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), .rodata 0x41b67 / base 0x1c9e10
LSDA:   render record 940 B (original 948 B)
```

Environment (canonical build; the vendored Darwin `libc++abi.a`/`libandroid_support.a`
already in `.cache` are the real ones):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

## 1. The 34e regression (measured)

`34e76bbe` ("HangarWindow render EH pass") changed, in `render`:

```diff
@@ item-kind test @@
                         if (item->isItem() || item->isShip() || item->isBluePrint() || item->isPendingProduct()) {
+                            volatile int padS = 0; (void) padS;
@@ credit-row block @@
-                    volatile int pad0 = count;
+                    // Sink: keeps the original 4-byte size/regalloc delta.
                     volatile int pad0b = count;
```

Measured in this tree (same compiler, same link):

| source | `render` raw | pct | insns | `.ARM.extab` | render record |
|---|---:|---:|---:|---:|---:|
| `34e76bbe^` (`pad0`+`pad0b`) | 0.8064427312775331 | **80.6** | 3626/3638 | 0x26290 | 940 B |
| `34e76bbe` (`volatile padS`+`pad0b`) | 0.7878871300757054 | 78.8 | 3626/3639 | 0x26298 | 948 B |

`byte_exact`/`linked_exact` are 4498 in both, because `render` is not byte-exact either
way; the only gate-visible difference is `.ARM.extab` size. The task's stated gate
(`0x26290 (-8)`, `render` 80.6 %) therefore describes `34e76bbe^`; this pass restores it.
The w4209 note (`extab_hangarwindow_sink_w4209.md`) is kept as the analysis of the
EH-sink trade; its "948 B / 156 rows" result is the `padS` shape, which is the 78.8 %
body.

## 2. Entry-block tie: original vs baseline

Original (`objdump -d --start-address=0x1490d8 --stop-address=0x149142`):

```
1490d8: push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r7,r8,r9,sl,fp}
1490e0: vpush {d8-d10}
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp
1490e8: ldr  r0,[pc,#808]  ; pool 149414 ; stack-chk guard GOT
1490ea: ldr  r1,[pc,#812]  ; pool 149418 ; Canvas GOT
1490ec: add  r0, pc
1490ee: add  r1, pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#788]  ; pool 14941c ; &Globals::appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr.w r8,[r0]                   ; r8 = &appManager
149110: ldr.w r0,[r8]
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]              ; spill this
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491cc
149126: mov  r6, fp
149128: ldr.w r0,[r6,#88]!              ; this->viewMode
14912c: cmp  r0,#0
14912e: bne.w 14a83c
149132: ldr  r0,[pc,#748]  ; pool 149420 ; &Globals::layout (lazy)
149134: add  r0, pc
149136: ldr.w sl,[r0]                   ; &layout -> sl
14913a: ldr.w r0,[sl]                   ; layout value
14913e: blx  drawBG
```

Current 80.6 % baseline:

```
1490e6: mov  fp, r0                     ; this -> fp   (same)
1490e8: ldr  r0,[pc,#816]  ; stack-chk guard GOT
1490ea: ldr  r1,[pc,#820]  ; &Globals::layout GOT   <-- hoisted
1490ec: ldr  r2,[pc,#820]  ; Canvas GOT
1490ee: add  r0, pc
1490f0: add  r1, pc
1490f2: add  r2, pc
1490f4: ldr  r0,[r0,#0]
1490f6: ldr.w sl,[r1]                   ; &layout -> sl   <-- 0x1490f6, not 0x149136
1490fa: ldr  r2,[r2,#0]
1490fc: str  r2,[sp,#140]
1490fe: str  r0,[sp,#112]
149100: ldr  r1,[r0,#0]
149102: ldr  r0,[r2,#0]
149104: ldr.w r4,[sl]                   ; layout value cached in r4
149108: str  r1,[sp,#372]
14910a: mov.w r1,#-1
14910e: blx  SetColor
...
14913e: mov  r0, r4                     ; drawBG uses the cached value
149140: blx  drawBG
```

The baseline is structurally right (`this -> fp`, `&layout -> sl`, frame 376) but the
source's top-level `Layout *layout = Globals::layout;` makes clang hoist the GOT load into
the entry block and cache the value in `r4`. The original's source has no such local: the
`Globals::layout` GOT address is CSE'd into `sl` at the first use (`drawBG`) and the value
is reloaded (`ldr.w rX,[sl]`) at every later use (38 reloads in both builds).

### Removing the top local flips the tie

`Layout *layout = Globals::layout;` removed and `layout->drawBG()` -> `Globals::layout->drawBG()`
(one-line source change, everything else identical):

```
render: 0.6161241587693999 (61.6 %)
frame=384  this=sl  &layout=fp  layout load @0x149136
```

So the lazy load lands at the original address, but the allocator now gives `this` `sl`
and `&layout` `fp`, and the frame grows 376 -> 384. This is the tie the previous passes
identified: the source shape that fixes the load address breaks the register roles.

## 3. Recovering `this -> fp` with a lazy load

Sweep (previous pass, reproduced here on the faithful body): cache the first *N*
`Globals::gameText` uses in a top-level `GameText *gameText = Globals::gameText;` local.

| N | frame | this | &layout | load | score |
|---:|---:|---|---|---|---:|
| 0 | 384 | sl | fp | 0x149136 | 61.4 % |
| 1 | 384 | sl | fp | 0x149136 | 61.4 % |
| 2 | 384 | fp | r9 | 0x149136 | 59.8 % |
| **3** | **376** | **fp** | **r9** | **0x149136** | **67.5 %** |
| 4..19 | 376 | fp | r9 | 0x149136 | 63-67 % |

N=3 is the first shape that matches the original's frame, `this` role, and load address.
The remaining mismatch is the `&layout` register: `r9` instead of `sl`. The reason is
visible in the prologue: with N=3, `sl` is used for the spilled `Canvas` value
(`mov sl,r0` at 0x14923c, restored from `[sp,#140]` at 0x1492a4), so `&layout` is demoted
to `r9` and spilled to `[sp,#132]` at 0x1491fa. The original spills `&layout` to
`[sp,#132]` at 0x1491b0 and keeps `sl = &layout`; it reloads `Canvas` into `r8` from
`[sp,#140]` instead.

N=3 prologue (first 22 instructions are instruction-identical to the original; only the
PC-relative literal offsets differ because the body is 24 B short):

```
1490e6: mov  fp, r0
1490e8: ldr  r0,[pc,#852]
1490ea: ldr  r1,[pc,#856]
1490ec: add  r0, pc
1490ee: add  r1, pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#832]
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr.w r8,[r0]
149110: ldr.w r0,[r8]
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491c8
149126: mov  r6, fp
149128: ldr.w r0,[r6,#88]!
14912c: cmp  r0,#0
14912e: bne.w ...
149132: ldr  r0,[pc,#792]
149134: add  r0, pc
149136: ldr.w r9,[r0]        ; &layout -> r9 (original: sl)
14913a: ldr.w r0,[r9]
14913e: blx  drawBG
```

### Hybrid: pre-34e body + lazy load + N=3

Taking the pre-34e source, deleting only the top `Layout *layout` local (using
`Globals::layout->drawBG()`), and caching three `gameText` uses gives:

```
render: 0.702418797512094 (70.2 %)  9688 B  3626/3609 insns
frame=376  this=fp  &layout=r5  load @0x149136
```

Its first 22 instructions are **byte-identical to the original including the literal
offsets** (`ldr r0,[pc,#808]`, `ldr r1,[pc,#812]`), and `0x149132`/`0x149136` match the
original's instruction sequence except for the register:

```
149132: ldr  r0,[pc,#748]   ; same pool word as original
149134: add  r0, pc
149136: ldr  r5,[r0]        ; original: ldr.w sl,[r0]
149138: ldr  r0,[r5]        ; original: ldr.w r0,[sl]
14913a: blx  drawBG
```

It is 12 B short of 9700, so every later branch target is shifted; the score is still
below the hoisted baseline. This is the closest plain-C++ shape found to the original
entry block.

## 4. GOT-load counts inside `render`

Decoded from the `ldr rX,[pc,#N]; add rX,pc` PIC idiom against the `.rel.dyn` GOT slots
(previous pass, re-used here):

| symbol | original | 80.6 baseline |
|---|---:|---:|
| `Globals::appManager` | 1 | 1 |
| `Globals::layout` | **2** | **1** (value cached in `r4`) |
| `Globals::w` | **3** | **2** |
| `Globals::h` | 2 | 2 |
| `Globals::gameText` | 10 | 10 |
| `Globals::imageFactory` | 3 | 3 |
| `Globals::status` | 8 | 8 |
| `Globals::options` | 1 | 1 |

The original's second `Globals::layout` load is at `0x14a85a` (immediately before
`drawFooter`, after the `viewMode == 1` recursive-`render()` block); the pool word is at
`0x14aa7c` and resolves to the same GOT slot `0x210224`. The baseline's only `layout`
delta is therefore the prologue hoist/cache plus the missing `drawFooter`
rematerialisation.

## 5. Free-credits statement order (Ghidra)

Ghidra's `render` at the free-credits loop (lines ~915-920 of the decompile) is exactly:

```c
iVar10 = PaintCanvas::getWidth(...);
iVar11 = layout->field_0x2c_rowHeight;
uVar34 = PaintCanvas::getHeight(...);
PaintCanvas::SetColor(...);
... textY ... textX ...
iVar11 = iVar10 + iVar19 + iVar11;
```

i.e. **width -> rowHeight -> height -> SetColor -> textY -> textX**. The current source
is height-first (`int height = button->getHeight(); int textX = button->getWidth() + ...;
SetColor; textY; DrawString`). Implementing Ghidra's order faithfully measures 53.1 % on
the lazy body and 61.7 % on the hoisted body, so the current height-first shape is a
better local optimum even though it differs from Ghidra's statement order.

## 6. `.ARM.extab`: the missing row and the sink trade

Per-function extab record deltas of the 80.6 % build (record length = next extab record
address minus this one, parsed from `readelf --unwind`):

```
0x971b8  Generator::createMission        orig 172  ours 176   +4
0xc0214  Level::createShip               orig 336  ours 328   -8
0xce8ac  RecordHandler::recordStoreWrite orig 656  ours 640  -16
0x137d18 LevelScript::process            orig 348  ours 352   +4
0x1490d8 HangarWindow::render            orig 948  ours 940   -8
0x163b90 Hud::draw                       orig 476  ours 492  +16
                                                              ----
                                                                -8
```

`render`'s missing row is at code offset 1406 (`0x1490d8 + 1406 = 0x149656`):

```
orig  (0x149656, 4, lp=0x14b5a2, 0)   (0x149660, 4, lp=0x14b64c, 0)
ours  (0x149656, 18, lp=0x14b64a, 0)
```

This is the two-call amount block:

```cpp
int right, left;
if (tab == 1) {
    right = item->item->getStationAmount();   // 0x149656
    left  = item->item->getAmount();          // 0x149660
}
```

Both original landing pads (`0x14b5a2`, `0x14b64c`) are 2-byte thunks to `0x14b698`
(destroy the `name` String at `sp+0x138`); ours shares `0x14b64a` -> `0x14b68e`
(destroy `sp+0x12c` then `sp+0x138`). The split is a clang EH-scope artifact of the
original source, not a code difference.

Sink sweep on the 80.6 % body (dead locals at the item-kind test):

| form | score | `.ARM.extab` | render record |
|---|---:|---:|---:|
| none (baseline) | 80.6 | 0x26290 | 940 |
| `int padS = 0; (void) padS;` | **79.1** | **0x26298** | **948** |
| `int padS = count; (void) padS;` | 79.1 | 0x26298 | 948 |
| `{ }`, `if (0) { }`, `(void) 0;`, `;` | 80.6 | 0x26290 | 940 |
| `AbyssEngine::String padS;` | 78.2 | 0x2629c | 952 |
| `volatile int padS = 0;` (34e) | 78.8 | 0x26298 | 948 |

The 1.5-point cost of the working sink is the new 2-byte landing-pad thunk: it shifts
every later landing-pad branch target, and the verifier compares branch targets
literally (`loc_<offset>`). The main body is unchanged. A sink that adds the row without
emitting a thunk was not found; the section-size gate therefore trades against the
function score. This pass keeps the higher function score and restores the task's stated
`-8` gate.

The 34e doc text (`.ARM.extab` size-exact, 948 B / 156 rows) is reverted in
`docs/IDENTICALITY.md` in the same commit, per `AGENTS.md`.

## 7. Commands used

```sh
# score one function (autojunk=False, official verifier internals)
python3 /dev/shm/w4243/score.py            # 80.6, raw 0.8064427312775331

# prologue roles
python3 /dev/shm/w4243/roles.py            # frame=376 this=fp

# full opcode diff
python3 /dev/shm/w4243/diff.py > /dev/shm/w4243/diff_cur.txt

# extab per-record deltas
python3 /dev/shm/w4243/extab_deltas.py

# gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

## 8. What remains

- `render` is still 19.4 % off. The main unresolved axes are (a) the `&layout` register
  (`sl` vs `r9`/`r5`) once the load is lazy, and (b) the 38 `[sl]` value reloads vs the
  baseline's cached value. A source shape that keeps `this -> fp`, frame 376, lazy load
  at `0x149136`, *and* `&layout -> sl` would likely jump well past 80.6 %; the N=3
  `gameText` weight nudge gets three of the four.
- The `.ARM.extab` row split needs a source shape that gives the two amount calls
  different landing pads without shifting the landing-pad code (or a compensating +8
  record elsewhere, e.g. a partial `RecordHandler::recordStoreWrite` fix).
