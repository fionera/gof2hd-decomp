# LevelScript::process — wave 146 / w9204 (prologue `strd` slot order: the
# faithful accessor order DOES flip it; body-at-case-70 companion still open)

Status: **analysis only, no code change landed** (source restored to wave-145
HEAD `31297fadc`). Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU
`src/game/world/LevelScript.cpp`. Scratch helpers in `/dev/shm/w9204/` (not
tracked): `fdiff.py`, `reldiff.py`, `measure.sh`, `perm.py`/`perm2.py`,
`gen2.py`/`gen3.py`/`gen4.py`, `weights.py`/`weights2.py`, `ssc.txt`,
`pre_pei.txt`.

## Result

```
                         wave-145 HEAD     this pass
process raw diffbytes    54,895            54,895   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
.rel.plt                 0 diff            0 diff
byte_exact               4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## 1. Prologue `strd` slot order — w9003's "declaration order cannot fix it" is WRONG

w9003's table reported all six declaration permutations as `strd r6,r5` (delta
first). Re-measured on the **wave-145** base, three of the six produce the
original's `strd r5,r6,[sp,#72]` (messages @72, delta @76):

| accessor order | size | raw | prologue `strd` |
|---|---:|---:|---|
| enemies, player, messages (HEAD) | 57232 | 54,895 | `r6,r5` |
| enemies, messages, player | 57232 | 54,895 | `r6,r5` |
| player, enemies, messages | 57208 | 55,374 | `r4,r6` |
| player, messages, enemies | 57308 | 55,379 | **`r5,r6`** |
| messages, enemies, player | 57308 | 55,379 | **`r5,r6`** |
| **messages, player, enemies** | **57308** | **55,367** | **`r5,r6`** |

`messages, player, enemies` is the **witnessed original accessor order**
(`0x137d3e getMessages`, `0x137d46 getPlayer`, `0x137d50 getEnemies`), and it is
the only order whose prologue matches the original instruction-for-instruction
through `strd r5,r6,[sp,#72]` (including `strd r4,r0,[sp,#80]`).  So the slot
order is *not* a pure StackSlotColoring weight artefact that no declaration can
reach — it follows directly from which accessor is called first (`getMessages`
before `getEnemies`).  The catch: the faithful order is **+76 B (57308)**, not
size-neutral.

### Why the six-permutation result differs from w9003
w9003 measured on the wave-144 base `eb13ada9b`.  Wave-145 (w9002 state-test
order 4,5,3 + w9004 second `getSpeed`) changed the CFG/block frequencies enough
that the `messages` vs `delta` spill weights now cross when `getMessages` is
scheduled first.  The mechanism is still StackSlotColoring (weight = sum of
BB frequencies of slot refs); the declaration order changes the *schedule* of
the three accessor calls, which changes the mission==1 arm block frequency
(`BB65`, the `(*messages)[2]` load) that dominates messages' weight.

### The +76 is a real code-growth, not a local pad
The faithful variant's prologue matches the original up to `strd r5,r6,[sp,#72]`
(only `status` lands in `r9` instead of `r4`, and the first `beq.w` target is
+6).  The +76 accumulates over the whole function (20830 orig insns vs 20675
ours; ours has fewer but longer insns), so a `-76` companion is needed to land
it at 57232.  Not found this wave.

## 2. body-at-case-70 — the ±4 is the status load + the un-shared tail

Reproduced w9001's shape on the wave-145 base (body at the start of case 70,
guard `getCurrentCampaignMission() == 50`, pre-switch mission 50/51 keep
`goto wormholeExitBody`):

| variant | size | raw | body tail | note |
|---|---:|---:|---|---|
| `hoist=0, n=0` (faithful, reloads `enemies->data_`) | 57388 | 55,271 | inline | `enemies` forced to `r8` (high) → 4-byte reloads |
| **`hoist=0, n=1`** (faithful body + `unsigned int n`) | **57236** | **55,377** | inline | `enemies` in `r6` (low), matches original reloads |
| `hoist=1, n=0` (`KIPlayer **en = enemies->data_`) | 57228 | 55,403 | inline | METRIC-ONLY (original reloads `data_` 4×) |
| `hoist=1, n=1` | 57220 | 55,166 | inline | |

The `hoist=0, n=1` body is **+4** vs the original's 84-byte body, and the +4 is
exactly two 2-byte items:

* `ldr.w r0,[r9]` (4 B) for `Globals::status` in the body vs the original's
  `ldr r0,[r6,#0]` (2 B) — ours keeps `status` in a high register (`r9`), the
  original in a low one (`r6`);
* the tail `movs r0,#1; str.w r0,[fp,#28]` (6 B) vs the original's shared
  `b.w 0x1381aa` (4 B) — the body's `m_nState = 1` is *not* tail-merged in the
  case-70 placement (it is in the HEAD placement, whose body ends `b.w 0x1382d8`).

So the genuine companion must supply **-2 (status into a low reg) and -2
(share the `m_nState = 1` tail)**, or a single -4 elsewhere.  Both were tried
individually and neither is additive:

| edit | size | raw |
|---|---:|---:|
| `int n` instead of `unsigned int n` | 57236 | 55,377 (identical codegen) |
| `i != n` | 57236 | 55,377 (identical codegen) |
| `Status *st = Globals::status;` in body | 57244 | 55,268 |
| `m_nState = 1; break;` | 57244 | 55,325 |
| `goto missionFiveStateOne` (tail share, split `Vector *scratch`) | 56964 | 55,071 |
| condition moved to pre-switch (both arms) + body at case-70 start | 57268 | 55,337 |
| condition in pre-switch + body after case-70 state-0 | 57052 | 55,361 |
| HEAD + remove `if (delta == 0x7d) setStanding` anchor | 57216 | 54,930 |
| body(0,1) + remove anchor | 57380 | 55,294 (non-additive) |

The `goto missionFiveStateOne` tail-share is worth far more than -2 because the
`goto` also changes the surrounding block placement; it does **not** land at
57232.

## Open questions

1. Get `Globals::status` into a low register inside the case-70 body (the
   original uses `r6`) — this is the missing -2.  A local `Status *st` is +8,
   not -2.
2. Make the case-70 body's `m_nState = 1` share the mission-5 tail at
   `0x1381aa` without the `goto` block-placement blow-up.
3. The faithful accessor order (`messages, player, enemies`) is +76; find the
   -76 companion so the correct `strd r5,r6,[sp,#72]` can be landed at 57232.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9204/fdiff.py _ZN11LevelScript7processEi   # 54,895
```
