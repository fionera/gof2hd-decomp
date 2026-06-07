# TouchButton::setYPosition
elf 0x169374 body 14
Sig: undefined __thiscall setYPosition(TouchButton * this, int param_1)

## decompile
```c

/* TouchButton::setYPosition(int) */

void __thiscall TouchButton::setYPosition(TouchButton *this,int param_1)

{
  (*(code *)(DAT_001ac464 + 0x1ac468))(this,*(undefined4 *)(this + 0x78),param_1,this[0x74]);
  return;
}

```

## target disasm
```
  00179374: mov r2,r1
  00179376: ldrb.w r3,[r0,#0x74]
  0017937a: ldr r1,[r0,#0x78]
  0017937c: b.w 0x001ac458
  001ac458: bx pc
```
