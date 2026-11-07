# MenuTouchWindow::OnTouchBegin wave 129 (w7659): RA rematerialisation tie-break audit, no source win

Date: 2026-11-07. Worktree `wt/w7659`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `Android (4751641 based on r328903)`, `-Oz`,
image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`). Baseline and best measured this wave:
**98.092643 %**, 1896 B, `byte=False`, `linked=False`, **18** differing
instruction lines (header excluded).

Gate at HEAD and after this wave: `byte_exact 4506 / linked_exact 4506`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
**RATCHET PASS**. No source change landed (the residual is unchanged), so the
before/after counts are identical: **4506/4506 -> 4506/4506**.

## TL;DR

1. **Resolved the wave-128 "measurement inconsistency".** It was a reporting
   bug in `sweep2.py`, not nondeterminism: the script stored `r[0]` (the
   *original* size, always 1896) instead of `r[1]` (our size). Re-measured
   HEAD twice (`1896/18` both) and all-direct twice (`1892/164` both); the
   harness is deterministic.
2. Tested ~120 new source shapes this wave (see table). Every one falls into
   exactly one of two buckets: **1896 B / 18 diffs** (cached layout value
   `%29` live across the first `GetImage2DWidth` call; `&Globals::layout`
   `%99` rematerialised from `[sp,#16]`) or **1892 B / 164 diffs** (`%29`
   dead; `%99` kept in `fp`; `Globals::Canvas` value moves to `r5`).
3. The original has **both** properties at once: `%29` is dead at the call
   (`12a1fc` is its last use) **and** `%99` is rematerialised
   (`ldr r2,[sp,#16]` at `12a212`, `ldr r0,[sp,#16]` at `12a23c`). No legal
   C++ shape tested (now ~770 cumulative across waves 118-129) produces that
   combination.
4. RA knob sweep (`-regalloc-csr-first-time-cost` 0..500000,
   `-spp-rematerialization-threshold` 0..5000, `-exhaustive-register-search`,
   `-enable-deferred-spilling`, `-disable-spill-hoist`,
   `-sink-insts-to-avoid-spills`, `-split-spill-mode=size|speed`) leaves the
   all-direct shape at 1892/164.

## 1. Measurement determinism (resolved)

`/dev/shm/w7659/sweep2.py` appended `(cp, ls, r[0], r[2])`, i.e. the
*original* size and the diff count, not our size. That is why the same body
appeared as "1896" in `sweep2.json` and "1892" in a manual `vary2.run`. Fixed
harness (`/dev/shm/w7659/h.py`, `hw.py`) records `r[1]`. HEAD is stable at
1896/18; all-direct is stable at 1892/164.

## 2. New source shapes tested this wave

All measurements are full gold link + `tools/verify/verify.py --show`
(`/dev/shm/w7659/measure.py`); size is the linked function size, diffs are
structural `|` lines (header excluded).

### 2.1 Caching a field before the first call (`%29` dead at the call)

| variant | size | diffs |
|---|---:|---:|
| `int top = layout.field_0x20_top;` before first call | 1892 | 78 |
| `int lm = layout.field_0xc_leftMargin;` before first call | 1892 | 78 |
| both `top` and `lm` cached before first call | 1888 | 96 |
| `int right = Globals::w - layout.buttonInsetX;` (oracle) | 1892 | 164 |
| oracle `a,b,c` comma form, all-direct (wave-128 `v_oracle2`) | 1892 | 164 |
| oracle `a,b,c` comma form, ref for `a`/`inset` only | 1892 | 164 |
| oracle `a,b,c` comma form, ref for `c`/`lm` (live across call) | 1896 | 18 |
| oracle `a,b,c` comma form, ref for `a`/`top` (live across call) | 1896 | 18 |
| `unsigned idx = this->field_0x1e0;` cached, all-direct | 1888 | 168 |
| `unsigned *hb = this->heapBufB;` cached, all-direct | 1888 | 172 |
| `idx` + `hb` cached, all-direct | 1888 | 164 |
| `PaintCanvas *canvas = Globals::Canvas;` in block, all-direct | 1892 | 157 |
| `right` cached + `idx` cached, all-direct | 1888 | 168 |

### 2.2 Structure / scope

| variant | size | diffs |
|---|---:|---:|
| HEAD (`Layout &layout` after scrollSlots loop, `ref ref dir dir`) | **1896** | **18** |
| `Layout &layout` declared before scrollSlots loop | 1892 | 119 |
| `Layout &layout` declared at `case 15:` top (whole case braced) | 1920 | 683 |
| nested `if (c1) { Layout &layout = *Globals::layout; if (c2) ... }` (ref) | 1896 | 163 |
| nested `if` with `Layout *lp` | 1896 | 163 |
| nested `if`, all-direct | 1892 | 164 |
| lambda `[&]{ Layout &layout = *Globals::layout; return c2; }()` | 1892 | 164 |
| `bool hit = false; if (c1) hit = c2; scrollbarHit = hit;` | 1892 | 168 |
| `bool hit = c1 && c2; scrollbarHit = hit;` | 1892 | 168 |
| `scrollbarHit = c1 && c2;` | 1892 | 168 |
| `scrollbarHit = 0; if (c1 && c2) scrollbarHit = 1;` | 1896 | 167 |
| two nested `if`s, `scrollbarHit = 0` first | 1896 | 167 |
| `Layout *lp;` assigned inside c2 (`(lp = Globals::layout)->...`) | 1892 | 164 |
| `Layout *lp;` assigned in c2, c3 uses `lp` | 1892 | 166 |
| function-top `Layout *layout` pointer used in case 15 | 1908 | 673 |
| function-top `PaintCanvas *canvas` + all-direct case 15 | 1896 | 728 |
| `canvas` assigned between the two conditions | 1900 | 164 |
| `PaintCanvas &canvas` hoisted after function-top `layout` (unused) | 1896 | 18 |

### 2.3 Type / spelling / address expression

| variant | size | diffs |
|---|---:|---:|
| `const Layout &layout` / `const Layout *lp` | 1896 | 18 |
| `auto &layout` / `auto *lp` | 1896 | 18 |
| `Layout **lp = &Globals::layout; (*lp)->...` | 1892 | 164 |
| `Layout **lp = (Layout **)&Globals::layout; (*lp)->...` | 1892 | 164 |
| `*(Layout *volatile *)&Globals::layout` for c2 only | 1896 | 126 |
| same for c2+c3 / all three | 1896 | 129 / 135 |
| `Layout *volatile lp = Globals::layout;` | 1916 | 765 |

### 2.4 Parenthesisation / operand order (`ref ref dir dir` unless noted)

| c2 expression | size | diffs |
|---|---:|---:|
| `T + (L + H)` (HEAD) | **1896** | **18** |
| `L + (T + H)` | 1896 | 18 |
| `(T + L) + H` | 1896 | 23 |
| `(T + H) + L` | 1896 | 53 |
| `H + (T + L)` | 1896 | 54 |
| `(H + L) + T` | 1896 | 54 |
| `(L + H) + T` | 1892 | 86 |
| `L + (H + T)` | 1892 | 86 |
| oracle condition order (`right - width < x`, `... > x`) | 1912 | 295 |
| c1 written `w - width - inset` | 1892 | 170 |
| c3 written `width + inset` | 1896 | 22 |

All eight parenthesisations of the all-direct shape stay at 1892/164-169.

### 2.5 RA knobs (all-direct shape, manual TU compile)

`-regalloc-csr-first-time-cost=0,1,10,50,100,200,500,1000,2000,5000,10000`
-> 1892/164; `50000` -> 1892/716; `100000` -> 2008/796; `200000+` -> 2128/790.
`-spp-rematerialization-threshold=0..5000` -> 1892/164.
`-exhaustive-register-search`, `-enable-deferred-spilling=1`,
`-disable-spill-hoist=1`, `-sink-insts-to-avoid-spills=1`,
`-split-spill-mode=size|speed` -> 1892/164.

## 3. MIR evidence (re-confirmed)

Pre-RA IR is identical between HEAD and all-direct except for the liveness of
the cached layout value. `%99` is the GOT load of `&Globals::layout`
(`t2LDRi12 %98:tgpr, 0` at bb.1), used at bb.42 (c1), bb.44 (c2) and bb.47
(c3).

HEAD post-RA (`# *** IR Dump After Greedy Register Allocator ***`):

```
bb.42:  %29  = t2LDRi12 %508, 0       ; layout value
        %306 = t2LDRi12 %29, 40       ; buttonInsetX
        tBL GetImage2DWidth
bb.44:  %511 = t2LDRi12 %stack.0, 0   ; %99 REMAT
        %319 = t2LDRi12 %511, 0       ; fresh value
        %320 = t2LDRi12 %29, 32       ; top <-- %29 LIVE ACROSS CALL
        %323 = t2LDRi12 %319, 12      ; leftMargin
        tBL GetImage2DHeight
bb.47:  %510 = t2LDRi12 %stack.0, 0   ; %99 REMAT
        %335 = t2LDRi12 %510, 0
```

All-direct post-RA:

```
bb.42:  %301 = t2LDRi12 %508, 0       ; %508 = %99 remat, kept in fp
        %306 = t2LDRi12 %301, 40
        tBL GetImage2DWidth
bb.44:  %319 = t2LDRi12 %508, 0       ; %508 still live in fp
        %322 = t2LDRi12 %319, 12
        %323 = t2LDRi12 %319, 32      ; both fresh
        tBL GetImage2DHeight
bb.47:  %335 = t2LDRi12 %508, 0
```

The original (disassembly) is the *third* combination:

```
12a1bc  ldr    r4, [sp, #16]     ; %99 remat (before scrollSlots loop)
12a1f0  ldr    r2, [r4, #0]      ; %29
12a1fc  ldr    r2, [r2, #40]     ; buttonInsetX  (%29 dies here)
12a200  blx    GetImage2DWidth
12a212  ldr    r2, [sp, #16]     ; %99 REMAT
12a218  ldr    r2, [r2, #0]      ; fresh value
12a21e  ldr    r5, [r2, #12]     ; leftMargin fresh
12a220  ldr    r6, [r2, #32]     ; top fresh
12a222  blx    GetImage2DHeight
12a23c  ldr    r0, [sp, #16]     ; %99 REMAT
12a240  ldr    r2, [r0, #0]      ; fresh value
12a246  ldr    r5, [r2, #40]     ; buttonInsetX fresh
```

`fp` is `Globals::Canvas` in both the original and HEAD; all-direct moves the
Canvas value to `r5` and puts `&Globals::layout` in `fp`, which is what makes
it 4 B shorter (1x 32-bit `ldr.w fp,[sp,#16]` + 2-byte `ldr r5,[r1]` instead
of 3x 16-bit `ldr rX,[sp,#16]` + 4-byte `ldr.w fp,[r1]`).

The address rematerialisation offsets are otherwise identical between HEAD
and the original: `[sp,#16]` loads at function offsets `0x40`, `0x14c`,
`0x3b0`, `0x426`, `0x5e8`, `0x63e`, `0x668`, `0x6e2` in both. HEAD already
has the original's address/Canvas handling; the only semantic difference is
that `top` is read from the cached `%29` (`ldr r4,[r6,#32]`) instead of the
fresh value (`ldr r6,[r2,#32]`).

## 4. Root cause (re-confirmed, new framing)

The residual is a single Greedy-RA live-range/rematerialisation tie-break for
`%99` (`&Globals::layout`) that is **coupled to the liveness of `%29`** (the
cached `Globals::layout` value):

* `%29` live across the first call (every `ref` use of `layout` after the
  call): `fp = Globals::Canvas`, `%99` rematerialised -> **1896 B / 18 diffs**
  (one field read cached).
* `%29` dead at the first call (all-direct, or any cached local): `fp =
  &Globals::layout`, Canvas value -> `r5`, function shrinks -> **1892 B /
  164 diffs**.

The original is `%29` dead **and** `%99` rematerialised. The two decisions
could not be separated by any source shape tested this wave. The remaining
18 diffs are: Region A (`case 8`, `ip` vs `r4` for `listEntryWidth`), Region B
(first-condition register permutation in `case 15`), Region C (`top` from the
cached `%29`), and the third `if` (`buttonInsetX` in `r4` vs `r5`).

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w7659
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Scratch (not tracked): `/dev/shm/w7659/` — `measure.py` (compile TU ->
gold-link mini `.so` -> `verify.py --show`), `cc.sh`/`cc2.sh` (exact NDK r18b
flags), `h.py`/`hw.py` (case-15 replacement harnesses), `sw*.py` (sweeps),
`head.mir`/`all.mir` (`-print-after-all`), `orig.txt` (original disassembly).
