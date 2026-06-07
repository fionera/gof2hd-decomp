# BoundingAAB::projectCollisionOnSurface
elf 0x98280 body 348
Sig: undefined __stdcall projectCollisionOnSurface(Vector * param_1)

## decompile
```c

/* BoundingAAB::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void BoundingAAB::projectCollisionOnSurface(Vector *param_1)

{
  int *piVar1;
  float *pfVar2;
  int in_r1;
  int iVar3;
  Vector *in_r2;
  code *pcVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float local_b4;
  float local_b0 [4];
  undefined4 local_a0;
  float local_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  float local_90;
  undefined4 local_8c;
  undefined4 uStack_88;
  undefined4 local_84;
  float local_80;
  undefined4 local_7c;
  undefined4 uStack_78;
  float local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  float local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  float local_58;
  int local_54;
  
  piVar1 = *(int **)(DAT_000a83dc + 0xa829c);
  local_54 = *piVar1;
  fVar5 = *(float *)(in_r1 + 8) + *(float *)(in_r1 + 0x14);
  local_98 = 0;
  uStack_94 = 0;
  local_8c = 0;
  uStack_88 = 0;
  local_9c = *(float *)in_r2 - (fVar5 + *(float *)(in_r1 + 0x20));
  fVar6 = *(float *)in_r2 - (fVar5 - *(float *)(in_r1 + 0x20));
  fVar5 = *(float *)(in_r1 + 0xc) + *(float *)(in_r1 + 0x18);
  local_84 = 0;
  local_7c = 0;
  uStack_78 = 0;
  local_70 = 0;
  fVar7 = *(float *)(in_r2 + 4) - (fVar5 + *(float *)(in_r1 + 0x24));
  fVar8 = *(float *)(in_r2 + 4) - (fVar5 - *(float *)(in_r1 + 0x24));
  fVar5 = *(float *)(in_r1 + 0x10) + *(float *)(in_r1 + 0x1c);
  local_6c = 0;
  uStack_68 = 0;
  local_60 = 0;
  uStack_5c = 0;
  pcVar4 = *(code **)(DAT_000a83e0 + 0xa8334);
  fVar9 = *(float *)(in_r2 + 8) - (fVar5 + *(float *)(in_r1 + 0x28));
  fVar10 = *(float *)(in_r2 + 8) - (fVar5 - *(float *)(in_r1 + 0x28));
  local_90 = fVar6;
  local_80 = fVar7;
  local_74 = fVar8;
  local_64 = fVar9;
  local_58 = fVar10;
  fVar5 = (float)(*pcVar4)(local_9c);
  local_b4 = fVar5;
  local_b0[0] = (float)(*pcVar4)(fVar6);
  local_b0[1] = (float)(*pcVar4)(fVar7);
  local_b0[2] = (float)(*pcVar4)(fVar8);
  local_b0[3] = (float)(*pcVar4)(fVar9);
  local_a0 = (*pcVar4)(fVar10);
  pfVar2 = &local_b4;
  for (iVar3 = 1; pfVar2 = pfVar2 + 1, iVar3 != 6; iVar3 = iVar3 + 1) {
    if ((int)((uint)(*pfVar2 < fVar5) << 0x1f) < 0) {
      fVar5 = *pfVar2;
    }
  }
  AbyssEngine::AEMath::operator-(param_1,in_r2);
  if (*piVar1 != local_54) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a8280: push {r4,r5,r6,r7,lr}
  000a8282: add r7,sp,#0xc
  000a8284: push {r7,r8,r9,r10,r11}
  000a8288: vpush {d8,d9,d10,d11,d12}
  000a828c: sub sp,#0x70
  000a828e: str r0,[sp,#0x4]
  000a8290: mov.w r8,#0x0
  000a8294: ldr r0,[0x000a83dc]
  000a8296: mov r9,r2
  000a8298: add r0,pc
  000a829a: ldr r0,[r0,#0x0]
  000a829c: str r0,[sp,#0x8]
  000a829e: ldr r0,[r0,#0x0]
  000a82a0: str r0,[sp,#0x6c]
  000a82a2: vldr.32 s0,[r1,#0x8]
  000a82a6: vldr.32 s6,[r1,#0x14]
  000a82aa: vldr.32 s12,[r1,#0x20]
  000a82ae: vadd.f32 s0,s0,s6
  000a82b2: vldr.32 s2,[r1,#0xc]
  000a82b6: vldr.32 s8,[r1,#0x18]
  000a82ba: vldr.32 s14,[r1,#0x24]
  000a82be: vldr.32 s4,[r1,#0x10]
  000a82c2: vldr.32 s10,[r1,#0x1c]
  000a82c6: vadd.f32 s6,s0,s12
  000a82ca: vsub.f32 s0,s0,s12
  000a82ce: vldr.32 s12,[r2]
  000a82d2: strd r8,r8,[sp,#0x28]
  000a82d6: strd r8,r8,[sp,#0x34]
  000a82da: vsub.f32 s6,s12,s6
  000a82de: vsub.f32 s16,s12,s0
  000a82e2: vadd.f32 s0,s2,s8
  000a82e6: vstr.32 s6,[sp,#0x24]
  000a82ea: vmov r0,s6
  000a82ee: vstr.32 s16,[sp,#0x30]
  000a82f2: vadd.f32 s2,s0,s14
  000a82f6: vldr.32 s8,[r2,#0x4]
  000a82fa: vsub.f32 s0,s0,s14
  000a82fe: str.w r8,[sp,#0x3c]
  000a8302: strd r8,r8,[sp,#0x44]
  000a8306: str.w r8,[sp,#0x50]
  000a830a: vsub.f32 s18,s8,s2
  000a830e: vsub.f32 s20,s8,s0
  000a8312: vadd.f32 s2,s4,s10
  000a8316: vstr.32 s18,[sp,#0x40]
  000a831a: vstr.32 s20,[sp,#0x4c]
  000a831e: vldr.32 s0,[r1,#0x28]
  000a8322: ldr r1,[0x000a83e0]
  000a8324: vadd.f32 s4,s2,s0
  000a8328: vsub.f32 s0,s2,s0
  000a832c: vldr.32 s2,[r2,#0x8]
  000a8330: add r1,pc
  000a8332: strd r8,r8,[sp,#0x54]
  000a8336: strd r8,r8,[sp,#0x60]
  000a833a: ldr.w r11,[r1,#0x0]
  000a833e: vsub.f32 s22,s2,s4
  000a8342: vsub.f32 s24,s2,s0
  000a8346: vstr.32 s22,[sp,#0x5c]
  000a834a: vstr.32 s24,[sp,#0x68]
  000a834e: blx r11
  000a8350: vmov r1,s16
  000a8354: str r0,[sp,#0xc]
  000a8356: vmov s16,r0
  000a835a: vmov r6,s18
  000a835e: vmov r5,s20
  000a8362: vmov r4,s22
  000a8366: vmov r10,s24
  000a836a: mov r0,r1
  000a836c: blx r11
  000a836e: str r0,[sp,#0x10]
  000a8370: mov r0,r6
  000a8372: blx r11
  000a8374: str r0,[sp,#0x14]
  000a8376: mov r0,r5
  000a8378: blx r11
  000a837a: str r0,[sp,#0x18]
  000a837c: mov r0,r4
  000a837e: blx r11
  000a8380: str r0,[sp,#0x1c]
  000a8382: mov r0,r10
  000a8384: blx r11
  000a8386: str r0,[sp,#0x20]
  000a8388: add r0,sp,#0xc
  000a838a: adds r0,#0x4
  000a838c: movs r1,#0x1
  000a838e: b 0x000a83a8
  000a8390: vldr.32 s0,[r0]
  000a8394: adds r0,#0x4
  000a8396: vcmpe.f32 s0,s16
  000a839a: vmrs apsr,fpscr
  000a839e: itt mi
  000a83a0: vmov.mi.f32 s16,s0
  000a83a4: mov.mi r8,r1
  000a83a6: adds r1,#0x1
  000a83a8: cmp r1,#0x6
  000a83aa: bne 0x000a8390
  000a83ac: add.w r0,r8,r8, lsl #0x1
  000a83b0: add r1,sp,#0x24
  000a83b2: add.w r2,r1,r0, lsl #0x2
  000a83b6: ldr r0,[sp,#0x4]
  000a83b8: mov r1,r9
  000a83ba: blx 0x0006ec38
  000a83be: ldr r0,[sp,#0x6c]
  000a83c0: ldr r1,[sp,#0x8]
  000a83c2: ldr r1,[r1,#0x0]
  000a83c4: subs r0,r1,r0
  000a83c6: itttt eq
  000a83c8: add.eq sp,#0x70
  000a83ca: vpop.eq {d8,d9,d10,d11,d12}
  000a83ce: add.eq sp,#0x4
  000a83d0: pop.eq.w {r8,r9,r10,r11}
  000a83d4: it eq
  000a83d6: pop.eq {r4,r5,r6,r7,pc}
  000a83d8: blx 0x0006e824
```
