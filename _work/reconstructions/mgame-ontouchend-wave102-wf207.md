# MGame::OnTouchEnd (8080 B) — wf207 (wave 102): the first byte difference is
# the refined-cargo `amount` comparison; the tail cluster is an MBP artifact

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B).  Baseline = the wave-101 faithful shape (`f30ea0ef1`): 74.7 %,
8080 B, gate-green.  No source change landed this pass: every candidate that
fixes a proven content difference moves the register allocation and loses
more than it gains.  This note records the new evidence so the next pass does
not re-derive it.

## 1. The first byte difference is at 0x17a5ca (refined-cargo loop)

The faithful shape is byte-identical to the original from the function entry
through 0x17a5c8 (419 instructions / the first 1158 bytes).  The first
difference is the post-loop guard of the black-market "refine ore" report:

```
orig  17a5ca: f1b9 0f00   cmp.w  r9, #0
      17a5ce: d040        beq.n  17a652          ; if (amount == 0) skip the report
ours  17a5ca: f1b9 3fff   cmp.w  r9, #0xffffffff
      17a5ce: dc40        bgt.n  17a652          ; if (count > 0) skip the report
```

`r9` is `amount` (0, -1, -2, …; `17a58c mov.w r9,#0`, `17a5b6 sub.w r9,r9,#1`),
and the report string is built from `rsb r1, r9, #0` (`-amount`).  So the
original source is:

```cpp
if (amount != 0) {
    report += String("\n") + String(-amount) + String("t ") +
              *Globals::gameText->getText(1274 + refined);
}
```

not the current `int count = -amount; if (count > 0) { … String(count) … }`.
This is a **real content difference**, not a codegen artifact.

### Why it cannot be landed as-is

Any spelling that makes clang emit `cmp #0` (`amount != 0`, `count != 0`,
`count`, `count > 0u`, `unsigned count > 0`) also lets InstCombine replace the
`amount--` loop counter with `count++` and swaps the two long-lived
callee-saved values (`this` and `Globals::status` exchange `sl`↔`r9` at
0x17a154).  Result: 8076 B (not size-exact) and the fuzzy score drops:

| variant | size | pct | gate |
|---|---|---|---|
| faithful baseline (`count > 0`) | 8080 | **74.7** | green |
| `if (amount != 0)` + `String(-amount)` | 8076 | 68.4 | byte 4480 |
| `int count = -amount; if (count != 0)` | 8076 | 68.4 | byte 4480 |
| `if (count)` | 8076 | 68.4 | byte 4480 |
| `unsigned count = -amount; if (count > 0)` | 8076 | 68.4 | byte 4480 |
| `if (count > 0u)` | 8076 | 68.4 | byte 4480 |
| `if (amount < 0)` + `String(-amount)` | 8084 | 68.4 | byte 3102 |
| tuned baseline + `if (count != 0)` | 8076 | 71.4 | byte 4480 |

`if (count >= 1)` canonicalises back to `count > 0` (identical IR, 74.7 %).
The register-allocation penalty is global (the first difference moves to
instruction 7, `mov sl,fp` vs `mov r9,fp`), so it costs ~6 points even though
the comparison itself is only 4 bytes.

## 2. Tail cluster: flat wingman gets the right order, nested gets the right CFG

Original tail block order (address order):

```
[0x800 arm 0x17ae8c -> b test][maneuver 0x17aeac -> b isAutoPilot]
[pause-menu 0x17aebc …][MC chain][0x400 arm 0x17afd8][initHudMenu 0x17afde]
[test 0x17afe2][isAutoPilot 0x17afe8][epilogue 0x17b004]
[__stack_chk_fail 0x17b016][hud0 body 0x17b01a -> b isAutoPilot]
```

The faithful **nested** shape (`else { if (wingmanCommand >= 0) … if (hudTouchFlags == 0) … }`)
produces the correct CFG (weaponSlot body → isAutoPilot = skips the test;
wingman body → test) but MBP places the tail as
`[test 0x17aeb2][hud0 body 0x17aebc] … [maneuver 0x17aefc] … [isAutoPilot 0x17b95a]`.

The **flat** shape (`} else if (wingmanCommand >= 0) { … } if (hudTouchFlags == 0) …`)
produces the original's tail order
`[test 0x17b930][isAutoPilot 0x17b93a][epilogue][stack_chk_fail][hud0 body 0x17b968]`
but the wrong weaponSlot-body successor: it falls through to the test
(`17b92c blx closeHudMenu; 17b930 ldr hudTouchFlags`) instead of jumping to
isAutoPilot.  That is exactly the tuned baseline's 78.4 % shape.

Isolation: flat wingman + **unguarded** hud0 (hud0 body matches the original's
`ldr r0,[fp,#116]; blx closeHudMenu` with no null check) = 76.6 %, 8076 B,
and still has the original's tail order.  So the tail order is controlled by
the wingman nesting, not by the hud0 null guard.

## 3. MBP conflict numbers (faithful nested shape, `-debug-only=block-placement`)

Built with the prior agent's instrumented llc 7.0.0
(`/home/debian/llvm7build/build/bin/llc`, `-debug-only=block-placement`):

```
bb.226 (test, L2117)        f=2861459437
bb.227 (hud0 body, L2118)   f=1073047289
bb.228 (resetGunDelay,L2127) f=301417777
bb.229 (isAutoPilot, L2144) f=5202653523
bb.218 (maneuver, L2138)    f=650331690
```

* `bb.209 (0x800 arm) -> bb.226` is accepted (100 %, CFG break).
* `bb.226 -> bb.229` (62.5 %) is **rejected**:
  `pred bb.227 prededge=1073047289 *hp=858437831 >= candedge=1788412148 *cp=357682429`.
  Since `freq(bb.227) = 0.375 * freq(bb.226)` (the `icmp eq …,0` zero-heuristic
  is 37.5 %), `0.375*0.8 = 0.30 >= 0.625*0.2 = 0.125` always holds.  The test
  would have to be < 15.625 % for `hasBetterLayoutPredecessor` to accept it.
* `bb.226 -> bb.227` (37.5 %) is taken, then `bb.227 -> bb.229` is rejected
  (conflict with `bb.228`: `301417777*0.8 = 241134221 >= 1073047289*0.2 = 214609457`),
  `bb.227 -> bb.228` (39.06 %) is taken, `bb.228 -> bb.229` is rejected
  (conflict with `bb.218`: `650331690*0.8 = 520265351 >= 301417777*0.2 = 60283555`),
  and `selectBestCandidateBlock` falls back to the highest-frequency unplaced
  block, `bb.210` (pause-menu entry, 0x001086294651), which is why the whole
  pause-menu/MC-chain region is spliced into the tail chain.
* `bb.209 -> bb.226` is only accepted because `bb.225` (shared initHudMenu,
  f=2032286531) is already in the chain and is skipped by the predecessor
  loop.  If it were unplaced, `2032286531*0.8 >= 812914612*0.2` would reject
  the edge and put the 0x800 arm in its own chain (the original's layout).

`if (!hudMenuOpen) { switch … } else { chain }` (swap the if/else) changes the
IR block order but produces byte-identical code (74.7 %).

## 4. Measured variant table (official `tools/verify` row)

| variant | size | pct | gate |
|---|---|---|---|
| faithful baseline (wave-101) | 8080 | **74.7** | green |
| tuned baseline (flat wingman + guarded hud0 + 2 getCampaignMission) | 8080 | **78.4** | green |
| tuned + 3rd getCampaignMission | 8080 | 78.3 | green |
| tuned + `ia_first` | 8076 | 76.4 | byte 4480 |
| flat wingman + unguarded hud0 | 8076 | 76.6 | byte 4480 |
| `if (!hudMenuOpen) { switch } else { chain }` | 8080 | 74.7 | green (identical code) |
| `weaponSlot != -1` / `wingmanCommand != -1` / `!hudTouchFlags` / `count >= 1` | 8080 | 74.7 | green (identical IR) |

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# MBP trace
.cache/ndk/.../clang++ <MGame.cpp flags> -S -emit-llvm -o /tmp/MGame.ll -c src/game/menu/MGame.cpp
sed -E 's/, variables: ![0-9]+//g' MGame.ll > MGame_strip.ll
/home/debian/llvm7build/build/bin/llc -mtriple=armv7-none-linux-androideabi \
  -mcpu=cortex-a8 -mattr=+neon -float-abi=soft -filetype=asm \
  -debug-only=block-placement MGame_strip.ll -o /dev/null
```

Scratch: `/dev/shm/wf207/` (baseline/tuned source copies, `base.s`, `tuned.s`,
`cur_mbp.txt`, `cur_after.txt`).  Not tracked.

## 6. Conclusion / next step

The remaining gap is two independent things:

1. The `amount != 0` comparison (real content, 4 bytes at 0x17a5ca).  Landing
   it needs a source shape that keeps `amount` as the loop counter and keeps
   `this` in `sl` / `Globals::status` in `r9`; every spelling tried so far
   lets InstCombine rewrite the loop and swap the callee-saved registers.
2. The tail-cluster placement.  The faithful nested shape has the correct CFG
   but MBP splices the pause-menu/MC-chain region into the tail chain because
   `bb.226 -> bb.229` is blocked by the 37.5 % `hudTouchFlags == 0` edge.
   The flat shape fixes the tail order but breaks the weaponSlot-body
   successor (and therefore is not the real source shape).

## 7. RESOLVED (this pass): the real source is a *positive* counter

The `amount != 0` dead end in §1 was caused by the loop being written as a
*negative* counter.  The original source counts **up**:

```cpp
int amount = 0;
while (ship->hasCargo(ore, fee)) {
    ship->removeCargo(ore, fee);
    ship->addCargo((*Globals::items)[refined]->makeItem(1));
    amount++;
    refinedAny = true;
}
if (amount > 0) {
    report += String("\n") + String(amount) + String("t ") +
              *Globals::gameText->getText(1274 + refined);
}
```

Why this matters: `count = -amount; if (count > 0)` canonicalises to
`icmp slt amount, 0` (Basic, 50 % branch) and LSR keeps the decrementing IV;
the wrong `cmp.w r9,#-1; bgt`.  A positive `amount++` with `if (amount > 0)`
is folded by InstCombine to `icmp eq amount, 0` (ICmpZero, 37.5 % branch) but
the backend still materialises `-amount` in the loop register, so it emits
exactly the original:

```
17a58c  mov.w   r9, #0
17a5b6  sub.w   r9, r9, #1        ; r9 = -amount
17a5ca  cmp.w   r9, #0
17a5ce  beq.n   17a652            ; amount == 0
17a5da  rsb     r1, r9, #0        ; String(amount)
```

Measured with `emit_s.sh` + `screen.py` (thisreg / counterop / cmp / branch):

| loop source | thisreg | counterop | cmp | branch |
|---|---|---|---|---|
| `amount--; count=-amount; if (count>0)` (old) | r10 (sl) | sub r9 | `#-1` | bgt |
| `amount--; if (amount!=0) String(-amount)` | r9 | add r8 | `#0` | beq |
| `amount++; if (amount>0) String(amount)` | **r10 (sl)** | **sub r9** | **`#0`** | **beq** |
| `amount++; if (amount>=1) String(amount)` | r10 | sub r9 | `#0` | beq |
| `amount++; if (0<amount) String(amount)` | r10 | sub r9 | `#0` | beq |
| `amount++; if (amount) String(amount)` | r9 | add r8 | `#0` | beq |

`if (amount > 0)` / `>= 1` / `0 < amount` are equivalent; `!= 0` and `if (amount)`
are not (they keep the raw incrementing IV and swap `sl`/`r9`).

The report block is now **byte-identical** (54 instructions, verified with a
text diff of the disassembly from `cmp.w r9,#0` through `ldr.w r9,[sp,#36]`).

### WeaponSlot body call order

The original's weaponSlot tail is

```
17ba0c  pauseOpen = 0
17ba12  hudMenuOpen = 0
17ba18  resumeSounds()
17ba20  closeHudMenu()
```

The old source had `resumeSounds()` *before* the two stores.  Moving the
stores in front of the call is a real content fix (the two stores are
independent, so it is semantically identical).

## 8. Measured after landing

Official verifier row (`tools/verify/verify.py --show`), gate `check` green:

| variant | size | pct | diff bytes | gate |
|---|---|---|---|---|
| wave-101 baseline | 8080 | 74.7 | 4505 | green |
| **landed: positive counter + weaponSlot call order** | **8080** | **74.8** | **4463** | **green** |
| positive counter only | 8080 | 74.9 | 4466 | green |
| positive counter + `hudMenuOpen;pauseOpen` store order | 8080 | 73.6 | 4463 | green |
| tuned hill-climb (flat wingman + guarded hud0 + mission reorder) | 8080 | 78.4 | 3965 | green |

`diff bytes` = number of differing bytes in the 8080-byte function
(`/dev/shm/wf207/firstdiff.py`).  The remaining differences are **only block
placement**: the first differing byte is 0x16f, the `b.w` to the epilogue
(original `17b004`, ours `17b976`).  Every instruction in the report block and
the weaponSlot body now matches; the tail chain is placed 0x972 bytes later.

The 78.4 % tuned variant is *not* faithful: it uses the flat wingman shape
(original is nested: weaponSlot body jumps straight to isAutoPilot), a guarded
`closeHudMenu()` (original has no null check), and reorders the mission block
(original calls `getCampaignMission()` twice and tests `isInstantActionMission`
before `isCampaignMission`).

## 9. Reproduction

```
cmake --build cmake-build-match --target check -j8    # RATCHET PASS
cmake --build cmake-build-match --target drift -j8    # 0 drifted fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5MGame10OnTouchEndEiiPv
```

Scratch: `/dev/shm/wf207/` (`var_A.cpp` = counter only, `var_H.cpp` = landed,
`var_I.cpp` = store-order variant, `firstdiff.py`, `probe2.py`).
