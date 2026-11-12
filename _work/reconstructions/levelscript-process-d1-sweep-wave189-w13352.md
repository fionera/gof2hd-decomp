# LevelScript::process — wave 189 / w13352: fresh adjacent-statement sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13352`, branch
`wave189-w13352`, base master `fbc94984a` (raw **51,845**, `.text` 73,946,
`.ARM.extab` 615).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13352`.

Task: state-machine dispatch / register-allocation residual.  The wave-189
statement-permutation sweeps were all run on the 51,917 wave-188 body; this
pass re-ran the independent adjacent-statement (`d=1`) sweep on the current
51,845 body with the w13332 harness (exact NDK r18b compile + gold link).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,845           51,844     (-1)
process aligned insns    20,997           20,992     (-5)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,946           73,945
.ARM.extab diffbytes     615              615        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 byte-identical   byte-identical   (29,392 B)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

## Landed change

Mission-154 (`case 154`) state-3 block, source lines 4629-4630:

```c++
-  m_pCamera->setTarget(player->geometry);
-  player->setSpeed(0.0f);
+  player->setSpeed(0.0f);
+  m_pCamera->setTarget(player->geometry);
```

The two calls have disjoint receivers (`PlayerEgo` vs `TargetFollowCamera`)
and neither reads state written by the other, so the reorder is
semantics-preserving.  Measured alone: raw **51,845 -> 51,844** (-1),
size 57,232/57,232 exact, aligned 20,997 -> 20,992, `.rel.plt` byte-identical,
`.ARM.extab` content unchanged (615).

## Sweep

The w13332 harness (`stmts.py`/`model.py`/`gen.py`/`eval.py`/`sweep.py`,
copied into `scratch/w13352/`) was re-pointed at this worktree.  A full
`d=1` sweep of all 1,526 unique independent adjacent statement pairs on the
current body (0.37 s/candidate, 4 workers) found exactly **7** size-exact raw
wins, all -1:

| 0-based lines | pair | safe? |
|---|---|---|
| 828/829 | `getStarSystem()->switchPlanetForIntro()` / `switchSkyboxForIntro()` | yes, but `.rel.plt` moves (PLT first-reference order) — rejected |
| 889/890 | `m_tempVec += Vector{...}` / `setPosition(m_tempVec)` | read-after-write on `m_tempVec` — unsafe |
| 2531/2532 | `camPos.y += 800` / `setPosition(camPos)` | read-after-write on `camPos` — unsafe |
| 3734/3735 | `*camPos += *tmp*400` / `setPosition(*camPos)` | read-after-write — unsafe |
| 4541/4542 | `m_tempVec += Vector{...}` / `setPosition(m_tempVec)` | read-after-write — unsafe |
| **4628/4629** | `setTarget(player->geometry)` / `setSpeed(0.0f)` | **independent — landed** |
| 4962/4963 | `m_tempVec += Vector{...}` / `chaser->setPosition(...)` | read-after-write — unsafe |

Note the harness line numbers are 0-based (`stmts.parse` stores `i`), so the
landed pair is source lines 4629/4630.

The `switchPlanetForIntro`/`switchSkyboxForIntro` swap is also a raw -1 but
changes `.rel.plt` (the first reference to one of the two imports moves), so
it is not landable under the byte-identical `.rel.plt` rule.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13352
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51844 aligned= 20992 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `elfread.py`,
`sweep.py`, `linkonly.txt`, `res_d1.jsonl`, `secdiff.py`, `rawruns.py`,
`apply_swap.py`, `measure.sh`) live in
`/home/debian/GalaxyOnFire2/scratch/w13352/`; scratch, not tracked.

## Second landed change (d=2..4 sweep)

A fresh `d=2..4` sweep on the post-landing 51,844 body (4,774 candidates,
4 workers) found 31 size-exact raw wins, all -1..-3.  The largest safe one is
landed: mission-125 cutscene-exit block, source lines 2337-2339, move
`m_bCinematicBreak = 1;` below `m_pCamera->setTarget(...)` and
`m_pCamera->setPosition(...)`.

```
raw 51,844 -> 51,841 (-3), aligned 20,992 (unchanged)
.text 73,945 -> 73,942 (-3), total 74,580 -> 74,577
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```

`m_bCinematicBreak` is a plain flag byte; neither camera call reads it, so the
reorder is semantics-preserving.  The other `d=2..4` winners are either
read-after-write rotations (`m_tempVec` / `camPos` / `*tempVec` / `*scratch`)
or `setPosition`-before-`+=` rotations and were rejected as unsafe.

## Third landed change (d=2..4 sweep)

Mission-41 state-2 block, source lines 2061-2063: move
`(*enemies)[0]->geometry->updateReferenceMatrix();` below
`KIPlayer *freighter = ...; freighter->translate(...);`.

```
raw 51,841 -> 51,838 (-3), aligned 20,992 -> 21,002
.text 73,942 -> 73,939 (-3), total 74,577 -> 74,574
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```

The reorder is a raw win; `.rel.plt` and `.ARM.extab` are unchanged.

## Fourth landed change

Station-dock block, source lines 5103-5106: move `fighter->setVisible(false);`
below `m_nScriptTimerA = 0; m_nScriptTimerB = 0;`.

```
raw 51,838 -> 51,836 (-2), aligned 21,002 -> 21,018
.text 73,939 -> 73,937 (-2), total 74,574 -> 74,572
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```

`fighter->setVisible` and the two timer stores are independent.

## Fifth landed change

Station-dock block, source lines 5035-5037: hoist
`player->geometry->setDirection(...)` above
`m_pCamera->setPosition(m_tempVec + Vector{35000,0,70000})`.

```
raw 51,836 -> 51,834 (-2), aligned 21,018 -> 21,028
.text 73,937 -> 73,935 (-2), total 74,572 -> 74,570
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```

## Sixth landed change

Mission-145 state-1 block, source lines 4411-4413: hoist
`m_pCamera->setPosition(m_tempVec);` above the second
`m_tempVec += ...getRightVector() * 3000.0f;` addend.

```
raw 51,834 -> 51,832 (-2), aligned 21,028 -> 21,033
.text 73,935 -> 73,933 (-2), total 74,570 -> 74,568
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```

## Seventh landed change

Camera-local block, source lines 4205-4208: hoist
`AEMath::MatrixSetTranslation(local, *tempVec);` above the last two
`*tempVec +=` addends.

```
raw 51,832 -> 51,830 (-2), aligned 21,033 -> 21,479
.text 73,931 -> 73,931?  (73,933 -> 73,931, -2), total 74,568 -> 74,566
.ARM.extab 615 (unchanged), .rel.plt byte-identical, size 57,232/57,232
```
