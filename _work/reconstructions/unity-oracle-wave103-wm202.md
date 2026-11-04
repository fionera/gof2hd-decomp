# Unity oracle wave 103 — index of the 24 non-exact functions

**Status:** research note, 2026-11-04. Worktree `wt/wm202`, branch
`wave103-wm202`, base `1d2b4398e`. **No source change landed** — the only
source experiment (the `Level::createScene` branch reorder) was a strict
regression and was reverted. This commit contains only this note.

This note is the map for the function agents: it locates every one of the 24
non-byte-exact functions in the new third-party Ghidra decompilation
(“the oracle”), records the oracle's source shape, and separates what is
**verified against the original binary** from what is only **oracle says**.

---

## 0. TL;DR

* The oracle is a full Ghidra decompilation of the **KiritoJPK NDK-r14 /
  clang-3.8 `-O2` mod rebuild**, not of the shipped 2.0.16 binary. It is a
  **logic/source-shape oracle only**. Its addresses, function sizes, stack
  frames and some field offsets differ from the original.
* All 20 unique targets (24 rows: `Layout`, `Globals`, `MenuTouchWindow`,
  `ModStation` each have a C1/C2 alias) were located and indexed. The oracle
  block for each is in the table in §4.
* **Verified contradictions against the original binary** (i.e. the oracle is
  right and our source is not):
  * `Level::createScene`: original compares `missionPtr` in the order
    **2, 4, 23**; our source tests **23, 4, 2**. Reordering the source to
    2/4/23 measured **88.3 → 85.9**, size 2252 B unchanged, gate unchanged —
    reverted. (§5.6)
  * `Gun::calcCharacterCollision`: original post-damage statement order is
    **F P H L K** (`field_54=1`, `playerGun`, `setHitVector`, `lifetimes`,
    `lastHitKIPlayer`); our source is **F H K L P**. The faithful order is
    already proven to collapse the allocator (90.0 → 64.8) in
    `gun-calccharactercollision-w4244.md`. (§5.2)
  * `Level::createMission`: original tests **184 before 183**; our source has
    `case 183` before `case 184`. Not yet measured. (§5.12)
* **Oracle artifacts that must NOT be copied into source** (verified against
  the original binary):
  * `Level::createShip`: oracle's `setShipGroup` 8-argument call is a
    decompiler artifact — the original passes 3 args (`r1=group, r2=type,
    r3=group`) at `0xc071c`. Oracle's `this_01[0x3c]=1` is also wrong: the
    original is a **byte store at 0x40** (`strb.w r0,[r9,#64]` at `0xc0b6c`).
    Oracle's “one `setWreckedMeshId` after the switch” is the compiler's
    tail-merge of the per-case calls; our per-case source is correct. (§5.1)
  * `MGame::OnTouchEnd`: oracle says `amount++` / `if (0 < amount)` /
    `String(amount)`. The original decrements (`sub.w r9,r9,#1` at
    `0x17a5b6`), compares `== 0` (`cmp.w r9,#0; beq` at `0x17a5ca`), and
    negates for the string (`rsb r1,r9,#0` at `0x17a5da`). The wave-102
    `amount != 0` / `String(-amount)` reading stands. (§5.3)
  * `PlayerEgo::calcCollision`: oracle's `this_01[0x38]` / `this_01[0x6d]`
    are KIPlayer **-4 revision shifts**; the original uses `0x3c` / `0x71`
    (our `stealFlagByte` / `field_0x71`). The original's damage body is
    **first** (load `[r8,#0x3c]`, `isDying`, `isDead`, … at `0x9f244`), so
    our `if (stealFlagByte) { damage } else { visible }` matches the original
    block order; the oracle's visible-first order does not. (§5.5)
  * `ModStation::ModStation`: oracle stores `this+0x100=0` first; the
    original's first store is `strd r1,r1,[r5,#260]` = **0x104/0x108**
    (`0xd5310`), no 0x100 store. (§5.9)
  * `Globals::Globals`: oracle's pair stores `_20_8_` / `_28_8_` are a
    revision/compiler artifact; the original uses `strd r4,r4,[r1]` (0/4)
    and `strd r4,r0,[r1,#36]` (0x24/0x28), and stores `_84_4_` / `_88_4_`
    individually at `0xe334a` / `0xe335e`. The oracle's **early `movw
    #0x247` / `#0x201`** shape *is* present in the original
    (`0xe3236`, `0xe3328`) and is what the wave-101 Globals note calls the
    `movw #583` hoist. (§5.8)
* **Oracle confirms our source** (no action):
  * `Generator::createMission`: the campaign `<16` / type-15 option branch is
    a 5-entry table. Original `tbb` at `0x97310`, table bytes
    `15 03 12 14 0c` → **0→11, 1→0, 2→7, 3→4, 4→12**, exactly our switch.
    (§5.4)
  * `Status::missionCompleted`: our source's non-ascending body order is the
    wave-102 decoded original order; the oracle's ascending case order is a
    decompiler/jump-table artifact. (§5.7)
  * `Hud::draw`: oracle's gamma block is the plain receiver-first form
    (`this = Globals::status; getStation(); getIndex();
    getCurrentCampaignMission(); getGammaRayDamagePerSecond(this, …)`),
    matching the landed wave-102 shape. Current 92.1 % is the
    `.ARM.extab`-constrained shape, not the 94.5 % local optimum. (§5.13)
* The oracle's `_FUNCTIONS.tsv` addresses are for the mod rebuild and are
  **not** the original's. Always use `nm -D` on the original. (§2)

---

## 1. Oracle provenance and caveats

Oracle root (read-only):

```
/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/
  Reference/decompiled/native/
    game/*.c   engine/*.c
    _FUNCTIONS.tsv   INDEX.md
```

It is a Ghidra decompilation of the **KiritoJPK NDK-r14 / clang-3.8 `-O2`
mod rebuild** of the same game-source family. It is *not* the shipped
`android_2.0.16_libgof2hdaa.so`:

| property | original 2.0.16 | oracle mod rebuild |
|---|---|---|
| compiler | NDK r18b clang 7.0.2 `-Oz` | NDK r14 clang 3.8 `-O2` |
| `Level::createShip` | `0xc0214`, 2764 B | `0xcf83c`, 2968 B |
| `LevelScript::process` | `0x137d18`, 57232 B | `0x160d50`, 57020 B |
| `Layout::Layout` | `0xd1028`, 7072 B | `0xe2a00`, 8140 B |
| `MGame::OnTouchEnd` | `0x17a144`, 8080 B | `0x1a98d8`, 8834 B |
| `KIPlayer` `field_0x71` | 0x71 | 0x6d |
| `KIPlayer` `stealFlagByte` | 0x3c | 0x38 |
| `KIPlayer` `field_0x8c` | 0x8c | 0x88 |
| `ModStation` first `this` store | 0x104/0x108 | 0x100/0x104/0x108 |
| `Level::createShip` `this+0x40` byte | 0x40 | 0x3c |

The oracle binary itself in the repo is a **git-lfs pointer**, not an ELF, so
no oracle disassembly is available. Every “verified” claim below is against
the shipped original `.so`.

**Ghidra note.** The Ghidra project
(`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`)
contains only `/Demangler/<Class>` structures of length 1 — there are **no
reconstructed struct data types**. Therefore “oracle struct layout vs Ghidra”
had to be done from the original disassembly + our headers' `static_assert`s +
the `drift` target (which reports **0 drifted fields**). See §6.

---

## 2. Exact commands

### 2.1 Baseline (canonical build; already configured in this worktree)

```sh
cd /home/debian/GalaxyOnFire2/wt/wm202
cmake --build cmake-build-match --target check -j8
```

Output:

```text
compared 4524  avg 99.9%  byte_exact 4500  linked_exact 4500
stub_zero_size 0  orig_zero_size 0  missing 0  wrong_type 0  extra 0
data: exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

The 24 non-exact rows were extracted from `report.json`:

```sh
python3 - <<'PY'
import json
d = json.load(open('report.json'))
for r in d['rows']:
    if not r['byte']:
        print(f"{r['pct']:5.1f} {r['osize']:6d} {r['usize']:6d} {r['name']}")
PY
```

### 2.2 First structural difference per function

The verifier prints a side-by-side disassembly; the first line containing
`|` is the first structural difference. The loop used was:

```sh
for s in _ZN6LayoutC1Ev _ZN11LevelScript7processEi \
         _ZN6Status16missionCompletedEbbx \
         _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
         _ZN5Level10createShipEiiiP8Waypointbb \
         _ZN5Radar4drawEP6PlayerP3Hudi _ZN5MGame10OnTouchEndEiiPv \
         _ZN12HangarWindow6renderEv _ZN7GlobalsC1Ev _ZN9RocketGun6updateEi \
         _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
         _ZN5Level11createSceneEv _ZN3Gun22calcCharacterCollisionEv \
         _ZN5Level13createMissionEv _ZN3Hud4drawExxP9PlayerEgobjj \
         _ZN15MenuTouchWindowC1Ei _ZN11SpaceLounge10drawLoungeEv \
         _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN10ModStationC1Ev \
         _ZN5MGame8OnUpdateEv; do
  python3 tools/verify/verify.py \
    --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    --ours cmake-build-match/libgof2hdaa.so \
    --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
    --show "$s"
done
```

### 2.3 Original disassembly

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D --defined-only /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | grep <symbol>
$OD -d --start-address=0x... --stop-address=0x... \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

### 2.4 Oracle extraction

```sh
O=/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native
sed -n '5,2026p'      $O/game/Layout.c      > /dev/shm/wm202/oracle_Layout_layout.c
sed -n '1502,10248p'  $O/game/LevelScript.c > /dev/shm/wm202/oracle_LevelScript_process.c
sed -n '3062,3458p'   $O/game/Status.c      > /dev/shm/wm202/oracle_Status_missionCompleted.c
sed -n '1380,1724p'   $O/game/Generator.c   > /dev/shm/wm202/oracle_Generator_createMission.c
sed -n '12105,12658p' $O/game/Level.c       > /dev/shm/wm202/oracle_Level_createShip.c
sed -n '506,2692p'    $O/game/Radar.c       > /dev/shm/wm202/oracle_Radar_draw.c
sed -n '2094,3692p'   $O/game/MGame.c       > /dev/shm/wm202/oracle_MGame_OnTouchEnd.c
sed -n '975,2210p'    $O/game/HangarWindow.c> /dev/shm/wm202/oracle_HangarWindow_render.c
sed -n '5,88p'        $O/game/Globals.c     > /dev/shm/wm202/oracle_Globals_Globals.c
sed -n '321,686p'     $O/game/RocketGun.c   > /dev/shm/wm202/oracle_RocketGun_update.c
sed -n '5093,5262p'   $O/game/PlayerEgo.c   > /dev/shm/wm202/oracle_PlayerEgo_calcCollision.c
sed -n '4564,4977p'   $O/game/Level.c       > /dev/shm/wm202/oracle_Level_createScene.c
sed -n '1075,1477p'   $O/game/Gun.c         > /dev/shm/wm202/oracle_Gun_calcCharacterCollision.c
sed -n '1889,4563p'   $O/game/Level.c       > /dev/shm/wm202/oracle_Level_createMission.c
sed -n '2411,3746p'   $O/game/Hud.c         > /dev/shm/wm202/oracle_Hud_draw.c
sed -n '5,1307p'      $O/game/MenuTouchWindow.c > /dev/shm/wm202/oracle_MenuTouchWindow_ctor.c
sed -n '4090,4412p'   $O/game/SpaceLounge.c > /dev/shm/wm202/oracle_SpaceLounge_drawLounge.c
sed -n '4341,4716p'   $O/game/MenuTouchWindow.c > /dev/shm/wm202/oracle_MenuTouchWindow_OnTouchBegin.c
sed -n '5,224p'       $O/game/ModStation.c  > /dev/shm/wm202/oracle_ModStation_ctor.c
sed -n '4178,6055p'   $O/game/MGame.c       > /dev/shm/wm202/oracle_MGame_OnUpdate.c
```

---

## 3. Baseline gate (measured before and after this pass)

```text
byte_exact 4500/4524   linked_exact 4500/4524   size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0
data_exact 843/843     imports 207
.text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70  .ARM.extab 0x26298
lint CLEAN  drift 0   RATCHET PASS
```

The note-only commit does not change any of these numbers.

---

## 4. Per-function index (oracle line ranges + first difference)

`first diff` is the first line of the verifier's side-by-side that contains
`|`; the classification is **source-shape** (a real content/order difference)
or **RA/codegen** (register allocation, stack slot, literal-pool offset,
branch-target shift).

| # | function (row pct / size) | oracle file:lines | first diff | class |
|---:|---|---|---|---|
| 1 | `Layout::Layout` C1/C2 (47.8, 7072 B) | `game/Layout.c:5-2026` | line 3: prologue `stmdb` pushes `r6..fp` vs `r5..fp` | RA |
| 2 | `LevelScript::process` (53.4, 57232 B) | `game/LevelScript.c:1502-10248` | line 7: `ldr r0,[pc,#888]` vs `#908` | pool/codegen |
| 3 | `Status::missionCompleted` (60.4, 1432 B) | `game/Status.c:3062-3458` | line 30: `bcs.w 0x562` vs `0x560` | codegen (2 B) |
| 4 | `Generator::createMission` (64.4, 1996 B) | `game/Generator.c:1380-1724` | line 5: `sub sp,#96` vs `#112` | frame/RA |
| 5 | `Level::createShip` (66.0, 2764 B) | `game/Level.c:12105-12658` | line 5: `str r1,[sp,#60]` vs `strd r1,r2,[sp,#56]` | RA |
| 6 | `Radar::draw` (73.9, 9908 B) | `game/Radar.c:506-2692` | line 4: `vpush {d8-d13}` vs `{d8-d11}` | RA |
| 7 | `MGame::OnTouchEnd` (74.8, 8080 B) | `game/MGame.c:2094-3692` | line 135: `b.w 0xec0` vs `0x1832` | CFG/tail-merge |
| 8 | `HangarWindow::render` (80.2, 9700 B) | `game/HangarWindow.c:975-2210` | line 9: `add r0,pc` vs `ldr r2,[pc,#812]` | codegen |
| 9 | `Globals::Globals` C1/C2 (80.5, 612 B) | `game/Globals.c:5-88` | line 35: `movw r6,#583` vs store | source-shape (hoist) |
| 10 | `RocketGun::update` (86.9, 1908 B) | `game/RocketGun.c:321-686` | line 8: `mov r4,r1` vs `mov r5,r1` | RA |
| 11 | `PlayerEgo::calcCollision` (87.6, 968 B) | `game/PlayerEgo.c:5093-5262` | line 36: `str.w r9,[sp,#20]` vs `add r0,pc` | store order/RA |
| 12 | `Level::createScene` (88.3, 2252 B) | `game/Level.c:4564-4977` | line 461: `add.w r9,…` vs `add.w r8,…` | RA |
| 13 | `Gun::calcCharacterCollision` (90.0, 1984 B) | `game/Gun.c:1075-1477` | line 7: `ldr r0,[pc,#848]` vs `#832` | pool/codegen |
| 14 | `Level::createMission` (92.0, 17172 B) | `game/Level.c:1889-4563` | line 29: `str.w fp,[sp,#88]` vs `[sp,#84]` | frame layout |
| 15 | `Hud::draw` (92.1, 9332 B) | `game/Hud.c:2411-3746` | line 367: `ldr r6,[r0]` vs `ldr r0,[r0]` | codegen |
| 16 | `MenuTouchWindow::MenuTouchWindow` C1/C2 (95.1, 9600 B) | `game/MenuTouchWindow.c:5-1307` | line 14: `str r1,[sp,#60]` vs `#56` | frame layout |
| 17 | `SpaceLounge::drawLounge` (95.5, 1948 B) | `game/SpaceLounge.c:4090-4412` | line 143: `ldr.w r9,[r6,#148]` vs `fp` | RA |
| 18 | `MenuTouchWindow::OnTouchBegin` (97.7, 1896 B) | `game/MenuTouchWindow.c:4341-4716` | line 403: `ldr.w ip,[sl,#340]` vs `r4` | RA |
| 19 | `ModStation::ModStation` C1/C2 (98.4, 1008 B) | `game/ModStation.c:5-224` | line 119: `ldr r3,[pc,#632]` vs `r4` | RA |
| 20 | `MGame::OnUpdate` (99.9, 11440 B) | `game/MGame.c:4178-6055` | line 1993: `ldr.w r0,[r9]` vs `ldr.w r5,[r9]` | RA |

Only rows **1, 9, 11, 12, 14** have a source-shape component; the rest are
already at their local RA/codegen optimum (the wave-101/102 notes for each
function document the sweeps).

---

## 5. Detailed findings

### 5.1 `Level::createShip` (66.0 %, 2764 B)

Oracle block: `game/Level.c:12105-12658` (554 lines). Original: `0xc0214`,
2764 B.

**Oracle says** (all three are artifacts, verified against the original):

1. `PlayerFighter::setShipGroup` is called with **8 arguments**
   (`this_01, uVar11, param_3, _param_5, uVar22, fVar19, uVar21, fVar20,
   uVar13`). **Verified false.** Original `0xc071c` is `blx r5` with only
   `r0=this`, `r1=getShipGroup(...)`, `r2=type` (`[sp,#52]`), `r3=group`
   (`r8`). The extra `extraout_s*` registers are Ghidra's uninitialised
   float-argument noise.
2. `this_01[0x3c] = 1`. **Verified false.** Original `0xc0b6c` is
   `strb.w r0,[r9,#64]` — a **byte store at 0x40**. Our
   `fixed->field_0x40 = 1` is correct.
3. `setWreckedMeshId` is called **once after the race switch** with `iVar1`
   set per case. **Compiler tail-merge, not source shape.** Original has
   `setWreckedMeshId` calls at `0xc0a76` (case 3) and `0xc0b2e` (shared by
   cases 0/1/2 via `b.n 0xc0b2c`). Our per-case source reproduces this.

**Oracle confirms our source:** the cargo delete for types `0x2c`/`0x31`
(`operator_delete` of `[this+0x4c]`, then `[this+0x4c]=0`) and
`field_0x25 = 0` for type `0x33` (`*(PlayerFighter *)(this_01 + 0x21) = 0`
in the oracle; original `[r9,#0x25]` byte).

**Spawn coordinates:** the oracle computes the three spawn coordinates
before `setEmpData`; the wave-102 landed shape is the semantically-correct
mixed `int x / float y / float z` with statement order `xzy` (73.8 %). The
all-`int` faithful form cannot keep 2764 B (see
`level-createship-wave102-wf206.md`).

### 5.2 `Gun::calcCharacterCollision` (90.0 %, 1984 B)

Oracle block: `game/Gun.c:1075-1477` (403 lines). Original: `0x152f90`,
1984 B.

**Oracle says** the post-damage order is:

```
F  field_54[0] = 1
P  if (playerGun) level->field_30 = 1
H  setHitVector(-velocity)
L  lifetimes[i] = -1000000
K  lastHitKIPlayer = getKIPlayer()
```

**Verified against the original** at `0x153310..0x153350`:

```
153318  strb.w r1,[r8,#84]        ; F
15331c  ldrb.w r0,[r9,#240]       ; playerGun
153320  cbz r0, 15332a
153326  strb.w r1,[r0,#48]        ; P
15333c  blx setHitVector          ; H
153346  str.w r1,[r0,fp,lsl #2]   ; L
153350  str.w r0,[r9]             ; K
```

Our source is **F H K L P** (tuned). The faithful **F P H L K** is already
proven to collapse the allocator (`this` moves `r9 → r4`, 1912 B, 64.8 %) in
`gun-calccharactercollision-w4244.md`. Do not “fix” the order without
re-running the full permutation sweep.

Oracle's `KIPlayer` field offsets `0x6d` / `0x88` are **-4 revision
shifts**; the original uses `0x71` / `0x8c` (our `field_0x71`,
`field_0x8c`). The oracle's `this+0x3c` (`lifetimes`) and `this+0x40`
(`hitFlags`) are the same offsets as the original (`0x153340`, `0x153344`),
so do not “correct” those two.

### 5.3 `MGame::OnTouchEnd` (74.8 %, 8080 B)

Oracle block: `game/MGame.c:2094-3692` (1599 lines). Original: `0x17a144`,
8080 B.

**Oracle says** (refined-cargo loop, oracle lines 644-659):

```c
iVar11 = 0;
while (hasCargo(...)) { removeCargo; addCargo; iVar11 = iVar11 + 1; bVar3 = true; }
if (0 < iVar11) {
    report += String("\n") + String(iVar11) + String("t ") +
              *Globals::gameText->getText(1274 + refined);
}
```

**Verified against the original** at `0x17a58c..0x17a5da`:

```
17a58c  mov.w r9,#0             ; amount = 0
17a5b6  sub.w r9,r9,#1          ; amount--
17a5ca  cmp.w r9,#0
17a5ce  beq.n 17a652            ; if (amount == 0) skip
17a5da  rsb r1,r9,#0            ; String(-amount)
17a614  addw r1,sl,#1274        ; getText(1274 + refined)
```

So the original source is `amount--` with `if (amount != 0)` and
`String(-amount)`, **not** the oracle's `amount++` / `String(amount)`. The
wave-102 note (`mgame-ontouchend-wave102-wf207.md`) already proved this is a
real content difference but that landing it moves the register allocation and
costs ~6 points (8076 B, 68.4 %). The oracle's `iVar35 + 0x4fa` is the same
`1274 + refined` expression.

**Tail cluster:** the wave-102 note's flat-vs-nested wingman/hud analysis
stands. The oracle's tail order is not a byte oracle.

### 5.4 `Generator::createMission` (64.4 %, 1996 B)

Oracle block: `game/Generator.c:1380-1724` (345 lines). Original: `0x971b8`,
1996 B.

**Oracle says** the campaign-mission `<16` / type-15 option branch uses a
**5-entry table** at `DAT_00251f60`:

```c
if ((mission < 0x10) || (type == 0xf && options[0x37] == 0)) {
    uVar10 = rnd.nextInt(5);
    type = *(uint *)(&DAT_00251f60 + uVar10 * 4);
}
```

**Verified against the original** at `0x972e0..0x9733e`: the original uses a
`tbb [pc,r0]` at `0x97310` with table bytes at `0x97314` =
`15 03 12 14 0c`, i.e.

| `nextInt(5)` | target | type |
|---:|---|---:|
| 0 | `0x9733e` (r2 preset 11) | **11** |
| 1 | `0x9731a` (`movs r2,#0`) | **0** |
| 2 | `0x97338` (`movs r2,#7`) | **7** |
| 3 | `0x9733c` (`movs r2,#4`) | **4** |
| 4 | `0x9732c` (`movs r2,#12`) | **12** |

Our `switch (choice) { 0→11, 1→0, 2→7, 3→4, 4→12 }` is **exactly right**.
The oracle's table values are not readable (oracle binary is an LFS pointer),
but the mapping is confirmed from the original.

The oracle's `do { … } while (guard)` / `break` loop and
`do { … } while (systems->size()==0)` route loop are source-shape hints; the
wave-101/102 notes already explored the loop forms
(`generator-createmission-faithful-loop-slot-chain-w101.md`,
`…-faithful-p12-w4808.md`).

### 5.5 `PlayerEgo::calcCollision` (87.6 %, 968 B)

Oracle block: `game/PlayerEgo.c:5093-5262` (170 lines). Original: `0x9f0cc`,
968 B.

**Oracle says** the top guard is:

```c
if (objects != null &&
    (dockedFlag == 0 || dockingPointIndex - 1 > 2) &&
    objects->size() != 0) {
    do { … } while (i < size);
}
```

Our source returns early and uses a top-tested `for`. Semantically the same;
the frame/RA effect is already documented in
`playerego-calccollision-wave102-wf211.md` (the two invented redundant bounds
checks are load-bearing for the 968-byte register file).

**Oracle's KIPlayer offsets are -4 revision shifts** (`0x38`, `0x6d`); the
original uses `0x3c` (`stealFlagByte`) and `0x71` (`field_0x71`), verified at
`0x9f16e` (`ldrb [r8,#0x71]`) and `0x9f244` (`ldrb [r8,#0x3c]`).

**Block order:** the original's damage body is **first** (`0x9f244` loads
`[r8,#0x3c]`, then `isDying`/`isDead` at `0x9f24e`/`0x9f25a`), then the
visible body at `0x9f2f8` (`isVisible`). Our
`if (stealFlagByte) { damage } else { visible }` matches the original's
physical order; the oracle's visible-first order does not.

### 5.6 `Level::createScene` (88.3 %, 2252 B)

Oracle block: `game/Level.c:4564-4977` (414 lines). Original: `0xb42ec`,
2252 B.

**Oracle says** the top-level `missionPtr` branch order is **2, 4, 23**:

```c
iVar3 = *(int *)(this + 0xc0);
if (iVar3 == 2) { … if (iVar3 == 4) { … if (iVar3 == 0x17) { … } } }
```

**Verified against the original** at `0xb4320`:

```
b4320  ldr.w r0,[r6,#0xc0]
b432a  cmp r0,#2
b432c  beq b4358
b432e  cmp r0,#4
b4332  beq b442a
b4334  cmp r0,#23
b4336  bne b4b2c
```

Our source tests **23, 4, 2**. Reordering the source blocks to 2, 4, 23
measured:

| shape | fuzzy | size | gate |
|---|---:|---:|---|
| shipped (23, 4, 2) | **88.3** | 2252 | green |
| reordered (2, 4, 23) | **85.9** | 2252 | byte 4500 / linked 4500 unchanged |

The reorder was reverted (`git checkout -- src/game/world/Level.cpp`). So the
original's branch order is proven, but the source order that reproduces it is
not the score-optimal one. The first diff of the shipped shape is at line 461
(the parts-loop preheader `add.w r9/r8`), i.e. pure RA.

### 5.7 `Status::missionCompleted` (60.4 %, 1432 B)

Oracle block: `game/Status.c:3062-3458` (397 lines). Original: `0xabf38`,
1432 B.

**Oracle says** the `Mission::getType` switch cases are in ascending order
(`0x96, 0x97, …, 0xbd`). This is the decompiler's jump-table order.

**Verified against the original** (wave-102
`status-missioncompleted-wave102-wm101.md`): the physical body order is

```
167/174, 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

Our source's case order matches this exactly. Do **not** reorder to the
oracle's ascending order.

### 5.8 `Globals::Globals` C1/C2 (80.5 %, 612 B)

Oracle block: `game/Globals.c:5-88` (84 lines). Original: `0xe31e0`, 612 B.

**Oracle says:**

```c
shipTemplate = 0; Canvas = 0; recordHandler = 0; appManager = 0;
options._84_4_ = 0x247;          // early resWidth
gameText = 0; generator = 0; …
options._20_8_ = 0x3f0000003f000000;
options._28_8_ = 0x3f19999a3f19999a;
…
if (1.0 <= options._68_4_) options._84_4_ = 0x33e;
options._88_4_ = 0x201;          // resHeight between conditions
…
```

**Verified against the original:**

* The early `movw r6,#583` (`0x247`) is real: `0xe3236`.
* The `movw r0,#513` (`0x201`) between the conditions is real: `0xe3328`.
* The conditional `movwge #0x33e` / `movwls #0x19f` and
  `movwge #0x2da` / `movwls #0x16d` are real: `0xe3330..0xe3358`.
* The final stores are **individual** `str r6,[r1,#84]` and
  `str r0,[r1,#88]` at `0xe334a` / `0xe335e`, not a pair store.
* The original's pair stores are `strd r4,r4,[r1]` (offsets 0/4) and
  `strd r4,r0,[r1,#36]` (0x24/0x28), not the oracle's `_20_8_` / `_28_8_`.
  The oracle's pair layout is a revision/compiler artifact.

The wave-101 `globals-globals-c1-c2-w5106.md` note already measured the
`movw #583` hoist and found it costs 7.5-8.8 points; the 80.5 % basin stands.

### 5.9 `ModStation::ModStation` C1/C2 (98.4 %, 1008 B)

Oracle block: `game/ModStation.c:5-224` (220 lines). Original: `0xd52d0`,
1008 B.

**Oracle says** the first `this` stores are `*(this+0x100)=0`,
`*(this+0x104)=0`, `*(this+0x108)=0`.

**Verified against the original:** the first `this` store is
`strd r1,r1,[r5,#260]` = **0x104/0x108** at `0xd5310`; there is **no 0x100
store**. The oracle's 0x100 is a revision difference. Our emitted code matches
the original's `strd`/`str` sequence at `0xd52d0`.

### 5.10 `Layout::Layout` C1/C2 (47.8 %, 7072 B)

Oracle block: `game/Layout.c:5-2026` (2022 lines). Original: `0xd1028`,
7072 B.

**Oracle says** the non-HD branch initialises fields in **ascending offset
order** (`0x4, 0x8, 0xc, 0x10, 0x14, 0x18, 0x1c, 0x20, …`). Our source uses a
tuned “hill” order (`0x4, 0x8, 0x1c, 0x10, 0xc, …`).

**Verified against the original:** all 204 initialised fields have the same
32-bit value in all 32 flag combinations (wave-102
`layout-layout-c1-c2-wf102.md`, emulated both binaries, 0 mismatches). The
residual is literal-pool / vector-store grouping, not values or order. The
oracle's natural offset order is a **hypothesis** for a future order sweep;
the wave-102 note already tried the iOS order and several hill orders and
found no improvement over 47.8 %.

### 5.11 `LevelScript::process` (53.4 %, 57232 B)

Oracle block: `game/LevelScript.c:1502-10248` (8747 lines). Original:
`0x137d18`, 57232 B.

This is the largest target. The oracle's top-level structure is:

| oracle lines | construct |
|---|---|
| 350-500 | pre-switch `getCurrentCampaignMission` specials |
| 513-800 | `switch (*(this+0x1c))` — state machine cases 1..12 |
| 995-1146 | second `switch (*(this+0x1c))` — cases 1..5 |
| 1345-1616 | `switch (iVar37)` — intro states 6..15 |
| 1760-2499 | `switch (mission)` — cases 0x83, 0x87, 0x8b, 0x8e, 0x90, 0x91 |
| 2549-3008 | nested `switch` — mission-131 sub-states |
| 4149-4521 | nested `switch` — mission-139/142/144 sub-states |
| 4668-5360 | nested `switch (*(this+0x1c))` — mission-145 sub-states |
| 5693-6159 | nested `switch` — mission-145 states 1..10 |
| 7460-7969 | nested `switch` — late mission states |
| 8298-8476 | final `switch` — states 1,2,6,7,8,9,10 |

The oracle's mission switch only has `0x83, 0x87, 0x8b, 0x8e, 0x90, 0x91`
(131, 135, 139, 142, 144, 145); our source also has 158, 64, 69, 73, 67, 70,
29, 41, 80, 56, 92, 157, 125, 40, 102, 154. The oracle's decompiler split the
mission switch into several `switch(iVar7)` statements (lines 1760, 2300,
2549, 4149, 4894, 5693, 7460, 8298), so the case list is not directly
comparable. Use the oracle for **constants and field accesses inside a case**,
not for the top-level case order. The wave-101/102 notes
(`levelscript-process-*.md`) are the authority on the landed shape.

### 5.12 `Level::createMission` (92.0 %, 17172 B)

Oracle block: `game/Level.c:1889-4563` (2675 lines). Original: `0xaffd8`,
17172 B.

**Oracle says** the `Mission::getType` switch is on types 1..15 in the order
`1, 2, 3/5, 4, 6, 7, 8/11/13/14, 9, 10, 12, 15`, and the 183/184 special
cases are checked **184 before 183** (oracle lines 2390, 2499).

**Verified against the original** at `0xb10c6`:

```
b10c6  cmp r0,#184
b10c8  beq.w b2700
b10cc  cmp r0,#183
b10ce  bne.w b02ca
```

So the original's check order is **184 then 183**. Our source has `case 183`
first (line 2138) and `case 184` last (line 2511). This is a **new,
unmeasured source-shape difference**. The oracle's type-switch order
(1,2,3/5,4,6,7,8/11/13/14,9,10,12,15) is *not* the original's physical body
order (the original `tbh` table at `0xb02a8` lays out 8/11/13/14, 3/5, 1, 2,
4, 6, 7, 9, 10, 12, 15); our source order (3/5, 1, 2, 6, 7, 9, 10, 12, 15, 4)
is closer to the physical order. Treat the type-switch order as
**hypothesis**, the 184/183 order as **verified but unmeasured**.

The wave-102 landed changes (case 9 crate store order, case-4 tail hunter
loop) are unchanged and still correct.

### 5.13 `Hud::draw` (92.1 %, 9332 B)

Oracle block: `game/Hud.c:2411-3746` (1336 lines). Original: `0x163b90`,
9332 B.

**Oracle says** the gamma block is the plain receiver-first form (oracle
lines 220-225):

```c
this = Globals::status;
pSVar12 = Status::getStation(Globals::status);
iVar8 = Station::getIndex(pSVar12);
iVar9 = Status::getCurrentCampaignMission(Globals::status);
fVar10 = Status::getGammaRayDamagePerSecond(this, iVar8, iVar9);
```

**Verified against the original** (wave-102
`hud-draw-gamma-reload-w102-wf215.md`): the receiver is the first load,
`Globals::status` is reloaded for the mission argument. Our source's
`Status *st = Globals::status; st->getGammaRayDamagePerSecond(
st->getStation()->getIndex(), st->getCurrentCampaignMission())` matches.

**Current 92.1 % vs the note's 94.5 %:** the wave-102 `.ARM.extab`
size-exact commit (`42164ddfc`) reordered the second-bar block in
`Hud.cpp` (moved `String text` / `barOffset` / `int barW` after the
`fadeIn` computation) to fit the extab budget. That is the 94.5 → 92.1
trade. The gamma block itself is unchanged.

### 5.14 `MGame::OnUpdate` (99.9 %, 11440 B)

Oracle block: `game/MGame.c:4178-6055` (1878 lines). Original: `0x17c8d8`,
11440 B.

The first diff is at verifier line 1993:

```
orig: ldr.w r0,[r9] ; blx …
ours: ldr.w r5,[r9] ; mov r0,r5 ; blx …
```

This is a **register-allocation** difference (a `Globals::status` / HUD
receiver kept in `r5` instead of `r0`), not a source-shape difference. The
oracle's call sequence matches ours; no action.

### 5.15 `Radar::draw` (73.9 %, 9908 B)

Oracle block: `game/Radar.c:506-2692` (2187 lines). Original: `0x12e050`,
9908 B.

First diff at line 4: `vpush {d8-d13}` vs `{d8-d11}` — RA. The wave-102
`radar-draw-pass17-wf204.md` note landed a size-neutral `otherLock` hint that
moves the cold block 0x74 bytes earlier (73.868 → 73.897 raw). The oracle's
block order is not a byte oracle; no new source-shape claim.

### 5.16 `HangarWindow::render` (80.2 %, 9700 B)

Oracle block: `game/HangarWindow.c:975-2210` (1236 lines). Original:
`0x1490d8`, 9700 B.

First diff at line 9: load ordering (`add r0,pc` vs `ldr r2,[pc,#812]`) —
codegen. The wave-101/102 notes
(`hangarwindow_render_shipbranch_w101.md`,
`hangarwindow_render_w102.md`) already decoded the ship-price branch order
from disassembly. The oracle's ship branch is at oracle lines 425, 529, 631;
it is consistent with the wave-101 reading (`tab==1 && upgradeMode` first,
then `tab==1 || i>=2` → difference price). No contradiction.

### 5.17 `MenuTouchWindow::MenuTouchWindow` C1/C2 (95.1 %, 9600 B)

Oracle block: `game/MenuTouchWindow.c:5-1307` (1303 lines). Original:
`0x12299c`, 9600 B.

Oracle uses the `*(this+4)` anchor (`*(undefined4 **)(this + 4) = puVar3;`
at oracle line 115) and `addButton(this, …, *(undefined4 *)(this+4), …)`,
matching our source. First diff is a stack-slot shift (`[sp,#60]` vs
`[sp,#56]`). The wave-102 note (`menutouchwindow-ctor-wave102-wf216.md`)
already swept 216 declaration/order variants; the residual is a greedy-RA
permutation. No new source-shape claim.

### 5.18 `SpaceLounge::drawLounge` (95.5 %, 1948 B)

Oracle block: `game/SpaceLounge.c:4090-4412` (323 lines). Original:
`0x170d38`, 1948 B.

First diff at line 143: `ldr.w r9,[r6,#148]` vs `ldr.w fp,[r6,#148]` — RA.
The oracle's call sequence (SetColor → Agent::isKnown → isStoryAgent →
getRace → getText → …) matches our source. No source-shape claim.

### 5.19 `MenuTouchWindow::OnTouchBegin` (97.7 %, 1896 B)

Oracle block: `game/MenuTouchWindow.c:4341-4716` (376 lines). Original:
`0x129bd4`, 1896 B.

First diff at line 403: `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` — RA.
The oracle's case/button order matches our source. No source-shape claim.

### 5.20 `RocketGun::update` (86.9 %, 1908 B)

Oracle block: `game/RocketGun.c:321-686` (366 lines). Original: `0x15ecd8`,
1908 B.

First diff at line 8: `mov r4,r1` vs `mov r5,r1` — RA. The wave-102 note
(`rocketgun-update-wf210.md`) proved that ~120 expression-level variants that
reach 86.9 % compile to **byte-identical machine code**; the residual is a
hard fixed point of the front end + allocator. No source-shape claim.

---

## 6. Struct layouts vs Ghidra

The Ghidra project has **no reconstructed struct data types** — only
`/Demangler/<Class>` structures of length 1. The headless command used was:

```sh
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2/_work/bins android_2.0.16_libgof2hdaa.so_ghidra \
  -process android_2.0.16_libgof2hdaa.so -readOnly -noanalysis \
  -scriptPath /dev/shm/wm202 -postScript DumpStructs.java
```

Result: every `/Demangler/<Class>` has length 1. Therefore the struct check
was done from the original disassembly + our headers' `static_assert`s + the
`drift` target. `drift` reports **0 drifted fields** on the shipped tree.

The oracle's struct fields that differ from the original (all verified from
disassembly):

| class | field | original | oracle | evidence |
|---|---|---:|---:|---|
| `KIPlayer` | `stealFlagByte` | 0x3c | 0x38 | `ldrb [r8,#0x3c]` @ `0x9f244` |
| `KIPlayer` | `field_0x71` | 0x71 | 0x6d | `ldrb [r8,#0x71]` @ `0x9f16e` |
| `KIPlayer` | `field_0x8c` | 0x8c | 0x88 | wave-102 Gun note |
| `Level` | `missionPtr` | 0xc0 | 0xc0 | `ldr [r6,#0xc0]` @ `0xb4320`, `0xc0720` |
| `Level` | `field_0x40` (fixed ship flag) | 0x40 byte | 0x3c | `strb.w r0,[r9,#64]` @ `0xc0b6c` |
| `ModStation` | first ctor store | 0x104/0x108 | 0x100/0x104/0x108 | `strd r1,r1,[r5,#260]` @ `0xd5310` |
| `Globals::options` | resWidth/resHeight | 0x54/0x58 | 0x54/0x58 | `str r6,[r1,#84]`, `str r0,[r1,#88]` @ `0xe334a`/`0xe335e` |

The `-4` shift on the three `KIPlayer` fields is consistent across the oracle
and is the single most important caveat when reading oracle field accesses.

---

## 7. Proven vs hypothesis

### Proven (verified against the original binary)

1. `Level::createScene` top-level branch order is **2, 4, 23** (`0xb4320`).
   Our source is 23, 4, 2. Reordering to 2/4/23 measured **88.3 → 85.9**
   (size 2252 unchanged); reverted.
2. `Gun::calcCharacterCollision` post-damage order is **F P H L K**
   (`0x153318..0x153350`). Our source is F H K L P; the faithful order is
   already proven to collapse the allocator.
3. `MGame::OnTouchEnd` refined-cargo counter is **negative**
   (`sub.w r9,r9,#1`), the guard is `== 0`, and the string is
   `String(-amount)` (`0x17a5b6..0x17a5da`). The oracle's `amount++` is a
   revision/compiler artifact.
4. `Level::createShip`: `setShipGroup` is a **3-argument** call
   (`0xc071c`); the fixed-ship flag is a **byte store at 0x40**
   (`0xc0b6c`); `setWreckedMeshId` is per-case (tail-merged by the compiler).
   The oracle's 8-arg call, `0x3c` store and single-call shape are artifacts.
5. `Generator::createMission` campaign `<16` table maps
   **0→11, 1→0, 2→7, 3→4, 4→12** (original `tbb` at `0x97310`). Our switch
   matches.
6. `PlayerEgo::calcCollision` uses `stealFlagByte` at **0x3c** and
   `field_0x71` at **0x71**; the damage body is physically first
   (`0x9f244`). The oracle's `0x38`/`0x6d` and visible-first order are
   revision/compiler artifacts.
7. `ModStation::ModStation` first `this` store is **0x104/0x108**, no 0x100
   (`0xd5310`).
8. `Globals::Globals` has the early `movw #0x247` (`0xe3236`) and `#0x201`
   (`0xe3328`), but the final resWidth/resHeight stores are individual
   (`0xe334a`/`0xe335e`), not the oracle's pair stores.
9. `Status::missionCompleted` body order is the non-ascending wave-102 order,
   not the oracle's ascending case order.
10. `Hud::draw` gamma block is the plain receiver-first form, matching our
    source.

### Hypothesis (oracle says, not yet measured)

1. `Level::createMission`: original tests **184 before 183** (`0xb10c6`);
   our source has 183 before 184. A source reorder is unmeasured.
2. `Level::createMission`: the oracle's type-switch order
   (1,2,3/5,4,6,7,8/11/13/14,9,10,12,15) is a source-shape hint, but the
   original `tbh` body order (8/11/13/14, 3/5, 1, 2, 4, 6, 7, 9, 10, 12, 15)
   is closer to our current order.
3. `Layout::Layout`: the oracle's ascending-offset init order is a candidate
   for a future order sweep; wave-102 already tried the iOS order and several
   hill orders without beating 47.8 %.
4. `Globals::Globals`: the oracle's `shipTemplate, Canvas, recordHandler,
   appManager` prologue order differs from our `shipTemplate, recordHandler,
   Canvas`; wave-101's 59-token annealing found nothing above 80.5 %.

### Not a source-shape difference (RA/codegen only)

`Layout` (prologue), `LevelScript::process` (pool), `Status::missionCompleted`
(2-byte branch), `Generator::createMission` (frame 96 vs 112), `Level::createShip`
(prologue store), `Radar::draw` (vpush set), `MGame::OnTouchEnd` (tail merge),
`HangarWindow::render` (load order), `RocketGun::update` (reg), `PlayerEgo`
(store order), `Level::createMission` (stack slot 88 vs 84), `Hud::draw` (load
order), `MenuTouchWindow` ctor (stack slot), `SpaceLounge` (reg),
`MenuTouchWindow::OnTouchBegin` (reg), `ModStation` (reg), `MGame::OnUpdate`
(reg).

---

## 8. Reproduction / scratch

All scratch is under `/dev/shm/wm202/`:

| file | contents |
|---|---|
| `oracle_*.c` | the 20 oracle blocks listed in §2.4 |
| `firstdiff.log` | first structural diff for all 20 symbols |
| `createship.dis` | original `Level::createShip` disassembly |
| `gencm.dis` | original `Generator::createMission` disassembly |
| `createmission.dis` | original `Level::createMission` disassembly |
| `playerego.dis` | original `PlayerEgo::calcCollision` disassembly |
| `createScene_reorder.txt` | side-by-side of the reverted 2/4/23 experiment |
| `verify_reorder.log` | gate output of the reverted experiment |
| `oracle_summary.txt` | per-function call sequences + `this` offsets |
| `DumpStructs.java`, `ghidra_structs.log` | headless Ghidra struct dump |

The canonical build was not touched. `report.json` is auto-synced by `check`
and is not part of this commit.
