# AEGeometry::setDirection
elf 0xa4688 body 230
Sig: undefined __stdcall setDirection(Vector * param_1, Vector * param_2)

## decompile
```c

/* AEGeometry::setDirection(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&)
    */

void AEGeometry::setDirection(Vector *param_1,Vector *param_2)

{
  int *piVar1;
  undefined4 uVar2;
  undefined8 *in_r2;
  code *pcVar3;
  float extraout_s1;
  float extraout_s2;
  AEMath local_c0 [60];
  undefined4 local_84;
  undefined4 uStack_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 local_60;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 local_50;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 local_40;
  int local_28;
  
  piVar1 = *(int **)(DAT_000b4770 + 0xb469e);
  local_28 = *piVar1;
  uVar2 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(param_1 + 0x2c));
  __aeabi_memcpy(&local_68,uVar2,0x3c);
  local_78 = *in_r2;
  local_70 = *(undefined4 *)(in_r2 + 1);
  AbyssEngine::AEMath::VectorCross((Vector *)&local_84,(Vector *)&local_78);
  AbyssEngine::AEMath::VectorNormalize(local_c0,(Vector *)&local_84);
  pcVar3 = *(code **)(DAT_000b4774 + 0xb46e6);
  (*pcVar3)((Vector *)&local_84,local_c0);
  AbyssEngine::AEMath::VectorCross((Vector *)local_c0,param_2);
  (*pcVar3)((Vector *)&local_78,(Vector *)local_c0);
  AbyssEngine::AEMath::VectorNormalize(local_c0,(Vector *)&local_78);
  (*pcVar3)((Vector *)&local_78,local_c0);
  local_48 = local_7c;
  uStack_44 = local_70;
  local_58 = uStack_80;
  uStack_54 = local_78._4_4_;
  local_68 = local_84;
  uStack_64 = (undefined4)local_78;
  local_60 = *(undefined4 *)param_2;
  local_50 = *(undefined4 *)(param_2 + 4);
  local_40 = *(undefined4 *)(param_2 + 8);
  AbyssEngine::PaintCanvas::TransformSetLocal(*(uint *)(param_1 + 0x2c),*(Matrix **)(param_1 + 0xc))
  ;
  AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(param_1 + 0x2c));
  AbyssEngine::AEMath::MatrixSetScaling
            ((Matrix *)local_c0,*(float *)(param_1 + 0x44),extraout_s1,extraout_s2);
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b4688: push {r4,r5,r6,r7,lr}
  000b468a: add r7,sp,#0xc
  000b468c: push {r8,r9,r10,r11}
  000b4690: sub sp,#0xa4
  000b4692: mov r4,r0
  000b4694: ldr r0,[0x000b4770]
  000b4696: mov r11,r1
  000b4698: mov r6,r2
  000b469a: add r0,pc
  000b469c: ldr r0,[r0,#0x0]
  000b469e: str r0,[sp,#0x4]
  000b46a0: ldr r0,[r0,#0x0]
  000b46a2: str r0,[sp,#0xa0]
  000b46a4: ldr r1,[r4,#0xc]
  000b46a6: ldr r0,[r4,#0x2c]
  000b46a8: blx 0x000720c4
  000b46ac: mov r1,r0
  000b46ae: add r0,sp,#0x60
  000b46b0: movs r2,#0x3c
  000b46b2: blx 0x0006f1e4
  000b46b6: add.w r9,sp,#0x50
  000b46ba: vldr.64 d16,[r6]
  000b46be: ldr r0,[r6,#0x8]
  000b46c0: add r6,sp,#0x44
  000b46c2: str r0,[sp,#0x58]
  000b46c4: mov r1,r9
  000b46c6: mov r0,r6
  000b46c8: mov r2,r11
  000b46ca: vstr.64 d16,[sp,#0x50]
  000b46ce: blx 0x0006ec8c
  000b46d2: add.w r10,sp,#0x8
  000b46d6: mov r1,r6
  000b46d8: mov r0,r10
  000b46da: blx 0x0006ec80
  000b46de: ldr r0,[0x000b4774]
  000b46e0: mov r1,r10
  000b46e2: add r0,pc
  000b46e4: ldr.w r8,[r0,#0x0]
  000b46e8: mov r0,r6
  000b46ea: blx r8
  000b46ec: add.w r10,sp,#0x8
  000b46f0: mov r1,r11
  000b46f2: mov r2,r6
  000b46f4: mov r0,r10
  000b46f6: blx 0x0006ec8c
  000b46fa: mov r0,r9
  000b46fc: mov r1,r10
  000b46fe: blx r8
  000b4700: add r6,sp,#0x8
  000b4702: mov r1,r9
  000b4704: mov r0,r6
  000b4706: blx 0x0006ec80
  000b470a: mov r0,r9
  000b470c: mov r1,r6
  000b470e: blx r8
  000b4710: ldrd r1,r2,[sp,#0x54]
  000b4714: ldrd r3,r6,[sp,#0x44]
  000b4718: ldrd r5,r0,[sp,#0x4c]
  000b471c: strd r5,r2,[sp,#0x80]
  000b4720: strd r6,r1,[sp,#0x70]
  000b4724: strd r3,r0,[sp,#0x60]
  000b4728: ldm.w r11!,{r0,r1,r2}
  000b472c: str r0,[sp,#0x68]
  000b472e: str r1,[sp,#0x78]
  000b4730: str r2,[sp,#0x88]
  000b4732: add r2,sp,#0x60
  000b4734: ldr r1,[r4,#0xc]
  000b4736: ldr r0,[r4,#0x2c]
  000b4738: blx 0x000721c0
  000b473c: ldr r1,[r4,#0xc]
  000b473e: ldr r0,[r4,#0x2c]
  000b4740: blx 0x000720c4
  000b4744: ldrd r2,r3,[r4,#0x3c]
  000b4748: mov r1,r0
  000b474a: vldr.32 s0,[r4,#0x44]
  000b474e: add r0,sp,#0x8
  000b4750: vstr.32 s0,[sp]
  000b4754: blx 0x0006f814
  000b4758: ldr r0,[sp,#0xa0]
  000b475a: ldr r1,[sp,#0x4]
  000b475c: ldr r1,[r1,#0x0]
  000b475e: subs r0,r1,r0
  000b4760: ittt eq
  000b4762: add.eq sp,#0xa4
  000b4764: pop.eq.w {r8,r9,r10,r11}
  000b4768: pop.eq {r4,r5,r6,r7,pc}
  000b476a: blx 0x0006e824
```
