# NewsTicker::OnTouchMove
elf 0x15e328 body 70
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* NewsTicker::OnTouchMove(int, int) */

bool NewsTicker::OnTouchMove(int param_1,int param_2)

{
  undefined4 *puVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  float fVar4;
  
  if (*(char *)(param_1 + 0x28) != '\0') {
    fVar2 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x2c) - param_2,
                                       (byte)(in_fpscr >> 0x16) & 3);
    fVar4 = *(float *)param_1;
    puVar1 = *(undefined4 **)(DAT_0016e370 + 0x16e346);
    *(float *)param_1 = fVar4 - fVar2;
    fVar3 = (float)VectorSignedToFloat(*puVar1,(byte)(in_fpscr >> 0x16) & 3);
    if (fVar3 < fVar4 - fVar2) {
      *(float *)param_1 = fVar3;
    }
    *(int *)(param_1 + 0x2c) = param_2;
  }
  return *(char *)(param_1 + 0x28) != '\0';
}

```

## target disasm
```
  0016e328: mov r2,r0
  0016e32a: ldrb.w r0,[r0,#0x28]
  0016e32e: cbz r0,0x0016e366
  0016e330: ldr r3,[r2,#0x2c]
  0016e332: subs r3,r3,r1
  0016e334: vmov s0,r3
  0016e338: vcvt.f32.s32 s0,s0
  0016e33c: vldr.32 s2,[r2]
  0016e340: ldr r3,[0x0016e370]
  0016e342: add r3,pc
  0016e344: ldr r3,[r3,#0x0]
  0016e346: vsub.f32 s2,s2,s0
  0016e34a: vstr.32 s2,[r2]
  0016e34e: vldr.32 s0,[r3]
  0016e352: vcvt.f32.s32 s0,s0
  0016e356: vcmpe.f32 s2,s0
  0016e35a: vmrs apsr,fpscr
  0016e35e: it gt
  0016e360: vstr.gt.32 s0,[r2]
  0016e364: str r1,[r2,#0x2c]
  0016e366: cmp r0,#0x0
  0016e368: it ne
  0016e36a: mov.ne r0,#0x1
  0016e36c: bx lr
```
