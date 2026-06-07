# FModSound::play
elf 0x932f4 body 548
Sig: undefined __stdcall play(int param_1, Vector * param_2, Vector * param_3, float param_4)

## decompile
```c

/* FModSound::play(int, AbyssEngine::AEMath::Vector*, AbyssEngine::AEMath::Vector*, float) */

void FModSound::play(int param_1,Vector *param_2,Vector *param_3,float param_4)

{
  int *piVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  int *piVar7;
  FMOD_VECTOR *pFVar8;
  uint uVar9;
  FMOD_VECTOR *pFVar10;
  undefined4 *in_r3;
  int *piVar11;
  bool bVar12;
  undefined4 uVar13;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  ulonglong in_d5;
  undefined8 in_d6;
  EventCategory **in_s14;
  float in_stack_00000000;
  undefined8 local_48;
  undefined8 local_40;
  EventCategory **local_38;
  int local_34;
  
  piVar11 = *(int **)(DAT_000a351c + 0xa330c);
  local_34 = *piVar11;
  local_48 = in_d5;
  local_38 = in_s14;
  if (((*(int *)(param_1 + 0x2404) != 0) && (param_2 < (Vector *)0x8f5)) &&
     (*(uint *)(param_1 + 0x23fc) != 0)) {
    piVar1 = (int *)(param_1 + (int)param_2 * 4 + 0x18);
    local_38 = (EventCategory **)*piVar1;
    bVar2 = local_38 == (EventCategory **)0x0;
    local_40 = in_d6;
    if (bVar2) {
      param_4 = (float)FMOD::EventSystem::getEventBySystemID
                                 (*(uint *)(param_1 + 0x23fc),(uint)param_2,(Event **)&DAT_00000004)
      ;
    }
    if (local_38 != (EventCategory **)0x0) {
      uVar13 = 0xbf800000;
      if (*(char *)(param_1 + 8) == '\0') {
        uVar13 = DAT_000a3518;
      }
      FMOD::Event::setPitch(local_38,uVar13,1);
      if (in_stack_00000000 != 0.0) {
        FMOD::Event::setPitch(local_38,in_stack_00000000,0);
      }
      iVar4 = FMOD::Event::getCategory(local_38);
      if (iVar4 == 0) {
        param_4 = (float)(**(code **)*local_40._4_4_)(local_40._4_4_,(int)&local_48 + 4,&local_40);
        iVar4 = local_48._4_4_;
        iVar5 = local_48._4_4_ + 1;
        bVar12 = local_48._4_4_ == 0;
        local_48 = CONCAT44(iVar5,(int)local_48);
        if (bVar12) {
          *(Vector **)param_1 = param_2;
        }
        else if (2 < iVar4) goto LAB_000a3500;
        if (*(char *)(iVar5 + param_1 + 0x11) != '\0') {
          if (param_3 == (Vector *)0x0) {
            bVar12 = false;
          }
          else {
            puVar6 = *(undefined4 **)(param_1 + 0x2434);
            if (puVar6 == (undefined4 *)0x0) {
              puVar6 = operator_new(0xc);
              *puVar6 = 0;
              puVar6[1] = 0;
              puVar6[2] = 0;
              *(undefined4 **)(param_1 + 0x2434) = puVar6;
            }
            *puVar6 = *(undefined4 *)param_3;
            puVar6[1] = *(undefined4 *)(param_3 + 4);
            bVar12 = true;
            puVar6[2] = *(undefined4 *)(param_3 + 8);
          }
          if (in_r3 == (undefined4 *)0x0) {
            bVar3 = false;
          }
          else {
            puVar6 = *(undefined4 **)(param_1 + 0x2438);
            if (puVar6 == (undefined4 *)0x0) {
              puVar6 = operator_new(0xc);
              *puVar6 = 0;
              puVar6[1] = 0;
              puVar6[2] = 0;
              *(undefined4 **)(param_1 + 0x2438) = puVar6;
            }
            *puVar6 = *in_r3;
            puVar6[1] = in_r3[1];
            bVar3 = true;
            puVar6[2] = in_r3[2];
          }
          if (bVar2) {
            iVar4 = FMOD::EventSystem::getEventBySystemID
                              (*(uint *)(param_1 + 0x23fc),(uint)param_2,(Event **)0x0);
            param_4 = extraout_s0_01;
            if (iVar4 != 0) goto LAB_000a3500;
            *piVar1 = (int)local_38;
          }
          if ((bool)(bVar12 | bVar3)) {
            if (bVar12) {
              pFVar8 = *(FMOD_VECTOR **)(param_1 + 0x2434);
            }
            else {
              pFVar8 = (FMOD_VECTOR *)0x0;
            }
            if (bVar3) {
              pFVar10 = *(FMOD_VECTOR **)(param_1 + 0x2438);
            }
            else {
              pFVar10 = (FMOD_VECTOR *)0x0;
            }
            FMOD::Event::set3DAttributes((FMOD_VECTOR *)*piVar1,pFVar8,pFVar10);
          }
          local_48 = local_48 & 0xffffffff00000000;
          iVar4 = FMOD::Event::getProperty
                            ((char *)*piVar1,(void *)**(undefined4 **)(DAT_000a3520 + 0xa34ca),
                             (bool)((char)&stack0xffffffe0 + -0x28));
          if ((iVar4 == 0) && ((int)local_48 == 1)) {
            uVar9 = 0;
            piVar1 = (int *)(param_1 + 0x2410);
            do {
              piVar7 = piVar1;
              if (4 < uVar9) goto LAB_000a34f8;
              uVar9 = uVar9 + 1;
              piVar1 = piVar7 + 1;
            } while (*piVar7 != -1);
            *piVar7 = (int)param_2;
          }
LAB_000a34f8:
          param_4 = (float)FMOD::Event::start();
        }
      }
      else {
        param_4 = extraout_s0;
        if (((EventGroup **)*piVar1 != (EventGroup **)0x0) &&
           (iVar4 = FMOD::Event::getParentGroup((EventGroup **)*piVar1), param_4 = extraout_s0_00,
           iVar4 == 0)) {
          param_4 = (float)(**(code **)(*(int *)local_40 + 8))((int *)local_40,*piVar1,0);
          *piVar1 = 0;
        }
      }
    }
  }
LAB_000a3500:
  if (*piVar11 == local_34) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_4);
}

```

## target disasm
```
  000a32f4: push {r4,r5,r6,r7,lr}
  000a32f6: add r7,sp,#0xc
  000a32f8: push {r7,r8,r9,r10,r11}
  000a32fc: vpush {d4,d5,d6,d7,d8}
  000a3300: mov r5,r0
  000a3302: ldr r0,[0x000a351c]
  000a3304: mov r8,r3
  000a3306: mov r9,r2
  000a3308: add r0,pc
  000a330a: mov r4,r1
  000a330c: ldr r6,[r0,#0x0]
  000a330e: ldr r0,[r6,#0x0]
  000a3310: str r0,[sp,#0x1c]
  000a3312: movw r0,#0x2404
  000a3316: ldr r0,[r5,r0]
  000a3318: cmp r0,#0x0
  000a331a: beq.w 0x000a3500
  000a331e: movw r0,#0x8f4
  000a3322: cmp r4,r0
  000a3324: bhi.w 0x000a3500
  000a3328: movw r11,#0x23fc
  000a332c: ldr.w r1,[r5,r11]
  000a3330: cmp r1,#0x0
  000a3332: beq.w 0x000a3500
  000a3336: add.w r10,r5,r4, lsl #0x2
  000a333a: ldr.w r0,[r10,#0x18]!
  000a333e: str r0,[sp,#0x18]
  000a3340: cbz r0,0x000a3346
  000a3342: movs r1,#0x1
  000a3344: b 0x000a3356
  000a3346: add r3,sp,#0x18
  000a3348: mov r0,r1
  000a334a: mov r1,r4
  000a334c: movs r2,#0x4
  000a334e: blx 0x00071554
  000a3352: ldr r0,[sp,#0x18]
  000a3354: movs r1,#0x0
  000a3356: cmp r0,#0x0
  000a3358: beq.w 0x000a3500
  000a335c: str r1,[sp,#0x4]
  000a335e: vmov.f32 s0,0xbf800000
  000a3362: ldrb r1,[r5,#0x8]
  000a3364: movs r2,#0x1
  000a3366: vldr.32 s2,[pc,#0x1b0]
  000a336a: cmp r1,#0x0
  000a336c: vldr.32 s16,[r7,#0x8]
  000a3370: it eq
  000a3372: vmov.eq.f32 s0,s2
  000a3376: vmov r1,s0
  000a337a: blx 0x00071560
  000a337e: vcmpe.f32 s16,#0
  000a3382: vmrs apsr,fpscr
  000a3386: beq 0x000a3394
  000a3388: vmov r1,s16
  000a338c: ldr r0,[sp,#0x18]
  000a338e: movs r2,#0x0
  000a3390: blx 0x00071560
  000a3394: ldr r0,[sp,#0x18]
  000a3396: add r1,sp,#0x14
  000a3398: blx 0x0007156c
  000a339c: cbz r0,0x000a33ca
  000a339e: ldr.w r0,[r10,#0x0]
  000a33a2: cmp r0,#0x0
  000a33a4: beq.w 0x000a3500
  000a33a8: add r1,sp,#0x10
  000a33aa: blx 0x000714dc
  000a33ae: cmp r0,#0x0
  000a33b0: bne.w 0x000a3500
  000a33b4: ldr r0,[sp,#0x10]
  000a33b6: movs r4,#0x0
  000a33b8: ldr.w r1,[r10,#0x0]
  000a33bc: ldr r2,[r0,#0x0]
  000a33be: ldr r3,[r2,#0x8]
  000a33c0: movs r2,#0x0
  000a33c2: blx r3
  000a33c4: str.w r4,[r10,#0x0]
  000a33c8: b 0x000a3500
  000a33ca: ldr r0,[sp,#0x14]
  000a33cc: add r2,sp,#0x10
  000a33ce: ldr r1,[r0,#0x0]
  000a33d0: ldr r3,[r1,#0x0]
  000a33d2: add r1,sp,#0xc
  000a33d4: blx r3
  000a33d6: ldr r1,[sp,#0xc]
  000a33d8: adds r0,r1,#0x1
  000a33da: cmp r1,#0x0
  000a33dc: str r0,[sp,#0xc]
  000a33de: beq 0x000a33e6
  000a33e0: cmp r1,#0x2
  000a33e2: ble 0x000a33e8
  000a33e4: b 0x000a3500
  000a33e6: str r4,[r5,#0x0]
  000a33e8: add r0,r5
  000a33ea: ldrb r0,[r0,#0x11]
  000a33ec: cmp r0,#0x0
  000a33ee: beq.w 0x000a3500
  000a33f2: cmp.w r9,#0x0
  000a33f6: beq 0x000a342e
  000a33f8: movw r1,#0x2434
  000a33fc: ldr r0,[r5,r1]
  000a33fe: cbnz r0,0x000a3416
  000a3400: adds r0,r5,r1
  000a3402: str r0,[sp,#0x0]
  000a3404: movs r0,#0xc
  000a3406: blx 0x0006eb24
  000a340a: movs r1,#0x0
  000a340c: strd r1,r1,[r0,#0x0]
  000a3410: str r1,[r0,#0x8]
  000a3412: ldr r1,[sp,#0x0]
  000a3414: str r0,[r1,#0x0]
  000a3416: ldr.w r1,[r9,#0x0]
  000a341a: str r1,[r0,#0x0]
  000a341c: ldr.w r1,[r9,#0x4]
  000a3420: str r1,[r0,#0x4]
  000a3422: ldr.w r1,[r9,#0x8]
  000a3426: mov.w r9,#0x1
  000a342a: str r1,[r0,#0x8]
  000a342c: b 0x000a3432
  000a342e: mov.w r9,#0x0
  000a3432: cmp.w r8,#0x0
  000a3436: beq 0x000a346e
  000a3438: movw r1,#0x2438
  000a343c: ldr r0,[r5,r1]
  000a343e: cbnz r0,0x000a3456
  000a3440: adds r0,r5,r1
  000a3442: str r0,[sp,#0x0]
  000a3444: movs r0,#0xc
  000a3446: blx 0x0006eb24
  000a344a: movs r1,#0x0
  000a344c: strd r1,r1,[r0,#0x0]
  000a3450: str r1,[r0,#0x8]
  000a3452: ldr r1,[sp,#0x0]
  000a3454: str r0,[r1,#0x0]
  000a3456: ldr.w r1,[r8,#0x0]
  000a345a: str r1,[r0,#0x0]
  000a345c: ldr.w r1,[r8,#0x4]
  000a3460: str r1,[r0,#0x4]
  000a3462: ldr.w r1,[r8,#0x8]
  000a3466: mov.w r8,#0x1
  000a346a: str r1,[r0,#0x8]
  000a346c: b 0x000a3472
  000a346e: mov.w r8,#0x0
  000a3472: ldr r0,[sp,#0x4]
  000a3474: cbnz r0,0x000a348e
  000a3476: add.w r0,r5,r11
  000a347a: add r3,sp,#0x18
  000a347c: mov r1,r4
  000a347e: movs r2,#0x0
  000a3480: ldr r0,[r0,#0x0]
  000a3482: blx 0x00071554
  000a3486: cbnz r0,0x000a3500
  000a3488: ldr r0,[sp,#0x18]
  000a348a: str.w r0,[r10,#0x0]
  000a348e: orr.w r0,r9,r8
  000a3492: cmp r0,#0x1
  000a3494: bne 0x000a34c0
  000a3496: ldr.w r0,[r10,#0x0]
  000a349a: cmp.w r9,#0x0
  000a349e: beq 0x000a34a8
  000a34a0: movw r1,#0x2434
  000a34a4: ldr r1,[r5,r1]
  000a34a6: b 0x000a34aa
  000a34a8: movs r1,#0x0
  000a34aa: cmp.w r8,#0x0
  000a34ae: beq 0x000a34b8
  000a34b0: movw r2,#0x2438
  000a34b4: ldr r2,[r5,r2]
  000a34b6: b 0x000a34ba
  000a34b8: movs r2,#0x0
  000a34ba: movs r3,#0x0
  000a34bc: blx 0x00071578
  000a34c0: ldr r0,[0x000a3520]
  000a34c2: add r2,sp,#0x8
  000a34c4: movs r3,#0x1
  000a34c6: add r0,pc
  000a34c8: ldr r1,[r0,#0x0]
  000a34ca: movs r0,#0x0
  000a34cc: str r0,[sp,#0x8]
  000a34ce: ldr.w r0,[r10,#0x0]
  000a34d2: ldr r1,[r1,#0x0]
  000a34d4: blx 0x00071584
  000a34d8: cbnz r0,0x000a34f8
  000a34da: ldr r0,[sp,#0x8]
  000a34dc: cmp r0,#0x1
  000a34de: bne 0x000a34f8
  000a34e0: movw r0,#0x2410
  000a34e4: movs r1,#0x0
  000a34e6: add r0,r5
  000a34e8: cmp r1,#0x4
  000a34ea: bhi 0x000a34f8
  000a34ec: ldmia r0!,{r2}
  000a34ee: adds r1,#0x1
  000a34f0: adds r2,#0x1
  000a34f2: bne 0x000a34e8
  000a34f4: str.w r4,[r0,#-0x4]
  000a34f8: ldr.w r0,[r10,#0x0]
  000a34fc: blx 0x00071590
  000a3500: ldr r0,[sp,#0x1c]
  000a3502: ldr r1,[r6,#0x0]
  000a3504: subs r0,r1,r0
  000a3506: itttt eq
  000a3508: vpop.eq {d4,d5,d6,d7,d8}
  000a350c: add.eq sp,#0x4
  000a350e: pop.eq.w {r8,r9,r10,r11}
  000a3512: pop.eq {r4,r5,r6,r7,pc}
  000a3514: blx 0x0006e824
```
