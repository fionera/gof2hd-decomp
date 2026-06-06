# ScrollTouchWindow::setTextCentered

ELF vaddr 0x174484, body 8 bytes
Signature: undefined __stdcall setTextCentered(bool param_1)

## Ghidra decompile
```c

/* ScrollTouchWindow::setTextCentered(bool) */

void ScrollTouchWindow::setTextCentered(bool param_1)

{
  (*(code *)(DAT_001ac6f4 + 0x1ac6f8))(*(undefined4 *)(uint)param_1);
  return;
}

```

## Target disassembly (must match)
```
  00184484: ldr r0,[r0,#0x0]
  00184486: b.w 0x001ac6e8
  001ac6e8: bx pc
```

Verify: tools/try.sh work/src/setTextCentered.cpp setTextCentered 174484 32
