# TouchButton::setPosition
elf 0x1692a8 body 80
Sig: undefined __thiscall setPosition(TouchButton * this, int param_1, int param_2, uchar param_3)

## decompile
```c

/* TouchButton::setPosition(int, int, unsigned char) */

void __thiscall TouchButton::setPosition(TouchButton *this,int param_1,int param_2,uchar param_3)

{
  uint uVar1;
  
  uVar1 = (uint)param_3;
  *(int *)(this + 0x78) = param_1;
  *(int *)(this + 0x7c) = param_2;
  this[0x74] = (TouchButton)param_3;
  if ((param_3 & 0x20) != 0) {
    param_2 = param_2 - *(int *)(this + 0x8c);
    *(int *)(this + 0x7c) = param_2;
  }
  if ((int)(uVar1 << 0x1e) < 0) {
    param_1 = param_1 - *(int *)(this + 0x90);
    *(int *)(this + 0x78) = param_1;
  }
  if ((int)(uVar1 << 0x19) < 0) {
    *(int *)(this + 0x7c) = param_2 - *(int *)(this + 0x8c) / 2;
  }
  if ((int)(uVar1 << 0x1d) < 0) {
    *(int *)(this + 0x78) = param_1 - *(int *)(this + 0x90) / 2;
  }
  return;
}

```

## target disasm
```
  001792a8: strd r1,r2,[r0,#0x78]
  001792ac: tst r3,#0x20
  001792b0: strb.w r3,[r0,#0x74]
  001792b4: ittt ne
  001792b6: ldr.ne.w r12,[r0,#0x8c]
  001792ba: sub.ne.w r2,r2,r12
  001792be: str.ne r2,[r0,#0x7c]
  001792c0: lsls.w r12,r3,#0x1e
  001792c4: ittt mi
  001792c6: ldr.mi.w r12,[r0,#0x90]
  001792ca: sub.mi.w r1,r1,r12
  001792ce: str.mi r1,[r0,#0x78]
  001792d0: lsls.w r12,r3,#0x19
  001792d4: itttt mi
  001792d6: ldr.mi.w r12,[r0,#0x8c]
  001792da: add.mi.w r12,r12,r12, lsr #0x1f
  001792de: sub.mi.w r2,r2,r12, asr #0x1
  001792e2: str.mi r2,[r0,#0x7c]
  001792e4: lsls r2,r3,#0x1d
  001792e6: itttt mi
  001792e8: ldr.mi.w r2,[r0,#0x90]
  001792ec: add.mi.w r2,r2,r2, lsr #0x1f
  001792f0: sub.mi.w r1,r1,r2, asr #0x1
  001792f4: str.mi r1,[r0,#0x78]
  001792f6: bx lr
```
