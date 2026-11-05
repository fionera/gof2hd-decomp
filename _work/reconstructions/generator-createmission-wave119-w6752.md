# `Generator::createMission` — wave 119 (w6752): volatile initial `amount` store

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA unchanged).

Status: **source change landed.**  `volatile int amount` is replaced by plain
`int amount`, and the else-branch initialiser `amount = 0;` becomes a
volatile-qualified store `*(volatile int *) &amount = 0;`.  The address-taken
local keeps the same dedicated stack slot (`sp+92`) and the same four inline
amount stores, but the volatile store pins the initial store in the scheduling
window, which moves one instruction (`ldr r6, [sp, #32]`, the `systems` reload
in the `type == 15` arm) from after the amount store to before it — matching the
original's order.

```
before (HEAD):  83.5 %  1996 B  frame 112  .ARM.extab 0x2628c
after:          83.6 %  1996 B  frame 112  .ARM.extab 0x2628c (bytes identical)
```

Full `check`:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.rodata`
`0x41c27`, `.ARM.extab` `0x2628c` — section headers and the whole `.ARM.extab`
image are **byte-identical** to the pre-change build (`cmp` of the section
bytes).  The LSDA record is unchanged.

## 1. The landed delta

```diff
-    volatile int amount;
+    int amount;
...
-        amount = 0;
+        *(volatile int *) &amount = 0;
```

The only instruction-stream change is one `ldr r6, [sp, #32]` moving from
`0x9754e` to `0x97546` (the original has it at `0x97536`).  Everything else is
instruction-for-instruction identical to the previous build.

## 2. Measurements (all `usize == 1996` unless noted)

| variant | pct | frame | note |
|---|---:|---:|---|
| HEAD (wave-119/w6749) | 83.5 | 112 | `volatile int amount`, padA double |
| **volatile initial store (landed)** | **83.6** | **112** | `int amount` + `*(volatile int*)&amount = 0` |
| volatile store on case 0/3/5 | 83.6 | 112 | same instruction move |
| volatile store on case 2/11 | 83.6 | 112 | same |
| volatile store on case 8 | 83.6 | 112 | same |
| all amount stores volatile | 83.5 | 112 | |
| `int amount` + padA, plain `amount = 0` | 70.2 | 112 | 1992 B, amount at `sp+28` |
| `int amount`, no padA | 79.1 | 96 | 1980 B, amount at `sp+24` |
| `int amount`, no padA, all stores volatile | 71.9 | 96 | 1984 B |
| `volatile int amount` only, no padA | 71.9 | 96 | 1984 B |
| `volatile char`/`short`/`long long` padA | 79.4 | 104 | 1996 B |
| `volatile float`/`int` padA | 79.3/79.6 | 104 | 1992 B |
| `int itemId` declared with `int level` | 79.5 | 96 | 1984 B (code identical to 1980, +4 B pool) |
| no `pick == 12` hint | 80.7 | 112 | loop rotates, head `+0x8e` |
| no mask hint | 82.1 | 112 | |
| `__builtin_expect(offer == 5, 0/1)` | 63.9/40.0 | 112 | |
| `__builtin_expect(race < 4 && type == 13, 0/1)` | 44.4/65.6 | 112 | |
| `do { } while (!selected)` | 46.5 | 112 | 2028 B |
| `if/else if` instead of the switch | 76.0 | 96 | 1980 B |
| explicit empty cases 1/4/6/7/9/10 | 79.1 | 96 | 1980 B |
| `int amount = 0;` at declaration | 79.1 | 96 | 1980 B |
| `amount = 0U` / `(int)0` / `0L` / `(int)(0.0f)` | 70.2 | 112 | 1992 B |

## 3. Why 83.6 and not higher

The remaining delta is the same block-placement/register-allocation set
documented in `generator-createmission-wave119-w6749.md`:

* **frame 112 vs 96.**  The dedicated `amount` slot (`sp+92`) and the `padA`
  double (`sp+96`) keep the frame at 112; the original reuses `sp+28` for
  `amount` and has no pad.  Every plain `int amount` build drops to frame 96
  and 1980/1984 B, but then LLVM sinks the per-case amount stores and the
  function is 16 B short; the volatile/address-taken forms keep the stores but
  force the dedicated slot.
* **`pick == 12` block order.**  The original falls through into the
  `pick == 12` arm; ours falls through into the `selected = true; type = pick`
  arm.  Removing the `__builtin_expect(pick == 12, 0)` hint flips the block but
  rotates the whole loop (head `+0x90` -> `+0x8e`, 80.7 %).
* **shared `type` store block.**  The offer==5 / type==13 shared `type` store
  is placed after the offer arm in ours and after the type==13 arm in the
  original.
* **cold `getRoutes() == nullptr` block.**  Original places it at `+0x3ce`
  (after case 15); ours places it inline.

## 4. Reproduction

```sh
# from HEAD
python3 - <<'PY'
p='src/game/mission/Generator.cpp'
s=open(p).read()
s=s.replace("    volatile int amount;\n","    int amount;\n")
s=s.replace("        amount = 0;\n        ++difficulty;",
            "        *(volatile int *) &amount = 0;\n        ++difficulty;")
open(p,'w').write(s)
PY
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch: `/dev/shm/w6752/` (`Generator.base.cpp`, `qscore.py`, `ndiff.py`,
`extab.py`, `b*.py` variant sweeps).  Not part of the repo.
