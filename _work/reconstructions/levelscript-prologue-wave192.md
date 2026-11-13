# `LevelScript::process` prologue — wave 192, 2026-11-13

Function `_ZN11LevelScript7processEi` @ `0x137d18`, 57,232 B. Current residual raw
53,203 / aligned 18,234 (master `16279fdd4`). This note localises the **first
divergence** and the only remaining prologue difference.

## Original prologue (0x137d18..0x137d56)

```
137d18  b5f0        push  {r4,r5,r6,r7,lr}
137d1a  af03        add   r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7,r8,r9,sl,fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub   sp, #432
137d26  4682        mov   sl, r0            <-- this  -> sl (r10)
137d28  48de        ldr   r0, [pc,#888]     <-- &Globals::globals
137d2a  4654        mov   r4, sl
137d2c  460e        mov   r6, r1
137d2e  4478        add   r0, pc
137d30  f8d0 b000   ldr.w fp, [r0]          <-- globals -> fp (r11)
137d34  f8db 0000   ldr.w r0, [fp]
137d38  906b        str   r0, [sp,#428]
137d3a  f854 0f18   ldr.w r0, [r4,#24]!     <-- &this->m_pLevel (pre-index)
137d3e  f72a e8d8   blx   Level::getMessages()
137d42  4605        mov   r5, r0
137d44  6820        ldr   r0, [r4,#0]         <-- RELOAD this->m_pLevel
137d46  f72a e976   blx   Level::getPlayer()
137d4a  e9cd 4014   strd  r4, r0, [sp,#80]
137d4e  6820        ldr   r0, [r4,#0]         <-- RELOAD this->m_pLevel
137d50  f72a e8b0   blx   Level::getEnemies()
137d54  4680        mov   r8, r0
```

## Finding 1 — the `Level *level` local is wrong (confirmed)

Our source had:

```cpp
Array<RadioMessage *> *messages = m_pLevel->getMessages();
Level *level = m_pLevel;                 // <-- caches the field in a register
PlayerEgo *player = level->getPlayer();
Array<KIPlayer *> *enemies = level->getEnemies();
```

The original reloads `[r4,#0]` (i.e. `this->m_pLevel`) before **each** call, so the
source must be:

```cpp
Array<RadioMessage *> *messages = m_pLevel->getMessages();
PlayerEgo *player = m_pLevel->getPlayer();
Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
```

Measured: with the local removed the prologue becomes **instruction-for-instruction
identical** to the original at `137d42..137d56` (`mov r5,r0` / `ldr r0,[r4]` /
`blx getPlayer` / `strd r4,r0,[sp,#80]` / `ldr r0,[r4]` / `blx getEnemies` /
`mov r8,r0`). Overall raw went 53,203 -> 53,322 (+119), aligned unchanged
18,234, size exact -- not a win on its own, because the rest of the function still
uses the cached `level` shape, but it **proves** the prologue's source form.

## Finding 2 — the only remaining prologue difference is a sl/fp swap

With Finding 1 applied, the first 20 instructions differ in exactly four words:

| addr | original | ours |
|---|---|---|
| `137d26` | `mov sl, r0` (this -> sl) | `mov fp, r0` (this -> fp) |
| `137d2a` | `mov r4, sl` | `mov r4, fp` |
| `137d30` | `ldr.w fp, [r0]` (globals -> fp) | `ldr.w sl, [r0]` (globals -> sl) |
| `137d34` | `ldr.w r0, [fp]` | `ldr.w r0, [sl]` |

Everything else (prologue, `getMessages`, `strd`, `getPlayer`, `getEnemies`, the
`Globals::status` load and `getCurrentCampaignMission` call) is identical. So the
whole 57 KB cascade starts from **`this` and `Globals::globals` being assigned to the
wrong callee-saved registers** (a pure swap of r10/r11).

Levers to try: make `Globals::globals` live from the first statement (so the
allocator prefers fp for it), or add/remove a callee-saved live range at entry so the
allocator's r10/r11 order flips. Do not chase later clusters until this swap is
resolved -- every downstream offset depends on it.
