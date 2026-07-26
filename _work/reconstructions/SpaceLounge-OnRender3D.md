# `SpaceLounge::OnRender3D` layout checkpoint

Target: `_ZN11SpaceLounge10OnRender3DEv`

- Original: `0x173760`, 50 bytes.
- Current: 50 bytes, 94.7%.
- Original TU: `Classes/Game/SpaceLounge.cpp`.
- Current TU: `src/game/menu/SpaceLounge.cpp`.

Every decoded instruction is exact or link-only except one byte load:

```text
original  ldrb r1, [r4, #0x1c]
current   ldrb r1, [r4, #0x1f]
```

The original `+0x1c` field is conclusively `listVisible`, not the current
member named `chatActive`:

- The constructor initializes `listVisible` at `+0x1c` independently from
  the ChoiceWindow-related bytes at `+0x19`, `+0x1a`, and `+0x1b`.
- `onKeyPress` stores 1 at `0x16f062` immediately after
  `ListItemWindow::set`; its back-key path tests and clears the same byte.
- `draw3DShip`, `draw`, and `update` test `+0x1c` before dispatching the
  ListItemWindow path.
- `OnTouchBegin`, `OnTouchMove`, and `OnTouchEnd` use `+0x1c` to select
  ListItemWindow input and clear it when the list closes.

Other `OnRender3D` fields already match: `starMap +0x04`,
`listWindow +0x0c`, `mapVisible +0x34`, and `cutScene +0x44`.

The current header places `chatActive/popupActive/choiceVisible/listVisible`
at `+0x1c/+0x1d/+0x1e/+0x1f`. Changing this one method to test
`chatActive` would match bytes but lie about semantics. Requeue only as an
exclusive atomic `SpaceLounge.h` flag-layout pass using all constructor,
key, draw, update, and touch witnesses. Do not use raw-offset reads.
