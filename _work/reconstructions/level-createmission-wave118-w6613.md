# w6613: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-118:
# variant sweep around the alien-turret selector and the case-183 if-chain; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6613`, branch `wave118-w6613`,
base `3d4a98a3e`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine, wave-117 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             shape 94.983498 %      nopc 92.970297 %
                             6055 orig / 6065 ours insns
gate: byte 4501/4524, linked 4501/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26294 (4 B short).
```

`shape`/`nopc` use `/dev/shm/w6613/score.py` (register names + `[pc,#N]`
collapsed / only `[pc,#N]` collapsed).

## 1. Result

No source variant beat the baseline. The best measured score remains
**92.293729 % / 17172 B / 6065 insns**. The worktree is left with only this
note; `src/game/world/Level.cpp` is unchanged.

## 2. What the original actually looks like

Re-decompiled with REA/Ghidra 12.1.2 (`rea decompile ... --procedure
_ZN5Level13createMissionEv`) and cross-checked against the Thumb disassembly.
The original source shape is now clear for the two hot regions:

### Alien-turret loop (`src/game/world/Level.cpp:1950-1974`)

```text
b3274  ldr.w r0,[r4,#248]        ; this->enemies          (r4 = this)
b327a  ldr.w r1,[r0,r3,lsl #2]   ; (*enemies)[friendCount+field_164]
b3284  blx   getPosition         ; Vector base
b328a  add   r5,sp,#212          ; r5 = &MatrixRotateVector return slot
b328c  mov.w r9,#0               ; r9 = k*24
b3294  add.w fp,r3,r2            ; fp = i
b329a  mov   r0,r4               ; createStaticObject(nullptr,6772,true)
b32a4  blx   createStaticObject
b32bc  ldr.w r6,[r0,fp,lsl #2]   ; r6 = gun
b32c6  ldr   r0,[sp,#64]         ; bigAlienWave
b32c8  cbz   r0,b32d0            ; plain conditional, IN-LOOP
b32ca  ldr   r1,[pc,#732]        ; kAlienStationTurrets
b32d0  ldr   r1,[pc,#728]        ; kAlienOutpostTurrets
b32d4  add.w r0,r1,r9            ; t = table + k*24
b32d8  mov   sl,r1               ; sl = t
b32da  add   r4,sp,#312          ; r4 = &temp
b32f4  mov   r0,r5               ; v = MatrixRotateVector(...)
b3304  ldr   r0,[r6,#8]          ; gun->geometry
b3308  blx   setRotation
b3320  blx   MatrixRotateVector  ; second rotate
b335e  ldr   r0,[r6,#0]          ; gun->setPosition(v) (virtual, vtable+68)
b3366  ldr   r1,[pc,#584]        ; 6.0f
b3368  mov   r0,r6               ; setScaling on the CACHED gun
b336a  blx   setScaling
b3372  str   r0,[r6,#80]         ; gun->cargo = nullptr
b337e  str   r0,[r6,#40]         ; gun->shipGroup = localRace
```

So the original is the faithful form:

```cpp
KIPlayer *gun = (*this->enemies)[i];
gun->player->setMaxHitpoints(1000);
const float (*t)[6] = bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets;
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
gun->geometry->setRotation(v);
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
((PlayerTurret *) gun)->setScaling(6.0f);
gun->shipGroup = localRace;
gun->cargo = nullptr;
```

The current source differs in two ways, both of which are *needed* for the
17172 B size/layout:

* the selector carries the fake `i < 0` anchor
  (`(bigAlienWave + (i < 0))`), which keeps the select in-loop but emits
  `cmp.w r1,r9,asr #31; it ne; movne` instead of the original `cbz`;
* `setScaling` is issued through a fresh `(*this->enemies)[i]` load *before*
  `setRotation`, not through the cached `gun` after `setPosition`.

### Register allocation (root residual)

The original keeps `this` in **r4** from the loop preheader through the whole
turret loop (`gun`=r6, `&return-slot`=r5, `k*24`=r9, `i`=fp, `t`=sl). Our
build reloads `this` into **r5** at the loop preheader (`gun`=r4,
`&return-slot`=r8, `k*24`=fp, `i`=r9, `t`=r6). The same swap then propagates
through the `pirateFortress` loop and the whole `if (raid)` block (original
r4=this, ours r5=this), which is what makes the 46/23/21/18/15-instruction
clusters at `orig[4581:4634]`, `[5185:5216]`, `[5057:5088]`, `[4645:4671]`,
`[5092:5120]`. Every source reordering tried below either keeps the swap or
loses the 17172 B size.

## 3. Variants measured this wave (official verifier)

All measurements are `official % / size / insns`; baseline is
`92.293729 / 17172 / 6065`.

### 3.1 Alien-turret selector / setScaling

| variant | official % | size | insns |
|---|---|---|---|
| baseline (`i<0` anchor, fresh `(*enemies)[i]` setScaling before setRotation) | **92.293729** | 17172 | 6065 |
| setScaling on cached `gun` (`((PlayerTurret *) gun)->setScaling(6.0f)`) | 90.922594 | 17164 | 6063 |
| plain ternary `bigAlienWave ? … : …` (no anchor) | 88.380543 | 17140 | 6054 |
| plain ternary + cached gun | 88.334847 | 17140 | 6058 |
| faithful order (setScaling after setPosition) + cached gun | 92.176927 | 17172 | 6063 |
| faithful order + fresh `(*enemies)[i]` setScaling | 92.244224 | 17172 | 6065 |
| faithful order + plain ternary | 88.367869 | 17140 | 6058 |
| 1-D `const float *t = (…)[k]` + fresh, current order | 92.159128 | 17172 | 6061 |
| 1-D + fresh, faithful order | 92.192143 | 17172 | 6061 |
| 1-D + cached gun, current order | 90.870068 | 17164 | 6059 |
| 1-D + cached gun, faithful order | 90.915092 | 17180 | 6064 |
| cached gun + anchor `(bigAlienWave + (i<0) + (field_168<0))` | 92.227723 | 17172 | 6065 |
| cached gun + anchor `(… + (hostileCount<0))` / `(… + (field_164<0))` | 92.227723 | 17172 | 6065 |
| cached gun + anchor `(… + (alienGuns<0))` (folds) | 90.922594 | 17164 | 6063 |
| cached gun + `afterrot` order + `field_168<0` anchor | 92.211221 | 17172 | 6065 |
| cached gun + faithful order + `field_168<0` anchor | 90.84908 | 17180 | 6064 |
| named locals `Vector dir/pos` (two locals) | 90.626807 | 17148 | 6054 |
| named local `dir` reused | 88.47424 | 17168 | 6057 |

Anchor alternatives (all with the baseline body):

| anchor | official % | size | insns |
|---|---|---|---|
| `(bigAlienWave + (i<0))` (baseline) | **92.293729** | 17172 | 6065 |
| `(bigAlienWave + (field_168<0))` | 92.185824 | 17172 | 6064 |
| `(bigAlienWave + (hostileCount<0))` | 92.185824 | 17172 | 6064 |
| `(bigAlienWave \| (field_168>>31))` | 92.136315 | 17172 | 6064 |
| `(bigAlienWave \| (hostileCount>>31))` | 92.136315 | 17172 | 6064 |
| `(bigAlienWave \| (field_164>>31))` | 92.136315 | 17172 | 6064 |
| `(bigAlienWave + (field_168&1))` | 92.136315 | 17172 | 6064 |
| `(bigAlienWave \| (alienGuns>>31))` (folds) | 88.380543 | 17140 | 6054 |
| `*(volatile int *)&bigAlienWave` (plain) | 87.749711 | 17140 | 6059 |
| `*(volatile int *)&bigAlienWave + (i<0)` | 87.740444 | 17140 | 6058 |

Loop declaration / induction spellings (all neutral):

| variant | official % | size | insns |
|---|---|---|---|
| `for (int i = …, k = 0; …; i++, k++)` | 92.293729 | 17172 | 6065 |
| `int i = …; int k = 0; for (; …; i++, k++)` | 92.293729 | 17172 | 6065 |
| `for (…; k++, i++)` | 92.293729 | 17172 | 6065 |
| `unsigned k` | 92.293729 | 17172 | 6065 |
| `i != bound` | 92.293729 | 17172 | 6065 |
| `unsigned i` (anchor folds) | 88.380543 | 17140 | 6054 |
| `Vector v;` (no initializer) | 92.293729 | 17172 | 6065 |
| `Vector base` before `Matrix rotation` | 92.128713 | 17172 | 6065 |
| `int k` before `Vector base` | 92.293729 | 17172 | 6065 |
| `if (alienGuns != 0 && this->enemies != nullptr)` | 75.925163 | 17368 | 6078 |

### 3.2 Case-183 convoy if-chain (`Level.cpp:2192-2210`)

Original: tests `2,1,0`; bodies laid out `0,1,2,default`; `cmp r1,#2; beq;
cmp r1,#1; beq; cbnz r1,default; body0`. Ours: tests `0,1,2`; bodies
`2,0,1,default`.

| variant | official % | size | insns |
|---|---|---|---|
| baseline source order `2,1,0` (emits tests 0,1,2) | **92.293729** | 17172 | 6065 |
| source order `0,1,2` (emits tests 2,1,0 but bodies 2,0,1) | 92.145215 | 17172 | 6065 |
| `switch (k) { case 2,1,0, default }` | 92.293729 | 17172 | 6065 |
| `switch (k) { case 0,1,2,3 }` (jump table `tbb`) | 92.21883 | 17172 | 6064 |
| nested `if (k==2) … else { if (k==1) … else { if (k==0) … } }` | 92.293729 | 17172 | 6065 |
| binary tree `if (k<2) { if(k==0)…else… } else { if(k==2)…else… }` | 89.668262 | 17164 | 6063 |

### 3.3 Case4Tail hunter loop (`Level.cpp:2122-2131`)

The original reloads `this->enemies->size()` on every back-edge
(`ldr.w r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc`). Our `volatile
unsigned n` reloads `n` from `sp+312` instead. Removing the latch matches the
loop condition but changes the EH-pad layout:

| variant | official % | size | insns |
|---|---|---|---|
| baseline `volatile unsigned n`, `i < n` | **92.293729** | 17172 | 6065 |
| `for (unsigned i = 0; i < this->enemies->size(); i++)` (no latch) | 91.308296 | 17164 | 6060 |
| latch + `i < this->enemies->size()` | 90.86709 | 17176 | 6066 |
| `volatile int n` | 92.293729 | 17172 | 6065 |
| `volatile unsigned n = hunterCount` | 91.228649 | 17168 | 6064 |
| `Objective(18, 0, n, this)` | 90.627063 | 17172 | 6065 |
| `i < (unsigned) n` | 92.293729 | 17172 | 6065 |

### 3.4 Stack-slot swap (`status` vs `raid`)

First mismatch is instruction 28: original `str.w fp,[sp,#88]` (`status` at
sp+88, `raid` at sp+84); ours `str.w fp,[sp,#84]` (reversed). 17 refs each.
Declaration/initialisation spellings do not move it:

| variant | official % | size | insns |
|---|---|---|---|
| baseline `bool raid;` + if/else | **92.293729** | 17172 | 6065 |
| `bool raid = false;` + if/else | 92.293729 | 17172 | 6065 |
| `bool raid = cond ? false : …;` | 92.293729 | 17172 | 6065 |
| `bool raid; raid = cond ? … : …;` | 92.293729 | 17172 | 6065 |

### 3.5 Misc. type / declaration variants (all neutral)

`int *const bigShipsCount/jumperCount`, `unsigned alienGuns`,
`unsigned fortressGuns`, both unsigned: all `92.293729 / 17172 / 6065`.

## 4. Residual analysis (unchanged)

* **Stack-slot swap** `status` (orig sp+88 / ours sp+84) vs `raid`
  (orig sp+84 / ours sp+88); first mismatch at insn 28.
* **`this` register** r4 (orig) vs r5 (ours) from the alien-turret loop
  preheader onward; `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp;
  `i` fp vs r9; `t` sl vs r6. This is the dominant residual.
* **Alien-turret selector**: original in-loop `ldr r0,[sp,#64]; cbz`; ours
  in-loop `cmp.w r1,r9,asr #31; it ne; movne`. The `i<0` anchor is required
  to keep the select in-loop and preserve 17172 B (plain ternary -> 17140 B /
  88.38 %).
* **Case-183 convoy if-chain**: test order 2,1,0 (orig) vs 0,1,2 (ours).
  Reordering fixes the tests but changes body layout/regalloc and drops
  official to 92.145215.
* **Raid loot block**: original keeps `spawnPosition` components in float
  registers (`vadd.f32 s2,s16,s22`, `vadd.f32 s4,s18,s20`); ours reloads
  `sp+312/316/320` (`vldr s2,[r,#312]` ...). 4 extra instructions.
* **Case4Tail loop**: original reloads `enemies->size()`; ours reloads the
  volatile `n` from sp+312. Removing the latch fixes the loop but changes the
  EH-pad layout (17164 B / 91.31 %).
* `.ARM.extab` still 4 B short (`0x26294` vs `0x26298`); 27 sections and
  `2,187,328 B` file size unchanged by every variant above.

## 5. Commands

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6613/dump2.py
python3 /dev/shm/w6613/score.py
cmake --build cmake-build-match --target check -j6
```

Scratch tools (not tracked): `/dev/shm/w6613/{score,dump2,clusters2,region,
shapediff,blocks,blockalign,linemap3,opdiff,try}.py`, `Level.base.cpp`,
`rea.c`, `rea.json`, `w1..w24.json`.
