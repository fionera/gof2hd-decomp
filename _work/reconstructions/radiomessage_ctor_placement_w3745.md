# Section-order removal: `RadioMessage` ctor placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master `df7bdb79`
("docs: lock HackingGame emission order"). Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`). All numbers below were produced in this worktree with the
commands shown. Scratch output is under `/tmp/w3745_rm/`.

This note closes the next open item after
`_work/reconstructions/section_order_removal_w3745.md` §17 and
`_work/reconstructions/hackinggame_dtor_placement_w3745.md`: the first plain
(no `--section-ordering-file`) mismatch at this HEAD is
`RadioMessage::RadioMessage(int, int, int, int)` at `0x15158c` (ours
`0x1515f2`). Reordering the `RadioMessage.cpp` definitions to the original
binary's address order makes the whole `RadioMessage` TU address-exact in the
plain link. The source change is gate-neutral.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `df7bdb79`) | **4497 / 4497** | **3132** | **3439 / 4524** | **3283** | `RadioMessage::RadioMessage(int,int,int,int)` `0x15158c` vs `0x1515f2` |
| after (this pass) | **4497 / 4497** | **2852** | **3456 / 4524** | **3305** | `BoundingSphere::collide(float,float,float)` `0x151d68` vs `0x151ee8` |

Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @ 0x69440`,
`.rodata` 0 differing bytes, lint `CLEAN ✓`, drift `0`.

The plain `byte_exact` drop (3132 -> 2852) is **not** a code or ordering
regression: every one of the 286 lost functions differs only by a `+0x10`
literal-pool delta caused by the plain link's `.rodata` base moving from
`0x1c9e10` to `0x1c9e20` (see §5). The 6 gained functions are the
`RadioMessage` 4-arg/5-arg constructors and the destructor, which moved onto
their original addresses.

The entire `RadioMessage` block is now at the original addresses in the plain
link:

| symbol | original | before | after |
|---|---:|---:|---:|
| `RadioMessage::RadioMessage(int,int,int,int)` | `0x15158c` | `0x1515f2` | **`0x15158c`** |
| `RadioMessage::reset()` | `0x1515bc` | `0x15166c` | **`0x1515bc`** |
| `RadioMessage::RadioMessage(int,int,int,int,int)` | `0x1515c4` | `0x15158c` | **`0x1515c4`** |
| `RadioMessage::RadioMessage(int,int,Objective*)` | `0x15162a` | `0x151622` | **`0x15162a`** |
| `RadioMessage::~RadioMessage()` | `0x151650` | `0x151648` | **`0x151650`** |
| `RadioMessage::getSoundID()` | `0x151666` | `0x15168e` | **`0x151666`** |
| `RadioMessage::getTextID()` | `0x15166a` | `0x151686` | **`0x15166a`** |
| `RadioMessage::getImageID()` | `0x15166e` | `0x15168a` | **`0x15166e`** |
| `RadioMessage::setRadio(Radio*)` | `0x151672` | `0x151682` | **`0x151672`** |
| `RadioMessage::isTriggered()` | `0x151676` | `0x15167c` | **`0x151676`** |
| `RadioMessage::isOver()` | `0x15167c` | `0x151666` | **`0x15167c`** |
| `RadioMessage::finish()` | `0x151682` | `0x15165e` | **`0x151682`** |
| `RadioMessage::trigger()` | `0x15168a` | `0x151674` | **`0x15168a`** |
| `RadioMessage::triggered(long long,PlayerEgo*,LevelScript*)` | `0x151694` | `0x151694` | `0x151694` |

Every `RadioMessage` symbol is `byte=True` in the plain verify report; the
next plain mismatch is the next TU (`BoundingSphere::collide`).

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The brief's `4496 / 4496` baseline is stale; this HEAD is `4497 / 4497`.

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_rm/noorder_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_rm/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_rm/noorder_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_rm/noorder_base.so
```

```text
byte_exact 3132  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3439/4524 same; prefix 3283;
            first (0x15158c, '_ZN12RadioMessageC1Eiiii', 0x1515f2)
```

## 2. Original order (binary witness)

`nm -nSD --defined-only` on the original gives the emitted `RadioMessage`
order:

```text
0015158c  RadioMessage::RadioMessage(int,int,int,int)         0x30
001515bc  RadioMessage::reset()                               0x08
001515c4  RadioMessage::RadioMessage(int,int,int,int,int)     0x66
0015162a  RadioMessage::RadioMessage(int,int,Objective*)      0x26
00151650  RadioMessage::~RadioMessage()                       0x16
00151666  RadioMessage::getSoundID()                          0x04
0015166a  RadioMessage::getTextID()                           0x04
0015166e  RadioMessage::getImageID()                          0x04
00151672  RadioMessage::setRadio(Radio*)                      0x04
00151676  RadioMessage::isTriggered()                         0x06
0015167c  RadioMessage::isOver()                              0x06
00151682  RadioMessage::finish()                              0x08
0015168a  RadioMessage::trigger()                             0x08
00151694  RadioMessage::triggered(long long,PlayerEgo*,LevelScript*) 0x694
```

The HD source-layout dump
(`_work/original_layout/original_layout.json`, `Classes/Game/RadioMessage.cpp`)
independently witnesses the 4-arg constructor first and the accessor order
`getTextID, getImageID, setRadio, isTriggered, isOver, finish, trigger,
triggered`; the Android binary additionally emits `reset`, `getSoundID` and the
3-arg constructor, and is the authority for their positions.

## 3. Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/core/RadioMessage.cpp.obj`
(section-header order = emission order) before the change:

```text
RadioMessage::RadioMessage(int,int,int,int,int)
RadioMessage::RadioMessage(int,int,int,int)
RadioMessage::RadioMessage(int,int,Objective*)
RadioMessage::~RadioMessage()
RadioMessage::finish()
RadioMessage::isOver()
RadioMessage::reset()
RadioMessage::trigger()
RadioMessage::isTriggered()
RadioMessage::setRadio(Radio*)
RadioMessage::getTextID()
RadioMessage::getImageID()
RadioMessage::getSoundID()
RadioMessage::triggered(long long,PlayerEgo*,LevelScript*)
```

This is exactly the source order. `RadioMessage.cpp` has no cross-function
references among its own strong functions except `triggered`'s call to
`Radio::setCurrentMessage` (another TU), so clang emits the strong functions in
definition order. The original order is therefore reproduced by reordering the
definitions; no body edit, flag, pragma or anchor is involved.

## 4. Source change

Only change: reorder the top-level definitions in
`src/game/core/RadioMessage.cpp` to the original address order:

```text
before (source / object order):
  ctor(5), ctor(4), ctor(3), dtor, finish, isOver, reset, trigger,
  isTriggered, setRadio, getTextID, getImageID, getSoundID, triggered

after:
  ctor(4), reset, ctor(5), ctor(3), dtor, getSoundID, getTextID,
  getImageID, setRadio, isTriggered, isOver, finish, trigger, triggered
```

The bodies are unchanged. `reset` sits between the 4-arg and 5-arg
constructors, exactly as in the original; its body is the same three stores the
constructors already inline, so this is a pure placement change.

## 5. Controlled A/B

Each row is a full `check` rebuild plus the plain no-order relink.

| # | source | gate byte/linked | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | **3132** | 3439 | 3283 | `RadioMessage::RadioMessage(int,int,int,int)` `0x15158c` vs `0x1515f2` |
| 1 | full `RadioMessage` reorder (landed) | **4497 / 4497** | **2852** | **3456** | **3305** | `BoundingSphere::collide` `0x151d68` vs `0x151ee8` |
| 2 | row 1 + `BoundingSphere` reorder (scratch, not landed) | **4497 / 4497** | 2855 | 3461 | 3320 | `Gun::setOffset` `0x151f8c` vs `0x151fec` |

Row 2 is included only to show the next TU is independent; it was reverted
because `BoundingSphere` is the next frontier, not this pass.

Commands (same for every row):

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_rm/noorder_final.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_rm/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_rm/noorder_final.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_rm/noorder_final.so
```

Final gate:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Final plain no-order:

```text
byte_exact 2852  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3456/4524 same; prefix 3305;
            first (0x151d68, '_ZN14BoundingSphere7collideEfff', 0x151ee8)
```

### 5.1 Gate `.so` A/B

The gate `.so` is byte-identical in `.text` and `.rodata`; only the build-id
and the `.ARM.exidx`/`.ARM.extab` record order for the reordered functions
change:

| section | before | after | differing bytes |
|---|---:|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` | 0 |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` | 0 |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` | 2 |
| `.ARM.extab` | `0x2628c @ 0x1a3b78` | `0x2628c @ 0x1a3b78` | 2 |
| `.note.gnu.build-id` | — | — | 20 |

`cmp -l` gives 24 differing bytes total (20 build-id + 2 + 2). All other
sections are byte-identical.

### 5.2 Why plain `byte_exact` drops: `.rodata` base alignment

Plain-link section layout:

| section | original | before | after |
|---|---:|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337e8 @ 0x69440` | `0x1337e8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f58 @ 0x19cc28` | `0x6f60 @ 0x19cc28` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x2628c @ 0x1a3b80` | `0x2628c @ 0x1a3b88` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e20` |

The reorder gives `reset` its own `.ARM.exidx` entry (it is separated from the
other no-unwind accessors by the 5-arg and 3-arg constructors), so the plain
`.ARM.exidx` grows by 8 bytes (`0x6f58 -> 0x6f60`). The plain `.text` is
already `+0x20` vs the original (the known empty-`.text`/marker artifact), so
the raw `.rodata` start crosses the 16-byte alignment boundary:
`0x19cc28 + 0x6f60 + 0x2628c = 0x1c9e14 -> 0x1c9e20`.

Every one of the 286 functions that lose `byte_exact` differs only by that
global `+0x10` shift. The check used:

```python
# for each function that was byte=True before and byte=False after:
# compare the raw 4-byte words at the same original address
ds = sorted(set(uw - ow for each differing word))
# result: 286 / 286 functions have ds == [0x10], 0 exceptions
```

So the drop is a link-layout side effect, not a change in any function body,
call target or ordering. The `RadioMessage` block itself is byte-exact.

## 6. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.
* Baseline plain no-order is `byte 3132 / linked 4497`, `3439/4524` addresses,
  prefix `3283`; first mismatch `RadioMessage::RadioMessage(int,int,int,int)`
  at `0x15158c` (ours `0x1515f2`).
* The original emits the `RadioMessage` functions in the order listed in §2;
  the baseline object emits them in source order, which differs.
* Reordering the definitions is gate-neutral (`4497/4497`, `.text` and
  `.rodata` byte-identical, lint `CLEAN`, drift 0) and makes every
  `RadioMessage` symbol address-exact in the plain link.
* The plain link's next mismatch is `BoundingSphere::collide`
  (`0x151d68` original, `0x151ee8` ours).
* The 286 plain `byte_exact` losses are all `+0x10` literal-pool deltas from
  the `.rodata` base moving `0x1c9e10 -> 0x1c9e20`; there are no other
  differing words.
* The `.ARM.exidx` growth by 8 bytes is the `reset` entry, which the original
  also has at `0x1515bc`.

**Hypothesis / still open**

* The plain `.text` `+0x20` and `.ARM.extab` `-0xc` are the known plain-link
  artifacts; until they are fixed, any TU reorder that adds an exidx entry can
  cross the `.rodata` 16-byte alignment boundary and depress the raw
  `byte_exact` count. The exact-address count and prefix are the more stable
  frontier metric for this campaign.
* The next plain frontier is `BoundingSphere.cpp`: our object order is
  `ctor, dtor, update, projectCollisionOnSurface, outerCollide,
  getCollisionNormal, collide`; the original is `ctor, dtor, collide,
  outerCollide, getCollisionNormal, update, projectCollisionOnSurface`.
  A scratch reorder of that TU gives plain `2855 / 3461 / 3320` and moves the
  frontier to `Gun::setOffset`; it was not landed here.
* The ordering file is still required: the plain link is `2852`, not `4497`.
