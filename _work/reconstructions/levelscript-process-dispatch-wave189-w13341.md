# LevelScript::process — wave 189 / w13341: state-5 sound-call order (raw 51874 -> 51873)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13341`, branch
`wave189-w13341`, base master `30e0a4013`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13341`.

Task: state-machine dispatch / switch-layout pass.  The dispatch skeleton was
re-derived from the original Thumb disassembly, all six `else if` chain orders
of the state-4 / state-5 / state-3 arms were measured, and the state-5 body was
re-ordered against the original call sequence.  One size-exact,
`.rel.plt`-identical raw win landed.

```
                         base (HEAD)      this pass
process raw diffbytes    51,874           51,873     (-1)
process aligned insns    21,005           20,997     (-8)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,982           73,981
.ARM.extab diffbytes     615              615        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
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

## 1. Landed change

State-5 body (`case 5` docking sounds), source lines 720-721 of the base:

```c++
-  Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);
-  Globals::sound->play(158, nullptr, nullptr, 0.0f);
+  Globals::sound->play(158, nullptr, nullptr, 0.0f);
+  Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);
```

Measured alone on the wave-189 base: raw **51,874 -> 51,873** (-1), size
57,232/57,232 exact, aligned 21,005 -> 20,997, `.rel.plt` unchanged.

This is faithful: the original state-5 body (`0x139010..0x139078`) issues
`FModSound::play(157)` (`0x139028`), `updateEvent3DAttributes(157)` (`0x139040`),
`FModSound::play(158)` (`0x13904e`), `updateEvent3DAttributes(158)` (`0x139060`),
`stop(player->field_0x1c)` (`0x139068`), `play(161)` (`0x139078`).  The base had
the `update(158)` / `play(158)` pair reversed.  The two calls are independent
(`play` does not read the zero vector; `update` does not read playback state),
so the swap is semantics-preserving.

Re-ordering the whole state-5 sound block to the original sequence
(`play157, update157, play158, update158, stop, play161`) measures raw
**51,878** (+5 vs base), so only the single pair swap is kept.

## 2. Original dispatch skeleton (ground truth, re-derived)

Mission-0 dispatch, original `0x1389c8..0x139b22`:

```
1389c8  cmp r1,#4 ; beq.w 138d2a        (state 4 body)
1389ce  cmp r1,#3 ; bne.w 138fd8        (state 5 check)
1389d4  <state 3 body>                   ... b.w 137f7c (tail)
138d2a  <state 4 body>                   ... beq.w 138fd4 on !isOver(msg10)
138df4  movs r0,#5 ; b.w 13d4ec          (state=5 shared store -> tail)
138fd4  ldr.w r1,[sl,#28]                (reload m_nState)
138fd8  cmp r1,#5 ; bne.w 139b1a         (switch)
138fde  <isOver(messages[12])> ; beq.w 139b16
139b16  ldr.w r1,[sl,#28]                (reload after state-5 body)
139b1a  subs r0,r1,#6 ; cmp r0,#9 ; bhi.w 13f906
139b22  tbh [pc,r0,lsl #1]               10 entries -> states 6..15
13f906  cmp r1,#16 ; bne.w 137f7c        (state 16 = switch default)
```

The original source is therefore the natural `else if` chain **4, 3, 5** with
the `switch (m_nState)` after it; the state-4 true arm constant-propagates
`m_nState = 5` and jumps straight to the tail, while its `!isOver` arm jumps
directly to the state-5 check (state 3 is known false).  Our base instead
compiles the chain as **5, 4, 3-folded-into-switch**: `cmp r1,#5; beq`,
`cmp r1,#4; bne`, then a **13-entry** `subs r0,r1,#3; cmp r0,#12; tbh`
covering states 3 and 6..15.

## 3. Chain-order sweep (all six orders, current base)

Each variant keeps the three arm bodies byte-for-byte and only permutes their
order in the `else if` chain.  Build+link with the exact NDK r18b `-Oz` flags.

| chain order | raw | aligned | size | .rel.plt |
|---|---:|---:|---|---|
| **4,5,3 (base)** | **51,874** | 21,005 | 57,232 | identical |
| 4,3,5 | 55,046 | 21,365 | 57,240 (+8) | DIFF |
| 5,4,3 | 55,437 | 24,710 | 57,268 (+36) | DIFF |
| 5,3,4 | 55,437 | 24,710 | 57,268 (+36) | DIFF |
| 3,4,5 | 55,120 | 21,505 | 57,248 (+16) | DIFF |
| 3,5,4 | 53,162 | 21,032 | 57,232 | DIFF |

`4,3,5` is the only order that reproduces the original 10-entry `tbh`
(`subs r0,r1,#6; cmp r0,#9`) and a separate state-3 compare, but the compiler
then rotates the chain to a `3,5,4` layout, grows the frame/body by 8 B and
rotates five `.rel.plt` entries.  It is **not** a raw win.  `3,5,4` is
size-exact but raw +1,288 and `.rel.plt` differs.  The base `4,5,3` order is
the best raw and the only chain order that keeps `.rel.plt` byte-identical.

## 4. Adjacent-statement sweep

A fresh adjacent single-statement swap sweep over the whole mission-0
dispatch region (source lines 679-760: state-4, state-5 and state-3 arms,
~70 candidates, exact compile+link, 1.8 s/candidate) found exactly one raw
win (the landed pair above).  All other adjacent swaps are neutral or worse.

## 5. Conclusion

The remaining dispatch residual is codegen, not source shape: the base's
`5,4,3` chain order and 13-entry `tbh` are what clang 7 `-Oz` produces from
this source, and every source permutation that recovers the original
`4,3,5`/10-entry skeleton costs size and `.rel.plt` identity.  The rest of the
residual is the known `this`=fp vs `this`=sl prologue register-map shift and
per-body scheduling.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13341
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51873 aligned= 20997 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses: `measure.sh` (build + raw + rel.plt md5),
`fastscore.py` (function-byte diff), `adj_sweep.py` (adjacent swap sweep),
`orders.py` (six chain orders).
