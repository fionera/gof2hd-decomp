# ModStation ctor + MGame::OnUpdate — wave 98 (w4820)

Status: research pass, 2026-11-04. Worktree `wt/w4820`, branch `wave98-w4820`,
based on master `fe4f5e7cf`. **No source change landed**: both targets are
already at their best measured shape and every new alternative tried this pass
regressed the raw diff (or was size-neutral but byte-different). Gate unchanged.

Targets:
* `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases), 1008 B @ 0xd52d0.
  Residual **39 differing bytes / 8 runs**.
* `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8. Residual **41 differing bytes /
  1 run** at `+0x17bb..+0x17e3` (absolute 0x17e093..0x17e0bb).

All numbers measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the
tree's NDK r18b clang 7.0.2 and the official verifier. Scratch drivers live in
`/dev/shm/w4820` (`fdiff.py`, `try_mgame.py`, `batch10.py`..`batch16.py`,
`ms_order.py`, `ms_batch*.py`, `cc_greedy.sh`, `cc_ir.sh`, `cc_ll.sh`).

## 0. Gate (unchanged before and after this pass)

| metric | value |
|---|---:|
| byte_exact / linked_exact | 4500 / 4524 |
| size-exact | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` / `.rodata` / `.ARM.exidx` | `0x1337c8` / `0x41b67` / `0x6f70` |
| `.ARM.extab` | `0x26294` (-4) |

`cmake --build cmake-build-match --target check -j8` ends with `RATCHET PASS`.

Both targets are **byte-exact or not**; `byte_exact` only rises at 0 diff. A
39-byte or 41-byte residual is therefore gate-neutral.

---

# Part 1 — `MGame::OnUpdate` (41 B / 1 run)

## 1.1 Baseline residual (recap)

The whole 11440-byte body differs in exactly one run, the mission-23
idle-contact condition (`src/game/menu/MGame.cpp:2770-2776`):

```
orig                                                    ours (baseline)
17e088 ldr r0,[pc,#872]                                 17e088 ldr r0,[pc,#872]
17e08a add r0,pc                                        17e08a add r0,pc
17e08c ldr.w r9,[r0]   ; &Globals::status               17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; Status*                        17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission                    17e094 mov r0,r5
                                                        17e096 blx getCurrentCampaignMission
17e098 cmp r0,#23                                       17e09a cmp r0,#23
17e09a bne 17e176                                       17e09c bne 17e176
17e09c ldr.w r0,[sl]                                    17e09e ldr.w r0,[sl]
17e0a0 blx isMining                                     17e0a2 blx isMining
17e0a4 cmp r0,#0                                        17e0a6 cmp r0,#0
17e0a6 bne 17e176                                       17e0a8 bne 17e176
17e0a8 ldr.w r0,[r9]   ; Status*                        17e0aa ldr.w r0,[r9]
17e0ac blx getMission                                   17e0ae blx getMission
17e0b0 blx isEmpty                                      17e0b2 blx isEmpty
17e0b4 cmp r0,#0                                        17e0b6 cmp r0,#0
17e0b6 beq 17e176                                       17e0b8 beq 17e176
17e0b8 ldr.w r0,[r9]   ; Status*                        17e0ba mov r0,r5
17e0bc blx getPlayingTime                               17e0bc blx getPlayingTime
17e0c0 ldr.w r2,[r9]   ; Status*                        17e0be ldr.w r2,[r9]
```

The original has **four fresh `ldr [r9]` loads, no cached `Status*`, and no
`mov r0,rN` before any `Status::` call**. The baseline has one cached `st`
load in r5 and two `mov`s. The byte count is exactly neutral
(`ldr r5` + 2 movs = 8 B; two `ldr r0` = 8 B), so the run is a pure
permutation. `&Globals::status` is in **r9 in both** builds; everything
outside `0x17e088..0x17e0c4` is byte-identical.

## 1.2 Confirmed this pass: the original's IR is the no-`st` shape

Re-dumped the faithful `-Oz` machine IR (`-mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN5MGame8OnUpdateEv`, 1.1 M lines, ~7 s) for both
the baseline and the no-`st` source. After the virtual-register rewriter:

```
baseline  bb.238 liveins $r6,$r10,$r11
          $r9 = t2LDRi12 $r0,0          ; &Globals::status (GOT)
          $r5 = t2LDRi12 $r9,0          ; st
          $r0 = COPY $r5                ; ccm receiver
          ...
no-st     bb.238 liveins $r6,$r10,$r11
          $r5 = t2LDRi12 $r0,0          ; &Globals::status -> r5
          $r0 = t2LDRi12 $r5,0          ; ccm receiver (fresh)
          ...
          bb.242  $r9 = COPY $r5        ; split at the body boundary
```

So the no-`st` IR is exactly the original's shape (four distinct loads), but
the greedy allocator splits `&Globals::status` at the mission-block/body
boundary and puts the first segment in **r5** (2-byte `ldr rX,[r5]`), then
copies to r9 for the body. The whole function re-colours: 11392 B, 5148 diff /
212 runs. The `st` local is a *size anchor* that buys the r9 colour at the
cost of the two `mov`s.

## 1.3 The r5-occupancy requirement (restated precisely)

The `&Globals::status` vreg is a long, low-density range (72 uses, one live
range `0x17e08c..0x17e846`). It is **not rematerialisable** (a non-const
`.bss` GOT load; see wave-96 §5). The allocator always splits it at the
body boundary (`%2930 = COPY %2931`); the two segments can then take different
registers. In the baseline segment 1 is r9 *because r5 is taken by `st`*. In
the no-`st` build segment 1 is r5.

Therefore the original must contain **a value that is live in r5 somewhere in
`0x17e08c..0x17e846` and outranks the base**, while the four `Status*` loads
stay independent. r5 is *not used* in `0x17e044..0x17e12a` in the original
(the previous r5 value dies at `0x17e044`, the next write is `0x17e12a`), so
the occupant is a value live across the window but not read/written in it —
like `r6 = &this->elapsedTime`. The reconstruction has only one such value
(r6); the original must have two. **This is the one missing source fact.**

## 1.4 New lead: `_CARLA_ANNOYING_CALLS`

The iOS 1.1.19 source-layout dump lists the statics of `Classes/Game/MGame.cpp`
as `_CARLA_ANNOYING_CALLS`, `_CHAR_MKKT_BKKT`, `_CHAR_TOMMY`, `_CHAR_CARLA`
(`source file list dump/gof2hd_1.1.19_ios.cpp:1574`). `_CARLA_ANNOYING_CALLS`
is also a static in `Classes/Game/Status.cpp` (line 3082), i.e. it lives in a
header included by both TUs (the `CharacterImages.h` pattern). The current
`MGame.cpp` has **no** `CARLA_ANNOYING_CALLS`; it hard-codes 23/24/122/124 and
uses a `gMGameFaces` concatenation of `CHAR_TOMMY+CHAR_CARLA+CHAR_ERRKT`.

This is a real source-shape lead for the idle-contact code (the mission-23/24
blocks are Carla's "annoying calls"). It does **not** by itself change the
mission-23 block: a `static const int CARLA_ANNOYING_CALLS[] = {23, 24}` used
as `CARLA_ANNOYING_CALLS[0]` folds to `cmp r0,#23` and gives the same 11392 /
5148 no-`st` build (measured: `static_arr`). It is worth pursuing in a future
pass because the array may be *non-const* in the original (a real load, not a
fold), or it may be the source of the missing r5 occupant in a different
block.

Searched the Android binary for the obvious arrays (`{23,24}`,
`{23,24,122,123,124}`, `{122,123,124}`, `{533,534,535,536}`,
`{464,465,467,468}`, `{3164,3166}`): only `{23,24}` exists, at `0x18f430`
inside `.text` (a literal pool), not `.data`/`.rodata`. So the array is either
const-folded or has a different element type.

## 1.5 Variant sweep this pass

All measured with the official verifier (`link_so` + `fdiff.py`). Baseline
rows are from wave 96/97; new rows are marked ★.

| variant | size | diff | runs |
|---|---:|---:|---:|
| **baseline `st` at getPlayingTime** | **11440** | **41** | **1** |
| no `st` (reference) | 11392 | 5148 | 212 |
| `st` at ccm + pt (same IR as baseline) | 11440 | 41 | 1 |
| `st` at ccm + field | 11436 | 4993 | 301 |
| `st` at ccm + mission | 11396 | 5129 | 240 |
| `st` at mission + pt | 11436 | 4982 | 303 |
| `st` at ccm + mission + field | 11436 | 4966 | 308 |
| `st` at ccm + pt + field | 11436 | 4981 | 307 |
| `st` at mission + pt + field | 11432 | 5033 | 270 |
| `st` at all four | 11432 | 5033 | 270 |
| ★ `__atomic_signal_fence` between `st` and the `if` (`fence_pt`) | 11440 | 853 | 36 |
| ★ same + `st` used at ccm (`fence_ccm_pt`) | 11440 | 41 | 1 |
| ★ `st` assigned in the condition, `(st = G)->getPlayingTime()` | 11392 | 5148 | 212 |
| ★ `st` assigned in the condition after getMission | 11392 | 5148 | 212 |
| ★ lambda `[&]{ Status *st = G; return ...; }()` | 11392 | 5148 | 212 |
| ★ `__builtin_assume(st != nullptr)` + `st` at pt | 11392 | 5148 | 212 |
| ★ `__builtin_assume(G != nullptr)` | 11392 | 5148 | 212 |
| ★ `st == st` tautology in the condition | 11392 | 5148 | 212 |
| ★ `st != nullptr` in the condition | 11444 | 4997 | 310 |
| ★ `st->field_0x100;` dead read before the `if` | 11392 | 5148 | 212 |
| ★ `static const int CARLA_ANNOYING_CALLS[2]={23,24}` | 11392 | 5148 | 212 |
| ★ `st` used at the body's field / pt / both | 11524 / 11528 / 11524 | 6234 / 6265 / 6230 | 295 / 285 / 297 |
| ★ remove the `hints` local (use `Globals::hints[...]`) | 11440 | 52 | 3 |
| ★ remove `hints` local **and** `st` | 11392 | 5146 | 214 |
| ★ `LevelScript *ls = this->levelScript` used across the block | 11476 | 6210 | 318 |

Key facts:

* Every shape that keeps `st` in the ccm block reproduces the baseline's 41 B
  (the ccm load is GVN'd into `st`).
* Every shape that removes the cached `st` gives the four fresh loads but
  re-colours `&Globals::status` to r5 (11392 / 5148).
* The fence variant (`fence_pt`) is the closest structural match: it keeps r9
  and gives four fresh loads, but it adds one `ldr r5,[r9]` (st) and one
  `mov r0,r5` (pt): 4 loads + 1 mov, 2 B longer than the original's 4 loads +
  0 movs, which shifts every later branch offset (36 runs).
* `hints`-local removal is the only other shape with the base still in r9
  (`outside == 0` in wave-96 terms), but it adds 11 B in the hint block.
  Removing `hints` *and* `st` still re-colours to r5.

## 1.6 Next steps

1. Find the missing r5 occupant. The most promising remaining lever is a
   source value that is live across `0x17e08c..0x17e846` but not used in
   `0x17e044..0x17e12a`. Candidates to try: a cached `this->levelScript` or
   `this->hud` whose *use* is in the mission body or the hint blocks (the
   plain `LevelScript *ls` variant re-colours everything, so it needs to be a
   value the original already keeps in a register, not a new local).
2. Chase `_CARLA_ANNOYING_CALLS`: recover its definition from the iOS 1.1.19
   layout (it lives in a shared header) and check whether the original's
   mission-23/24 source used it in a non-foldable way. A non-const array load
   would change the block's IR enough to perturb RA.
3. The faithful oracle is clang `-Oz` on the emitted `.ll` (not `llc -O2`);
   `-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5MGame8OnUpdateEv`
   is the cheap way to see the split (7 s, 1.1 M lines).

---

# Part 2 — `ModStation` ctor (39 B / 8 runs)

## 2.1 Baseline residual (recap)

`C1`/`C2` alias, 1008 B @ 0xd52d0. Only the near-camera rotation block and the
two literal-pool slots differ:

```
+0x169 (1B)  orig=4b ours=4c        ; ldr r3 vs r4 (YawIPad vs Yaw)
+0x16b (1B)  orig=4c ours=4b        ; ldr r4 vs r3
+0x16e..+0x179 (12B)                ; yaw load / race spill / pitch copy order
+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18c (5B)                 ; schedule
+0x18e..+0x19e (17B)                ; schedule
+0x3e4 (1B)  orig=16 ours=34        ; pool slot
+0x3e8 (1B)  orig=38 ours=12        ; pool slot
```

Two independent backend choices (wave-97 §3-6):

1. **LDRLIT emission order.** Original: `Pitch, YawIPad, Yaw` (the `.rodata`
   order). Ours: `Pitch, Yaw, YawIPad`. Cause: `SimplifyCFG` if-converts the
   ternary to `select %ne, YawIPad, Yaw`, then `InstCombineSelect`
   canonicalises a one-use non-canonical `icmp ne` to `icmp eq` and swaps the
   operands (`select %eq, Yaw, YawIPad`). ISel visits the select operands
   (cond, true, false), so it emits `Yaw` first.
2. **Post-RA schedule** of the yaw load, the `race` spill and the `pitchBits`
   copy (the `+0x186..+0x19e` runs).

## 2.2 New experiments this pass

All measured with `fdiff.py _ZN10ModStationC1Ev 0x3e4 0x3e8`.

### 2.2.1 Dead second use of the `ipad` icmp (to block canonicalisation)

The InstCombine rule requires `m_OneUse(icmp)`. Tried adding dead/used second
selects on a local `bool ipad = Globals::iPad;`:

| variant | bytes | runs | pool order | `.rodata` |
|---|---:|---:|---|---|
| baseline | 39 | 8 | Pitch,Yaw,YawIPad | Pitch,YawIPad,Yaw |
| `bool ipad` + dead `ipad ? Pitch : Yaw` | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `bool ipad` + dead `ipad ? YawIPad : Pitch` | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `bool ipad` + dead `int unused = ipad ? 1 : 2` | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `bool ipad` + `yawTableB = !ipad ? Yaw : YawIPad` | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `bool ipad` + `yawTableB = ipad ? YawIPad : Yaw` | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `bool ipad` + `const void *` second select | 39 | 8 | Pitch,Yaw,YawIPad | ok |

The dead selects are eliminated before InstCombine, so the final IR still has
`%99 = icmp eq i8 %98, 0` with one use (`%100 = select %99, Yaw, YawIPad`).
**Dead second uses do not work.**

### 2.2.2 Non-canonical predicate forms

| near-yaw expression | bytes | runs | pool | `.rodata` |
|---|---:|---:|---|---|
| `iPad ? YawIPad : Yaw` (baseline) | 39 | 8 | Pitch,Yaw,YawIPad | ok |
| `iPad ? Yaw : YawIPad` (flip) | 39 | 8 | Pitch,YawIPad,Yaw | **swap** |
| `!iPad ? YawIPad : Yaw` | 1000 | 691/91 | **Pitch,YawIPad,Yaw** | **ok** |
| `!iPad ? Yaw : YawIPad` | 39 | 8 | Pitch,Yaw,YawIPad | **swap** |
| `iPad == 0 ? YawIPad : Yaw` | 1000 | 691/91 | **Pitch,YawIPad,Yaw** | **ok** |
| `iPad != 0 ? YawIPad : Yaw` | 39 | 8 | Pitch,Yaw,YawIPad | ok |

**Important new result:** `!iPad ? YawIPad : Yaw` (equivalently
`iPad == 0 ? YawIPad : Yaw`) gives **both** the correct LDRLIT order
(`Pitch, YawIPad, Yaw`) *and* the correct `.rodata` order
(`Pitch, YawIPad, Yaw`). This is the shape the wave-97 note said was needed
but not found. However, changing only the near yaw perturbs the **far** block:
the near/far selects become complementary, the compiler re-derives the far
yaw from the near one, the far block re-colours (r6 instead of r5) and the
function shrinks to 1000 B (692 diff / 91 runs). So it is not a strict
improvement yet.

### 2.2.3 Far-block `!iPad` form (the wave-97 "next lever")

Changing **both** the near and far yaw ternaries to `!iPad ? YawIPad : Yaw`
(`both_not`) restores the 1008 B size and the 39/8 residual, and the near
block's LDRLIT order stays correct (`Pitch, YawIPad, Yaw`). But the near
block's *move* is still the wrong way round:

```
orig  near: ldr r3,YawIPad ; ldr r4,Yaw ; mov r0,r3 ; it eq ; moveq r0,r4
both_not  : ldr r4,YawIPad ; ldr r3,Yaw ; mov r0,r3 ; it eq ; moveq r0,r4
```

The original's select is `%ne, T=YawIPad, F=Yaw` (default = T, conditional
(EQ) = F); `both_not` is `%eq, T=YawIPad, F=Yaw` (default = F, conditional
(EQ) = T). Both give the same pool order, so this is a pure predicate
difference. The full 4x4 near/far form table is: `A/A`, `A/D`, `B/B`, `B/C`,
`C/B`, `C/C`, `D/A`, `D/D` all give **39/8** (1008 B); the complementary
pairs (`A/B`, `A/C`, `B/A`, `B/D`, `C/A`, `C/D`, `D/B`, `D/C`) shrink to
1000 B and 691/91. No combination reaches 0.

### 2.2.4 Forcing the `%ne` select with a shared icmp

`InstCombineSelect` only canonicalises a **one-use** `icmp ne`; a multi-use
icmp keeps `%ne` and therefore the original move order. Sharing the
`Globals::iPad` comparison between the near-yaw select and another select does
produce `%ne` (verified in the MIR: `cmp.w sl,#0 ; it ne ; movne`), but the
shared value is then live across `MatrixSetRotation`, so the register
allocator keeps it in the callee-saved `sl` and the whole function re-colours:

| shared with | bytes | diff | runs |
|---|---:|---:|---:|
| nothing (`bool`/`int`/`char` local, near only) | 1008 | 39 | 8 |
| far **pos** select | 1000 | 684 | 47 |
| far **yaw** select | 984 | 580 | 72 |
| far pos + far yaw | 968 | 684 | 59 |
| camKey pos select (wave-97) | 1008 | 562 | 98 |

The type of the local (`bool`, `char`, `unsigned`, `int`) makes no
difference. The load is not rematerialised: the RA holds the value in `sl`
across the call. The original instead reloads `Globals::iPad` from the GOT
pointer (`ldrb.w r1,[sl]`, where `sl` is `&Globals::iPad`) in *each* block, so
its IR must have separate loads/icmps -- which are one-use and canonicalise.
The `%ne` shape and the separate-load shape therefore seem mutually
exclusive in the front end; the wave-92 pass-5 hand-edited IR (`best16d`,
11/4) shows the backend *can* emit the right order with a `%eq` select, but
that IR instruction order is not reachable from C++.

## 2.3 Next steps

1. The remaining lever is the wave-92 `best16d` IR instruction order (select
   before the pitch bitcast/load, yaw gep after the pitch load) combined with
   a front-end shape that produces it. The wave-98 `!iPad` result shows the
   pool order is reachable from source; the missing piece is the move/register
   order, which needs either a multi-use icmp without cross-call liveness or
   the `best16d` DAG order.
2. The post-RA schedule (the `+0x186..+0x19e` runs) is independent and still
   needs a source shape that makes the yaw load precede the `race` spill and
   the pitch copy. Wave-97 §6 covers the scheduler details; the store-before-
   load order is self-reinforcing (`killed $r8` in the input MIR).

---

# Reproduction

```
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4820/fdiff.py _ZN10ModStationC1Ev 0x3e4 0x3e8
python3 /dev/shm/w4820/fdiff.py _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
```

Variant drivers in `/dev/shm/w4820`:
* `try_mgame.py <block>` / `batch10..16.py` — MGame condition sweeps.
* `ms_order.py` — ModStation near-yaw expression sweep (prints pool + `.rodata`
  order from the emitted `.s`).
* `ms_batch.py` / `ms_batch2.py` — ModStation dead-select sweeps.
* `cc_greedy.sh` / `cc_ir.sh` / `cc_ll.sh` — faithful `-Oz` `.s` / `.ll` /
  `-print-after-all` dumps for one TU.
