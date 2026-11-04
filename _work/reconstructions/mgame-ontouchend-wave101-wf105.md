# MGame::OnTouchEnd (8080 B) — wf105 (wave 101): land the fully faithful
# content shape (size-exact, gate-green); the tuned baseline still scores higher

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B).  This pass lands the first **size-exact, gate-green** reconstruction that
contains all four content differences proven from the original disassembly:

1. `m_mcrecall` — the mission ternary re-calls `Status::getCampaignMission()` in
   its else arm (the original has three calls, not two; wave-98 §1.2).
2. `h_both` — the `hudTouchFlags == 0` body is unguarded and calls
   `Hud::closeHudMenu()` first (wave-98 §1.1).
3. `ia_first` — `Mission::isInstantActionMission()` is tested **before**
   `Mission::isCampaignMission()` (wave-99 §2).
4. `t_nested` — the `hudTouchFlags == 0` block is nested under the `else` of the
   weaponSlot check, so the weaponSlot body skips the test (wave-97 §4).

Measured result: **74.7 %** (official `tools/verify` row), **8080 B** (size-exact).
The tuned baseline remains **78.4 %** at the same 8080 B.  Both are gate-green
(`byte_exact 4500`, `linked_exact 4500`, `data 843/843`, imports 207, lint CLEAN,
drift 0, `.text 0x1337c8`, `.ARM.exidx 0x6f70`, `.rodata 0x41b67`).  The faithful
shape is landed because it is the real source shape; the fuzzy metric is
dominated by block placement and is reported separately.

## 1. Why the faithful shape scores lower

The tuned baseline keeps the `[hudTouchFlags test][isAutoPilot][epilogue]` tail
cluster at the end of the function (0x17b930), matching the original's first
~1230 instructions almost instruction-for-instruction.  The faithful `t_nested`
shape moves that cluster to ~0x17aeb2 (the original has it at 0x0e9e), which is
*structurally* correct but breaks the long common prefix that
`SequenceMatcher` rewards.  The remaining gap is still block order, not content:
the call-symbol multiset is identical except for the third `getCampaignMission`
call, which this shape now supplies.

## 2. New evidence: the `keepObjectives` false-arm frequency window

The original's block order around the tail is

```
[starMap cleanup 0x0e00][keepObjectives FALSE 0x0e16][MC chain 0x0e1a]
[0x400 body 0x0e94][shared blx 0x0e9a][test 0x0e9e][isAutoPilot 0x0ea4]
```

`MachineBlockPlacement::selectBestCandidateBlock` picks the highest-frequency
worklist head, so the original's order requires

```
freq(starMap cleanup) > freq(keepObjectives FALSE) > freq(0x400 body)
```

With the witnessed frequencies `freq(starMap cleanup) = 1 568 504 022` and
`freq(0x400 body) = 463 615 365`, the false arm must lie in
`(463 615 365, 1 568 504 022)`.  In our IR the false arm is
`0.375 * freq(keepObjectives test) = 303 284 957`, which is **below** the 0x400
body, so MBP places the 0x400 body first.

The false-arm probability is the zero-heuristic probability of
`icmp eq i8 %campaign->hasWon(), 0` (taken = false arm = 37.5 %).  For the false
arm to land in the window the success edge of the preceding
`if (!hasFailed())` must be 62.5 %, i.e. the IR branch must be
`icmp ne %notFailed, 0` with the success block taken.  NDK clang 7.0.2
canonicalises every spelling of `!hasFailed()` we tried
(`bool nf = !hasFailed(); if (nf)`, `int nf = !hasFailed(); if (nf)`,
`hasFailed() == 0`, `!hasFailed()` directly, `hasFailed() ^ 1`) to
`icmp eq i8 %hasFailed, 0` (37.5 %), so the source alone cannot move the false
arm into the window.  The original binary's `cmp r4,#0; beq.w <success>` is
consistent with either IR predicate; the block order is the only evidence that
the original's success edge was 62.5 %.

## 3. Measured variants (official `tools/verify` row)

All variants keep the four faithful content fixes unless noted.  `pct` is the
report's rounded value; the baseline exact ratio is 78.3565.

| variant | size | pct | gate |
|---|---|---|---|
| tuned baseline (flat wingman, guarded hud0, 2 getCampaignMission) | 8080 | **78.4** | green |
| **faithful (m_mcrecall + h_both + ia_first + t_nested)** | **8080** | **74.7** | **green** |
| faithful + `__builtin_expect(failed, 0)` | 8068 | 72.7 | byte 4283 |
| faithful + `__builtin_expect(campaign->hasWon(), 0)` | 8088 | 73.5 | byte 3102 |
| baseline + `__builtin_expect(failed, 0)` | 8064 | 71.3 | byte 4283 |
| baseline + `__builtin_expect(campaign->hasWon(), 0)` | 8092 | 74.6 | byte 3102 |
| m_mcrecall + h_both + ia_first (no t_nested) | 8076 | 76.4 | byte 4480 |

The `__builtin_expect` probes confirm the frequency model: `expect(failed, 0)`
makes the false arm ~727 M and moves it before the 0x400 body, but the extra
code/alignment and the moved starMap-cleanup chain cost more than the tie is
worth.  No probe beats 78.4.

## 4. Reproduction

```
# full gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
# target row
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
```

Scratch harness: `/dev/shm/wf105/` (baseline/faithful source copies, `show_*.txt`,
`verify_*.txt`).  Not tracked.

## 5. Conclusion

The four content fixes are proven from the binary and are now landed together in
a size-exact 8080 B body with a green gate.  The tuned baseline's 78.4 % remains
the best fuzzy score; the faithful shape scores 74.7 % because its correct early
tail cluster breaks the long common prefix.  The remaining byte gap is the
`keepObjectives` false-arm frequency: the original's order needs the false arm
in `(463 615 365, 1 568 504 022)`, which requires a 62.5 % success edge on
`!hasFailed()` that clang 7.0.2 canonicalises away.
