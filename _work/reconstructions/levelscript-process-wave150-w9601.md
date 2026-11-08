# LevelScript::process — wave 150 / w9601: case-154 s9 completion placed
# immediately before the case-157 strong arm (adjacency found, size +40)

Status: **analysis only, source pristine** (no source change landed; the
adjacency shape is +40 B and would shift every following function).
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w9601`, base master
`7eed2966e` (wave 149).  Scratch in `/dev/shm/w9601/`.

```
                         baseline (HEAD)   best adjacency shape
process raw diffbytes    54,898            55,314
align_diff TOTAL         15,802            20,160
orig / ours size         57232 / 57232     57232 / 57252
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              (size +20: not landable)
```

## 1. NEW result: the c154 s9 completion CAN be placed immediately before
##    the strong arm, with a two-sided `__builtin_expect`

The lever is a *pair* of state-switch weights, one on each mission:

* case-154 state-9 guard: `__builtin_expect(m_nState == 9, 1)`
  (line 4672) — raises the frequency of the whole case-154 s9 chain so
  the block-placement pass (`selectBestCandidateBlock`, which picks the
  highest-frequency work-list block) places it before the strong arm;
* case-157 state-4 guard: `__builtin_expect(m_nState == 4, 0)`
  (line 4807) — lowers the state-5 arm so the strong arm is placed after.

Result (`e9 + s4e0`), disassembly of the built object:

```
c184  ldr r1,[pc,#308]          ; c154 s9 completion begins
c192  movs r1,#8                ; <- c154 marker
...
c288  movs r0,#6
c28a  strd r4,r4,[r9]
c28e  b.w 98a2                  ; completion ends
c292  ldr r6,[sp,#88]           ; <- strong arm begins (no block between)
c294  movs r5,#11
c296  movs r4,#0
c298  b.n c2d4
```

i.e. the completion block is **immediately followed** by the strong arm,
exactly like the original (`orig 0x144a12 b.n 144e36` then
`0x144a14 ldr r6,[sp,#96]; 0x144a16 movs r5,#11`).

Gap table (marker `movs r1,#8` → strong `movs r5,#11`; the mission's
`setAlwaysFriend`→strong gap is 6 B smaller):

| variant | size | c154 | strong | gap(marker) | gap(setAlwaysFriend) |
|---|---:|---|---|---:|---:|
| original | 57232 | 0x14490e | 0x144a16 | **0x108** | **0xe4** |
| baseline | 57232 | 0xcd2e | 0xc656 | -0x6d8 | -0x6d8 |
| `e9 + s4e0` | 57272 | 0xc192 | 0xc294 | **0x102** | **0xde** |
| `e9 + s7e0` | 57252 | 0xc3e6 | 0xc4ea | **0x104** | 0xe0 |
| `e9 + s4e0 + inc` | 57264 | 0xc18a | 0xc28e | 0x104 | 0xe0 |
| `e9 + s7e0 + inc` | 57248 | 0xc3e2 | 0xc4e8 | 0x106 | 0xe2 |
| `e9 + s4e0 + s7e0` | 57256 | 0xc196 | 0xc298 | 0x102 | 0xde |

`inc` = `m_nState = 6` → `m_nState++` (line 4711, see §2).

**The residual 6-byte gap is exactly the c154-completion tail difference**
(§2): the original inlines `m_nState++` (16 B) while our build tail-merges
the increment (12 B + shared blocks).  With the tail fixed the shape would
be byte-identical in this region.

## 2. The `m_nState = 6` size hack is semantically wrong

Wave 148 (commit `595eee933`) replaced the case-154 state-9 completion's
`m_nState++` with `m_nState = 6` as a **+4 B companion** for the strong-arm
`m_bCinematicBreak` removal (its message says so explicitly).  But the
original at `0x144a08` is

```
144a08  ldr.w r0,[sl,#28]   ; m_nState
144a0c  adds  r0,#1
144a0e  str.w r0,[sl,#28]   ; m_nState++
```

i.e. `m_nState++` (state 9 → 10), not `m_nState = 6` (which would loop
back to state 6).  Our build tail-merges the increment into the shared
`adds r0,#1; b.w 98ca` / `str r0,[sl,#28]` blocks, so `m_nState++`
alone gives **57228 B (-4)**.  A *different* +4 companion is needed if
this is to be landed; `int destroyed = 0;` (measured) has no size effect.

## 3. Where the +40 B comes from

It is **not** the target block: the c154→strong region is 0x102 vs the
original 0x108, i.e. 6 B *shorter*.  The +40 B is branch relaxation in
the hot region: the `e9` expect turns the case-154 state switch from a
`tbh` jump table into a compare chain and reorders the mission case
entries (135/145/142/144 are no longer monotonic).  `align_diff` rises
15,802 → 20,160 for the same reason.

## 4. Case-entry table (`e9 + s4e0`, size 57272)

Produced by the `table.py` below (same parser as w9303/w9505).  Rows
`0` and `132..143` are parser artefacts of the `tbh` padding; the real
case order is the first 31 rows.

```
case      orig     ours  dEntry   spanO   spanU    dSpan
158     138862   138862      +0     586     614      -28
131     138aac   138ac8     -28     426     430       -4
64      138c56   138c76     -32    1386    1784     -398
135     1391c0   13936e    -430     310    -578     +888
145     1392f6   13912c    +458     590    1016     -426
142     139544   139524     +32     156    -124     +280
144     1395e0   1394a8    +312     124     286     -162
139     13965c   1395c6    +150     196     204       -8
69      139720   139692    +142     276     276       +0
73      139834   1397a6    +142      94      94       +0
67      139892   139804    +142     272     320      -48
70      1399a2   139944     +94   49926   50060     -134
29      139ca2   139a64    +574     180     208      -28
41      139d56   139b34    +546     222     254      -32
80      139e34   139c32    +514     224    1244    -1020
56      139f14   13a10e    -506     136    -954    +1090
125     139f9c   139d54    +584      98    1068     -970
92      139ffe   13a180    -386     778     748      +30
157     13a308   13a46c    -356     464     454      +10
40      13a4d8   13a632    -346     384     566     -182
102     13a658   13a868    -528     278    -220     +498
154     13a76e   13a78c     -30     450     354      +96
24      13a930   13a8ee     +66     130     148      -18
14      13a9b2   13a982     +48     370     792     -422
21      13ab24   13ac9a    -374     134    -414     +548
42      13abaa   13aafc    +174     152     600     -448
91      13ac42   13ad54    -274     106     114       -8
94      13acac   13adc6    -282     504    -558    +1062
105     13aea4   13ab98    +780     314    -210     +524
114     13afde   13aac6   +1304      54    1282    -1228
16      13b014   13afc8     +76   40482   38960    +1522
```

The baseline table (wave 149) is unchanged from w9303/w9505:
`158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16`.

## 5. Other expect shapes measured (all size-exact unless noted)

| variant | size | gap(marker) |
|---|---:|---:|
| `expect(s4,1)` (w9505) | 57228 | 0x6a0 |
| `expect(s2,1)` | 57200 | 0x956 |
| `expect(s3,1)` | 57248 | 0x8e4 |
| `expect(s2,0)` | 57216 | 0x5c8 |
| `expect(s3,0)` | 57220 | 0x5c8 |
| c154 `isOver` e1 + s7e0 | **57232** | 0xa3c |
| c154 `isOver` e1 + s4e0 | 57228 | 0xaf2 |
| c154 `timer>=2001` e1 | 57232 | -0x6d8 |
| c154 `timer>=2001` e0 | 57204 | -0x1790 |
| `expect(isRunningHDonWeakDevice,1)` | 57224 | 0x11c2 |
| `expect(isRunningHDonWeakDevice,0)` | 57224 | -0x3120 |

`c154 isOver e1 + c157 s7e0` is the only shape found that is both
size-exact (57232) and gate-green, but its gap is 0xa3c and its raw
(55,343) / aligned (19,685) both regress, so it was not landed.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9601/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9601/align_diff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9601/table.py
```

`fdiff.py` / `align_diff.py` are the shared tools with the worktree path
substituted.  The probe used here locates the c154 marker
(`movs r1,#8` + `str r1,[r0,#40]`) and the strong arm (`movs r5,#11`).

## 7. Open questions

1. Land the adjacency shape at exactly 57232 B.  The +40 B is hot-region
   branch relaxation; the target block itself is 6 B *shorter* because of
   the `m_nState = 6` tail-merge hack.  Fixing `m_nState++` (-4) and
   finding a new +4 companion plus a hot-region -36 is the remaining
   work.
2. `expect(m_nState == 9, 1)` on case 154 changes the case-154 state
   switch from a `tbh` jump table to a compare chain.  A weaker weight
   (clang 7 only has 1:2000) or a different case-154 guard may raise the
   chain frequency without destroying the jump table.
3. The mission case order is reordered by both `e9` and `s4e0`; the
   baseline order is only preserved with no expects at all.

## 8. Probe (self-contained)

```python
# probe: locate c154 marker and strong arm in a linked .so
import subprocess, re
OBJ='.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
def ins(path, name='_ZN11LevelScript7processEi'):
    out=subprocess.check_output([OBJ,'-T',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name: a,sz=int(p[0],16),int(p[4],16)
    out=subprocess.check_output([OBJ,'-d','--start-address=%d'%a,
                                 '--stop-address=%d'%(a+sz),path],text=True)
    r=[]
    for l in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+((?:[0-9a-f]{2,8}\s*)+)\s+(.*)$',l)
        if m: r.append((int(m.group(1),16),m.group(3).strip()))
    return r
def probe(path):
    r=ins(path); res={}
    for i,(ad,t) in enumerate(r):
        if re.match(r'movs\s+r1,\s*#8',t) and i+2<len(r) and \
           re.match(r'str\s+r1,\s*\[r0,\s*#40\]',r[i+2][1]):
            res['c154']=ad; break
    for ad,t in r:
        if re.match(r'movs\s+r5,\s*#11',t): res['strong']=ad; break
    return res
```

The case-entry table parser is the one inlined in
`_work/reconstructions/levelscript-process-wave147-w9303.md` §Reproduction
(reads the mission compare chain and the two `tbh` tables).
