# MenuTouchWindow::OnTouchBegin pass 6 — w4520

Date: 2026-11-03. Branch `wave95-w4520` (base master `4a88bd6e`).
Gate before/after: **byte_exact 4500 / linked_exact 4500 / size-exact 4524**,
data_exact 843/843, imports 207, `.text` `0x1337c8`, `.rodata` `0x41b67`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26290` (-8), lint `CLEAN ✓`, drift 0,
`RATCHET PASS`.  **No code change landed**; this pass adds ~130 further
source-shape measurements and two new pieces of evidence (a byte-patched
score probe and the preheader-register identity) to the pass-5 conclusion.

Prior notes (read first):

* `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass3-w4203.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass4-w4247.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass5-w4419.md`

## Current state (re-measured this pass)

| symbol | size orig/ours | official fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **97.7 %** (raw 97.68392370572208) | **27** (1 header + 2 case-8 + 24 case-15) |

The function is size-exact and semantically complete.  No `volatile`,
`__builtin_expect`, `__builtin_prefetch`, padding local or size anchor exists
inside `OnTouchBegin`.

## New evidence 1 — the case-15 preheader is already byte-identical

The pass-5 note described the residual as "Canvas in `fp` / layout
rematerialised from `[sp,#16]`".  A closer reading of the *current* shape
shows the preheader already matches the original exactly:

```
12a1ba: 2500        movs   r5, #0
12a1bc: 9c04        ldr    r4, [sp, #16]      ; &Globals::layout   <-- both
12a1be: e007        b.n    12a1d0
```

i.e. in the current 97.7 % shape the layout address is in **`r4`**, not `fp`,
and `fp` is loaded with `&Globals::Canvas` in the first `if`
(`ldr.w fp,[r1]` at `0x12a1ee`), exactly as in the original.  The residual is
therefore *not* "which global address gets `fp`" — that is already right.  It
is only:

1. the first `if` caches the **layout value** in `r6` (`ldr r6,[r4,#0]`,
   `ldr r0,[r6,#32]` top hoist, `str r0,[sp,#12]`) whereas the original loads
   the value into the caller-saved `r2` (`ldr r2,[r4,#0]`) and does not hoist
   `top`;
2. the second `if` uses the cached `r6` value and the `[sp,#12]` top spill,
   whereas the original re-loads `[sp,#16]` -> `[r2]` and loads
   `leftMargin`/`top` **before** the `GetImage2DHeight` call;
3. the third `if` is byte-identical except the final temporary (`r5` vs `r4`).

The current shape is the only one found that keeps `r4 = &Globals::layout`
(short range, dies after the first `if`) **and** `fp = &Globals::Canvas`
(long range, across both calls).  The direct source shape instead keeps
`&Globals::layout` in `fp` across both calls (because the second/third `if`
re-use the same SSA address value); that is 4 B shorter (1892 B) and scores
only 90.3 %.

## New evidence 2 — a byte-patched score probe for case 8

To measure the exact value of the case-8 tie-break independently of the
case-15 residual, the two differing bytes were patched directly into a copy of
our `.so` (`.text` file offset == VMA in this link):

```
orig  129fd2: f8da c154   ldr.w  ip, [sl, #340]   ; listEntryWidth
      129fd8: 4463        add    r3, ip
ours  129fd2: f8da 4154   ldr.w  r4, [sl, #340]
      129fd8: 4423        add    r3, r4
```

Raw `difflib` ratio over the normalised instruction stream (`autojunk=False`):

| build | raw ratio | reported |
|---|---:|---:|
| current `libgof2hdaa.so` | 97.68392370572208 | 97.7 |
| current + case-8 bytes patched | 97.82016348773843 | 97.8 |

So fixing only the case-8 `ip`/`r4` tie-break is worth **+0.14 raw / +0.1
reported**, and would *not* change `byte_exact` (the function still differs in
case 15).  Conversely, the case-15 residual is worth the remaining ~2.2.

## Source-shape sweep this pass

All rows replace only the relevant block, rebuild the whole `.so`
(`cmake --build cmake-build-match --target link_so -j8`) and score with the
official verifier.

### A. Case-15 family: 96-variant sweep

Dimensions: Canvas access ∈ {direct, local `PaintCanvas *canvas`}; each of
first-`if` buttonInsetX / second-`if` leftMargin / second-`if` top / third-`if`
buttonInsetX ∈ {direct, local `Layout &layout`}; second-condition order ∈
{H+TOP+LM, TOP+LM+H}; `int top` local ∈ {yes,no}.  Best rows:

| variant | size | fuzzy |
|---|---:|---:|
| `c0_drdd_HTOPLM_t1` (first direct, lm ref, top local, third direct) | 1896 | **97.7** |
| `c0_rrdd_HTOPLM_t1` (current shape) | 1896 | **97.7** |
| `c0_drdd_TOPLMH_t1` | 1896 | 97.5 |
| `c0_rrdd_TOPLMH_t1` | 1896 | 97.5 |
| `c0_ddrd_TOPLMH_t0` | 1896 | 97.1 |
| any local-`canvas` variant | 1880–1896 | ≤ 90.5 |

The local-`PaintCanvas` variants all lose the `&Globals::Canvas` address in
`fp` (they cache the Canvas *value*), so they are strictly worse.

### B. Case-8 family: 24-variant sweep

Six association orders of `leftMargin + top + listEntryWidth` in the first
`if`'s fourth condition × four local configurations (`int width`, `Layout
&layout`, `int top`+`int lm`, none).  Best rows:

| variant | size | fuzzy |
|---|---:|---:|
| `LM+TOP+W` (current) | 1896 | **97.7** |
| `TOP+LM+W` | 1896 | 97.7 |
| `TOP+W+LM` | 1896 | 97.7 |
| `W+LM+TOP` / `LM+W+TOP` / `W+TOP+LM` | 1896 | 97.5 |
| any local-`width`/`layout`/`top`+`lm` variant | 1892–1916 | ≤ 87.9 |

The current `LM+TOP+W` order is already optimal; the `ip`/`r4` choice is not
driven by the expression association.

### C. Targeted shapes (all measured, all ≤ 97.7)

* local `PaintCanvas *canvas` / `PaintCanvas &canvas` / `PaintCanvas *&canvas`
  / `PaintCanvas **canvas = &Globals::Canvas` (with direct layout,
  TOP+LM+H): 1892 B, 90.3–90.5 %.
* comma-assigned `Layout *layout` in the first operand, in the second operand,
  or in both: 1892 B, 90.3 %.
* nested `if` with a local `Layout *layout`/`Layout &layout` in the taken
  branch, with `bool hit`, with direct assignments, and with `int top`:
  1888–1896 B, 88.9–90.1 %.
* two local `Layout *` copies (`l1`/`l2`), `Layout **p2 = &Globals::layout`,
  `Layout *&p = Globals::layout`: 1892 B, 90.3–90.9 %.
* separate `{ Layout &layout = *Globals::layout; ... }` scopes for the
  scrollbar block and the drag block: 1892 B, 88.4–90.9 %.
* function-scope `Layout *layout`/`Layout &layout` and `int w = Globals::w`
  locals: 1884–1892 B, 89.5–90.6 %.
* pointer instead of reference for the current shape (`Layout *layout` +
  `int top` + `layout->...`, third direct): 1896 B, **97.7 %** (identical
  code to the reference form).

## Machine-IR evidence (re-confirmed)

`clang++ -mllvm -print-machineinstrs` on the TU shows:

* the direct shape's pre-RA IR has three `tLDRLIT_ga_pcrel @_ZN7Globals6layoutE`
  loads (bb.38/bb.39/bb.41), but the GOT address `%stack.0` is a single SSA
  value; after Machine CSE the second/third `if` re-use it, so the allocator
  keeps it in `fp` across both calls (1892 B);
* the current shape's pre-RA IR loads the layout value once (`%29`) and uses
  it in both the first and second `if` (`%321 = [%29,12]`, `%30 = [%29,32]`),
  so the value is live across the first call and is assigned `r6`; the address
  `%508` is then dead after the first `if` and the third `if` re-loads
  `%stack.0` (matching the original's third `if`).

The original's second `if` (`ldr r2,[sp,#16]; ldr r2,[r2,#0]; ldr r5,[r2,#12];
ldr r6,[r2,#32]; blx`) requires the layout **value** to be re-loaded after the
first call and the address to be re-loaded from the spill slot.  Every source
form that makes the second `if` use direct `Globals::layout` also makes the
address live across the first call, and the allocator then puts it in `fp`
(the 1892 B direct shape).  Every source form that keeps the address dead
(caches the value in a local reference) also makes the second `if` use the
cached value (the 1896 B current shape).  The two requirements have not been
satisfied simultaneously by any shape measured across passes 3–6.

## Conclusion

The current shape is the local optimum of the reachable source space
(~500 variants measured across passes 3–6).  The residual is a clang 7.0.2
Greedy-RA live-range-splitting/rematerialisation tie-break on the spilled
`&Globals::layout` address, not a missing source construct:

* case 8: 2 bytes, `ip` vs `r4` for the `field_0x154` temporary;
* case 15: the layout value is cached in `r6` (current) instead of re-loaded
  from `[sp,#16]` (original), with the `top` load hoisted/spilled.

No source shape found in this pass is a strict improvement; the note is
committed alone.

## Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8

# official per-function score / side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv

# raw (unrounded) score probe, including the case-8 byte patch
python3 - <<'PY'
import sys, difflib, shutil
sys.path.insert(0, 'tools/verify')
import verify
obj = '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
of = verify.Elf32('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
shutil.copy('cmake-build-match/libgof2hdaa.so', '/dev/shm/w4520/patched.so')
with open('/dev/shm/w4520/patched.so', 'r+b') as f:
    f.seek(0x129fd2); f.write(of.bytes_at(0x129fd2, 2))
    f.seek(0x129fd8); f.write(of.bytes_at(0x129fd8, 2))
def raw(path):
    ours = verify.Elf32(path); orig = of.functions()
    o = orig['_ZN15MenuTouchWindow12OnTouchBeginEiiPv']
    u = ours.functions()['_ZN15MenuTouchWindow12OnTouchBeginEiiPv']
    common = set(orig) & set(ours.functions())
    orr = verify.Resolver(orig, common, of.plt_map(), (0, 0x200000))
    urr = verify.Resolver(ours.functions(), common, ours.plt_map(), (0, 0x200000))
    co = {x[2] for x in of.objects()} & {x[2] for x in ours.objects()}
    orr.attach_data(of, co); urr.attach_data(ours, co)
    imp = ({v for k, v in of.got_map().values() if k == 'sym'} &
           {v for k, v in ours.got_map().values() if k == 'sym'})
    orr.imports = urr.imports = imp
    _, on = verify.disassemble(obj, of.bytes_at(o['value'], o['size']), o['thumb'], o['value'], orr)
    _, un = verify.disassemble(obj, ours.bytes_at(u['value'], u['size']), u['thumb'], u['value'], urr)
    on, un = verify.reconcile(on, un)
    return difflib.SequenceMatcher(None, on, un, autojunk=False).ratio() * 100
print('current        ', raw('cmake-build-match/libgof2hdaa.so'))
print('case8 patched  ', raw('/dev/shm/w4520/patched.so'))
PY
```

Scratch harness (not tracked, `/dev/shm/w4520`): `MenuTouchWindow.cpp.orig`,
`measure.py`, `sweep.py`, `sweep2.py`, `sweep8b.py`, `apply.py`, `apply8.py`,
`run.sh`, `run8.sh`, `dump.sh`, `mi_*.log`.
