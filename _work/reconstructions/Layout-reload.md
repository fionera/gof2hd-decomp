# `Layout::reload` reconstruction checkpoint

Target: `_ZN6Layout6reloadEv`

- Original: `0xd2c20`, 1064 bytes, 348 decoded instructions.
- Landed partial: 952 bytes, 319 decoded instructions, 61.5%.
- Previous state: 896 bytes, 52.6%.
- Original TU: `Classes/Game/Layout.cpp`.
- Current TU: `src/game/ui/Layout.cpp`.

## Verified improvements

The former `TouchButton_ctorStr`, `TouchButton_ctorImg`, and
`TouchButton_ctorImg2` undefined shims were not original callees. The original
calls real `TouchButton` constructors:

| Call | Constructor | Verified arguments |
|---|---|---|
| `0xd2ee8` | `_ZN11TouchButtonC1ERKN11AbyssEngine6StringEiiih` | text `0xaa`, kind `2`, `this+0x28`, `Globals::h-3`, flags `0x21` |
| `0xd2f34` | `_ZN11TouchButtonC1Ejiiih` | image `0x535`, kind `2`, `this+0x28`, `Globals::h-this+0x3fc`, flags `0x21` |
| `0xd2f5c` | `_ZN11TouchButtonC1ERKN11AbyssEngine6StringEiiih` | text `0xab`, kind `2`, `this+0x28`, `Globals::h-this+0x3fc`, flags `0x21` |
| `0xd2fa2` | `_ZN11TouchButtonC1Ejiiiihh` | image `0x471`, kind `1`, `Globals::w`, `0`, `this+0x3c`, `0x12`, `0x04` |

All allocations are 200 bytes (`sizeof(TouchButton)`). Clang already hoists
the secondary button's common allocation before its image-validity branch;
no direct or placement `operator new` call is needed.

The original `GetTextHeight` argument comes from GOT `0x21029c`,
`_ZN7Globals4fontE`. The tail also proves paired-byte zero stores:

- `fading` and `fadeOut` at `Layout+0x400`;
- `rewardMessageActive` and `rewardMessageFlag` at `Layout+0x2ec`.

These semantics are present in the landed partial.

## Remaining structural mismatch

The original prologue saves `r8-r11` and allocates an 84-byte frame. The
landed partial uses an effective 28-byte frame. Before the iPad branch, the
original computes and spills the following output addresses:

| Location | Destination | Resource |
|---|---:|---:|
| `sp+24` | `this+0x360` | `0x530` |
| `sp+28` | `this+0x364` | `0x531` |
| `sp+32` | `this+0x368` | `0x52f` |
| `sp+36` | `this+0x384` | `0x47c` |
| `sp+40` | `this+0x380` | `0x47d` |
| `sp+44` | `this+0x388` | `0x47b` |
| `sp+48` | `this+0x38c` | `0x47a` |
| `sp+52` | `this+0x390` | `0x484` |
| `sp+56` | `this+0x394` | `0x483` |
| `sp+60` | `this+0x3a0` | `0x50c` |
| `sp+64` | `this+0x39c` | `0x50d` |
| `sp+68` | `this+0x370` | branch-final `0x488` / `0x6bc` |
| `fp` | `this+0x354` | `0x47f` / `0x6ba` |
| `sl` | `this+0x358` | `0x479` / `0x6b9` |
| `r9` | `this+0x35c` | `0x478` / `0x6b8` |
| `r5` | `this+0x36c` | `0x489` / `0x6b7` |
| `r2` | `this+0x350` | `0x480` / `0x6bb` |
| `sp+72` | local `img471` | `0x471` |
| `sp+76` | local `img535` | `0x535` |
| `sp+80` | stack canary | epilogue check |

Each address spill is loaded exactly once for its corresponding
`Image2DCreate` call. There is no indexed table access, pointer escape, loop,
or table-base register, so a user-authored destination table is not proven.

## Exhausted hypotheses

1. Reading `Globals::Canvas` directly for every call, rather than retaining a
   local `PaintCanvas *`, reduced the target from 952 to 940 bytes and did not
   recover the spill frame. It was reverted.
2. Explicit local pointer/reference tables would only force backend state and
   are unsupported by the disassembly.
3. Duplicating the eleven common post-branch calls in both iPad arms could let
   LLVM GVN/tail factoring create the observed live ranges, but no sibling
   source idiom or binary evidence proves that source shape. It was not tried.

Requeue only with new source-shape evidence. Do not use volatile objects,
dummy reads, placement allocation, or artificial stack forcing.
