# SpaceLounge::drawLounge — pad-split / greedy-RA cascade re-examined (w4818, wave 98)

Status: research note (2026-11-04). Worktree `wt/w4818`, branch `wave98-w4818`,
base `fe4f5e7cf`. **No `src/` change landed**: every source shape tested this
wave keeps the official score at 95.5 (object oracle 95.3). The tree is clean.

Gate (`cmake --build cmake-build-match --target check -j8`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.5`, `osize = usize = 1948`,
`byte = linked = false` (from `report.json`). Object-level oracle
(`/dev/shm/w4818/score.py`, official compile command, `autojunk=False`) reports
`95.3`, size 1948.

Prior work: `spacelounge-drawlounge-w4519.md` (newest), `-w4246.md`,
`-w4227.md`, `-w4118.md`, `-w4018.md`, `-w3838.md`, `-w3818.md`,
`SpaceLounge-OnRender3D.md`, `SpaceLounge-layout.md`. This note records the new
evidence gathered in wave 98 and supersedes nothing.

## 1. Harness correction: the instrumented-clang knobs take raw vreg numbers

The w4519 note quotes `HINTVREG=pad:r11`, `PRIOVREG=pad`, `HINTVREG=pad:r9`,
etc. The instrumented clang in `/tmp/llvm_android_702b/build2` parses both
fields with `strtoul`, so `pad` parses as 0 and the printed `%2` parses as 2;
neither matches. The raw vreg of pad is `0x80000002 = 2147483650`
(`printReg` shows it as `%2`). With the raw number the knobs fire:

```text
HINTVREG=2147483650:9  -> ENQ %2 size=6400 prio=0xE0001900 stage=1
PRIOVREG=2147483650    -> ENQ %2 size=6400 prio=0xE0001900 stage=1
(base)                 -> ENQ %2 size=6400 prio=0xA0001900 stage=1
```

(`VRW` already used the raw number in w4519, e.g. `2147483650:1.45`; the
`HINTVREG`/`PRIOVREG` rows of the w4519 table should be re-read with this in
mind — `HINTVREG=pad:r9` was a no-op.)

## 2. A physical hint on pad does not reproduce the target

`HINTVREG=2147483650:<any>` sets bit 30, so pad (size 6400) is selected before
x (size 4220). But pad then lands in **r6**, the body grows to **1980 B**, and
the object score drops to **76.4**. The hinted register value is irrelevant
(r3/r5/r9/r11 all give the same result) because the hinted register is
occupied at that point and the normal allocation order takes over. So "give pad
a copy hint" is not the answer.

## 3. VRW sweep: pad→r9 alone is not sufficient

Sweeping `VRW=2147483650:<f>` on the current flat source (object oracle):

```text
f              size  pct   pad  x    lh  top
1.000          1948  95.3  fp   r9   fp  r2   <- current build
1.05 - 1.35    1948  94.7  fp   r9   r8  r1
1.375 - 2.175  1948  95.7  r9   r5   r8  r1   <- closest
2.20 - 2.275   1940  91.2  r6   r5   r8  r1
>= 2.30        1952  79.9  fp   r5   r8  r1
```

The best (95.7) has pad→r9 and x→r5, but lineHeight goes to r8 and top to r1
(the original wants lineHeight fp and top r2→r8). So pad→r9 is necessary but
not sufficient; the rest of the cascade still differs. The `VRW=1.45` diff is
the flat 4-instruction clamp arm plus the top/lineHeight swap and an early
`str.w r8, [sp,#12]` agent spill.

On the faithful (`pad2`) source the same sweep gives size **1944** (4 bytes
short) and at most **93.2**, so it cannot be landed.

## 4. Why pad is split in the current build (RA trace)

From `/dev/shm/w4818/trace_dl_base.txt` (`RA_TRACE=1`):

- pad `%2` is a global interval, size 6400, weight `2.154168e-04`, **no COPY in
  MIR** (7 ALU/load uses only), priority `0xA0001900`.
- x `%609` is the coalesced phi (the `%43 = COPY %609` at the join is gone from
  the RA trace — `%43` is never selected), size 4220, weight `2.946139e-04`,
  hint `$r3`, priority `0xE0000000+4220`.
- Selection order among the clamp values: x→r5 (hint r3 is occupied), boxW
  `%47`→r6, top-pad `%46`→r5 (evicts x: r5 is cost 0, the free r9 is cost 1),
  x re-enqueued→r9, x-pad `%45`→r6, boxW→r10, Canvas `%365`→r11, then pad.
- At pad's selection r9 holds x (2.95e-4) and r11 holds Canvas (2.17e-4); pad
  (2.15e-4) can evict neither, so it is split (stage 2). The split copy lands
  in r5/fp, which lets pad share fp with lineHeight (their intervals no longer
  overlap after the split).

The original keeps pad whole in r9 and lineHeight in fp. That means pad's
interval must extend past lineHeight's def (`0x1712e6`) and pad must be
selected with r9 free or evictable; then x is forced to split `r5 -> r3 -> r9`
and lineHeight takes fp. The current build does the transpose: pad is split
(fp + r5) and x stays whole in r9.

## 5. Source shapes tried this wave (all <= 95.3 object)

```text
variant                                             pct   size  pad/x/lh
pad2 local in top+boxW, flat clamp (`pad2const`)     95.3  1948  fp/r9/fp
  (built and verified officially: 95.5, same as base)
faithful (pad2 in top+boxW+right arm)               93.8  1948  fp/r9/fp
3-way `else { x = centerX; }` phi, flat             95.3  1948  fp/r9/fp
3-way phi, faithful                                 93.8  1948  fp/r9/fp
`const int pad`                                     95.3  1948  fp/r9/fp
right arm `-(width+pad+pad)` / `+(-(width+pad+pad))` 95.2  1948  fp/r9/fp
anchor-free `if (agent->isKnown())`                 91.8  1952  r5/r9/fp
box locals xyw                                      95.3  1948  fp/r9/fp
box locals wxy / xwy (faithful)                     71.6/71.1 1968/1976 r9/r5/r8
box locals yxw                                      94.5  1948  r5/r9/fp
```

The anchor-free form is the interesting one: it gives **pad=r5, x=r9** (the
transpose of the target) but grows to 1952 B via an extra `mov r9, r5` in the
width block. The `__builtin_expect(agent->isKnown(), 1)` anchor is still
required for the 1948-byte body.

## 6. Conclusion

The current flat source remains the best (95.3 object / 95.5 official). The
residual is a greedy-RA cascade: the original keeps pad whole in r9, which
forces x to split `r5 -> r3 -> r9` and lets lineHeight take fp; the current
build splits pad instead (fp + r5) and keeps x whole in r9. No source shape
tested this wave reaches the original cascade without changing the body size.

## Reproduction

```bash
# object oracle (absolute path required)
python3 /dev/shm/w4818/score.py \
  /home/debian/GalaxyOnFire2/wt/w4818/src/game/menu/SpaceLounge.cpp
# RA trace / knobs (instrumented LLVM 7, same revision as NDK r18b)
python3 /dev/shm/w4818/cctrace.py /dev/shm/w4818/block_base.txt   # RA_TRACE=1
HINTVREG=2147483650:9 python3 /dev/shm/w4818/ccenv.py /dev/shm/w4818/block_base.txt
VRW=2147483650:1.45  python3 /dev/shm/w4818/ccenv.py /dev/shm/w4818/block_base.txt
# official
cmake --build cmake-build-match --target check -j8
```

Scratch: `/dev/shm/w4818/` (`block_base.txt`, `block_ghidra.txt`,
`trace_dl_base.txt`, `trace_hint_pad9.txt`, `vsw2.py`, `vsw3.py`,
`trybox.py`, `cf_*.txt`, `cur_show.txt`, `pad2const_show.txt`).
