# LevelScript::process — DATA-FLOW pass (wave 186, w13003)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13003`, branch
`wave186-w13003`, base master `2d02567f6` (wave-185 harvest, raw **52,075**,
`.text` 74,402).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13003`.

Continuation of the wave-182/183/185 data-flow passes (w12604, w12703,
w12903).  One size-exact, `.rel.plt`-identical raw win landed.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,075           52,074     (-1)
process aligned insns    20,766           20,765     (-1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,402           74,401     (-1)
.ARM.extab diffbytes     647              647        (size/base exact at 0x1a3b78, 156,312 B)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,069           75,068     (-1)
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
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).

## Landed change (source line 3825)

Mission-0 `m_nState == 9` hitpoint/route gate, inside the
`__builtin_expect(..., 0)` condition:

```cpp
// before
reinterpret_cast<Route *>(player->getRoute())->getWaypoint(0)->state != 0, 0)) {
// after
(uint8_t) reinterpret_cast<Route *>(player->getRoute())->getWaypoint(0)->state, 0)) {
```

`Waypoint::state` is a genuine `uint16_t` at offset `0x130`
(`Waypoint::reached()` writes `movw r1,#0x101; strh.w r1,[r0,#304]` and
`Waypoint::reset()` writes a byte, both byte-identical in our tree).  The
original `process()` however reads only the **low byte** at this site:

```
142bc8: f890 0130   ldrb.w r0, [r0, #304]   ; original
142bc8: f8b0 0130   ldrh.w r0, [r0, #304]   ; ours before this change
142bcc: 2800        cmp    r0, #0
142bce: f001 8711   beq.w  ...
```

The other two `state` reads in `process()` (source lines 3981 and 4289) are
already written as `(uint8_t) ...->state` and emit `ldrb.w`; this site was the
only one still reading the halfword.  The explicit `(uint8_t)` cast is
behaviour-identical for the `!= 0` test (the low byte is the flag that
`reset()` clears) and makes the emitted load match the original.

Measured alone on the wave-185 body: raw **52,075 -> 52,074** (-1 B), size
57,232/57,232 exact, `.text` 74,402 -> 74,401, `.rel.plt` unchanged.

## Method

`scratch/w13003/harness.py` compiles one `LevelScript.cpp` variant with the
exact `compile_commands.json` command, runs the exact `build.ninja` gold link
(stop before strip), and measures with `_work/reconstructions/measure_w11901.py`
plus `tools/fn_diff.py`; ~1.5 s/candidate.  `side.py` / `diffmap.py` map our
`-g` line info onto the original disassembly.

The `ldrb`/`ldrh` mismatch was found by listing every `ldrb`/`ldrh` in the
original function body and comparing the real code sites against ours.  The
original's only real `ldrh`-looking words are literal-pool data; the three
`ldrb.w [rX,#304]` sites are the `Waypoint::state` reads.

## Re-probed and rejected on the current body

All size-exact and `.rel.plt`-identical unless noted; base 52,075.

* `(float) -delta` -> `(float) (-delta)` at all 29 sites: neutral (identical
  code).
* `-(float) delta * 0.5f` -> `(float) (-delta) * 0.5f` (line 1080, case 2):
  matches the original shape (`negs` + `vcvt` of `-delta`, then `vmul` by
  `+0.5`) but is size-changing: raw 54,198, size **57,240** (+8).  The original
  case-2 translate at `0x13ba5c` does `vmov.f32 s0,#96` (+0.5) and converts
  both `delta` and `-delta`; our body converts `delta` once and uses
  `vmov.f32 s0,#224` (-0.5).  A size-neutral companion of exactly -8 B was not
  found, so this is not landed.
* `(float) delta * -10.0f` -> `(float) (delta * -10)` at lines 4996/5021/5042/
  5166: +1/+2 raw each (size-exact).  The original materialises `-10` as an
  integer `add/neg/lsl` + `vcvt` (no `vmov.f32 #164`), but the local shape win
  does not compose into a raw win.
* `(float) (delta * 3)` -> `(float) delta * 3.0f` (line 1227): +357 raw,
  size-exact.  `(float) (delta * 4)` -> `(float) delta * 4.0f` (line 1174):
  +372 raw, size-exact.
* `(float) (2 * delta)` -> `(float) 2.0f * delta`: +47 raw (size-exact);
  `(float) (-2 * delta)` -> `(float) -2.0f * delta`: +31 raw (size-exact);
  `(float) (-18 * delta)` -> `(float) -18.0f * delta`: neutral.
* `(float) (m_nScriptTimerA - 4000)` -> split cast: +6 raw, size-exact.
* `__builtin_expect` flips at lines 4977 and 4183: size 57,240 / 57,232 but
  raw 53,871 / 52,176 (reject).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13003
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52074 aligned= 20765 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52074 size=57232/57232 text=74401 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses (`harness.py`, `try.py`, `sweep.py`, `sweep2.py`, `side.py`,
`diffmap.py`, `typediff.py`, `elf.py`) and result tables (`sweep/res.json`,
`sweep2/res.json`) live in `/home/debian/GalaxyOnFire2/scratch/w13003/`; they
are scratch, not tracked.
