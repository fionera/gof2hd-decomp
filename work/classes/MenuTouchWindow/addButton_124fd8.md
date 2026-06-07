# MenuTouchWindow::addButton
elf 0x124fd8 body 266
Sig: undefined __thiscall addButton(MenuTouchWindow * this, int param_1, String param_2, int param_3, Array * param_4, int param_5)

## decompile
```c

/* MenuTouchWindow::addButton(int, AbyssEngine::String, int, Array<TouchButton*>*, int) */

void __thiscall
MenuTouchWindow::addButton
          (MenuTouchWindow *this,int param_1,String *param_3,int param_4,Array *param_5,int param_6)

{
  int iVar1;
  TouchButton *this_00;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  float local_34;
  float local_30;
  
  piVar5 = *(int **)(DAT_001350f0 + 0x134fee);
  iVar1 = *piVar5;
  this_00 = operator_new(200);
  TouchButton::TouchButton
            (this_00,param_3,0,**(int **)(DAT_001350fc + 0x135014) / 2 - *(int *)(this + 0x1a8) / 2,
             (*(int *)(**(int **)(DAT_001350f4 + 0x13500c) + 0x30) + *(int *)(this + 0x1b0)) *
             param_4 + (param_6 - *(int *)(this + 0x1ac)) + **(int **)(DAT_001350f8 + 0x135010) / 2,
             *(int *)(this + 0x1a8),'\x11','\x04');
  *(int *)this_00 = param_1;
  *(int *)(this_00 + 4) = param_1 >> 0x1f;
  ArrayAdd<TouchButton*>(this_00,param_5);
  iVar4 = *(int *)(DAT_00135100 + 0x135076);
  iVar2 = *(int *)(DAT_00135104 + 0x13507e);
  for (uVar3 = 0; uVar3 < *(uint *)param_5; uVar3 = uVar3 + 1) {
    if (uVar3 < 10) {
      TouchButton::getPosition();
      *(int *)(iVar4 + uVar3 * 4) = (int)local_34;
      TouchButton::getPosition();
      *(int *)(iVar2 + uVar3 * 4) = (int)local_30;
    }
  }
  **(uint **)(DAT_00135108 + 0x1350ca) = *(uint *)param_5;
  if (*piVar5 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00134fd8: push {r4,r5,r6,r7,lr}
  00134fda: add r7,sp,#0xc
  00134fdc: push {r8,r9,r10,r11}
  00134fe0: sub sp,#0x24
  00134fe2: mov r5,r0
  00134fe4: ldr r0,[0x001350f0]
  00134fe6: mov r9,r3
  00134fe8: mov r8,r2
  00134fea: add r0,pc
  00134fec: mov r11,r1
  00134fee: ldr.w r10,[r0,#0x0]
  00134ff2: ldr.w r0,[r10,#0x0]
  00134ff6: str r0,[sp,#0x20]
  00134ff8: movs r0,#0xc8
  00134ffa: blx 0x0006eb24
  00134ffe: mov r6,r0
  00135000: ldr r0,[0x001350f4]
  00135002: ldr r1,[0x001350f8]
  00135004: add.w r5,r5,#0x1a8
  00135008: add r0,pc
  0013500a: ldr r2,[0x001350fc]
  0013500c: add r1,pc
  0013500e: ldmia r5,{r3,r4,r5}
  00135010: add r2,pc
  00135012: ldr r0,[r0,#0x0]
  00135014: ldr r1,[r1,#0x0]
  00135016: ldr.w r12,[r7,#0xc]
  0013501a: ldr r0,[r0,#0x0]
  0013501c: ldr r1,[r1,#0x0]
  0013501e: sub.w r4,r12,r4
  00135022: ldr r2,[r2,#0x0]
  00135024: ldr r0,[r0,#0x30]
  00135026: add.w r1,r1,r1, lsr #0x1f
  0013502a: add r0,r5
  0013502c: ldr r2,[r2,#0x0]
  0013502e: add.w r1,r4,r1, asr #0x1
  00135032: mla r0,r0,r9,r1
  00135036: movs r5,#0x11
  00135038: movs r1,#0x4
  0013503a: stm sp,{r0,r3,r5}
  0013503e: add.w r0,r3,r3, lsr #0x1f
  00135042: str r1,[sp,#0xc]
  00135044: add.w r1,r2,r2, lsr #0x1f
  00135048: movs r2,#0x0
  0013504a: asrs r1,r1,#0x1
  0013504c: sub.w r3,r1,r0, asr #0x1
  00135050: mov r0,r6
  00135052: mov r1,r8
  00135054: blx 0x00074f50
  00135058: ldr r5,[r7,#0x8]
  0013505a: asr.w r0,r11, asr #0x1f
  0013505e: strd r11,r0,[r6,#0x0]
  00135062: mov r0,r6
  00135064: mov r1,r5
  00135066: blx 0x0007648c
  0013506a: ldr r0,[0x00135100]
  0013506c: add.w r8,sp,#0x14
  00135070: movs r6,#0x0
  00135072: add r0,pc
  00135074: ldr.w r9,[r0,#0x0]
  00135078: ldr r0,[0x00135104]
  0013507a: add r0,pc
  0013507c: ldr r4,[r0,#0x0]
  0013507e: b 0x001350be
  00135080: cmp r6,#0x9
  00135082: bhi 0x001350bc
  00135084: ldr r0,[r5,#0x4]
  00135086: ldr.w r1,[r0,r6,lsl #0x2]
  0013508a: mov r0,r8
  0013508c: blx 0x00074dd0
  00135090: vldr.32 s0,[sp,#0x14]
  00135094: vcvt.s32.f32 s0,s0
  00135098: vmov r0,s0
  0013509c: str.w r0,[r9,r6,lsl #0x2]
  001350a0: ldr r0,[r5,#0x4]
  001350a2: ldr.w r1,[r0,r6,lsl #0x2]
  001350a6: mov r0,r8
  001350a8: blx 0x00074dd0
  001350ac: vldr.32 s0,[sp,#0x18]
  001350b0: vcvt.s32.f32 s0,s0
  001350b4: vmov r0,s0
  001350b8: str.w r0,[r4,r6,lsl #0x2]
  001350bc: adds r6,#0x1
  001350be: ldr r0,[r5,#0x0]
  001350c0: cmp r6,r0
  001350c2: bcc 0x00135080
  001350c4: ldr r1,[0x00135108]
  001350c6: add r1,pc
  001350c8: ldr r1,[r1,#0x0]
  001350ca: str r0,[r1,#0x0]
  001350cc: ldr r0,[sp,#0x20]
  001350ce: ldr.w r1,[r10,#0x0]
  001350d2: subs r0,r1,r0
  001350d4: ittt eq
  001350d6: add.eq sp,#0x24
  001350d8: pop.eq.w {r8,r9,r10,r11}
  001350dc: pop.eq {r4,r5,r6,r7,pc}
  001350de: blx 0x0006e824
```
