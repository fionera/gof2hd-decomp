# FModSound::updateAll
elf 0x92ff0 body 494
Sig: undefined __stdcall updateAll(Vector * param_1, Vector * param_2, Vector * param_3, Vector * param_4)

## decompile
```c

/* FModSound::updateAll(AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*,
   AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*) */

void FModSound::updateAll(Vector *param_1,Vector *param_2,Vector *param_3,Vector *param_4)

{
  bool bVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  FMOD_VECTOR *pFVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  FMOD_VECTOR *pFVar9;
  FMOD_VECTOR *pFVar10;
  Vector *pVVar11;
  int *unaff_r6;
  uint uVar12;
  int iVar13;
  undefined4 *in_stack_00000000;
  
  piVar2 = *(int **)(DAT_000a31e0 + 0xa3004);
  iVar3 = *piVar2;
  iVar13 = *(int *)(param_1 + 0x23fc);
  if (iVar13 == 0) goto LAB_000a31c8;
  if (param_2 == (Vector *)0x0) {
    uVar8 = 0;
  }
  else {
    puVar4 = *(undefined4 **)(param_1 + 0x2424);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = operator_new(0xc);
      *puVar4 = 0;
      puVar4[1] = 0;
      puVar4[2] = 0;
      *(undefined4 **)(param_1 + 0x2424) = puVar4;
    }
    uVar8 = 1;
    *puVar4 = *(undefined4 *)param_2;
    puVar4[1] = *(undefined4 *)(param_2 + 4);
    puVar4[2] = *(undefined4 *)(param_2 + 8);
  }
  if (param_3 == (Vector *)0x0) {
    pVVar11 = (Vector *)0x0;
  }
  else {
    puVar4 = *(undefined4 **)(param_1 + 0x2428);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = operator_new(0xc);
      *puVar4 = 0;
      puVar4[1] = 0;
      puVar4[2] = 0;
      *(undefined4 **)(param_1 + 0x2428) = puVar4;
    }
    pVVar11 = (Vector *)0x1;
    *puVar4 = *(undefined4 *)param_3;
    puVar4[1] = *(undefined4 *)(param_3 + 4);
    puVar4[2] = *(undefined4 *)(param_3 + 8);
  }
  if (param_4 == (Vector *)0x0) {
    uVar12 = 0;
  }
  else {
    puVar4 = *(undefined4 **)(param_1 + 0x242c);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = operator_new(0xc);
      *puVar4 = 0;
      puVar4[1] = 0;
      puVar4[2] = 0;
      *(undefined4 **)(param_1 + 0x242c) = puVar4;
    }
    uVar12 = 1;
    *puVar4 = *(undefined4 *)param_4;
    puVar4[1] = *(undefined4 *)(param_4 + 4);
    puVar4[2] = *(undefined4 *)(param_4 + 8);
  }
  if (in_stack_00000000 == (undefined4 *)0x0) {
    if ((uVar8 | (uint)pVVar11 | uVar12) == 1) {
      bVar1 = false;
      goto LAB_000a3114;
    }
  }
  else {
    puVar4 = *(undefined4 **)(param_1 + 0x2430);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = operator_new(0xc);
      *puVar4 = 0;
      puVar4[1] = 0;
      puVar4[2] = 0;
      *(undefined4 **)(param_1 + 0x2430) = puVar4;
    }
    *puVar4 = *in_stack_00000000;
    puVar4[1] = in_stack_00000000[1];
    puVar4[2] = in_stack_00000000[2];
    bVar1 = true;
LAB_000a3114:
    if (uVar8 == 0) {
      pFVar9 = (FMOD_VECTOR *)0x0;
    }
    else {
      pFVar9 = *(FMOD_VECTOR **)(param_1 + 0x2424);
    }
    if (bVar1) {
      pFVar10 = *(FMOD_VECTOR **)(param_1 + 0x2430);
    }
    else {
      pFVar10 = (FMOD_VECTOR *)0x0;
    }
    if (pVVar11 == (Vector *)0x0) {
      pFVar5 = (FMOD_VECTOR *)0x0;
    }
    else {
      pFVar5 = *(FMOD_VECTOR **)(param_1 + 0x2428);
    }
    FMOD::EventSystem::set3DListenerAttributes(iVar13,(FMOD_VECTOR *)0x0,pFVar9,pFVar10,pFVar5);
  }
  FMOD::EventSystem::update();
  for (iVar13 = 0; iVar13 != 5; iVar13 = iVar13 + 1) {
    iVar7 = *(int *)(param_1 + iVar13 * 4 + 0x2410);
    iVar6 = 0;
    if (iVar7 != -1) {
      pVVar11 = param_1 + iVar7 * 4 + 0x18;
      iVar6 = *(int *)pVVar11;
    }
    if (((iVar7 != -1 && iVar6 != 0) && (iVar6 = isPlaying((int)param_1), iVar6 == 0)) &&
       (iVar6 = FMOD::Event::getParentGroup(*(EventGroup ***)pVVar11), iVar6 == 0)) {
      (**(code **)(*unaff_r6 + 8))(unaff_r6,*(int *)pVVar11,0);
      *(int *)pVVar11 = 0;
      *(undefined4 *)(param_1 + iVar13 * 4 + 0x2410) = 0xffffffff;
    }
  }
LAB_000a31c8:
  if (*piVar2 != iVar3) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a2ff0: push {r4,r5,r6,r7,lr}
  000a2ff2: add r7,sp,#0xc
  000a2ff4: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000a2ff8: mov r10,r0
  000a2ffa: ldr r0,[0x000a31e0]
  000a2ffc: mov r9,r3
  000a2ffe: mov r5,r2
  000a3000: add r0,pc
  000a3002: mov r6,r1
  000a3004: ldr r0,[r0,#0x0]
  000a3006: str r0,[sp,#0x8]
  000a3008: ldr r0,[r0,#0x0]
  000a300a: str r0,[sp,#0x10]
  000a300c: movw r0,#0x23fc
  000a3010: ldr.w r8,[r10,r0]
  000a3014: cmp.w r8,#0x0
  000a3018: beq.w 0x000a31c8
  000a301c: cbz r6,0x000a304c
  000a301e: movw r1,#0x2424
  000a3022: ldr.w r0,[r10,r1]
  000a3026: cbnz r0,0x000a303c
  000a3028: movs r0,#0xc
  000a302a: add.w r4,r10,r1
  000a302e: blx 0x0006eb24
  000a3032: movs r1,#0x0
  000a3034: strd r1,r1,[r0,#0x0]
  000a3038: str r1,[r0,#0x8]
  000a303a: str r0,[r4,#0x0]
  000a303c: ldr r1,[r6,#0x0]
  000a303e: movs r2,#0x1
  000a3040: str r1,[r0,#0x0]
  000a3042: ldr r1,[r6,#0x4]
  000a3044: str r1,[r0,#0x4]
  000a3046: ldr r1,[r6,#0x8]
  000a3048: str r1,[r0,#0x8]
  000a304a: b 0x000a304e
  000a304c: movs r2,#0x0
  000a304e: cbz r5,0x000a3082
  000a3050: movw r1,#0x2428
  000a3054: ldr.w r0,[r10,r1]
  000a3058: cbnz r0,0x000a3072
  000a305a: movs r0,#0xc
  000a305c: add.w r4,r10,r1
  000a3060: mov r6,r2
  000a3062: blx 0x0006eb24
  000a3066: mov r2,r6
  000a3068: movs r1,#0x0
  000a306a: strd r1,r1,[r0,#0x0]
  000a306e: str r1,[r0,#0x8]
  000a3070: str r0,[r4,#0x0]
  000a3072: ldr r1,[r5,#0x0]
  000a3074: movs r4,#0x1
  000a3076: str r1,[r0,#0x0]
  000a3078: ldr r1,[r5,#0x4]
  000a307a: str r1,[r0,#0x4]
  000a307c: ldr r1,[r5,#0x8]
  000a307e: str r1,[r0,#0x8]
  000a3080: b 0x000a3084
  000a3082: movs r4,#0x0
  000a3084: ldr.w r11,[r7,#0x8]
  000a3088: cmp.w r9,#0x0
  000a308c: beq 0x000a30c6
  000a308e: movw r1,#0x242c
  000a3092: ldr.w r0,[r10,r1]
  000a3096: cbnz r0,0x000a30b0
  000a3098: movs r0,#0xc
  000a309a: add.w r5,r10,r1
  000a309e: mov r6,r2
  000a30a0: blx 0x0006eb24
  000a30a4: mov r2,r6
  000a30a6: movs r1,#0x0
  000a30a8: strd r1,r1,[r0,#0x0]
  000a30ac: str r1,[r0,#0x8]
  000a30ae: str r0,[r5,#0x0]
  000a30b0: ldr.w r1,[r9,#0x0]
  000a30b4: movs r6,#0x1
  000a30b6: str r1,[r0,#0x0]
  000a30b8: ldr.w r1,[r9,#0x4]
  000a30bc: str r1,[r0,#0x4]
  000a30be: ldr.w r1,[r9,#0x8]
  000a30c2: str r1,[r0,#0x8]
  000a30c4: b 0x000a30c8
  000a30c6: movs r6,#0x0
  000a30c8: cmp.w r11,#0x0
  000a30cc: beq 0x000a3108
  000a30ce: movw r1,#0x2430
  000a30d2: ldr.w r0,[r10,r1]
  000a30d6: cbnz r0,0x000a30f2
  000a30d8: movs r0,#0xc
  000a30da: add.w r9,r10,r1
  000a30de: mov r5,r2
  000a30e0: blx 0x0006eb24
  000a30e4: mov r2,r5
  000a30e6: movs r1,#0x0
  000a30e8: strd r1,r1,[r0,#0x0]
  000a30ec: str r1,[r0,#0x8]
  000a30ee: str.w r0,[r9,#0x0]
  000a30f2: ldr.w r1,[r11,#0x0]
  000a30f6: str r1,[r0,#0x0]
  000a30f8: ldr.w r1,[r11,#0x4]
  000a30fc: str r1,[r0,#0x4]
  000a30fe: ldr.w r1,[r11,#0x8]
  000a3102: str r1,[r0,#0x8]
  000a3104: movs r0,#0x1
  000a3106: b 0x000a3114
  000a3108: orr.w r0,r2,r4
  000a310c: orrs r0,r6
  000a310e: cmp r0,#0x1
  000a3110: bne 0x000a3164
  000a3112: movs r0,#0x0
  000a3114: cbz r2,0x000a3120
  000a3116: movw r1,#0x2424
  000a311a: ldr.w r2,[r10,r1]
  000a311e: b 0x000a3122
  000a3120: movs r2,#0x0
  000a3122: cbz r0,0x000a312e
  000a3124: movw r0,#0x2430
  000a3128: ldr.w r3,[r10,r0]
  000a312c: b 0x000a3130
  000a312e: movs r3,#0x0
  000a3130: movw r0,#0x23fc
  000a3134: add.w r5,r10,r0
  000a3138: cbz r4,0x000a3144
  000a313a: movw r0,#0x2428
  000a313e: ldr.w r0,[r10,r0]
  000a3142: b 0x000a3146
  000a3144: movs r0,#0x0
  000a3146: cbz r6,0x000a3152
  000a3148: movw r1,#0x242c
  000a314c: ldr.w r1,[r10,r1]
  000a3150: b 0x000a3154
  000a3152: movs r1,#0x0
  000a3154: strd r0,r1,[sp,#0x0]
  000a3158: mov r0,r8
  000a315a: movs r1,#0x0
  000a315c: blx 0x00071524
  000a3160: ldr.w r8,[r5,#0x0]
  000a3164: mov r0,r8
  000a3166: blx 0x00071530
  000a316a: movw r0,#0x2410
  000a316e: add.w r6,r10,r0
  000a3172: add.w r11,sp,#0xc
  000a3176: mov.w r8,#0x0
  000a317a: mov.w r9,#0xffffffff
  000a317e: movs r5,#0x0
  000a3180: cmp r5,#0x5
  000a3182: beq 0x000a31c8
  000a3184: ldr.w r1,[r6,r5,lsl #0x2]
  000a3188: adds r0,r1,#0x1
  000a318a: ittt ne
  000a318c: add.ne.w r4,r10,r1, lsl #0x2
  000a3190: ldr.ne.w r0,[r4,#0x18]!
  000a3194: cmp.ne r0,#0x0
  000a3196: bne 0x000a319c
  000a3198: adds r5,#0x1
  000a319a: b 0x000a3180
  000a319c: mov r0,r10
  000a319e: blx 0x0007153c
  000a31a2: cmp r0,#0x0
  000a31a4: bne 0x000a3198
  000a31a6: ldr r0,[r4,#0x0]
  000a31a8: mov r1,r11
  000a31aa: blx 0x000714dc
  000a31ae: cmp r0,#0x0
  000a31b0: bne 0x000a3198
  000a31b2: ldr r0,[sp,#0xc]
  000a31b4: ldr r1,[r4,#0x0]
  000a31b6: ldr r2,[r0,#0x0]
  000a31b8: ldr r3,[r2,#0x8]
  000a31ba: movs r2,#0x0
  000a31bc: blx r3
  000a31be: str.w r8,[r4,#0x0]
  000a31c2: str.w r9,[r6,r5,lsl #0x2]
  000a31c6: b 0x000a3198
  000a31c8: ldr r0,[sp,#0x10]
  000a31ca: ldr r1,[sp,#0x8]
  000a31cc: ldr r1,[r1,#0x0]
  000a31ce: subs r0,r1,r0
  000a31d0: ittt eq
  000a31d2: add.eq sp,#0x14
  000a31d4: pop.eq.w {r8,r9,r10,r11}
  000a31d8: pop.eq {r4,r5,r6,r7,pc}
  000a31da: blx 0x0006e824
```
