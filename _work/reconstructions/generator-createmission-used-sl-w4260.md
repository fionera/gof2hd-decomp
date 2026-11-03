# Generator::createMission — `used -> sl` and the frame-104 boundary (w4260)

Status: **research note, no source change landed** (wave 92, 2026-11-03). The landed
`short pick` / `goto assignType` shape from w4232 remains the best measured form:
**74.3 %** (`osize 1996  usize 1996`, 727 instructions). No source spelling found in this
pass beats it; every candidate either keeps the short-pick loop and spills `used`, or fixes
`used -> sl` and loses `systems -> r8` / frame 96.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

Gate at HEAD (unchanged by this note):

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

(`byte_exact` is 4499 against the checked-in 4498 baseline; the extra exact function is
already present at HEAD and is not caused by this pass.)

## 1. The landed shape and its measured score

The HEAD loop is the w4232 form: `short pick`, `for (; !selected; --guard)`,
`if (pick == 8) goto assignType`, `__builtin_expect(pick == 10, 1)` with
`type = 10; if (race > 3) continue;`, `goto assignType` on the sum-14 reset, a
`type = pick; continue;` on the used-data path, and a shared `assignType: type = pick;`
latch. Official verifier (autojunk=False) on HEAD:

```
osize=1996 usize=1996 pct=74.3 byte=False linked=False
```

The loop is at `0x9724e..0x972da` (ours) vs `0x97248..0x972da` (original): the body is
structurally the original but starts 6 bytes late and carries four `short`-encoding
artefacts.

## 2. Exact remaining differences (HEAD)

### 2.1 Prologue / loop setup

```
original                                   ours
971f6: str r4,[sp,#20]   ; canary addr     971f6: str r4,[sp,#16]
97218: str r0,[sp,#28]   ; targetStation   97218: str r0,[sp,#20]
97220: str.w r9,[sp,#24] ; agent           97220: str.w r9,[sp,#24]
97228: strd r6,r0,[sp,#32] ; status,race   97228: str r0,[sp,#36]; 97234: str r6,[sp,#28]
9722c: movs r5,#0        ; zero            9722a: mov.w sl,#0        ; zero
97230: movw r9,#999      ; guard           97230: movw r5,#999      ; guard
97234: movs r2,#0        ; type            97236: mov.w r9,#524288   ; 8<<16 constant
9723a: ldr.w sl,[r0,#80] ; used            9723a: movs r6,#0        ; type
                                             97240: ldr r0,[r0,#80]; str r0,[sp,#32] ; used
```

The slot map is therefore:

```
original: 16=galaxy/bonus 20=canary 24=agent/difficulty 28=targetStation/amount
          32=status 36=race/type 40=rnd
ours:     16=canary 20=targetStation/amount 24=agent 28=status/bonus
          32=used 36=race/type 40=rnd
```

### 2.2 Loop body

```
original                                   ours
97260: sub.w r9,r9,#1                      97264: sxth r6,r0
97264: mov r6,r0                           97266: cmp.w r9,r0,lsl#16
97266: cmp r0,#8                           9726a: beq loc_11a
9726a: cmp r6,#10                          9726c: lsls r1,r0,#16
97270: movs r2,#10                         9726e: cmp.w r1,#655360
97274: bhi loc_11c                         97272: bne loc_c6
97276: cmp.w sl,#0                         97278: bls loc_c6
9727a: beq loc_f8                          9727a: movs r6,#10
9727c: ldr.w r0,[sl,#4]                    9727c: b loc_11a
97280: ldrb r1,[r0,r6]                     9727e: ldr r3,[sp,#32]
97282: cbz r1,loc_f4                       97280: cbz r3,loc_f8
97284: ldr.w r1,[sl]                       97282: ldr r0,[r3,#4]
9729a: mov r2,r6                           97284: ldrb r2,[r0,r6]
972b0: cmp r6,#12                          97286: cbz r2,loc_f4
972c0: movs r2,#12                         97288: ldr r1,[r3,#0]
972c8: it ne                              972b0: cmp.w r1,#786432
972ca: movne r2,r6                         972c8: it eq
972d2: mov r2,r6                           972ca: moveq r6,#12
972d4: movs.w r0,fp,lsl#31                 972d2: subs r5,#1
                                           972d4: movs.w r0,fp,lsl#31
```

The `pick == 12` select is `it eq / moveq r6,#12` because `type` is coalesced with
`pick` (`r6`); the original keeps `type` in `r2` and emits
`movs r2,#12; it ne; movne r2,r6`. The shared latch is likewise absent in ours (the
`type = pick` is a no-op after coalescing), and the original's `mov r2,r6` at `972d2`
has no counterpart.

### 2.3 Tail

* `used` is `[sp,#32]` and the null test is `cbz r3`; the original has `sl` and
  `cmp.w sl,#0 / beq`.
* `difficulty` lands in `r5` (ours) not `r6` (original), so the clamp is
  `cmp r5,#10; it ge; movge r5,#10; ldr r0,[sp,#28]; mov r9,r5` instead of
  `cmp r6,#10; it ge; movge r6,#10; ldr r0,[sp,#32]; str r6,[sp,#24]`.
* The `type == 8` block sinks the `amount` store to the join:
  ours `mov r5,r0; adds r0,r6,#5; b loc_442` then `str r0,[sp,#20]` at the join;
  original `mov r1,r0; adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1; b 9760a`.
* The `getRoutes() == nullptr` block is inline and `cbz`; the original places it
  out-of-line at `97586` and tests `cmp r0,#0; beq.w 97586`.

## 3. Root-cause chain (measured)

The four loop artefacts and the `used` spill are one chain:

1. `short pick` makes clang canonicalise `(short)x == 8` and `(short)x == 10` into a
   `switch i32 (x << 16)` (confirmed in the IR: `%45 = shl i32 %44, 16`, then
   `switch i32 %45` with cases `524288` and `655360`). The switch lowering keeps
   `0x80000` in a callee-saved register (`r9`).
2. `r9` is therefore unavailable for `guard`; the allocator puts `guard` in `r5` and the
   `itemId = 0` / `amount = 0` zero in `sl`.
3. `sl` is therefore unavailable for `used`; `used` is spilled to `[sp,#32]`.
4. `used` in `[sp,#32]` plus the canary at `[sp,#16]` produce the shifted slot map and
   the `cbz` null test.

With `int pick` the switch disappears (the IR compares `%44` directly), `used` does go
to `sl`, and the loop body becomes the original's instruction-for-instruction. But the
allocator then permutes the three long-lived callee-saved values:

```
original: canary r4, systems r8, agent r9, guard r9 (agent spilled)
int pick: canary r9, systems r4, agent r8, guard r8 (agent spilled)
```

and the tail grows to frame 104 because `agent` and `difficulty` no longer share
`[sp,#24]` (agent is reloaded into `r5`/`r8` instead of `r9`, so its slot stays live and
`difficulty` needs a fresh slot). The int-pick family is therefore a loop win and a tail
loss.

## 4. Experiments and official scores

All scores are the official verifier (`score.py` = `verify.py` scoring path,
`autojunk=False`) after a full `link_so`.

### 4.1 `used -> sl` without `int pick`

| variant | pct | size | features |
|---|---:|---:|---|
| HEAD (`short pick`) | **74.3** | 1996 | sys_r8, frame96, used[sp,#32] |
| `unsigned guard` | 61.6 | 2000 | used_sl, frame96, **systems -> fp** |
| `unsigned short guard` | 61.6 | 2000 | used_sl, frame96, systems -> fp |
| `char guard` | 61.5 | 2000 | used_sl, frame96, systems -> fp |
| `int pick` (same loop) | 48.0 | 2028 | used_sl, frame104, sys off r8 |
| `int pick` + `galaxy_after_from` | 48.3 | 2020 | used_sl, frame104 |
| `int pick` + `nolocal` | 53.2 | 2028 | used_sl, frame96, sys off r8 |
| cand1 (int, expect, gotos, w4232) | 71.1 | 1996 | sys_r8, used_sl, frame104 |
| cand1 + `galaxy_after_from` | 70.1 | 1988 | sys_r8, used_sl, frame104 |
| cand1 + `nolocal` | 67.4 | 1980 | sys_r8, used_sl, frame96 |

cand1 is the closest: its loop matches 516/727 instructions (the original loop body is
essentially exact) but its tail is frame 104 and loses ~15 points. No tail reordering
tried (`galaxy` before/after clamp/after fromSystem, `nolocal`, `fromSystemPtr`/`toSystem`
order, `itemId`/`amount` zero vs uninitialised, `bonus`/`difficulty`/`itemId` declarations
moved to the top) produced frame 96 with `sys_r8` and `used_sl` together.

### 4.2 `type == 8` store sinking

Every faithful spelling still sinks the `amount` store to the join and keeps
`difficulty` in `r5`:

| form | pct |
|---|---:|
| `amount = nextInt(15) + 5; difficulty = getTecLevel();` (HEAD) | 74.3 |
| `int raw = nextInt(15); difficulty = getTecLevel(); amount = raw + 5;` | 74.3 |
| `int amt = nextInt(15) + 5; difficulty = getTecLevel(); amount = amt;` | 74.3 |
| `amount = nextInt(15); difficulty = getTecLevel(); amount += 5;` | 74.3 |
| `difficulty = getTecLevel(); amount = nextInt(15) + 5;` | 71.8 |
| `int tec = getTecLevel(); amount = nextInt(15) + 5; difficulty = tec;` | 71.8 |
| `volatile int amount` | 64.8 (frame104) |

The w4104 `typeassign='none' + nested_after` spelling does keep the store in-block
(`mov r1,r0; adds r0,r6,#5; str r0,[sp,#N]; mov r6,r1`) but changes the loop to
`pick == 10`-first and scores 56.2.

### 4.3 Other sweeps (all <= 74.3)

* Single `__builtin_expect` sweep over 23 conditions (p8, p10, race, used, sum, p12,
  index, guard, for-condition, type==8, type==12, offer): best 74.3; the strongest
  regressions are `p8_0` 55.8, `p12_0` 73.3, `type8_1` 42.0.
* `pick == 12` ternary form: 73.6 (the w4232 pre-goto shape).
* `pick == 8` / `pick == 10` / `pick == 12` reversed, cast, xor, bit-and, bool, switch,
  `int p = pick` aliases: all 74.3 or worse; the switch form is 49.2.
* Micro-variants (`if (used)`, `14 == sum`, `= 0`/`'\\0'` reset, `!=` loop, `(int)` casts,
  `int16_t pick`, `unsigned guard`): all 74.3 except `unsigned guard` 61.6.
* 400-variant random search over `{pick type} x {for/while/for_dec} x {p8 goto/cont/setcont}
  x {p10 nested/nested_after/plain/and/and_set} x {sum14 goto/cont} x {usedtrue
  setcont/cont} x {p12 goto/tern/expect0} x {else sel/none} x {latch assign/none} x
  {itemId/amount zero/uninit} x {galaxy before/after}`: best 74.3 (the landed shape).
* 1440 declaration permutations and 324 type/declaration variants from w4104: no
  `sys_r8 + used_sl + frame96` hit for the pick-first int shapes.

## 5. What remains

The original is almost certainly `int pick` (the loop compares `r0`/`r6` directly and the
`pick == 12` select uses a non-folded `r2`). To reach it we need the int-pick allocator to
choose `canary -> r4, systems -> r8, agent -> r9 (spilled), guard -> r9` and to keep
`agent` in `r9` after the loop so `difficulty` can reuse `[sp,#24]`. No source spelling
tried so far flips that permutation. The `short pick` family is a stable local optimum at
74.3 because its `0x80000` switch constant is exactly what keeps `systems -> r8` and the
frame at 96.

The next plausible lever is the `type == 8` store: if the backend can be made to store
`amount` in-block, `difficulty` moves to `r6`, which should force `galaxy` to spill to
`[sp,#16]` and shift the whole slot map to the original's. Every faithful statement order
tried still sinks the store; the only shape that does not (`none + nested_after`) also
reorders the loop.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4260
cmake --build cmake-build-match --target check -j8          # RATCHET PASS, byte 4499
python3 /dev/shm/w4260/score.py cmake-build-match/libgof2hdaa.so
# createMission row: osize 1996  usize 1996  pct 74.3
```

Scratch harness (not committed, `/dev/shm/w4260`): `head.cpp` (HEAD source),
`evalsrc.sh` (one-TU rebuild + official score), `score.py` (official per-function score),
`fast.py`/`loopdump.py` (one-TU feature scan), `evalvar.py`/`evalmatrix.py`/
`evalexpect.py`/`evalt8*.py`/`evalint*.py`/`randsearch.py` (the sweeps tabulated above),
and the w4232 evidence reused from `/dev/shm/w4232` (`cand1.cpp`, `sweep_*.out`).
