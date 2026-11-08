# Radar::draw wave 153 (w10003) — else-arm String shapes: the faithful shape is already
# optimal; every both-metric-improving shape is 4-8 B short of 9,908

**Status:** analysis only, no source change landed, 2026-11-08. Worktree `wt/w10003`, based on
`master` (`4417db0b4`, wave 153). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave152-w9902.md`, `radar-draw-wave152-w9901-pad30-shape.md`,
`radar-draw-wave152-w9802.md`.

Baseline (HEAD, stock gold, **no** pad hack): original 9,908 B; ours 9,908 B; raw same-address
diff **8,577**; instruction-index alignment **1,484**; LSDA **19 rows / 132 B** (11 LP!=0);
`.ARM.extab` 0x26298; `.rodata` 0x1c9e10; byte_exact 4507.

## 1. The original else arm is exactly w9902's scoped copy

Manual disasm of the original at `0x12e974` (the `i != 0 || alienOrbit` arm; the gameText
`i == 0 && !alienOrbit` arm is the *later* block at `0x12ea2a`):

```
12e974 ldr.w r0,[r4,#312]     ; landmarks
12e978 add   r5, sp, #232     ; r5 = &tmp (String sret)
...    (load landmark pos / camera)
12e9a6 blx   calcDistance
12e9aa ldr   r6, [sp,#104]    ; r6 = &this->lockLabel
12e9b0 blx   String::operator=
12e9b4 mov   r0, r5
12e9b6 blx   String::~String   ; destroyed BEFORE DrawString
12e9de mov   r2, r6
12e9e0 blx   DrawString
```

So the original is `String tmp = calcDistance(...); lockLabel = tmp; ~tmp; DrawString(...lockLabel)`.
Our landed w9902 shape emits the identical instruction sequence; only the register choices
(`r9`/`fp` vs the original `r6`/`r5`) and the stack slots differ. The `if` arm
(`0x12eacc`) is also a temp + `operator=` + `~String` before `DrawString`, so both arms are
structurally faithful.

## 2. New hypotheses measured (all on the wave-153 HEAD base)

`order_ds_then_assign` = `String tmp = calcDistance(...); DrawString(...tmp...); lockLabel = tmp;`
inside one scope (temp destroyed at scope end, i.e. after `DrawString`).

| else-arm shape | usize | raw | aligned | LSDA rows |
|---|---:|---:|---:|---:|
| HEAD w9902 scoped copy (landed) | 9908 | **8577** | **1484** | 19 |
| `const String& r = calcDistance(...);` + scoped store | 9908 | 8577 | 1484 | 19 |
| `noscope` (`String tmp; store; DrawString(tmp)` one scope) | 9904 | 8510 | 1450 | 19 |
| `const String&` + `noscope` lifetime | 9904 | 8510 | 1450 | 19 |
| `order_ds_then_assign` (DrawString, then store) | 9900 | **8349** | **1465** | 19 |
| `tmp_then_ds_tmp` (store, DrawString(member), scope ends) | 9900 | 8354 | 1471 | 19 |
| `const String&` + `tmp_then_ds_tmp` lifetime | 9900 | 8354 | 1471 | 19 |
| `default_assign` (`String tmp; tmp = calcDistance(...)`) | 9932 | 8584 | 1470 | 21 |
| `copy_member_scope` (`lockLabel = calcDistance; {String c=lockLabel; DrawString(c);}`) | 9920 | 8638 | 1443 | 21 |
| `copy_member_scope2` (`{String c=lockLabel;}` then `DrawString(member)`) | 9888 | 9163 | 1518 | 19 |

`const String&` binding is codegen-identical to the value form (lifetime extension does the
same thing), so the "reference" hypothesis is falsified.

## 3. The hard constraint: 9,908 B is mandatory

A 4-byte-short function shifts every later function and collapses byte_exact. Measured:
`noscope` (9,904 B) → verify **byte_exact 3641** (from 4507). So a landing must be exactly
9,908 B.

Type-toggle sweeps (1-2 toggles on each shape; 3 toggles on `tmp_then_ds_tmp`) found the
9,908 B hits:

| shape + toggles | raw | aligned |
|---|---:|---:|
| HEAD (no toggle) | 8577 | 1484 |
| HEAD + `alwaysEnemySeen:int`, `marked:int` | **8534** | 1542 |
| HEAD + `landmarkHit:int`, `enemyHit:bool` | 8739 | **1483** |
| `noscope` + `marked:int` | 8593 | 1533 |
| `tmp_then_ds_tmp` + `landmarkHit:int`, `marked:int` | 8563 | 1684 |
| `tmp_then_ds_tmp` + `marked:int`, `landmarkHit:int`, `alwaysEnemySeen:int` | 8552 | 1679 |

**No 9,908 B shape improves both metrics.** Every shape that improves both (`order_ds_then_assign`
8349/1465, `noscope` 8510/1450, `tmp_then_ds_tmp` 8354/1471) is 4-8 B short, and the only
size companions that reach 9,908 are type widenings that rotate the prologue and regress the
other metric.

## 4. Root cause of the residual (for the next wave)

The original keeps `campaign = getCurrentCampaignMission()` in `sl` across the landmark loop and
uses `fp` as the loop index. Ours spills `campaign` to `[sp,#144]` and uses `sl` as the loop
index (because the else-arm temp takes `fp`). This is the same circular register-allocation knot
the w9902 note describes: the else-arm temp register choice (`fp` vs `r5`) drives the loop-index
register (`sl` vs `fp`), which drives whether `campaign` is spilled. The `order_ds_then_assign`
shape frees `fp` for the loop index and is the closest, but its extra `DrawString`-before-store
order costs 8 B.

Also note the original keeps the else-arm `offset` as a *field byte offset* in `r9`
(`ldr.w ip,[r4,r9]`, `r9` = 0xfc or 0x100) rather than as a loaded value; ours loads the value
into a register and `add`s it. That is a separate codegen difference worth chasing.

## 5. Open questions

1. A faithful +8 B body addition on `order_ds_then_assign` (9,900 B) would land 8349/1465 at
   9,908 B — the best both-metric result seen. The +8 must not rotate the prologue.
2. Or a +4 B addition on `noscope` (9,904 B) → 8510/1450 at 9,908 B.
3. `campaign` in `sl`: if a source shape makes clang keep it, the whole function's register
   allocation may snap toward the original.

## 6. Gate

No source change landed; HEAD gate unchanged (byte_exact 4507, linked_exact 4507, size 4524,
data 843, imports 207, lint clean, drift 0). Radar::draw stays raw **8,577** / aligned **1,484**
at 9,908 B / 19 LSDA rows.
