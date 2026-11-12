# SpaceLounge::drawLounge — wave-190 RA lever sweep (w13397)

Status: research note (2026-11-12). Worktree `wt/w13397`, branch `wave190-w13397`,
base `0c9af5cea`. **No `src/` change landed**: every size-exact shape tested keeps
the committed raw/aligned score. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **175**, aligned **41**, size **1948/1948**, insn **741/741**
* after: unchanged (note only)
* gate re-run at the end: `RATCHET PASS` — byte 4507 / linked 4507, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN

Prior work: `spacelounge-drawlounge-wave176-w12018.md` (and the notes it cites),
`status-generator-spacelounge-wave155-w10110.md`. This note supersedes nothing.

## 1. Residual is pure RA (re-confirmed)

`tools/fn_diff.py` gives 175/41/1948/741. The 25 differing runs are all in the
lounge (`mode == 0`) half; the conversation half is byte-identical. The single
root cause is the clamp colouring:

```
value        original        HEAD
pad          r9              fp (r11) + copy in r5
x            r5 -> r9        r9 (whole)
top-pad      r5              r5
lineHeight   r11             r11
width        r6              r6
top          r8              r8
```

The original's `x` is split at the drawBox argument setup (r5 early, `mov r9, r3`
late); HEAD's `x` is one interval in r9 and `pad` is split into fp + r5.

## 2. Correct instrumented-compiler register numbering (fixes a wave-176 error)

The patched clang at `/tmp/llvm_android_702b/build2` prints physical registers
with ARM enum numbers offset by 66:

```
r0=66 r1=67 r2=68 r3=69 r4=70 r5=71 r6=72 r7=73
r8=74 r9=75 r10=76 r11=77 r12=78
```

So the wave-176 note's "HINTVREG=pad:75 (hint pad→r11)" is actually **pad→r9**,
and "HINTVREG=pad:75, x:71" is **pad→r9, x→r5** (243/57, size 1948). All earlier
`HINTVREG`/`FORCEVREG` conclusions that named r11 for 75 are off by two.

## 3. New sweeps (all neutral or worse than 175/41)

Object-level oracle (`score.sh`), official compile command. `run_inst.sh` drives
the instrumented compiler.

### 3.1 Weight / priority knobs (instrumented)

| knob | result |
|---|---|
| `XWFACTOR=1.5` / `2.0` | 203/62 size 1948 (insn 740) |
| `XWFACTOR>=2.2` | 1762/190 size 1952 |
| `VRW=x:2.2..3.0` | 1856/165 size 1972 |
| `VRW=x:2.2, top-pad:0.5` | 1852/166 size 1968 |
| `VRW=top-pad:0.4` | 1859/165 size 1972 |
| `NOHINTCO=x` | 1865/241 size 1968 |
| `NOHINTCO=top-pad` | 1843/154 size 1964 |
| `NOHINTCO=x, PRIOVREG=lineHeight` | 1866/286 size 1968 |
| `NOHINTCO=x, PRIOVREG=lineHeight,pad,pad2` | 311/91 size 1948 |
| `NOHINTCO=x, PRIOVREG=lineHeight,pad,pad2,width,agent` | 310/91 size 1948 |
| `HINTVREG=pad:r9` | 1883/209 size 1980 |
| `HINTVREG=pad:r9,pad2:r10` | 188/57 size 1948 |
| `HINTVREG=pad:r9,pad2:r10,lineHeight:r11,width:r6,agent:r8` | 194/69 size 1948 |
| `HINTVREG=pad:r9,x:r5` | 243/57 size 1948 |
| `PRIOVREG=pad` | 1884/225 size 1980 |
| `PRIOVREG=pad HINTVREG=pad:r9,x:r5` | 358/77 size 1948 |
| `NOEVICT=1` | 1905/239 size 1996 |

`HINTVREG=pad:r9,pad2:r10` reproduces the target *pad=r9, x=r5* colouring, but
`x` then lands in r11 instead of splitting, and `pad` is later rematerialised, so
the score is 188/57 (worse than 175/41). The original needs every callee-saved
register occupied when `x` is re-enqueued so greedy's split stage produces the
r5/r9 split; the natural priority order (x is hinted, `0xE0000000+4220`) always
selects `x` before `pad`/`pad2`/`lineHeight`, so a free register always exists.

### 3.2 Source shapes

* `int pad2 = pad + pad;` used in top / right arm / box width: 445/54 (same as
  the faithful `-(width + pad*2)` form).
* `int boxW = pad * 2 + width;` before the clamp, used in the right arm and box
  calls: 517/93 size 1940 (gate-fatal).
* `pad` fully inlined as `Globals::layout->field_0x94` (all / top+clamp / box
  only): 1840/209, 1890/212, 1846/180 (sizes 1956/1984/1956).
* `x` as `unsigned` 176/42; `unsigned pad` 177/43; both unsigned 177/43;
  `x` as `long`/`int32_t`/`register` neutral; `x volatile` 1874/445 size 1972.
* `const Vector` topLeft/bottomRight neutral; `Vector&` 269/79.
* `Agent *const` / `__restrict` neutral.
* `int textX = x;` (before/after lineHeight) neutral (copy coalesced).
* `int textTop = top;` before DrawString 1550/188 size 1964.
* `int textY = top + field_0x2c0;` before DrawString 1552/217 size 1972.
* width-add as ternary / plain `if` 550/76 size 1952; expect(0) 1879/205 size 1980.
* `if (centerX ...) { ... } int x = centerX;` neutral.
* `int x = (int)(...)` inlined (no centerX) 1133/117 size 1948.
* name/role as statement-if chains: 1679/200 size 1992, 1740/219 size 2028.
* `Mission *mission = agent->getMission();` cache 1607/176; `int offer` cache
  1098/385 size 1940.
* `String sp("  ")` local 1327/259 size 1936.
* `__builtin_expect` on name `isKnown` (1/0) 1574/266 size 1960, 391/146 size
  1948; on `isStoryAgent` 1585/224 size 1956; on final `isKnown` (1/0)
  233/75 size 1948, 1145/173 size 1952; on `mode==0` value 0 1659/449 size 1944.
* `-mllvm -regalloc-csr-first-time-cost` on HEAD: 0..26 → 175/41, 27+ → 293/61.
* branch weights: L=7,U=1 optimal; L=3,U=1 and L=6,U=2 also 175/41; everything
  else 908/133 or worse.
* MIR/scheduler flags (`-enable-misched=false`, `-enable-post-misched=false`,
  `-misched-postra=false`, `-tail-dup-size=0`, `-tail-dup-placement-threshold=0`,
  `-enable-early-taildup=false`, `-enable-copy-propagation=false`,
  `-lcr-max-depth`, `-lcr-max-interf`, `-enable-local-reassign=true`,
  `-regalloc=basic/fast/pbqp`) all neutral or worse.

## 4. Why the faithful right arm scores 445, not a win

The faithful `x = Globals::w - inset - (width + pad * 2)` does produce the
original's 2-instruction right arm (`itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1`),
but it materialises `mov r5, fp` in **both** arms instead of the single
`mov r9, r3` at the join. Net body size is still 1948, but the 2-byte shift moves
the epilogue and every downstream branch target, so the literal-pool branch
offsets and the unwind cleanup branches all differ: 39 runs / 445 bytes vs
25 runs / 175 bytes. The faithful form is *structurally* correct but not a raw
win under the byte-diff metric.

The faithful form's RA is otherwise identical to HEAD (x%607: r5 → evicted by
top-pad → r9; pad%2 rematerialised to fp), confirmed with `RA_TRACE`.

## 5. Conclusion

The committed flat clamp remains the best size-exact shape: **raw 175 /
aligned 41 / 1948 B / 741 insns**. The residual is the greedy-RA fact that
`x` (hinted, prio `0xE0000000+4220`) is selected before `pad` (`0xA0000000+6400`),
so `x` takes r5, `top-pad` evicts it to r9, and `pad` lands in fp. Every source
shape found that forces `pad=r9` either changes the value graph (faithful arm,
445) or leaves `x` in r11 instead of the r5/r9 split (188–243). No size-exact
raw win was found.

## 6. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
# object-level oracle
/home/debian/GalaxyOnFire2/scratch/w13397/score.sh
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13397` (`score.sh`, `score_bw.sh`,
`batch.py`, `b1.py`..`b53.py`, `run_inst.sh`, `showdiff.py`, `SpaceLounge.cpp.base`).
The instrumented clang is `/tmp/llvm_android_702b/build2/bin/clang++`; its
`HINTVREG`/`PRIOVREG`/`VRW`/`NOEVICT`/`XWFACTOR`/`FORCEVREG` knobs and the
`RA_TRACE` env are documented above (register numbers offset by 66).
