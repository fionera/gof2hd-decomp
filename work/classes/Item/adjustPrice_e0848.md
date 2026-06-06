# Item::adjustPrice
elf 0xe0848 body 66
Sig: undefined __stdcall adjustPrice(Station * param_1)

## decompile
```c

/* Item::adjustPrice(Station*) */

void Item::adjustPrice(Station *param_1)

{
  int iVar1;
  int iVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  
  iVar2 = *(int *)(param_1 + 0x20);
  iVar1 = Station::getTecLevel();
  fVar3 = (float)VectorSignedToFloat(10 - iVar1,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x24) - *(int *)(param_1 + 0x20),
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(int *)(param_1 + 0x18) = (int)((fVar3 / 10.0) * fVar4) + iVar2;
  return;
}

```
## target disasm
```
  000f0848: push {r4,r5,r7,lr}
  000f084a: add r7,sp,#0x8
  000f084c: mov r4,r0
  000f084e: ldr r5,[r0,#0x20]
  000f0850: mov r0,r1
  000f0852: blx 0x00071890
  000f0856: rsb.w r0,r0,#0xa
  000f085a: vmov.f32 s0,0x41200000
  000f085e: vmov s2,r0
  000f0862: vcvt.f32.s32 s2,s2
  000f0866: ldrd r0,r1,[r4,#0x20]
  000f086a: subs r0,r1,r0
  000f086c: vdiv.f32 s0,s2,s0
  000f0870: vmov s2,r0
  000f0874: vcvt.f32.s32 s2,s2
  000f0878: vmul.f32 s0,s0,s2
  000f087c: vcvt.s32.f32 s0,s0
  000f0880: vmov r0,s0
  000f0884: add r0,r5
  000f0886: str r0,[r4,#0x18]
  000f0888: pop {r4,r5,r7,pc}
```
