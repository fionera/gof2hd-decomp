# Radar::draw wave 154 (w10002) — `otherLock` is an `int` spilled to `[sp,#144]`; `bool otherLock` reproduces the whole prologue map but is 120 B short

**Status:** analysis pass, 2026-11-08. Worktree `wt/w10002`, based on `master`
(`4417db0b4`, wave 153). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave152-w9903.md`, `radar-draw-wave151-w9803.md`,
`radar-draw-wave151-w9703.md`.

Baseline (HEAD, wave 153): original 9,908 B; ours 9,908 B (frame 296); raw
same-address diff **8,577**; instruction-index alignment **1,484**; all 27 section
sizes exact; file size exact; `.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical;
`.rel.plt` byte-identical.

## 1. New ground truth: what the original does with `campaign` and `otherLock`

Manual disassembly of the original (image base 0x10000, function at `0x12e050`):

```
12e0f4  blx  Status::getCurrentCampaignMission()
12e0f8  mov  sl, r0                 ; campaign -> sl, live to 12e38a
...
12e388  movs r0, #1
12e38a  cmp.w sl, #2                ; last use of campaign
12e38e  str  r0, [sp, #144]         ; otherLock = 1  -> [sp,#144]
...
12ec3e  movs r1, #0
12ec40  movs r0, #0
12ec42  str  r1, [r4, #4]           ; lockedEnemy = nullptr
12ec44  str  r0, [sp, #144]         ; otherLock = false
12ec46  str  r1, [r4, #8]           ; dockNavPtr = nullptr
```

So in the original **`otherLock` is a 4-byte `int` that lives in the stack slot
`[sp,#144]`**, and `campaign` owns `sl` from the prologue until `12e38a`.  The two
values share the slot/register pair: `[sp,#144]` is reused for `campaign` first and
then for `otherLock` (their live ranges only touch at `12e38a`/`12e38e`).

`missionBlocksPlanets` owns `fp` (`mov.w fp,#0` at `12e07a` and `12e0d8`).

The original's enemy-loop preheader (`12ec60`-`12ecec`) is:

```
12ecc8  add.w r9, r4, #396          ; r9 = &this->lockLabel (0x18c), loop-invariant
12ecd0  mov.w sl, #68               ; 68 -> sl (rematerialised into sl at 4 sites)
12ecd8  mov.w fp, #0                ; i -> fp
12ece0  movs  r6, #0                ; bossSeen -> r6
```

## 2. The one-toggle shape that reproduces the prologue map

`int otherLock = otherLockFlag;` -> `bool otherLock = otherLockFlag;` (a pure
type change; `otherLockFlag` stays a `bool`):

| | size | raw | align | campaign | otherLock | `missionBlocksPlanets` | 68 |
|---|---:|---:|---:|---|---|---|---|
| original | 9908 | – | – | `sl` | `[sp,#144]` | `fp` | `sl` |
| HEAD base (`int otherLock`) | 9908 | 8577 | 1484 | `[sp,#144]` | `[sp,#144]` | `sl` | `fp` (remat) |
| `bool otherLock` | **9788** | 9140 | 1739 | **`sl`** | **`[sp,#144]`** | **`fp`** | `r9` |

`bool otherLock` is the *only* single toggle found that reproduces **all three**
of the original's early assignments (`campaign`->`sl`, `otherLock`->`[sp,#144]`,
`missionBlocksPlanets`->`fp`).  It is exactly the mechanism the w9903 note asked
for.  But it is **120 B short** (9,788 vs 9,908) and it hands `r9` to the constant
68 while `&lockLabel` goes to a stack slot (`12ec1e: add.w r1,r4,#396;
str r1,[sp,#76]`), i.e. the `r9`/`sl` pair is still swapped relative to the
original.

## 3. Falsified / measured this wave

All at `-Oz`, `link_so`, size shown; `campaign` column = register chosen right
after the `getCurrentCampaignMission` call.

| variant | size | raw | align | campaign |
|---|---:|---:|---:|---|
| HEAD base | 9908 | 8577 | 1484 | `[sp,#144]` |
| `bool otherLock` | 9788 | 9140 | 1739 | `sl` |
| `merge_otherlock` (drop `otherLockFlag`, `int otherLock = ...` directly) | 9836 | 8682 | 1586 | `[sp,#144]` |
| `bool otherLock` + `int planetHit` | 9804 | 9065 | 1743 | `sl` |
| `bool otherLock` + `int landmarkHit` | 9784 | 9124 | 1821 | `sl` |
| `bool otherLock` + `int marked` | 9796 | 9121 | 1723 | `sl` |
| `bool otherLock` + `int planetHit` + `int landmarkHit` | 9800 | 9039 | 1824 | `sl` |
| `bool otherLock` + `int planetHit` + `int landmarkHit` + `int marked` | 9808 | 9114 | 1818 | `sl` |
| `bool otherLock` + `bool notTurretMode` | 9788 | 9136 | 1760 | `fp` |
| `int landmarkHit` + `bool enemyHit` | 9908 | 8739 | **1483** | `[sp,#144]` |

(`bool otherLock` + `bool notTurretMode` puts `68` in `sl` — the only shape seen
with `c68=sl` — but `campaign` then goes to `fp`, and it is 120 B short.)

A pair sweep over all 24 toggles (300 pairs) was run; every `campaign`->`sl` pair
keeps `68` in `r9` and stays 100-124 B short, and every 9,908 B pair keeps
`campaign` on the stack.  The only 9,908 B shape with a better *aligned* count
than HEAD is `int landmarkHit` + `bool enemyHit` (9,908 / **8,739** / **1,483**),
i.e. an alignment win of 1 paid for with a raw loss of 162.

The `merge_otherlock` result is informative: dropping the `otherLockFlag` temporary
and assigning `otherLock` directly (which is what the original's `movne r5,#1` /
`str r0,[sp,#144]` codegen implies) does **not** move `campaign` into `sl`; the
`bool` type is what does it.

## 4. Conclusion / open questions

1. The original source almost certainly has `bool otherLock` (or an equivalent
   0/1-typed `otherLock`), because that is the only thing that reproduces the
   `campaign`->`sl` / `otherLock`->`[sp,#144]` / `missionBlocksPlanets`->`fp`
   triple.
2. The remaining 120 B must come from a **body** difference that our source is
   missing (or from a set of type widenings that together add exactly 120 B).
   No combination of the 24 known single toggles reaches 9,908 with
   `campaign` in `sl`.
3. Even with the prologue map reproduced, `r9`/`sl` are swapped: the original
   keeps `&this->lockLabel` (0x18c) in `r9` and rematerialises `68` into `sl`;
   `bool otherLock` does the opposite.  This is a global-allocation tie-break, so
   a body difference that changes the number of uses of `&lockLabel` or of `68`
   in the enemy loop is the most likely lever.
4. HEAD's 9,908 B / raw 8,577 remains the best measured shape; the base was kept
   (no source change) because every `campaign`->`sl` shape found is both short and
   raw-worse.

## 5. Gate

Unchanged from wave 153 (no source change landed):
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size `4524/4524`,
data `843/843`, imports `207`, lint CLEAN, drift 0, parity `5370/5370`.
