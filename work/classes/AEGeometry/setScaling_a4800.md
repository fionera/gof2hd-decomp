# AEGeometry::setScaling
elf 0xa4800 body 112
Sig: undefined __stdcall setScaling(float param_1, float param_2, float param_3)

## decompile
```c

/* AEGeometry::setScaling(float, float, float) */

void AEGeometry::setScaling(float param_1,float param_2,float param_3)

{
  int in_r0;
  undefined4 uVar1;
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  int *piVar2;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_60 [60];
  int local_24;
  
  piVar2 = *(int **)(DAT_000b4870 + 0xb4816);
  local_24 = *piVar2;
  uVar1 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_60,uVar1,*(undefined4 *)(in_r0 + 0x30),*(undefined4 *)(in_r0 + 0x34),
             *(undefined4 *)(in_r0 + 0x38),*(undefined4 *)(in_r0 + 0x4c));
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  AbyssEngine::AEMath::MatrixSetScaling(aMStack_60,extraout_s0,extraout_s1,extraout_s2);
  *(undefined4 *)(in_r0 + 0x3c) = in_r1;
  *(undefined4 *)(in_r0 + 0x40) = in_r2;
  *(undefined4 *)(in_r0 + 0x44) = in_r3;
  if (*piVar2 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b4800: push {r4,r5,r6,r7,lr}
  000b4802: add r7,sp,#0xc
  000b4804: push {r8,r9,r11}
  000b4808: sub sp,#0x48
  000b480a: mov r6,r0
  000b480c: ldr r0,[0x000b4870]
  000b480e: mov r4,r1
  000b4810: mov r8,r3
  000b4812: add r0,pc
  000b4814: mov r9,r2
  000b4816: ldr r5,[r0,#0x0]
  000b4818: ldr r0,[r5,#0x0]
  000b481a: str r0,[sp,#0x44]
  000b481c: ldr r1,[r6,#0xc]
  000b481e: ldr r0,[r6,#0x2c]
  000b4820: blx 0x000720c4
  000b4824: mov r1,r0
  000b4826: ldrd r2,r3,[r6,#0x30]
  000b482a: ldr r0,[r6,#0x4c]
  000b482c: vldr.32 s0,[r6,#0x38]
  000b4830: str r0,[sp,#0x4]
  000b4832: add r0,sp,#0x8
  000b4834: vstr.32 s0,[sp]
  000b4838: blx 0x00073060
  000b483c: ldr r1,[r6,#0xc]
  000b483e: ldr r0,[r6,#0x2c]
  000b4840: blx 0x000720c4
  000b4844: mov r1,r0
  000b4846: add r0,sp,#0x8
  000b4848: mov r2,r4
  000b484a: mov r3,r9
  000b484c: str.w r8,[sp,#0x0]
  000b4850: blx 0x0006f814
  000b4854: strd r4,r9,[r6,#0x3c]
  000b4858: str.w r8,[r6,#0x44]
  000b485c: ldr r0,[sp,#0x44]
  000b485e: ldr r1,[r5,#0x0]
  000b4860: subs r0,r1,r0
  000b4862: ittt eq
  000b4864: add.eq sp,#0x48
  000b4866: pop.eq.w {r8,r9,r11}
  000b486a: pop.eq {r4,r5,r6,r7,pc}
  000b486c: blx 0x0006e824
```
