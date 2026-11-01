# `HangarWindow::render` — removing the `pad3` dead local vs. the real LSDA record

Symbol: `_ZN12HangarWindow6renderEv` (mangled), original address `0x1490d8`,
size `0x25e4` (9700 B). TU: `src/game/ui/HangarWindow.cpp`.
Branch: `wave87-w3572`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`.

## TL;DR

`pad3` is **not** a reconstruction of the original LSDA record. It is a pure
section-size lever: it splits the `buttons[8]->draw()` call out of the
surrounding call-site row and adds **12 bytes** to `render`'s `.ARM.extab`
record (916 → 928 B). Moving the same dead local to `buttons[9]` (or
`buttons[7]`) reproduces the *identical* gate numbers (byte 4496 / linked 4496 /
`.ARM.extab 0x26290` / `.rodata 0x1c9e10`), so the location is irrelevant to the
gate.

Removing `pad3` alone is **not** a strict improvement:

| variant | byte_exact | linked_exact | `.text` | `.ARM.extab` | `.rodata` base | render size | render LSDA rows | render record |
|---|---:|---:|---|---|---|---:|---:|---:|
| **baseline (pad3)** | **4496** | **4496** | `0x1337c8` | `0x26290` | `0x1c9e10` | `0x25e4` | 153 | 928 |
| **pad3 removed** | **3931** | 4496 | `0x1337c8` | `0x26284` | `0x1c9e00` | `0x25e0` | 151 | 916 |
| original binary | — | — | `0x1337c8` | `0x26298` | `0x1c9e10` | `0x25e4` | 156 | 948 |

The 565-function `byte_exact` loss is attributable to the `.ARM.extab` record
shrinking past a 16-byte boundary: `.ARM.extab` base `0x1a3b78` + `0x26284` = `0x1c9dfc`,
which rounds the next section base to `0x1c9e00` instead of `0x1c9e10`. Every
literal-pool reference into `.rodata` then shifts by `0x10`.

The original's real record is 948 B / 156 rows. Ours is 928 / 153 *with* `pad3`
and 916 / 151 *without*. So `pad3` only compensates 12 of the 32-byte shortfall;
it does not restore the real record. Restoring the real record requires body
reconstruction (see §5), not deleting the pad. **No source change was landed;
this note is the deliverable.**

## 1. Baseline (measured)

```
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

```
cmake --build cmake-build-match --target drift -j8   -> === DRIFT: 0 field(s) across 0 class(es) ===
cmake --build cmake-build-match --target lint  -j8   -> CLEAN ✓ (no new soft violations; hard invariants 0)
```

Section table (`readelf -SW`):

| section | original | ours (baseline) |
|---|---|---|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x26290 @ 0x1a3b78` (**−8**) |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` |
| `.bss` | `0x69ac @ 0x216970` | `0x69ac @ 0x216970` |

`report.json`: `render` `pct 78.7`, `osize 9700`, `usize 9700`, `byte False`,
`linked False`.

## 2. The `pad3` A/B

`pad3` lives at `src/game/ui/HangarWindow.cpp:587`:

```cpp
(*this->buttons)[8]->setVisible(true);
int pad3 = 0; (void) pad3;
(*this->buttons)[8]->draw();
```

Deleting that one line and relinking (full `link_so` + `verify.py`) gives the
table in the TL;DR. `.text` size/base and `.rodata` size are unchanged; only
`.ARM.extab` size and the `.rodata` base move.

The mechanism is exactly the 16-byte `.rodata` alignment boundary:

```
0x1a3b78 + 0x26290 = 0x1c9e08  -> aligned 0x1c9e10   (baseline)
0x1a3b78 + 0x26284 = 0x1c9dfc  -> aligned 0x1c9e00   (no pad3)
```

`.rodata` content is byte-identical in both builds (size `0x41b67`), so the
`0x10` base shift rewrites every pc-relative literal that points into
`.rodata`; that is the 4496 → 3931 `byte_exact` drop.

## 3. What `pad3` actually does to the LSDA

`render`'s LSDA rows around the `buttons[8]` block (row tuple is
`(call_site_start, length, landing_pad, action)`, offsets relative to
`0x1490d8`):

original:
```
(2854,   6, 9664)
(2874,  26, 9356)
(2912,   4, 9224)
(2918,  32, 9328)
(2992,   4, 9418)   <- getAmount() isolated, cleanup lp 9418
(3008, 154, 9588)   <- getIndex/getCurrentAmount/buttons[8]/buttons[9]/getWidth, lp 9588
(3174, 224, 9664)
```

baseline (with `pad3`):
```
(2876,  22, 9360)
(2910,  38, 9336)
(2988,  80, 9594)   <- getIndex/getCurrentAmount/setPosition(8)/setVisible(8)
(3076,   4, 9338)   <- buttons[8]->draw() isolated  (this is pad3)
(3114,  44, 9594)   <- setPosition(9)/setVisible(9)/draw(9)/getWidth
(3172, 226, 9662)
```

`pad3` removed:
```
(2876,  22, 9358)
(2910,  38, 9336)
(2988, 170, 9592)   <- getAmount .. getWidth all in one row
(3172, 226, 9662)
```

So `pad3` splits the **`draw()`** call (one row → three rows: +2 rows, +8 B
call-site table, +4 B action table = +12 B record). The original does **not**
split `draw()`; it splits the **`getAmount()`** call (a 4-byte row with a
distinct cleanup landing pad `9418`). `pad3` is therefore the wrong split, in
the wrong place, for the wrong reason.

The original's `getAmount()` isolation is systematic, not a one-off: the same
4-byte isolated row appears for both `getStationAmount`/`getAmount` pairs and
both `else`-branch `getAmount` calls:

| call site | original row | ours (baseline) |
|---|---|---|
| `getStationAmount` @ `0x149656` | `(1406, 4, 9418)` | merged into `(1404, 18, 9592)` |
| `getAmount` @ `0x149660` | `(1416, 4, 9588)` | merged into `(1404, 18, 9592)` |
| `getAmount` @ `0x149c88` | `(2992, 4, 9418)` | merged into `(2988, 80, 9594)` |
| `getStationAmount` @ `0x149e26` | `(3406, 4, 9512)` | merged |
| `getAmount` @ `0x149e30` | `(3416, 4, 9600)` | merged |
| `getAmount` @ `0x149f14` | `(3644, 4, 9512)` | merged |

Ghidra (`HangarWindow::render` at Ghidra `0x1590d8`) confirms the source shape is
the same as ours (`if (tab == 1) { getStationAmount; getAmount; } else {
getAmount; getIndex; getCurrentAmount; }`), so the isolation is an EH
cleanup-scope difference, not a different statement order.

## 4. `pad3` is interchangeable with any other 12-byte LSDA pad

Controlled A/B: delete `pad3`, insert the same `int padN = 0; (void) padN;`
after `buttons[9]->setVisible(true)` (or `buttons[7]->setVisible(true)`):

| variant | byte_exact | linked_exact | `.ARM.extab` | `.rodata` base | render size | rows | record |
|---|---:|---:|---|---|---:|---:|---:|
| baseline (`pad3` @ b8) | 4496 | 4496 | `0x26290` | `0x1c9e10` | `0x25e4` | 153 | 928 |
| no `pad3`, pad @ b9 | 4496 | 4496 | `0x26290` | `0x1c9e10` | `0x25e4` | 153 | 928 |
| no `pad3`, pad @ b7 | 4496 | 4496 | `0x26290` | `0x1c9e10` | `0x25e4` | 153 | 928 |

The row *contents* differ (b9 gives `(2988,146)` + `(3142,4)` + `(3154,4)`
instead of b8's `(2988,80)` + `(3076,4)` + `(3114,44)`), but the row *count* and
record *size* are identical, so the gate is identical. This is direct proof that
`pad3` is a size anchor, not a reconstruction.

## 5. Other proven body differences (the real shortfall)

`render`'s call sequence is identical to the original's except for these
measured differences (`objdump -d`, `blx` target sequence diff):

1. **3 extra `AbyssEngine::String::operator=` calls** (ours 21, original 18).
   They come from the three `description = description;` self-assignments at
   `HangarWindow.cpp:982-984`. Removing those three lines (keeping `pad3`)
   gives byte 3734, `.ARM.extab 0x2628c`, render `0x25c8`, 152 rows — a
   regression, so those lines are load-bearing for the current codegen.
2. **`getWidth`/`getHeight` order is swapped** in the free-credits loop
   (`HangarWindow.cpp:1071-1072`). Original:
   `0x14ad28 blx getWidth`, `0x14ad38 blx getHeight`. Ours:
   `0x14ad28 blx getHeight`, `0x14ad30 blx getWidth`. Swapping the two source
   lines to match the original gives byte 4367, `.ARM.extab 0x26290`, render
   `0x25e0`, 153 rows (render shrinks 4 B; still a regression on its own).
3. **Prologue global-load scheduling**: the original loads two pc-relative
   globals (`Globals::layout`, `Globals::Canvas`) before `SetColor` and loads
   `Globals::appManager` later; ours loads three at the top
   (`ldr r0/r1/r2 [pc]` + `add pc` ×3). This is the `3 ldr [pc]` vs `2 ldr [pc]`
   delta visible in `verify.py --show`.

`.ARM.extab` per-record deltas (`exidx_table.py --diff`) currently sum to −8:

```
ArrayAdd<KIPlayer>            orig 336 ours 328  -8
LevelScript::~LevelScript     orig 348 ours 352  +4
HangarWindow::render          orig 948 ours 928 -20
RecordHandler::readWanted     orig 656 ours 640 -16
Hud::drawChallengeModeScore   orig 476 ours 492 +16
StarMap::drawOnScreenInfo     orig 116 ours 128 +12
Generator::createAgent        orig 172 ours 176  +4
total -8 (-0x8)
```

Note: `exidx_table.py --diff` mislabels `render` as `render3D` because the
dynsym Thumb-bit values (`render` at `0x1490d9`, `render3D` at `0x1490c9`) are
not masked; the `-20` row is `render` (verified directly by `fn == 0x1490d8`).

Because the other six records already sum to `+12`, making `render`'s record
exact (−20 → 0) would overshoot the total to `+12` (`.ARM.extab 0x262a4`).
Restoring `.ARM.extab 0x26298` therefore needs body work in several of those
functions, not just `render`.

## 6. Conclusion

* `pad3` is a dead local that exists only to add 12 B to `render`'s LSDA record
  so `.ARM.extab` stays on the `0x26290` side of the 16-byte `.rodata`
  boundary. It is interchangeable with any other 12-byte pad.
* Deleting it alone is a **regression**: `byte_exact` 4496 → 3931,
  `.ARM.extab` 0x26290 → 0x26284, `.rodata` base 0x1c9e10 → 0x1c9e00.
* The real LSDA record is 948 B / 156 rows; ours is 928 / 153 with the pad and
  916 / 151 without. The real record is not restored by removing the pad.
* The real shortfall is body shape: the original isolates the
  `getStationAmount`/`getAmount` calls with distinct cleanup landing pads, has
  three fewer `String::operator=` calls, and orders `getWidth` before
  `getHeight`. Those are the reconstruction targets.

No source/flag change is a strict improvement, so only this note is committed.

## 7. Proven vs hypothesis

Proven (measured in this tree against the original binary):

* Baseline gate is byte 4496 / linked 4496 / `.text 0x1337c8` / `.ARM.extab
  0x26290` / `.rodata 0x1c9e10` / drift 0 / lint clean.
* Deleting `pad3` alone gives byte 3931 / linked 4496 / `.ARM.extab 0x26284` /
  `.rodata 0x1c9e00` / render `0x25e0` / 151 rows / record 916.
* `pad3` splits the `buttons[8]->draw()` call into its own row (+2 rows, +12 B
  record); the original instead isolates the `getAmount()` call with lp 9418.
* `pad3` at b8, a pad at b9, and a pad at b7 all give identical gate numbers
  (4496 / 4496 / `0x26290` / `0x1c9e10` / `0x25e4` / 153 rows / 928 B).
* Ours has 3 extra `String::operator=` calls (21 vs 18) from
  `description = description;` ×3.
* Ours orders `getHeight` before `getWidth` at `0x14ad28`; the original orders
  `getWidth` before `getHeight`.
* The `.rodata` base moves because `0x1a3b78 + 0x26284` rounds to `0x1c9e00`.

Hypothesis / still open:

* That the 565-function `byte_exact` loss is entirely the `.rodata` base shift
  (the section is otherwise byte-identical and `.text` size/base are unchanged;
  a per-function audit of which of the 565 reference `.rodata` was not run).
* That reproducing the original's `getStationAmount`/`getAmount` cleanup-scope
  isolation will add the missing rows. The exact source shape that produces the
  distinct landing pads (lp 9418 vs 9588) is not yet reconstructed; the
  `String` temporaries live at `sp+0x138` (lp 9418 destroys only this) and
  `sp+0x12c` (lp 9588 destroys both) and the compiler's cleanup state differs
  between the two calls even though the emitted code between them is only
  `ldr/mov/ldr`.
* That the `getWidth`/`getHeight` swap becomes a strict improvement once the
  surrounding body is closer (it currently shrinks `render` by 4 B).

## 8. Commands

```sh
# baseline
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# pad3 A/B: edit src/game/ui/HangarWindow.cpp:587, then
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
readelf -SW cmake-build-match/libgof2hdaa.so
objdump -T cmake-build-match/libgof2hdaa.so | grep HangarWindow6renderEv
python3 _work/reconstructions/exidx_table.py --diff \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

LSDA row dump used throughout (row tuple = start, length, landing pad, action,
offsets relative to `0x1490d8`):

```python
import sys
sys.path.insert(0, '_work/reconstructions')
import exidx_table as E
def table(so, fn=0x1490d8):
    p = E.parse(so); d = p['data']
    for r in p['entries']:
        if r['fn'] == fn:
            off = r['extab'] - p['secs']['.ARM.extab']['addr']
            b = d[p['secs']['.ARM.extab']['off'] + off + 7]
            return E.parse_lsda(d, p['secs']['.ARM.extab']['off'] + off + 4 + 4 * (1 + b))
for so in sys.argv[1:]:
    ls = table(so)
    print(so, len(ls['table']))
    for row in ls['table']:
        if 2800 <= row[0] <= 3450:
            print(row)
```

Record lengths come from `exidx_table.record_lengths()`; `render`'s record is
the distance from its `.ARM.extab` target to the next target.
