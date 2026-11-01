# `.rodata` -16 B: the DialogueWindow table region

Status: research note + landed source fix (2026-11-01, worker `w3416`, branch `wave87-w3416`,
base `bd9125f0`). All numbers below were measured in this worktree with
`cmake --build cmake-build-match --target check -j8` unless stated otherwise.

## Result in one line

The DialogueWindow TU's plain `.rodata` is a single **0x290c-byte** block that is now
**byte-identical** to the original (`orig 0x2037b0..0x2060bc` vs `ours 0x2037a8..0x2060b4`).
The residual `-0x10` `.rodata` size delta is **not** in DialogueWindow: it is an `+8` base shift
produced by the two TUs before it (ObjectGun `-0x20`, Hud `+0x28`), which is then rounded up to
`+0x10` by the 16-byte alignment of `PlayerGasCloud`'s `.rodata`.

Two source changes were needed (both in `src/game/ui/DialogueWindow.cpp`):

1. the two `static const char ...[ ] = ""` objects were replaced by direct `String("", false)`
   calls (the original's empty string lives in the merge pool at `0x1ca472`, not in plain
   `.rodata`);
2. `g_dw_successScript` is `[2060]`, not `[2056]` — the original TU has 16 zero bytes after the
   last referenced success entry (indices 2056..2059 are never read, see below).

## Gate measurements

| tree state | byte_exact | linked_exact | `.rodata` size | TU `.rodata` | TU `.rodata.str1.1` | first content diff |
|---|---|---|---|---|---|---|
| baseline (`bd9125f0`) | 4171 | 4495 | 0x41b57 (orig 0x41b67, -0x10) | 0x2904 | 0x13 | 0x1fb590 |
| exp 1: empty-string statics removed | **4173** | 4495 | 0x41b47 (-0x20) | 0x28fc | 0x14 | 0x1fb590 |
| exp 1 + exp 2: `successScript[2060]` | **4173** | 4495 | 0x41b57 (-0x10) | **0x290c** | 0x14 | 0x1fb590 |

In all three states: `compared 4524`, `stubs 0`, `missing 0`, `extra 0`, `avg 99.89%`,
`data: exact 843 of 843`, `sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207`,
lint `CLEAN ✓`, drift `0 field(s) across 0 class(es)`, `RATCHET PASS`.

The ratchet's only reported gain is `byte_exact 4171 -> 4173` (the two
`DialogueWindow::DialogueWindow(String*, String*, int*)` C1/C2 rows). Exp 2 does not move the
function-level gate count (the six remaining DialogueWindow rows are literal-pool-only deltas,
see below), but it makes 10,508 bytes of `.rodata` content match exactly and is landed with this
note.

## The block, symbol by symbol

Original VMAs were recovered by resolving the `ldr rX,[pc,#N]` / `add rX,pc` literal-pool words
in the original's `init`, `hasBriefingDialogue`, `hasSuccessDialogue`, `length` and
`loadContent` (all at the same `.text` addresses as ours, because `.text` is size- and
order-exact). Our VMAs are from the unstripped link (`arm-linux-androideabi-nm`).

| symbol | TU offset | orig VMA | ours VMA (exp1+2) | ours VMA (baseline) |
|---|---|---|---|---|
| `g_dw_briefingCounts[0xa2]` (0x288) | 0x0000 | 0x2037b0 | 0x2037a8 | 0x2037a8 |
| `g_dw_successCounts[0xa2]` (0x288) | 0x0288 | 0x203a38 | 0x203a30 | 0x203a30 |
| `g_dw_briefingScript[120]` (0x1e0) | 0x0510 | 0x203cc0 | 0x203cb8 | 0x203cb8 |
| `g_dw_successScript[2060]` (0x2030) | 0x06f0 | 0x203ea0 | 0x203e98 | 0x203e98 (`[2056]`, 0x2020) |
| `g_dw_station108ScriptLate[36]` (0x90) | 0x2720 | 0x205ed0 | 0x205ec8 | 0x205ec8 |
| `g_dw_station108Script[12]` (0x30) | 0x27b0 | 0x205f60 | 0x205f58 | 0x205f58 |
| `g_dw_genericBriefingTexts[5]` (0x14) | 0x27e0 | 0x205f90 | 0x205f88 | 0x205f88 |
| `g_dw_genericSuccessTexts[5]` (0x14) | 0x27f4 | 0x205fa4 | 0x205f9c | 0x205f9c |
| `g_dw_genericOtherTexts[5]` (0x14) | 0x2808 | 0x205fb8 | 0x205fb0 | 0x205fb0 |
| `g_dw_germanBriefingTexts[20]` (0x50) | 0x281c | 0x205fcc | 0x205fc4 | 0x205fc4 |
| `g_dw_germanSuccessTexts[20]` (0x50) | 0x286c | 0x20601c | 0x206014 | 0x206014 |
| `g_dw_germanOtherTexts[20]` (0x50) | 0x28bc | 0x20606c | 0x206064 | 0x206064 |
| end of TU `.rodata` | 0x290c | 0x2060bc | 0x2060b4 | 0x2060ac |

The original's `init` loop resolves `g_dw_briefingCounts` to **0x2037b0** and
`g_dw_successCounts` to **0x203a38**; `loadContent` resolves `g_dw_station108ScriptLate` to
**0x205ed0** (`0x167826 + pool word 0x9e6a6`) and `g_dw_station108Script` to **0x205f60**
(`0x167cde + pool word 0x9e27e`). Ours are the same values minus 8.

Byte comparison with the exp1+2 object:

```
orig[0x2037b0 .. 0x2060bc] == ours[0x2037a8 .. 0x2060b4]   -> True  (0x290c bytes)
orig[0x2060bc .. 0x2068d8] == ours[0x2060b4 .. 0x2068d0]   -> True  (0x81c bytes, ModMainMenu etc.)
orig[0x2068e0 .. 0x20b977] == ours[0x2068d0 .. 0x20b967]   -> True  (rest of .rodata, +0x10 shift)
```

So after the DialogueWindow TU the *only* remaining `.rodata` content difference is the `+8`
shift (which is a size difference, not a content difference) until the end of the section.

## Why the block was 8 bytes early: it is not a DialogueWindow problem

The original's block starts at `0x2037b0`, ours at `0x2037a8`. The 8 bytes at `0x2037a8..0x2037b0`
in the original are the last int of **Hud's** `.rodata` (`ed 00 00 00` = 237, the last element of
`SECURITY_COLORS_RGB`); the same int exists in ours at `0x2037a0..0x2037a8`. The shift comes from
the two TUs before DialogueWindow:

* **ObjectGun** (`o168`): the original's plain `.rodata` is **0x20 shorter** than ours. The
  `.rodata._ZTS11AbstractGun` input section (14 B) sits at `0x203747` in the original and at
  `0x203767` in ours; the section base (`0x2033b8`) is the same, so ObjectGun's own `.rodata`
  content is 0x20 bytes shorter (its content also differs, not this worker's topic).
  `ObjectGun -0x20`.
* **Hud** (`o170`): from the end of the AbstractGun typeinfo to the DialogueWindow block the
  original has **0x58** bytes of plain `.rodata`: `{1190, 1187, 1189, 1188}` (the
  `RACE_IMAGES_BIG` values) followed by six `-1` ints, then `SECURITY_COLORS_RGB[4][3]` (0x30),
  ending at `0x2037b0`. Ours has only **0x30** (`SECURITY_COLORS_RGB`, ending at `0x2037a8`),
  because `g_Hud_raceBadge[4]` is emitted into `.rodata.cst16` (merge constants) and never lands
  in plain `.rodata`. This region is `+0x28`.
* Net at DialogueWindow: `+0x28 - 0x20 = +0x8`. (The exact original section split between
  ObjectGun and Hud inside the 0x58-byte region is not observable in the stripped binary; the
  bytes and the net size are.)
* At `PlayerGasCloud` (`o180`, `.rodata` aligned 16) the `+8` rounds up to `+16`, which is exactly
  the total `.rodata` size delta `0x41b67 - 0x41b57 = 0x10`.

Consequence: making Hud's plain `.rodata` 0x28 bytes larger (and fixing ObjectGun) would move the
whole DialogueWindow block to its original VMAs, which would also turn the six remaining
non-byte-exact DialogueWindow functions byte-exact, because their only differences are the
literal-pool words holding these addresses.

## Why the two source changes are correct

### 1. Empty-string statics -> `String("", false)`

The original's `init` loads `String(const char*, bool)`'s first argument from the merge pool:
`ldr r1,[pc,#536]` at `0x166cb6`, `add r1,pc` at `0x166cbe` ->
`0x166cc2 + 0x637b0 = 0x1ca472`, inside the `** merge strings` group at
`0x1c9e10..0x1e7569`. That group is byte-identical to ours (the first `.rodata` diff is at
`0x1fb590`, far later), so the original's empty string is at `0x1ca472` in both.

The baseline decompiled `init`/the `String*,String*,int*` ctor through two named 1-byte arrays
(`g_dw_defaultAgentName`, `g_dw_emptyString`) which clang emitted in plain `.rodata` (at TU
offsets 0x510 and 0x2714, each padded to 4 bytes). Replacing them with direct `""` literals
merges the empty string into `.rodata.str1.1` (0x13 -> 0x14 bytes) and removes 8 bytes from
plain `.rodata`; the two `DialogueWindow(String*,String*,int*)` ctors become byte-exact
(`byte_exact 4171 -> 4173`).

### 2. `g_dw_successScript[2060]`

The original's TU `.rodata` is 0x290c bytes, and the original has 16 zero bytes at
`0x205ec0..0x205ed0`, immediately before `g_dw_station108ScriptLate` (0x205ed0). Those bytes are
**not** the empty string: the empty string is at `0x1ca472`, inside the first merge group
(`0x1c9e10..0x1e7569`), which is byte-identical to ours (the first `.rodata` diff is at
`0x1fb590`). They are 4-byte-aligned and exactly 16 bytes, so they cannot be a 1-byte literal
plus alignment padding; they are 16 bytes of plain `.rodata` from the DialogueWindow TU.

`g_dw_successCounts` sums to 2056 (`sum(g_dw_successCounts) == 2056`), and `loadContent` indexes
`successOffsets[mission] + (page*2|1)`, whose maximum is 2055. So indices 2056..2059 are
unreferenced. Declaring the table `[2060]` with four trailing zeros reproduces the original's
bytes exactly and makes the whole 0x290c block byte-identical. The iOS symbol dumps list a single
`_CAMPAIGN_SUCCESS` symbol (no separate tail array), which is consistent with the 16 bytes being
part of that array.

## The six remaining non-byte-exact DialogueWindow functions

`loadContent`, `hasSuccessDialogue`, `hasBriefingDialogue`, `init`, `length`,
`pickGermanGenericTextBecauseWeSaved100EurosWithThat` are all `linked=True`, `pct=100.0`, i.e.
their instruction streams are identical and only the literal-pool words differ. Example
(`hasBriefingDialogue`, `0x1671b4..0x1671f4`):

```
15c15
< 1671d0: c5ec   stmia r5!, {r2, r3, r5, r6, r7}   ; pool word 0x9c5ec -> 0x2037b0
> 1671d0: c5e4   stmia r5!, {r2, r5, r6, r7}        ; pool word 0x9c5e4 -> 0x2037a8
```

`init` differs only at `0x166ec8`/`0x166ecc` (the `successCounts`/`briefingCounts` pool words).
They become byte-exact automatically once the block base is fixed by Hud/ObjectGun.

## Exact commands

```
cmake --preset default
cmake --build cmake-build-match --target check -j8     # gate (baseline / exp1 / exp1+2)
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift
# map of the current link: append -Map=/tmp/w3416_exp2.map to the ld.gold COMMAND of the
# `link_so` rule in cmake-build-match/build.ninja (and skip the trailing strip so nm works):
/tmp/linkcmd.sh /tmp/w3416_exp2.so -Map=/tmp/w3416_exp2.map
```

Section sizes: `readelf -SW cmake-build-match/libgof2hdaa.so` and
`arm-linux-androideabi-readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/DialogueWindow.cpp.obj`.
Symbol VMAs: `arm-linux-androideabi-nm` on the unstripped `/tmp/w3416_exp2.so` (the build-tree
`.so` is stripped). Original literal-pool words were resolved with the NDK
`arm-linux-androideabi-objdump -d -j .text` plus a small Python `ldr [pc,#N]`/`add pc` resolver
(`target = add_insn_addr + 4 + pool_word`).

## Proven vs hypothesis

**Proven (measured):**
* The original DialogueWindow TU `.rodata` block is `0x2037b0..0x2060bc` (0x290c) and is
  byte-identical to ours after the two changes.
* The original's `init`/`loadContent` literal pools resolve to 0x2037b0/0x203a38/0x205ed0/0x205f60,
  i.e. exactly 8 bytes above ours.
* The region between the end of the AbstractGun typeinfo and the DialogueWindow block is 0x58
  bytes in the original (`{1190,1187,1189,1188}` + six `-1` + 0x30 colour table) and 0x30 in
  ours.
* The original's ObjectGun plain `.rodata` is 0x20 shorter than ours.
* The `+8` net shift is rounded to `+0x10` at the 16-byte-aligned `PlayerGasCloud` `.rodata`,
  which is the whole `.rodata` size delta.
* Exp 1 alone is `byte_exact 4171 -> 4173`; exp 2 alone does not change the function-level gate.

**Hypothesis (not directly recoverable from the stripped binary):**
* That the original source declared `CAMPAIGN_SUCCESS` as a single 2060-entry array rather than a
  2056-entry array plus a separate 16-byte zero object. The bytes are identical either way; the
  iOS symbol list (one `_CAMPAIGN_SUCCESS`, no tail symbol) favours the 2060-entry reading.
* The exact source split of Hud's 0x58 bytes (one `RACE_IMAGES_BIG[10]` vs `RACE_IMAGES_BIG[4]`
  plus a 6-int `-1` array). The bytes and the total size are proven; the declaration shape is not.

## Doc correction

`docs/IDENTICALITY.md` and `_work/campaign.md` stated the first `.rodata` content diff as
`0x1e9f90` (and the prose said `0x1f9ffb`). Both are wrong: measured here, the first differing
byte is **VMA 0x1fb590** (offset 0x31780, the `Generator.cpp` table). `0x1e9f90` and `0x1f9ffb`
are byte-identical between orig and ours. The docs are corrected in the same commit as this note.
