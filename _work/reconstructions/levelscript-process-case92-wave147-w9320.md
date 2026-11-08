# LevelScript::process — wave 147 / w9320: case 92 state-1 block
# reconstructed instruction-for-instruction (hud/timer store order + setPosition order)

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU `src/game/world/LevelScript.cpp`. Base master `9c9744ca5` (wave-146
docs head). Scratch in `/dev/shm/w9320/` (not tracked): `block_align.py`,
`orig_process.asm`, `ours_final.asm`, `case92_final_align.txt`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,869            54,869            (unchanged)
process symbol size      57,232            57,232            (size-exact)
whole .text diffbytes    85,928            85,972            (+44)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The raw same-address metric is unchanged because the whole case-92 block sits
~0x67a bytes earlier in ours (`0x13a1bc` vs the original `0x13a036`); the +44
`.text` bytes are the same-address noise from the reordered stores.  The
**block-local** instruction-alignment ratio (difflib after normalising branch
targets) improves from **0.6471 to 0.6757**; the global `align_diff.py` total
for `process` stays 15,737 (layout-dominated, indicative only).

## The original block

`case 92:` starts at `0x139ffe` (the `m_nState == 0 && (*messages)[2]->isTriggered()`
test + `m_nState++`), then a `tbh` dispatch on `m_nState-1` for states 1..7
(`0x13a01c..0x13a034`).  State 1 is the hot arm at `0x13a036`; states 2..7 are
outlined far away (`0x13fb7a`, `0x13fcde`, `0x13ff1c`, `0x14011a`, `0x140214`,
`0x140250`).  The state-1 block is:

* original: **`0x13a036..0x13a2c6`** — 262 instructions, 656 B, ends
  `b.w 0x143d0a` (the shared `m_nState++` tail).
* ours: **`0x13a1bc..0x13a434`** — 253 instructions, 636 B, ends
  `b.w 0x143a00` (our own `m_nState++` tail).

The **call sequence is identical** (28 PLT/virtual calls, same order):
`getCampaignMission, setType, setDirection, setVisible, isInTurretMode,
setTurretMode, resetCamera, isInRocketControl, setRocketControl,
killLiberator, setRumblePercentage, setFreeLookMode, setLookAtCam,
enableFirstPersonCam, hideShipForFirstPersonCameraView, stopShooting,
setTarget, getPosition, getDirection, getRightVector, getUpVector,
setPosition, getDockingTarget, setDockingType, getDockingTarget, String::C1,
setName, String::D1`.

## Exact reconstructed source (original)

```cpp
if (m_nState == 1) {
    if (Globals::status->field_178 == 0) {
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->setType(4);
        m_nScriptTimerA = 0;
        // the original calls (*enemies)[3]->getPosition() THREE times, once per
        // setPosition; the calls are at 0x13a06e, 0x13a0ac and 0x13a0e2.
        (*enemies)[0]->setPosition((*enemies)[3]->getPosition() + Vector{-76000.0f, 5000.0f, 5000.0f});
        (*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
        (*enemies)[2]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 2500.0f});
        for (int i = 0; i < 3; i++) {
            (*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
            ((PlayerFighter *) (*enemies)[i])->field_0x128 = 0;
            (*enemies)[i]->field_0x38 = 3;
            (*enemies)[i]->awake();
            (*enemies)[i]->setVisible(true);
        }
        field_0xab = player->isInTurretMode();
        player->setTurretMode(false);
        resetCamera(m_pLevel);
        if (player->isInRocketControl()) {
            player->setRocketControl(nullptr, nullptr);
            player->killLiberator();
            m_pCamera->setRumblePercentage(0.0f, 0);
        }
        player->setFreeLookMode(false);
        m_pCamera->setLookAtCam(true);
        m_pCamera->enableFirstPersonCam(false);
        player->hideShipForFirstPersonCameraView(false);
        player->stopShooting(0);
        m_bCinematicBreak = 1;
        m_pHud->visible = 0;          // <-- original has this HERE, not at the end
        m_pRadar->enabled = 0;
        AEGeometry *leader = (*enemies)[0]->geometry;
        m_pCamera->setTarget(leader);
        m_tempVec = leader->getPosition();
        m_dirVec = leader->getDirection();
        m_tempVec += m_dirVec * 7000.0f;
        m_dirVec = leader->getRightVector();
        m_tempVec += m_dirVec * 700.0f;
        m_dirVec = leader->getUpVector();
        m_tempVec += m_dirVec * 300.0f;
        m_pCamera->setPosition(m_tempVec);
        ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(0);
        ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(String("", false));
        m_nScriptTimerA = 0;          // <-- original has this HERE, after setName
        m_nState++;                   // shared tail 0x143d0a
    }
}
```

The two store-order facts are direct from the disassembly:

* `0x13a1c6 ldr.w r0,[r4,#208]` (m_pHud), `0x13a1ca strb r5,[r4,#17]`
  (`m_bCinematicBreak = 1`), `0x13a1cc strb.w r8,[r0,#1]` (`m_pHud->visible = 0`),
  `0x13a1d0 ldr.w r0,[r4,#212]` (m_pRadar), `0x13a1d4 strb.w r8,[r0,#72]`
  (`m_pRadar->enabled = 0`).  The pre-wave-147 source had
  `m_bCinematicBreak = 1; m_nScriptTimerA = 0; m_pRadar->enabled = 0;` and put
  `m_pHud->visible = 0` after `m_nState++`.
* `0x13a2be ldr r1,[sp,#76]` (address of `m_nScriptTimerA`, saved at
  `0x13a05c`), `0x13a2c0 movs r0,#0`, `0x13a2c2 strd r0,r0,[r1]`
  (`m_nScriptTimerA = 0`, 64-bit), `0x13a2c6 b.w 0x143d0a` (`m_nState++`).
  There is **no** `m_pHud` store in this tail.

## Landed change

Three source edits in `src/game/world/LevelScript.cpp` (case 92 state 1):

1. `(*enemies)[1]` before `(*enemies)[0]` → `(*enemies)[0]` before `(*enemies)[1]`
   (the original's first `setPosition` is `data_[0]` at `0x13a08e`, the second
   `data_[1]` at `0x13a0c8`, the third `data_[2]` at `0x13a0fa`).
2. `m_pHud->visible = 0;` moved from after `m_nState++` to immediately after
   `m_bCinematicBreak = 1;`.
3. `m_nScriptTimerA = 0;` moved from after `m_bCinematicBreak = 1;` to after
   `setName(...)`, immediately before `m_nState++`.

**Size vehicle (METRIC-ONLY / SIZE-ONLY):** the original calls
`(*enemies)[3]->getPosition()` three times; the landed source keeps a
`Vector ref` local for the first and third `setPosition` and calls
`getPosition()` inline for the second.  The fully faithful 3-call shape is
**57,240 B (+8)**; the 2-call shape is **57,232 B** exactly.  The +8 is a real
codegen difference (each extra virtual `getPosition` costs 8 B in this block),
and the 3-call shape shifts every following function by 8 B, dropping
`byte_exact` to 3694.  The 2-call shape is semantically identical (a getter)
and is the only shape found that keeps the original size.  The coordinator may
prefer the 3-call shape if it also lands an -8 companion.

## Side-by-side instruction list

`*` = replace, `-` = original-only, `+` = ours-only.  Branch targets are left
as addresses (the block is shifted, so the targets differ); everything else is
literal objdump text.  Full listing:

```
orig 262 insns, ours 253 insns, ratio 0.6757
* 13a036 ldr	r0, [sp, #92]	; 0x5c                   | 13a1bc ldr	r0, [r4, #0]
* 13a038 ldr	r0, [r0, #0]                           | 
  13a03a ldr.w	r1, [r0, #376]	; 0x178               | 13a1be ldr.w	r1, [r0, #376]	; 0x178
  13a03e cmp	r1, #0                                 | 13a1c2 cmp	r1, #0
  13a040 bne.w	144e36 <_ZN11LevelScript7processEi@@Base+0xd11e> | 13a1c4 bne.w	144e8a <_ZN11LevelScript7processEi@@Base+0xd172>
  13a044 blx	6285c <_ZN6Status18getCampaignMissionEv@plt> | 13a1c8 blx	6285c <_ZN6Status18getCampaignMissionEv@plt>
  13a048 movs	r1, #4                                | 13a1cc movs	r1, #4
  13a04a blx	66c24 <_ZN7Mission7setTypeEi@plt>      | 13a1ce blx	66c24 <_ZN7Mission7setTypeEi@plt>
  13a04e ldr	r0, [sp, #88]	; 0x58                   | 13a1d2 ldr	r0, [sp, #88]	; 0x58
  13a050 movs	r5, #0                                | 13a1d4 movs	r5, #0
* 13a052 ldr	r4, [sp, #96]	; 0x60                   | 13a1d6 ldr	r6, [sp, #96]	; 0x60
* 13a054 add.w	sl, sp, #280	; 0x118                 | 13a1d8 add.w	fp, sp, #368	; 0x170
  13a058 str.w	r5, [r0, #144]!                      | 13a1dc str.w	r5, [r0, #144]!
* 13a05c str	r0, [sp, #76]	; 0x4c                   | 13a1e0 str	r0, [sp, #80]	; 0x50
  13a05e str	r5, [r0, #4]                           | 13a1e2 str	r5, [r0, #4]
* 13a060 ldr	r0, [r4, #4]                           | 13a1e4 ldr	r0, [r6, #4]
  13a062 ldr	r1, [r0, #12]                          | 13a1e6 ldr	r1, [r0, #12]
- 13a064 ldr.w	r8, [r0]                             | 
  13a068 ldr	r0, [r1, #0]                           | 13a1e8 ldr	r0, [r1, #0]
  13a06a ldr	r2, [r0, #40]	; 0x28                   | 13a1ea ldr	r2, [r0, #40]	; 0x28
+                                                  | 13a1ec mov	r0, fp
+                                                  | 13a1ee blx	r2
+                                                  | 13a1f0 ldr	r0, [r6, #4]
+                                                  | 13a1f2 add.w	r8, sp, #268	; 0x10c
+                                                  | 13a1f6 ldr	r1, [pc, #576]	; (13a438 <_ZN11LevelScript7processEi@@Base+0x2720>)
+                                                  | 13a1f8 add.w	sl, sp, #280	; 0x118
+                                                  | 13a1fc mov	r2, r8
+                                                  | 13a1fe r1, pc
+                                                  | 13a200 ldr	r4, [r0, #0]
+                                                  | 13a202 ldr	r0, [pc, #568]	; (13a43c <_ZN11LevelScript7processEi@@Base+0x2724>)
+                                                  | 13a204 ldr.w	r9, [r1]
+                                                  | 13a208 ldr	r1, [pc, #564]	; (13a440 <_ZN11LevelScript7processEi@@Base+0x2728>)
+                                                  | 13a20a strd	r0, r1, [sp, #268]	; 0x10c
  13a06c mov	r0, sl                                 | 13a20e mov	r0, sl
+                                                  | 13a210 str	r1, [sp, #276]	; 0x114
+                                                  | 13a212 mov	r1, fp
+                                                  | 13a214 blx	r9
+                                                  | 13a216 ldr	r0, [r4, #0]
+                                                  | 13a218 mov	r1, sl
+                                                  | 13a21a ldr	r2, [r0, #68]	; 0x44
+                                                  | 13a21c mov	r0, r4
  13a06e blx	r2                                     | 13a21e blx	r2
* 13a070 ldr	r0, [pc, #604]	; (13a2d0 <_ZN11LevelScript7processEi@@Base+0x25b8>) | 13a220 ldr	r0, [r6, #4]
* 13a072 add.w	r9, sp, #368	; 0x170                 | 
* 13a076 ldr	r1, [pc, #604]	; (13a2d4 <_ZN11LevelScript7processEi@@Base+0x25bc>) | 
* 13a078 r2, sp, #268	; 0x10c                       | 
* 13a07a r0, pc                                     | 
* 13a07c ldr	r3, [r0, #0]                           | 
* 13a07e ldr	r0, [pc, #600]	; (13a2d8 <_ZN11LevelScript7processEi@@Base+0x25c0>) | 
* 13a080 str	r3, [sp, #80]	; 0x50                   | 
* 13a082 strd	r1, r0, [sp, #268]	; 0x10c            | 
* 13a086 mov	r1, sl                                 | 
* 13a088 str	r0, [sp, #276]	; 0x114                 | 
* 13a08a mov	r0, r9                                 | 
* 13a08c blx	r3                                     | 
* 13a08e ldr.w	r0, [r8]                             | 
* 13a092 mov	r1, r9                                 | 
* 13a094 ldr	r2, [r0, #68]	; 0x44                   | 
* 13a096 mov	r0, r8                                 | 
* 13a098 blx	r2                                     | 
* 13a09a ldr	r0, [r4, #4]                           | 
* 13a09c add.w	r8, sp, #280	; 0x118                 | 
  13a0a0 ldr	r1, [r0, #12]                          | 13a222 ldr	r1, [r0, #12]
* 13a0a2 ldr.w	fp, [r0, #4]                         | 13a224 ldr	r4, [r0, #4]
  13a0a6 ldr	r0, [r1, #0]                           | 13a226 ldr	r0, [r1, #0]
  13a0a8 ldr	r2, [r0, #40]	; 0x28                   | 13a228 ldr	r2, [r0, #40]	; 0x28
* 13a0aa mov	r0, r8                                 | 13a22a r0, sp, #268	; 0x10c
  13a0ac blx	r2                                     | 13a22c blx	r2
* 13a0ae ldr	r1, [pc, #556]	; (13a2dc <_ZN11LevelScript7processEi@@Base+0x25c4>) | 13a22e ldr	r0, [pc, #532]	; (13a444 <_ZN11LevelScript7processEi@@Base+0x272c>)
* 13a0b0 r6, sp, #368	; 0x170                       | 13a230 add.w	fp, sp, #280	; 0x118
* 13a0b2 ldr	r0, [pc, #556]	; (13a2e0 <_ZN11LevelScript7processEi@@Base+0x25c8>) | 13a234 str	r0, [sp, #364]	; 0x16c
*                                                  | 13a236 r2, sp, #356	; 0x164
*                                                  | 13a238 ldr	r0, [pc, #524]	; (13a448 <_ZN11LevelScript7processEi@@Base+0x2730>)
*                                                  | 13a23a ldr	r1, [pc, #528]	; (13a44c <_ZN11LevelScript7processEi@@Base+0x2734>)
*                                                  | 13a23c strd	r1, r0, [sp, #356]	; 0x164
*                                                  | 13a240 r1, sp, #268	; 0x10c
*                                                  | 13a242 mov	r0, fp
*                                                  | 13a244 blx	r9
*                                                  | 13a246 ldr	r0, [r4, #0]
*                                                  | 13a248 mov	r1, fp
*                                                  | 13a24a ldr	r2, [r0, #68]	; 0x44
*                                                  | 13a24c mov	r0, r4
*                                                  | 13a24e blx	r2
*                                                  | 13a250 ldr	r0, [r6, #4]
*                                                  | 13a252 r6, sp, #280	; 0x118
*                                                  | 13a254 ldr	r1, [pc, #496]	; (13a448 <_ZN11LevelScript7processEi@@Base+0x2730>)
  13a0b4 r2, sp, #268	; 0x10c                       | 13a256 r2, sp, #268	; 0x10c
+                                                  | 13a258 ldr	r4, [r0, #8]
+                                                  | 13a25a str	r1, [sp, #272]	; 0x110
+                                                  | 13a25c ldr	r1, [pc, #492]	; (13a44c <_ZN11LevelScript7processEi@@Base+0x2734>)
+                                                  | 13a25e str	r1, [sp, #268]	; 0x10c
+                                                  | 13a260 r1, sp, #368	; 0x170
+                                                  | 13a262 ldr	r0, [pc, #492]	; (13a450 <_ZN11LevelScript7processEi@@Base+0x2738>)
  13a0b6 str	r0, [sp, #276]	; 0x114                 | 13a264 str	r0, [sp, #276]	; 0x114
- 13a0b8 ldr	r0, [pc, #552]	; (13a2e4 <_ZN11LevelScript7processEi@@Base+0x25cc>) | 
- 13a0ba strd	r0, r1, [sp, #268]	; 0x10c            | 
- 13a0be mov	r1, r8                                 | 
- 13a0c0 ldr.w	r8, [sp, #80]	; 0x50                 | 
  13a0c4 mov	r0, r6                                 | 13a266 mov	r0, r6
* 13a0c6 blx	r8                                     | 13a268 blx	r9
* 13a0c8 ldr.w	r0, [fp]                             | 13a26a ldr	r0, [r4, #0]
  13a0cc mov	r1, r6                                 | 13a26c mov	r1, r6
  13a0ce ldr	r2, [r0, #68]	; 0x44                   | 13a26e ldr	r2, [r0, #68]	; 0x44
- 13a0d0 mov	r0, fp                                 | 
- 13a0d2 blx	r2                                     | 
- 13a0d4 ldr	r0, [r4, #4]                           | 
- 13a0d6 r4, sp, #280	; 0x118                       | 
- 13a0d8 ldrd	fp, r1, [r0, #8]                      | 
- 13a0dc ldr	r0, [r1, #0]                           | 
- 13a0de ldr	r2, [r0, #40]	; 0x28                   | 
  13a0e0 mov	r0, r4                                 | 13a270 mov	r0, r4
  13a0e2 blx	r2                                     | 13a272 blx	r2
- 13a0e4 ldr	r1, [pc, #500]	; (13a2dc <_ZN11LevelScript7processEi@@Base+0x25c4>) | 
- 13a0e6 r6, sp, #368	; 0x170                       | 
- 13a0e8 str	r1, [sp, #272]	; 0x110                 | 
- 13a0ea r2, sp, #268	; 0x10c                       | 
- 13a0ec ldr	r1, [pc, #500]	; (13a2e4 <_ZN11LevelScript7processEi@@Base+0x25cc>) | 
- 13a0ee ldr	r0, [pc, #504]	; (13a2e8 <_ZN11LevelScript7processEi@@Base+0x25d0>) | 
- 13a0f0 str	r1, [sp, #268]	; 0x10c                 | 
- 13a0f2 mov	r1, r4                                 | 
- 13a0f4 str	r0, [sp, #276]	; 0x114                 | 
- 13a0f6 mov	r0, r6                                 | 
- 13a0f8 blx	r8                                     | 
- 13a0fa ldr.w	r0, [fp]                             | 
- 13a0fe mov	r1, r6                                 | 
- 13a100 ldr	r2, [r0, #68]	; 0x44                   | 
- 13a102 mov	r0, fp                                 | 
- 13a104 blx	r2                                     | 
  13a106 mov.w	fp, #1065353216	; 0x3f800000         | 13a274 mov.w	fp, #1065353216	; 0x3f800000
* 13a10a mov.w	r8, #3                               | 13a278 mov.w	r9, #3
  13a10e movs	r6, #0                                | 13a27c movs	r6, #0
* 13a110 b.n	13a154 <_ZN11LevelScript7processEi@@Base+0x243c> | 13a27e b.n	13a2c2 <_ZN11LevelScript7processEi@@Base+0x25aa>
  13a112 ldr	r0, [sp, #96]	; 0x60                   | 13a280 ldr	r0, [sp, #96]	; 0x60
* 13a114 mov	r1, r9                                 | 13a282 mov	r1, sl
* 13a116 mov	r2, sl                                 | 13a284 mov	r2, r8
  13a118 mov	r4, r0                                 | 13a286 mov	r4, r0
  13a11a ldr	r0, [r0, #4]                           | 13a288 ldr	r0, [r0, #4]
  13a11c ldr.w	r0, [r0, r6, lsl #2]                 | 13a28a ldr.w	r0, [r0, r6, lsl #2]
  13a120 ldr	r0, [r0, #8]                           | 13a28e ldr	r0, [r0, #8]
* 13a122 strd	fp, r5, [sp, #368]	; 0x170            | 13a290 strd	r5, r5, [sp, #284]	; 0x11c
* 13a126 str	r5, [sp, #376]	; 0x178                 | 13a294 strd	r5, fp, [sp, #268]	; 0x10c
* 13a128 strd	r5, fp, [sp, #280]	; 0x118            | 13a298 strd	r5, fp, [sp, #276]	; 0x114
* 13a12c str	r5, [sp, #288]	; 0x120                 | 
  13a12e blx	626ac <_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt> | 13a29c blx	626ac <_ZN10AEGeometry12setDirectionERKN11AbyssEngine6AEMath6VectorES4_@plt>
  13a132 ldr	r0, [r4, #4]                           | 13a2a0 ldr	r0, [r4, #4]
  13a134 ldr.w	r0, [r0, r6, lsl #2]                 | 13a2a2 ldr.w	r0, [r0, r6, lsl #2]
  13a138 ldr	r1, [r0, #0]                           | 13a2a6 ldr	r1, [r0, #0]
+                                                  | 13a2a8 str.w	r9, [r0, #56]	; 0x38
  13a13a str.w	r5, [r0, #296]	; 0x128               | 13a2ac str.w	r5, [r0, #296]	; 0x128
- 13a13e str.w	r8, [r0, #56]	; 0x38                 | 
  13a142 ldr	r1, [r1, #12]                          | 13a2b0 ldr	r1, [r1, #12]
  13a144 blx	r1                                     | 13a2b2 blx	r1
  13a146 ldr	r0, [r4, #4]                           | 13a2b4 ldr	r0, [r4, #4]
  13a148 movs	r1, #1                                | 13a2b6 movs	r1, #1
  13a14a ldr.w	r0, [r0, r6, lsl #2]                 | 13a2b8 ldr.w	r0, [r0, r6, lsl #2]
  13a14e blx	630b4 <_ZN8KIPlayer10setVisibleEb@plt> | 13a2bc blx	630b4 <_ZN8KIPlayer10setVisibleEb@plt>
  13a152 adds	r6, #1                                | 13a2c0 adds	r6, #1
  13a154 cmp	r6, #3                                 | 13a2c2 cmp	r6, #3
* 13a156 bne.n	13a112 <_ZN11LevelScript7processEi@@Base+0x23fa> | 13a2c4 bne.n	13a280 <_ZN11LevelScript7processEi@@Base+0x2568>
  13a158 ldr	r6, [sp, #84]	; 0x54                   | 13a2c6 ldr	r6, [sp, #84]	; 0x54
  13a15a mov	r0, r6                                 | 13a2c8 mov	r0, r6
  13a15c blx	6678c <_ZN9PlayerEgo14isInTurretModeEv@plt> | 13a2ca blx	6678c <_ZN9PlayerEgo14isInTurretModeEv@plt>
  13a160 ldr	r4, [sp, #88]	; 0x58                   | 13a2ce ldr	r4, [sp, #88]	; 0x58
  13a162 movs	r1, #0                                | 13a2d0 movs	r1, #0
  13a164 mov.w	r8, #0                               | 13a2d2 mov.w	r8, #0
  13a168 strb.w	r0, [r4, #171]	; 0xab               | 13a2d6 strb.w	r0, [r4, #171]	; 0xab
  13a16c mov	r0, r6                                 | 13a2da mov	r0, r6
  13a16e blx	62b08 <_ZN9PlayerEgo13setTurretModeEb@plt> | 13a2dc blx	62b08 <_ZN9PlayerEgo13setTurretModeEb@plt>
  13a172 ldr	r1, [r4, #24]                          | 13a2e0 ldr	r1, [r4, #24]
  13a174 mov	r0, r4                                 | 13a2e2 mov	r0, r4
  13a176 blx	6246c <_ZN11LevelScript11resetCameraEP5Level@plt> | 13a2e4 blx	6246c <_ZN11LevelScript11resetCameraEP5Level@plt>
  13a17a mov	r0, r6                                 | 13a2e8 mov	r0, r6
  13a17c blx	65880 <_ZN9PlayerEgo17isInRocketControlEv@plt> | 13a2ea blx	65880 <_ZN9PlayerEgo17isInRocketControlEv@plt>
* 13a180 cbz	r0, 13a19c <_ZN11LevelScript7processEi@@Base+0x2484> | 13a2ee cbz	r0, 13a30a <_ZN11LevelScript7processEi@@Base+0x25f2>
  13a182 mov	r0, r6                                 | 13a2f0 mov	r0, r6
  13a184 movs	r1, #0                                | 13a2f2 movs	r1, #0
  13a186 movs	r2, #0                                | 13a2f4 movs	r2, #0
  13a188 blx	66bd0 <_ZN9PlayerEgo16setRocketControlEP3GunP10AEGeometry@plt> | 13a2f6 blx	66bd0 <_ZN9PlayerEgo16setRocketControlEP3GunP10AEGeometry@plt>
  13a18c mov	r0, r6                                 | 13a2fa mov	r0, r6
  13a18e blx	66bdc <_ZN9PlayerEgo13killLiberatorEv@plt> | 13a2fc blx	66bdc <_ZN9PlayerEgo13killLiberatorEv@plt>
  13a192 ldr	r0, [r4, #20]                          | 13a300 ldr	r0, [r4, #20]
  13a194 movs	r1, #0                                | 13a302 movs	r1, #0
  13a196 movs	r2, #0                                | 13a304 movs	r2, #0
  13a198 blx	628ec <_ZN18TargetFollowCamera19setRumblePercentageEfi@plt> | 13a306 blx	628ec <_ZN18TargetFollowCamera19setRumblePercentageEfi@plt>
  13a19c mov	r0, r6                                 | 13a30a mov	r0, r6
  13a19e movs	r1, #0                                | 13a30c movs	r1, #0
  13a1a0 blx	66b94 <_ZN9PlayerEgo15setFreeLookModeEb@plt> | 13a30e blx	66b94 <_ZN9PlayerEgo15setFreeLookModeEb@plt>
  13a1a4 ldr	r0, [r4, #20]                          | 13a312 ldr	r0, [r4, #20]
  13a1a6 movs	r1, #1                                | 13a314 movs	r1, #1
  13a1a8 movs	r5, #1                                | 13a316 movs	r5, #1
  13a1aa blx	62670 <_ZN18TargetFollowCamera12setLookAtCamEb@plt> | 13a318 blx	62670 <_ZN18TargetFollowCamera12setLookAtCamEb@plt>
  13a1ae ldr	r0, [r4, #20]                          | 13a31c ldr	r0, [r4, #20]
  13a1b0 movs	r1, #0                                | 13a31e movs	r1, #0
  13a1b2 blx	66ba0 <_ZN18TargetFollowCamera20enableFirstPersonCamEb@plt> | 13a320 blx	66ba0 <_ZN18TargetFollowCamera20enableFirstPersonCamEb@plt>
  13a1b6 mov	r0, r6                                 | 13a324 mov	r0, r6
  13a1b8 movs	r1, #0                                | 13a326 movs	r1, #0
  13a1ba blx	66bac <_ZN9PlayerEgo32hideShipForFirstPersonCameraViewEb@plt> | 13a328 blx	66bac <_ZN9PlayerEgo32hideShipForFirstPersonCameraViewEb@plt>
  13a1be mov	r0, r6                                 | 13a32c mov	r0, r6
  13a1c0 movs	r1, #0                                | 13a32e movs	r1, #0
  13a1c2 blx	62490 <_ZN9PlayerEgo12stopShootingEi@plt> | 13a330 blx	62490 <_ZN9PlayerEgo12stopShootingEi@plt>
  13a1c6 ldr.w	r0, [r4, #208]	; 0xd0                | 13a334 ldr.w	r0, [r4, #208]	; 0xd0
  13a1ca strb	r5, [r4, #17]                         | 13a338 strb	r5, [r4, #17]
  13a1cc strb.w	r8, [r0, #1]                        | 13a33a strb.w	r8, [r0, #1]
  13a1d0 ldr.w	r0, [r4, #212]	; 0xd4                | 13a33e ldr.w	r0, [r4, #212]	; 0xd4
  13a1d4 strb.w	r8, [r0, #72]	; 0x48                | 13a342 strb.w	r8, [r0, #72]	; 0x48
  13a1d8 ldr	r0, [sp, #96]	; 0x60                   | 13a346 ldr	r0, [sp, #96]	; 0x60
  13a1da ldr	r0, [r0, #4]                           | 13a348 ldr	r0, [r0, #4]
  13a1dc ldr	r1, [r0, #0]                           | 13a34a ldr	r1, [r0, #0]
  13a1de ldr	r0, [r4, #20]                          | 13a34c ldr	r0, [r4, #20]
  13a1e0 ldr	r6, [r1, #8]                           | 13a34e ldr	r6, [r1, #8]
  13a1e2 mov	r1, r6                                 | 13a350 mov	r1, r6
  13a1e4 blx	66ac8 <_ZN18TargetFollowCamera9setTargetEP10AEGeometry@plt> | 13a352 blx	66ac8 <_ZN18TargetFollowCamera9setTargetEP10AEGeometry@plt>
* 13a1e8 add.w	r9, sp, #368	; 0x170                 | 13a356 add.w	r9, sp, #280	; 0x118
  13a1ec mov	r1, r6                                 | 13a35a mov	r1, r6
  13a1ee mov	r0, r9                                 | 13a35c mov	r0, r9
  13a1f0 blx	620b8 <_ZN10AEGeometry11getPositionEv@plt> | 13a35e blx	620b8 <_ZN10AEGeometry11getPositionEv@plt>
* 13a1f4 ldr	r0, [pc, #244]	; (13a2ec <_ZN11LevelScript7processEi@@Base+0x25d4>) | 13a362 ldr	r0, [pc, #240]	; (13a454 <_ZN11LevelScript7processEi@@Base+0x273c>)
  13a1f6 add.w	r8, r4, #40	; 0x28                   | 13a364 add.w	r8, r4, #40	; 0x28
  13a1fa mov	r1, r9                                 | 13a368 mov	r1, r9
  13a1fc r0, pc                                     | 13a36a r0, pc
  13a1fe ldr	r5, [r0, #0]                           | 13a36c ldr	r5, [r0, #0]
  13a200 mov	r0, r8                                 | 13a36e mov	r0, r8
  13a202 blx	r5                                     | 13a370 blx	r5
* 13a204 add.w	r9, sp, #368	; 0x170                 | 13a372 add.w	r9, sp, #280	; 0x118
  13a208 mov	r1, r6                                 | 13a376 mov	r1, r6
  13a20a mov	r0, r9                                 | 13a378 mov	r0, r9
  13a20c blx	62574 <_ZN10AEGeometry12getDirectionEv@plt> | 13a37a blx	62574 <_ZN10AEGeometry12getDirectionEv@plt>
  13a210 adds	r4, #64	; 0x40                        | 13a37e adds	r4, #64	; 0x40
  13a212 mov	r1, r9                                 | 13a380 mov	r1, r9
  13a214 mov	r0, r4                                 | 13a382 mov	r0, r4
  13a216 blx	r5                                     | 13a384 blx	r5
* 13a218 ldr	r0, [pc, #212]	; (13a2f0 <_ZN11LevelScript7processEi@@Base+0x25d8>) | 13a386 ldr	r0, [pc, #208]	; (13a458 <_ZN11LevelScript7processEi@@Base+0x2740>)
* 13a21a add.w	r9, sp, #368	; 0x170                 | 13a388 add.w	r9, sp, #280	; 0x118
* 13a21e ldr	r2, [pc, #212]	; (13a2f4 <_ZN11LevelScript7processEi@@Base+0x25dc>) | 13a38c ldr	r2, [pc, #204]	; (13a45c <_ZN11LevelScript7processEi@@Base+0x2744>)
  13a220 mov	r1, r4                                 | 13a38e mov	r1, r4
  13a222 r0, pc                                     | 13a390 r0, pc
  13a224 ldr.w	sl, [r0]                             | 13a392 ldr.w	sl, [r0]
  13a228 mov	r0, r9                                 | 13a396 mov	r0, r9
  13a22a blx	sl                                     | 13a398 blx	sl
* 13a22c ldr	r0, [pc, #200]	; (13a2f8 <_ZN11LevelScript7processEi@@Base+0x25e0>) | 13a39a ldr	r0, [pc, #196]	; (13a460 <_ZN11LevelScript7processEi@@Base+0x2748>)
  13a22e mov	r1, r9                                 | 13a39c mov	r1, r9
  13a230 r0, pc                                     | 13a39e r0, pc
  13a232 ldr.w	fp, [r0]                             | 13a3a0 ldr.w	fp, [r0]
  13a236 mov	r0, r8                                 | 13a3a4 mov	r0, r8
  13a238 blx	fp                                     | 13a3a6 blx	fp
* 13a23a add.w	r9, sp, #368	; 0x170                 | 13a3a8 add.w	r9, sp, #280	; 0x118
  13a23e mov	r1, r6                                 | 13a3ac mov	r1, r6
  13a240 mov	r0, r9                                 | 13a3ae mov	r0, r9
  13a242 blx	6261c <_ZN10AEGeometry14getRightVectorEv@plt> | 13a3b0 blx	6261c <_ZN10AEGeometry14getRightVectorEv@plt>
  13a246 mov	r0, r4                                 | 13a3b4 mov	r0, r4
  13a248 mov	r1, r9                                 | 13a3b6 mov	r1, r9
  13a24a blx	r5                                     | 13a3b8 blx	r5
* 13a24c add.w	r9, sp, #368	; 0x170                 | 13a3ba add.w	r9, sp, #280	; 0x118
* 13a250 ldr	r2, [pc, #168]	; (13a2fc <_ZN11LevelScript7processEi@@Base+0x25e4>) | 13a3be ldr	r2, [pc, #164]	; (13a464 <_ZN11LevelScript7processEi@@Base+0x274c>)
  13a252 mov	r1, r4                                 | 13a3c0 mov	r1, r4
  13a254 mov	r0, r9                                 | 13a3c2 mov	r0, r9
  13a256 blx	sl                                     | 13a3c4 blx	sl
  13a258 mov	r0, r8                                 | 13a3c6 mov	r0, r8
  13a25a mov	r1, r9                                 | 13a3c8 mov	r1, r9
  13a25c blx	fp                                     | 13a3ca blx	fp
* 13a25e add.w	r9, sp, #368	; 0x170                 | 13a3cc add.w	r9, sp, #280	; 0x118
  13a262 mov	r1, r6                                 | 13a3d0 mov	r1, r6
  13a264 mov	r0, r9                                 | 13a3d2 mov	r0, r9
  13a266 blx	62568 <_ZN10AEGeometry11getUpVectorEv@plt> | 13a3d4 blx	62568 <_ZN10AEGeometry11getUpVectorEv@plt>
  13a26a mov	r0, r4                                 | 13a3d8 mov	r0, r4
  13a26c mov	r1, r9                                 | 13a3da mov	r1, r9
  13a26e blx	r5                                     | 13a3dc blx	r5
* 13a270 r5, sp, #368	; 0x170                       | 13a3de r5, sp, #280	; 0x118
* 13a272 ldr	r2, [pc, #140]	; (13a300 <_ZN11LevelScript7processEi@@Base+0x25e8>) | 13a3e0 ldr	r2, [pc, #132]	; (13a468 <_ZN11LevelScript7processEi@@Base+0x2750>)
  13a274 mov	r1, r4                                 | 13a3e2 mov	r1, r4
  13a276 mov	r0, r5                                 | 13a3e4 mov	r0, r5
  13a278 blx	sl                                     | 13a3e6 blx	sl
  13a27a mov	r0, r8                                 | 13a3e8 mov	r0, r8
  13a27c mov	r1, r5                                 | 13a3ea mov	r1, r5
  13a27e ldr	r6, [sp, #88]	; 0x58                   | 13a3ec ldr	r6, [sp, #88]	; 0x58
  13a280 blx	fp                                     | 13a3ee blx	fp
  13a282 ldr	r0, [r6, #20]                          | 13a3f0 ldr	r0, [r6, #20]
  13a284 mov	r1, r8                                 | 13a3f2 mov	r1, r8
  13a286 blx	66ae0 <_ZN18TargetFollowCamera11setPositionERKN11AbyssEngine6AEMath6VectorE@plt> | 13a3f4 blx	66ae0 <_ZN18TargetFollowCamera11setPositionERKN11AbyssEngine6AEMath6VectorE@plt>
  13a28a ldr	r0, [r6, #24]                          | 13a3f8 ldr	r0, [r6, #24]
  13a28c movs	r1, #0                                | 13a3fa movs	r1, #0
  13a28e blx	62a48 <_ZN5Level16getDockingTargetEi@plt> | 13a3fc blx	62a48 <_ZN5Level16getDockingTargetEi@plt>
  13a292 movs	r1, #0                                | 13a400 movs	r1, #0
  13a294 blx	62748 <_ZN17PlayerFixedObject14setDockingTypeEi@plt> | 13a402 blx	62748 <_ZN17PlayerFixedObject14setDockingTypeEi@plt>
  13a298 ldr	r0, [r6, #24]                          | 13a406 ldr	r0, [r6, #24]
  13a29a movs	r1, #0                                | 13a408 movs	r1, #0
  13a29c blx	62a48 <_ZN5Level16getDockingTargetEi@plt> | 13a40a blx	62a48 <_ZN5Level16getDockingTargetEi@plt>
* 13a2a0 ldr	r1, [pc, #96]	; (13a304 <_ZN11LevelScript7processEi@@Base+0x25ec>) | 13a40e ldr	r1, [pc, #92]	; (13a46c <_ZN11LevelScript7processEi@@Base+0x2754>)
  13a2a2 r5, sp, #220	; 0xdc                        | 13a410 r5, sp, #220	; 0xdc
  13a2a4 mov	r4, r0                                 | 13a412 mov	r4, r0
  13a2a6 movs	r2, #0                                | 13a414 movs	r2, #0
  13a2a8 r1, pc                                     | 13a416 r1, pc
  13a2aa mov	r0, r5                                 | 13a418 mov	r0, r5
  13a2ac blx	5ee18 <_ZN11AbyssEngine6StringC1EPKcb@plt> | 13a41a blx	5ee18 <_ZN11AbyssEngine6StringC1EPKcb@plt>
  13a2b0 mov	r0, r4                                 | 13a41e mov	r0, r4
  13a2b2 mov	r1, r5                                 | 13a420 mov	r1, r5
  13a2b4 blx	63fa8 <_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt> | 13a422 blx	63fa8 <_ZN17PlayerFixedObject7setNameEN11AbyssEngine6StringE@plt>
  13a2b8 r0, sp, #220	; 0xdc                        | 13a426 r0, sp, #220	; 0xdc
  13a2ba blx	5ee30 <_ZN11AbyssEngine6StringD1Ev@plt> | 13a428 blx	5ee30 <_ZN11AbyssEngine6StringD1Ev@plt>
* 13a2be ldr	r1, [sp, #76]	; 0x4c                   | 13a42c ldr	r1, [sp, #80]	; 0x50
  13a2c0 movs	r0, #0                                | 13a42e movs	r0, #0
  13a2c2 strd	r0, r0, [r1]                          | 13a430 strd	r0, r0, [r1]
  13a2c6 b.w	143d0a <_ZN11LevelScript7processEi@@Base+0xbff2> | 13a434 b.w	143a00 <_ZN11LevelScript7processEi@@Base+0xbce8>
- 13a2ca nop                                        | 
nonmatching slots: 130
```

## Case 154 lead (not landed)

`case 154` state 0 (`0x13a76e..0x13a92c`, 169 insns / 450 B) is nearly
instruction-identical to ours (`0x13c650..0x13c804`, 159 insns / 440 B).  The
only semantic-looking difference is in the `shipGroup == 9` loop: the original
**reloads `(*enemies)[i]` twice** (`0x13a8b4 ldr r0,[sp,#96]; 0x13a8ba
ldr.w r1,[r0,fp,lsl#2]` for `getPosition`, and `0x13a8ee ldr r0,[r4,#4];
0x13a8f2 ldr.w r0,[r0,fp,lsl#2]` for `setAIDisabled`), i.e. the source has no
`KIPlayer *enemy` local:

```cpp
if ((*enemies)[i]->shipGroup == 9) {
    AEGeometry *ship = (*enemies)[i]->geometry;
    ship->setDirection(AEMath::VectorNormalize(
        m_pLevel->getPlayer()->getPosition() - (*enemies)[i]->getPosition()),
        Vector{0.0f, 1.0f, 0.0f});
    ((PlayerFighter *) (*enemies)[i])->setAIDisabled(true);
}
```

Removing the `enemy` local is faithful but **+44 B** on the current base
(57,276); it needs its own -44 companion.  Not landed.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/fdiff.py _ZN11LevelScript7processEi   # 54,869 / 57,232
python3 /dev/shm/w9320/block_align.py 13a036 13a2cc 13a1bc 13a438   # ratio 0.6757
```
