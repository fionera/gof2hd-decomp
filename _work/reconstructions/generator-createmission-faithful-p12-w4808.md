# Generator::createMission — the faithful `int pick` loop is reachable, but only at frame 104
# (w4808)

Status: **research note, no source change landed** (wave 98, 2026-11-04, worktree `w4808`,
branch `wave98-w4808`, base `fe4f5e7cf`). The landed `short pick` / `goto assignType` shape
(w4232/w4257/w4508/w4706) stays at **74.3 %** (`osize 1996  usize 1996`, 727 instructions).

Gate at HEAD (unchanged by this note):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass closes out the w4706 line. It records four new measured facts:

1. **The original's loop is reproduced instruction-for-instruction** by `int pick` + *no*
   loop-top `type = pick` + `__builtin_expect(pick == 12, 0)` + `goto assignType`
   (`f_p12exp`, 71.1 % / 1992 B) — the only residual loop differences are stack-slot offsets
   and the `assignType` branch target.
2. **That build's frame is 104, not 96**, because the `Galaxy *galaxy` local keeps
   `fromSystemPtr` live across the `toSystem` computation and forces a second temp slot.
   Dropping the local (or moving it after `toSystem`) restores frame 96 but re-allocates the
   post-loop region and drops the score to 68.9 %.
3. **All 720 declaration permutations** of `{race, level, selected, type, guard, used}` on the
   frame-96 shape score **68.9** — the slot assignment is not declaration-order sensitive.
4. **No variant reaches 1996 B with a score above 74.3.** The best 1996 B `int pick` build is
   `f_p12exp` + the `0.5000001f` anchor at **70.9 %**, frame 104.

## 1. The original loop, verbatim

Original (`android_2.0.16_libgof2hdaa.so`, `0x97248..0x972d8`):

```
97248: f04f 0b00  mov.w   fp, #0                 ; selected = false
9724c: f1b9 0f01  cmp.w   r9, #1                 ; guard < 1
97250: bfb8       it      lt
97252: f04f 0b01  movlt.w fp, #1
97256: 980a       ldr     r0, [sp, #40]           ; &Globals::rnd
97258: 210f       movs    r1, #15
9725a: 6800       ldr     r0, [r0, #0]
9725c: f7ca eaf4  blx     AERandom::nextInt
97260: f1a9 0901  sub.w   r9, r9, #1             ; --guard
97264: 4606       mov     r6, r0                 ; pick (int, no sxth)
97266: 2808       cmp     r0, #8
97268: d033       beq.n   972d2                  ; pick == 8 -> assignType
9726a: 2e0a       cmp     r6, #10
9726c: d103       bne.n   97276
9726e: 9809       ldr     r0, [sp, #36]           ; race
97270: 220a       movs    r2, #10                 ; type = 10  (r2, not r6)
97272: 2803       cmp     r0, #3
97274: d82e       bhi.n   972d4                  ; continue
97276: f1ba 0f00  cmp.w   sl, #0                 ; used != nullptr
9727a: d019       beq.n   972b0
9727c: f8da 0004  ldr.w   r0, [sl, #4]           ; used->data
97280: 5d81       ldrb    r1, [r0, r6]
97282: b199       cbz     r1, 972ac
97284: f8da 1000  ldr.w   r1, [sl]               ; used->size
97288: 2200       movs    r2, #0                 ; sum = 0
9728a: 2300       movs    r3, #0                 ; i = 0
9728c: e002       b.n     97294
9728e: 5c84       ldrb    r4, [r0, r2]
97290: 3201       adds    r2, #1
97292: 4423       add     r3, r4
97294: 4291       cmp     r1, r2
97296: d1fa       bne.n   9728e
97298: 2b0e       cmp     r3, #14
9729a: 4632       mov     r2, r6                 ; type = pick
9729c: d11a       bne.n   972d4                  ; continue
9729e: e002       b.n     972a6
972a0: f800 5b01  strb.w  r5, [r0], #1
972a4: 3901       subs    r1, #1
972a6: 2900       cmp     r1, #0
972a8: d1fa       bne.n   972a0
972aa: e012       b.n     972d2                  ; assignType
972ac: 2101       movs    r1, #1
972ae: 5581       strb    r1, [r0, r6]           ; used[pick] = true
972b0: 2e0c       cmp     r6, #12
972b2: d10c       bne.n   972ce
972b4: 9808       ldr     r0, [sp, #32]           ; &Globals::status
972b6: 6800       ldr     r0, [r0, #0]
972b8: f7ca ebaa  blx     Status::getSystem
972bc: f7ca ebde  blx     SolarSystem::getIndex
972c0: 220c       movs    r2, #12                 ; type = 12
972c2: 2819       cmp     r0, #25
972c4: f04f 0b01  mov.w   fp, #1                 ; selected = true
972c8: bf18       it      ne
972ca: 4632       movne   r2, r6                 ; type = pick
972cc: e002       b.n     972d4
972ce: f04f 0b01  mov.w   fp, #1                 ; selected = true
972d2: 4632       mov     r2, r6                 ; assignType: type = pick
972d4: ea5f 70cb  movs.w  r0, fp, lsl #31
972d8: d0b6       beq.n   97248
```

`type` lives in **r2** and `pick` in **r6** for the whole loop. `&Globals::status` is in r6
before the loop (`strd r6,r0,[sp,#32]` spills it together with `race`).

## 2. The faithful spelling: `f_p12exp`

`f_p12exp` = HEAD with

* `for (; !selected; --guard)` → `while (!selected)` and `--guard` after `nextInt`,
* `short pick` → `int pick`,
* the loop-top `type = pick` removed (it is dead; clang deletes it either way),
* `if (pick == 12)` → `if (__builtin_expect(pick == 12, 0))`,
* `0.5000001f` → `0.5f`.

Result (official verifier): **pct 71.1, osize 1996, usize 1992, frame 104, 724 insns**.

The loop from `blx nextInt` through `cmp r6,#12` is the original's instruction sequence.
The only `|` lines in that span are stack offsets and the `beq.n 0x11a` / `beq.n 0x100`
branch target:

```
  str  r0, [sp, #28]        | str  r0, [sp, #32]      ; targetStation
  str  r5, [sp, #28]        | str  r5, [sp, #32]
  str.w r9, [sp, #24]       | str.w r9, [sp, #36]     ; agent
  strd r6, r0, [sp, #32]    | strd r6, r0, [sp, #40]  ; &status, race
  ldr  r0, [pc, #716]       | ldr  r0, [pc, #724]
  str  r0, [sp, #40]        | str  r0, [sp, #48]      ; &rnd
  ldr  r0, [sp, #40]        | ldr  r0, [sp, #48]
  beq.n 0x11a               | beq.n 0x100
  ldr  r0, [sp, #36]        | ldr  r0, [sp, #44]      ; race
  b.n  0x11a                | b.n  0x100
```

The `pick == 12` block is the same 36 bytes as the original, but the block order is inverted:
the original falls through into the cold `getSystem/getIndex` block and branches to
`mov fp,#1; mov r2,r6`; `f_p12exp` branches to the cold block and keeps
`mov fp,#1; mov r2,r6; b latch` inline.

## 3. The frame-96 variants all lose the tail

The frame difference is one stack slot. The original uses **7** slots:

```
16=galaxy/bonus  20=&__stack_chk_guard  24=agent/difficulty
28=targetStation/amount  32=&Globals::status  36=race  40=&Globals::rnd
```

`f_p12exp` uses **8**:

```
20=amount  24=fromSystemPtr/difficulty  28=&__stack_chk_guard
32=targetStation  36=agent  40=&Globals::status  44=race  48=&Globals::rnd
```

The extra slot is `fromSystemPtr`, which `f_p12exp` spills to `[sp,#24]` because the
`Galaxy *galaxy` local keeps `galaxy->visited` in r6 across the `toSystem` computation.
The original instead spills the `galaxy` temp to the low slot 16 and keeps `fromSystemPtr`
in r6.

Restoring frame 96 requires changing the tail, and every form measured loses more than the
loop gains:

| tail form | pct | usize | frame | note |
|---|---:|---:|---:|---|
| `galaxy` before `fromSystem`, used in `toSystem` and `distance` (`f_p12exp`) | 71.1 | 1992 | 104 | faithful loop, 8 slots |
| same + `0.5000001f` anchor | 70.9 | 1996 | 104 | best 1996 B `int pick` build |
| `galaxy` declared after `toSystem`, `toSystem` uses `Globals::galaxy` (`ff_p12exp`) | 68.9 | 1992 | 96 | 7 slots, tail re-allocated |
| no local at all, two `Globals::galaxy` loads (`nogal_p12exp`) | 68.9 | 1992 | 96 | `&Globals::status` lands in r9, not r6 |
| same + anchor | 68.7 | 1996 | 96 | |
| `galaxy` after `fromSystemPtr`, used in `toSystem`/`distance` (`v_g_last2`) | 70.1 | 1984 | 104 | 12 B short |
| `galaxy` local for `distance`, `Globals::galaxy` for `toSystem` (`v_toglob`) | 64.3 | 2008 | 104 | 12 B long |
| both `fromSystem`/`fromSystemPtr` inlined into `distance` (`inline_both`) | 68.4 | 1996 | 104 | |

All 720 permutations of the six declarations `{race, level, selected, type, guard, used}` on
`nogal_p12exp` score exactly **68.9 / 1992** — the slot assignment is not declaration-order
sensitive, so the w4706 "declaration order" avenue is closed.

## 4. `used->data()[type]` (`e1`) separates the registers but not the loop

`e1` (HEAD with `if (used->data()[type])`) is the only `short pick` spelling that keeps
`type` in r2 and `pick` in r5. It does produce the original's `movs r2,#10`, but it also
adds a loop-top `mov r2,r5` and moves `guard` from r5 to r6:

```
e1: pct 67.4  osize 1996  usize 1996  frame 96
    e1 + 0.5f: pct 67.5  osize 1996  usize 1992  frame 96
```

Removing the loop-top `type = pick` from `e1` makes it 2044 B (55.5 %). `e1` + `int pick`
is 49.0 / 1964 (w4706). So the separation is real but not usable.

## 5. Constant-pool placement

The original's trailing literal pool starts at **`0x9795c`** and its first word is `95.0`
(`0x42be0000`). Ours places `95.0` inline in the landing-pad region (`0x97928`/`0x9792a`)
and starts the trailing pool at `0x9795c` with `1200.0` (`0x44960000`). The extra
`0.5000001f` word (`0x3f000002`) sits at `0x97974` in the HEAD build. This is a downstream
consequence of the different `type == 0` block shape, not a separate root cause.

## 6. The `getRoutes() == nullptr` cold block

The original keeps the `type = 4; if (nextInt(2) == 0) type = 1;` block **out of line** at
`0x97586` (`973a8: beq.w 97586`). Ours inlines it (`973a8: cbz r0, 973f8`). Adding
`__builtin_expect(getRoutes() == nullptr, 0)` moves it out of line but re-allocates the whole
function: with `0.5f` it is **56.9 / 1968**, and `__builtin_expect(..., 1)` is 43.9 / 2020.
The block placement is a consequence of the different CFG, not source-controllable.

## 7. What remains

The whole gap is still the single `type`/`pick` coalescing question, but the w4706 hypothesis
("`0.5f` becomes usable once the loop grows by 4 B") is now measured to be insufficient: the
faithful `int pick` loop is reachable, but it always costs the tail one stack slot (frame 104
instead of 96) unless the `galaxy` local is removed, and removing the local re-allocates the
post-loop region and costs more than the loop gains.

Concretely, the next pass should look for a source shape in which:

* `type` is a 32-bit value that is **not** coalesced with `pick` (so `movs r2,#10`,
  `movs r2,#12` and the latch `mov r2,r6` survive), and
* `fromSystemPtr` stays in a register (so the tail uses 7 slots and `&Globals::status`
  reloads into r6, not r9), and
* the `pick == 12` cold block is placed inline with the hot path after it.

Every form measured so far gets at most two of the three.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4808
cmake --build cmake-build-match --target check -j8     # RATCHET PASS, byte 4500
python3 /dev/shm/w4808/score.py                        # pct=74.3 osize=1996 usize=1996
```

Scratch harness (not committed, `/dev/shm/w4808`): `run.sh <body.cpp>` (splice a whole
`createMission` body into the pristine `Generator.cpp`, `link_so`, score), `score.py`
(official single-symbol score, `--show` for the side-by-side diff), `feat.py` (score + frame
size), `sweep_nogal.py` (the 720-permutation declaration sweep), `Generator.head.cpp`
(pristine file), `head_func.cpp` (landed body), `f_p12exp.cpp` / `ff_p12exp.cpp` /
`nogal_p12exp.cpp` / `e1.cpp` / `k1.cpp` (the candidate bodies above).
