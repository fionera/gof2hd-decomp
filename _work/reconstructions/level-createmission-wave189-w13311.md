# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13311

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13311`, base master `ad42660c5` (wave-188 harvest 4,
raw **3,444**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: branch-layout / EH-LSDA pass on top of wave-188. **No size-exact raw win
was found; `src/` is left at the base.** This note records the evidence and the
measured non-wins so the next worker does not repeat them.

## 1. Result

```text
                                     size            raw    aligned  insn
base ad42660c5                       17172/17172     3444   1021     6206/6197
```

Sections (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,047
.ARM.extab   size   156,312 /   156,312   differing bytes    639  (base exact @ 0x1a3b78)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id  36 B, 20 differing bytes
```

Total differing bytes **74,706** (`74047 + 639 + 20`), unchanged.

Gate on the base tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact,
`RATCHET PASS`. No baseline update.

## 2. Mission-type TBH table (authoritative targets)

The dispatch table is the TBH at VMA `0xb02a8`; its halfword table starts at
`0xb02ac` (base = `0xb02ac`). 15 entries, mission types 0..14:

```text
orig entries:  0903 0a14 0012 0b8a 0012 0baa 0c3a 000f 0cf8 0d86 000f 0f3e 000f 000f 105e
ours entries:  0905 0a13 0012 0b87 0012 0ba8 0c38 000f 0cf6 0d84 000f 0f3a 000f 000f 105a

orig targets (fn offsets): bd7 ce8 2e6 e5e 2e6 e7e f0e 2e3 fcc 105a 2e3 1212 2e3 2e3 1332
ours targets (fn offsets): bd9 ce7 2e6 e5b 2e6 e7c f0c 2e3 fca 1058 2e3 120e 2e3 2e3 132e
```

`ours - orig`: `+2, -1, 0, -3, 0, -2, -2, 0, -2, -2, 0, -4, 0, 0, -4`. The
case bodies are individually 1–4 B shorter in ours and the deficits accumulate;
the function is size-exact overall, so the missing bytes are made up elsewhere
(mostly the case-183 body). This is why the TBH targets cannot be fixed one at a
time without a co-designed multi-case pass.

## 3. Case-183 convoy `switch (k)` — the 52 B vs 48 B shape

Original VMA `0xb1418`–`0xb144e` (function offsets `0x1440`–`0x1476`). It is a
**compare chain** (not a jump table) and every arm ends in a branch to one shared
`str r1,[sp,#380]`:

```text
b1418 beq  case2            ; dispatch order 2, 1, 0/default
b141a cmp  r1,#1
b141c beq  case1
b141e cbnz r1, default
b1420 case0: ldr r1,(b15bc)=-3000 ; str r1,#376 ; ldr r1,(b15c0)=20000 ; str r1,#372 ;
             ldr r1,(b15c4)=-50000 ; b b144e
b142c case1: ldr r1,(b15c8)=-7000 ; str r1,#376 ; ldr r1,(b15cc)=10000 ; str r1,#372 ;
             ldr r1,(b15d0)=-30000 ; b b144e
b1438 case2: ldr r1,(b15d4)=25000 ; str.w sl,#376 ; str r1,#372 ;
             ldr r1,(b15d8)=-36000 ; b b144e
b1444 default: ldr r1,(b15dc)=6000 ; str r1,#376 ; ldr r1,(b15e0)=12000 ; str r1,#372 ;
             ldr r1,(b15e4)=-45000
b144e shared: str r1,[sp,#380]
```

So the original's per-arm store order is `o1,o0,o2` (case2 is `o0,o1,o2`), with
`o2` last everywhere, and clang tail-merged the four `str r1,[sp,#380]` into one
shared store (48 B total: 12+12+12+10+2). Original literal pool at `0xb15bc`:

```text
-3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000, 6000, 12000, -45000
```

Original semantic values: case0 `{20000,-3000,-50000}`, case1
`{10000,-7000,-30000}`, case2 `{25000,0,-36000}`, default `{12000,6000,-45000}`.

Our base is 52 B: case0 12 B (shares the tail with case2), case2 14 B, case1
14 B, default 12 B. The landed wave-188 body uses a **value permutation**
(case1 `{25000,-3000,-36000}`, case2 `{10000,-7000,-30000}`) and non-uniform
store orders (case1 `o1,o2,o0`; default `o0,o2,o1`) to keep clang on the
compare-chain form.

### 3a. Why the original 48 B shape cannot be reproduced directly

With **all four arms `o2`-last** clang's LICM hoists the four `o2` constants out
of the convoy loop into the preheader (`vldr s16/s18/s20/s22` at `0xb13a8`-
`0xb13ba`) and each arm becomes `vmov.f32 s0,sN` + `vstr s0,[sp,#380]`; the
shared store disappears and the function grows. With a *uniform* arm shape
clang instead materialises a stack lookup table and the function shrinks. Both
are size-fatal:

```text
variant (all real link_so + tools/fn_diff.py)
  all arms o2-last, original values                 17188  raw 11291  (LICM vldr form)
  all arms o2-last, current (permuted) values       17164  raw 11283  (stack-table form)
  float o2; assigned in arms + store after switch   17188 / 17164 (same two forms)
  ((int*)offset)[2] = bit pattern in each arm       17164  raw 11114
  if/else-if chain instead of switch                17188  raw 11291
  offset[] declared outside the for loop            same as above
  Vector offset; offset.x/.y/.z, original order     17192  raw 11445
  Vector offset; offset.x/.y/.z, current order      17176  raw 10910
  case1 o2-last only (others base)                  17172  raw 3477
  default o2-last only (others base)                17172  raw 3481
  case1 + default o2-last (base case0/case2)        17188  raw 11294
```

The 48 B shape needs the `o2` literal loads to stay **inside** each arm (GPR
`ldr r1,[pc,...]`, not FP registers) *and* the shared tail to survive. In the
original that happens; with our body clang 7 either LICMs the loads (4 arms
`o2`-last) or keeps the 52 B inline form (≤3 arms `o2`-last). The 48 B switch
alone would make the function 17,168 B, so it must be co-designed with the
case1/case2/case4 bodies (see §2).

## 4. Other measured structural differences (all non-wins)

### 4a. `raid` cold-block order (function offsets `0x4da`–`0x4ee`)

Original places the `raid = false` block (`0x4e6`: `movs r0,#0; str r0,[sp,#84];
b 0x4da`) **before** the `security == 0 -> 90` block (`0x4ec`); ours reverses
them. Source is `if (introSystem && !hardCoreMode()) raid = false; else raid =
raidRoll < (security == 0 ? 90 : security == 1 ? 65 : security == 2 ? 35 : 10);`.

```text
threshold-first rewrite        17204  raw 16258  (size-fatal)
negated condition              17172  raw 3444   (identical codegen)
inverted if/else               17172  raw 3444   (identical codegen)
ternary security==1 first      17172  raw 3444   (identical codegen)
ternary security==2 first      17172  raw 3448
ternary 0;2;1 order            17172  raw 3448
ternary without 35 arm         17196  raw 16162  (size-fatal)
```

### 4b. `nameLeader` block always returns (function offsets `0x40b2`–`0x40da`)

Original has `b.w b02ca` after the `setName(...)` at `0x40da`; the `case4Else:`
block at `0x40de` is therefore only reachable via `goto case4Else` (line 2527).
Our body falls through into `case4Else` and carries an extra `ldr r5,[sp,#108]`
at `0x40da` instead. Adding the explicit `return;`:

```text
if (nameLeader) {...} else {return;} return;   17164  raw 10964  (size-fatal)
if (!nameLeader) return; setName(...);         17172  raw 3444   (identical codegen)
```

### 4c. Case-9 `setPosition` block (function offsets `0x36d8`–`0x3748`)

Original calls the five `setPosition`s in **ascending** index order
`[attackerCount], [+1], [+2], [+3], [+4]`; our source has `[+0],[+1],[+3],[+2],
[+4]` (lines 2389–2393). Swapping `+2`/`+3` makes the instruction stream match,
but the literal pool base is 12 B earlier in ours (`0xb3a8c` vs `0xb3a98`), so
the `ldr [pc,#imm]` immediates still differ and the net raw count rises:

```text
swap +2/+3 (ascending)                         17172  raw 3450
swap +2/+3 + case2 setAlwaysEnemy reorder      17172  raw 3458
```

### 4d. Case-2 convoy loop call order (function offsets `0x1862`–`0x189e`)

Original calls `player->setAlwaysEnemy(true)` **before** `setRoute(clone())` /
`getRoute()->reachWaypoint(0)`; ours calls it last. Reordering the source does
not move the scheduled call (clang re-schedules), and every variant is worse:

```text
setAlwaysEnemy before setRoute                 17172  raw 3452
setAlwaysEnemy between setRoute and reach      17172  raw 3452
setAlwaysEnemy before setPosition              17172  raw 3461
setAlwaysEnemy after offset += 2000            17172  raw 3444  (identical)
drop KIPlayer *ship local                      17172  raw 3444  (identical)
ship-> variants                                17140  raw 11303 (size-fatal)
```

### 4e. case4Tail prologue (`0x40da`ff)

```text
Status *status = Globals::status local         17172  raw 3444  (identical)
hunters computed before opt44                  17172  raw 3447
new Array between opt44 and hunters            17172  raw 3457
```

## 5. Residual byte clusters (function offsets, base build)

```text
0000-1440     28
1440-1a24   1176   case-183/1/2 bodies (switch + TBH-target deltas)
1a24-2ac7      9
2ac7-2d2a     17
2d2a-3256      1
3256-350f    427   stationPirates/extraAliens loops (register allocation)
350f-36d8      3
36d8-3e24   1460   setPosition chain + raid/alien blocks (register allocation)
3e24-40da      5
40da-41f8    238   case4Else/case4Tail prologue (register allocation)
41f8-4202      1
4202-429c     76   case4Tail loop
429c-4314      3
total       3444
```

The two big clusters (`0x1440-0x1a24`, `0x36d8-0x3e24`) are dominated by
whole-function register-allocation choices (`sl` vs `r8` for `this`, `r5` vs
`r6`/`r8` for `Globals::status`, spilled vs register-held `KIPlayer *ship`) and
by the ±1–4 B case-body sizes. They are not reachable by local statement
reordering.

## 6. EH / LSDA

`.ARM.exidx` entry for `0xaffd8` is at index 1614 (`0x19fe78`) and points to
`.ARM.extab` `0x1a9ac0` in both binaries. `.ARM.extab` is size- and base-exact
(156,312 B @ `0x1a3b78`); its content differs in 639 bytes, first at byte 23
(`a6 07 00 e4` vs `a5 07 00 e4`), 125 differing bytes in the first 1,100. The LSDA scope counts
differ by one call-site entry, which is a consequence of the ±4/∓2/∓4 case-body
size deltas in §2, not independently fixable without the co-designed case pass.

## 7. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13311
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3444 aligned=1021 size=17172/17172`, `.text` diff 74,047,
`.ARM.extab` diff 639, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13311/`
(`measure.sh`, `side.py`, `brdiff2.py`, `diffblocks.py`, `regiondiff.py`,
`secdiff.py`, `dump_sw.py`, `exp1.py`..`exp19.py`, `Level.base.cpp`).
