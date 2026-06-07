# TouchButton::setPosition
elf 0x16936c body 8
Sig: undefined __thiscall setPosition(TouchButton * this, int param_1, int param_2)

## decompile
```c

/* TouchButton::setPosition(int, int) */

void __thiscall TouchButton::setPosition(TouchButton *this,int param_1,int param_2)

{
  (*(code *)(DAT_001ac464 + 0x1ac468))();
  return;
}

```

## target disasm
```
  0017936c: ldrb.w r3,[r0,#0x74]
  00179370: b.w 0x001ac458
```
