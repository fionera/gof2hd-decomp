# ChoiceWindow::setMiscButton
elf 0x147068 body 304
Sig: undefined __thiscall setMiscButton(ChoiceWindow * this, String * param_1)

## decompile
```c

/* ChoiceWindow::setMiscButton(AbyssEngine::String const&) */

void __thiscall ChoiceWindow::setMiscButton(ChoiceWindow *this,String *param_1)

{
  ChoiceWindow *pCVar1;
  int iVar2;
  int iVar3;
  TouchButton *this_00;
  int iVar4;
  int *piVar5;
  
  if ((*(int *)(this + 0x10) == 0) || (*(int *)(this + 0x14) == 0)) {
    iVar4 = *(int *)(this + 0x48);
    iVar3 = **(int **)(DAT_001571a8 + 0x1570ac);
    iVar2 = iVar4 + *(int *)(iVar3 + 0x40) * 2;
  }
  else {
    iVar2 = TouchButton::getWidth();
    iVar3 = TouchButton::getWidth();
    iVar4 = *(int *)(this + 0x48);
    iVar2 = iVar3 + iVar2 + iVar4;
    iVar3 = **(int **)(DAT_001571a4 + 0x157096);
  }
  this_00 = operator_new(200);
  TouchButton::TouchButton
            (this_00,param_1,0,(*(int *)this + *(int *)(this + 8) / 2) - *(int *)(iVar3 + 0x4c) / 2,
             (*(int *)(this + 4) + *(int *)(this + 0xc)) - iVar4,iVar2,'\"','\x04');
  iVar4 = DAT_001571ac;
  *(TouchButton **)(this + 0x18) = this_00;
  pCVar1 = this + 0x48;
  piVar5 = *(int **)(iVar4 + 0x157100);
  setHeight(this,(*(int *)(this + 0xc) - *(int *)(*piVar5 + 8)) + *(int *)pCVar1 * 2 +
                 *(int *)(*piVar5 + 0x30));
  TouchButton::setPosition
            (*(TouchButton **)(this + 0x18),*(int *)this + *(int *)(this + 8) / 2,
             (*(int *)(this + 4) + *(int *)(this + 0xc)) - *(int *)(this + 0x48),'$');
  if (*(TouchButton **)(this + 0x10) != (TouchButton *)0x0) {
    TouchButton::translate
              (*(TouchButton **)(this + 0x10),0,-(*(int *)(*piVar5 + 0x30) + *(int *)pCVar1));
  }
  if (*(TouchButton **)(this + 0x14) != (TouchButton *)0x0) {
    TouchButton::translate
              (*(TouchButton **)(this + 0x14),0,-(*(int *)(*piVar5 + 0x30) + *(int *)pCVar1));
  }
  (*(code *)(DAT_001ac474 + 0x1ac478))
            (*(undefined4 *)(this + 0x1c),
             (**(int **)(DAT_001571b0 + 0x15716e) / 2 - *(int *)(this + 0xc) / 2) +
             *(int *)(*piVar5 + 8));
  return;
}

```

## target disasm
```
  00157068: push {r4,r5,r6,r7,lr}
  0015706a: add r7,sp,#0xc
  0015706c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00157070: mov r4,r0
  00157072: ldr r0,[r0,#0x10]
  00157074: str r1,[sp,#0x10]
  00157076: cbz r0,0x001570a0
  00157078: ldr r1,[r4,#0x14]
  0015707a: cbz r1,0x001570a0
  0015707c: blx 0x00074da0
  00157080: mov r6,r0
  00157082: ldr r0,[r4,#0x14]
  00157084: blx 0x00074da0
  00157088: ldr r1,[0x001571a4]
  0015708a: mov r9,r4
  0015708c: ldr.w r8,[r9,#0x48]!
  00157090: add r0,r6
  00157092: add r1,pc
  00157094: add.w r10,r0,r8
  00157098: ldr r1,[r1,#0x0]
  0015709a: ldr.w r11,[r1,#0x0]
  0015709e: b 0x001570b8
  001570a0: ldr r0,[0x001571a8]
  001570a2: mov r9,r4
  001570a4: ldr.w r8,[r9,#0x48]!
  001570a8: add r0,pc
  001570aa: ldr r0,[r0,#0x0]
  001570ac: ldr.w r11,[r0,#0x0]
  001570b0: ldr.w r0,[r11,#0x40]
  001570b4: add.w r10,r8,r0, lsl #0x1
  001570b8: movs r0,#0xc8
  001570ba: blx 0x0006eb24
  001570be: mov r6,r0
  001570c0: ldm.w r4,{r0,r1,r2,r3}
  001570c4: ldr.w r5,[r11,#0x4c]
  001570c8: add r1,r3
  001570ca: mov.w lr,#0x22
  001570ce: sub.w r1,r1,r8
  001570d2: mov.w r12,#0x4
  001570d6: stm sp,{r1,r10,lr}
  001570da: add.w r1,r2,r2, lsr #0x1f
  001570de: movs r2,#0x0
  001570e0: add.w r0,r0,r1, asr #0x1
  001570e4: add.w r1,r5,r5, lsr #0x1f
  001570e8: str.w r12,[sp,#0xc]
  001570ec: sub.w r3,r0,r1, asr #0x1
  001570f0: ldr r1,[sp,#0x10]
  001570f2: mov r0,r6
  001570f4: blx 0x00074f50
  001570f8: ldr r0,[0x001571ac]
  001570fa: str r6,[r4,#0x18]
  001570fc: add r0,pc
  001570fe: ldr r1,[r4,#0xc]
  00157100: ldr r2,[r4,#0x48]
  00157102: ldr.w r8,[r0,#0x0]
  00157106: ldr.w r0,[r8,#0x0]
  0015710a: ldr r3,[r0,#0x8]
  0015710c: ldr r0,[r0,#0x30]
  0015710e: subs r1,r1,r3
  00157110: add.w r1,r1,r2, lsl #0x1
  00157114: add r1,r0
  00157116: mov r0,r4
  00157118: blx 0x00076d98
  0015711c: ldm.w r4,{r1,r2,r3,r6}
  00157120: ldr r5,[r4,#0x48]
  00157122: ldr r0,[r4,#0x18]
  00157124: add.w r3,r3,r3, lsr #0x1f
  00157128: add r2,r6
  0015712a: subs r2,r2,r5
  0015712c: add.w r1,r1,r3, asr #0x1
  00157130: movs r3,#0x24
  00157132: blx 0x00076d80
  00157136: ldr r0,[r4,#0x10]
  00157138: cbz r0,0x0015714e
  0015713a: ldr.w r1,[r8,#0x0]
  0015713e: ldr.w r2,[r9,#0x0]
  00157142: ldr r1,[r1,#0x30]
  00157144: add r1,r2
  00157146: rsbs r2,r1
  00157148: movs r1,#0x0
  0015714a: blx 0x00076da4
  0015714e: ldr r0,[r4,#0x14]
  00157150: cbz r0,0x00157166
  00157152: ldr.w r1,[r8,#0x0]
  00157156: ldr.w r2,[r9,#0x0]
  0015715a: ldr r1,[r1,#0x30]
  0015715c: add r1,r2
  0015715e: rsbs r2,r1
  00157160: movs r1,#0x0
  00157162: blx 0x00076da4
  00157166: ldr r0,[0x001571b0]
  00157168: ldr r2,[r4,#0xc]
  0015716a: add r0,pc
  0015716c: ldr.w r3,[r8,#0x0]
  00157170: ldr r1,[r0,#0x0]
  00157172: add.w r2,r2,r2, lsr #0x1f
  00157176: ldr r3,[r3,#0x8]
  00157178: ldr r0,[r4,#0x1c]
  0015717a: ldr r1,[r1,#0x0]
  0015717c: add.w r1,r1,r1, lsr #0x1f
  00157180: asrs r1,r1,#0x1
  00157182: sub.w r1,r1,r2, asr #0x1
  00157186: add r1,r3
  00157188: add sp,#0x14
  0015718a: pop.w {r8,r9,r10,r11}
  0015718e: pop.w {r4,r5,r6,r7,lr}
  00157192: b.w 0x001ac468
  001ac468: bx pc
```
