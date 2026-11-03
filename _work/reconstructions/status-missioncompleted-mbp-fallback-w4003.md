# `Status::missionCompleted` — MBP fallback and the two-switch source shape (w4003)

Status: research note (no source change landed). Worktree `wt/w4003`, branch
`wave90-w4003`, base master `360c7467`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

Topic: the faithful direct `switch ((short) m->getType())` is **57.7 % /
1432 B**. This pass used a locally built **assertions-enabled LLVM 7.0.0**
(`llc -debug-only=block-placement`) to read the real `MachineBlockPlacement`
decision log for the function, and re-tested the two-switch hypothesis. The
MBP mechanism is now proven: the case bodies are laid out by
`selectBestCandidateBlock` in **jump-table successor order (ascending type)**,
and the only plain-source shape that reverses that to the original's
source/group order is a **sequential two-switch form**, which reproduces the
original's table *and* body order exactly but costs **76 B** of body code
(1508 B / 53.2 %). No landable change was found.

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`,
`osize 1432`, `usize 1432`. `.text 0x1337c8`, `.rodata 0x41b67`, lint
`CLEAN ✓`, drift 0. After every experiment below the tree was restored and
`check` re-run to the same 4498 / 4498.

## 1. Original layout (re-derived from the ELF)

`Status::missionCompleted` is at `0xabf38`, size 1432, one
`Mission::getType()` call (`0xabfd6`), no `sxth`, two in-text TBH tables:

```
abfda: sub.w r1, r0, #150     ; i32 return used directly
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1] ; 40 halfwords @ 0xabfe6 (types 150..189)
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1] ; 7 halfwords @ 0xac052 (types 8..14)
```

Decoding the two tables gives the original's *body* order:

```
167/174, 8, 0/11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default
```

which is exactly the `case` order in `src/game/mission/Status.cpp`. The
tables themselves are value-indexed; only the *target addresses* are in
source order. The original's `default` is the loop latch (`0xac45a`:
`add fp, fp, #1; b abf82`), placed after case 172; the shared tails
(`return 0` / `return m` / `setWon`) follow it.

## 2. The real MBP trace (assertions build of LLVM 7.0.0)

The NDK clang is a release build (`-debug-only` is not registered). A local
LLVM 7.0.0 with `LLVM_ENABLE_ASSERTIONS=ON` was built (`llc`, ARM only,
~45 s on 128 cores) and run on the project's own `.ll`:

```sh
cd /dev/shm/w4003
curl -sL -o llvm-7.0.0.src.tar.xz https://releases.llvm.org/7.0.0/llvm-7.0.0.src.tar.xz
tar xf llvm-7.0.0.src.tar.xz
cmake -G Ninja -S llvm-7.0.0.src -B llvm-build -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_TARGETS_TO_BUILD=ARM \
      -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_DOCS=OFF
ninja -C llvm-build llc -j64

# project IR (clang 7.0.2); strip the 7.0.2-only `variables:` field
sed 's/, variables: ![0-9]*//g' cur.ll > cur7.ll
llvm-build/bin/llc -mtriple=armv7-none-linux-androideabi -mattr=+neon \
    -float-abi=soft -filetype=asm -o /dev/null \
    -debug-only=block-placement cur7.ll 2> mbpdbg.txt
```

The trace for `_ZN6Status16missionCompletedEbbx` (block numbers are the
local build's; the structure matches the NDK build, whose MBP result is the
same value order) shows the whole mechanism:

```
Selecting best successor for: %bb.10          ; switch header
    Candidate: %bb.11  ... 18.33%             ; low jump table
    Candidate: %bb.100 ... 81.67%             ; high jump table
    Selected: %bb.100
Merging from %bb.10 to %bb.100
Selecting best successor for: %bb.100         ; high jump table
    Candidate: %bb.34 (4.10%) ... %bb.16 (8.19%) ...
    Selected: %bb.16                          ; 167/174 (two case values)
Merging from %bb.100 to %bb.16
Selecting best successor for: %bb.16
    Not a candidate: %bb.17 -> 100.00% (non-cold CFG conflict)
    %bb.11 -> 1.022594752 (freq)
    %bb.34 -> 0.1865889213 (freq)
    %bb.36 -> 0.1865889213 (freq)
    ...                                        ; all remaining bodies equal
Merging from %bb.16 to %bb.11                 ; selectBestCandidateBlock
```

Two facts follow directly:

1. `selectBestSuccessor(high-JT)` picks **167/174** only because it is the
   one body with two case values (8.19 % vs 4.10 %). Every other body ties.
2. The remaining bodies are placed by **`selectBestCandidateBlock`**, which
   walks `BlockWorkList` and keeps the first block of maximal frequency.
   With all body frequencies equal (`0.1865889213`), the tie is broken by
   *worklist order*. `BlockWorkList` was filled by
   `markBlockSuccessors(high-JT)`, i.e. `MBB->successors()` order, which is
   `buildJumpTable`'s `Table` order = **ascending type value**.

The default (loop latch) is rejected from `selectBestSuccessor` with
`(non-cold CFG conflict)` and is placed later, which is why it lands after
172.

`getFirstUnplacedBlock` (the MBP fallback that walks MBB order = source
order) is **never used** in this function: the trace contains no
`Unnatural loop CFG detected` line, and the worklist is always non-empty
after the high jump table is merged.

## 3. Why the two-switch form gets the original order

The sequential two-switch shape (first switch = group A + low types, second
switch = group B) is the only plain form that reproduces the original:

```cpp
int type = m->getType();
switch (type) { /* 167/174, 8, 0/11, 13, 14, 150..155, 158, 162, 166,
                   170, 184, 189 */ }
switch (type) { /* 156, 157, 160, 163, 164, 165, 168, 169, 171, 172 */ }
```

At the IR level the two switches stay separate (no `SimplifyCFG` merge: the
first switch's bodies all fall through to the second switch header). At
ISel each switch gets its own jump-table MBB (24 successors and 11
successors, each value-sorted *within its group*), so MBP lays group A's
bodies out first and group B's second. `ARMConstantIslands` then merges the
two adjacent high tables into a single 40-entry TBH — exactly the original's
table shape.

Measured with the official verifier: **1508 B / 53.2 %**, and the decoded
high table is

```
150->0x206 151->0x20c 152->0x212 153->0x244 154->0x24a 155->0x262
156->0x43c 157->0x466 158->0x274 159->default 160->0x4a0 161->default
162->0x2ce 163->0x4ce 164->0x4ea 165->0x4ee 166->0x332 167->0x0fe
168->0x4f6 169->0x50e 170->0x39a 171->0x512 172->0x52c 173->default
174->0x0fe 175..183->default 184->0x3a8 185..188->default 189->0x3e2
```

The *sharing pattern* is identical to the original's (167 and 174 share a
body; 159/161/173/175..183/185..188 are the default; the low table is
`8, default, default, 11, default, 13, 14`). The body order is exactly

```
167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172 default
```

i.e. the original. This is strong evidence that the original source really
was written as two sequential switches on the same stored type.

## 4. Where the 76 bytes go: register allocation, not dispatch

The dispatch/prologue is **the same 254 bytes** in both builds; the whole
76-byte difference is inside the case bodies.

| region | original | two-switch |
|---|---:|---:|
| prologue + loop header + both dispatches + tables | 254 | 254 |
| 27 case bodies + latch + shared tails | 1178 | 1254 |
| **total** | **1432** | **1508** |

Per-body sizes (bytes, original vs two-switch):

| body | orig | 2sw | | body | orig | 2sw |
|---|---:|---:|---|---|---:|---:|
| 167/174 | 42 | 48 | | 170 | 14 | 14 |
| 8 | 100 | 116 | | 184 | 58 | 58 |
| 0/11 | 10 | 34 | | 189 | 60 | 90 |
| 13 | 14 | 14 | | 156 | 42 | 42 |
| 14 | 52 | 52 | | 157 | 52 | 58 |
| 150 | 6 | 6 | | 160 | 46 | 46 |
| 151 | 6 | 6 | | 163 | 28 | 28 |
| 152 | 52 | 50 | | 164 | 4 | 4 |
| 153 | 6 | 6 | | 165 | 8 | 8 |
| 154 | 24 | 24 | | 168 | 30 | 24 |
| 155 | 18 | 18 | | 169 | 4 | 4 |
| 158 | 92 | 90 | | 171 | 26 | 26 |
| 162 | 102 | 100 | | 172 | 64 | 64 |
| 166 | 100 | 104 | | latch+tails | 118 | 120 |

The cause is visible in the prologue:

```
original (0xabf38):   mov r6, r0      ; this
                      mov.w fp, #0    ; i
                      mov r8, r1      ; atStation
two-switch (v_seq):   mov r6, r0      ; this
                      mov.w r8, #0    ; i
                      mov r9, r1      ; atStation
                      mov fp, r0      ; type   <-- extra live value
```

`type` is live from the first dispatch to the second dispatch, across every
group-A body. It takes `fp`; `i` is pushed down to `r8` and `atStation` to
`r9`; `this` no longer fits in `r6` and is spilled to `[sp,#16]` (the
bodies then reload it with `ldr r0, [sp,#16]` instead of using `r6`). That
different scratch-register choice also breaks tail merging: e.g. the
original's `0/11` body is 10 B because the station check is shared with
cases 165/171 at `0xac404`, while the two-switch body inlines it (34 B).

So the two-switch form is *the right source shape* but not a byte-match: to
land it the register allocator would have to keep `type` out of a
callee-saved register (the original's `type` lives in `r0` along the
dispatch path only). A single-switch lowering cannot do that, and a
two-switch lowering must keep the value live across the first switch.

## 5. Variant sweep (compile-only layout, official verifier for scored rows)

`D` is the shared default group. Every row is the high-table body order.

| source form | size | high-table body order |
|---|---:|---|
| baseline `switch ((short) m->getType())` | 1432 | value order (57.7 %) |
| `switch (m->getType())` (int) | 1428 | value order (60.2 %) |
| `unsigned` / `const int` / `register int` switch | 1508 | value order |
| `short type` switch | 1512 | value order |
| `switch (type)` + `default: break;` at top/bottom | 1428 | value order |
| case blocks reordered (asc/rev/group-first/167-last) | 1428–1432 | value order |
| `if/else` ladder on `int type` | 1428 | value order |
| nested `default: switch (type)` | 1428 | value order (switches merge in SimplifyCFG) |
| nested + `if (type==0x7fffffff) break;` / `goto` / `type+0` | 1428 | value order |
| nested + `(short) type` inner | 1412 | garbled |
| **sequential two-switch** (`int type`) | **1508** | **exact original order (53.2 %)** |
| two-switch, 2nd switch `m->getType()` (2 calls) | 1468 | group B merged with default |
| two-switch, 2nd switch `(short) type` | 1548 | garbled |
| two-switch, 2nd switch `type & 0xff` | 1516 | group B merged with default |
| two-switch, `short`/`unsigned`/`const`/`register` type | 1508–1512 | exact original order |
| two-switch, explicit `default: break;` in either switch | 1508 | exact original order |
| `__builtin_expect(type, N)` nested | 1356–1448 | expected case peeled first |
| `-mllvm -force-loop-cold-block` | 1432 | value order, 184/189 before D |
| `-mllvm -loop-to-cold-block-ratio=1/2/10/100` | 1432 | value order |

Two negative results worth recording:

* **Predecessor count is not the discriminator.** `nextCampaignMission`
  (byte-exact, 8492 B) has 157 jump-table successors, 120 of which have a
  self-loop predecessor, and its NDK MBP result is the identity permutation
  — yet its successor list is also value order. So "single predecessor ⇒
  pushed to `BlockWorkList`" does not by itself explain either function.
* **`-force-loop-cold-block` does not reach the fallback.** It outlines the
  bodies from the loop chain, but the high-JT block stays in the loop chain
  and `markBlockSuccessors` still pushes the bodies in successor order when
  the chain is merged in the final `buildChain`.

## 6. Conclusion — proven vs open

**Proven (commands above):**

1. The original's high TBH is value-indexed (150..189) and its *bodies* are
   in source order; the original's `default` is the loop latch, placed after
   case 172; the shared tails follow.
2. The real LLVM 7 `MachineBlockPlacement` lays the current build's bodies
   out by `selectBestCandidateBlock` in **jump-table successor order**
   (ascending type), after peeling 167/174 with `selectBestSuccessor`
   (8.19 % vs the tied 4.10 %). The default is rejected there as a
   `non-cold CFG conflict`.
3. `getFirstUnplacedBlock` (MBB/source order) is never used for this
   function because the worklist is never empty once the high jump table is
   merged.
4. The sequential two-switch form reproduces the original's table sharing
   pattern *and* body order exactly, but is 1508 B / 53.2 %.
5. The entire 76-byte excess is in the case bodies; the dispatch/prologue is
   the same 254 bytes. The cause is `type` being live across the first
   switch (`fp`), which pushes `i`→`r8`, `atStation`→`r9`, spills `this`
   to `[sp,#16]`, and loses tail merges (0/11: 34 B vs 10 B; 189: 90 B vs
   60 B).
6. No tested plain single-switch shape, case ordering, `default` placement,
   nested form, or MBP/switch flag produces the original body order at
   1432 B.

**Open:** the original source is almost certainly the two-switch form (the
table sharing pattern is a fingerprint), and the original compiler managed
to keep `type` out of the callee-saved set. A source shape that lets clang
7.0.2 rematerialise/keep the type in `r0` across the first switch — or a
single switch whose jump-table successor list is source-ordered — would
close the remaining 76 B. The `.missioncase` rank table is *not* needed and
must not be re-added.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4003
cmake --build cmake-build-match --target check -j8   # RATCHET PASS, byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# body order of the current object / of a variant
/dev/shm/w4003/cc.sh "$PWD/src/game/mission/Status.cpp" /dev/shm/w4003/cur.o
python3 /dev/shm/w4003/layout.py /dev/shm/w4003/cur.o

# real MBP decision log (local assertions LLVM 7.0.0, see §2)
sed 's/, variables: ![0-9]*//g' cur.ll > cur7.ll
/dev/shm/w4003/llvm-build/bin/llc -mtriple=armv7-none-linux-androideabi \
    -mattr=+neon -float-abi=soft -filetype=asm -o /dev/null \
    -debug-only=block-placement cur7.ll 2> mbpdbg.txt
```

Scratch (not committed): `/dev/shm/w4003/` — `v_seq.cpp` (two-switch),
`v_nest.cpp`, `nv_*.cpp`, `qv_*.cpp`, `rv_*.cpp`, `pv_*.cpp`, `tv_*.cpp`
(variant sweep), `layout.py`, `decode.py`, `bodysizes2.py`, `mbp_mc.txt`
(the MBP trace), `llvm-7.0.0.src/`, `llvm-build/`.
