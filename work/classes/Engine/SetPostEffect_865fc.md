# Engine::SetPostEffect
elf 0x865fc body 290
Sig: undefined __thiscall SetPostEffect(Engine * this, uint param_1, bool param_2)

## decompile
```c

/* AbyssEngine::Engine::SetPostEffect(unsigned int, bool) */

void __thiscall AbyssEngine::Engine::SetPostEffect(Engine *this,uint param_1,bool param_2)

{
  int iVar1;
  FBOContainer *pFVar2;
  uint uVar3;
  char *pcVar4;
  int iVar5;
  undefined1 uVar6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_00096740 + 0x96610);
  iVar1 = *piVar7;
  if ((*(int *)(this + 0x414) == 0) && (param_2)) {
    pFVar2 = operator_new(0x38);
    String::String((String *)&stack0xffffffd0,(char *)(DAT_00096744 + 0x96632),false);
    FBOContainer::FBOContainer(pFVar2,this,&stack0xffffffd0);
    *(FBOContainer **)(this + 0x414) = pFVar2;
    String::~String((String *)&stack0xffffffd0);
    if (*(int *)(**(int **)(this + 0x30) + 0x30) == 2) {
      iVar5 = *(int *)(this + 0x368);
      uVar6 = (undefined1)*(undefined4 *)(this + 0x36c);
    }
    else {
      uVar6 = (undefined1)*(undefined4 *)(this + 0x368);
      iVar5 = *(int *)(this + 0x36c);
    }
    FBOContainer::Create(*(int *)(this + 0x414),iVar5,(bool)uVar6,true);
  }
  iVar5 = DAT_00096754;
  if (param_1 == DAT_00096738) {
    if (!param_2) {
      pcVar4 = (char *)(DAT_00096750 + 0x966e4);
      if (*pcVar4 == '\x01') {
        *(uint *)(this + 0x410) = *(uint *)(this + 0x410) & 0xfffffffb;
        *pcVar4 = '\0';
        *(undefined4 *)(iVar5 + 0x966fc) = 1;
      }
      else {
        *pcVar4 = *(int *)(DAT_00096758 + 0x96708) < 1;
      }
      goto LAB_000966c8;
    }
    iVar5 = *(int *)(DAT_0009674c + 0x9669e);
    if (0 < iVar5) {
      *(int *)(DAT_0009674c + 0x9669e) = iVar5 + -1;
      goto LAB_000966c8;
    }
    uVar3 = *(uint *)(this + 0x410) | 4;
  }
  else if (param_1 == DAT_0009673c) {
    *(bool *)*(undefined4 *)(DAT_00096748 + 0x966b2) = param_2;
    if (param_2) {
      uVar3 = *(uint *)(this + 0x410) | 2;
    }
    else {
      uVar3 = *(uint *)(this + 0x410) & 0xfffffffd;
    }
  }
  else {
    if (param_1 != 0x1400000) goto LAB_000966c8;
    if (param_2) {
      uVar3 = *(uint *)(this + 0x410) | 1;
    }
    else {
      uVar3 = *(uint *)(this + 0x410) & 0xfffffffe;
    }
  }
  *(uint *)(this + 0x410) = uVar3;
LAB_000966c8:
  if (*piVar7 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000965fc: push {r4,r5,r6,r7,lr}
  000965fe: add r7,sp,#0xc
  00096600: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  00096604: mov r4,r0
  00096606: ldr r0,[0x00096740]
  00096608: mov r8,r2
  0009660a: mov r6,r1
  0009660c: add r0,pc
  0009660e: ldr.w r9,[r0,#0x0]
  00096612: ldr.w r0,[r9,#0x0]
  00096616: str r0,[sp,#0x14]
  00096618: ldr.w r0,[r4,#0x414]
  0009661c: cbnz r0,0x0009666c
  0009661e: cmp.w r8,#0x0
  00096622: beq 0x0009666c
  00096624: movs r0,#0x38
  00096626: blx 0x0006eb24
  0009662a: ldr r1,[0x00096744]
  0009662c: mov r5,r0
  0009662e: add r1,pc
  00096630: add r0,sp,#0x8
  00096632: movs r2,#0x0
  00096634: blx 0x0006ee18
  00096638: add r2,sp,#0x8
  0009663a: mov r0,r5
  0009663c: mov r1,r4
  0009663e: blx 0x0006ee24
  00096642: add r0,sp,#0x8
  00096644: str.w r5,[r4,#0x414]
  00096648: blx 0x0006ee30
  0009664c: ldr r0,[r4,#0x30]
  0009664e: movs r3,#0x0
  00096650: ldr r1,[r0,#0x0]
  00096652: ldr.w r0,[r4,#0x414]
  00096656: ldr r1,[r1,#0x30]
  00096658: cmp r1,#0x2
  0009665a: ite ne
  0009665c: ldrd.ne r2,r1,[r4,#0x368]
  00096660: ldrd.eq r1,r2,[r4,#0x368]
  00096664: str r3,[sp,#0x0]
  00096666: movs r3,#0x1
  00096668: blx 0x0006ee3c
  0009666c: ldr r0,[0x00096738]
  0009666e: cmp r6,r0
  00096670: beq 0x00096692
  00096672: ldr r0,[0x0009673c]
  00096674: cmp r6,r0
  00096676: beq 0x000966a8
  00096678: cmp.w r6,#0x1400000
  0009667c: bne 0x000966c8
  0009667e: ldr.w r0,[r4,#0x410]
  00096682: cmp.w r8,#0x0
  00096686: ite eq
  00096688: bic.eq r0,r0,#0x1
  0009668c: orr.ne r0,r0,#0x1
  00096690: b 0x000966c4
  00096692: cmp.w r8,#0x0
  00096696: beq 0x000966de
  00096698: ldr r0,[0x0009674c]
  0009669a: add r0,pc
  0009669c: ldr r1,[r0,#0x0]
  0009669e: cmp r1,#0x0
  000966a0: ble 0x00096714
  000966a2: subs r1,#0x1
  000966a4: str r1,[r0,#0x0]
  000966a6: b 0x000966c8
  000966a8: ldr r0,[0x00096748]
  000966aa: cmp.w r8,#0x0
  000966ae: add r0,pc
  000966b0: ldr r0,[r0,#0x0]
  000966b2: strb.w r8,[r0,#0x0]
  000966b6: ldr.w r0,[r4,#0x410]
  000966ba: ite eq
  000966bc: bic.eq r0,r0,#0x2
  000966c0: orr.ne r0,r0,#0x2
  000966c4: str.w r0,[r4,#0x410]
  000966c8: ldr r0,[sp,#0x14]
  000966ca: ldr.w r1,[r9,#0x0]
  000966ce: subs r0,r1,r0
  000966d0: ittt eq
  000966d2: add.eq sp,#0x18
  000966d4: pop.eq.w {r8,r9,r11}
  000966d8: pop.eq {r4,r5,r6,r7,pc}
  000966da: blx 0x0006e824
  000966de: ldr r0,[0x00096750]
  000966e0: add r0,pc
  000966e2: ldrb r1,[r0,#0x0]
  000966e4: cmp r1,#0x1
  000966e6: bne 0x00096702
  000966e8: ldr.w r1,[r4,#0x410]
  000966ec: ldr r2,[0x00096754]
  000966ee: bic r1,r1,#0x4
  000966f2: str.w r1,[r4,#0x410]
  000966f6: movs r1,#0x0
  000966f8: add r2,pc
  000966fa: strb r1,[r0,#0x0]
  000966fc: movs r0,#0x1
  000966fe: str r0,[r2,#0x0]
  00096700: b 0x000966c8
  00096702: ldr r1,[0x00096758]
  00096704: add r1,pc
  00096706: ldr r1,[r1,#0x0]
  00096708: cmp r1,#0x0
  0009670a: ite gt
  0009670c: mov.gt r1,#0x0
  0009670e: mov.le r1,#0x1
  00096710: strb r1,[r0,#0x0]
  00096712: b 0x000966c8
  00096714: ldr.w r0,[r4,#0x410]
  00096718: orr r0,r0,#0x4
  0009671c: b 0x000966c4
```
