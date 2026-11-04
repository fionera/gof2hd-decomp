# Hud::draw — the gamma-block `Globals::status` reload vs the `this` spill (w101/wf114)

**Status:** research note, 2026-11-04 (wave 101). Worktree `wt/wf114`, branch
`wave101-wf114`, base `master 3856949ab`. No source change landed: every variant measured
at or below the 93.0 % baseline. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (file 0x163b90).

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports **207**, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, lint **CLEAN**, drift **0**, **RATCHET PASS**. Per-function score
**93.0 % -> 93.0 %** (raw `difflib` ratio 92.9604 %, 371 opcodes).

This note extends `hud-draw-second-bar-order-w5015.md`,
`hud-draw-gamma-store-dock-order-w4515.md`, `hud-draw-status-slot-w4253.md` and
`hud-draw-this-spill-w3910.md`. It pins down **why the gamma reload and the `this` spill
have never been reproduced together**, and records the full variant matrix.

---

## 0. TL;DR

1. The original gamma block is the *plain* three-use form
   `Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
   Globals::status->getCurrentCampaignMission())`: clang CSEs the receiver and `getStation`
   receiver into one load, then **reloads** `Globals::status` for the mission argument after
   the `getStation`/`getIndex` calls. Both the reload and the late `str r6,[sp,#108]` are
   reproduced exactly by the plain form.
2. The `this` spill at `163e04` (`str r5,[sp,#104]`) is a **greedy-allocator live-range
   split** of `this` at `bb.28` (the `hasShieldBar` block). It is reproduced by the current
   landed source (a `Status *st` local used for the mission argument, plus the
   `Globals::status == 0 ||` damage-block guard), which is a *tuned* shape, not the original
   statement form.
3. The two are **mutually exclusive in every source shape tried this session**: every variant
   with the gamma reload loses the `this` split (frame 216, 9312 B); every variant with the
   `this` split keeps a single `Globals::status` load (frame 224, 9332 B).
4. Root cause (proved from `-mllvm -print-after-all`): the split is created by the greedy
   allocator and correlates with the **number of `Globals::status` loads in `bb.37`**. With
   one load (`%646` reused for all three calls) the allocator splits `this` at `bb.28`; with
   two loads (the reload `%649`) it does not. The original has two loads *and* the split, so
   the original's split must come from a construct outside the gamma block that has not been
   found yet.
5. Current baseline is the best measured shape (93.0 %). The plain/full-reload shape is
   73.6 % because the missing split costs 8 bytes of frame and rotates every stack slot.

---

## 1. Ground truth: original gamma block (file offset = VMA)

```
164008: strd  fp, r9, [sp, #116]      ; spill ego, &Globals::Canvas
16400c: ldr   r0, [pc, #276]          ; GOT offset for &Globals::status
16400e: add   r0, pc
164010: ldr   r6, [r0, #0]            ; r6 = &Globals::status
164012: ldr   r4, [r6, #0]            ; r4 = Globals::status   (load #1)
164014: mov   r0, r4
164016: blx   Status::getStation
16401a: blx   Station::getIndex
16401e: mov   r5, r0                  ; r5 = station index
164020: ldr   r0, [r6, #0]            ; *** reload Globals::status (load #2) ***
164022: str   r6, [sp, #108]          ; *** spill &Globals::status, late ***
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4                  ; receiver = load #1
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
```

The current landed source emits (baseline, 9332 B):

```
16400c: ldr   r0, [pc, #276]
164010: add   r0, pc
164012: ldr   r0, [r0, #0]            ; &Globals::status
164014: str   r0, [sp, #108]          ; spill EARLY
164016: ldr   r4, [r0, #0]            ; load #1 only
164018: mov   r0, r4
16401a: blx   Status::getStation
16401e: blx   Station::getIndex
164022: mov   r5, r0
164024: mov   r0, r4                  ; no reload
164026: blx   Status::getCurrentCampaignMission
```

Source (landed):

```cpp
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                       st->getCurrentCampaignMission()) > 0.0f) {
```

The plain form

```cpp
    if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

emits the original gamma block **byte for byte** (including the reload and the late spill) but
builds to **9312 B** (frame 216) because the `this` split disappears.

## 2. The `this` split is a greedy-allocator live-range split

`-mllvm -print-after-all` on the baseline (`Greedy Register Allocator` dump) shows the split
inserted in `bb.28`:

```
bb.28 (%ir-block.225):
  %535:tgpr   = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
  %2832:gpr   = t2LDRi12 %535, 0
  t2STRi12    %2832, %stack.49
  %539:gpr    = t2LDRi12 %2832, 0
  %540:gpr    = t2LDRi12 %2774, 0
  %2906:gprnopc = COPY %2905:gprnopc      ; <-- split of `this` (%2905)
  %2900:gprnopc = t2LDRi12 %539, 488
  ...
  %542 = t2LDRBi12 %2906, 543            ; hasShieldBar
  t2CMPri %542, 0
  t2STRi12 %2906, %stack.59              ; <-- this spill
  t2STRi12 %2900, %stack.63              ; dividerOffset spill
```

`%2905` is `this` from entry to `bb.27`; `%2906` is the split interval used from `bb.28`
onward. The Virtual Register Rewriter then maps `%2906 -> $r5` and spills it, giving the
original's `str r5,[sp,#104]` and the six reloads at `163e1e/163e26/163e2e/164040/1640ec/16413e`.

The plain gamma block has **no** `%2906 = COPY %2905` in `bb.28`; `this` stays in `$r5` (and
is moved to `sl` before the gamma block).

## 3. Why the two are exclusive: load-count correlation

The only IR difference between the baseline and the plain form is one extra
`t2LDRi12 ... @_ZN7Globals6statusE` in `bb.37` (the reload). Everything else in the
`Live Register Matrix` dump is identical (verified by diffing the normalised dumps).

| gamma block shape | `Globals::status` loads | `this` split | frame | size | pct |
|---|---|---|---|---|---|
| `st->f(st->g(), st->h())` (landed) | 1 | **yes** | 224 | 9332 | **93.0** |
| `Globals::status->f(Globals::status->g(), Globals::status->h())` | 2 | no | 216 | 9312 | 73.6 |
| `st->f(st->g(), Globals::status->h())` | 2 | no | 216 | 9312 | 73.6 |
| `Globals::status->f(st->g(), Globals::status->h())` | 2 | no | 216 | 9312 | 73.6 |
| `Globals::status->f(st->g(), st->h())` | 1 | yes | 224 | 9332 | 93.0 |
| `Globals::status->f(Globals::status->g(), st->h())` | 1 | yes | 224 | 9332 | 93.0 |
| `st->f(Globals::status->g(), st->h())` | 1 | yes | 224 | 9332 | 93.0 |
| two locals `st`,`st2` (compiler CSEs) | 1 | yes | 224 | 9332 | 93.0 |
| `Status *&st = Globals::status` (3 loads) | 3 | no | 216 | 9312 | 73.6 |
| `Status &sr = *Globals::status` (object ref) | 1 | yes | 224 | 9332 | 93.0 |
| `st->f(gidx, gmis)` with `gidx`,`gmis` hoisted | 2 | no | 216 | 9312 | 73.6 |
| `gmis = Globals::status->h()` hoisted, `st->f(st->g(), gmis)` | 1 (CSE) | yes | 224 | 9332 | 92.8 |

The split exists **iff** the gamma block reads `Globals::status` exactly once (i.e. the
mission argument is the cached `st`). Any reload of the global removes it.

## 4. What was tried to get both (all measured, official verifier)

Every entry below keeps the landed guard `Globals::status == 0 || this->field_0x4b8 >= 1`
unless stated. `size` is our function size; `pct` the official fuzzy score.

| variant | size | pct | reload | this spill |
|---|---|---|---|---|
| landed baseline | 9332 | 93.0 | no | yes |
| plain 3-use | 9312 | 73.6 | yes | no |
| plain + `Hud *self = this` (top / before shield / before gamma) | 9312 | 73.6 | yes | no |
| plain + `PaintCanvas *canvas` hoisted before the shield `if` | 9304 | 69.2 | yes | no |
| plain + `int pad_ = this->field_0x470;` + `if (pad_) SetColor(...)` | 9332 | 79.3 | yes | no |
| plain + `Status *st` hoisted before the shield block | 9312 | 73.6 | yes | no |
| plain + `(void) st` after the gamma block | 9312 | 73.6 | yes | no |
| plain + `int dividerOffset` as `long` / `unsigned` | 9312 | 73.6 | yes | no |
| plain + `int dividerOffset` as `short` | 9324 | 72.8 | yes | no |
| plain + `dividerOffset` moved after `SetColor` | 9308 | 73.2 | yes | no |
| plain + `dividerOffset` moved after the armour declarations | 9308 | 73.2 | yes | no |
| plain + `SetColor` before `dividerOffset` | 9308 | 73.2 | yes | no |
| plain + guard `this->field_0x4b8 >= 1 \|\| Globals::status == 0` | 9320 | 73.4 | yes | no |
| plain + guard `Globals::status != 0 && ...` | 9316 | 73.6 | yes | no |
| plain + guard `Globals::status && ...` | 9316 | 73.6 | yes | no |
| plain + guard `!Globals::status \|\| ...` | 9312 | 73.6 | yes | no |
| plain + guard `Globals::status->getMission() == 0 \|\| ...` | 9316 | 73.4 | yes | no |
| plain + guard `Globals::status->inAlienOrbit() \|\| ...` | 9316 | 73.4 | yes | no |
| plain + guard `Globals::status->getCurrentCampaignMission() \|\| ...` | 9316 | 73.4 | yes | no |
| hoisted `gidx`/`gmis` + `PaintCanvas *cv = Globals::Canvas` live across the calls + `&& cv != 0` | 9332 | 78.5 | yes | no |
| hoisted `gidx`/`gmis` + `cv` live + `this` field live (or `ego` live) | 9332 | 78.5 | yes | no |
| plain + `__builtin_expect(cond, 0)` | 9328 | 68.3 | yes | no |
| plain + `__builtin_expect(cond, 1)` | 9340 | 74.6 | yes | no |
| `st` + mission arg `Globals::status->h()` (i.e. C) | 9312 | 73.6 | yes | no |
| C + `st != 0` appended to the condition | 9316 | 73.7 | yes | no |
| C + `st == 0` ORed into the condition | 9316 | 73.7 | yes | no |
| C + `(st, Globals::status->h())` comma | 9312 | 73.6 | yes | no |
| C + `(Globals::status, Globals::status->h())` comma | 9312 | 73.6 | yes | no |
| C + `Status **sp = &Globals::status; (*sp)->h()` | 9312 | 73.6 | yes | no |
| C + `Status *const st` | 9312 | 73.6 | yes | no |
| C + `Status *volatile st` | 9264 | 72.0 | no | no |
| C + cast `((Status *) Globals::status)->...` (receiver / arg1 / both) | 9312 | 73.6 | yes | no |
| `st` + `(Globals::status ? st->h() : 0)` | 9328 | 72.6 | no | no |
| `st` + `(Globals::status && st->h())` | 9336 | 73.3 | no | no |
| `st` + `st->f(st->g(), (Globals::status, st->h()))` | 9332 | 93.0 | no | yes |
| `st` + `st->f(st->g(), Globals::status == st ? h() : h())` | 9332 | 93.0 | no | yes |
| `gmis = Globals::status->h()` first, then `st->f(st->g(), gmis)` | 9332 | 92.8 | no (CSE) | yes |
| `gmis = st->h()` first, then `st->f(st->g(), gmis)` | 9332 | 92.8 | no | yes |
| `gidx`/`gmis` hoisted from `st`, `st->f(gidx, gmis)` | 9332 | 93.0 | no | yes |
| `gidx` hoisted from `st`, `st->f(gidx, st->h())` | 9332 | 93.0 | no | yes |
| `unsigned`/`long` `gidx` hoisted, `st->f(gidx, st->h())` | 9332 | 93.0 | no | yes |
| `short` `gidx` hoisted | 9332 | 92.9 | no | yes |
| gammaY/gammaBarY declaration order swapped | 9332 | 93.0 | no | yes |
| gammaY/gammaBarY as `x + (x - y)` | 9332 | 93.0 | no | yes |
| gammaY/gammaBarY as `x*2 - y` | 9348 | 85.6 | no | yes |
| guard removed (`if (this->field_0x4b8 >= 1)`) | 9264 | 82.9 | no | no |
| guard + mission arg `Globals::status` + no guard | 9240 | 71.6 | yes | no |
| `st == 0 \|\|` guard (instead of `Globals::status == 0`) | 9348 | 60.9 | no | no |
| `Globals::status == 0 \|\| st == 0 \|\| ...` | 9352 | 60.7 | no | no |

## 5. Conclusion

The landed 9332 B / 93.0 % shape is the best achievable this session. The original's
combination — two `Globals::status` loads in the gamma block **and** the `this` live-range
split at `bb.28` — was not reproduced by any of ~60 source shapes. The split is a greedy
allocator artefact that correlates perfectly with the gamma block having a single
`Globals::status` load; the original has two. The remaining hypothesis is that the original
source contains a construct *outside* the gamma block (a use of `this` or of
`Globals::status` that raises the split weight at `bb.28`) that has not been identified.
The dead `ldr.w r8,[sp,#108]` reloads at `164176`/`164210` in the original's damage block
suggest such a construct exists (the original reads `Globals::status` there in a way that
survives to the allocator but not to the final code).

Once that construct is found, the gamma block, the `&Globals::status` `r8` live range, the
duplicated `String::~String`, the `#60 Globals::h` scratch slot and the stack-slot rotation
should all fall into place.

## 6. Reproduce

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8          # RATCHET PASS, byte 4500

python3 tools/verify/verify.py \
  --build-dir /dev/shm/wf114/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Raw MIR dumps (234 MB each) were produced with the NDK clang + the exact build flags plus
`-mllvm -print-after-all`; the `Greedy Register Allocator` / `Virtual Register Rewriter`
sections for `_ZN3Hud4drawExxP9PlayerEgobjj` are the evidence in §2–§3.
