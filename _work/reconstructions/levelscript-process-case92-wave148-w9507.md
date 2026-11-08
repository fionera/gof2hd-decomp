# LevelScript::process — wave 148 / w9507: case-92 state-1 block,
# the faithful 3-call `getPosition()` shape re-measured (block-local 0.7165)
# and the prologue accessor-order live-range lead

Status: **analysis only, no code change landed** (source pristine at wave-148
HEAD `fece77a64`).  Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Scratch in
`/dev/shm/w9507/` (not tracked): `fdiff.py`, `align_diff.py`, `block_align.py`,
`measure.sh`, `LevelScript.base.cpp`.

## Result

```
                         HEAD (wave 148)   3-call variant   prologue reorder   prologue+3call
process raw diffbytes    54,898            55,219 (+321)    55,251 (+353)      55,301 (+403)
process aligned insns    15,802            15,797 (-5)      15,376 (-426)      15,358 (-444)
process symbol size      57,232            57,240 (+8)      57,304 (+72)       57,312 (+80)
block-local ratio        0.6782            0.7165           0.7165*            0.7165
block local eq/insns     177/261           187/261          187/261*           187/261
byte_exact               4507              2741 (not landed) -                  -
linked_exact             4507              4507             -                  -
size-exact               4524/4524         4523/4524        -                  -
data_exact               843/843           843/843          -                  -
imports                  207               207              -                  -
lint                     CLEAN ✓           CLEAN ✓          -                  -
RATCHET                  PASS              FAIL (+8 size)   -                  -
```

`*` prologue-reorder block ratio measured on the prologue+3call build; the
prologue reorder alone keeps the same block code, only the address shifts.

The block metric is the w9320/w9406 metric: difflib over the block instruction
streams after stripping `;` comments and `<...>` symbol annotations and
normalising every hex token of >=4 digits (branch targets, PC-relative
addresses) to `T`.  It is **not** the per-immediate normalisation; using that
gives an inflated 0.8506 and is wrong.  Ranges are `[start, start+size)`;
`0x294` (orig) / `0x284` (ours) include the final `b.w`.

## The faithful 3-call shape (re-confirmed, not landed)

The original calls `(*enemies)[3]->getPosition()` **three times**, once per
`setPosition` (calls at `0x13a06e`, `0x13a0ac`, `0x13a0e2`).  The landed
wave-148 source keeps a `Vector ref` for the first/third call and inlines the
second, to hold 57,232 B.  The faithful source is:

```cpp
(*enemies)[0]->setPosition((*enemies)[3]->getPosition() + Vector{-76000.0f, 5000.0f, 5000.0f});
(*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
(*enemies)[2]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 2500.0f});
```

Measured on top of the landed store-order swap:

* raw same-address diff: **55,219** (vs 54,898 landed)
* symbol size: **57,240 B (+8)** — the third virtual `getPosition()` sequence
  costs 8 B in this block
* block-local instruction alignment: **0.7165 (187/261)** vs 0.6782 (177/261),
  i.e. **+10 matching instructions** in the 261-instruction block
* global `align_diff.py`: **15,797** vs 15,802 (-5)

This reproduces the w9406 measurement exactly (the note's 0.7165 / 187).  The
earlier 0.8506 in this session was a normalisation bug (it normalised
immediates too); the correct metric is 0.7165.

### The block is structurally complete

A chunk-by-chunk difflib of the 3-call block against the original shows **no
remaining statement differences**.  All 28 calls are present and in the same
order.  The only non-matching chunks are:

1. **Prologue (6 B).**  Original `13a036 ldr r0,[sp,#92]; 13a038 ldr r0,[r0,#0]`
   reloads the `status` pointer from the stack; ours `13a1bc ldr r0,[r4,#0]`
   keeps it in `r4`.  This is the same `this`/`status` live-range difference
   documented in wave-121 w7104.
2. **Group 2 (10 B).**  Original spills the `operator+` pointer to `sp+80`
   (`13a080 str r3,[sp,#80]`) and reloads it (`13a0c0 ldr.w r8,[sp,#80]`)
   because it lives in caller-saved `r3`; ours keeps it in callee-saved `sl`
   and never spills.  Again pure register pressure.

So the block is 16 B shorter than the original (640 vs 656 B excluding `b.w`)
purely because our build has lower register pressure, and the rest of the
function is correspondingly +24 B.  This is the same whole-function live-range
gap as wave-121 w7104.

## The -8 companion is still the case-125 dispatch

The dispatch still shows the 6 B of recoverable code identified by w9406:

* original: `13a024 e8df f010 tbh [pc, r0, lsl #1]`, table at `0x13a028`,
  state-1 block immediately at `0x13a036`.
* ours: `13a176 f20f 010c addw r1, pc, #12`, `13a17a e8d1 f010 tbh [r1,...]`,
  `13a17e bf00 nop`, table at `0x13a184`, 40 B of case-125 literal pool at
  `0x13a194`, state-1 block at `0x13a1bc`.

The `addw`+`nop` (10 B vs 4 B) exists because the case-125 literal pool sits
between the `tbh` table and the state-1 block.  In the original the case-125
literal pool is at `0x13a2cc` (after the case-92 block), because the original
outlines the case-125 state-1/2 blocks to `0x13b43e`/`0x13b518` (w9303).  Fixing
the case-125 placement should let clang emit `tbh [pc, ...]` and recover 6 B;
the remaining 2 B are table/pool alignment.  That is the expected -8 companion
for the 3-call shape.  **Do not land the 3-call shape alone** — it is +8 B and
drops `byte_exact` to 2741.

## NEW lead: the prologue accessor order is a -426 aligned win

The original prologue calls `getMessages`, `getPlayer`, `getEnemies`; the
current source calls `getEnemies`, `getPlayer`, `getMessages`.  Reordering the
three locals to the faithful order:

```diff
-    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    PlayerEgo *player = m_pLevel->getPlayer();
-    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    PlayerEgo *player = m_pLevel->getPlayer();
+    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
```

makes the prologue byte-identical to the original (`getMessages` -> `r5`,
`getPlayer` -> `[sp+80]`, `getEnemies` -> `r8`, `this` in `r4`), and the
**global `align_diff.py` total drops 15,802 -> 15,376 (-426)**.  This is the
largest structural improvement found this wave.  It is not landable alone
because it is **+72 B** (57,304): the faithful order makes `messages` spill to
`[sp+76]` at the mission-1 use, which is exactly the +72 B wave-121 w7104
documented.  Combining it with the 3-call shape gives 57,312 B / aligned
15,358 / block 0.7165.

Wave-121 concluded the faithful order is "necessary but not sufficient" and
that matching it requires making `sl` the preferred scratch register (the
original spills `this` to `[sp+88]` and reloads it 328 times; ours keeps `this`
in `sl` and uses `r9` for `status`).  The +72 B is the size cost of that
live-range mismatch, not a separate bug.  The next wave should pair the
faithful prologue with a `sl` live-range anchor (the `Vector` temporaries
around `0x1394ce`/`0x13a054` are the best first target) and then re-check
whether the function returns to 57,232 B.

## Other hypotheses tried (all worse)

* `KIPlayer *leader = (*enemies)[3];` + three `leader->getPosition()` calls:
  **+32 B** (57,264), block 0.7088 (185/261) — the saved `data_[3]` loads cost
  more than the third `getPosition()`.
* `Vector ref` + inline second + inline third: **+16 B** (57,248), block
  0.7088 (185/261).
* `__builtin_expect(Globals::status->field_178 == 0, 1)` on the block guard
  (3-call): size unchanged (+8), raw +1 (55,220), aligned back to 15,802,
  block 0.7165 — no help.
* `__builtin_expect(Globals::status->field_178 == 0, 0)`: **+24 B** (57,256),
  aligned 19,205, block 0.2490 — much worse.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9507/fdiff.py _ZN11LevelScript7processEi          # 54,898 / 57,232
python3 /dev/shm/w9507/align_diff.py _ZN11LevelScript7processEi     # 15,802
python3 /dev/shm/w9507/block_align.py 0x13a036 0x294 0x13a1bc 0x27c  # 0.6782
```

`block_align.py` (self-contained):

```python
import subprocess, sys, re, difflib
OBJ='/home/debian/GalaxyOnFire2/wt/w9507/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS='/home/debian/GalaxyOnFire2/wt/w9507/cmake-build-match/libgof2hdaa.so'
def disasm(path,addr,size):
    out=subprocess.check_output([OBJ,'-d','--start-address=%d'%addr,'--stop-address=%d'%(addr+size),path],text=True)
    ins=[]
    for l in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$',l)
        if not m: continue
        a=int(m.group(1),16); t=m.group(2).strip()
        if t: ins.append((a,t))
    return ins
def norm(t):
    t=t.split(';')[0]
    t=re.sub(r'<[^>]*>','',t)
    t=re.sub(r'\b[0-9a-f]{4,8}\b','T',t)
    return ' '.join(t.split())
def run(oa,osz,ua,usz):
    o=[norm(t) for a,t in disasm(ORIG,oa,osz)]
    u=[norm(t) for a,t in disasm(OURS,ua,usz)]
    sm=difflib.SequenceMatcher(None,o,u,autojunk=False)
    eq=sum(b.size for b in sm.get_matching_blocks())
    print('orig insns',len(o),'ours insns',len(u),'eq',eq,'ratio %.4f'%(eq/max(len(o),len(u))))
run(int(sys.argv[1],16),int(sys.argv[2],16),int(sys.argv[3],16),int(sys.argv[4],16))
```
