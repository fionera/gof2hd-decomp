# `Generator::createMission` — wave 126 (w7608): frame-96 source search

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**).

Status: **research note, no source change landed.**  HEAD stays at the wave-124
shape (`volatile short padA` + `volatile int *ap = &amount` + `Galaxy *&galaxy` +
named `toSystem`) at **90.0 % / 1996 B / frame 104 / LSDA 172 B**.  This pass
re-derived the original's exact stack layout, found a source shape that
reproduces it (frame 96, all seven spill slots at the original offsets), and
isolated the last 4-byte size delta that keeps it out of the tree.

```
before (HEAD bf365a964):  90.0 %  1996 B  frame 104  LSDA 172 B (exact)
after:                    90.0 %  1996 B  frame 104  LSDA 172 B (exact)
```

Full gate (`check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. The original's exact frame (from the linked disassembly)

The original is **frame 96** with **seven** 4-byte register spill slots and **no
frame objects except the canary and four `String`s**:

```
sp+16  galaxy (97614) / bonus (97720)
sp+20  &__stack_chk_guard (971f6)
sp+24  agent (97220) / difficulty (9752a, 9761c)
sp+28  targetStation (97218) / amount (974a8, 974b4, 97604)
sp+32  status (97228) / rem (9770e) / mission (9775e)
sp+36  race (97228) / type (97340)
sp+40  rnd (97244)
sp+44..92  four String objects (construction sites `add r0,sp,#80` / `#68` /
           `#56` / `#44`)
sp+92  canary (971d2)
```

HEAD's frame is **104**:

```
sp+20  &__stack_chk_guard
sp+24  agent / bonus
sp+28  targetStation / difficulty
sp+32  status / rem / mission
sp+36  race
sp+40  rnd
sp+44..92  four Strings
sp+92  amount alloca (`volatile int *ap = &amount`)
sp+98  padA (`volatile short`)
sp+100 canary
```

So HEAD already matches the original for slots `sp+20..40`; the whole 8-byte
delta is the `amount` alloca (4) + `padA` (2) + alignment.  `sp+16` is unused
in HEAD (the original's `galaxy`/`bonus` slot).

## 2. A source shape that reproduces the original's frame

Removing the alloca anchors and using a **local** `Galaxy *` for `distance`
with `Globals::galaxy` for `getStation`, plus a **volatile `rem`**, gives the
original's exact 7-slot layout:

```c++
    int itemId;
    int amount;                       // plain local, no padA
    ...
    } else {
        ++difficulty;
        amount = 0;
        ...
    }
    Galaxy *galaxy = Globals::galaxy; // value spilled to sp+16
    ...
    toSystem = Globals::galaxy->getStation(targetStation)->getSystem(),
    ...
    volatile int rem = rawBonus % 50; // forces rem out of r9
```

Measured: **90.7 % / 2000 B / frame 96**, slots `sp+16..40` (galaxy, canary,
agent/difficulty, targetStation/amount, status, race, rnd) — i.e. the original
layout exactly.  The 4-byte overshoot is the only blocker: a size change shifts
every following `.text` function and drops `byte_exact` (the `bl` PLT offsets
are PC-relative), so `check` fails.

Why `volatile` on `rem` is needed: with a plain `int rem`, clang keeps `rem` in
`r9` and rematerialises `agent` from its spill slot at `9776a`, so agent's slot
stays live across difficulty and the allocator needs an 8th slot.  The original
spills `rem` to `sp+32` (status's dead slot) and keeps `agent` in `r9`.  The
volatile forces the spill, which is what unlocks the 7-slot layout.

## 3. The last 4 bytes: reward-modifier block placement

The 2000-byte shape diverges from the original at `97696`:

```
orig: 97696 d005        beq.n 976a4      (type == 9 -> 1.2)
      9769a f040 80e2   bne.w 97862      (other -> cold block after costs)
ours: 97696 d004        beq.n 976a2
      9769a d142        bne.n 97722      (other block placed before costs)
```

The original lays the `type == 8 / 11 / 3 / 5` reward modifiers **after** the
`costs`/`Mission` construction (`97862`); ours (and HEAD) place them inline
(`976aa`/`97722`).  The short/long branch encodings then differ by 4 bytes in
total.  No tested source tweak (reward-modifier order, `Galaxy *&` vs `Galaxy *`,
`bonus` declaration, `int`/`unsigned`/`long` on `rem`, amount-first type-8 arm,
`count` type) moves the cold block to `97862` while keeping the frame-96 slots.

## 4. Semantically-correct variants measured (all 1996 B unless noted)

| variant | pct | usize | frame | note |
|---|---:|---:|---:|---|
| **HEAD (padA + volatile-pointer amount + `Galaxy *&`)** | **90.0** | **1996** | **104** | landed |
| plain amount + local `Galaxy *` + `volatile int rem` | **90.7** | **2000** | **96** | exact slots, 4 B long |
| same, `volatile unsigned/long rem`, `(void)rem`, `rem+rawBonus` | 90.7 | 2000 | 96 | identical code |
| same, `bonus -= rem` | **94.9** | **1996** | **96** | **WRONG**: yields `bonus = rawBonus`, not `rawBonus - rem` |
| same, `getMissionBonus(race)` instead of `getMissionBonus(agent->getRace())` | 90.4 | 1992 | 96 | drops the `Agent::getRace` call |
| plain amount + local `Galaxy *`, plain `int rem` | 84.1 | 2000 | 104 | `rem` in r9, agent rematerialised |
| plain amount + `Galaxy *&` + `volatile int rem` | 87.9 | 1988 | 96 | no galaxy spill |
| natural separate reward statements (no comma expr) | 81.6 | 1996 | 96 | slot base shifts |
| plain amount + natural statements + local galaxy | 84.1 | 2000 | 104 | 218 differing bytes, shifted slots |
| `Galaxy *&` + natural statements + volatile rem | 87.2 | 1988 | 96 | |

The `bonus -= rem` row is a trap: it scores 94.9 % at exactly 1996 B, but it is
**not faithful** — the original computes `bonus = rawBonus - rem`
(`subne r5,r6,r0` with `r6 = rawBonus`), whereas `bonus -= rem` after
`bonus = rawBonus + rem` folds to `bonus = rawBonus`.  It is not landed.

## 5. Remaining branch delta (independent of the frame)

`bhi.n 0x1d0` (orig) vs `bhi.n 0x242` (ours) at the `type > 14` test
(`9735a`): the original still falls through the `race < 4 && type == 13` test
even when `type > 14` proves `type != 13`; clang folds it in ours.  The
`__builtin_expect(race < 4 && type == 13, 1)` anchor is mandatory for the 172 B
LSDA (removing it gives 168 B and 1988 B).  Moving the hint to the
`type < 15 && 0x4801` test, `race <= 3`, or swapping the operands leaves the
code byte-identical (90.0 %).

## 6. Reproduction

```sh
cp /dev/shm/w7608/Generator.base.cpp src/game/mission/Generator.cpp   # HEAD wave-124 shape
# frame-96 shape (90.7 %, 2000 B):
#   drop `volatile short padA;` and `padA = 0, (void) padA,`
#   `int amount;` plain, `++difficulty; amount = 0;` in the else arm
#   `Galaxy *galaxy = Globals::galaxy;` + `Globals::galaxy->getStation(...)`
#   `volatile int rem = rawBonus % 50;`
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w7608/qscore.py        # pct / osize / usize
```

Scratch harness (not in the repo): `/dev/shm/w7608/` — `Generator.base.cpp`
(landed source), `qscore.py` (single-function verify score, ~0.1 s),
`fastscore.py` (raw byte diff), `sweep*.py` (variant sweeps), `cc.sh`
(exact-flags compile helper).
