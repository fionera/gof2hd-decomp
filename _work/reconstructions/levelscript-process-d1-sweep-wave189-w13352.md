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
