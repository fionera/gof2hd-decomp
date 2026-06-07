# Standing::getMissionBonus
elf 0x11d9a0 body 72
Sig: undefined __thiscall getMissionBonus(Standing * this, int param_1)

## decompile
```c

/* Standing::getMissionBonus(int) */

undefined4 __thiscall Standing::getMissionBonus(Standing *this,int param_1)

{
  int iVar1;
  uint in_fpscr;
  float fVar2;
  undefined4 uVar3;
  undefined8 in_d0;
  undefined8 uVar4;
  undefined4 in_s3;
  
  uVar3 = DAT_0012d9ec;
  switch(param_1) {
  case 0:
    iVar1 = **(int **)this;
    break;
  case 1:
    iVar1 = **(int **)this;
    goto LAB_0012d9cc;
  case 2:
    iVar1 = *(int *)(*(int *)this + 4);
    break;
  case 3:
    iVar1 = *(int *)(*(int *)this + 4);
LAB_0012d9cc:
    iVar1 = -iVar1;
    break;
  default:
    goto LAB_0012d9e6;
  }
  fVar2 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
  uVar4 = FloatVectorMax(CONCAT44((int)((ulonglong)in_d0 >> 0x20),fVar2 / DAT_0012d9f0),
                         CONCAT44(in_s3,DAT_0012d9ec),2,0x20);
  uVar3 = (int)uVar4;
LAB_0012d9e6:
  return uVar3;
}

```

## target disasm
```
  0012d9a0: cmp r1,#0x3
  0012d9a2: bhi 0x0012d9b4
  0012d9a4: tbb [pc,r1]
  0012d9ac: ldr r0,[r0,#0x0]
  0012d9ae: vldr.32 s0,[r0]
  0012d9b2: b 0x0012d9d2
  0012d9b4: vldr.32 s0,[pc,#0x34]
  0012d9b8: b 0x0012d9e6
  0012d9ba: ldr r0,[r0,#0x0]
  0012d9bc: ldr r0,[r0,#0x0]
  0012d9be: b 0x0012d9cc
  0012d9c0: ldr r0,[r0,#0x0]
  0012d9c2: vldr.32 s0,[r0,#0x4]
  0012d9c6: b 0x0012d9d2
  0012d9c8: ldr r0,[r0,#0x0]
  0012d9ca: ldr r0,[r0,#0x4]
  0012d9cc: rsbs r0,r0
  0012d9ce: vmov s0,r0
  0012d9d2: vcvt.f32.s32 s0,s0
  0012d9d6: vldr.32 s2,[pc,#0x18]
  0012d9da: vdiv.f32 s0,s0,s2
  0012d9de: vldr.32 s2,[pc,#0xc]
  0012d9e2: vmax.f32 d0,d0,d1
  0012d9e6: vmov r0,s0
  0012d9ea: bx lr
```
