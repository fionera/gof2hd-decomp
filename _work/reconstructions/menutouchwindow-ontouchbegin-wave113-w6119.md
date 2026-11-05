# MenuTouchWindow::OnTouchBegin — wave 113 (w6119)

Status: research pass, 2026-11-05. Worktree `wt/w6119`, branch
`wave113-w6119`, base = current master (`a979ace18`). **No `src/` change
landed**: the baseline source is still the best measured shape. This note
re-derives the residual from the 7.0.2 machine IR and records one more batch
of measured source shapes; it extends
`menutouchwindow-ontouchbegin-wave112-w6017.md` (and its wave 106/109/110
predecessors).

Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`; real VMA = Ghidra − 0x10000).

| symbol | size orig/ours | fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **98.1** | 18 |

Gate before and after (pristine source): `RATCHET PASS`, byte_exact 4500 /
linked_exact 4500 / size 4524 / data 843 / imports 207 / lint `CLEAN ✓` /
drift 0. Structural state unchanged: 27 sections, file size 2,187,328 B,
`.text` size 0x1337c8, `.ARM.exidx` 0x6f70, `.ARM.extab` **0x26298**
(size- and base-exact vs the original; the wave-110 prompt's "4 B short"
observation is stale — current master already has the full 0x26298).

## 1. The residual, re-derived

The 18 structural lines are two independent groups.

### 1a. case 8 first hit-test (2 lines) — `ip` vs `r4`

At `0x129fd2` (case 8, the up/down-button hit test):

```
orig:  ldr.w ip, [sl, #340]     ours:  ldr.w r4, [sl, #340]
       add   r3, ip                    add   r3, r4
```

`this->listEntryWidth` (offset 0x154) is loaded into a short-lived `rGPR`
and added to `leftMargin + top`. Both `ip` and `r4` are dead at that point;
the same expression in case 3 (`0x129d4a`) already matches with `ip` in both
binaries. Pure greedy tie-break: in case 3, `r4` is occupied by
`&Globals::layout` (loaded at `0x129d20` and live across the `play()` call),
so greedy falls through to `ip`; in case 8 nothing occupies `r4`, so greedy
takes it.

### 1b. case 15 scrollbar block (16 lines) — rematerialise address vs cache value

Original (`0x12a1bc` sets `r4 = [sp,#16] = &Globals::layout`, `fp = &Globals::Canvas`):

```
block 1:  ldr r2,[r4,#0]      ; Globals::layout  (fresh)
          ldr r2,[r2,#40]     ; buttonInsetX
block 2:  ldr r2,[sp,#16]     ; &Globals::layout (rematerialised)
          ldr r2,[r2,#0]      ; Globals::layout
          ldr r5,[r2,#12]     ; leftMargin
          ldr r6,[r2,#32]     ; top
block 3:  ldr r0,[sp,#16]     ; &Globals::layout (rematerialised)
          ldr r2,[r0,#0]      ; Globals::layout
          ldr r5,[r2,#40]     ; buttonInsetX
```

Ours (baseline `Layout &layout = *Globals::layout;` used for block-1 inset and
block-2 top):

```
block 1:  ldr r6,[r4,#0]      ; Globals::layout cached in r6
          ldr r5,[r6,#40]     ; buttonInsetX
block 2:  ldr r2,[sp,#16]     ; &Globals::layout
          ldr r4,[r6,#32]     ; top  <- cached r6, not fresh load
          ldr r2,[r2,#0]      ; Globals::layout
          ldr r5,[r2,#12]     ; leftMargin
block 3:  ldr r0,[sp,#16]     ; &Globals::layout
          ldr r2,[r0,#0]      ; Globals::layout
          ldr r4,[r2,#40]     ; buttonInsetX
```

Both binaries already keep `fp = &Globals::Canvas` and rematerialise
`&Globals::layout` from `sp+16`; the only difference is that the baseline
caches the *value* `Globals::layout` in `r6` across the `GetImage2DWidth`
call and reuses it for `top`.

## 2. MIR evidence (clang 7.0.2 `-mllvm -print-after-all`)

Baseline (this source), pre-RA around the case-15 block:

```
%508:gpr  = t2LDRi12 %stack.0, 0        ; &Globals::layout
%29:gpr   = t2LDRi12 %508:gpr, 0        ; Globals::layout  (cached)
%304:gpr  = t2LDRi12 %300:tgpr, 0       ; &Globals::Canvas
%306:rgpr = t2LDRi12 %29:gpr, 40        ; block 1 buttonInsetX
%320:rgpr = t2LDRi12 %29:gpr, 32        ; block 2 top  <- reuses %29
```

Post-RA: `%508 -> $r4`, `%29 -> $r6`, `%304 -> $r11 (fp)`.

All-direct source (`Globals::layout->` at every site), pre-RA:

```
%508:gpr  = t2LDRi12 %stack.0, 0        ; &Globals::layout
%301:gpr  = t2LDRi12 %508:gpr, 0        ; block 1 value
%319:gpr  = t2LDRi12 %508:gpr, 0        ; block 2 value (fresh)
%335:gpr  = t2LDRi12 %508:gpr, 0        ; block 3 value (fresh)
%304:gpr  = t2LDRi12 %299:tgpr, 0       ; &Globals::Canvas
```

Post-RA: `%508 -> $r11 (fp)`, `%304 -> $r5`.

So the all-direct shape has the original's exact fresh-load pattern but greedy
puts the *layout address* in `fp` and the *Canvas address* in `r5`, the
opposite of the original. The baseline's reference local adds a live-across-
call value, which raises pressure enough to spill `%508` to `sp+16` and give
`fp` to `&Globals::Canvas` — but the same local is what makes `%29` cacheable
for `top`. The two effects are coupled; no tested C++ spelling separates them
without adding an instruction.

## 3. Measured shapes this wave

`H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`.
All variants keep the rest of the function at baseline.

| variant | size | fuzzy | diff lines |
|---|---:|---:|---:|
| **baseline `Layout &layout` + cached top** | **1896** | **98.1** | **18** |
| `const Layout &layout` (same uses) | 1896 | 98.1 | 18 |
| `Layout *const layout = Globals::layout` (same uses) | 1896 | 98.1 | 18 |
| block-1 comparison reversed (`w - inset - width < x`) | 1896 | 98.1 | 18 |
| block-3 comparison reversed (`inset + width > x`) | 1896 | 98.1 | 18 |
| case-8 4th condition right-assoc `lm + (top + width)` | 1896 | 98.1 | 18 |
| case-8 4th condition `top + lm + width` | 1896 | 98.1 | 18 |
| case-8 4th condition `lm + width + top` | 1896 | 98.0 | 19 |
| case-8 4th condition subtract form `y - lm - top < width` | 1896 | 87.5 | 112 |
| case-8 `{ int w = listEntryWidth; ... }` braces + local | 1892 | 88.2 | 337 |

Earlier waves (106/109/110/112) measured the rest of the shape space:
all-direct 90.3/164; both cached 90.9/163; `Layout **lp` 90.3/164;
`Layout *&lp` 90.3; `PaintCanvas *canvas` 90.5; `int w/inset/top/lm` locals
90.3–90.6; bool assign / ternary / nested-if 89.4–90.3; `w - width - inset`
88.7; `(top + H) + lm` 94.4; `volatile` 89.7 / 76.2; function-top locals
45.0–52.4; case-8 casts / `+0` / `*1` / `(unsigned)` 97.8–98.1; case-8
`width + top + lm` 97.7; case-8 `{ }` block + `Layout *l` 66.6.

## 4. Conclusion

The baseline is a genuine local optimum: it is the unique shape that keeps
`fp = &Globals::Canvas` and rematerialises `&Globals::layout` from `sp+16`,
at the cost of the `r6` value cache for `top`. The all-direct shape has the
original's exact pre-RA IR for the block but greedy assigns `&Globals::layout`
to `fp` (saving 4 B, 1892 B, 90.3 %). The case-8 `ip`/`r4` difference is a
pure allocator tie-break that canonicalises back to `r4` under every tested
spelling. No landable improvement was found.

Before → after: byte 4500 → 4500, linked 4500 → 4500, fuzzy 98.1 → 98.1,
size 1896 B. No `src/` change; only this note.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6119
TMPDIR=/dev/shm/w6119/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w6119/tmp cmake --build cmake-build-match --target check -j6
```

Per-variant measurement: build `link_so`, then
`tools/verify/verify.py --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv`.
MIR: compile the TU with the `compile_commands.json` command plus
`-mllvm -print-after-all`; the greedy dump is the last
`# Machine code for function _ZN15MenuTouchWindow12OnTouchBeginEiiPv` block
before `Virtual Register Rewriter`.
