# w6751: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# angle D: raid/station/extraAliens statement order, turret `this` reload placement,
# case-183/case4Tail order, `guns` materialisation; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6751`, branch `wave119-w6751`,
based on current master (`d2a5ce778`). Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .text 0x1337c8 both,
     .rodata 0x41c27 (orig 0x41b67; +192 B mission-case table),
     .ARM.extab 0x26294 (orig 0x26298; 4 B short).
```

`check` was run before and after this pass; the worktree is left with only this
note (`src/game/world/Level.cpp` is unchanged).

## 1. Result

No source variant beat the baseline. Best measured score remains
**92.293729 % / 17172 B / 6065 insns**. This note is the only commit.

## 2. New findings this pass

### 2.1 The swapped 4-byte slot is `raid` vs the cached `&__stack_chk_guard`

The first mismatch (insn 28) is:

```text
orig  b0024  str.w fp, [sp, #88]     ; fp = &__stack_chk_guard (prologue)
ours  b0024  str.w fp, [sp, #84]
```

`fp` is loaded at `afff0` from the literal that points at `__stack_chk_guard`
and is only re-read once (`b02ca`, epilogue). `raid` lives in the *other*
4-byte slot (`[sp,#84]` orig / `[sp,#88]` ours, 17 refs). So the SSC renumbering
is between `raid` (17 refs) and the guard-address spill (2 refs), **not**
`Globals::rnd` (which is at `sp+120` in both). The two slots are otherwise
interchangeable 4-byte spills; every source spelling of `raid` tried this pass
(`bool`/`int`/`unsigned`/`char`/`short`, `=false`, expression form, declaration
moved to function top) either keeps the swap or changes the code (see §3).

### 2.2 Alien-turret `this`: one reload in the original, two in ours

The original reloads `this` **once before the branch** and reuses it for both
the `base` computation and the loop body:

```text
orig  b322e  ldr.w sl,[sp,#52]      ; alienGuns
      b3232  ldr r4,[sp,#124]       ; this   <- before `cmp`
      b3234  cmp.w sl,#0
      b3238  beq.w b3398
      b3274  ldr.w r0,[r4,#248]     ; base: this->enemies
      ...
      b32da  add r4,sp,#312         ; &rotation clobbers r4 (this dead here)
      b336e  ldr r4,[sp,#124]       ; this reloaded for the loop bound
```

Ours has **two** reloads of the same value in the preheader/body:

```text
ours  b3222  ldr r6,[sp,#52]
      b3224  cmp r6,#0
      b3226  beq.w b3394
      b3260  ldr r4,[sp,#124]       ; body (createStaticObject / gun load)
      b3280  ldr r5,[sp,#124]       ; preheader (loop bound)
```

MIR (`-mllvm -print-after-all`, VRR dump) confirms two distinct defs:
`%4907 = t2LDRi12 %stack.75` in bb.249 and `%4908 = t2LDRi12 %stack.75` in
bb.252. `%4908` is used at the top of bb.252 **and again at the bottom**
(`%4851/%4850/%4849 = t2LDRi12 %4908, 352/356/360`), so its live range spans
the whole body and cannot be clobbered by `&rotation`; the allocator therefore
gives `gun` r4 and `this` r5. In the original the value is split at the
`&rotation` clobber, which is why `this` stays in r4 and `gun` takes r6.
This is a PEI reload-placement difference, not a source live-range difference:
the source shape is already the faithful one.

The `pirateFortress` loop is *not* affected — original and ours both use
`this`=r5, `gun`=r4 there (only the register names in the preceding
`kBaseTurrets` preheader differ).

### 2.3 Raid preheader: ours hoists `guns + friendCount`

```text
orig  b3752  ldr.w r8,[sp,#108]   ; status
      b3756  ldr r3,[sp,#84]      ; raid
      b3758  ldr.w fp,[sp,#40]    ; fortressGuns
      b375c  cmp r3,#0
      b375e  beq.w b396c
      ...
      b37e6  add.w r5,sl,fp       ; guns = alienGuns + fortressGuns (after branch)
ours  b3752  ldr r4,[sp,#48]      ; guns
      b3754  ldr r3,[sp,#88]      ; raid
      b3756  add r4,r6            ; guns + friendCount
      b3758  str r4,[sp,#112]
      b375a  cmp r3,#0
```

The original keeps `alienGuns` and `fortressGuns` in separate slots
(`sl`/`fp`) and only adds them at the loop bound; ours materialises a single
`guns` slot (`sp+48`) and CSEs `guns + friendCount` into `sp+112`. Removing the
`int guns` declaration entirely (inlining `alienGuns + fortressGuns` at every
use) is *worse*: 88.822899 %, 17200 B. Moving the declaration below the raid
block gives the same 17200 B. So the original does have a `guns` value, but the
allocator never gave it a slot.

### 2.4 `case4Tail` loop

```text
orig  b41c8  ldr.w r0,[r8,#248]   ; this->enemies
      b41cc  ldr r0,[r0,#0]       ; ->size() reloaded every iteration
      b41ce  cmp r9,r0
      b41d0  bcc.n b416e
ours  b41c8  ldr r0,[sp,#312]     ; volatile n latch
      b41ca  cmp r9,r0
      b41cc  bcc.n b416c
```

The original also passes the non-null pointer `Globals::rnd` (held in `fp`) as
the `true` argument of `createShip` (`strd r0,fp,[sp]`), whereas ours
materialises `movs r0,#1; str r0,[sp,#4]`.  The `volatile unsigned n` latch is
still the best-scoring form (removing it -> 91.308296 %, 17164 B; `unsigned`
non-volatile -> 92.145215 %).

### 2.5 `&&` at line 1904 is materialised as `orrs`, not short-circuited

```text
orig  b2e04  subs.w r0,fp,r3      ; i - (friendCount+field_164)
      b2e08  mov r1,r0
      b2e0a  it ne
      b2e0c  movne r1,#1
      b2e0e  ldr r2,[sp,#60]      ; alienWave
      b2e10  eor.w r2,r2,#1
      b2e14  orrs r1,r2
      b2e16  bne.w b2f2a
ours  b2e04  ldr r0,[sp,#60]
      b2e06  cmp r0,#0
      b2e08  beq.w b2f20
      b2e0c  cmp fp,r3
      b2e0e  bne.w b2f20
```

The original is 3 instructions *longer* here.  De Morgan / `&` / named-bool
spellings were already measured neutral or worse in w6711/w6733; this pass
re-confirmed that no source form flips it.

## 3. Variants measured this pass (official verifier)

Baseline is `92.293729 / 17172`. Every variant keeps 27 sections, file size
2,187,328 B and `.text` 0x1337c8 unless noted.

### 3.1 Turret body / selector / declarations

| variant | official % | size |
|---|---|---|
| **baseline** | **92.293729** | 17172 |
| `k++, i++` instead of `i++, k++` | 92.293729 | 17172 |
| `Vector v;` instead of `= {0,0,0}` | 92.293729 | 17172 |
| `int k = 0` in the `for` init | 92.293729 | 17172 |
| `int k` before `Vector base` | 92.293729 | 17172 |
| `PlayerTurret *gun = (PlayerTurret *) (*enemies)[i]` | 92.293729 | 17172 |
| `Level *self = this;` (unused / bound / body / both) | 92.293729 | 17172 |
| `const float (*t)[6]; if (a) t=A; else t=B;` | 92.293729 | 17172 |
| `((PlayerTurret *) (*enemies)[i])->setScaling` moved last (fresh) | 92.244224 | 17172 |
| `gun->cargo` before `gun->shipGroup` | 92.277228 | 17172 |
| faithful order + cached `gun` (setRotation,setPosition,setScaling,cargo,shipGroup) | 92.160423 | 17172 |
| `Vector base` before `Matrix rotation` | 92.062706 | 17172 |
| `gun->setPosition(base + v)` (Vector operator+) | 88.633735 | 17132 |
| `gun->setPosition(base + v)` (turret only) | 90.678106 | 17152 |
| `Vector vt(t[k][3..5]); v = MatrixRotateVector(rotation, vt);` | 88.652307 | 17164 |
| plain ternary (no `i<0` anchor) | 88.380543 | 17140 |
| `if/else` table assign (no anchor) | 88.380543 | 17140 |
| loop bound via `*jumperCount`/`*bigShipsCount` | 92.293729 | 17172 |
| `for (int k=0;k<alienGuns;k++) { int i=start+k; ... }` | 87.099167 | 17204 |
| `k++` at end of body | 92.293729 | 17172 |
| `unsigned i` loop | 88.380543 | 17140 |

Anchor alternatives (all with the baseline body):

| anchor | official % | size |
|---|---|---|
| `(bigAlienWave + (i < 0))` (baseline) | **92.293729** | 17172 |
| `(bigAlienWave + ((i & 0x80000000) != 0))` | 92.293729 | 17172 |
| `(bigAlienWave + ((i >> 31) & 1))` | 92.293729 | 17172 |
| `(bigAlienWave + (i < 0) * 1)` | 92.293729 | 17172 |
| `(bigAlienWave + (this->field_164 < 0))` | 92.185824 | 17172 |
| `(bigAlienWave + (this->field_168 < 0))` | 92.185824 | 17172 |
| `(bigAlienWave + (localRace < 0))` | 88.391678 | 17148 |
| `(bigAlienWave + (pirates < 0))` | 88.391678 | 17148 |
| `(bigAlienWave + (alienWave < 0))` | 88.380543 | 17140 |
| `(bigAlienWave + (security < 0))` | 79.326288 | 17148 |
| `(bigAlienWave + (raidRoll < 0))` | 60.803689 | 17168 |

The `i<0` anchor is the only one that keeps both the in-loop select and the
17172 B size; every non-`i` anchor changes the size and drops the score.

### 3.2 `guns` / raid block

| variant | official % | size |
|---|---|---|
| **baseline** `int guns = alienGuns + fortressGuns;` | **92.293729** | 17172 |
| `const int guns` / `unsigned alienGuns, fortressGuns` | 92.293729 | 17172 |
| raid loop inlines `alienGuns + fortressGuns` (either order) | 92.293729 | 17172 |
| raid bound via `*jumperCount`/`*bigShipsCount` | 92.293729 | 17172 |
| `bool hard` before `int raidType` | 92.277228 | 17172 |
| `hardCoreMode()` inlined into the `?:` | 91.354562 | 17192 |
| `initStreamOutPosition && raidRace == 8` operand swap | 92.293729 | 17172 |
| `getLevel() + (hard ? 40 : 20)` | 92.293729 | 17172 |
| `raid != false` | 92.293729 | 17172 |
| `int guns` removed, all uses inlined | 88.822899 | 17200 |
| `int guns` moved below the raid block | 88.822899 | 17200 |

### 3.3 case-183 / case4Tail / misc

| variant | official % | size |
|---|---|---|
| **baseline** if-chain `2,1,0` | **92.293729** | 17172 |
| `switch` default-first, cases `2,1,0` | 92.293729 | 17172 |
| `switch` cases `0,2,1` | 92.145215 | 17172 |
| `float offset[3] = {0,0,0};` | 89.158416 | 17176 |
| `volatile unsigned n` -> `unsigned n` (case4Tail) | 92.145215 | 17172 |
| `volatile unsigned n` -> `volatile int n` | 92.293729 | 17172 |
| case4Tail `createShip(..., 1, 0)` | 92.293729 | 17172 |
| `if (alienGuns > 0)` / `if (alienGuns)` | 92.293729 | 17172 |
| `if (pirateFortress != 0)` | 92.293729 | 17172 |
| `getCurrentCampaignMission() > 100 && < 145` | 92.293729 | 17172 |
| `raid && getCurrentCampaignMission() > 31` | 92.293729 | 17172 |
| `bool raid` declared at function top | 91.452145 | 17172 |
| `bigAlienWave`/`alienWave` declaration swap | 90.973597 | 17172 |

## 4. Residual analysis (unchanged)

* **`this` register** r4 (orig) vs r5 (ours) from the alien-turret preheader
  onward; `gun` r6 vs r4; `k*24` r9 vs fp; `i` fp vs r9; `t` sl vs r6.
  Dominant residual (~2.7 points). Root cause: PEI places the `this` reload in
  the *body* (bb.252) instead of the *condition* block, so `%4908` is live
  across the whole body and is not split at the `&rotation` clobber.
* **Stack-slot swap**: `raid` (sp+84 orig / sp+88 ours) vs the cached
  `&__stack_chk_guard` (sp+88 / sp+84); 17 vs 2 refs.
* **Alien-turret selector**: original in-loop `ldr r0,[sp,#64]; cbz`; ours
  `cmp.w r1,r9,asr #31; it ne; movne` (the `i<0` anchor).
* **Raid preheader**: ours CSEs `guns + friendCount` into `sp+112` before the
  `cmp raid,#0`; the original computes `alienGuns + fortressGuns` after it.
* **case-183 convoy chain**: test order `2,1,0` (orig) vs `0,1,2` (ours); the
  2-byte test-chain delta shifts every later `loc_*` target.
* **`&&` at 1904/1907**: original ORs both negated operands, ours
  short-circuits.
* **case4Tail**: original reloads `this->enemies->size()` and reuses the
  non-null `Globals::rnd` pointer as `true`; ours uses the `volatile n` latch
  and materialises `1`.
* `.ARM.extab` still 4 B short (`0x26294` vs `0x26298`); 27 sections and
  2,187,328 B file size unchanged by every variant above.

## 5. Commands

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6751/score.py             # official % / size / insns
python3 /dev/shm/w6751/srcline7.py          # per-cluster census + source lines + addr
cmake --build cmake-build-match --target check -j6
```

Scratch tools (not tracked): `/dev/shm/w6751/{score,var,try_,dis,exp,one_show,
srcline6,srcline7,batch7..batch18}.py`, `Level.base.cpp` (== HEAD),
`Level.ll` (optimized IR), `orig_all.txt` / `ours_all.txt` (full objdump),
`pa1.txt` (`-mllvm -print-after-all`, VRR/SSC/PEI dumps).
