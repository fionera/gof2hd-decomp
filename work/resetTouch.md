# TouchButton::resetTouch

ELF vaddr 0x1699c8, body 8 bytes
Signature: undefined __thiscall resetTouch(TouchButton * this)

## Ghidra decompile
```c

/* TouchButton::resetTouch() */

void __thiscall TouchButton::resetTouch(TouchButton *this)

{
  this[0xb0] = (TouchButton)0x0;
  return;
}

```

## Target disassembly (must match)
```
  001799c8: movs r1,#0x0
  001799ca: strb.w r1,[r0,#0xb0]
  001799ce: bx lr
```

Verify: tools/try.sh work/src/resetTouch.cpp resetTouch 1699c8 32
