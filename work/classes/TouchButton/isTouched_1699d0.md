# TouchButton::isTouched
elf 0x1699d0 body 6
Sig: undefined __thiscall isTouched(TouchButton * this)

## decompile
```c

/* TouchButton::isTouched() */

TouchButton __thiscall TouchButton::isTouched(TouchButton *this)

{
  return this[0xb0];
}

```

## target disasm
```
  001799d0: ldrb.w r0,[r0,#0xb0]
  001799d4: bx lr
```
