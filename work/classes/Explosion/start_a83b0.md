# Explosion::start
elf 0xa83b0 body 370
Sig: undefined __stdcall start(Vector * param_1, Vector * param_2)

## decompile
```c

/* Explosion::start(AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) */

void Explosion::start(Vector *param_1,Vector *param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint *puVar3;
  Vector *in_r2;
  int *piVar4;
  uint uVar5;
  int *piVar6;
  uint *puVar7;
  uint in_fpscr;
  float fVar8;
  float extraout_s1;
  undefined8 local_68;
  undefined4 local_60;
  int local_28;
  
  piVar6 = *(int **)(DAT_000b8530 + 0xb83c6);
  local_28 = *piVar6;
  AEGeometry::setPosition(*(AEGeometry **)(param_1 + 4),param_2);
  puVar7 = *(uint **)(DAT_000b8534 + 0xb83e0);
  iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar7);
  *(undefined1 *)(iVar1 + 0xed) = 1;
  if (*(int *)(*(int *)(param_1 + 4) + 0x14) != -1) {
    iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar7);
    *(undefined1 *)(iVar1 + 0xed) = 1;
  }
  if (*(AEGeometry **)(param_1 + 8) != (AEGeometry *)0x0) {
    AEGeometry::setPosition(*(AEGeometry **)(param_1 + 8),param_2);
    iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar7);
    *(undefined1 *)(iVar1 + 0xed) = 1;
  }
  if (*(int *)param_1 - 8U < 3) {
    piVar4 = *(int **)(DAT_000b8538 + 0xb843e);
    uVar2 = AbyssEngine::AERandom::nextInt(*piVar4);
    fVar8 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::MatrixSetRotation
              ((Matrix *)&local_68,fVar8 / DAT_000b8524,extraout_s1,DAT_000b8524);
    uVar2 = AbyssEngine::AERandom::nextInt(*piVar4);
    fVar8 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
    setScaling(fVar8);
  }
  else if (*(int *)param_1 == 0xb) {
    local_68 = 0x3f80000000000000;
    local_60 = 0;
    AEGeometry::setDirection(*(Vector **)(param_1 + 4),in_r2);
    local_68 = 0x3f80000000000000;
    local_60 = 0;
    AEGeometry::setDirection(*(Vector **)(param_1 + 8),in_r2);
  }
  puVar3 = *(uint **)(param_1 + 0xc);
  if (puVar3 != (uint *)0x0) {
    for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
      AEGeometry::setPosition(*(AEGeometry **)(puVar3[1] + uVar5 * 4),param_2);
      iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar7);
      *(undefined1 *)(iVar1 + 0xed) = 1;
      puVar3 = *(uint **)(param_1 + 0xc);
    }
  }
  param_1[0x20] = (Vector)0x1;
  local_68 = *(undefined8 *)param_2;
  local_60 = *(undefined4 *)(param_2 + 8);
  playSound(param_1);
  if (*piVar6 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b83b0: push {r4,r5,r6,r7,lr}
  000b83b2: add r7,sp,#0xc
  000b83b4: push {r8,r9,r10,r11}
  000b83b8: sub sp,#0x4c
  000b83ba: mov r11,r0
  000b83bc: ldr r0,[0x000b8530]
  000b83be: mov r6,r2
  000b83c0: mov r9,r1
  000b83c2: add r0,pc
  000b83c4: ldr.w r8,[r0,#0x0]
  000b83c8: ldr.w r0,[r8,#0x0]
  000b83cc: str r0,[sp,#0x48]
  000b83ce: ldr.w r0,[r11,#0x4]
  000b83d2: blx 0x00072148
  000b83d6: ldr r0,[0x000b8534]
  000b83d8: ldr.w r1,[r11,#0x4]
  000b83dc: add r0,pc
  000b83de: ldr.w r10,[r0,#0x0]
  000b83e2: ldr r1,[r1,#0xc]
  000b83e4: ldr.w r0,[r10,#0x0]
  000b83e8: blx 0x00072088
  000b83ec: movs r4,#0x1
  000b83ee: strb.w r4,[r0,#0xed]
  000b83f2: ldr.w r0,[r11,#0x4]
  000b83f6: ldr r1,[r0,#0x14]
  000b83f8: adds r0,r1,#0x1
  000b83fa: beq 0x000b8408
  000b83fc: ldr.w r0,[r10,#0x0]
  000b8400: blx 0x00072088
  000b8404: strb.w r4,[r0,#0xed]
  000b8408: ldr.w r0,[r11,#0x8]
  000b840c: cbz r0,0x000b8428
  000b840e: mov r1,r9
  000b8410: blx 0x00072148
  000b8414: ldr.w r0,[r11,#0x8]
  000b8418: ldr r1,[r0,#0xc]
  000b841a: ldr.w r0,[r10,#0x0]
  000b841e: blx 0x00072088
  000b8422: movs r1,#0x1
  000b8424: strb.w r1,[r0,#0xed]
  000b8428: ldr.w r0,[r11,#0x0]
  000b842c: sub.w r1,r0,#0x8
  000b8430: cmp r1,#0x3
  000b8432: bcs 0x000b848e
  000b8434: ldr r0,[0x000b8538]
  000b8436: movw r1,#0xc45
  000b843a: add r0,pc
  000b843c: ldr r4,[r0,#0x0]
  000b843e: ldr r0,[r4,#0x0]
  000b8440: blx 0x00071848
  000b8444: vmov s0,r0
  000b8448: vldr.32 s2,[pc,#0xd8]
  000b844c: add.w r1,r11,#0x2c
  000b8450: add r0,sp,#0x8
  000b8452: vcvt.f32.s32 s0,s0
  000b8456: movs r2,#0x0
  000b8458: movs r3,#0x0
  000b845a: vdiv.f32 s0,s0,s2
  000b845e: vstr.32 s0,[sp]
  000b8462: blx 0x00072094
  000b8466: ldr r0,[r4,#0x0]
  000b8468: movs r1,#0x28
  000b846a: blx 0x00071848
  000b846e: vmov s0,r0
  000b8472: vldr.32 s2,[pc,#0xb4]
  000b8476: vldr.32 s4,[pc,#0xb4]
  000b847a: mov r0,r11
  000b847c: vcvt.f32.s32 s0,s0
  000b8480: vmla.f32 s4,s0,s2
  000b8484: vmov r1,s4
  000b8488: blx 0x000727fc
  000b848c: b 0x000b84bc
  000b848e: cmp r0,#0xb
  000b8490: bne 0x000b84bc
  000b8492: ldr.w r0,[r11,#0x4]
  000b8496: add r2,sp,#0x8
  000b8498: mov.w r4,#0x3f800000
  000b849c: mov r1,r6
  000b849e: movs r5,#0x0
  000b84a0: str r4,[sp,#0xc]
  000b84a2: str r5,[sp,#0x8]
  000b84a4: str r5,[sp,#0x10]
  000b84a6: blx 0x000726ac
  000b84aa: ldr.w r0,[r11,#0x8]
  000b84ae: add r2,sp,#0x8
  000b84b0: mov r1,r6
  000b84b2: strd r5,r4,[sp,#0x8]
  000b84b6: str r5,[sp,#0x10]
  000b84b8: blx 0x000726ac
  000b84bc: ldr.w r0,[r11,#0xc]
  000b84c0: cbz r0,0x000b84f0
  000b84c2: movs r5,#0x0
  000b84c4: movs r4,#0x1
  000b84c6: b 0x000b84ea
  000b84c8: ldr r0,[r0,#0x4]
  000b84ca: mov r1,r9
  000b84cc: ldr.w r6,[r0,r5,lsl #0x2]
  000b84d0: mov r0,r6
  000b84d2: blx 0x00072148
  000b84d6: ldr.w r0,[r10,#0x0]
  000b84da: ldr r1,[r6,#0xc]
  000b84dc: blx 0x00072088
  000b84e0: strb.w r4,[r0,#0xed]
  000b84e4: adds r5,#0x1
  000b84e6: ldr.w r0,[r11,#0xc]
  000b84ea: ldr r1,[r0,#0x0]
  000b84ec: cmp r5,r1
  000b84ee: bcc 0x000b84c8
  000b84f0: movs r0,#0x1
  000b84f2: add r1,sp,#0x8
  000b84f4: strb.w r0,[r11,#0x20]
  000b84f8: vldr.64 d16,[r9]
  000b84fc: ldr.w r0,[r9,#0x8]
  000b8500: str r0,[sp,#0x10]
  000b8502: mov r0,r11
  000b8504: vstr.64 d16,[sp,#0x8]
  000b8508: blx 0x0007357c
  000b850c: ldr r0,[sp,#0x48]
  000b850e: ldr.w r1,[r8,#0x0]
  000b8512: subs r0,r1,r0
  000b8514: ittt eq
  000b8516: add.eq sp,#0x4c
  000b8518: pop.eq.w {r8,r9,r10,r11}
  000b851c: pop.eq {r4,r5,r6,r7,pc}
  000b851e: blx 0x0006e824
```
