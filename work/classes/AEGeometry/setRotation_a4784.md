# AEGeometry::setRotation
elf 0xa4784 body 110
Sig: undefined __thiscall setRotation(AEGeometry * this, float param_1, float param_2, float param_3)

## decompile
```c

/* AEGeometry::setRotation(float, float, float) */

void __thiscall AEGeometry::setRotation(AEGeometry *this,float param_1,float param_2,float param_3)

{
  undefined4 uVar1;
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  int *piVar2;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_60 [60];
  int local_24;
  
  piVar2 = *(int **)(DAT_000b47f4 + 0xb479a);
  local_24 = *piVar2;
  uVar1 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(this + 0x2c));
  AbyssEngine::AEMath::MatrixSetRotation(aMStack_60,uVar1);
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(this + 0x2c));
  AbyssEngine::AEMath::MatrixSetScaling(aMStack_60,*(float *)(this + 0x44),extraout_s1,extraout_s2);
  *(undefined4 *)(this + 0x30) = in_r1;
  *(undefined4 *)(this + 0x34) = in_r2;
  *(undefined4 *)(this + 0x38) = in_r3;
  if (*piVar2 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b4784: push {r4,r5,r6,r7,lr}
  000b4786: add r7,sp,#0xc
  000b4788: push {r8,r9,r11}
  000b478c: sub sp,#0x48
  000b478e: mov r6,r0
  000b4790: ldr r0,[0x000b47f4]
  000b4792: mov r4,r1
  000b4794: mov r8,r3
  000b4796: add r0,pc
  000b4798: mov r9,r2
  000b479a: ldr r5,[r0,#0x0]
  000b479c: ldr r0,[r5,#0x0]
  000b479e: str r0,[sp,#0x44]
  000b47a0: ldr r1,[r6,#0xc]
  000b47a2: ldr r0,[r6,#0x2c]
  000b47a4: blx 0x000720c4
  000b47a8: mov r1,r0
  000b47aa: ldr r0,[r6,#0x4c]
  000b47ac: mov r2,r4
  000b47ae: mov r3,r9
  000b47b0: strd r8,r0,[sp,#0x0]
  000b47b4: add r0,sp,#0x8
  000b47b6: blx 0x00073060
  000b47ba: ldr r1,[r6,#0xc]
  000b47bc: ldr r0,[r6,#0x2c]
  000b47be: blx 0x000720c4
  000b47c2: ldrd r2,r3,[r6,#0x3c]
  000b47c6: mov r1,r0
  000b47c8: vldr.32 s0,[r6,#0x44]
  000b47cc: add r0,sp,#0x8
  000b47ce: vstr.32 s0,[sp]
  000b47d2: blx 0x0006f814
  000b47d6: strd r4,r9,[r6,#0x30]
  000b47da: str.w r8,[r6,#0x38]
  000b47de: ldr r0,[sp,#0x44]
  000b47e0: ldr r1,[r5,#0x0]
  000b47e2: subs r0,r1,r0
  000b47e4: ittt eq
  000b47e6: add.eq sp,#0x48
  000b47e8: pop.eq.w {r8,r9,r11}
  000b47ec: pop.eq {r4,r5,r6,r7,pc}
  000b47ee: blx 0x0006e824
```
