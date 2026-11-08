# LevelScript::process — wave 150 / w9707: the faithful `messages,player,enemies`
# prologue landed at exactly 57,232 B (the +72 is now +4; -4 companion set found)

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9707`, base master `db9658a51` (wave 150).  Scratch in
`/dev/shm/w9707/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,792            55,320            (+528)
process aligned insns    17,605            17,244            (-361)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    86,863            87,391            (+528)
.ARM.extab diffbytes     1,326             1,327             (+1)
.ARM.exidx               0                 0
.rel.plt                 0                 0                 (byte-identical)
.rodata / .data / .bss   exact             exact
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. The mission's "+72 B" is stale: on the wave-150 base the faithful
##    prologue is only **+4 B**

w9603 measured the faithful `messages, player, enemies` order at +72 B on the
wave-149 base.  w9602 then landed the case-125 independent-if restructure
(`2,3,default`), which removed the +56 B case-125 pool relocation and the
+4 B EH-cleanup growth.  Re-measured on the wave-150 base, the faithful order
is:

```
faithful alone:  raw 55,276   aligned 17,222   size 57,236 (+4)
```

So the whole remaining residual is the **prologue status register**: the
original loads `Globals::status` into `r4` (`ldr r4,[r0,#0]`, 2 B) and then
`ldr r0,[r4,#0]` (2 B); our build puts it in `r9` (`ldr.w r9,[r0]` 4 B +
`ldr.w r0,[r9]` 4 B).  The faithful prologue is otherwise instruction-for-
instruction identical to the original through `strd r5,r6,[sp,#72]` /
`beq.w`.  Confirmed by direct disassembly of both prologues.

The +4 is exactly the two 32-bit status loads.  It is the same global
allocator choice documented by w9302/w9603 (`status` in `r9`, `this` in `fp`,
canary in `sl`); the original has `status` in `r4`, `this` in `sl`, canary in
`fp`.  No local source shape tried in w9302/w9603 moved it without changing
the layout.

## 2. The landed -4 companion set (all genuine faithfulness fixes)

The faithful prologue needs a -4 B companion.  Three independent faithfulness
fixes net exactly -4 B on top of it:

1. **Case-92 state-1: the faithful 3-call `getPosition()` shape** (w9406).
   The original calls `(*enemies)[3]->getPosition()` three times, once per
   `setPosition` (calls at `0x13a06e`, `0x13a0ac`, `0x13a0e2`).  The source
   kept a `Vector ref` for the first/third call; deleting it is +8 B in
   isolation and raises the block-local instruction alignment from 0.6782 to
   0.7165 (177/261 -> 187/261).
2. **Case-144 state-3: drop the duplicate `setStation(getStation())`.**
   Original `0x141490..0x1414a8` is `nextCampaignMission(true)`,
   `getStation(98)`, `departStation(...)` — there is no `setStation` call.
   The source had a spurious `Globals::status->setStation(...)`.  -16 B in
   isolation.
3. **Case-154 state-9 completion: `m_nState++`, not `m_nState = 6`.**
   Original `0x144a08..0x144a0e` is `ldr m_nState; adds #1; str m_nState`
   (state 9 -> 10).  `m_nState = 6` would loop back to state 6.  +4 B in
   isolation on this base.

Net: faithful prologue (+4) + case-92 3-call (+8) + case-144 setStation
removal (-16) + case-154 tail (+4) = **exactly 57,232 B**.

Measured combinations (all built + linked):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD `enemies,messages,player` | 57232 | 54,792 | 17,605 |
| faithful `messages,player,enemies` | 57236 | 55,276 | 17,222 |
| faithful + case-92 3-call | 57244 | 55,291 | — |
| faithful + case-154 `++` | 57240 | 54,918 | — |
| faithful + case-144 setStation drop | 57220 | 55,362 | — |
| faithful + c92 + c154 | 57248 | 55,307 | — |
| **faithful + c92 + c154 + c144 drop (landed)** | **57232** | **55,320** | **17,244** |
| faithful + c154 + `e24` revert (metric-only) | 57232 | 55,294 | 17,242 |

The last row is a metric-only alternative: it reverts the case-24
`getEnemies()` re-fetch (a faithfulness regression) instead of the three
genuine fixes.  It is 26 raw bytes / 2 aligned instructions better but less
faithful, so the all-faithful set was landed.

## 3. Why the raw metric rises (+528) while aligned falls (-361)

The faithful order changes the whole-function register allocation: `messages`
is reloaded from `[sp,#72]` instead of being cached in `r9` (the original has
exactly one `ldr.w r9,[sp,#72]` at `0x14198e`; the old `emp` order had five
extra caches).  The 3-call case-92 shape adds a third virtual `getPosition()`
sequence.  Both are structurally closer to the original (aligned -361) but
recolour many following instructions, so the same-address byte metric rises.
This is the expected trade for a faithful-order landing; the gate is
`byte_exact`/`linked_exact`, both unchanged at 4507.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9707/fdiff.py _ZN11LevelScript7processEi        # 55,320 / 57,232
python3 /dev/shm/w9707/align_diff.py _ZN11LevelScript7processEi  # 17,244
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 5. Open questions

1. The prologue status register is still `r9` vs the original `r4`.  Getting
   it into `r4` would remove the last 4 B of prologue difference; every
   `__builtin_expect` shape tried (mission guards, state guards) changes the
   size in 8/16/72-byte quanta and does not preserve the original mission-0
   block placement (`beq.w 137e84`).
2. The case-125 state-1/2 blocks are still inline (`spanU 1032` vs the
   original's outlined `spanO 98` at `0x13b43e`/`0x13b518`).  Outlining them
   is the remaining cold-region shape; it did not change the total size in
   the w9602 restructure.
3. `e24`/`e16a` (the two case-24/case-16 `getEnemies()` re-fetches) are the
   only -8/-16 levers found; both are faithfulness fixes and are currently
   landed.
