# Generator::createMission — `type`/`pick` coalescing is the single root cause; `0.5f` is
# proven but the loop is exactly 4 B short (w4706)

Status: **research note, no source change landed** (wave 97, 2026-11-03, worktree `w4706`,
branch `wave97-w4706`, base `9b58a39d5`). The landed `short pick` / `goto assignType` shape
(w4232/w4257/w4508) stays at **74.3 %** (`osize 1996  usize 1996`, 727 instructions).

Gate at HEAD (unchanged by this note):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass closes out the w4508 line of enquiry. It adds four measured facts:

1. **`0.5f` is proven by the binary** — the original emits `eeb6 1a00 vmov.f32 s2, #96`
   (`0.5` immediate) at `977ce`; our `0.5000001f` anchor emits `vldr s2,[pc,#424]` (a pool
   word). Dropping the anchor scores **74.5 %** but lands at **1992 B**, and the 4-byte deficit
   is *exactly* the loop (ours 128 B, original 132 B), not the pool.
2. **`short type` is the only spelling that reproduces the original's `mov r6,r0` for `pick`**
   — but it truncates `type` and collapses the score to 51.1 % / 2024 B.
3. **The `type`/`pick` coalescing is the single root cause of the whole remaining gap.** Every
   residual difference descends from it (see §3).
4. **The original's frame-96 + `used->sl` + `guard->r9` + `zero->r5` + `systems->r8` map IS
   reachable** with the full-`int` loop (`k1` below, 68.7 % / 1996 B) — but `agent` then lands
   in `fp`, and the score is 5.6 points below HEAD.

## 1. The `0.5f` deficit is the loop, not the pool

The original's `type == 8` costs block:

```
977c4: 9809        ldr     r0, [sp, #36]
977c6: 2808        cmp     r0, #8
977c8: ee10 5a10   vmov    r5, s0
977cc: d10a        bne.n   977e4
977ce: eeb6 1a00   vmov.f32 s2, #96        ; 0.5f  (immediate, no pool word)
977d2: eeb8 0ac0   vcvt.f32.s32 s0, s0
977d6: ee20 0a01   vmul.f32 s0, s0, s2
977da: eebd 0ac0   vcvt.s32.f32 s0, s0
977de: ee10 5a10   vmov    r5, s0
977e2: e011        b.n     97808
```

Ours (HEAD) has the same instruction count but `vldr s2,[pc,#424]` against a pool word
`0x3f000002` at the very end of the function. Replacing the anchor with plain `0.5f`:

```
HEAD + 0.5f : pct=74.5  osize=1996  usize=1992  n_ours=726
```

**74.5 > 74.3, but 1992 B is unusable on its own**: `createMission` is not the last function in
`.text`, so a 4-byte shrink shifts every following function and every `bl`-to-PLT displacement,
and `byte_exact` collapses (4499 -> 2615 in the w4508 measurement). The function must stay
exactly 1996 B.

Accounting for the 4 bytes (both functions are 727 instructions / 1996 B at HEAD):

| region | original | ours (HEAD) |
|---|---:|---:|
| loop `97248..972da` / `9724e..972dc` | 132 B | **128 B** |
| trailing constant pool | 10 words (`95.0` first at `9795c`) | 11 words (`0.5000001f` extra) |
| net | 0 | 0 |

So `0.5f` (which deletes the 11th pool word) needs the loop to grow by exactly 4 B, i.e. the
`movs r2,#10` (+2) and the non-folded `pick == 12` select (+2) — both of which require `type`
to be a *different* virtual register from `pick`.

## 2. The loop diff, instruction by instruction

Original `97256..972da`:

```
97256 ldr   r0,[sp,#40]        ; rnd
97258 movs  r1,#15
9725a ldr   r0,[r0,#0]
9725c blx   nextInt
97260 sub.w r9,r9,#1           ; --guard
97264 mov   r6,r0              ; pick
97266 cmp   r0,#8               ; pick == 8   (on r0!)
97268 beq.n 972d2
9726a cmp   r6,#10
9726c bne.n 97276
9726e ldr   r0,[sp,#36]        ; race
97270 movs  r2,#10              ; type = 10   <<< separate register
97272 cmp   r0,#3
97274 bhi.n 972d4
...
9729a mov   r2,r6              ; type = pick
...
972b0 cmp   r6,#12
972c0 movs  r2,#12              ; type = 12
972c4 mov.w fp,#1
972c8 it    ne
972ca movne r2,r6              ; type = pick
972d2 mov   r2,r6              ; <<< shared latch: type = pick
972d4 movs.w r0,fp,lsl#31
972d8 beq.n 97248
```

Ours (HEAD), same span:

```
9725a ldr   r0,[sp,#40]
9725c movs  r1,#15
9725e ldr   r0,[r0,#0]
97260 blx   nextInt
97264 sxth  r6,r0              ; pick  (short -> sxth)
97266 cmp.w r9,r0,lsl#16       ; pick == 8   (0x80000 in r9)
9726a beq.n 972d2
9726c lsls  r1,r0,#16
9726e cmp.w r1,#655360         ; pick == 10
97272 bne.n 9727e
97274 ldr   r0,[sp,#36]
97276 cmp   r0,#3
97278 bls.n 9727e
9727a movs  r6,#10              ; type = 10   <<< folded into pick (r6)
9727c b.n   972d2
...
972b0 cmp.w r1,#786432         ; pick == 12
972c8 it    eq
972ca moveq r6,#12              ; type = 12   <<< folded into pick
972d2 subs  r5,#1               ; --guard (moved to the latch)
```

Differences: `sxth` vs `mov`, the short-compare canonicalisation (2 extra instructions),
the missing `movs r2,#10`, the missing `movs r2,#12`, the folded `pick == 12` select, the
missing `mov r2,r6` latch, and `--guard` at the latch instead of after `nextInt`.

## 3. Why `type` and `pick` are coalesced — and why that cascades

In the original, `type` lives in `r2` and `pick` in `r6` for the whole loop. In every
`short pick` build measured here, clang 7.0.2's greedy coalescer merges them into `r6`.

The cascade (all of it follows from `type == pick` in `r6`):

* `movs r2,#10` / `movs r2,#12` disappear (folded into `movs r6,#10` / `moveq r6,#12`).
* `mov r2,r6` at the shared latch disappears.
* `type = 0` is emitted as `movs r6,#0`, which **evicts `&Globals::status` from `r6`**
  (`str r6,[sp,#28]` at `97234`) — the original keeps `&Globals::status` in `r6` from
  `971e8` to the epilogue.
* Because `r6` is free, the `Galaxy *galaxy` local stays in a register in ours; in the
  original it must be spilled to `[sp,#16]` (`97614 str`, `97644 ldr`).
* Because `status` is a stack slot in ours, `used` (`ldr [r0,#80]`) is spilled to `[sp,#32]`
  instead of `sl`, and the SSC weight order becomes
  `rnd > race > used > status > type > agent > targetStation` instead of the original
  `rnd > race > status > targetStation > agent > &__stack_chk_guard > galaxy`.
* That ordering is what produces the residual slot map `16=&guard 20=targetStation
  24=agent 28=status 32=used 36=race 40=rnd` instead of the original
  `16=galaxy 20=&guard 24=agent 28=targetStation 32=&status 36=race 40=rnd`, and the missing
  `strd r6,r0,[sp,#32]` pairing.

So the w4508 "p10-expect trades the frame for `used->sl`" observation is a *symptom*: the
p10 hint changes the loop's register pressure enough to move `type` off `r6`, but not enough
to keep it off `pick`.

### 3.1 `short type` does separate them — and breaks everything else

`short type` is the only spelling found that makes `pick` emit `mov r6,r0` (no `sxth`), i.e.
gives `pick` the original's encoding, and it produces `sxth r2,r6` for the `type = pick`
truncation. But `type` is then a 16-bit phi: `pct=51.1  usize=2024`. `int pick` + `short type`
gives `mov r6,r0` too (`pct=53.4  usize=2012`).

Type sweeps on the landed body (`short pick` fixed):

| `type` spelling | pct | usize | loop encoding |
|---|---:|---:|---|
| `int32_t` (HEAD) | 74.3 | 1996 | `sxth r6,r0` / `movs r6,#10` |
| `int` | 74.3 | 1996 | identical |
| `unsigned` | 74.3 | 1996 | identical |
| `long` | 74.3 | 1996 | identical |
| `short` | 51.1 | 2024 | `mov r6,r0` + `sxth r2,r6` |

## 4. The frame-96 + `used->sl` map is reachable, but not with `agent->r9`

`k1` = the full-`int` loop, p10 expect, `used` declared first, `itemId`/`amount` initialised,
`Galaxy *galaxy` local removed:

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
        type = pick;
        if (pick == 8) goto assignType;
        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin
            type = 10;
            if (race > 3) continue;
        }
        ... (unchanged) ...
```

```
k1: pct=68.7  osize=1996  usize=1996
    entry r8=systems r9=agent, guard=r9, used=sl, zero=r5, systems=r8, agent=fp
    frame 96: 92=canary 20=&guard 24=targetStation 28=&status 32=race 36=agent/type
```

This is the w4508 `int5b` row. It reproduces the original's **frame size, `used -> sl`,
`guard -> r9`, `zero -> r5` and `systems -> r8`**, and its loop is instruction-for-instruction
the original's — but `agent` lands in `fp` instead of `r9`, and the tail loses the slot matches,
so it scores 5.6 points below HEAD. Adding the `galaxy` local back (`h_p10_init`) gives
70.9 % / 1996 but the frame grows to 104.

## 5. Everything measured this pass (official verifier, `autojunk=False`)

| variant | pct | usize | note |
|---|---:|---:|---|
| HEAD (landed) | **74.3** | **1996** | baseline |
| HEAD + `0.5f` | **74.5** | **1992** | best score; 4 B short (loop) |
| HEAD + `0.5f` + duplicated `case 3:`/`case 5:` | 70.6 | 1996 | +4 B but the switch region worsens |
| `k1` (int pick, p10, used-first, init, no galaxy) | 68.7 | 1996 | original frame + `used->sl`; `agent->fp` |
| `h_p10_init` (`k1` + galaxy local) | 70.9 | 1996 | frame 104 |
| `hit1` (int pick, no p10, p12 expect 0, uninit) | 64.8 | 1972 | |
| `head_nop10` | 60.0 | 1988 | |
| `int pick` on the landed body | 48.0 | 2028 | |
| `short type` | 51.1 | 2024 | `mov r6,r0` for pick |
| `int pick` + `short type` | 53.4 | 2012 | `mov r6,r0` for pick |
| `getMissionBonus(race)` instead of `agent->getRace()` | 72.2 | 1988 | |
| `Galaxy *galaxy` local removed | 64.3 | 1988 | |
| `int itemId; int amount;` (uninit) | 73.7 | 1980 | |
| `int *imageParts = agent->getImageParts();` before the ctor | 57.8 | 1996 | |
| same, hoisted before `int bonus;` | 65.1 | 1988 | |
| `__builtin_expect(getRoutes()==nullptr, 0)` | 56.6 | 1972 | frame 104 |
| inverted `getRoutes() != nullptr` / `bool noRoutes` / `== 0` / `goto`-forced order | 74.3 | 1996 | byte-identical to HEAD |
| `do { } while (!found)` for the route search | 32.2 | 2028 | |
| `for (int guard = 999; ...)` in the for-init | 74.3 | 1996 | identical |
| `do { } while (!selected)` loop | 60.9 | 1988 | |
| `for (;;) { ... if (selected) break; --guard; }` | 50.7 | 1996 | |
| p12 forms (`type = 12; if (!=25) type = pick;`, `goto` variants) | 73.8-74.5 | 1992 | |
| 120 declaration permutations x {galaxy before/none/after} x {0.5f y/n} | 74.3 max | 1996 | no order beats HEAD |
| 32 combos {short,int} pick x {for,while} x {p10} x {used-first} x {init} | 74.3 max | 1996 | no combo beats HEAD |
| `int`/`int32_t`/`unsigned` on `level`/`bonus`/`costRem`/`difficulty`/`fromSystem`/`guard`/`sum` | 74.3-74.5 | 1992-1996 | all neutral or 1992 |
| `uint32_t`/`int32_t` `choice`, `difficulty > 9`, `5 + nextInt(15)` | 74.5 | 1992 | neutral |

**No variant reaches 1996 B with a score above 74.3.** Every `0.5f` build is 1992; every
1996 B build is either HEAD or scores lower.

## 6. Block placement of the `getRoutes() == nullptr` cold block is not source-controllable

The original places the `type = 4; if (nextInt(2)==0) type = 1;` block **out of line** at
`97586..9759a` (reached by `973a8: beq.w 97586`), between the `type == 15` block and the
switch jump table. Ours keeps it inline at `973f8` (`973a8: cbz r0, 973f8`), which costs
~11 matching instructions. None of the source spellings tried moves it:

* `if (getRoutes() == nullptr) {...} else {...}` (landed) — inline
* inverted `if (getRoutes() != nullptr) {...} else {...}` — inline
* `bool noRoutes = ...; if (noRoutes)` — inline
* `if (!getRoutes())`, `if (getRoutes() == 0)` — inline
* `goto noRoutes; ... noRoutes: ...` forced order — inline
* `type = 4; if (getRoutes() != nullptr) {...} else if (nextInt(2)==0) type = 1;` — inline
* `__builtin_expect(getRoutes() == nullptr, 0)` — out of line, but re-allocates the whole
  function (frame 104, 1972 B, 56.6 %)

The block is placed inline because `MachineBlockPlacement` chains it to the join; the original's
placement is a consequence of the *different* CFG that the non-coalesced `type` produces, not of
a source-level hint.

## 7. What remains

The whole remaining gap reduces to one register-allocation question:

> find a source spelling in which `type` is a 32-bit value that is **not** coalesced with
> `pick` (so `movs r2,#10`, `movs r2,#12` and the latch `mov r2,r6` survive) while `pick`
> stays `short` (so `systems -> r8`, `rnd -> [sp,#40]`, frame 96 survive).

Every candidate found so far gets one of the two. `short type` gets the `pick` encoding but
truncates `type`; `int pick` gets the loop but loses `systems -> r8` / frame 96 / `used -> sl`
(unless `used` is declared first and `galaxy` is dropped, which then loses `agent -> r9`).

### 7.1 First source form found that separates `type` from `pick`

Indexing `used` with `type` instead of `pick` (`if (used->data()[type])`) is the **only**
spelling found so far that makes clang keep `type` in `r2` and `pick` in `r5`:

```
e1 = HEAD with `if (used->data()[type])`:
97264 sxth  r5,r0              ; pick
97266 cmp.w r9,r0,lsl#16
9726a beq.n 972d6
9726c lsls  r1,r0,#16
9726e cmp.w r1,#655360
97272 mov   r2,r5              ; type = pick   <<< a real copy now
97274 bne.n 97282
97276 ldr   r0,[sp,#36]
97278 movs  r2,#10             ; type = 10     <<< THE ORIGINAL'S INSTRUCTION
9727a cmp   r0,#3
9727c bls.n 97282
9727e movs  r5,#10
97280 b.n   972d6
```

`e1` scores **67.4 % / 1996 B** (728 instructions): the extra loop-top `mov r2,r5` and the
resulting shift of `guard` from `r5` to `r6` cost more than the `movs r2,#10` buys. Variants
measured: `e1` + `int pick` 49.0/1964; `e1` + `used->data()[type] = true` 67.5/2004;
`e1` + non-folded p12 (`f1`) 67.2/1996; `f1` + `0.5f` 67.4/1992; `e1` + `0.5f` 67.5/1992.
So the separation is achievable, but not *together with* the rest of the original's
allocation. A future pass should start from `e1` and try to recover `guard -> r9` /
`used -> sl` (e.g. by declaring `used` first, or by dropping the loop-top `type = pick` and
re-deriving `type` in the used branch).

With that in hand, `0.5f` becomes usable: the loop grows by exactly the 4 B that the removed
`0.5000001f` pool word costs, and the function stays at 1996 B. That should be worth roughly
+0.2 (the `0.5f` gain) plus the ~8 loop instructions, i.e. into the high 70s.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4706
cmake --build cmake-build-match --target check -j8     # RATCHET PASS, byte 4500
python3 /dev/shm/w4706/score.py                        # pct=74.3 osize=1996 usize=1996
```

Scratch harness (not committed, `/dev/shm/w4706`): `run.sh <body.cpp>` (splice a whole
`createMission` body into the pristine `Generator.cpp`, `link_so`, score), `score.py` (official
single-symbol score), `feat.py` (entry regs, guard/used/zero regs, prologue slot list),
`vrun2.py <files...>` (apply+build+score+features for many bodies), `sdiff.py`/`regions.py`
(official-normalised per-region diff), `sizes.py` (per-region byte deltas), `chk.sh` (loop
encoding probe), `search3.py` (the 120-order declaration sweep), and the candidate bodies
`head_func.cpp` (landed), `v_half.cpp`/`x0.cpp` (0.5f), `hit1.cpp`, `head_nop10.cpp`,
`h_p10_init.cpp`, `k1.cpp`, `y1.cpp`, `q0..q8.cpp` (pick/type matrix).
