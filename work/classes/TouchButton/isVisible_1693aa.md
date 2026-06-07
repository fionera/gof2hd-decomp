# TouchButton::isVisible
elf 0x1693aa body 6
Sig: undefined __thiscall isVisible(TouchButton * this)

## decompile
```c

/* TouchButton::isVisible() */

TouchButton __thiscall TouchButton::isVisible(TouchButton *this)

{
  return this[0xb2];
}

```

## target disasm
```
  001793aa: ldrb.w r0,[r0,#0xb2]
  001793ae: bx lr
```
