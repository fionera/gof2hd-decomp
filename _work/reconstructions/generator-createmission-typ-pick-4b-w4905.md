# Generator::createMission — the `assignType` label position controls the loop layout;
# the last 4 B are the status/targetStation stack-slot swap (w4905)

Status: **research note, no source change landed** (wave 99, 2026-11-04, worktree `w4905`,
branch `wave99-w4905`, base `44225192c`). The landed `short pick` / `goto assignType` shape
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

This pass closes the remaining gap to one concrete question and adds three measured facts:

1. **The `assignType` label position controls the loop's machine block layout.**  Moving the
   label *before* `selected = true` (`assignType: type = pick; selected = true;`) makes clang 7
   place the loop condition at the **bottom** (`movs.w r0,fp,lsl#31; beq <body>`) exactly like
   the original, and emits the original's `movs r2,#10` / `mov r2,r6` / `movs r2,#12` /
   `movne r2,r6` — i.e. `type` stays in `r2` and `pick` in `r6`.  The variant is **67.7 % /
   1992 B** (it is 4 B short and the `pick == 8` path wrongly sets `selected`).
2. **`int pick` + `--guard` in the body + no loop-top `type = pick` (`k1b`) reproduces the
   original's *entire* register allocation and frame 96** — `systems->r8`, `agent->r9`
   (spilled to `[sp,24]`), `guard->r9`, `used->sl`, `zero->r5`, `type->r2`, `pick->r6`,
   `selected->fp`, `rnd->[sp,40]`.  The **only** residual is that `status` and `targetStation`
   are swapped: ours `status->[sp,28]`, `targetStation->[sp,32]`; original
   `targetStation->[sp,28]`, `status->[sp,32]`.
3. **The swap is the missing `strd r6,r0,[sp,#32]`.**  In the original, `status` and `race` are
   spilled together right after `agent->getRace()` (`97228: strd r6,r0,[sp,#32]`), which forces
   `status` into the 8-byte-aligned slot `[sp,32]` and leaves `[sp,28]` for `targetStation`.
   In every variant we can build, the scheduler spills `race` at `97228` and `status` later at
   `97232` (`str r6,[sp,#28]`), so no `strd` forms.  Fixing this one slot would make the whole
   tail match: `ldr r6,[sp,#32]` (status), `ldrd r9,r4,[sp,#24]` (agent,targetStation),
   `ldr fp,[sp,#36]` (race).

## 1. The 4 B are the loop, and `0.5f` is proven

The original's `type == 8` costs block uses `eeb6 1a00 vmov.f32 s2, #96` (`0.5f` immediate,
no pool word) at `977ce`; HEAD uses `vldr s2,[pc,#424]` against the `0x3f000002`
(`0.5000001f`) anchor at the end of the pool.  Replacing the anchor with `0.5f`:

```
HEAD + 0.5f : pct=74.5  osize=1996  usize=1992  n_ours=726
```

74.5 > 74.3, but 1992 B is unusable: `createMission` is not the last function in `.text`, so a
4-byte shrink shifts every following function and `byte_exact` collapses (measured
**4500 -> 2615**, `RATCHET FAIL`).  The 4 bytes must be added back as real code.

Accounting (both functions are 727 instructions / 1996 B at HEAD):

| region | original | ours (HEAD) |
|---|---:|---:|
| loop `97248..972d8` / `9724e..972d8` | 144 B | 138 B |
| trailing constant pool | 10 words | 11 words (`0.5000001f`) |
| net (code + pool) | 0 | 0 |

`0.5f` deletes the 11th pool word, so the loop must grow by the same 4 bytes: the two
`mov r2,r6` copies at `9729a` (used branch) and `972d2` (shared latch), plus the
`movs r2,#10`/`movs r2,#12` that coalescing folds into `r6`.  That requires `type` to be a
different virtual register from `pick`.

## 2. The loop diff, instruction by instruction

Original `97248..972d8` (59 instructions):

```
97248 mov.w fp,#0            ; selected = 0
9724c cmp.w r9,#1            ; guard < 1
97250 it lt
97252 movlt.w fp,#1
97256 ldr r0,[sp,#40]        ; rnd
97258 movs r1,#15
9725a ldr r0,[r0,#0]
9725c blx nextInt
97260 sub.w r9,r9,#1          ; --guard  (in the body, after nextInt)
97264 mov r6,r0               ; pick     (int: no sxth)
97266 cmp r0,#8               ; compare on r0
97268 beq.n 972d2             ; -> shared latch
9726a cmp r6,#10
9726c bne.n 97276
9726e ldr r0,[sp,#36]         ; race
97270 movs r2,#10             ; type = 10   <<< separate register
97272 cmp r0,#3
97274 bhi.n 972d4             ; continue
97276 cmp.w sl,#0             ; used != nullptr
9727a beq.n 972b0
9727c ldr.w r0,[sl,#4]        ; used->data()
97280 ldrb r1,[r0,r6]         ; used[pick]
97282 cbz r1,972ac
97284 ldr.w r1,[sl]           ; used->size()
97288 movs r2,#0
9728a movs r3,#0
9728c b.n 97294
9728e ldrb r4,[r0,r2]
97290 adds r2,#1
97292 add r3,r4
97294 cmp r1,r2
97296 bne.n 9728e
97298 cmp r3,#14
9729a mov r2,r6               ; type = pick  <<< before the branch
9729c bne.n 972d4             ; continue
9729e b.n 972a6
972a0 strb.w r5,[r0],#1       ; clear used[]
972a4 subs r1,#1
972a6 cmp r1,#0
972a8 bne.n 972a0
972aa b.n 972d2               ; -> shared latch
972ac movs r1,#1
972ae strb r1,[r0,r6]         ; used[pick] = true
972b0 cmp r6,#12
972b2 bne.n 972ce
972b4 ldr r0,[sp,#32]         ; status
972b6 ldr r0,[r0,#0]
972b8 blx getSystem
972bc blx getIndex
972c0 movs r2,#12             ; type = 12
972c2 cmp r0,#25
972c4 mov.w fp,#1             ; selected = true
972c8 it ne
972ca movne r2,r6             ; type = pick
972cc b.n 972d4
972ce mov.w fp,#1             ; selected = true (default)
972d2 mov r2,r6               ; assignType: type = pick   <<< shared latch
972d4 movs.w r0,fp,lsl#31     ; while (!selected)
972d8 beq.n 97248
```

Ours (HEAD), same span (57 instructions):

```
9724e mov.w fp,#0
97252 cmp r5,#1               ; guard in r5, not r9
97254 it lt
97256 movlt.w fp,#1
9725a ldr r0,[sp,#40]
9725c movs r1,#15
9725e ldr r0,[r0,#0]
97260 blx nextInt
97264 sxth r6,r0              ; pick is short -> sxth
97266 cmp.w r9,r0,lsl#16      ; short compare canonicalisation
9726a beq.n 972d2
9726c lsls r1,r0,#16
9726e cmp.w r1,#655360        ; pick == 10
97272 bne.n 9727e
97274 ldr r0,[sp,#36]
97276 cmp r0,#3
97278 bls.n 9727e
9727a movs r6,#10             ; type = 10   <<< folded into pick
9727c b.n 972d2
9727e ldr r3,[sp,#32]         ; used (spilled)
97280 cbz r3,972b0
97282 ldr r0,[r3,#4]
97284 ldrb r2,[r0,r6]
97286 cbz r2,972ac
97288 ldr r1,[r3,#0]
9728a movs r2,#0
9728c movs r3,#0
9728e b.n 97296
97290 ldrb r4,[r0,r2]
97292 adds r2,#1
97294 add r3,r4
97296 cmp r1,r2
97298 bne.n 97290
9729a cmp r3,#14
9729c beq.n 972a6              ; no type = pick here
9729e b.n 972d2
972a0 strb.w sl,[r0],#1       ; zero in sl, not r5
972a4 subs r1,#1
972a6 cmp r1,#0
972a8 bne.n 972a0
972aa b.n 972d2
972ac movs r2,#1
972ae strb r2,[r0,r6]
972b0 cmp.w r1,#786432        ; pick == 12 (short compare)
972b4 bne.n 972ce
972b6 ldr r0,[sp,#28]         ; status
972b8 ldr r0,[r0,#0]
972ba blx getSystem
972be blx getIndex
972c2 cmp r0,#25
972c4 mov.w fp,#1
972c8 it eq
972ca moveq r6,#12            ; type = 12 folded into pick
972cc b.n 972d2
972ce mov.w fp,#1
972d2 subs r5,#1              ; --guard at the latch (for-increment)
972d4 movs.w r0,fp,lsl#31
972d8 beq.n 9724e
```

Differences: `sxth` vs `mov`, the short-compare canonicalisation (2 extra instructions),
the missing `movs r2,#10`, the missing `movs r2,#12`, the missing `mov r2,r6` at `9729a`,
the missing shared-latch `mov r2,r6`, `--guard` at the latch instead of after `nextInt`, and
the whole register/slot cascade in §3.

## 3. Why `type` and `pick` are coalesced — and why that cascades

In the original `type` lives in `r2` and `pick` in `r6` for the whole loop.  In every
`short pick` build measured here, clang 7.0.2's greedy coalescer merges them into `r6`
because the loop-top `type = pick` makes their live ranges identical.

The cascade (all of it follows from `type == pick` in `r6`):

* `movs r2,#10` / `movs r2,#12` disappear (folded into `movs r6,#10` / `moveq r6,#12`).
* `mov r2,r6` at the shared latch disappears.
* `type = 0` is emitted as `movs r6,#0`, which **evicts `&Globals::status` from `r6`**
  (`str r6,[sp,#28]` at `97234`) — the original keeps `&Globals::status` in `r6` from
  `971e8` to the epilogue.
* Because `r6` is free, `guard` lands in `r5` (not `r9`), the `zero` value lands in `sl`
  (not `r5`), and `used` is spilled to `[sp,32]` (instead of `sl`).
* Because `status` is a stack slot in ours, the SSC weight order becomes
  `rnd > race > used > status > type > agent > targetStation` instead of the original
  `rnd > race > status > targetStation > agent > &__stack_chk_guard > galaxy`, producing
  `16=&guard 20=targetStation 24=agent 28=status 32=used 36=race 40=rnd` instead of the
  original `16=galaxy 20=&guard 24=agent 28=targetStation 32=&status 36=race 40=rnd`.
* After the loop, `mov r4,r6` (type from `r6`) must precede `ldr r6,[sp,#28]` (status), so
  the two instructions are emitted in the opposite order to the original
  (`ldr r6,[sp,#32]; mov r4,r2`).

## 4. `k1b`: the original's allocation, except the status/targetStation slot swap

`k1b` = `int pick`, `while (!selected)`, `--guard` after `nextInt`, **no** loop-top
`type = pick`, `assignType` label at the end (`selected = true; assignType: type = pick;`):

```
k1b: pct=67.0  osize=1996  usize=1996  n_ours=726
     systems->r8  agent->r9 (spilled [sp,24])  guard->r9  used->sl  zero->r5
     type->r2  pick->r6  selected->fp  frame 96
     slots: 20=&guard 24=agent 28=status 32=targetStation 36=race 40=rnd
```

Its loop is the original's modulo block placement (`bot=0`: the condition is at the top, so
the `pick == 8` path gets its own `mov r2,r6; b check` instead of sharing the latch).  Its
**entry and register allocation are the original's**; only the two slots are swapped.

The original's tail:

```
972da ldr r6,[sp,#32]        ; status
972dc mov r4,r2              ; type
972de ldr r0,[r6,#0]
...
97306 ldrd r9,r4,[sp,#24]    ; agent, targetStation
9730a movs r2,#11
9730c ldr.w fp,[sp,#36]      ; race
```

`k1b`'s tail:

```
972dc ldr.w r9,[sp,#28]      ; status
972e0 mov r4,r2
972e2 ldr.w r0,[r9]
...
9730c ldr.w fp,[sp,#24]      ; agent
97310 movs r2,#11
97312 ldrd r4,r6,[sp,#32]     ; targetStation, race
```

So if `status` were at `[sp,32]` and `targetStation` at `[sp,28]`, the tail would be
`ldr r6,[sp,#32]; mov r4,r2; ...; ldrd r9,r4,[sp,#24]; movs r2,#11; ldr fp,[sp,#36]` —
instruction-for-instruction the original.  The score would jump far above 74.3.

## 5. The `strd` is the cause of the slot swap

Original entry:

```
971f6 str r4,[sp,#20]        ; &__stack_chk_guard
97218 str r0,[sp,#28]        ; targetStation
97220 str.w r9,[sp,#24]      ; agent
97228 strd r6,r0,[sp,#32]    ; status, race   <<< paired, 8-byte aligned
97244 str r0,[sp,#40]        ; rnd
```

`k1b` entry:

```
971f6 str r4,[sp,#20]        ; &__stack_chk_guard
97218 str r0,[sp,#32]        ; targetStation
97220 str.w r9,[sp,#24]      ; agent
97228 str r0,[sp,#36]        ; race
97232 str r6,[sp,#28]        ; status         <<< separate, after the used load
97244 str r0,[sp,#40]        ; rnd
```

The original's scheduler spills `status` at `97228`, immediately after `getRace` and before the
`ldr r0,[r6,#0]` used to fetch `Globals::status->field_50`; the two stores merge into `strd`
and the 8-byte alignment forces `status` to `[sp,32]`.  In every variant we can build, the
scheduler keeps `status` in `r6` across the `field_50` load and spills it later at `97232`, so
no `strd` forms and `[sp,28]` is taken by `status`.

Tried without effect (the entry/tail code is byte-identical in all of them):

* ~150 declaration/statement permutations of `race/level/selected/type/guard/used`
  (including `used` first, `used` last, `type` first, `type` last, `race` after `used`);
* `Galaxy *galaxy = Globals::galaxy;` declared early or in the tail;
* `Status *st = Globals::status;` / `AERandom *rnd0 = Globals::rnd;` locals;
* `int dummy = targetStation/systems->size();` live-across-loop locals;
* `int`/`unsigned`/`uint32_t`/`int32_t`/`long` for `targetStation` and `race`;
* `bool`/`int`/`char`/`short` for `selected`; `int`/`short`/`unsigned`/`int8_t`/`int16_t`/
  `uint16_t`/`long` for `guard`; `int`/`uint32_t`/`char`/`short` for `selected`;
* `k1b` + label-first loop (`assignType: type = pick; selected = true;`), which does give
  `bot=1` but keeps `status->[sp,28]`.

## 6. The `assignType` label position controls the block layout

`make_loop` variants that differ only in the label/`selected` order:

| tail form | pct | usize | `movs r2,#10` | `mov r2,r6` | check at bottom |
|---|---:|---:|:--:|:--:|:--:|
| `selected = true; assignType: type = pick;` (k1b) | 67.0 | 1996 | yes | yes | **no** |
| `assignType: type = pick; selected = true;` (V2) | 67.7 | 1992 | yes | yes | **yes** |
| `selected = true; continue; assignType: ...` | 65.8 | 1992 | no | no | yes |
| `selected = true; goto assignType; assignType: ...` | 67.0 | 1996 | yes | yes | no |
| `selected = true; assignType: ; type = pick;` | 67.0 | 1996 | yes | yes | no |

V2's loop is the original's layout (condition at the bottom, `beq <body>`) but it has an extra
`mov.w fp,#1` in the `pick == 8` path because the label now precedes `selected = true`, i.e.
`pick == 8` wrongly exits the loop.  This is the only source shape found that produces the
original's block layout while keeping `type` in `r2`.

## 6b. Branch hints move the block layout — but not the slot map

Two `__builtin_expect` hints are worth recording:

* `k1b + __builtin_expect(pick == 12, 0)` (**`X_p12exp`**) is **68.7 % / 1996 B** and its loop
  is instruction-for-instruction the original's, including the block layout:
  `972d2 movne r2,r6; 972d4 movs.w r0,fp,lsl#31; 972d8 beq <body>`.  Its tail is still
  `k1b`'s (`ldr r9,[sp,#28]` status, `ldr fp,[sp,#24]` agent, `ldrd r4,r6,[sp,#32]`).
* `k1b + __builtin_expect(used != nullptr, 1)` (**`X_usedexp`**) is the only variant found
  that produces the original's **slot map** and the `strd r6,r0,[sp,#32]` pairing
  (`20=&guard 24=agent 28=targetStation 32=status 36=race 40=rnd`), but it is **1956 B /
  57.4 %** and moves `agent` to `r8`, `systems` out of `r8`, and `status` to `r9`.
  `HEAD + __builtin_expect(used != nullptr, 1)` likewise gives `strd32=1` at 1988 B / 57.9 %.

So the `strd` is reachable from source (the branch hint changes the block probabilities and
the spill schedule), but not yet together with `int pick`'s loop and frame 96.

## 7. The `0.5f` + pool-word trade

Since the 4 bytes are the loop, we also measured whether a *different* 4-byte pool word can
stand in for the removed `0.5000001f`.  Perturbing an immediate constant to force a pool word
keeps the size at 1996 but loses the immediate match:

| variant | pct | usize | note |
|---|---:|---:|---|
| HEAD | **74.3** | **1996** | baseline |
| HEAD + `0.5f` | **74.5** | 1992 | best score; 4 B short |
| HEAD + `0.5f` + `1.0000001f` | 74.0 | 1996 | `vmov.f32 s4,#112` -> `vldr` |
| HEAD + `0.5f` + `5.0000005f` | 74.1 | 1996 | `vmov.f32 s2,#20` -> `vldr` |
| HEAD + `0.5f` + `18.000002f` | 74.0 | 1996 | `vmov.f32 s2,#50` -> `vldr` |
| HEAD + `0.5f` + `2.0000002f` | 72.2 | 1996 | |
| HEAD + `0.5f` + `10.000001f` | 71.1 | 2000 | |
| HEAD + `0.5f` + `1.2000001f` | 74.5 | 1992 | 1.2 already a pool word |
| HEAD + `0.5f` + `0.70000005f` | 74.5 | 1992 | 0.7 already a pool word |
| HEAD + `0.5f` + `0.6000001f` | 74.3 | 1992 | 0.6 already a pool word |
| HEAD + `0.5f` + `1.7000001f` | 74.5 | 1992 | 1.7 already a pool word |
| HEAD + `0.5f` + `95.00001f` | 74.5 | 1992 | 95.0 already a pool word |

No 1996-byte variant beats 74.3.  Adding a non-matching instruction to `HEAD + 0.5f` would
give 2*541/(727+727) = 74.4, but every faithful 4-byte addition we tried either got folded or
re-allocated the tail and lost more than it gained (e.g. `if (itemId < 0) itemId = 0;` is
1996 B but 72.4 %).

## 8. What remains (one concrete experiment)

The whole remaining gap reduces to one register-allocation question:

> make clang spill `status` at `97228` together with `race` (forming `strd r6,r0,[sp,#32]`)
> instead of at `97232` (`str r6,[sp,#28]`), while keeping `int pick` + `--guard` in the body
> (`k1b`'s loop and allocation).

If that `strd` forms, `status` moves to `[sp,32]`, `targetStation` to `[sp,28]`, and the whole
tail (`ldr r6,[sp,#32]; mov r4,r2; ldrd r9,r4,[sp,#24]; ldr fp,[sp,#36]`) matches the original
in `k1b` — which is already 1996 B.  The `assignType` label position (V2) is the only lever
found that changes the loop's block layout; a future pass should try to combine V2's
check-at-bottom loop with the correct `selected` semantics *and* the `strd` entry schedule.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4905
cmake --build cmake-build-match --target check -j8     # RATCHET PASS, byte 4500
python3 /dev/shm/w4905/score.py                        # pct=74.3 osize=1996 usize=1996
```

Scratch harness (not committed, `/dev/shm/w4905`): `try.sh <body.cpp>` (splice a whole
`createMission` body into the pristine `Generator.cpp`, `link_so`, score), `score.py`
(official single-symbol score), `probe2.sh <body.cpp>` (score + loop feature flags
`m10/mr6/bot/strd32`), `genfull.py` (PRE/LOOP/CAMP/GR/POST split + `make_loop`), `genloop.py`,
`genlab.py`, `genv2.py`, `search2.py` (random combinatorial sweep including the label-first
loop form).  Candidate bodies: `head_func.cpp` (landed), `H_half.cpp`, `k1b.cpp`,
`V2_k1_int_0.5000001f_0.cpp`, `ip_while_body.cpp`, `F_one.cpp`, `F_f50.cpp`.

## Appendix A — candidate source bodies (self-contained)

`k1b` (the original's loop + allocation; only the status/targetStation slot swap remains):

```c++
    uint32_t race = agent->getRace();
    Array<bool> *used = Globals::status->field_50;
    bool selected = false;
    int level;
    int guard = 999;
    int32_t type = 0;
    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        if (pick == 8) goto assignType;
        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin
            type = 10;
            if (race > 3) continue;
        }
        if (used != nullptr) {
            if (used->data()[pick]) {
                int sum = 0;
                uint32_t i;
                for (i = 0; i < used->size(); ++i) {
                    sum += used->data()[i];
                }
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        used->data()[i] = false;
                    }
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

`V2` (the original's block layout; the `pick == 8` path wrongly sets `selected`):

```c++
    // identical declarations and loop body, but the tail is
        selected = true;
    assignType:
        type = pick;
        selected = true;
    }
```

`HEAD` (landed, 74.3 % / 1996 B) is the same as `k1b` except `short pick`, the loop-top
`type = pick;`, `for (; !selected; --guard)` with `--guard` in the increment, and the
`0.5000001f` anchor.
