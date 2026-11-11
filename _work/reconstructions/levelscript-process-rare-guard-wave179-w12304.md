# LevelScript::process — wave 179 / w12304: case-142 spark-guard expect

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12304`, branch
`wave179-w12304`, base master `64014ce6f` (wave-178 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12304/`.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    53,082           53,079     (-3)
process aligned insns    20,149           20,150     (+1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,729
.text diffbytes          77,936           77,933     (-3)
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
included), from `tools/fn_diff.py`; it is the primary metric.  The +1 aligned
instruction is a block-placement side effect and does not change the size.

## The change

One `__builtin_expect` hint in the mission-142 (`case 142`) state-1 gas-cloud
scan, at source line 4289.  The loop walks the level's gas clouds and sets
`m_nState = 2` as soon as one has sparks.  The original lays the store out as
the cold path (`it ne; strne` after the `getSparks` call); the hint tells clang
the sparks test is unlikely, which reproduces that branch layout.

```cpp
// before
if (((PlayerGasCloud *) (*lvl4280->getGasClouds())[i])->getSparks() != nullptr) {
// after
if (__builtin_expect(((PlayerGasCloud *) (*lvl4280->getGasClouds())[i])->getSparks() != nullptr, 0)) { // lint: arm_builtin
```

Individual measurement: raw **53,079** (base 53,082), size 57,232 exact,
`.rel.plt` byte-identical.  The `Level *lvl4280` alias immediately above the
loop is load-bearing: removing it (using `m_pLevel` directly) gives 53,082
even with the hint, and the hint with `expect(..., 1)` or no hint is also
53,082.

Original loop (0x13fac0..0x13fae8) for reference:

```
13fac0: ldr r6,[sp,#80]            ; &m_pLevel stack slot
13fac2: ldr r0,[r6,#0]             ; m_pLevel
13fac4: blx getGasClouds
13fac8: ldr r0,[r0,#4]             ; array data
13faca: ldr.w r0,[r0,r4,lsl #2]    ; clouds[i]
13face: blx getSparks
13fad2: cmp r0,#0
13fad4: ldr r0,[sp,#88]            ; this
13fad6: add.w r4,r4,#1
13fada: it ne
13fadc: strne r5,[r0,#28]          ; m_nState = 2
13fade: ldr r0,[r6,#0]
13fae0: blx getGasClouds
13fae4: ldr r0,[r0,#0]             ; count
13fae6: cmp r4,r0
13fae8: bcc.n 13fac0
```

## Sweeps

All cells were built with the real gold link (`ninja -C cmake-build-match
libgof2hdaa.so -j2`) and measured from the candidate ELF: raw same-address
diff, function size, whole-`.text` diff and `.rel.plt` md5.

| sweep | cells | variants | size-exact | wins (< 53,082) | size-changing |
|---|---:|---:|---:|---:|---:|
| sub-expression `&&`/`||` operands + simple whole conditions | 843 | 1,686 | 826 | **1** | 860 |
| whole conditions of rare/compound guards | 402 | 804 | 188 | 0 | 616 |
| flip/strip every existing `__builtin_expect` | 134 | 268 | 132 | 0 | 136 |

The sub-expression sweep is the wave-176
`levelscript-process-subs-sweep-w12004.py` cell generator run unchanged on the
wave-179 base (it wraps every `&&`/`||` operand, the inner expression of every
existing `__builtin_expect`, and the inner expression of every `!(...)`).  The
whole-condition sweep wraps the full condition of every rare/compound guard
(`m_nState`, `getCurrentCampaignMission`, `field_0x24`, `m_bStartSequence`,
`field_0xa9`, `m_nScriptTimer*`, `isOver`/`isTriggered`, `inAlienOrbit`, …).
The only size-exact cell below the base is line 4289 variant 0; every other
size-exact cell is neutral at 53,082 (subs) or 53,079 (whole/flip, measured on
top of the landed hint).

Notable near misses (all size-changing, so not landable):

| cell | raw | size | note |
|---|---:|---:|---|
| 4286 `getGasClouds() != nullptr` expect 1 | 53,346 | 57,232 | worse than base |
| 1480 deactivation whole condition expect 1 | 53,170 | 57,232 | worse |
| 5153 `m_nScriptTimerA < 7000` flip 0→1 | 53,198 | 57,252 | +20 B (w12301 territory) |
| 5101 `m_nScriptTimerB > 2000` expect 1 | 53,150 | 57,228 | -4 B |
| 5315 `i < enemies->count` expect 1 | 53,160 | 57,236 | +4 B |

## Rejected variants of the case-142 loop

| variant | raw | size |
|---|---:|---:|
| landed (`lvl4280` alias + expect 0) | **53,079** | 57,232 |
| `m_pLevel` directly + expect 0 | 53,082 | 57,232 |
| `lvl4280` alias, no hint / expect 1 | 53,082 | 57,232 |
| `lvl4280` alias, `int i` | 53,079 | 57,232 |
| `lvl4280` alias, `while` / reversed compare / `i != count` / `++i` | 53,079 | 57,232 |
| `m_pLevel` directly, `continue` form | 53,081 | 57,232 |
| `m_pLevel` directly, ternary `? :` store | 53,081 | 57,232 |
| `lvl4280` alias, ternary `? :` store | 54,227 | 57,240 |

The loop-rotation shape (original is bottom-tested, ours top-tested) does not
change raw; the `it ne; strne` if-conversion is not reproduced by any tested
source form at -Oz.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **53,079** / aligned **20,150** / size
**57,232**; `.text` **77,933**; `.rel.plt` byte-identical
(md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12304
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53079 aligned= 20150 size= 57232/57232 insn= 20729/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53079 size=57232/57232 text=77933 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Scratch harnesses (not tracked): `subs.py` (wave-176 generator, `--mode
subs|flips`), `whole.py` (rare/compound whole-condition sweep), `var142.py`
(case-142 loop variants), `vartern.py` (ternary-condition hints), all in
`/home/debian/GalaxyOnFire2/scratch/w12304/`.
