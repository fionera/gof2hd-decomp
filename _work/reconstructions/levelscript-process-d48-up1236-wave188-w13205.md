# LevelScript::process — wave 188 / w13205: K=4..8 single-move sweep, first win

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13205`, branch
`wave188-w13205`, base master `ce16f45f1` (wave-187 harvest 5, raw **51,930**,
aligned 20,928).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13205`.

The w13005/w13105 harness was not preserved in scratch, so it was rebuilt from
the w13105 note (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `sweep.py`,
`elfread.py`).  The K=4..8 (d4..8) single-move sweep was re-run on the current
51,930 body.  One size-exact, `.rel.plt`-identical raw winner was found and is
landed below; the sweep continues for the rest of the body.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,925     (-5)
process aligned insns    20,928           20,928     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,223           74,218     (-5)
.ARM.extab diffbytes     639              639        (unchanged; size/base exact)
.ARM.exidx diffbytes     0                0
.rodata / .data          exact            exact
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`.  `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Landed move

Line numbers are from the base (`ce16f45f1`, 1-based); the harness statement
ids are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 1241 | up | `m_nState = 12;` | `m_tempVec = (*enemies)[enemies->count - 10]->geometry->getPosition();` (1237) … `m_pLevel->lodManager->forceUpdate(30, false);` (1240) |

The block is the mission-0 state-12 camera staging.  `m_nState = 12` is a pure
flag store; every crossed statement reads/writes only `m_tempVec` (via the
`getPosition()` result and the `.y`/`.z`/`.x` adds) or calls
`m_pLevel->lodManager->forceUpdate`.  No crossed statement reads `m_nState`, so
the move is semantics-preserving.  It is worth **-5 raw bytes**.

## Harness bug found and fixed

The first sweep run reported every one of the 7,749 candidates as exactly
51,930.  The link command in `build.ninja` spells the LevelScript object as
`CMakeFiles/gof2.dir/./src/game/world/LevelScript.cpp.obj` (note the `./`),
while the harness replaced the un-dotted path, so every candidate was linked
with the **baseline** object.  After fixing the replacement string the sweep
immediately produced the -5 winner above (and size-changing candidates such as
`up 610,613` at +88 B).  Future workers should grep the exact object path out
of `linkonly.txt` rather than reconstructing it.

## Method

* `stmts.py` scans the `process` body (file lines 605-5372), tracks brace
  depth and keeps single-line `;`-terminated statements with balanced
  braces/parens, excluding control keywords/labels/`break`/`return`.
  **2,843 statements in 784 runs** (max run 29).
* `model.py` computes prefix-aware read/write access chains per statement
  (`(*x)[i]->y` normalised to `x[i]->y`; casts stripped; index expressions
  read; calls read+write their receiver and keep same-receiver call order).
* `gen.py` generates both directions for every pair within a run, keeping a
  move only when the moved statement is independent of every crossed statement
  including the far endpoint.  d4..8: **6,204 candidates**.
* `eval.py` compiles one candidate with the exact `compile_commands.json`
  command, runs the exact gold link command (stopping before strip), and
  reports the same-address raw diff, function size and `.rel.plt` md5 with a
  minimal ELF32 reader (`elfread.py`).  Four workers, `ulimit -v 12000000`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13205
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51925 aligned= 20928 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `elfread.py`, `res_d48b.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13205/`; they are scratch, not tracked.
