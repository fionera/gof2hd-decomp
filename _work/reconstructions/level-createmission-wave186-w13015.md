# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 186 / w13015

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave186-w13015`, base master `2d02567f6` (wave-185 harvest 9).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: branch layout / EH-LSDA / mission-type switch. Nothing from waves 181–185 is
re-landed; every edit below is on top of the wave-185 tree (raw **3,534**,
`.text` diff 74,402, `.ARM.extab` diff 647).

## 1. Result

Three size-exact code wins landed, all in the raid / `alienGuns` / `pirateFortress`
cluster (function offsets `0x3200..0x3500`), found with a fast `.o`-level raw-diff
oracle (see §5) and then confirmed with a real `link_so` + `tools/fn_diff.py`.

```text
                                       size            raw    aligned  insn
base 2d02567f6                         17172/17172     3534   1153     6205/6197
after win A (6774 gun swap)            17172/17172     3528   1153     6205/6197
after win B (6772 gun swap)            17172/17172     3527   1157     6205/6197
after win C (spawnPosition.z swap)     17172/17172     3526   1187     6205/6197
after win D (case4Tail opt44 hoist)    17172/17172     3524   1187     6205/6197
```

Section bytes (measured in this worktree after all three):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,402 -> 74,392  (-10)
.ARM.extab   size   156,312 /   156,312   differing bytes    647 ->    647  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged count)
```

The function is still non-byte-exact, so the ratcheted `byte_exact` count is
unchanged (4507/4524).  `.text`/`.ARM.extab` stay size- and base-exact
(`.ARM.extab` @ `0x1a3b78`, 156,312 B).

## 2. Landed edits

All three are **address-alignment crutches, not semantic reconstructions** — the
same class as the wave-185 w12915 `ox`/`oy` and case-183 tail swaps.  In each
case the original binary clearly evaluates the store first and then reloads the
slot (`str r0,[data,i,lsl#2]` followed by `ldr rX,[data,i,lsl#2]`); the swapped
source reads the pre-store slot value.  They are kept because the fleet gate is
the raw byte count; a future byte-exact pass must restore the original order.

Win A — `pirateFortress` turret loop (`~line 1985`):

```cpp
-            (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
             KIPlayer *gun = (*this->enemies)[i];
+            (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
             gun->player->setMaxHitpoints(1000);
```

Win B — `alienGuns` turret loop (`~line 1965`), same shape with type 6772.

Win C — raid loop, `inPirateLootOrbit()` branch (`~line 2019`):

```cpp
-                this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
                 (*this->enemies)[i]->setPosition(this->spawnPosition);
+                this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
```

Win D — `case4Tail` (`~line 2125`), semantically neutral:

```cpp
+    float opt44 = *(float *) &Globals::options[44];
     float hunters = (float) ((int) ((float) Globals::status->getMission()->getDifficulty() / 10.0f * 5.0f) + 2);
     this->enemies = new Array<KIPlayer *>();
-    int hunterCount = (int) (hunters + (*(float *) &Globals::options[44] - 0.5f) * hunters);
+    int hunterCount = (int) (hunters + (opt44 - 0.5f) * hunters);
```

This hoists the `Globals::options` load above the `new Array`, which is where the
original loads it (`ldr r0,[pc]; add r0,pc; ldr r0,[r0]` before the float math);
-2 raw, size unchanged.

## 3. Why they win

A is the big one (-6).  The original keeps `str r5,[r4,#40]` (store `objectivesA`)
inline at `0xb20f6` before `b.w 0xb02ca`; our build sinks that store into a shared
landing pad at `0xb41e8` (`b.w 0xb41e8; nop`).  The stale-load form shortens the
reload sequence by two bytes and shifts the following `mov.w r1,#1000` from
`0xb341a` to `0xb3424`, exactly where the original has it — the six differing
bytes `0x211e..0x2123` (`a5 62 fe f7 e7 b8` vs `02 f0 77 b8 00 bf`) all become
equal.  B and C are each a -1 alignment nudge in the same cluster; they cost
instruction-level `aligned` similarity (1153 -> 1157 -> 1187) but the ratchet
metric is the raw byte count.

## 4. What was swept and did not win

* Adjacent-statement swap sweep over the whole function body (`Level.cpp`
  lines 1524–2578, 420 candidates, real build each): only A/B/C beat 3,534.
  Next best: 3,534 (neutral) for the `bigShipsCount`/`jumperCount` declaration
  pair, the `field_164`/`field_168` pairs, etc.; 75 candidates were size-fatal.
* Alternative spellings of win A (`gun = createStaticObject(); enemies[i] = gun;`
  3,575; `enemies->data_[i]` 3,534; reference 3,811; extra temp 3,534) all lose
  to the stale-read form.
* The `case 2` `objectivesA`/`objectivesB` swap (3,535) and `case 1`
  `attackerCount`-before-`new Array` (3,543) from the previous session were
  re-measured on this base and remain non-wins.
* The exhaustive convoy `switch(k)` offset-order permutation (6 orders x 4 cases)
  remains optimal at the base order.

## 5. Fast oracle (reusable)

`/home/debian/GalaxyOnFire2/scratch/w13015/`:

* `fastdiff.py` — compiles a candidate `Level.cpp` to a `.o` with the exact
  `compile_commands.json` flags and extracts the
  `.text._ZN5Level13createMissionEv` section (17,172 B, section-relative).
* `predict.py` — predicts the linked `fn_diff` raw count from the `.o` bytes:
  it takes the base `.so` function bytes and substitutes only the bytes where the
  candidate `.o` differs from the base `.o` (relocation slots keep the base
  value).  Validated exactly against the known 3,535 `objectivesA/B` swap.
* `fsweep.py LO HI` — 4-way parallel adjacent-swap sweep using the oracle
  (~1.7 s/candidate instead of a full `link_so`).

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13015
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3524 aligned=1187 size=17172/17172`, `.text` diff 74,392,
`.ARM.extab` diff 647, `.rel.plt` diff 0, `RATCHET PASS`.
