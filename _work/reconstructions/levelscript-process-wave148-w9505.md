# LevelScript::process — wave 148 / w9505: state-machine switch weights via
# `__builtin_expect(m_nState == N)` (the if-chain→switch `!prof` lever)

Status: **analysis only, source pristine** (baseline re-measured, no source
win landed).  Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w9505`, base master
`fece77a64` (wave 148).  Scratch in `/dev/shm/w9505/`.

```
                         baseline (HEAD)   this pass
process raw diffbytes    54,898            54,898   (source pristine)
align_diff TOTAL         15,802            15,802
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4507              4507
linked_exact             4507              4507
```

## 1. NEW lever found: the case-157 state switch weights ARE source-controllable

The case-157 state machine is an `if / else if (m_nState == N)` chain, but
SimplifyCFG converts states 2..7 into a real `switch`:

```
switch i32 %13577, label %14201 [   ; states 2..7
    i32 2, label %13578
    i32 3, label %13648
    i32 4, label %13669
    i32 5, label %13839          ; <- isRunningHDonWeakDevice test
    i32 6, label %14004
    i32 7, label %14066
]
```

In the baseline this switch has **no `!prof`** (uniform 1/6 each).  Adding
`__builtin_expect(m_nState == N, V)` to the *source* `else if` guard survives
the if-chain→switch conversion as a `!prof !{branch_weights}` on the switch
(verified in the `-print-after-all` IR dump: the switch gains `!prof`).  This
is a new, previously untried lever: w9304/w9405 only put expects on the
mission switch scrutinee, the case-154 guards, and `isRunningHDonWeakDevice`.

Measured switch weights (from the post-placement MBB dump):

| variant | state2 | state3 | state4 | state5 | state6 | state7 |
|---|---:|---:|---:|---:|---:|---:|
| baseline | 16.7% | 16.7% | 16.7% | 16.7% | 16.7% | 16.7% |
| `expect(state==4,1)` | 33.3% | 33.3% | 33.3% | 0.01% | 0.00% | 0.00% |

i.e. `expect(state==4,1)` produces weights `[2000,2000,2000,1,1,1]` (all
cases up to and including the expected one become hot; the rest cold).

## 2. Effect on the target block placement

`sa` = strong-device arm (`movs r5,#11`), `c154` = case-154 state-9
completion (`setAlwaysFriend` + `resetGunDelay`), orig `sa=0x144a16`,
`c154=0x144932` (c154 immediately before sa, gap `0xe4`).

| variant | size | raw | align | sa | c154 | c154<sa? |
|---|---:|---:|---:|---|---|---|
| baseline | 57232 | **54898** | **15802** | 0x14436e | 0x144a6a | no |
| `expect(state==4,1)` | 57228 | 55139 | 19076 | 0x144ab0 | 0x144434 | **yes** |
| `expect(state==5,1)` | 57228 | 55204 | — | 0x143432 | 0x144670 | no |
| `expect(state==5,0)` | 57200 | 55337 | — | 0x1459c0 | 0x144662 | no |
| `expect(state==6,1)` | 57236 | 55345 | — | 0x1438a2 | 0x144738 | no |
| `expect(state==7,1)` | 57236 | 55160 | — | 0x143ee8 | 0x144960 | no |
| `expect(state==4,1)+expect(state==7,1)` | **57232** | 55279 | 19086 | 0x144cb2 | 0x144434 | **yes** |
| `expect(state==4,1)+expect(state==5,1)` | 57300 | 55409 | — | 0x144a3a | 0x14449c | yes |

`expect(state==4,1)` is the first shape found that puts the case-154
completion *before* the strong arm, and it moves `sa` from `0x14436e` to
`0x144ab0` — close to the original `0x144a16`.  But it costs 4 B (57228) and
both metrics regress.  The `+state==7` companion restores exactly 57232 B,
but it **reorders the mission case entries** (case 135/145/142/144/56/80/125
addresses are no longer monotonic in the original's order) and the raw metric
regresses by 381 B.

No shape tried puts the case-154 completion *immediately* before the strong
arm (`sa-c154` stays >= 0x47e, vs the original's 0xe4).  The residual is that
the case-157 state-4 body chain and the case-154 chain still sit between them.

## 3. Case-entry table (baseline, unchanged from w9303 order)

Case entry order is identical to the original:
`158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16`.

```
case      orig     ours  dEntry   spanO   spanU    dSpan
158     138862   138862      +0     586     614      -28
131     138aac   138ac8     -28     426     430       -4
64      138c56   138c76     -32    1386    1250     +136
135     1391c0   139158    +104     310     314       -4
145     1392f6   139292    +100     590     534      +56
142     139544   1394a8    +156     156     196      -40
144     1395e0   13956c    +116     124     124       +0
139     13965c   1395e8    +116     196     170      +26
69      139720   139692    +142     276     276       +0
73      139834   1397a6    +142      94      94       +0
67      139892   139804    +142     272     320      -48
70      1399a2   139944     +94   49926   50020      -94
29      139ca2   139a64    +574     180     208      -28
41      139d56   139b34    +546     222     254      -32
80      139e34   139c32    +514     224     228       -4
56      139f14   139d16    +510     136     130       +6
125     139f9c   139d98    +516      98     954     -856
92      139ffe   13a152    -340     778     798      -20
157     13a308   13a470    -360     464     462       +2
40      13a4d8   13a63e    -358     384     346      +38
102     13a658   13a798    -320     278     134     +144
154     13a76e   13a81e    -176     450     224     +226
24      13a930   13a8fe     +50     130     148      -18
14      13a9b2   13a992     +32     370     324      +46
21      13ab24   13aad6     +78     134     134       +0
42      139abaa  13ab5c     +78     152     156       -4
91      13ac42   13abf8     +74     106     114       -8
94      13acac   13ac6a     +66     504     554      -50
105     13aea4   13ae94     +16     314     258      +56
114     13afde   13af96     +72      54      54       +0
16      13b014   13afcc     +72   40482   40638     -156
tail    144e36   144e8a     -84    3698    3614      +84
```

(The `42` row's orig column is `13abaa`; typo above is cosmetic.)

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9505/fdiff.py _ZN11LevelScript7processEi        # 54898 / 57232
python3 /dev/shm/w9505/align_diff.py _ZN11LevelScript7processEi  # 15802
python3 /dev/shm/w9505/table.py                                  # case-entry table
```

`/dev/shm/w9505/drive.py` (patch+build+probe harness), `probe.py`
(strong-arm / case-154-completion locator), `blocks.py` (basic-block map),
`table.py` (case-entry table).

## 5. Open questions

1. `expect(m_nState == 4, 1)` proves the state-switch weight is a live lever,
   but clang 7 can only express 2000:1.  An intermediate weight (state 5 at
   roughly 1/6 .. 1/20) is needed to put the case-154 chain immediately before
   the strong arm without reordering the mission cases.  A source shape that
   yields an intermediate switch weight has not been found.
2. The `+state==7` size companion reorders the mission case entries, so the
   size restore must come from a change that does not touch the state switch.
3. Case 125's outlined nested switch (`-856 B` span) is still the largest
   single per-case delta.
