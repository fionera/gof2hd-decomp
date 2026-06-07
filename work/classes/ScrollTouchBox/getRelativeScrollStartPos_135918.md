# ScrollTouchBox::getRelativeScrollStartPos
elf 0x135918 body 42
Sig: undefined __thiscall getRelativeScrollStartPos(ScrollTouchBox * this)

## decompile
```c

/* ScrollTouchBox::getRelativeScrollStartPos() */

float __thiscall ScrollTouchBox::getRelativeScrollStartPos(ScrollTouchBox *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = DAT_00145944;
  if (*(int *)(this + 0x34) < 1) {
    fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x34),(byte)(in_fpscr >> 0x16) & 3);
    fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x18),(byte)(in_fpscr >> 0x16) & 3);
    fVar1 = -fVar1 / fVar2;
  }
  return fVar1;
}

```

## target disasm
```
  00145918: ldr r1,[r0,#0x34]
  0014591a: cmp r1,#0x0
  0014591c: ble 0x00145924
  0014591e: vldr.32 s0,[pc,#0x24]
  00145922: b 0x0014593c
  00145924: vmov s0,r1
  00145928: vcvt.f32.s32 s0,s0
  0014592c: vldr.32 s2,[r0,#0x18]
  00145930: vcvt.f32.s32 s2,s2
  00145934: vneg.f32 s0,s0
  00145938: vdiv.f32 s0,s0,s2
  0014593c: vmov r0,s0
  00145940: bx lr
```
