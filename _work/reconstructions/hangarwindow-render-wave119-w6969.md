# `HangarWindow::render` — wave 119 (w6969): ternaries land with a Generator −4

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B.  Worktree `/home/debian/GalaxyOnFire2/wt/w6969`,
branch `wave119-w6969`, base master `d512ec24e` (wave-119 continuation state).

Builds on `hangarwindow-render-wave119-w6933.md` (the ternary/pad3 change) and
`hangarwindow-render-wave119-w6942.md` (the −4 census), and replaces the expensive
`MGame::OnTouchEnd` −8 that w6942 used with a **free** −8 in
`Generator::createMission`.

```
before (HEAD d512ec24e):  HangarWindow 91.337281 %  9700 B  LSDA 944 B
                          Generator    84.710744 %  1996 B  LSDA 176 B
                          .ARM.extab 0x26298  .rodata 0x1c9e10
after:                    HangarWindow 91.655191 %  9700 B  LSDA 948 B (exact)
                          Generator    82.644628 %  1996 B  LSDA 168 B (−4 vs orig)
                          .ARM.extab 0x26294  .rodata 0x1c9e10
gate: byte 4505  linked 4505  data 843/843  imports 207  lint CLEAN  drift 0
      27 sections  file size 2,187,328 (exact)  avg 99.94  RATCHET PASS
```

## 1. The two source edits

### 1.1 `src/game/ui/HangarWindow.cpp` — the w6933 ternary shape

Replace the selected-item `right`/`left` `if/else` with two conditional operators and
delete the `volatile int pad3 = left;` codegen anchor.  This is the w6933 change
verbatim; the comment is kept because the ternary is load-bearing for the LSDA.

```diff
-                                    int right, left;
-                                    if (tab == 1) {
-                                        right = item->item->getStationAmount();
-                                        left = item->item->getAmount();
-                                    } else {
-                                        right = item->item->getAmount();
-                                        left = this->bluePrint->getCurrentAmount(item->item->getIndex());
-                                    }
+                                    int right = tab == 1 ? item->item->getStationAmount() : item->item->getAmount();
+                                    int left = tab == 1 ? item->item->getAmount() : this->bluePrint->getCurrentAmount(item->item->getIndex());
                                     if (left > 0) {
-                                        volatile int pad3 = left;
```

`render` stays at 9700 B; the record grows 944 → **948 B, exactly the original**.
The body is unchanged in size because the two `?:` arms add +4 B and deleting `pad3`
removes −4 B.

### 1.2 `src/game/mission/Generator.cpp` — drop the empty-destructor guard

The `NoopGuard` was an artificial EH-scope anchor around the type-8 item count:

```diff
-    struct NoopGuard { ~NoopGuard() {} };
...
-        int count; { NoopGuard g; count = Globals::rnd->nextInt(15); }
+        int count = Globals::rnd->nextInt(15);
```

The guard emits no code (`.text` stays **1996 B**), but its cleanup range is a separate
call-site row.  Removing it merges that row with the preceding `nextInt` range:

| | before | after |
|---|---:|---:|
| `createMission` record | 176 B | **168 B** |
| `cslen` | 152 | **146** |
| call-site rows | 26 | **25** |
| `.text` | 1996 | 1996 |

The merged row is `start=0x2ec len=0x12 lp=0x754` (was `0x2ec/0x6/0x756` +
`0x2fa/0x4/0x754`).  The 6-byte row removal takes the padded record from 176 to 168,
i.e. **−4 relative to the original's 172 B**.

## 2. Why the sizes work out

`.ARM.extab` starts at `0x1a3b78` (8 mod 16) and `.rodata` is 16-byte aligned.  After
the HangarWindow +4 the section is `0x2629c`; the Generator −8 takes it to
`0x26294`.  `0x1a3b78 + 0x26294 = 0x1c9e0c`, which aligns back to **`0x1c9e10`**, so
every section address (and the exact 2,187,328-byte file size) is unchanged.  The
allowed window is `0x26289..0x26298`; `0x26294` is inside it.

## 3. Cost

`HangarWindow::render` gains **+0.318 raw points** (91.337281 → 91.655191).
`Generator::createMission` loses 2.066 raw points (84.710744 → 82.644628) because
removing the guard changes its register allocation; both functions were already
non-byte-exact, so `byte_exact`/`linked_exact` stay at **4505/4505** and the
4524-function average stays 99.94.  This is much cheaper than the w6942
`MGame::OnTouchEnd` −8 (81.6 → 71.9, −9.7 points on that function).

## 4. Verification

```
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

`exidx_table.py --diff` now reports exactly one differing record:

```
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE: orig 172 ours 168 delta -4
total delta -4 (-0x4)
```

`HangarWindow::render` is exact (948 B).  `.text` is `0x1337c8` (exact), `.ARM.exidx`
`0x6f70` (exact), `.ARM.extab` `0x26294` (4 B short), `.rodata` `0x1c9e10` (exact),
27 sections, file size 2,187,328 B.

## 5. Doc follow-up (not committed here)

`docs/VALIDATION.md` §"Current state — ELF sections" and `docs/IDENTICALITY.md` still
describe the old cancelling pair (`HangarWindow` 944 vs 948, `Generator` 176 vs 172).
The new truth is `HangarWindow` exact at 948 B, `Generator` 168 vs 172 (−4), and
`.ARM.extab` `0x26294` (4 B short).  Update those sentences in the next docs commit.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```
