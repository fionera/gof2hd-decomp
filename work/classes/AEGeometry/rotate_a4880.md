# AEGeometry::rotate
elf 0xa4880 body 142
Sig: undefined __stdcall rotate(float param_1, float param_2, float param_3)

## decompile
```c

/* AEGeometry::rotate(float, float, float) */

void AEGeometry::rotate(float param_1,float param_2,float param_3)

{
  int in_r0;
  undefined4 uVar1;
  float in_r1;
  float in_r2;
  float in_r3;
  int *piVar2;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_50 [60];
  int local_14;
  
  piVar2 = *(int **)(DAT_000b4910 + 0xb4892);
  local_14 = *piVar2;
  *(float *)(in_r0 + 0x30) = *(float *)(in_r0 + 0x30) + in_r1;
  *(float *)(in_r0 + 0x34) = *(float *)(in_r0 + 0x34) + in_r2;
  *(float *)(in_r0 + 0x38) = *(float *)(in_r0 + 0x38) + in_r3;
  uVar1 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_50,uVar1,*(undefined4 *)(in_r0 + 0x30),*(undefined4 *)(in_r0 + 0x34),
             *(undefined4 *)(in_r0 + 0x38),*(undefined4 *)(in_r0 + 0x4c));
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  AbyssEngine::AEMath::MatrixSetScaling(aMStack_50,*(float *)(in_r0 + 0x44),extraout_s1,extraout_s2)
  ;
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b4880: push {r4,r5,r7,lr}
  000b4882: add r7,sp,#0x8
  000b4884: sub sp,#0x48
  000b4886: mov r4,r0
  000b4888: ldr r0,[0x000b4910]
  000b488a: vmov s0,r2
  000b488e: add r0,pc
  000b4890: vmov s2,r1
  000b4894: vmov s10,r3
  000b4898: ldr r5,[r0,#0x0]
  000b489a: ldr r0,[r5,#0x0]
  000b489c: str r0,[sp,#0x44]
  000b489e: vldr.32 s4,[r4,#0x30]
  000b48a2: vldr.32 s6,[r4,#0x34]
  000b48a6: vldr.32 s8,[r4,#0x38]
  000b48aa: vadd.f32 s2,s4,s2
  000b48ae: vadd.f32 s0,s6,s0
  000b48b2: ldr r0,[r4,#0x2c]
  000b48b4: vadd.f32 s4,s8,s10
  000b48b8: ldr r1,[r4,#0xc]
  000b48ba: vstr.32 s2,[r4,#0x30]
  000b48be: vstr.32 s0,[r4,#0x34]
  000b48c2: vstr.32 s4,[r4,#0x38]
  000b48c6: blx 0x000720c4
  000b48ca: mov r1,r0
  000b48cc: ldrd r2,r3,[r4,#0x30]
  000b48d0: ldr r0,[r4,#0x4c]
  000b48d2: vldr.32 s0,[r4,#0x38]
  000b48d6: str r0,[sp,#0x4]
  000b48d8: add r0,sp,#0x8
  000b48da: vstr.32 s0,[sp]
  000b48de: blx 0x00073060
  000b48e2: ldr r1,[r4,#0xc]
  000b48e4: ldr r0,[r4,#0x2c]
  000b48e6: blx 0x000720c4
  000b48ea: ldrd r2,r3,[r4,#0x3c]
  000b48ee: mov r1,r0
  000b48f0: vldr.32 s0,[r4,#0x44]
  000b48f4: add r0,sp,#0x8
  000b48f6: vstr.32 s0,[sp]
  000b48fa: blx 0x0006f814
  000b48fe: ldr r0,[sp,#0x44]
  000b4900: ldr r1,[r5,#0x0]
  000b4902: subs r0,r1,r0
  000b4904: itt eq
  000b4906: add.eq sp,#0x48
  000b4908: pop.eq {r4,r5,r7,pc}
  000b490a: blx 0x0006e824
```
