# Generator::createMission — the faithful `int pick` loop is instruction-for-instruction
# identical; the whole residual is one stack-slot chain rooted at the `amount`/`difficulty`
# schedule (wave 101)

Status: **research note, no source change landed** (wave 101, 2026-11-04, worktree `wf103`,
branch `wave101-wf103`, base `3856949ab`). The landed `short pick` / `goto assignType` shape
(w4232/w4257/w4508/w4706/w4808/w4905/w5005) stays at **74.3 %** (`osize 1996  usize 1996`).

Gate at HEAD (re-measured at the start and end of this pass, unchanged):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass adds three facts that the earlier notes only half-stated:

1. **The faithful `int pick` loop is instruction-for-instruction identical to the original.**
   With `base_ip.cpp` (faithful `while (!selected)` + `int pick` + `--guard` in the body + no
   loop-top `type = pick` + `__builtin_expect(pick == 10, 1)` + `__builtin_expect(pick == 12, 0)`
   + `goto assignType` + the `Galaxy *galaxy` local), the whole range `97248..972d8` matches
   the original *exactly* except (a) three `[sp,#N]` offsets and (b) the block placement of the
   `pick == 12` arm.  Every register, every immediate, every branch target inside the loop is
   right: `mov r6,r0`, `cmp r0,#8`, `movs r2,#10`, `mov r2,r6`, `movs r2,#12`,
   `it ne / movne r2,r6`, `movs.w r0,fp,lsl#31 / beq 97248`.  See §1.
2. **`base_ip` is frame 104, `base_ip_nogal` is frame 96** — the extra 8 bytes are exactly the
   `fromSystemPtr` spill.  The original is frame 96 *with* a `galaxy` spill, i.e. the original
   keeps `fromSystemPtr` in `r6` and spills `galaxy`, while every shape we can build does the
   opposite.  See §2.
3. **The cause is the `amount` / `difficulty` schedule in the `type == 8` arm.**  The original
   emits `blx getTecLevel; mov r1,r0; adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1` — i.e. it
   stores `amount` *inside the arm* and only then copies the `getTecLevel()` result into `r6`.
   Ours emits `blx getTecLevel; mov r5,r0; adds r0,r6,#5; b <shared amount store>` — the copy
   comes first and lands in `r5`.  `r6` (which holds `nextInt(15)`) is still live at the copy,
   so the allocator cannot use it.  Once `difficulty` is in `r6` (original), `r6` is occupied
   when `Globals::galaxy` is loaded at `97612`, `galaxy` spills to `[sp,16]`, `fromSystemPtr`
   takes `r6`, and the whole original slot map follows.  See §3.

No source spelling tried in this pass (or in w4508/w4706/w4808/w4905/w5005) makes clang 7.0.2
emit the original's `adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1` sequence, so the chain is not
broken yet.  HEAD therefore stays at 74.3 %.

## 1. The loop is already exact

`base_ip.cpp` (faithful `int pick`, `Galaxy *galaxy` local, `0.5000001f` anchor) vs the
original, `0x97248..0x972d8`, normalised to instruction text (`objdump -d`):

```
  original                                            base_ip
  mov.w  fp, #0                                       mov.w  fp, #0
  cmp.w  r9, #1                                       cmp.w  r9, #1
  it     lt                                           it     lt
  movlt.w fp, #1                                      movlt.w fp, #1
  ldr    r0, [sp, #40]        <-- rnd                 ldr    r0, [sp, #48]        <-- rnd
  movs   r1, #15                                      movs   r1, #15
  ldr    r0, [r0, #0]                                 ldr    r0, [r0, #0]
  blx    AERandom::nextInt                            blx    AERandom::nextInt
  sub.w  r9, r9, #1                                   sub.w  r9, r9, #1
  mov    r6, r0                                       mov    r6, r0
  cmp    r0, #8                                       cmp    r0, #8
  beq.n  972d2                <-- assignType          beq.n  972b8                <-- assignType
  cmp    r6, #10                                      cmp    r6, #10
  bne.n  97276                                        bne.n  97276
  ldr    r0, [sp, #36]        <-- race                ldr    r0, [sp, #44]        <-- race
  movs   r2, #10                                      movs   r2, #10
  cmp    r0, #3                                       cmp    r0, #3
  bhi.n  972d4                <-- continue             bhi.n  972d4                <-- continue
  cmp.w  sl, #0                                       cmp.w  sl, #0
  beq.n  972b0                                        beq.n  972b0
  ldr.w  r0, [sl, #4]                                 ldr.w  r0, [sl, #4]
  ldrb   r1, [r0, r6]                                 ldrb   r1, [r0, r6]
  cbz    r1, 972ac                                    cbz    r1, 972ac
  ldr.w  r1, [sl]                                     ldr.w  r1, [sl]
  movs   r2, #0                                       movs   r2, #0
  movs   r3, #0                                       movs   r3, #0
  b.n    97294                                        b.n    97294
  ldrb   r4, [r0, r2]                                 ldrb   r4, [r0, r2]
  adds   r2, #1                                       adds   r2, #1
  add    r3, r4                                       add    r3, r4
  cmp    r1, r2                                       cmp    r1, r2
  bne.n  9728e                                        bne.n  9728e
  cmp    r3, #14                                      cmp    r3, #14
  mov    r2, r6                                       mov    r2, r6
  bne.n  972d4                                        bne.n  972d4
  b.n    972a6                                        b.n    972a6
  strb.w r5, [r0], #1                                 strb.w r5, [r0], #1
  subs   r1, #1                                       subs   r1, #1
  cmp    r1, #0                                       cmp    r1, #0
  bne.n  972a0                                        bne.n  972a0
  b.n    972d2                <-- assignType          b.n    972b8                <-- assignType
  movs   r1, #1                                       movs   r1, #1
  strb   r1, [r0, r6]                                 strb   r1, [r0, r6]
  cmp    r6, #12                                      cmp    r6, #12
  bne.n  972ce                <-- NOT-taken           beq.n  972bc                <-- TAKEN
  ldr    r0, [sp, #32]        <-- status              mov.w  fp, #1
  ldr    r0, [r0, #0]                                 mov    r2, r6
  blx    Status::getSystem                            b.n    972d4
  blx    SolarSystem::getIndex                        ldr    r0, [sp, #40]        <-- status
  movs   r2, #12                                      ldr    r0, [r0, #0]
  cmp    r0, #25                                      blx    Status::getSystem
  mov.w  fp, #1                                       blx    SolarSystem::getIndex
  it     ne                                           movs   r2, #12
  movne  r2, r6                                       cmp    r0, #25
  b.n    972d4                                        mov.w  fp, #1
  mov.w  fp, #1                                       it     ne
  mov    r2, r6                                       movne  r2, r6
  movs.w r0, fp, lsl #31                              movs.w r0, fp, lsl #31
  beq.n  97248                                        beq.n  97248
```

So the only real residual in the loop is **which arm of `cmp r6,#12` is the fall-through**:
the original branches away on `pick != 12` (`bne.n 972ce`) and keeps the `pick == 12` body
inline; ours branches away on `pick == 12` (`beq.n 972bc`) and keeps the `pick != 12` arm
(`mov fp,#1; mov r2,r6`) inline.  This is `MachineBlockPlacement`, not a source difference
(the source is the same `if (pick == 12) { ... goto assignType; }` shape in both).

## 2. `base_ip` (frame 104) vs `base_ip_nogal` (frame 96): the extra slot is `fromSystemPtr`

| | frame | slots |
|---|---:|---|
| original | 96 | 16=galaxy/bonus, 20=&__stack_chk_guard, 24=agent/difficulty, 28=targetStation, 32=status/rem/mission, 36=race/type, 40=rnd |
| `base_ip` (galaxy local) | 104 | 20=bonus, 24=**fromSystemPtr**, 28=&__stack_chk_guard, 32=targetStation, 36=agent, 40=status, 44=race, 48=rnd |
| `base_ip_nogal` | 96 | 16=&__stack_chk_guard, 24=agent, 28=status, 32=targetStation, 36=race, 40=rnd (20 free) |

The original keeps `fromSystemPtr` in `r6` (`9762c: ldr.w r6,[r1,r0,lsl#2]`) and spills
`galaxy`; `base_ip` keeps `galaxy` in `r6` and spills `fromSystemPtr` to `[sp,24]`.  Both
spill exactly one of the two, but the original's `galaxy` slot is *shared with `bonus`*
(`[sp,16]` is written at `97614`, `976d8`, `97720`), while our `fromSystemPtr` slot is not
shared with `bonus`, so we need one more colour and the frame grows by 8.

`base_ip_nogal` is only two slot moves away from the original (it has `&__stack_chk_guard` at
16 instead of 20, and `status`/`targetStation` swapped at 28/32) and already reproduces every
register of the original (`systems->r8`, `agent->r9`, `guard->r9`, `used->sl`, `zero->r5`,
`type->r2`, `pick->r6`, `selected->fp`, `rnd->[sp,40]`) — but it scores **68.7 %** because
those two slot moves shift every load/store in the function.

## 3. The root of the chain: the `type == 8` arm's `amount` / `difficulty` schedule

Original, `0x9748e..0x974ac`:

```
9748e: ldr  r0, [sp, #40]        ; rnd
97490: ldr  r0, [r0, #0]
97492: movs r1, #15
97494: blx  AERandom::nextInt
97498: mov  r6, r0               ; r6 = nextInt(15)
9749a: ldr  r0, [r5, #4]         ; items->data()
9749c: ldr.w r0, [r0, sl, lsl#2]
974a0: blx  Item::getTecLevel
974a4: mov  r1, r0               ; r1 = tecLevel
974a6: adds r0, r6, #5           ; amount = nextInt(15) + 5
974a8: str  r0, [sp, #28]        ; amount stored IN the arm
974aa: mov  r6, r1               ; difficulty = tecLevel  (r6 reused)
974ac: b.n  9760a
```

Ours (`base_ip`), `0x974a6..0x974bc`:

```
974a6: blx  AERandom::nextInt
974aa: mov  r6, r0               ; r6 = nextInt(15)
974ac: ldr  r0, [r5, #4]
974ae: ldr.w r0, [r0, sl, lsl#2]
974b2: blx  Item::getTecLevel
974b6: mov  r5, r0               ; difficulty = tecLevel  -> r5, because r6 is still live
974b8: adds r0, r6, #5           ; amount = nextInt(15) + 5
974ba: b.n  975fa                ; shared store: 975fa str r0,[sp,#20]
```

Because `r6` still holds `nextInt(15)` at the copy, the allocator puts `difficulty` in `r5`.
The original's scheduler emitted the `adds` **before** the copy (it has to: `adds r0,r6,#5`
reads `r6` and `mov r6,r1` writes it), which frees `r6` for `difficulty`.  The two forms are
mutually recursive — the register choice decides the schedule and the schedule decides the
register — and every source spelling tried (below) keeps the copy first.

Downstream, once `difficulty` is in `r6`:

* `9760c: cmp r6,#10` / `97618: movge r6,#10` (original) instead of `cmp r5,#10` (ours);
* `97612: ldr r0,[r5,#0]` / `97614: str r0,[sp,#16]` — `galaxy` has no callee-saved register
  left and spills;
* `9762c: ldr.w r6,[r1,r0,lsl#2]` — `fromSystemPtr` takes the freed `r6`;
* `9761c: str r6,[sp,#24]` — `difficulty` reuses `agent`'s slot;
* the `strd r6,r0,[sp,#32]` pairing then forms because `status` and `race` end up adjacent.

## 4. Everything measured this pass

All rows are the official verifier (`tools/verify/verify.py --show` / `scorefn.py`), one
`link_so` per row.  `frame`/`strd32` from `objdump -d --start-address=0x971b8
--stop-address=0x97260`.

Faithful `int pick` loop (`base_ip` family; frame 104 unless noted):

| variant | source change | pct | usize | frame |
|---|---|---:|---:|---:|
| `base_ip` | galaxy local before `fromSystem`, used for both calls | **70.9** | 1996 | 104 |
| `base_ip_nogal` | no galaxy local at all | 68.7 | 1996 | **96** |
| A | galaxy before `fromSystem`; `Globals::galaxy->getStation`, `galaxy->distance` | 64.1 | 2012 | 104 |
| C | galaxy after `fromSystemPtr`; `Globals::galaxy->getStation` | 69.9 | 1988 | 104 |
| D | galaxy after `fromSystemPtr`; `galaxy->getStation` | 69.9 | 1988 | 104 |
| E | galaxy before; `galaxy->getStation`, `Globals::galaxy->distance` | 64.0 | 2012 | 104 |
| F | galaxy declared at top, both calls via galaxy | 62.2 | 1996 | 104 |
| G | galaxy at top, `Globals::galaxy->getStation` | 62.1 | 1992 | 104 |
| H | galaxy declared at top, assigned at reward block | 64.1 | 2012 | 104 |
| I | galaxy declared at top, assigned at reward, both via galaxy | 70.9 | 1996 | 104 |
| N | galaxy at top **and** used for `getAsteroidProbabilities` (type 15) | 52.3 | 1988 | 104 |
| O | same, `Globals::galaxy->getStation` | 52.1 | 1988 | 104 |
| J/K/L | `type = 12; if (getIndex() != 25) type = pick; continue;` | 45.4 | 1976 | 104 |
| q1/q2 | `items` hoisted out of the `type == 8` arm | 68.2 | 1988 | 104 |
| q3 | `items` declared before, assigned inside | 69.4 | 1996 | 104 |
| `r_base_ip_nogal` | `base_ip_nogal` + `Globals::items` used directly (no `items` local) | 69.7 | 2000 | **96** |
| `t1` | `r_base_ip_nogal` + plain `0.5f` (the anchor is no longer needed) | **69.9** | **1996** | **96** |
| a,b,f,g,h | `amount` via temp / `5 + nextInt(15)` / `(void)amount` | 70.9 | 1996 | 104 |
| c | `int tec = getTecLevel(); difficulty = tec;` | 69.1 | 2000 | 104 |
| d,e | `getTecLevel()` first, then `amount` | 68.7 | 1992 | 104 |
| s1-s4 | `t1`/`base_ip_nogal` + galaxy local in four positions | 53.3-69.4 | 1992-2028 | 104 |
| u1-u3 | `t1` + galaxy local in three positions | 53.3-69.4 | 1992-2024 | 104 |

HEAD family (`short pick`; frame 96):

| variant | source change | pct | usize |
|---|---|---:|---:|
| HEAD | landed | **74.3** | 1996 |
| p1 | HEAD + `type = 12; if (getIndex() != 25) type = pick;` | 73.6 | 1996 |
| p2 | HEAD + `amount` via temp | 74.3 | 1996 |
| p3 | HEAD + `tec` temp | 72.4 | 2000 |
| p4 | HEAD + `--guard` moved into the body | 65.2 | 2004 |
| p5 | HEAD + `while (!selected)` | 65.2 | 2004 |

**No variant beats 74.3 % at 1996 B.**  `HEAD + 0.5f` is still 74.5 % / 1992 B (w4905 §7) and
still unusable on its own because `createMission` is not the last function in `.text`.

### 4.1 `t1`: the closest faithful frame-96 shape (69.9 %, 1996 B)

`t1` = `base_ip_nogal` with the `Array<Item *> *items = Globals::items;` local removed (all
`items` uses spelled `Globals::items`) and the `0.5000001f` anchor replaced by plain `0.5f`.
It is **1996 B, frame 96**, and its slot map is

```
16 = (free)            20 = &__stack_chk_guard   24 = agent
28 = status            32 = targetStation        36 = race        40 = rnd
```

i.e. **five of the original's seven slots are already in place** (`20`, `24`, `36`, `40` and
the canary at `92`); only `galaxy -> 16` and the `status`/`targetStation` swap at `28`/`32`
are missing.  Removing the `items` local is what moves `&__stack_chk_guard` from `16` to `20`
and frees `16`.  Adding the `galaxy` local back in any of the seven positions tried (`s1-s4`,
`u1-u3`) re-colours the whole frame to 104 and scores 53-69, so the `galaxy` spill still has
to be produced *without* a source-level `Galaxy *galaxy` local — plausibly by the
`amount`/`difficulty` schedule of §3 leaving `r6` occupied at `97612`.

`t1` is a strictly more faithful 1996 B shape than the landed `HEAD` (real `int pick` loop,
real `0.5f`, frame 96, five of seven slots right) but scores 4.4 points lower on the fuzzy
metric because the two wrong slots shift every load/store.

## 5. What is proven vs still hypothesis

Proven (measured, reproducible):

* The faithful `int pick` loop's instruction stream is identical to the original's except
  stack offsets and the `pick == 12` arm placement (§1).
* `base_ip` is frame 104 because `fromSystemPtr` gets its own slot; `base_ip_nogal` is frame 96
  because it has no `galaxy` local (§2).
* The original schedules `adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1`; ours schedules
  `mov r5,r0; adds r0,r6,#5` with a shared amount store at `975fa` (§3).
* No source spelling tried in this pass produces the original's `amount`/`difficulty`
  schedule or a frame-96 build with a `galaxy` spill.

Hypothesis (not proven):

* That the `amount`/`difficulty` schedule is the *sole* root: making `difficulty` land in `r6`
  should cascade into `galaxy -> [sp,16]`, `fromSystemPtr -> r6`, `status -> [sp,32]`,
  `targetStation -> [sp,28]` and `strd r6,r0,[sp,#32]`, i.e. the whole original slot map.  The
  cascade is consistent with every observation but has not been demonstrated, because no
  source shape yet produces the first step.
* That the `pick == 12` arm placement is pure `MachineBlockPlacement` and not source-driven
  (the `bne`/`beq` inversion follows the CFG difference caused by `goto assignType`; the
  `V2` label-first form in w4905 §6 is the only lever found that flips it, and it re-allocates
  the function).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf103
cmake --build cmake-build-match --target check -j8     # RATCHET PASS, byte 4500

# splice a whole createMission body into the pristine Generator.cpp, link, score
python3 /dev/shm/wf103/splice.py /dev/shm/wf103/base_ip.cpp
cmake --build cmake-build-match --target link_so
python3 /dev/shm/wf103/scorefn.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# pct=70.9 osize=1996 usize=1996 nO=727 nU=725

# loop instruction diff (normalise addresses away)
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x97248 --stop-address=0x972dc cmake-build-match/libgof2hdaa.so
$OD -d --start-address=0x97248 --stop-address=0x972dc \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# prologue slot map
$OD -d --start-address=0x971b8 --stop-address=0x97260 cmake-build-match/libgof2hdaa.so

# Ghidra decompile of the original (project android_2.0.16_libgof2hdaa.so, image base 0x10000;
# real 0x971b8 -> Ghidra 0xa71b8)
cd /dev/shm/wf103
FN_ADDR=0xa71b8 OUT_PATH=/dev/shm/wf103/decomp_orig.c \
  timeout 600 /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2 GalaxyOnFire2 \
  -process android_2.0.16_libgof2hdaa.so -noanalysis \
  -scriptPath /home/debian/ghidra_scripts -postScript DecompFile.java
```

Scratch harness (not committed, `/dev/shm/wf103`): `splice.py` (body -> `Generator.cpp`),
`checkpro.sh <body.cpp>` (splice + `link_so` + `frame`/`strd32` + official score),
`scorefn.py` (official single-symbol score), `slots.py` (prologue store/slot dump),
`mk_k1b.py`, `mkg.py`/`mkg2.py`/`mkj.py`/`mkm.py`/`mkn.py`/`mkp.py`/`mkq.py` (the variant
generators used for §4), `decomp_orig.c` (Ghidra decompile), `orig_full.txt`, `mi.txt`
(`-mllvm -print-machineinstrs` dump used to read the stack-slot colouring of `base_ip_nogal`:
pre-SSC `%stack.0`=canary, `.5`=`&__stack_chk_guard`, `.6`=targetStation, `.7`=agent,
`.8`=rnd, `.9`=race, `.10`=status, `.11`=type; post-SSC `.0`=canary, `.5`=rnd, `.6`=race,
`.7`=targetStation/type, `.8`=status, `.9`=agent, `.10`=`&__stack_chk_guard`).

Candidate bodies: `head_body.cpp` (landed), `base_ip.cpp`, `base_ip_nogal.cpp`, `v/*.cpp`
(A-O, J/K/L, a-h, p1-p5, q1-q3).
