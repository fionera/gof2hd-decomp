# LevelScript::process — wave 134 / w7746 (prologue register map: local caching, first-use order, body caching)

Status: **note only, no source change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e` (wave-133 head).
Scratch helpers in `/dev/shm/w7746/` (not tracked): `base.cpp`, `score.py`,
`score2.py`, `harness.py`, `batch*.py`, `wd2.py`, `bigdiff.py`, `more.py`.

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      58.851615         58.851615
orig / ours insns        20169 / 20044     20169 / 20044
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7746/score2.py`).

## The prologue difference, re-measured

Original (`0x137d3a..0x137d72`):

```
137d3a  ldr.w r0,[r4,#24]!        ; r4 = &this->m_pLevel, r0 = m_pLevel
137d3e  blx   Level::getMessages
137d42  mov   r5,r0               ; messages -> r5
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; &m_pLevel, player
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getEnemies
137d54  mov   r8,r0               ; enemies -> r8
137d56  ldr   r0,[pc,#848]        ; GOT slot of Globals::status
137d58  add   r0,pc
137d5a  ldr   r4,[r0,#0]          ; r4 = Globals::status (the Status* value)
137d5c  ldr   r0,[r4,#0]
137d5e  blx   Status::getCurrentCampaignMission
137d62  cmp   r0,#0
137d64  str.w sl,[sp,#88]         ; this
137d68  strd  r4,r8,[sp,#92]      ; status, enemies
137d6c  mov   r0,r8
137d6e  strd  r5,r6,[sp,#72]      ; messages, delta
137d72  beq.w 137e84
```

Master (baseline, `enemies,player,messages`):

```
137d3a  ldr.w r0,[r4,#24]!
137d3e  blx   Level::getEnemies   ; <-- enemies first
137d42  mov   r8,r0
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getMessages  ; <-- messages last
137d54  mov   r4,r0               ; messages -> r4
137d56  ldr   r0,[pc,#884]
137d58  add   r0,pc
137d5a  ldr.w r9,[r0]             ; status -> r9
137d5e  ldr.w r0,[r9]
137d62  blx   Status::getCurrentCampaignMission
137d66  cmp   r0,#0
137d68  str.w sl,[sp,#88]
137d6c  strd  r4,r6,[sp,#72]      ; messages, delta (already the original pair order)
137d70  mov   r0,r8
137d72  strd  r9,r8,[sp,#92]
137d76  beq.w 137e8e
```

Only **6 instructions** differ: the two accessor calls are swapped, `messages`
is r4 (not r5), `Globals::status` is r9 (not r4), and the two `strd` spills are
in the other order. Everything else in the 30-instruction prologue matches.
The prologue is therefore ~0.03 % of the function; it cannot by itself move a
58.85 % score. The body is the real difference.

## Faithful call order: prologue solved, body re-coloured

`messages,player,enemies` (variant **F**, the emitted order of the original):

```
137d3a  ldr.w r0,[r4,#24]!
137d3e  blx   Level::getMessages
137d42  mov   r5,r0               ; messages -> r5  (MATCH)
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; (MATCH)
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getEnemies
137d54  mov   r8,r0               ; enemies -> r8    (MATCH)
137d56  ldr   r0,[pc,#884]
137d58  add   r0,pc
137d5a  ldr.w r9,[r0]             ; status -> r9     (orig r4)
...
137d6c  strd  r9,r8,[sp,#92]
137d70  mov   r0,r8
137d72  strd  r6,r5,[sp,#76]      ; delta,messages   (orig messages,delta at 72)
```

So the faithful order reproduces 4 of the 6 prologue differences. Variant **B**
= F + `player->rotate(...)` in the mission-1 body (the original loads the
spilled `player` at `137da6`, i.e. it does *not* call `getPlayer()` again):

```
137dac  ldr   r0,[sp,#84]         ; player local (MATCH original 137da6)
```

B's prologue is **2 instructions** from the original (`status` r9 vs r4 and the
`strd` pair order). But both variants are bigger:

| variant | size | insns | raw % |
|---|---:|---:|---:|
| **baseline** `e,p,m` | **57232** | **20044** | **58.8516** |
| F `m,p,e` (faithful order) | 57412 | 20039 | 57.5010 |
| B `m,p,e` + mission-1 `player` local | 57396 | 20032 | 57.3468 |
| C `Level *level` used for the 3 calls, `e,p,m` | 57232 | 20045 | 58.7656 |

The +164..+180 B is **body re-colouring**, not the prologue: B has 137 *fewer*
instructions than the original yet 164 more bytes, i.e. ~105 more instructions
widen from 16-bit to 32-bit (original 13044/7786 16/32-bit, baseline
12808/7904, messages-first 12682/8011). The original is the most 16-bit-heavy
of the three; its compactness comes from the body's low-register allocation,
which our incomplete body (125 fewer instructions) does not reproduce.

## New axes tried this pass (all built and scored)

Declaration order is a confirmed no-op: all 6 declaration permutations of
`{messages,player,enemies}` with the faithful `m,p,e` init order compile
**byte-identically** (57412 / 20039 / 57.5010). The initialiser (call) order is
the only thing that moves the prologue.

| variant | size | insns | raw % | note |
|---|---:|---:|---:|---|
| baseline | 57232 | 20044 | 58.8516 | current master |
| `Level *level = m_pLevel;` used for the 3 calls (`e,p,m`) | 57232 | 20045 | 58.7656 | keeps `m_pLevel` in r4, messages still r4 |
| `Level *level` used for the 3 calls + mission-1 `level->lodManager` | 57316 | 20019 | 58.0 | messages→r5! status still r9 |
| `Level *level` used for the 3 calls + mission-0 `level->lodManager` | 57304 | 20029 | 53.6 | |
| `Level *level` used for the 3 calls + *all* body `m_pLevel->` | 57024 | 20000 | 51.3 | `[sl,#24]` reloads 91→28 (orig 40) |
| `Level *level` + body-level `level->` only (prologue unchanged) | 57000 | 20012 | 48.0 | |
| `Status *status = Globals::status;` (prologue only) | 57320 | 20018 | 58.6 | caches the value, +ldr |
| `Status *status` + all body `Globals::status->` → `status->` | 57092 | 19905 | 48.2 | re-colours enemies→fp, player→r5, messages→r6 |
| B + `Status *status` (all uses) | 57092 | 19907 | 48.3 | |
| B + `Status *status` (prologue only) | 57396 | 20032 | 57.3 | = B |
| B + `Level *level` | 57396 | 20033 | 57.3 | = B |
| `this->m_pLevel->` prefix on the 3 calls | 57232 | 20044 | 58.8516 | byte-identical |
| split declarations `m,p,e` + init `e,p,m` | 57232 | 20044 | 58.8516 | byte-identical |
| `int mission = Globals::status->getCurrentCampaignMission();` cache | 57148 | 20067 | 39.2 | collapses the 21 compare calls |
| `if (m_pLevel == nullptr) return false;` after the 3 locals | 57320 | 20066 | 47.3 | |
| all body `m_pLevel->getPlayer()` → `player` (29 sites) | 57200 | 19978 | 55.3 | |
| all body `m_pLevel->getPlayer()` → `player`, `m,p,e` | 57248 | 19960 | 51.5 | |
| swap mission-0 / mission-1 branches in the dispatch | 57564 | 20101 | 49.2 | |
| inline `introMsg` / `msg2` / `msg6` locals | 57232 | 20044 | 58.8516 | already optimised away |

Key negative results:

* **Local caching of `Globals::status` is the wrong shape.** The original's
  `ldr r4,[r0,#0]` loads the *Status\* value* into r4 and then spills it at
  `[sp,#92]`; a `Status *status` local does reproduce that, but it re-colours
  the whole body (enemies→fp, player→r5, messages→r6) and lands at 57092 /
  48.2 %. The compiler only chooses r4 for the *address* temporary when r4 is
  free at the status load — which in the original happens because the
  `&this->m_pLevel` pointer was already spilled after `getPlayer`.
* **`Level *level` caching moves `messages` to r5** (the first-use axis the
  wave-132 note asked for) but only when `level` is also used in the body, and
  then `status` is still r9 and the size grows +84 B. It cannot get both
  `messages`→r5 and `status`→r4 without the faithful call order.

## Conclusion / next lead

The prologue register map is a **symptom of the body's live ranges**, not an
independent lever. The faithful source order `messages,player,enemies` (F) is
established by the emitted call order, and B additionally reproduces the
mission-1 `player` local; B's prologue is 2 instructions from the original.
But landing it needs a **-164 B body companion**, and the body is 125
instructions short of the original with 118 more 32-bit instructions. The
prologue is 6 instructions out of 20044, so even a perfect prologue cannot
raise the fuzzy score while the body differs.

Concrete next levers (unchanged from wave 132, now with the prologue settled):

1. Complete the ~125 missing body instructions (wave 131's missing
   `translate(-7000,500,17000)` at `0x13a4b8` is the first concrete one); the
   faithful `messages,player,enemies` order should then be size-neutral.
2. The body's 16/32-bit balance (original 13044/7786 vs baseline 12808/7904)
   is the dominant difference, not the prologue. A pass that reconstructs the
   low-register (r4–r7) allocation of the big state machines is the real lever.
3. Do **not** chase `Status *status` caching: it is a -140 B body re-colour
   (57092) and scores 48.2 %.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7746/score2.py                         # 58.851615, 57232/57232
python3 /dev/shm/w7746/batch1.py                         # 6 call-order permutations
python3 /dev/shm/w7746/batch12.py                        # 6 declaration permutations
```
