# MenuTouchWindow::OnTouchEnd (byte-exact) + MGame::OnUpdate residual — wave 94 (w4420)

Worktree `wt/w4420`, branch `wave94-w4420`, base master `f262bd4b`.
Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch drivers live in `/dev/shm/w4420` (`fdiff.py` raw-byte differ, `try_mg.sh`).

Build note: this is a git worktree, so the default `../_work` paths do not resolve.
Configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` receives no argument); call
`tools/verify/verify.py --show <mangled>` directly.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4499 / 4524 | **4500 / 4524** |
| linked_exact | 4499 / 4524 | **4500 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8).
The baseline was re-locked with `python3 tools/ratchet.py --update-baseline`; the only
changed rows are `byte_exact`/`linked_exact` 4499 -> 4500.

---

## 1. MenuTouchWindow::OnTouchEnd — 9 -> 0 differing bytes (byte-exact)

Target `_ZN15MenuTouchWindow10OnTouchEndEiiPv`, 8132 B @ 0x12aabc.
Baseline residual (wave 92, `menutouchwindow-ontouchend-r5-init-pass-w4245.md`):

```
+0x0487 (1B)  12af42  beq.w 12bf76 (orig)  vs  beq.w 12c062 (ours)
+0x1cdf (1B)  12c79c  ldr r0,[pc,#728]     vs  ldr r0,[pc,#724]
+0x1d9b (1B)  12c858  ldr r1,[pc,#536]     vs  ldr r1,[pc,#540]
+0x1fb7..b9   literal pool word 0x000b96da vs 0x000e365e
+0x1fbb..bd   literal pool word 0x000e365e vs 0x000b96da
```

Two independent source-shape changes fix all five runs.

### 1.1 The loadGame branch target: move the `__builtin_expect`

The wave-92 source was

```cpp
if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
    if (this->choiceWindow->hasChoice() &&
        __builtin_expect(this->loadGame(this->selectedRow) == 0, 0)) // anchor
        return 0;
}
```

The hint was on `loadGame() == 0` (unlikely).  The block-placement pass therefore put the
`return 0` block at 12af42 and the tail merge picked the startSupernova trampoline
(`movs r5,#0; ldr r6,[sp,#80]; b epilogue`), which rematerialises the canary pointer,
because the loadGame-return block has liveins `$r4,$r5,$r11` (no `$r6`).

Moving the same 2000:1 unlikely hint to the *other* conjunct — `hasChoice()` likely true —
gives the identical block layout but the loadGame return now merges with the epilogue
trampoline (12bf76, no `ldr r6`):

```cpp
if (__builtin_expect(this->choiceWindow->hasChoice(), 1) && // lint: arm_builtin anchor
    this->loadGame(this->selectedRow) == 0)
    return 0;
```

The 12af42 branch now targets 12bf76 and the `+0x0487` run disappears.  The anchor is
still load-bearing: the plain `hasChoice() && loadGame()==0` form grows the body
8132 -> 8148 (see §1.3).

### 1.2 The literal-pool swap: invert the two Google-Play `if/else` blocks

The remaining pool swap was between the `g_android_show_leaderboards` GOT word and the
`"No link with Google+"` string word.  The two source sites (case 23 / case 24) were

```cpp
if (g_android_gp_is_linked) {
    g_android_show_leaderboards = 1;
} else {
    this->choiceWindow->set(String("No link with Google+"), ...);
    ...
}
```

The const-pool order is the order the `tLDRpci` constants are created during ISel, which
follows the **IR block order**, not the final layout order.  For `if (cond) A else B`
clang emits the then-block A before the else-block B, so the leaderboard GOT word was
emitted before the string word.  The original has the string word first, i.e. its IR
block order is B then A.  Writing the condition negated puts the `"No link"` block in the
then-arm and the assignment in the else-arm:

```cpp
if (!g_android_gp_is_linked) {
    this->choiceWindow->set(String("No link with Google+"), ...);
    this->messageShowing = 1;
    this->genericConfirmB = 1;
} else {
    g_android_show_leaderboards = 1;
}
```

The branch is still emitted as `cmp r0,#0; beq <No-link>` with the leaderboards block as
the fall-through (identical machine code), but the two pool words are now emitted in the
original order, which also restores the two `ldr [pc,#N]` offsets.  The same inversion is
applied to the `g_android_show_achievements` twin (case 24).

### 1.3 Variant table (official verifier; raw differing bytes)

| variant | loadGame hint | GP if/else | size | diff bytes |
|---|---|---|---:|---:|
| baseline | `hasChoice() && expect(loadGame==0,0)` | `if (linked) A else B` | 8132 | 9 |
| V1 | `expect(hasChoice(),1) && loadGame==0` | `if (linked) A else B` | 8132 | 8 |
| **landed** | **`expect(hasChoice(),1) && loadGame==0`** | **`if (!linked) B else A`** | **8132** | **0** |
| V2 | `hasChoice() && expect(loadGame==0,0)` | `if (!linked) B else A` | 8132 | 1 |
| V3 | `expect(hasChoice() && loadGame==0,0)` | `if (!linked) B else A` | 8132 | 1 |
| V4 | plain `hasChoice() && loadGame==0` | `if (!linked) B else A` | 8148 | 6645 |
| V5 | `hasChoice() && expect(loadGame==0,1)` | `if (!linked) B else A` | 8148 | 6642 |
| V6 | `expect(hasChoice(),0) && ...` | `if (!linked) B else A` | 8140 | 6603 |

The landed shape is the unique 8132/0 combination.  The `__builtin_expect(hasChoice(),1)`
anchor is kept and documented on its own line (`// lint: arm_builtin anchor: ...`), because
the plain source form regresses the body by 16 bytes.

### 1.4 What is proved

* The 12af42 branch-target residual is **not** a tail-merge/block-count property: it is the
  placement of the branch-weight hint.  `expect(loadGame==0,0)` and
  `expect(hasChoice(),1)` give the same layout but different tail-merge groups.
* The literal-pool swap is **not** a consequence of the branch target.  It is the IR
  block order of the `if (g_android_gp_is_linked)` sites: inverting the condition (same
  machine code) flips the order in which the two `tLDRpci` constants are created.
* With both changes the function is byte-identical: 8132 B, 0 differing bytes.

---

## 2. MGame::OnUpdate — 41-byte `Status*` cache remains

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8.  Baseline residual is one 41-byte run
at `+0x17ba..+0x17e2` (absolute 0x17e093..0x17e0bb):

```
original                                   ours
17e08c ldr.w r9,[r0]   ; r9=&Globals::status  17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; Status*              17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission          17e094 mov r0,r5
17e0a8 ldr.w r0,[r9]                          17e0a8 ldr.w r0,[r9]
17e0ac blx getMission                         17e0ac blx getMission
17e0b8 ldr.w r0,[r9]                          17e0b8 mov r0,r5
17e0bc blx getPlayingTime                     17e0bc blx getPlayingTime
17e0c0 ldr.w r2,[r9]                          17e0c0 ldr.w r2,[r9]
```

The original has **no cached `Status*`**: all four accesses are fresh `ldr [r9]` loads.
Our reconstruction carries `Status *st = Globals::status;` used at `getPlayingTime()`, and
clang CSEs the first access with it, producing the `ldr r5,[r9]` + two `mov r0,r5`.

### 2.1 The two obvious shapes and why neither is exact

* **Keep `st`** (current): 11440 B, 41 differing bytes.  `st` occupies r5, which forces
  `&Globals::status` into r9 (matching the original's register) — but the two `mov`s differ.
* **Remove `st`** (all `Globals::status->`): 11392 B, 5149 differing bytes / 212 runs.
  `&Globals::status` is now free to take r5 (a 2-byte `ldr` base instead of the 4-byte
  `ldr.w` r9 form), which shrinks the whole region by 48 B and re-colours the rest of the
  function.

So the current `st` local is a size anchor that buys the correct r9 allocation at the cost
of the two `mov`s.  The original almost certainly has no `st`.

### 2.2 Variant sweep (all 16 `st`-use masks + structural forms)

`st` is `Status *st = Globals::status;`; the mask selects which of the four accesses
(`ccm`, `mission`, `pt`, `field_0x100`) go through `st`:

| mask | ccm | mission | pt | field | size | diff bytes |
|---:|:-:|:-:|:-:|:-:|---:|---:|
| 0 | - | - | - | - | 11392 | 5149 |
| 1 | x | - | - | - | 11392 | 5149 |
| 2 | - | x | - | - | 11396 | 5130 |
| 4 | - | - | x | - | **11440** | **41** |
| 5 | x | - | x | - | 11440 | 41 |
| 6 | - | x | x | - | 11436 | 4983 |
| 8 | - | - | - | x | 11436 | 4994 |
| 12 | - | - | x | x | 11436 | 4982 |
| 15 | x | x | x | x | 11432 | 5034 |

`mask 4` and `mask 5` are the same IR (the first access is CSE'd either way).  Every other
mask is worse.  Other forms measured:

| variant | size | diff bytes |
|---|---:|---:|
| `Status *&st = Globals::status;` all four `st->` | 11392 | 5149 |
| `Status **st = &Globals::status;` all four `(*st)->` | 11392 | 5149 |
| `st` assigned inside the condition (`(st = Globals::status)->...`) | 11392 | 5149 |
| `st` declared in the body, condition uses `Globals::status` | 11392 | 5149 |
| `long long last = ...field_0x100;` local | 11432 | 5041 |
| `__builtin_assume(st != 0)` / `(void) st` before the condition | 11392 | 5149 |
| `*(Status *volatile *)&Globals::status` at the first access | 11440 | 853 |
| `Status *st` used only at the later mission-24 `getPlayingTime` | 11440 | 270 |
| `Status *st` used at the >=122 `getPlayingTime` | 11440 | 540 |

### 2.3 Flag sweep (neutral)

Per-TU `-mllvm` options on `MGame.cpp` (current `st` shape):

| flag | size | diff bytes |
|---|---:|---:|
| `-spp-rematerialization-threshold=0` | 11440 | 41 |
| `-regalloc-csr-first-time-cost=0` | 11440 | 41 |
| `-regalloc-csr-first-time-cost=1000` | 11440 | 820 |
| `-enable-deferred-spilling` | 11440 | 335 |

The same flags on the no-`st` shape stay 11392/5149 (or worse).  No flag produces
11440/0.

### 2.4 What is proved / what remains

Proved: the 41 bytes are exactly the `ldr r5,[r9]; mov r0,r5` caching of `Status*`; the
original has four fresh `ldr [r9]` loads and keeps `&Globals::status` in r9.  The `st`
local is required to reproduce the r9 choice (without it clang picks r5 and the whole
function re-colours), but it necessarily introduces the two `mov`s.

Remaining: a source shape that makes clang keep `&Globals::status` in r9 while leaving the
four `Status*` loads uncached.  The greedy allocator's r5-vs-r9 choice for
`&Globals::status` is a whole-function property (the same GOT value is r5 at 17d7dc and r9
at 17e08c in the original), and every source spelling tried here either caches the load or
moves the base to r5.

---

## 3. Conclusion

* **MenuTouchWindow::OnTouchEnd**: byte-exact (8132 B, 0 diff).  Two source-shape fixes:
  move the branch-weight hint from `loadGame()==0` to `hasChoice()`, and invert the two
  Google-Play `if (g_android_gp_is_linked)` blocks.  Gate 4499 -> 4500 byte/linked.
* **MGame::OnUpdate**: still 41 bytes (99.9%).  The residual is the `Status*` cache; no
  source shape or flag found that reproduces the original's uncached four-load form with
  `&Globals::status` in r9.
