# TouchButton::setHalfTransparent
elf 0x16939e body 6
Sig: undefined __stdcall setHalfTransparent(bool param_1)

## decompile
```c

/* TouchButton::setHalfTransparent(bool) */

void TouchButton::setHalfTransparent(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0xb3) = in_r1;
  return;
}

```

## target disasm
```
  0017939e: strb.w r1,[r0,#0xb3]
  001793a2: bx lr
```
