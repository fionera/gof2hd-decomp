# Generator::createMission — `strd r6,r0,[sp,#32]` is reachable with `short pick`, but only
# with the `used->data()[type]` loop that costs more than it gains (w5005)

Status: **research note, no source change landed** (wave 100, 2026-11-04, worktree `w5005`,
branch `wave100-w5005`, base `44225192c`). The landed `short pick` / `goto assignType` shape
(w4232/w4257/w4508/w4706/w4808/w4905) stays at **74.3 %** (`osize 1996  usize 1996`, 727
instructions).

Gate at HEAD (unchanged by this note):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass attacks the one question w4905 left open ("make clang spill `status` at `97228`
together with `race`, forming `strd r6,r0,[sp,#32]`, while keeping `k1b`'s loop"). It adds
four measured facts:

1. **`strd r6,r0,[sp,#32]` IS reachable from source** — but only with
   `used->data()[type]` (the w4706 `e1` indexing), which separates `type` into `r2`.  The
   first such shape found is `while` + `short pick` + loop-top `type = pick` +
   `used->data()[type]` + no `p10`/`p12` hint (`cand1`): frame **96**, `strd r6,r0,[sp,#32]`,
   `used -> sl`, `zero -> r5`, `systems -> r8`, `agent -> r9`, `type -> r2`, `movs r2,#10`.
   It scores **61.5 %** because `guard -> r4`, `pick -> r9` and `agent`/`targetStation` swap
   `[sp,24]`/`[sp,28]`.
2. **A 192-variant sweep** of `{loop form} x {pick type} x {loop-top type=pick} x
   {used index} x {p10 expect} x {p12 expect}` confirms HEAD (`for` + `short pick` + top
   assign + `used[pick]` + p10 expect) is the **global maximum of the family** at fastscore
   **0.557** / official **74.3**.  The best non-HEAD shape is `while` + `int pick` +
   `used[pick]` + p10 + p12 expect at fastscore 0.521 / official **70.9**.
3. **A 9-feature register/slot scan** over the same sweep: no variant reaches 9/9.  The best
   is 7/9, split between the `while int` family (missing `frame96` + `strd32`) and the
   `while short ... used[type]` family (missing `guard -> r9` + `pick -> r6`).  HEAD itself is
   5/9 (`frame96`, `systems -> r8`, `agent -> r9`, `zero -> r5`, `pick -> r6`).
4. **`0.5f` still cannot be landed**: every 1996 B build that keeps `0.5f` and adds 4 B of
   real code either re-allocates the tail or perturbs a pool word; the best remains
   `HEAD + 0.5f` at **74.5 % / 1992 B** (w4905 §7).

## 1. Where the previous passes left it

* HEAD = `for (; !selected; --guard)` + `short pick` + loop-top `type = pick` +
  `used->data()[pick]` + `__builtin_expect(pick == 10, 1)` + `0.5000001f` anchor.
  `type` and `pick` coalesce into `r6`; `r9` is burned on the `8 << 16` short-compare
  constant; `used` spills to `[sp,32]`; `guard -> r5`; `zero -> sl`.
* `k1b` (w4905 §4) = `int pick` + `while (!selected)` + `--guard` in the body + no loop-top
  `type = pick` + `goto assignType`.  It reproduces the original's **entire** register
  allocation and frame 96 (`systems->r8`, `agent->r9`, `guard->r9`, `used->sl`, `zero->r5`,
  `type->r2`, `pick->r6`, `selected->fp`, `rnd->[sp,40]`) and scores **67.0 %**; the only
  residual is `status -> [sp,28]` / `targetStation -> [sp,32]` instead of the original's
  `targetStation -> [sp,28]` / `status -> [sp,32]` (no `strd`).
* w4905 found `X_usedexp` (`k1b` + `__builtin_expect(used != nullptr, 1)`) does produce the
  original slot map **and** the `strd`, but at 1956 B / 57.4 %.

## 2. The 192-variant sweep

Bodies are generated from the pristine HEAD body by substituting only the loop block.  The
six axes:

```
loop form   : for (; !selected; --guard) | while (!selected) | do {} while (!selected) | for (;;) + break
pick type   : short | int
loop top    : type = pick; | (none)
used index  : used->data()[pick] | used->data()[type]
p10 hint    : __builtin_expect(pick == 10, 1) | plain
p12 hint    : __builtin_expect(pick == 12, 0) | plain
```

Measured with the fast scorer (difflib over the normalised instruction stream, the same
metric `tools/verify/verify.py` uses) and the official verifier for the finalists:

| variant | fastscore | insns | usize | official pct |
|---|---:|---:|---:|---:|
| **HEAD** (`for`, short, top, `[pick]`, p10) | **0.557** | 724 | 1996 | **74.3** |
| `for`, short, top, `[pick]`, p10, p12exp | 0.554 | 724 | 1996 | — |
| `while`, int, top, `[pick]`, p10, p12exp (`cand2`) | 0.521 | 722 | 1996 | 70.9 |
| `while`, int, (no top), `[pick]`, p10, p12exp | 0.521 | 722 | 1996 | — |
| `for`, short, top, `[type]`, p10 (`cand4` = w4706 `e1`) | 0.515 | 724 | 1996 | 67.4 |
| `for`, short, top, `[type]`, p10, p12exp | 0.512 | 724 | 1996 | — |
| `while`, short, top, `[pick]`, p10 | 0.511 | 725 | 2004 | — |
| `while`, short, top, `[type]`, no p10 (`cand1`) | 0.467 | 724 | **1996** | **61.5** |
| `for`, short, top, `[type]`, no p10 (`cand5`) | 0.454 | 723 | 1992 | 60.0 |
| `for`, int, top, `[type]`, no p10 | 0.383 | 713 | 2028 | — |

HEAD is the global max.  Every `int pick` build drops to <= 0.521 because the `for` loop form
that keeps HEAD's tail allocation is incompatible with `int pick` (frame 104 / `systems->fp`),
and every `used->data()[type]` build drops to <= 0.515 because it adds the loop-top
`mov r2,r5` and moves `guard` off `r5`.

## 3. `strd r6,r0,[sp,#32]` is reachable — `cand1`

`cand1` = HEAD with the loop replaced by

```c++
    while (!selected) {
        selected = guard < 1;
        short pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;
        if (pick == 8) goto assignType;
        if (pick == 10) {
            type = 10;
            if (race > 3) continue;
        }
        if (used != nullptr) {
            if (used->data()[type]) {          // <<< e1 indexing
                int sum = 0;
                uint32_t i;
                for (i = 0; i < used->size(); ++i) sum += used->data()[i];
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) used->data()[i] = false;
                    goto assignType;
                }
                type = pick;
                continue;
            }
            used->data()[pick] = true;
        }
        if (pick == 12) {
            selected = true;
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 25) {
                type = 12;
            } else {
                goto assignType;
            }
            continue;
        }
        selected = true;
    assignType:
        type = pick;
    }
```

Its prologue:

```
sub   sp, #96
mov   r8, r2                    ; systems
mov   r9, r1                    ; agent
...
str   r0, [sp, #24]            ; targetStation
str.w r9, [sp, #28]            ; agent
blx   Agent::getRace
strd  r6, r0, [sp, #32]        ; <<< &Globals::status, race
movs  r5, #0                   ; zero
ldr   r0, [r6, #0]
movw  r4, #999                 ; guard
movs  r2, #0                   ; type
mov   fp, #0                   ; selected
ldr.w sl, [r0, #80]            ; used
str   r0, [sp, #40]            ; &Globals::rnd
```

and its loop head:

```
sxth.w r9, r0                  ; pick  (r9, not r6)
mov.w  r1, #524288             ; 8 << 16 materialised *inside* the loop (r1)
cmp.w  r1, r0, lsl #16
beq.n  ...
lsls   r1, r0, #16
cmp.w  r1, #655360
mov    r2, r9                  ; type = pick
bne.n  ...
ldr    r0, [sp, #36]           ; race
movs   r2, #10                 ; type = 10   <<< the original's instruction
```

So `cand1` has the original's **frame 96, `strd r6,r0,[sp,#32]`, `used -> sl`, `zero -> r5`,
`systems -> r8`, `agent -> r9`, `type -> r2`, `movs r2,#10`** — 7 of the 9 features in §4 —
but `guard -> r4` (original `r9`), `pick -> r9` (original `r6`), and
`agent`/`targetStation` swapped (`[sp,28]`/`[sp,24]` instead of `[sp,24]`/`[sp,28]`).
Official score **61.5 % / 1996 B**.

The `strd` forms here because the `used->data()[type]` indexing keeps `type` live across the
`pick == 10` path, which raises `type`'s interference and (through the changed block
probabilities) moves `status`'s SSC weight above both `targetStation`'s and `agent`'s: the
slot order becomes `rnd > race > status > agent > targetStation`, so `status` takes the
8-byte-aligned colour `[sp,32]` next to `race` and the two adjacent spill stores merge into
`strd r6,r0,[sp,#32]`.  (The original order is `rnd > race > status > targetStation > agent`;
`cand1` still has `agent`/`targetStation` swapped, but the `strd` only needs `status` next to
`race`.)

## 4. Nine-feature scan

Features: `frame96`, `systems->r8`, `agent->r9` (`mov r9,r1`), `zero->r5` (`movs r5,#0`),
`strd r6,r0,[sp,#32]`, `used->sl` (`ldr.w sl,[r0,#80]`), `movs r2,#10`, `mov r2,r6`,
`pick->r6` (`mov r6,r0`, no `sxth`).

| variant | score | fastscore | usize | missing |
|---|---:|---:|---:|---|
| `k1b` (`int5b`: int, while, no top, `[pick]`, p10, `used` declared first) | **8/9** | 0.509 | 1996 | strd32 |
| `while`, int, top, `[pick]`, p10, p12 (`cand2`, HEAD declaration order) | 7/9 | 0.521 | 1996 | frame96, strd32 |
| `while`, short, top, `[type]`, no p10 (`cand1`) | 7/9 | 0.467 | 1996 | guard->r9, pick->r6 |
| `dowhile`, int, top, `[type]`, no p10 | 7/9 | 0.366 | 1984 | agent->r9, mr6 |
| `for`, short, top, `[type]`, p10 (`cand4` = `e1`) | 6/9 | 0.515 | 1996 | strd32, used->sl, mr6 |
| `while`, int, top, `[type]`, p10 | 6/9 | 0.467 | 1972 | sys->r8, m10, mr6 |
| **HEAD** | 5/9 | 0.557 | 1996 | strd32, used->sl, m10, mr6 |

`k1b` reproduces all of the original's *registers* (`systems->r8`, `agent->r9`, `guard->r9`,
`used->sl`, `zero->r5`, `type->r2`, `pick->r6`, `movs r2,#10`, `mov r2,r6`) and frame 96; its
only missing feature is the `strd` (its `status` and `targetStation` slots are swapped).
Note the declaration order matters: the sweep's `while int ...` variants use HEAD's
declaration order and land at frame 104 (7/9), while `int5b` declares `used` first and lands
at frame 96 (8/9).

No source shape found reaches 9/9.  The two halves are mutually exclusive in every measured
build: the `used->data()[type]` indexing is what produces `strd32` (through the SSC weight
flip) but it also forces `pick` into `r9` and `guard` into `r4`; the `used->data()[pick]`
indexing keeps `guard->r9`/`pick->r6` but never produces the `strd`.

## 5. `0.5f` remains unlandable

`HEAD + 0.5f` is **74.5 % / 1992 B** (w4905 §7, re-confirmed).  The four missing bytes must be
real code, and the only matching 4 B in the original's loop are the two `mov r2,r6` copies
(`9729a`, `972d2`) plus `movs r2,#10`/`movs r2,#12`, all of which require `type` in `r2`.
Every `type`-in-`r2` shape measured here (`cand1`, `cand4`, `e1`, `k1b`) scores 61.5-67.4,
i.e. 7-13 points below HEAD, far more than the +0.2 that `0.5f` buys.

## 6. Conclusion

The remaining gap is still one register-allocation question, now narrowed further:

> find a source shape with `used->data()[pick]` (so `pick -> r6`, `guard -> r9`) **and** the
> SSC weight order that puts `status` above `targetStation` (so `status -> [sp,32]`,
> `targetStation -> [sp,28]` and `strd r6,r0,[sp,#32]` forms), **and** `int pick` (so
> `mov r6,r0` / `cmp r6,#10` / `cmp r6,#12` / `movs r2,#10` / `movs r2,#12`).

`k1b` has the first and third but not the second; `cand1` has the second (and `type -> r2`)
but not the first/third (`short pick` keeps `pick` in `r9`, `guard` in `r4`); `HEAD` has the
first but neither the second nor the third.  No source spelling tried in
w4508/w4706/w4808/w4905/w5005 gives all three.  The `strd` and the `used->data()[type]`
indexing are causally linked (the indexing raises `type`'s interference and flips the SSC
weight order), so the next pass should look for a *different* way to raise `status`'s SSC
weight above `targetStation`'s while keeping `used->data()[pick]` — e.g. an extra hot-block
reference to `Globals::status` that does not change the loop's register pressure, or a
`__builtin_expect` on a branch that contains a `status` reload.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5005
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4500

# official single-symbol score
./run.sh <body.cpp> <label>     # splices, link_so, prints pct/osize/usize

# fastscore (relative guide; HEAD = 0.557, official 74.3)
./compile.sh <body.cpp> <out.o> && python3 fastscore.py <out.o>

# 192-variant loop sweep + 9-feature scan
python3 bigsweep.py
python3 featscan.py

# strd probe
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d <out.o> | grep 'strd\s*r6, r0, \[sp, #32\]'
```

Scratch harness (not committed, `/dev/shm/w5005`): `body_head.cpp` (landed body),
`compile.sh`, `run.sh`, `fastscore.py`, `bigsweep.py` (the 192-variant generator/sweep),
`featscan.py` (the 9-feature scan), `mkvar.py`/`mkvar2.py`/`mke1.py`/`mke1b.py`/`mkdual.py`/
`mkdw.py`/`mkgal.py` (variant generators), `regions.py`/`show.py` (per-region diffs),
`weights.py` (SSC weights from `-print-machineinstrs`), and the candidate bodies
`int5b.cpp` (= `k1b`), `cand1.cpp`-`cand5.cpp`, `e1.cpp`, `faithful.cpp`, `x_p12exp.cpp`,
`gbody.cpp`, `dualA-D.cpp`.
