# FModSound::updateEvent3DAttributes
elf 0x935a8 body 578
Sig: undefined __thiscall updateEvent3DAttributes(FModSound * this, Event * param_1, int param_2, Vector * param_3, Vector * param_4, bool param_5)

## decompile
```c

/* FModSound::updateEvent3DAttributes(FMOD::Event*, int, AbyssEngine::AEMath::Vector*,
   AbyssEngine::AEMath::Vector*, bool) */

void __thiscall
FModSound::updateEvent3DAttributes
          (FModSound *this,Event *param_1,int param_2,Vector *param_3,Vector *param_4,bool param_5)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  FMOD_VECTOR *pFVar5;
  FMOD_VECTOR *pFVar6;
  int unaff_r4;
  FMOD_VECTOR *unaff_r5;
  undefined4 *unaff_r6;
  int *piVar7;
  undefined3 in_stack_00000005;
  Vector *local_38;
  
  piVar7 = *(int **)(DAT_000a37ec + 0xa35bc);
  iVar2 = *piVar7;
  local_38 = param_3;
  if ((*(int *)(this + 0x2404) != 0) && (this[0x11] != (FModSound)0x0)) {
    if ((param_1 == (Event *)0x0) ||
       (iVar3 = FMOD::Event::getCategory((EventCategory **)param_1), iVar3 == 0x24)) {
      iVar3 = FMOD::EventSystem::getEventBySystemID
                        (*(uint *)(this + 0x23fc),param_2,(Event **)&DAT_00000004);
      if ((((iVar3 == 0) &&
           (iVar3 = FMOD::Event::getCategory((EventCategory **)unaff_r5), iVar3 == 0)) &&
          (iVar3 = (**(code **)*unaff_r6)(unaff_r6,&local_38,&stack0xffffffcc), iVar3 == 0)) &&
         (local_38 = local_38 + 1, (local_38 + (int)this)[0x11] != (Vector)0x0)) {
        if (param_3 == (Vector *)0x0) {
          bVar1 = false;
        }
        else {
          puVar4 = *(undefined4 **)(this + 0x2434);
          if (puVar4 == (undefined4 *)0x0) {
            puVar4 = operator_new(0xc);
            *puVar4 = 0;
            puVar4[1] = 0;
            puVar4[2] = 0;
            *(undefined4 **)(this + 0x2434) = puVar4;
          }
          bVar1 = true;
          *puVar4 = *(undefined4 *)param_3;
          puVar4[1] = *(undefined4 *)(param_3 + 4);
          puVar4[2] = *(undefined4 *)(param_3 + 8);
        }
        if (param_4 == (Vector *)0x0) {
          if (!bVar1) goto LAB_000a366c;
          pFVar5 = *(FMOD_VECTOR **)(this + 0x2434);
          pFVar6 = (FMOD_VECTOR *)0x0;
        }
        else {
          puVar4 = *(undefined4 **)(this + 0x2438);
          if (puVar4 == (undefined4 *)0x0) {
            puVar4 = operator_new(0xc);
            *puVar4 = 0;
            puVar4[1] = 0;
            puVar4[2] = 0;
            *(undefined4 **)(this + 0x2438) = puVar4;
          }
          *puVar4 = *(undefined4 *)param_4;
          puVar4[1] = *(undefined4 *)(param_4 + 4);
          puVar4[2] = *(undefined4 *)(param_4 + 8);
          if (bVar1) {
            pFVar5 = *(FMOD_VECTOR **)(this + 0x2434);
          }
          else {
            pFVar5 = (FMOD_VECTOR *)0x0;
          }
          pFVar6 = *(FMOD_VECTOR **)(this + 0x2438);
        }
        iVar3 = FMOD::Event::set3DAttributes(unaff_r5,pFVar5,pFVar6);
        if ((iVar3 == 0) &&
           (iVar3 = FMOD::EventSystem::getEventBySystemID
                              (*(uint *)(this + 0x23fc),param_2,(Event **)0x0), iVar3 == 0)) {
          FMOD::Event::start();
        }
      }
    }
    else if (((iVar3 == 0) &&
             (iVar3 = (**(code **)*unaff_r6)(unaff_r6,&stack0xffffffcc,&stack0xffffffd0), iVar3 == 0
             )) && (this[unaff_r4 + 0x12] != (FModSound)0x0)) {
      if (param_3 == (Vector *)0x0) {
        bVar1 = false;
      }
      else {
        puVar4 = *(undefined4 **)(this + 0x2434);
        if (puVar4 == (undefined4 *)0x0) {
          puVar4 = operator_new(0xc);
          *puVar4 = 0;
          puVar4[1] = 0;
          puVar4[2] = 0;
          *(undefined4 **)(this + 0x2434) = puVar4;
        }
        bVar1 = true;
        *puVar4 = *(undefined4 *)param_3;
        puVar4[1] = *(undefined4 *)(param_3 + 4);
        puVar4[2] = *(undefined4 *)(param_3 + 8);
      }
      if (param_4 == (Vector *)0x0) {
        if (!bVar1) goto LAB_000a366c;
        pFVar6 = (FMOD_VECTOR *)0x0;
        pFVar5 = *(FMOD_VECTOR **)(this + 0x2434);
      }
      else {
        puVar4 = *(undefined4 **)(this + 0x2438);
        if (puVar4 == (undefined4 *)0x0) {
          puVar4 = operator_new(0xc);
          *puVar4 = 0;
          puVar4[1] = 0;
          puVar4[2] = 0;
          *(undefined4 **)(this + 0x2438) = puVar4;
        }
        *puVar4 = *(undefined4 *)param_4;
        puVar4[1] = *(undefined4 *)(param_4 + 4);
        puVar4[2] = *(undefined4 *)(param_4 + 8);
        if (bVar1) {
          pFVar5 = *(FMOD_VECTOR **)(this + 0x2434);
        }
        else {
          pFVar5 = (FMOD_VECTOR *)0x0;
        }
        pFVar6 = *(FMOD_VECTOR **)(this + 0x2438);
      }
      iVar3 = FMOD::Event::set3DAttributes((FMOD_VECTOR *)param_1,pFVar5,pFVar6);
      if ((iVar3 == 0) && (_param_5 != 0)) {
        FMOD::Event::getState((uint *)param_1);
        if ((int)local_38 << 0x1c < 0) {
          FMOD::Event::stop(SUB41(param_1,0));
        }
        FMOD::Event::start();
      }
    }
  }
LAB_000a366c:
  iVar2 = *piVar7 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  000a35a8: push {r4,r5,r6,r7,lr}
  000a35aa: add r7,sp,#0xc
  000a35ac: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000a35b0: mov r4,r0
  000a35b2: ldr r0,[0x000a37ec]
  000a35b4: mov r9,r3
  000a35b6: mov r8,r2
  000a35b8: add r0,pc
  000a35ba: mov r5,r1
  000a35bc: ldr r6,[r0,#0x0]
  000a35be: ldr r0,[r6,#0x0]
  000a35c0: str r0,[sp,#0x10]
  000a35c2: movw r0,#0x2404
  000a35c6: ldr r0,[r4,r0]
  000a35c8: cmp r0,#0x0
  000a35ca: beq 0x000a366c
  000a35cc: ldrb r0,[r4,#0x11]
  000a35ce: cmp r0,#0x0
  000a35d0: beq 0x000a366c
  000a35d2: ldr.w r10,[r7,#0x8]
  000a35d6: cbz r5,0x000a363e
  000a35d8: add r1,sp,#0xc
  000a35da: mov r0,r5
  000a35dc: blx 0x0007156c
  000a35e0: cmp r0,#0x24
  000a35e2: beq 0x000a363e
  000a35e4: cmp r0,#0x0
  000a35e6: bne 0x000a366a
  000a35e8: ldr r0,[sp,#0xc]
  000a35ea: add r2,sp,#0x8
  000a35ec: ldr r1,[r0,#0x0]
  000a35ee: ldr r3,[r1,#0x0]
  000a35f0: add r1,sp,#0x4
  000a35f2: blx r3
  000a35f4: cbnz r0,0x000a366c
  000a35f6: ldr r0,[sp,#0x4]
  000a35f8: adds r0,#0x1
  000a35fa: str r0,[sp,#0x4]
  000a35fc: add r0,r4
  000a35fe: ldrb r0,[r0,#0x11]
  000a3600: cbz r0,0x000a366c
  000a3602: cmp.w r9,#0x0
  000a3606: beq 0x000a36c6
  000a3608: movw r1,#0x2434
  000a360c: ldr r0,[r4,r1]
  000a360e: cbnz r0,0x000a3626
  000a3610: movs r0,#0xc
  000a3612: add.w r8,r4,r1
  000a3616: blx 0x0006eb24
  000a361a: movs r1,#0x0
  000a361c: strd r1,r1,[r0,#0x0]
  000a3620: str r1,[r0,#0x8]
  000a3622: str.w r0,[r8,#0x0]
  000a3626: ldr.w r1,[r9,#0x0]
  000a362a: mov.w r8,#0x1
  000a362e: str r1,[r0,#0x0]
  000a3630: ldr.w r1,[r9,#0x4]
  000a3634: str r1,[r0,#0x4]
  000a3636: ldr.w r1,[r9,#0x8]
  000a363a: str r1,[r0,#0x8]
  000a363c: b 0x000a36ca
  000a363e: movw r11,#0x23fc
  000a3642: add r3,sp,#0x8
  000a3644: ldr.w r0,[r4,r11]
  000a3648: mov r1,r8
  000a364a: movs r2,#0x4
  000a364c: blx 0x00071554
  000a3650: cbnz r0,0x000a366a
  000a3652: ldr r0,[sp,#0x8]
  000a3654: add r1,sp,#0xc
  000a3656: blx 0x0007156c
  000a365a: cbnz r0,0x000a366a
  000a365c: ldr r0,[sp,#0xc]
  000a365e: add r2,sp,#0x4
  000a3660: ldr r1,[r0,#0x0]
  000a3662: ldr r3,[r1,#0x0]
  000a3664: mov r1,sp
  000a3666: blx r3
  000a3668: cbz r0,0x000a3682
  000a366a: movs r5,#0x0
  000a366c: ldr r0,[sp,#0x10]
  000a366e: ldr r1,[r6,#0x0]
  000a3670: subs r0,r1,r0
  000a3672: itttt eq
  000a3674: mov.eq r0,r5
  000a3676: add.eq sp,#0x14
  000a3678: pop.eq.w {r8,r9,r10,r11}
  000a367c: pop.eq {r4,r5,r6,r7,pc}
  000a367e: blx 0x0006e824
  000a3682: ldr r0,[sp,#0x0]
  000a3684: adds r0,#0x1
  000a3686: str r0,[sp,#0x0]
  000a3688: add r0,r4
  000a368a: ldrb r0,[r0,#0x11]
  000a368c: cmp r0,#0x0
  000a368e: beq 0x000a366a
  000a3690: cmp.w r9,#0x0
  000a3694: beq 0x000a375c
  000a3696: movw r1,#0x2434
  000a369a: ldr r0,[r4,r1]
  000a369c: cbnz r0,0x000a36b0
  000a369e: movs r0,#0xc
  000a36a0: adds r5,r4,r1
  000a36a2: blx 0x0006eb24
  000a36a6: movs r1,#0x0
  000a36a8: strd r1,r1,[r0,#0x0]
  000a36ac: str r1,[r0,#0x8]
  000a36ae: str r0,[r5,#0x0]
  000a36b0: ldr.w r1,[r9,#0x0]
  000a36b4: movs r5,#0x1
  000a36b6: str r1,[r0,#0x0]
  000a36b8: ldr.w r1,[r9,#0x4]
  000a36bc: str r1,[r0,#0x4]
  000a36be: ldr.w r1,[r9,#0x8]
  000a36c2: str r1,[r0,#0x8]
  000a36c4: b 0x000a375e
  000a36c6: mov.w r8,#0x0
  000a36ca: cmp.w r10,#0x0
  000a36ce: beq 0x000a370e
  000a36d0: movw r1,#0x2438
  000a36d4: ldr r0,[r4,r1]
  000a36d6: cbnz r0,0x000a36ee
  000a36d8: movs r0,#0xc
  000a36da: add.w r9,r4,r1
  000a36de: blx 0x0006eb24
  000a36e2: movs r1,#0x0
  000a36e4: strd r1,r1,[r0,#0x0]
  000a36e8: str r1,[r0,#0x8]
  000a36ea: str.w r0,[r9,#0x0]
  000a36ee: ldr.w r1,[r10,#0x0]
  000a36f2: cmp.w r8,#0x0
  000a36f6: str r1,[r0,#0x0]
  000a36f8: ldr.w r1,[r10,#0x4]
  000a36fc: str r1,[r0,#0x4]
  000a36fe: ldr.w r1,[r10,#0x8]
  000a3702: str r1,[r0,#0x8]
  000a3704: beq 0x000a371e
  000a3706: movw r0,#0x2434
  000a370a: ldr r1,[r4,r0]
  000a370c: b 0x000a3720
  000a370e: cmp.w r8,#0x0
  000a3712: beq 0x000a366a
  000a3714: movw r0,#0x2434
  000a3718: movs r2,#0x0
  000a371a: ldr r1,[r4,r0]
  000a371c: b 0x000a3726
  000a371e: movs r1,#0x0
  000a3720: movw r0,#0x2438
  000a3724: ldr r2,[r4,r0]
  000a3726: mov r0,r5
  000a3728: movs r3,#0x0
  000a372a: blx 0x00071578
  000a372e: cbnz r0,0x000a3754
  000a3730: ldr r1,[r7,#0xc]
  000a3732: cbz r1,0x000a3754
  000a3734: mov r1,sp
  000a3736: mov r0,r5
  000a3738: blx 0x000714e8
  000a373c: ldrb.w r0,[sp,#0x0]
  000a3740: lsls r0,r0,#0x1c
  000a3742: bpl 0x000a374c
  000a3744: mov r0,r5
  000a3746: movs r1,#0x1
  000a3748: blx 0x000715a8
  000a374c: mov r0,r5
  000a374e: blx 0x00071590
  000a3752: b 0x000a366c
  000a3754: cmp r0,#0x0
  000a3756: bne.w 0x000a366a
  000a375a: b 0x000a366c
  000a375c: movs r5,#0x0
  000a375e: cmp.w r10,#0x0
  000a3762: beq 0x000a37a2
  000a3764: movw r1,#0x2438
  000a3768: ldr r0,[r4,r1]
  000a376a: cbnz r0,0x000a3782
  000a376c: movs r0,#0xc
  000a376e: add.w r9,r4,r1
  000a3772: blx 0x0006eb24
  000a3776: movs r1,#0x0
  000a3778: strd r1,r1,[r0,#0x0]
  000a377c: str r1,[r0,#0x8]
  000a377e: str.w r0,[r9,#0x0]
  000a3782: ldr.w r1,[r10,#0x0]
  000a3786: cmp r5,#0x0
  000a3788: str r1,[r0,#0x0]
  000a378a: ldr.w r1,[r10,#0x4]
  000a378e: str r1,[r0,#0x4]
  000a3790: ldr.w r1,[r10,#0x8]
  000a3794: str r1,[r0,#0x8]
  000a3796: ldr r0,[sp,#0x8]
  000a3798: beq 0x000a37b4
  000a379a: movw r1,#0x2434
  000a379e: ldr r1,[r4,r1]
  000a37a0: b 0x000a37b6
  000a37a2: cmp r5,#0x0
  000a37a4: beq.w 0x000a366a
  000a37a8: movw r1,#0x2434
  000a37ac: ldr r0,[sp,#0x8]
  000a37ae: ldr r1,[r4,r1]
  000a37b0: movs r2,#0x0
  000a37b2: b 0x000a37bc
  000a37b4: movs r1,#0x0
  000a37b6: movw r2,#0x2438
  000a37ba: ldr r2,[r4,r2]
  000a37bc: movs r3,#0x0
  000a37be: movs r5,#0x0
  000a37c0: blx 0x00071578
  000a37c4: cmp r0,#0x0
  000a37c6: bne.w 0x000a366c
  000a37ca: add.w r0,r4,r11
  000a37ce: add r3,sp,#0x8
  000a37d0: mov r1,r8
  000a37d2: movs r2,#0x0
  000a37d4: ldr r0,[r0,#0x0]
  000a37d6: blx 0x00071554
  000a37da: cmp r0,#0x0
  000a37dc: bne.w 0x000a366a
  000a37e0: ldr r0,[sp,#0x8]
  000a37e2: blx 0x00071590
  000a37e6: ldr r5,[sp,#0x8]
  000a37e8: b 0x000a366c
```
