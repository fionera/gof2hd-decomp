# PlayerFixedObject::getPosition
elf 0x154056 body 38
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerFixedObject::getPosition() */

void PlayerFixedObject::getPosition(void)

{
  undefined4 *in_r0;
  int in_r1;
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar2 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x17c),(byte)(in_fpscr >> 0x16) & 3);
  uVar1 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x178),(byte)(in_fpscr >> 0x16) & 3);
  uVar3 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x180),(byte)(in_fpscr >> 0x16) & 3);
  *in_r0 = uVar1;
  in_r0[1] = uVar2;
  in_r0[2] = uVar3;
  return;
}

```

## target disasm
```
  00164056: vldr.32 s0,[r1,#0x178]
  0016405a: vldr.32 s2,[r1,#0x17c]
  0016405e: vldr.32 s4,[r1,#0x180]
  00164062: vcvt.f32.s32 s2,s2
  00164066: vcvt.f32.s32 s0,s0
  0016406a: vcvt.f32.s32 s4,s4
  0016406e: vstr.32 s0,[r0]
  00164072: vstr.32 s2,[r0,#0x4]
  00164076: vstr.32 s4,[r0,#0x8]
  0016407a: bx lr
```
