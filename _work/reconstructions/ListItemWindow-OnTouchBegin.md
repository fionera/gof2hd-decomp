# `ListItemWindow::OnTouchBegin` layout blocker

## Target

- `_ZN14ListItemWindow12OnTouchBeginEii`
- 92.1%, 96/96 bytes
- TU: `src/game/ui/ListItemWindow.cpp`

The original and current bodies have the same instruction count and control
flow. The remaining structural differences are the offsets of the window
rectangle:

| Field | Original offset | Current named-field offset |
| --- | ---: | ---: |
| `x` | `+0x64` | `+0x5c` |
| `y` | `+0x68` | `+0x60` |
| `width` | `+0x6c` | `+0x64` |

This is not safe to repair with raw byte-offset reads in the `.cpp`.
`shipTransform`, `addonTransform`, and the preview-animation tail are already
verified at `+0x8c`, `+0x90`, and `+0x114` onward, so blindly inserting eight
bytes before `x` would shift known-correct later fields. Requeue only as an
exclusive, atomic `ListItemWindow.h` layout reconstruction using the full
binary struct layout and all class methods as witnesses.

The adjacent `update(int)` pass is independent of this blocker: its recovered
Matrix-by-value calls and addon-transform path compile to the original
352-byte size and improve the function from 45.7% to 77.0%.
