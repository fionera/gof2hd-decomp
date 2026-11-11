# `Level::createMission` — wave 177 / worker w12112

Target `_ZN5Level13createMissionEv` (17,172 B) in `src/game/world/Level.cpp`.
Branch `wave177-w12112`, base master `9445ac5fb` (post-wave-176 tree).

**Exit: one code change landed.**  Function raw **5,257 → 5,154**
(aligned **963 → 945**), size **17,172 / 17,172**.  `.text` differing
bytes **79,591 → 79,488** (−103).  `.rel.plt` remains byte-identical
(29,392 B, `cmp` clean).  Full `check` green.

## Winning change

Three source edits, all in the `alienGuns` / hunter-tail area.  Together
they are size-neutral (+4 −16 +12 = 0) and reduce raw by 103 bytes.

1. `alienGuns` is a branch, not a multiply:

```c++
-    int alienGuns = alienWave * (bigAlienWave == 1 ? 8 : 7);
+    int alienGuns = alienWave ? (bigAlienWave == 1 ? 8 : 7) : 0;
```

The original's code at `b2a20` is exactly:

```
ldr r0,[sp,#64]      ; bigAlienWave
movs r4,#7
cmp r0,#0
it ne
movne r4,#8
ldr r0,[sp,#60]      ; alienWave
cmp r0,#0
it eq
moveq r4,r0          ; alienWave == 0 -> 0
```

i.e. a two-level conditional, not `alienWave * (...)`.

2. The alien-gun loop condition was redundant:

```c++
-        const int end = this->friendCount + alienGuns + this->field_164 + this->field_168;
         for (int i = this->friendCount + this->field_164 + this->field_168;
-             (i < end) & (i < this->friendCount + alienGuns + this->field_164 + this->field_168); i++, k++) {
+             i < this->friendCount + alienGuns + this->field_164 + this->field_168; i++, k++) {
```

`(i < end) & (i < full)` produced an `itttt` block with two compares
(`cmp`/`addlt`×3/`cmplt`) and a saved `end` slot.  The original at
`b338a` recomputes the sum once per iteration and has a single
`cmp fp, r3; blt`:

```
ldmia r2,{r0,r1,r2}
ldr   sl,[sp,#52]     ; alienGuns
add   r3,r0,sl
add   r3,r1
add   r3,r2
cmp   fp,r3
blt   ...
```

3. The hunter-tail loop body reloads the array element for `setToSleep`:

```c++
-        KIPlayer *ship = this->createShip(
+        (*this->enemies)[i] = this->createShip(
                 8, 0, globals->getRandomEnemyFighter(8),
                 this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length())), true, false);
-        (*enemies)[i] = ship;
-        ship->setToSleep();
+        (*this->enemies)[i]->setToSleep();
```

The original at `b41ac` reloads `this->enemies->data[i]` before the
`setToSleep` call (`ldr [r8,#248]; ldr [r1,#4]; str; ldr [r8,#248];
ldr; ldr; blx`), which is what the `(*this->enemies)[i]` form emits.
The old `ship` local kept the pointer in a register and was 12 B shorter
in this block.

## Size accounting

The three edits are size-neutral as a group:

| edit | size delta vs base |
|---|---:|
| `alienGuns` branch | +4 |
| single-comparison loop condition | −16 |
| reload tail element | +12 |
| **total** | **0** |

The `alienGuns` branch and the loop-condition simplification were each
measured separately; neither alone is size-exact (`+4` and `−16`).
The tail reload supplies the missing `+12`.  The final function is
17,172 B and the literal pool is the original's 6 words (no extra
`Globals::rnd` literal).

## Selected measurements (all real `link_so` + `fn_diff`)

Baseline (post-wave-176): raw 5,257 / aligned 963 / 17,172.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 5,257 | 963 | 17,172 |
| `alienGuns` branch only | 5,279 | 959 | 17,172 |
| loop condition `i < full` only | 5,706 | 966 | 17,156 |
| `alienGuns` branch + loop condition | 5,165 | 950 | 17,160 |
| ... + `(*enemies)[i] = ship; (*enemies)[i]->setToSleep()` (old local) | 5,167 | 994 | 17,164 |
| **... + `(*this->enemies)[i] = ...; (*this->enemies)[i]->setToSleep()`** | **5,154** | **945** | **17,172** |
| `extraAliens` truthy (`__builtin_expect(extraAliens,1)`) | 5,198 | 938 | 17,176 |
| no `rnd` hoist (natural loop) | 5,167 | 975 | 17,176 |
| bool decl `PF/AW/BAW` | 12,381 | 1,030 | 17,180 |
| bool decl `AW/PF/BAW` | 9,660 | 957 | 17,172 |
| `switch` order 201 | 5,258 | 958 | 17,172 |

The two `+4` variants (`extraAliens` truthy, natural `rnd` loop) remain
unlandable on their own; the landed combination reaches the same size
with a larger raw win.

## Stack-slot note (unchanged)

The original's flag slots are `pirateFortress=sp+56`, `alienWave=sp+60`,
`bigAlienWave=sp+64`; ours remain `bigAlienWave=sp+56`,
`pirateFortress=sp+60`, `alienWave=sp+64` (a 4-slot rotation with
`extraAliens=sp+52` vs original `sp+44`).  Declaration-order and
`int`/`char` permutations were measured; the ones that change the slot
layout also change size (e.g. `PF/AW/BAW` = 17,180) or raw (9,660+), so
the rotation is left as a documented residual.  `extraAliens` truthy and
the natural `rnd` loop are the remaining near-miss `+4` shapes.

## Gate (post-change)

- `byte_exact` **4507 / 4524** (unchanged; function raw 5,154)
- `linked_exact` **4507 / 4524**
- `size_exact` **4524 / 4524**
- `data_exact` **843 / 843**
- imports **207**, extra exports **0**, missing **0**
- lint **CLEAN**, drift **0**, symbol parity **5370 / 5370**
- `.rel.plt` byte-identical; `.text` differing bytes **79,488**
