# LevelScript::process — DATA-FLOW pass (wave 183, w12703)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12703`, branch
`wave183-w12703`, base master `9555474d8` (wave-182 harvest 5, raw **52,412**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12703`.

Continuation of wave-182 w12604.  The wave-182 baseline note groups w12604's
data-flow cells into the 52,412 figure, but the cells are **not present in the
current tree**: the harvested body is w12605's (its note independently reports
52,523 -> 52,412), and the w12604 wins were lost when that body replaced the
w12604 tree.  Re-measured on the current body, all four cells still win and
compose, so they are re-landed here.

## Landed win (code)

Four data-flow cells, all size-exact and `.rel.plt`-identical:

| # | site | before | after | alone raw |
|---:|---|---|---|---:|
| 1 | Chain-A station-departure intro (`m_pCamera->translate`) | `(float) (-2 * delta)` | `(float) -delta * 2.0f` | 52,410 |
| 2 | escort route0 y (`int route0[3]`) | `(int) (r.y * 900000.0f + d.y * -900000.0f)` | `(int) ((r.y - d.y) * 900000.0f)` | 52,404 |
| 3 | escort route1 x (`int route1[3]`) | `(int) (d.x * -900000.0f - r.x * 900000.0f)` | `(int) ((d.x - r.x) * -900000.0f)` | 52,410 |
| 4 | mission==1 intro enemy[0] creep | `(float) delta * 0.2f * depth` | `(float) delta * (0.2f * depth)` | 52,409 |
| all four | | | | **52,397** |

Measured (base 52,412):

```
                         base (HEAD)      this pass
process raw diffbytes    52,412           52,397     (-15)
process aligned insns    22,050           22,087     (+37)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,728
.text diffbytes          75,015           75,000     (-15)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact at 0x1a3b78)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,681           75,666     (-15)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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
included) from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.  The aligned count rises
by 37 (register allocation), but raw and `.text` fall and every section size is
unchanged.

Cells 1–3 are the wave-182 w12604 cells (that note's "win 1", "win 2" and
"re-land of win 1"); cell 4 is w12604's "win 3".  The original disassembly
confirms each shape:

* cell 1: original emits `vcvt.f32.s32` of `delta`, then `vmul` by `2.0f`
  (not an integer `-2 * delta` followed by one conversion).
* cells 2/3: original emits a single `vcvt.f32.s32` of the integer difference
  `r.y - d.y` / `d.x - r.x` and one `vmul` by `900000.0f`, not two converted
  products and an add.
* cell 4: original associates `0.2f * depth` first, then multiplies by the
  converted delta.

## Rejected / neutral on this body

Re-probed the wave-182 w12604 rejected table on the 52,412 body (single edits,
all semantics-preserving, measured with the same harness):

| site | variant | raw |
|---|---|---:|
| escort route0 x | `(int) ((r.x - d.x) * 900000.0f)` | 52,404 (neutral vs cell 2 alone) |
| escort route1 y | `(int) ((d.y - r.y) * -900000.0f)` | 52,412 (neutral) |
| escort route0 z | `(int) ((r.z - d.z) * 900000.0f)` | 52,412 (neutral) |
| escort route1 z | `(int) ((d.z * 90000.0f - r.z * 900000.0f))` | 52,412 (neutral) |
| L309 `(float)(-2*delta)` | `(float) -delta * 2.0f` | 52,410 (same as cell 1, duplicate site) |

The other `(float)(N*delta)` sites from the w12604 table remain non-wins on
this body (they were measured against 52,523 there and the body has since
changed); no additional size-exact raw-lowering cell was found among them.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12703
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52397 aligned= 22087 size= 57232/57232 insn= 20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52397 size=57232/57232 text=75000 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The measurement harness (`eval.py`, `cc_cmd.txt`, `link_cmd.txt`, `secdiff.py`)
lives in `/home/debian/GalaxyOnFire2/scratch/w12703/`; it compiles one
`LevelScript.cpp` variant with the exact `compile_commands.json` command and
gold-links it with the exact `build.ninja` link line (stopping after the same
strip/reorder steps), then reports raw/size/`.rel.plt` md5.  Scratch, not
tracked.

## Second win: `(float) (-10 * delta)` (code)

Four sites in the mission>=2 general state machine (station/fighter moveForward and
the case-158 camera translate) read `(float) (-10 * delta)`.  The original
converts `delta` and multiplies by `-10.0f`; the integer product form forces an
extra negate/convert pair.  Changed all four occurrences uniformly to
`(float) delta * -10.0f`.

Only the first two sites contribute bytes; sites 3/4 are neutral, but the
uniform form keeps the source consistent and measures the same:

| site | line | alone raw |
|---|---:|---:|
| station->rootGeometry->moveForward | 4990 | 52,396 |
| ((PlayerStation *) ...)->rootGeometry->moveForward | 5015 | 52,395 |
| ((PlayerStation *) ...)->rootGeometry->moveForward | 5036 | 52,397 (neutral) |
| m_pCamera->translate (case 158) | 5158 | 52,397 (neutral) |
| all four | | **52,394** |

```
                         after win 1      after win 2
process raw diffbytes    52,397           52,394     (-3)
process aligned insns    22,087           22,105     (+18)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,724
.text diffbytes          75,000           74,997     (-3)
.ARM.extab diffbytes     646              646        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,666           75,663     (-3)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

The exact spelling matters: `(float) -delta * 10.0f` is much worse (raw 52,758),
while `(float) delta * -10.0f` and `(float) (-10.0f * delta)` both give 52,394.
