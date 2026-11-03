# Generator::createMission — the p10-expect switch: frame-96 vs `used->sl`, and the last
# agent/targetStation SSC swap (w4508)

Status: **research note, no source change landed** (wave 95, 2026-11-03, worktree `w4508`,
branch `wave95-w4508`, base `4a88bd6e`). The landed `short pick` / `goto assignType` shape
(w4232/w4257) stays at **74.3 %** (`osize 1996  usize 1996`, 727 instructions).

Gate at HEAD (unchanged by this note):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass adds three measured facts that the w4408 SSC note did not have:

1. **The `__builtin_expect(pick == 10, 1)` anchor is the switch between the original's frame
   and `used -> sl`.** Removing it from the landed shape (`head_nop10`) produces the original's
   *entire* prologue slot map and register map except one swap, at **60.0 % / 1988 B**.
2. **The original's frame is reachable with the full-`int` loop too** (`hit1`), at
   **64.8 % / 1972 B** — but only if the `p10` expect is also removed.
3. **The last SSC difference in both is a single swap: `agent` and `targetStation` exchange
   `[sp,#24]`/`[sp,#28]`.** It is a pure weight-ordering statement: the original needs
   `targetStation` (4.00) processed *before* `agent`, which happens iff `agent` is not reloaded
   after the loop (original weight 4.00, tie broken by FI 6 < FI 7). In every frame-96 build
   measured here `agent` is reloaded twice (blocks 107/134), giving it 6.08 and taking the
   lower colour.

## 1. Original slot map and register map (re-derived from the binary)

Original `0x971b8..0x97984`, `sub sp,#96`, canary value at `[sp,#92]`:

```
[sp,#16]  97614 str galaxy   97644 ldr galaxy   976d8/97720 str bonus   9783a ldr bonus
[sp,#20]  971f6 str &__stack_chk_guard                        978b2 ldr (epilogue)
[sp,#24]  97220 str agent    97306/97322 ldrd agent           9752a/9761c str difficulty
[sp,#28]  97218/9721c str targetStation   974a8/974b4/9753e/975ca/97604 str amount
[sp,#32]  97228 str &Globals::status  972b4/972da/97536/9761a/976ac/976dc ldr status
          9770e str rem  97718 ldr rem  9775e str mission  9776c/97824/9793c ldr mission
[sp,#36]  9726e ldr race   9730c/97326 ldr race   97340 str type   97356/9738e/97420/...
[sp,#40]  97244 str &Globals::rnd   then 11 loads
```

Colour order (SSC processing order, highest weight first):

```
rnd > race > status > targetStation > agent > &__stack_chk_guard > galaxy
```

Register map (all callee-saved except `r5`):

```
r4  &__stack_chk_guard -> targetStation (after 97306)
r5  scratch (memset zero, difficulty, itemId raw, ...)
r6  &Globals::status        (clobbered by the itemId block at 9743a, reloaded at 97536)
r7  frame pointer
r8  systems
r9  agent (spilled at 97220) -> guard (97230) -> agent (97306)
sl  used                    (9723a: ldr.w sl,[r0,#80])
fp  selected (loop) -> race (9730c/97326, live to the Mission ctor at 97788)
```

Two structural facts:

* `type` is stored once at `97340` and shares `race`'s slot `[sp,#36]` because `race`'s last
  stack load is `97326`, i.e. **before** the `type` store. `race` survives in `fp`.
* `&Globals::status` is a *rematerialisable* GOT address (`ldr r0,[sp,#32]; ldr r0,[r0]` at
  972b4/972da/...), so the allocator parks it in the scratch-prone `r6` and reloads it often.

## 2. Measured SSC weights — the three families

Method unchanged from w4408: `-mllvm -print-machineinstrs -mllvm -print-machine-bfi`, sum the
MBFI frequency of every non-`DBG_VALUE` `%stack.N` operand in the `After Greedy Register
Allocator` section (`weights2.py`), and read the final offsets from the `After
Prologue/Epilogue Insertion` frame objects (`fimap2.py`; `offset = 92 - (N - N_fi0)` for this
96-byte frame).

### 2.1 Landed HEAD (`short pick`, p10 expect) — 74.3 %, 1996 B

```
rnd 80.23 > race 34.98 > used 17.51 > status 11.43 > type 11.08 > agent 6.08 >
targetStation 4.00 > mission 3.12 > amount 3.06 > &guard 2.69 > ...
```

Frame: `16=&guard 20=targetStation/amount 24=agent/difficulty 28=status/bonus
32=used/mission 36=race/type 40=rnd`. `used` is on the stack, so it takes a colour the
original does not have.

### 2.2 `head_nop10` = HEAD with the p10 expect removed — 60.0 %, 1988 B

Only source change vs the landed body:

```c++
-        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin anchor: ...
+        if (pick == 10) {
```

```
rnd 80.23 > race 24.67 > status 16.15 > type 11.08 > agent 6.08 > targetStation 4.00 >
amount 3.06 > &guard 2.69 > ...
```

Frame (from the disassembly):

```
971f6: str r4,[sp,#20]        ; &__stack_chk_guard          == original
97218: str r0,[sp,#24]        ; targetStation               != original (28)
9721c: str r5,[sp,#24]
97220: str.w r9,[sp,#28]      ; agent                       != original (24)
97228: strd r6,r0,[sp,#32]    ; &status, race               == original (strd!)
97244: str r0,[sp,#40]        ; &rnd                        == original
```

and `[sp,#16]` is `galaxy`/`bonus` (`97620 str r0,[sp,#16]`, `97632 ldr r1,[sp,#16]`,
`976a8 str fp,[sp,#16]`), exactly like the original.

So **five of the seven original slots and the `strd` pairing are reproduced** by deleting the
p10 hint. The two that are not are `agent`/`targetStation` (24/28 swapped). The register map is
also right for `r8 = systems`, `r9 = agent` at entry, `r6 = &Globals::status`, `fp = race`; but
`used` moves to `r9` (original `sl`) and `guard` to `r4` (original `r9`), which is why the
score falls to 60.0 even though the frame is better.

The full diff vs the original is 142 regions; the non-register/slot ones are the known tail
shapes (`type == 8` amount store sunk to the join, `getRoutes()==nullptr` inline `cbz`,
`nextInt(2)==0` inline instead of out-of-line at `97586`, `0.5f` vs the `0.5000001f` anchor).

### 2.3 `hit1` = full-`int` loop + no p10 expect + galaxy local + uninitialised itemId/amount
### — 64.8 %, 1972 B

Source (whole body diff vs the landed body):

```c++
     uint32_t race = agent->getRace();
-    int level;
+    Array<bool> *used = Globals::status->field_50;
     bool selected = false;
-    int32_t type = 0;
+    int level;
     int guard = 999;
-    Array<bool> *used = Globals::status->field_50;
-    for (; !selected; --guard) {
+    int32_t type = 0;
+    while (!selected) {
         selected = guard < 1;
-        short pick = Globals::rnd->nextInt(15);
-        type = pick;
+        int pick = Globals::rnd->nextInt(15);
+        --guard;
         if (pick == 8) goto assignType;
-        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin anchor: ...
+        if (pick == 10) {
             type = 10;
             if (race > 3) continue;
         }
@@
-        if (pick == 12) {
+        if (__builtin_expect(pick == 12, 0)) {  // lint: arm_builtin
             selected = true;
@@
-    int itemId = 0;
-    int amount = 0;
+    int itemId;
+    int amount;
     if (type == 8) {
```

(The declaration order is `used, selected, level, guard, type` after `race`; the `galaxy`
local is the landed one.)

```
rnd 80.23 > race 24.67 > type 11.08 > status 6.72 > agent 6.08 > targetStation 4.00 > ...
```

Frame: `16=galaxy/bonus 20=&guard 24=targetStation/amount 28=agent/difficulty 32=&status/rem
36=race/type 40=rnd` — i.e. the original's frame **plus** the same `agent`/`targetStation`
swap. The loop body is instruction-for-instruction the original's (only the register names
differ: `r4` vs `r9` for guard, `r5` vs `r4` for the sum, `r8` vs `r5` for the memset zero).

## 3. Why the last swap happens — and why it is the same root cause

The SSC assignment rule (LLVM 7 `StackSlotColoring::ColorSlot`) tries the **lowest already-used
colour with non-overlapping intervals** first, then takes a new colour. For both frame-96
builds above:

| order | FI | weight | colour | slot | contents |
|---:|---:|---:|---:|---|---|
| 1 | 8  | 80.23 | 5 | 40 | rnd |
| 2 | 9  | 24.67 | 6 | 36 | race |
| 3 | 11 | 11.08 | 6 (share) | 36 | type |
| 4 | 10 | 16.15 / 6.72 | 7 | 32 | status |
| 5 | 7  | 6.08  | 8 | 28 | agent |
| 6 | 6  | 4.00  | 9 | 24 | targetStation |
| 7 | 5  | 2.69  | 10 | 20 | &guard |
| 8 | 15 | 2.08  | 11 | 16 | galaxy |

`type` shares `race`'s colour because `race`'s **stack** interval ends at `97326`, before the
`type` store at `97340` — this is the original behaviour and both frame-96 builds already have
it. The only remaining difference is step 5: `agent` is processed before `targetStation`
because its measured weight is 6.08 vs 4.00.

The 6.08 comes from **two extra `[sp,#28]`/`[sp,#24]` references** (MBFI blocks 107 and 134 in
`hit1`, 111 and 137 in `head_nop10`) — the `agent->getSystem()` call in the `type == 13` block
and the `agent->getRace()` call in the bonus. In the original `agent` is loaded once
(`97306`/`97322`) and kept in `r9` to `9775c`, so its slot interval has exactly the three
references `97220`/`97306`/`97322` and its weight ties `targetStation`'s 4.00; the tie is then
broken by FI index (targetStation is FI 6, agent FI 7), which is exactly the original order.

So the swap is *not* an independent SSC knob: it is the same `agent -> r9` register-allocation
requirement that the w4260/w4408 notes identified. In every build where `agent` is reloaded,
it outranks `targetStation` and takes `[sp,#28]`.

## 4. The p10-expect trade-off (measured)

`__builtin_expect(pick == 10, 1)` is what makes clang lay the loop out pick-first, but it also
changes the register allocation of the *whole* function. The two outcomes are mutually
exclusive in every variant measured:

| build | loop | p10 expect | frame | `strd r6,r0,[sp,#32]` | `used` | `systems->r8` | pct | size |
|---|---|---|---:|---:|---|---:|---:|---:|
| HEAD (landed) | `short` | yes | 96 | no | `[sp,#32]` | yes | **74.3** | 1996 |
| `head_nop10` | `short` | no | 96 | **yes** | `r9` | yes | 60.0 | 1988 |
| `head_nop10` + uninit itemId/amount | `short` | no | 96 | **yes** | `r9` | yes | 60.0 | 1988 |
| `int5b` | `int` | yes | 96 | no | **`sl`** | yes | 68.7 | 1996 |
| `hit1` | `int` | no | 96 | **yes** | `sl` | no | 64.8 | 1972 |
| `int5c` (`int5b` + galaxy local) | `int` | yes | 104 | no | `sl` | yes | 70.9 | 1996 |
| `h1_p10` (`hit1` + p10 expect) | `int` | yes | 104 | no | `sl` | yes | 70.9 | 1980 |

Reading the table: the p10 hint buys `used -> sl` (via the loop's switch-constant register
pressure) at the cost of the `&status`/`race` `strd` pairing and the `[sp,#16]` galaxy slot;
removing it does the reverse. `hit1` is the only build that has the original's frame *and* the
original's loop encoding, but it loses `systems -> r8` (agent and systems swap `r8`/`r9`).

## 5. Sweeps run this pass (official verifier, autojunk=False)

* **720 declaration permutations** of `{race, level, selected, type, guard, used}` on the
  `int pick` loop (the w4257 §5 body): the first 109 orders measured are all **68.5–68.7**,
  all frame 96, all `used -> sl`, none with `strd r6,r0,[sp,#32]`. (The sweep was stopped
  after 109 orders; no order changed the frame.)
* **24 tail permutations** of `{galaxy, fromSystem, fromSystemPtr, toSystem}` on `hit1`:
  the three orders that keep frame 96 (`gfpt`, `gftp`, `gtfp`) score 64.7–65.0 and all have
  `agent -> r8`; the other 21 give frame 104 (56.9–57.0). No order gives `systems -> r8`.
* **~350 random variants** over `{decl order} x {galaxy local: none/tail/top} x {type=pick at
  loop top} x {p12 expect} x {p10 expect} x {itemId/amount: decl/else/uninit} x {level at use}
  x {int race} x {int selected}`: the best score at size 1996 is **70.9** (frame 104);
  the only `strd r6,r0,[sp,#32]` hits are the `hit1`/`head_nop10` families (frame 96,
  `systems -> r8` only for the `short pick` one).
* **Single-knob probes** on `hit1`: `int race` 68.5, `int type` 68.7, `int selected` 68.5,
  `unsigned guard` 54.2 (1984), `int level = 0` 68.7, `Status *status` local 63.8 (1984),
  `AERandom *rnd` local 52.8 (1976), `level` at use 68.7.

## 6. `0.5f` is worth +0.2 but still 1992 B — and the missing 4 B are not the select

`HEAD + 0.5f` scores **74.5** at **1992 B** (w4408 §6.1). The 4-byte deficit is *not* the
`pick == 12` select: the landed `short pick` select is `cmp r0,#25; it eq; moveq r6,#12`
(6 B) and the original's is `movs r2,#12; cmp r0,#25; it ne; movne r2,r6` (8 B) — a 2-byte
difference, and the `movs r2,#10` (`type = 10` before the race test) is the other 2 bytes.
The `int pick` loop reproduces both (see `hit1`'s loop at `97264..972d6`, identical to the
original's `97264..972d8`), but `int5b + 0.5f` is **also** 1992 B, so the loop's +4 B is
cancelled elsewhere.

The residual 4 B in the landed shape are a **constant-pool placement difference**:
ours emits `0x42be0000` (95.0) at `97928`, in the middle of the exception landing pads,
because the `case 0` amount computation (`difficulty / 10.0f * 95.0f`) is the first pool user
in layout order; the original emits it as the first entry of the trailing pool at `9795c`
(`0x42be0000, 0x44960000, 0x45ab0000, 0x3f99999a, 0x3f333333, 0x3f19999a, 0x3fd9999a,
got:galaxy, got:items, got:globals`) and pads with a `nop` at `97922`. Our pool is
`1200.0, 5500.0, 1.2, 0.7, 0.6, 1.7, <anchor>, got:galaxy, got:items, got:globals` at
`9795c` plus the stray `95.0` at `97928` — 11 entries vs the original's 10. Dropping the
anchor gives the same 10 entries but the stray `95.0` still sits in the landing-pad region,
so the function lands at 1992.

## 7. What this proves / what remains

**Proved.**

* The original's prologue slot map (`16=galaxy 20=&guard 24=agent 28=targetStation
  32=&status 36=race/type 40=rnd`) and the `strd r6,r0,[sp,#32]` pairing are reachable from
  this source tree — `head_nop10` (short pick, no p10 hint) reproduces all of it except the
  `agent`/`targetStation` swap, and `hit1` (int pick, no p10 hint, galaxy local) reproduces
  the frame with the original's exact loop encoding.
* The p10 hint is the single knob that trades the frame for `used -> sl`; no variant measured
  has both.
* The last frame difference is one SSC weight flip (`agent` 6.08 vs `targetStation` 4.00),
  caused by `agent` being reloaded after the loop. If `agent` stays in `r9` (as in the
  original), its weight ties `targetStation`'s 4.00 and the FI tie-break puts `targetStation`
  first, giving the original frame exactly.
* The `0.5f` deficit is a constant-pool placement difference (stray `95.0` at `97928`), not
  the `pick == 12` select.

**Remains.**

* A source shape that simultaneously (a) keeps `agent` in `r9` across the whole tail,
  (b) keeps `used` in `sl`, and (c) keeps `systems` in `r8`. `head_nop10` has (a) and (c) but
  not (b); `hit1`/`int5b` have (b) but not (a)/(c). The agent/systems `r8`/`r9` assignment is
  a pure greedy-allocator permutation of the two incoming parameters (`mov r8,r2; mov r9,r1`
  vs `mov r9,r2; mov r8,r1`) and was not flipped by any declaration order, tail order, type
  change or local-variable probe tried.
* The tail block shapes that still differ in every frame-96 build: the `type == 8` `amount`
  store (`str r0,[sp,#28]` in-block at `974a8` vs sunk to the join), the
  `getRoutes() == nullptr` test (`cmp r0,#0; beq.w 97586` out-of-line vs inline `cbz`), the
  `nextInt(2) == 0` block (out-of-line at `97586..9759a` vs inline), and the `0.5f` pool
  placement above.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4508
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4500

# single-symbol score (official verifier, autojunk=False)
python3 /dev/shm/w4508/score.py

# exact SSC weights + final slot map for the current source
/dev/shm/w4508/mi.sh
python3 /dev/shm/w4508/weights2.py /dev/shm/w4508/mi.txt
python3 /dev/shm/w4508/fimap2.py   /dev/shm/w4508/mi.txt
```

Scratch harness (not committed, `/dev/shm/w4508`): `run.sh <body.cpp>` (splice a whole
`createMission` body into the pristine `Generator.cpp`, `link_so`, score), `score.py` (official
single-symbol score), `feat2.py` (score + frame + `strd r6,r0,[sp,#32]` + `&guard` slot +
`used->sl` + `systems->r8` features), `weights2.py`, `fimap2.py`, `diffstat.py`/`head.diff`/
`hit1.diff`/`hn.diff` (official-normalised per-region diffs), `rsearch.py`/`rsearch2.py`/
`rsearch3.py` (the random sweeps), `tailsweep.py` (the 24 tail orders), `decl_sweep.py`
(the declaration permutations), and the candidate bodies `head_func.cpp` (landed),
`head_nop10.cpp`, `int5b.cpp`, `int5c.cpp`, `hit1.cpp`, `h1*.cpp`, `h_half*.cpp`.
