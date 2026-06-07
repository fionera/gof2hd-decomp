# TouchButton::getHeight
elf 0x169366 body 6
Sig: undefined __thiscall getHeight(TouchButton * this)

## decompile
```c

/* TouchButton::getHeight() */

undefined4 __thiscall TouchButton::getHeight(TouchButton *this)

{
  return *(undefined4 *)(this + 0x88);
}

```

## target disasm
```
  00179366: ldr.w r0,[r0,#0x88]
  0017936a: bx lr
```
