# w7756 (wave-134): `Level::createMission` — tail-loop argument-order reconstruction (`.text` 85143 -> 85141)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7756`, based on master `ec0acea1e`.
Continues `level-createmission-wave133-w7733.md`, `…-wave132-w7710.md`,
`…-wave130b-w7677.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; linked `.text` `0xaffd8`, 17172 B). Ghidra 12.1.2 headless
(`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile … --procedure
_ZN5Level13createMissionEv`) plus manual Thumb disassembly. Build/flags unchanged
(NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.3 % (report row)   size 17172/17172 B
.text diff_bytes 85143 (whole section)   fn bytediff 5771
.ARM.extab diff 1416   .ARM.exidx/.rel.plt/.rodata/.data/.bss exact
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The wave-133 note quotes `.text` 85244; the actual current master measures
**85143** (matches the wave-133 docs sync commit `ec0acea1e`).  Use 85143.

## New hypothesis: rebuild the `case4Tail` loop statement-by-statement

The Ghidra decompile of the `case 4` tail is:

```c
for (uVar50 = 0; uVar50 < **(uint **)(this + 0xf8); uVar50 = uVar50 + 1) {
  iVar11 = Globals::getRandomEnemyFighter(Globals::globals,8);
  pAVar8 = Globals::rnd;
  pRVar21 = *(Route **)(this + 0x108);
  iVar14 = Route::length(pRVar21);
  iVar14 = AbyssEngine::AERandom::nextInt(pAVar8,iVar14);
  pWVar29 = (Waypoint *)Route::getWaypoint(pRVar21,iVar14);
  uVar12 = createShip(this,8,0,iVar11,pWVar29,true,false);
  *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar50 * 4) = uVar12;
  KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar50 * 4));
}
```

The original Thumb loop (`b416e..b41c8`) evaluates the waypoint index **before**
the `createShip` call and keeps `&Globals::rnd` in a stack slot, which frees
`fp` for the literal `1` (the `hostile` argument):

```text
b415c: ldr   r0, [pc,#392]      ; GOT Globals::globals
b415e: mov.w fp, #1             ; hostile = true, hoisted
b4162: mov.w r9, #0             ; i = 0
b4166: add   r0, pc
b4168: ldr.w sl, [r0]           ; sl = Globals::globals
b416c: b.n   b41c8
b416e: ldr.w r0, [sl]           ; Globals::globals
b4172: movs  r1, #8
b4174: blx   getRandomEnemyFighter
b4178: mov   r5, r0             ; type
b417a: ldr   r0, [sp,#120]      ; &Globals::rnd (spilled)
b417c: ldr.w r4, [r8,#264]      ; this->playerRoute
b4180: ldr   r6, [r0,#0]        ; rnd
b4182: mov   r0, r4
b4184: blx   Route::length
b4188: mov   r1, r0
b418a: mov   r0, r6
b418c: blx   AERandom::nextInt
b4190: mov   r1, r0
b4192: mov   r0, r4
b4194: blx   Route::getWaypoint
b4198: strd  r0, fp, [sp]       ; wp, true
b419c: movs  r0, #0
b419e: str   r0, [sp,#8]        ; false
...
```

The checked-in one-liner

```cpp
(*this->enemies)[i] = this->createShip(
        8, 0, Globals::globals->getRandomEnemyFighter(8),
        this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
```

lets clang evaluate `Globals::globals->getRandomEnemyFighter(8)` first and load
`this->playerRoute` before `Globals::rnd`; it then keeps `&Globals::rnd` in
`fp` and materialises `true` with `movs r0,#1; str r0,[sp,#4]` every iteration
(`str r0,[sp]` instead of `strd r0,fp,[sp]`).

**Landed change:** hoist the waypoint index into a statement-local `int idx`
before the `createShip` statement:

```cpp
int idx = Globals::rnd->nextInt(this->playerRoute->length());
(*this->enemies)[i] = this->createShip(
        8, 0, Globals::globals->getRandomEnemyFighter(8),
        this->playerRoute->getWaypoint(idx), true, false);
```

This reproduces the original's `&rnd` -> `playerRoute` -> `rnd` load order and
makes clang hoist `true` into `fp` (`mov.w fp,#1` + `strd r0,fp,[sp]`).  The
emitted loop is now:

```text
b414e: mov.w fp, #1             ; hostile = true, hoisted
b416e: ldr.w r0, [r9,#264]      ; this->playerRoute
b4172: ldr.w r5, [sl]           ; rnd
b4176: blx   Route::length
b417a: mov   r1, r0
b417c: mov   r0, r5
b417e: blx   AERandom::nextInt
b4182: mov   r5, r0             ; idx
b4184: ldr   r0, [r4,#0]        ; Globals::globals
b4186: movs  r1, #8
b4188: blx   getRandomEnemyFighter
b418c: mov   r6, r0
b418e: ldr.w r0, [r9,#264]
b4192: mov   r1, r5
b4194: blx   Route::getWaypoint
b4198: strd  r0, fp, [sp]       ; wp, true
b419c: movs  r0, #0
b419e: str   r0, [sp,#8]        ; false
```

`fp=1` and `strd` now match the original; `this` moved from `sl` to `r9`,
`Globals::globals` from `r8` to `r4`, and `&Globals::rnd` from `fp` to `sl`.

## Result

```text
.text diff  85143 -> 85141   (fn bytediff 5771 -> 5769)
fn size     17172 -> 17172
report pct  92.3  -> 92.3
```

## Variants measured this session (fresh `link_so` each)

| variant | `.text` | fn size | fn bytediff |
|---|---:|---:|---:|
| **baseline** | **85143** | **17172** | **5771** |
| **`int idx` before createShip (landed)** | **85141** | **17172** | **5769** |
| `const int idx` / one-line `int idx` | 85141 | 17172 | 5769 |
| `unsigned idx` | 85141 | 17172 | 5769 |
| `AERandom **rndp = &Globals::rnd; (*rndp)->nextInt(...)` | 85141 | 17172 | 5769 |
| `Level *self = this;` at tail top / mid (with `idx`) | 85141 | 17172 | 5769 |
| `int type = getRandomEnemyFighter(8); int idx = ...` | 85260 | 17172 | 5888 |
| `int type; int idx; Waypoint *wp` | 85260 | 17172 | 5888 |
| `Route *route = this->playerRoute; int idx = ...` | 85184 | 17172 | 5812 |
| `Waypoint *wp = ...getWaypoint(idx)` | 85245 | 17172 | 5873 |
| `AERandom *rnd = Globals::rnd` (value) | 85267 | 17172 | 5895 |
| `Globals *globals = Globals::globals` local | 85187 | 17172 | 5815 |
| faithful `this->enemies->size()` loop (no `volatile n`) | 832252 | 17164 | 5905 |
| faithful loop + `volatile unsigned n` padding variants | 85149–85155 | 17172 | 5777–5783 |
| faithful loop + `Level *volatile self` in `case4Else` | 85155 | 17172 | 5783 |
| `case 4` as plain if/else (remove `case4Else` label) | 875961 | 17180 | 5957 |
| `int` literals `1, 0` instead of `true, false` | 85143 | 17172 | 5771 |
| case-2 convoy `cargo`/`crateCaptured` swap | 85144 | 17172 | 5772 |
| case-183 `shipGroup`/`field_0x74` swap (both) | 85152 | 17172 | 5780 |
| case-12 loop `idx` temp (same pattern) | 835636 | 17164 | 9289 |

The case-12 loop also uses `getRandomEnemyFighter(8)`, but adding the same `idx`
temp there shrinks the function to 17164 (the register allocator drops the
`case4Else` reload), so it is not landed.

## Remaining tail residual (backend)

1. `this` in `r9` (ours) vs `r8` (original), `Globals::globals` in `r4` vs
   `sl`, `&Globals::rnd` in `sl` vs `[sp,#120]` — one register-allocation
   permutation away from the original tail.
2. The original evaluates `getRandomEnemyFighter(8)` **before** the waypoint
   index; the `int idx` temp necessarily evaluates the index first.  Making
   `type` a temp restores the order but loses the `fp=1`/`strd` hoist (85260).
3. `case4Tail` loop condition: original re-reads `this->enemies->size()` every
   iteration; the size-exact formulation still needs the `volatile unsigned n`
   latch (faithful loop is 17164 B).
4. The known wave-131/132/133 residuals are unchanged: alien-turret `cbz`
   select, `pirateBase` register choice, `start` arithmetic, branchless `&&`,
   `raid`/`Globals::status` spill-slot swap, case-183 body layout.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7756/measure.py       # .text diff + fn size + fn bytediff
python3 /dev/shm/w7756/diff.py          # per-block instruction diff
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Ghidra headless:

```text
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN5Level13createMissionEv --format md
```

Scratch: `/dev/shm/w7756/{Level.base.cpp,measure.py,diff.py,batch.py,batch1..10.json,
ours_fn.asm,ours_y3.asm,gh_fn.c}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85143 -> 85141
.ARM.extab diff 1416   -> 1416
.rel.plt        exact  -> exact
```
