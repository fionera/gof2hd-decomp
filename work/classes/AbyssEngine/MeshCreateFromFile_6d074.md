# AbyssEngine::MeshCreateFromFile
elf 0x6d074 body 768
Sig: undefined __stdcall MeshCreateFromFile(Engine * param_1, char * param_2, Mesh * * param_3, Material * param_4)

## decompile
```c

/* AbyssEngine::MeshCreateFromFile(AbyssEngine::Engine*, char const*, AbyssEngine::Mesh**,
   AbyssEngine::Material*) */

void AbyssEngine::MeshCreateFromFile(Engine *param_1,char *param_2,Mesh **param_3,Material *param_4)

{
  char cVar1;
  undefined4 uVar2;
  Mesh *pMVar3;
  int iVar4;
  Transform *this;
  uint uVar5;
  undefined4 *puVar6;
  int *piVar7;
  uint uVar8;
  undefined8 in_d5;
  undefined8 in_d6;
  uint in_s14;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  longlong lVar12;
  undefined8 local_50;
  undefined8 local_48;
  uint local_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar7 = *(int **)(DAT_0007d390 + 0x7d08e);
  local_3c = *piVar7;
  uVar2 = 0xfffffffc;
  local_50 = in_d5;
  local_48 = in_d6;
  local_40 = in_s14;
  if (param_1 == (Engine *)0x0 || param_2 == (char *)0x0) goto LAB_0007d35a;
  pMVar3 = operator_new(0x88);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar9 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *pMVar3 = (Mesh)0x0;
  *(undefined2 *)(pMVar3 + 0x22) = 0;
  pMVar3[0x38] = (Mesh)0x0;
  *(undefined4 *)(pMVar3 + 0x4c) = 0x3f800000;
  *(undefined4 *)(pMVar3 + 0x50) = 0;
  *(undefined4 *)(pMVar3 + 0x54) = 0;
  *(undefined4 *)(pMVar3 + 0x59) = 0;
  *(undefined4 *)(pMVar3 + 0x55) = 0;
  *(undefined4 *)(pMVar3 + 0x82) = 0;
  *(undefined4 *)(pMVar3 + 0x3c) = 0;
  *(undefined4 *)(pMVar3 + 0x40) = uVar2;
  *(undefined4 *)(pMVar3 + 0x44) = uVar9;
  *(undefined4 *)(pMVar3 + 0x48) = uVar10;
  *(undefined4 *)(pMVar3 + 0x12) = 0;
  *(undefined4 *)(pMVar3 + 0x16) = uVar2;
  *(undefined4 *)(pMVar3 + 0x1a) = uVar9;
  *(undefined4 *)(pMVar3 + 0x1e) = uVar10;
  *(undefined4 *)(pMVar3 + 2) = 0;
  *(undefined4 *)(pMVar3 + 6) = uVar2;
  *(undefined4 *)(pMVar3 + 10) = uVar9;
  *(undefined4 *)(pMVar3 + 0xe) = uVar10;
  *(undefined4 *)(pMVar3 + 0x28) = 0;
  *(undefined4 *)(pMVar3 + 0x2c) = uVar2;
  *(undefined4 *)(pMVar3 + 0x30) = uVar9;
  *(undefined4 *)(pMVar3 + 0x34) = uVar10;
  *(undefined4 *)(pMVar3 + 0x70) = 0;
  *(undefined4 *)(pMVar3 + 0x74) = uVar2;
  *(undefined4 *)(pMVar3 + 0x78) = uVar9;
  *(undefined4 *)(pMVar3 + 0x7c) = uVar10;
  *(undefined4 *)(pMVar3 + 0x60) = 0;
  *(undefined4 *)(pMVar3 + 100) = uVar2;
  *(undefined4 *)(pMVar3 + 0x68) = uVar9;
  *(undefined4 *)(pMVar3 + 0x6c) = uVar10;
  *(undefined4 *)(pMVar3 + 0x7e) = 0;
  *param_3 = pMVar3;
  local_40 = 0;
  *(Material **)(pMVar3 + 0x30) = param_4;
  iVar4 = AEFile::OpenRead((AEFile *)param_2,(char *)&local_40,(uint *)(pMVar3 + 0x60));
  if (iVar4 == 0) {
    if (*param_3 != (Mesh *)0x0) {
      operator_delete(*param_3);
    }
    *param_3 = (Mesh *)0x0;
  }
  else {
    local_48 = CONCAT44(DAT_0007d384,0x2a2a2a2a);
    iVar4 = AEFile::Read(7,&local_48,local_40);
    if (iVar4 != 0) {
      uVar8 = 0x1f;
      for (iVar4 = 0; iVar4 != 7; iVar4 = iVar4 + 1) {
        cVar1 = *(char *)((int)&local_48 + iVar4);
        if (*(char *)(DAT_0007d3a0 + 0x7d156 + iVar4) != cVar1) {
          uVar8 = uVar8 & 0xfffffffb;
        }
        if (*(char *)(DAT_0007d394 + 0x7d144 + iVar4) != cVar1) {
          uVar8 = uVar8 & 0xfffffffe;
        }
        if (*(char *)(DAT_0007d3a4 + 0x7d15c + iVar4) != cVar1) {
          uVar8 = uVar8 & 0xfffffffd;
        }
        if (*(char *)(DAT_0007d398 + 0x7d14a + iVar4) != cVar1) {
          uVar8 = uVar8 & 0xfffffff7;
        }
        if (*(char *)(DAT_0007d39c + 0x7d150 + iVar4) != cVar1) {
          uVar8 = uVar8 & 0xffffffef;
        }
      }
      if (uVar8 != 0) {
        puVar6 = *(undefined4 **)(DAT_0007d3a8 + 0x7d1c4);
        *puVar6 = DAT_0007d388;
        if (((((uVar8 & 0x1b) == 0) || (iVar4 = AEFile::Read(2,&local_48,local_40), iVar4 != 0)) &&
            (iVar4 = AEFile::Read(1,*param_3,local_40), iVar4 != 0)) && (**param_3 != (Mesh)0x0)) {
          if ((uVar8 & 0x1a) == 0) {
            iVar4 = MeshReadData(param_1,&local_40,uVar8,param_3,param_4);
            if (iVar4 != -1) goto LAB_0007d318;
          }
          else {
            iVar4 = AEFile::Read(2,(void *)((int)&local_50 + 6),local_40);
            if (iVar4 != 0) {
              if (local_50._6_2_ < 2) {
                iVar4 = MeshReadData(param_1,&local_40,uVar8,param_3,param_4);
                if (iVar4 == -1) goto LAB_0007d348;
              }
              else {
                this = operator_new(0x180);
                Transform::Transform(this);
                uVar2 = 0;
                uVar9 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
                uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
                uVar11 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
                *(Transform **)(*param_3 + 0x34) = this;
                for (uVar5 = 0; uVar5 < local_50._6_2_; uVar5 = uVar5 + 1) {
                  pMVar3 = operator_new(0x88);
                  *(undefined4 *)(pMVar3 + 0x82) = 0;
                  *(undefined4 *)(pMVar3 + 0x7e) = 0;
                  pMVar3[0x38] = (Mesh)0x0;
                  *(undefined2 *)(pMVar3 + 0x22) = 0;
                  *pMVar3 = (Mesh)0x0;
                  *(undefined4 *)(pMVar3 + 0x4c) = 0x3f800000;
                  *(undefined4 *)(pMVar3 + 0x50) = 0;
                  *(undefined4 *)(pMVar3 + 0x54) = 0;
                  *(undefined4 *)(pMVar3 + 0x59) = 0;
                  *(undefined4 *)(pMVar3 + 0x55) = 0;
                  *(undefined4 *)(pMVar3 + 0x70) = uVar2;
                  *(undefined4 *)(pMVar3 + 0x74) = uVar9;
                  *(undefined4 *)(pMVar3 + 0x78) = uVar10;
                  *(undefined4 *)(pMVar3 + 0x7c) = uVar11;
                  *(undefined4 *)(pMVar3 + 0x60) = uVar2;
                  *(undefined4 *)(pMVar3 + 100) = uVar9;
                  *(undefined4 *)(pMVar3 + 0x68) = uVar10;
                  *(undefined4 *)(pMVar3 + 0x6c) = uVar11;
                  *(undefined4 *)(pMVar3 + 0x28) = uVar2;
                  *(undefined4 *)(pMVar3 + 0x2c) = uVar9;
                  *(undefined4 *)(pMVar3 + 0x30) = uVar10;
                  *(undefined4 *)(pMVar3 + 0x34) = uVar11;
                  *(undefined4 *)(pMVar3 + 0x12) = uVar2;
                  *(undefined4 *)(pMVar3 + 0x16) = uVar9;
                  *(undefined4 *)(pMVar3 + 0x1a) = uVar10;
                  *(undefined4 *)(pMVar3 + 0x1e) = uVar11;
                  *(undefined4 *)(pMVar3 + 2) = uVar2;
                  *(undefined4 *)(pMVar3 + 6) = uVar9;
                  *(undefined4 *)(pMVar3 + 10) = uVar10;
                  *(undefined4 *)(pMVar3 + 0xe) = uVar11;
                  *(undefined4 *)(pMVar3 + 0x3c) = uVar2;
                  *(undefined4 *)(pMVar3 + 0x40) = uVar9;
                  *(undefined4 *)(pMVar3 + 0x44) = uVar10;
                  *(undefined4 *)(pMVar3 + 0x48) = uVar11;
                  local_50 = CONCAT44(local_50._4_4_,pMVar3);
                  pMVar3[0x84] = (Mesh)0x1;
                  *pMVar3 = **param_3;
                  *(Material **)(pMVar3 + 0x30) = param_4;
                  iVar4 = MeshReadData(param_1,&local_40,uVar8,(Mesh **)&local_50,param_4);
                  if (iVar4 == -1) goto LAB_0007d348;
                  AEMath::BSphere::Merge
                            ((BSphere *)(*param_3 + 0x3c),(BSphere *)((Mesh *)local_50 + 0x3c));
                  ArrayAdd<AbyssEngine::Mesh*>
                            ((Mesh *)local_50,(Array *)(*(int *)(*param_3 + 0x34) + 0x3c));
                }
              }
LAB_0007d318:
              AEFile::Close(local_40);
              if (*(Transform **)(*param_3 + 0x34) != (Transform *)0x0) {
                Transform::CollectAnimationData(*(Transform **)(*param_3 + 0x34));
                pMVar3 = *param_3;
                lVar12 = __aeabi_f2lz(*puVar6);
                Transform::SetAnimationRangeInTime
                          (CONCAT44((int)((ulonglong)lVar12 >> 0x20),*(undefined4 *)(pMVar3 + 0x34))
                           ,lVar12);
              }
              uVar2 = 1;
              goto LAB_0007d35a;
            }
          }
        }
      }
    }
LAB_0007d348:
    MeshRelease(param_1,param_3);
    AEFile::Close(local_40);
  }
  uVar2 = 0xffffffff;
LAB_0007d35a:
  if (*piVar7 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar2);
  }
  return;
}

```

## target disasm
```
  0007d074: push {r4,r5,r6,r7,lr}
  0007d076: add r7,sp,#0xc
  0007d078: push {r7,r8,r9,r10,r11}
  0007d07c: vpush {d1,d2,d3,d4,d5,d6,d7,d8,d9}
  0007d080: mov r10,r0
  0007d082: ldr r0,[0x0007d390]
  0007d084: cmp.w r10,#0x0
  0007d088: mov r6,r3
  0007d08a: add r0,pc
  0007d08c: mov r5,r2
  0007d08e: mov r4,r1
  0007d090: ldr.w r9,[r0,#0x0]
  0007d094: ldr.w r0,[r9,#0x0]
  0007d098: str r0,[sp,#0x34]
  0007d09a: mvn r0,#0x3
  0007d09e: it ne
  0007d0a0: cmp.ne r4,#0x0
  0007d0a2: beq.w 0x0007d35a
  0007d0a6: movs r0,#0x88
  0007d0a8: blx 0x0006eb24
  0007d0ac: movs r1,#0x0
  0007d0ae: vmov.i32 q8,#0x0
  0007d0b2: mov.w r2,#0x3f800000
  0007d0b6: strb r1,[r0,#0x0]
  0007d0b8: strh r1,[r0,#0x22]
  0007d0ba: strb.w r1,[r0,#0x38]
  0007d0be: strd r2,r1,[r0,#0x4c]
  0007d0c2: add.w r2,r0,#0x3c
  0007d0c6: str r1,[r0,#0x54]
  0007d0c8: str.w r1,[r0,#0x59]
  0007d0cc: str.w r1,[r0,#0x55]
  0007d0d0: str.w r1,[r0,#0x82]
  0007d0d4: vst1.32 {d16,d17},[r2]
  0007d0d8: add.w r2,r0,#0x12
  0007d0dc: vst1.16 {d16,d17},[r2]
  0007d0e0: adds r2,r0,#0x2
  0007d0e2: vst1.16 {d16,d17},[r2]
  0007d0e6: add.w r2,r0,#0x28
  0007d0ea: vst1.32 {d16,d17},[r2]
  0007d0ee: add.w r2,r0,#0x70
  0007d0f2: vst1.32 {d16,d17},[r2]
  0007d0f6: add.w r2,r0,#0x60
  0007d0fa: vst1.32 {d16,d17},[r2]
  0007d0fe: str.w r1,[r0,#0x7e]
  0007d102: str r0,[r5,#0x0]
  0007d104: str r1,[sp,#0x30]
  0007d106: add r1,sp,#0x30
  0007d108: str r6,[r0,#0x30]
  0007d10a: mov r0,r4
  0007d10c: blx 0x0006ecd4
  0007d110: cmp r0,#0x0
  0007d112: beq.w 0x0007d23a
  0007d116: strd r6,r5,[sp,#0x18]
  0007d11a: add r1,sp,#0x28
  0007d11c: ldr r0,[0x0007d384]
  0007d11e: ldr r2,[sp,#0x30]
  0007d120: str r0,[sp,#0x2c]
  0007d122: mov.w r0,#0x2a2a2a2a
  0007d126: str r0,[sp,#0x28]
  0007d128: movs r0,#0x7
  0007d12a: blx 0x0006eb90
  0007d12e: cmp r0,#0x0
  0007d130: beq.w 0x0007d348
  0007d134: strd r9,r10,[sp,#0x10]
  0007d138: movs r0,#0x0
  0007d13a: ldr r1,[0x0007d394]
  0007d13c: mov.w r11,#0x1f
  0007d140: add r1,pc
  0007d142: mov r12,r1
  0007d144: ldr r1,[0x0007d398]
  0007d146: add r1,pc
  0007d148: mov lr,r1
  0007d14a: ldr r1,[0x0007d39c]
  0007d14c: add r1,pc
  0007d14e: mov r10,r1
  0007d150: ldr r1,[0x0007d3a0]
  0007d152: add r1,pc
  0007d154: mov r9,r1
  0007d156: ldr r1,[0x0007d3a4]
  0007d158: add r1,pc
  0007d15a: mov r8,r1
  0007d15c: b 0x0007d1aa
  0007d15e: add r1,sp,#0x28
  0007d160: mov r6,r9
  0007d162: mov r2,r12
  0007d164: ldrb.w r6,[r9,r0]
  0007d168: ldrb r1,[r1,r0]
  0007d16a: mov r3,r8
  0007d16c: ldrb.w r2,[r12,r0]
  0007d170: mov r4,lr
  0007d172: ldrb.w r3,[r8,r0]
  0007d176: mov r4,r10
  0007d178: cmp r6,r1
  0007d17a: it ne
  0007d17c: bic.ne r11,r11,#0x4
  0007d180: ldrb.w r5,[lr,r0]
  0007d184: cmp r2,r1
  0007d186: ldrb.w r4,[r10,r0]
  0007d18a: it ne
  0007d18c: bic.ne r11,r11,#0x1
  0007d190: cmp r3,r1
  0007d192: it ne
  0007d194: bic.ne r11,r11,#0x2
  0007d198: cmp r5,r1
  0007d19a: it ne
  0007d19c: bic.ne r11,r11,#0x8
  0007d1a0: cmp r4,r1
  0007d1a2: it ne
  0007d1a4: bic.ne r11,r11,#0x10
  0007d1a8: adds r0,#0x1
  0007d1aa: cmp r0,#0x7
  0007d1ac: bne 0x0007d15e
  0007d1ae: ldrd r9,r10,[sp,#0x10]
  0007d1b2: cmp.w r11,#0x0
  0007d1b6: beq.w 0x0007d348
  0007d1ba: ldr r0,[0x0007d3a8]
  0007d1bc: tst r11,#0x1b
  0007d1c0: add r0,pc
  0007d1c2: ldr r5,[r0,#0x0]
  0007d1c4: ldr r0,[0x0007d388]
  0007d1c6: str r0,[r5,#0x0]
  0007d1c8: beq 0x0007d1da
  0007d1ca: ldr r2,[sp,#0x30]
  0007d1cc: add r1,sp,#0x28
  0007d1ce: movs r0,#0x2
  0007d1d0: blx 0x0006eb90
  0007d1d4: cmp r0,#0x0
  0007d1d6: beq.w 0x0007d348
  0007d1da: ldr r0,[sp,#0x1c]
  0007d1dc: ldr r2,[sp,#0x30]
  0007d1de: ldr r1,[r0,#0x0]
  0007d1e0: movs r0,#0x1
  0007d1e2: blx 0x0006eb90
  0007d1e6: cmp r0,#0x0
  0007d1e8: beq.w 0x0007d348
  0007d1ec: ldr r0,[sp,#0x1c]
  0007d1ee: ldr r0,[r0,#0x0]
  0007d1f0: ldrb r0,[r0,#0x0]
  0007d1f2: cmp r0,#0x0
  0007d1f4: beq.w 0x0007d348
  0007d1f8: tst r11,#0x1a
  0007d1fc: beq 0x0007d2ec
  0007d1fe: ldr r2,[sp,#0x30]
  0007d200: sub.w r1,r7,#0x42
  0007d204: movs r0,#0x2
  0007d206: str r5,[sp,#0xc]
  0007d208: blx 0x0006eb90
  0007d20c: ldrd r8,r6,[sp,#0x18]
  0007d210: cmp r0,#0x0
  0007d212: beq.w 0x0007d348
  0007d216: ldrh.w r0,[r7,#-0x42]
  0007d21a: cmp r0,#0x2
  0007d21c: bcc 0x0007d302
  0007d21e: mov.w r0,#0x180
  0007d222: blx 0x0006eb24
  0007d226: mov r4,r0
  0007d228: blx 0x0006eb84
  0007d22c: ldr r0,[r6,#0x0]
  0007d22e: vmov.i32 q4,#0x0
  0007d232: movs r5,#0x0
  0007d234: str r4,[r0,#0x34]
  0007d236: movs r4,#0x0
  0007d238: b 0x0007d2e2
  0007d23a: ldr r0,[r5,#0x0]
  0007d23c: cbz r0,0x0007d242
  0007d23e: blx 0x0006eb48
  0007d242: movs r0,#0x0
  0007d244: str r0,[r5,#0x0]
  0007d246: b 0x0007d356
  0007d248: movs r0,#0x88
  0007d24a: blx 0x0006eb24
  0007d24e: mov.w r1,#0x3f800000
  0007d252: str.w r5,[r0,#0x82]
  0007d256: str.w r5,[r0,#0x7e]
  0007d25a: add r3,sp,#0x20
  0007d25c: strb.w r5,[r0,#0x38]
  0007d260: mov r2,r11
  0007d262: strh r5,[r0,#0x22]
  0007d264: strb r5,[r0,#0x0]
  0007d266: strd r1,r5,[r0,#0x4c]
  0007d26a: add.w r1,r0,#0x70
  0007d26e: str r5,[r0,#0x54]
  0007d270: str.w r5,[r0,#0x59]
  0007d274: str.w r5,[r0,#0x55]
  0007d278: vst1.32 {d8,d9},[r1]
  0007d27c: add.w r1,r0,#0x60
  0007d280: vst1.32 {d8,d9},[r1]
  0007d284: add.w r1,r0,#0x28
  0007d288: vst1.32 {d8,d9},[r1]
  0007d28c: add.w r1,r0,#0x12
  0007d290: vst1.16 {d8,d9},[r1]
  0007d294: adds r1,r0,#0x2
  0007d296: vst1.16 {d8,d9},[r1]
  0007d29a: add.w r1,r0,#0x3c
  0007d29e: vst1.32 {d8,d9},[r1]
  0007d2a2: movs r1,#0x1
  0007d2a4: str r0,[sp,#0x20]
  0007d2a6: strb.w r1,[r0,#0x84]
  0007d2aa: ldr r1,[r6,#0x0]
  0007d2ac: ldrb r1,[r1,#0x0]
  0007d2ae: strb r1,[r0,#0x0]
  0007d2b0: add r1,sp,#0x30
  0007d2b2: str.w r8,[r0,#0x30]
  0007d2b6: mov r0,r10
  0007d2b8: str.w r8,[sp,#0x0]
  0007d2bc: blx 0x0006ecb0
  0007d2c0: adds r0,#0x1
  0007d2c2: beq 0x0007d348
  0007d2c4: ldr r0,[sp,#0x20]
  0007d2c6: ldr r2,[r6,#0x0]
  0007d2c8: add.w r1,r0,#0x3c
  0007d2cc: add.w r0,r2,#0x3c
  0007d2d0: blx 0x0006eca4
  0007d2d4: ldr r1,[r6,#0x0]
  0007d2d6: ldr r0,[sp,#0x20]
  0007d2d8: ldr r1,[r1,#0x34]
  0007d2da: adds r1,#0x3c
  0007d2dc: blx 0x0006ecbc
  0007d2e0: adds r4,#0x1
  0007d2e2: ldrh.w r0,[r7,#-0x42]
  0007d2e6: cmp r4,r0
  0007d2e8: bcc 0x0007d248
  0007d2ea: b 0x0007d316
  0007d2ec: ldr r3,[sp,#0x1c]
  0007d2ee: add r1,sp,#0x30
  0007d2f0: ldr r0,[sp,#0x18]
  0007d2f2: mov r2,r11
  0007d2f4: str r0,[sp,#0x0]
  0007d2f6: mov r0,r10
  0007d2f8: blx 0x0006ecb0
  0007d2fc: adds r0,#0x1
  0007d2fe: bne 0x0007d318
  0007d300: b 0x0007d348
  0007d302: add r1,sp,#0x30
  0007d304: mov r0,r10
  0007d306: mov r2,r11
  0007d308: mov r3,r6
  0007d30a: str.w r8,[sp,#0x0]
  0007d30e: blx 0x0006ecb0
  0007d312: adds r0,#0x1
  0007d314: beq 0x0007d348
  0007d316: ldr r5,[sp,#0xc]
  0007d318: ldr r0,[sp,#0x30]
  0007d31a: blx 0x0006ece0
  0007d31e: ldr r4,[sp,#0x1c]
  0007d320: ldr r0,[r4,#0x0]
  0007d322: ldr r0,[r0,#0x34]
  0007d324: cbz r0,0x0007d344
  0007d326: blx 0x0006ecec
  0007d32a: ldr r0,[r5,#0x0]
  0007d32c: ldr r4,[r4,#0x0]
  0007d32e: blx 0x0006ebb4
  0007d332: mov r2,r0
  0007d334: ldr r0,[r4,#0x34]
  0007d336: ldr r3,[0x0007d38c]
  0007d338: movs r6,#0x0
  0007d33a: strd r3,r6,[sp,#0x0]
  0007d33e: mov r3,r1
  0007d340: blx 0x0006ebc0
  0007d344: movs r0,#0x1
  0007d346: b 0x0007d35a
  0007d348: ldr r1,[sp,#0x1c]
  0007d34a: mov r0,r10
  0007d34c: blx 0x0006ebd8
  0007d350: ldr r0,[sp,#0x30]
  0007d352: blx 0x0006ece0
  0007d356: mov.w r0,#0xffffffff
  0007d35a: ldr r1,[sp,#0x34]
  0007d35c: ldr.w r2,[r9,#0x0]
  0007d360: subs r1,r2,r1
  0007d362: itttt eq
  0007d364: vpop.eq {d1,d2,d3,d4,d5,d6,d7,d8,d9}
  0007d368: add.eq sp,#0x4
  0007d36a: pop.eq.w {r8,r9,r10,r11}
  0007d36e: pop.eq {r4,r5,r6,r7,pc}
  0007d370: blx 0x0006e824
```
