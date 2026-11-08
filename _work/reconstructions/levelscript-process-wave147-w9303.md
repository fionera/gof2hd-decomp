# LevelScript::process — wave 147 / w9303: per-case size table + case-157
# tail localization (the 0x6ae shift is cold-block *ordering*, not size)

Status: **analysis only, no code change landed** (source pristine at wave-146
HEAD `9c9744ca5`). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU `src/game/world/LevelScript.cpp`. Scratch in `/dev/shm/w9303/`
(not tracked).

## Result

```
                         wave-146 HEAD     this pass
process raw diffbytes    54,869            54,869   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## Method: address->case map from the dispatch

The mission switch is a compare chain (not a jump table) at
`0x1387c6..0x13885e` (orig) / `0x1387d0..0x138868` (ours), plus two `tbh`
tables for the ranges 64..73 and 131..145:

* `sub.w r1, r0, #131; cmp r1, #14; bls.w 138a8a; ... tbh [pc, r1, lsl #1]`
  (table at `0x138a8e`; PC = `0x138a8e`)
* `sub.w r1, r0, #64; cmp r1, #9; bls.w 138c3e; ... tbh [pc, r1, lsl #1]`
  (table at `0x138c42`)

The parser (`/dev/shm/w9303/table.py`, reproduced below) reads the compare
chain and both `tbh` tables and emits every case entry address.  Case 158 is
the `bne.w` fall-through at `0x138862`.  The full per-case entry table:

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
70      1399a2   139944     +94     768     288     +480
29      139ca2   139a64    +574     180     208      -28
41      139d56   139b34    +546     222     254      -32
80      139e34   139c32    +514     224     228       -4
56      139f14   139d16    +510     136     130       +6
125     139f9c   139d98    +516      98     954     -856
92      139ffe   13a152    -340     778     794      -16
157     13a308   13a46c    -356     464     462       +2
40      13a4d8   13a63a    -354     384     346      +38
102     13a658   13a794    -316     278     134     +144
154     13a76e   13a81a    -172     450     224     +226
24      13a930   13a8fa     +54     130     148      -18
14      13a9b2   13a98e     +36     370     324      +46
21      13ab24   13aad2     +82     134     134       +0
42      13abaa   13ab58     +82     152     156       -4
91      13ac42   13abf4     +78     106     114       -8
94      13acac   13ac66     +70     504     554      -50
105     13aea4   13ae90     +20     314     258      +56
114     13afde   13af92     +76      54      54       +0
16      13b014   13afc8     +76   40482   40642     -160
tail    144e36   144e8a     -84    3698    3614      +84
```

(`span` = distance from this case entry to the next case entry in address
order; the last real case `16` runs to the post-switch tail `0x144e36` /
`0x144e8a`; `dSpan` sums to 0 over the whole function.)

## The key result: the case entry ORDER is identical

Sorted by address, both binaries lay the case entries out in exactly the same
order:

```
158, 131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70, 29, 41, 80, 56,
125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16
```

This is **not** source order (14,16,21,24,29,...) and **not** numeric: it is
the backend's block-placement order, and it is reproduced exactly by our
build.  So the case-157 tail shift is **not** caused by a different case
ordering, and not by a different total code size in any case region that the
entry map can see.

## The `dSpan` column is dominated by cold-block placement, not size

The large `dSpan` rows are cold blocks that the backend places between two
case entries in one binary and elsewhere in the other:

* **case 70, `dSpan +480`.**  Orig `0x1399a2..0x139ca2` starts with the
  case-70 state-0 body (`RadioMessage::isTriggered`, `setTurretMode`,
  `resetCamera`, ..., `m_nState = 1`) and then continues with the case-70
  state-1/2 blocks.  Ours `0x139944..0x139a64` contains *only* the state-0
  body; the state-1/2 blocks are placed elsewhere.  Same code, different
  position.
* **case 125, `dSpan -856`.**  Orig `0x139f9c..0x139ffe` is only the
  `Route`-creation block (`new Route`, `setPlayerRoute`, `setRoute`, timer
  reset) and ends `b.w 0x13b518`.  Ours `0x139d98..0x13a152` continues with
  the case-125 state-1/2 blocks inline.  Orig's continuation is at
  `0x13b43e`/`0x13b518` (after the case-92 entry).  Same code, different
  position.
* **case 102/154/64** are the same story.

So a raw entry-span table **cannot** localize a *size* difference: it
localizes *placement* differences.

## Localizing the case-157 tail (0x6ae)

The case-157 state-5 strong arm is at orig `0x144a14`, ours `0x144366`
(`d = +0x6ae`).  Both lie inside the case-16 span (the cold region):

```
                        orig        ours      d
case-16 entry          13b014      13afc8    +76
state-5 weak arm       141b7a      1418fc   +638
state-5 strong arm     144a14      144366  +1710
post-switch tail       144e36      144e8a    -84
```

The cumulative delta grows from `+76` at the cold-region start to `+1710` at
the strong arm, then falls back to `-84` at the tail.  The cold region is
`40482` B in orig and `40642` B in ours (`-160` total), so the `+1710` is a
pure **reordering** of cold blocks, not extra code.

The block immediately *before* the strong arm is different:

* orig `0x144960..0x144a12` = the case-157 state-4 completion block
  (`resetGunDelay`, `setLookAtCam`, `setFreeze`, `setSpeed`,
  `setComputerControlled`, `setVulnerable`, `setPosition`, `resetCamera`,
  `forceUpdate`, `if (field_0xaa) setAutoTurret(true)`, `m_nState++`).
* ours `0x1442b0..0x144362` = a different cold block
  (`TargetFollowCamera::translate`, `Level::getLandmarks`, ...).

So the upstream "block whose placement differs" is the **state-4 completion
block**: the original places it immediately before the state-5 strong arm;
ours places it elsewhere in the cold region.  This is exactly the kind of
reordering the wave-146 `!prof` finding predicts (the original was built with
branch weights, so the backend honours IR order rather than the no-profile
placement heuristic).

## Two real source differences found while localizing (not landed)

1. **The state-5 strong arm has an extra `m_bCinematicBreak = 1` in ours.**
   Orig `0x144a14`:
   ```
   144a14 ldr  r6, [sp, #96]
   144a16 movs r5, #11
   144a18 movs r4, #0
   144a1a b.n  144a2a
   ```
   Ours `0x144366`:
   ```
   144366 ldr.w r8, [sp, #88]
   14436a movs  r0, #1
   14436c movs  r5, #11
   14436e strb.w r0, [r8, #17]   ; m_bCinematicBreak = 1
   144372 b.n   144384
   ```
   The source's `} else { m_bCinematicBreak = 1; for (i=11..22) ... }` therefore
   does **not** match the original's strong arm.  (It is only 8 B, so it is
   not the 0x6ae cause, but it is a genuine divergence.)

2. **The `autoTurret` PLT anchor is still present and duplicates a pair.**
   Orig contains exactly one `autoTurretIsEnabled`/`setAutoTurret` pair
   (`0x13a7fc`/`0x13a80c`).  Ours contains two: `0x13a956`/`0x13a95e` (the
   line-1812 PLT anchor) and `0x13c6da`/`0x13c6ec`.  The wave-141 note already
   documents that the original does not contain line 1812; the anchor is a
   deliberate `.rel.plt`-order crutch and is worth `-16` B if the real
   mission-154 block is moved to its original position (`0x13a7fc`).

Neither difference is worth 0x6ae; both are downstream of the same cold-block
placement problem.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9303/table.py        # per-case entry table above
```

`table.py` (self-contained; paths are the w9303 worktree):

```python
import subprocess,re
OBJ='/home/debian/GalaxyOnFire2/wt/w9303/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS='/home/debian/GalaxyOnFire2/wt/w9303/cmake-build-match/libgof2hdaa.so'
NAME='_ZN11LevelScript7processEi'
def sym(path,name):
    out=subprocess.check_output([OBJ,'-T',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name: return int(p[0],16), int(p[4],16)
def disasm(path,addr,size):
    out=subprocess.check_output([OBJ,'-d','--start-address=%d'%addr,'--stop-address=%d'%(addr+size),path],text=True)
    ins=[]
    for l in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+((?:[0-9a-f]{2,8}\s*)+)\s+(.*)$',l)
        if m: ins.append((int(m.group(1),16),m.group(3).strip()))
    return ins
def parse(path):
    a,sz=sym(path,NAME); ins=disasm(path,a,sz); amap={x:t for x,t in ins}; cases={}
    dend=None
    for i,(ad,t) in enumerate(ins):
        if re.match(r'cmp\s+r0,\s*#158',t):
            for j in range(i+1,min(i+3,len(ins))):
                if re.match(r'bne(?:\.w)?\s+[0-9a-f]+',ins[j][1]): dend=ins[j][0]; break
        if dend: break
    dstart=dend-0x200
    for i,(ad,t) in enumerate(ins):
        if ad<dstart or ad>dend: continue
        m=re.match(r'cmp\s+r0,\s*#(\d+)',t)
        if m:
            for j in range(i+1,min(i+3,len(ins))):
                mm=re.match(r'beq(?:\.w)?\s+([0-9a-f]+)',ins[j][1])
                if mm:
                    v=int(mm.group(1),16)
                    if a<=v<a+sz: cases[int(m.group(1))]=v
                    break
    for i,(ad,t) in enumerate(ins):
        if ad<dstart or ad>dend: continue
        m=re.match(r'sub(?:\.w)?\s+r1,\s*r0,\s*#(\d+)',t)
        if not m: continue
        base=int(m.group(1)); tgt=None
        for j in range(i+1,min(i+4,len(ins))):
            mm=re.match(r'bls(?:\.w)?\s+([0-9a-f]+)',ins[j][1])
            if mm: tgt=int(mm.group(1),16); break
        if not tgt or tgt not in amap or not amap[tgt].startswith('tbh'): continue
        pc=tgt+4
        txt=subprocess.check_output([OBJ,'-h',path],text=True)
        for l in txt.splitlines():
            p=l.split()
            if p and p[1:2]==['.text']: vma=int(p[3],16); off=int(p[5],16)
        raw=open(path,'rb').read(); fo=off+(pc-vma)
        for k in range(64):
            val=int.from_bytes(raw[fo+2*k:fo+2*k+2],'little'); t2=pc+2*val
            if not (a<=t2<a+sz): break
            cases[base+k]=t2
    return a,sz,cases
oa,osz,oc=parse(OURS); ba,bsz,bc=parse(ORIG)
# add 158 fallthrough
oc[158]=0x138862; bc[158]=0x138862
keys=sorted(set(oc)&set(bc), key=lambda k: bc[k])
print('%-5s %8s %8s %7s %7s %7s %8s'%('case','orig','ours','dEntry','spanO','spanU','dSpan'))
for i,k in enumerate(keys):
    o=bc[k]; u=oc[k]
    no=keys[i+1] if i+1<len(keys) else None
    so=(bc[no]-o) if no else (ba+bsz-o)
    su=(oc[no]-u) if no else (oa+osz-u)
    print('%-5d %8x %8x %+7d %7d %7d %+8d'%(k,o,u,o-u,so,su,so-su))
```

## Open questions

1. The case-157 strong arm is placed `0x6ae` later in orig because the
   state-4 completion block is placed before it.  Find the `__builtin_expect`
   (branch-weight) shape that makes LLVM place the state-4 completion block
   immediately before the state-5 strong arm.  The `!prof` finding (w9214)
   says this is a branch-weight/IR-order issue, not a source-size issue.
2. The strong arm's `m_bCinematicBreak = 1` (8 B) is not in the original.
   Removing it changes the function size by `-8` unless a companion is found;
   it should be re-tested once the cold-block order is fixed.
3. The `autoTurret` PLT anchor (16 B) can be dropped once the mission-154
   state-0 block sits at the original `0x13a7fc`; that is the same
   cold-block-placement problem.
