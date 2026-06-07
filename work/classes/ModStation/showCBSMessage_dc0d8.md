# ModStation::showCBSMessage
elf 0xdc0d8 body 168
Sig: undefined __thiscall showCBSMessage(ModStation * this)

## decompile
```c

/* ModStation::showCBSMessage() */

void __thiscall ModStation::showCBSMessage(ModStation *this)

{
  String *pSVar1;
  code *pcVar2;
  int *piVar3;
  ChoiceWindow *this_00;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar3 = *(int **)(DAT_000ec1a4 + 0xec0f0);
  local_28 = *piVar3;
  AbyssEngine::String::String(aSStack_34,(char *)(DAT_000ec1a8 + 0xec100),false);
  AbyssEngine::String::String(aSStack_40,(char *)(DAT_000ec1ac + 0xec108),false);
  this_00 = (ChoiceWindow *)this->field_70;
  pSVar1 = (String *)GameText::getText(**(int **)(DAT_000ec1b0 + 0xec11c));
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000ec1b4 + 0xec12e),false);
  ChoiceWindow::set(this_00,pSVar1,aSStack_4c,true,aSStack_34,aSStack_40,aSStack_34,-1,-1);
  pcVar2 = *(code **)(DAT_000ec1b8 + 0xec156);
  (*pcVar2)(aSStack_4c);
  *(undefined1 *)((int)&this[1].field_4C + 1) = 1;
  *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 3) = 1;
  (*pcVar2)(aSStack_40);
  (*pcVar2)(aSStack_34);
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ec0d8: push {r4,r5,r6,r7,lr}
  000ec0da: add r7,sp,#0xc
  000ec0dc: push {r8,r9,r10,r11}
  000ec0e0: sub sp,#0x3c
  000ec0e2: mov r4,r0
  000ec0e4: ldr r0,[0x000ec1a4]
  000ec0e6: add.w r9,sp,#0x2c
  000ec0ea: movs r2,#0x0
  000ec0ec: add r0,pc
  000ec0ee: ldr.w r10,[r0,#0x0]
  000ec0f2: ldr.w r0,[r10,#0x0]
  000ec0f6: ldr r1,[0x000ec1a8]
  000ec0f8: str r0,[sp,#0x38]
  000ec0fa: mov r0,r9
  000ec0fc: add r1,pc
  000ec0fe: blx 0x0006ee18
  000ec102: ldr r1,[0x000ec1ac]
  000ec104: add r1,pc
  000ec106: add.w r8,sp,#0x20
  000ec10a: movs r2,#0x0
  000ec10c: mov r0,r8
  000ec10e: blx 0x0006ee18
  000ec112: ldr r0,[0x000ec1b0]
  000ec114: ldr.w r11,[r4,#0x70]
  000ec118: add r0,pc
  000ec11a: ldr r0,[r0,#0x0]
  000ec11c: ldr r0,[r0,#0x0]
  000ec11e: mov.w r1,#0x186
  000ec122: blx 0x00072f70
  000ec126: ldr r1,[0x000ec1b4]
  000ec128: mov r5,r0
  000ec12a: add r1,pc
  000ec12c: add r0,sp,#0x14
  000ec12e: movs r2,#0x0
  000ec130: blx 0x0006ee18
  000ec134: mov.w r0,#0xffffffff
  000ec138: add r2,sp,#0x14
  000ec13a: strd r9,r8,[sp,#0x0]
  000ec13e: mov r1,r5
  000ec140: strd r9,r0,[sp,#0x8]
  000ec144: movs r3,#0x1
  000ec146: str r0,[sp,#0x10]
  000ec148: mov r0,r11
  000ec14a: movs r6,#0x1
  000ec14c: blx 0x00075574
  000ec150: ldr r0,[0x000ec1b8]
  000ec152: add r0,pc
  000ec154: ldr r5,[r0,#0x0]
  000ec156: add r0,sp,#0x14
  000ec158: blx r5
  000ec15a: add r0,sp,#0x20
  000ec15c: strb.w r6,[r4,#0xdd]
  000ec160: strb.w r6,[r4,#0x63]
  000ec164: blx r5
  000ec166: add r0,sp,#0x2c
  000ec168: blx r5
  000ec16a: ldr r0,[sp,#0x38]
  000ec16c: ldr.w r1,[r10,#0x0]
  000ec170: subs r0,r1,r0
  000ec172: ittt eq
  000ec174: add.eq sp,#0x3c
  000ec176: pop.eq.w {r8,r9,r10,r11}
  000ec17a: pop.eq {r4,r5,r6,r7,pc}
  000ec17c: blx 0x0006e824
```
