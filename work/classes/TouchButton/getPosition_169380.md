# TouchButton::getPosition
elf 0x169380 body 30
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* TouchButton::getPosition() */

void TouchButton::getPosition(void)

{
  undefined4 *in_r0;
  int in_r1;
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x7c),(byte)(in_fpscr >> 0x16) & 3);
  uVar1 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x78),(byte)(in_fpscr >> 0x16) & 3);
  in_r0[2] = 0;
  *in_r0 = uVar1;
  in_r0[1] = uVar2;
  return;
}

```

## target disasm
```
  00179380: vldr.32 s0,[r1,#0x78]
  00179384: vldr.32 s2,[r1,#0x7c]
  00179388: movs r1,#0x0
  0017938a: vcvt.f32.s32 s2,s2
  0017938e: vcvt.f32.s32 s0,s0
  00179392: str r1,[r0,#0x8]
  00179394: vstr.32 s0,[r0]
  00179398: vstr.32 s2,[r0,#0x4]
  0017939c: bx lr
```
