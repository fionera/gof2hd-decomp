# MGame::useCloak
elf 0x179bfc body 254
Sig: undefined __thiscall useCloak(MGame * this)

## decompile
```c

/* MGame::useCloak() */

void __thiscall MGame::useCloak(MGame *this)

{
  int iVar1;
  ChoiceWindow *this_00;
  String *pSVar2;
  code *pcVar3;
  String *pSVar4;
  int *piVar5;
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar5 = *(int **)(DAT_00189d44 + 0x189c0e);
  local_24 = *piVar5;
  iVar1 = PlayerEgo::toggleCloaking();
  if (iVar1 == 0) {
    if (this->field_94 == 0) {
      this_00 = operator_new(0x5c);
      ChoiceWindow::ChoiceWindow(this_00);
      this->field_94 = (int)this_00;
    }
    iVar1 = Status::getShip();
    iVar1 = Ship::getFirstEquipmentOfSort(iVar1);
    if (iVar1 == 0) {
      iVar1 = 0;
    }
    else {
      iVar1 = Item::getAttribute(iVar1);
    }
    pSVar4 = (String *)this->field_94;
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_00189d4c + 0x189c66));
    AbyssEngine::String::String(aSStack_54,(char *)(DAT_00189d50 + 0x189c78),false);
    AbyssEngine::operator+(aSStack_48,pSVar2);
    AbyssEngine::String::String(aSStack_60,iVar1);
    AbyssEngine::operator+(aSStack_3c,aSStack_48);
    AbyssEngine::String::String(aSStack_6c,(char *)(DAT_00189d54 + 0x189c9e),false);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    ChoiceWindow::set(pSVar4);
    pcVar3 = *(code **)(DAT_00189d58 + 0x189cbc);
    (*pcVar3)(aSStack_30);
    (*pcVar3)(aSStack_6c);
    (*pcVar3)(aSStack_3c);
    (*pcVar3)(aSStack_60);
    (*pcVar3)(aSStack_48);
    (*pcVar3)(aSStack_54);
    *(undefined1 *)((int)&this->field_5C + 1) = 1;
    *(undefined1 *)((int)&this[1].field_4 + 2) = 1;
    pauseSounds();
  }
  if (*piVar5 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00189bfc: push {r4,r5,r6,r7,lr}
  00189bfe: add r7,sp,#0xc
  00189c00: push {r8,r9,r10}
  00189c04: sub sp,#0x50
  00189c06: mov r4,r0
  00189c08: ldr r0,[0x00189d44]
  00189c0a: add r0,pc
  00189c0c: ldr.w r10,[r0,#0x0]
  00189c10: ldr.w r0,[r10,#0x0]
  00189c14: str r0,[sp,#0x4c]
  00189c16: ldr r0,[r4,#0x58]
  00189c18: blx 0x00072784
  00189c1c: cmp r0,#0x0
  00189c1e: bne 0x00189ce4
  00189c20: ldr.w r0,[r4,#0x94]
  00189c24: cbnz r0,0x00189c36
  00189c26: movs r0,#0x5c
  00189c28: blx 0x0006eb24
  00189c2c: mov r5,r0
  00189c2e: blx 0x00074584
  00189c32: str.w r5,[r4,#0x94]
  00189c36: ldr r0,[0x00189d48]
  00189c38: add r0,pc
  00189c3a: ldr r0,[r0,#0x0]
  00189c3c: ldr r0,[r0,#0x0]
  00189c3e: blx 0x00071a58
  00189c42: movs r1,#0x15
  00189c44: blx 0x0007228c
  00189c48: cbz r0,0x00189c54
  00189c4a: movs r1,#0x26
  00189c4c: blx 0x000718e4
  00189c50: mov r9,r0
  00189c52: b 0x00189c58
  00189c54: mov.w r9,#0x0
  00189c58: ldr r0,[0x00189d4c]
  00189c5a: movw r1,#0x247
  00189c5e: ldr.w r8,[r4,#0x94]
  00189c62: add r0,pc
  00189c64: ldr r0,[r0,#0x0]
  00189c66: ldr r0,[r0,#0x0]
  00189c68: blx 0x00072f70
  00189c6c: ldr r1,[0x00189d50]
  00189c6e: add r6,sp,#0x1c
  00189c70: mov r5,r0
  00189c72: movs r2,#0x0
  00189c74: add r1,pc
  00189c76: mov r0,r6
  00189c78: blx 0x0006ee18
  00189c7c: add r0,sp,#0x28
  00189c7e: mov r1,r5
  00189c80: mov r2,r6
  00189c82: blx 0x0006ef98
  00189c86: add r0,sp,#0x10
  00189c88: mov r1,r9
  00189c8a: blx 0x0006f658
  00189c8e: add r0,sp,#0x34
  00189c90: add r1,sp,#0x28
  00189c92: add r2,sp,#0x10
  00189c94: blx 0x0006ef98
  00189c98: ldr r1,[0x00189d54]
  00189c9a: add r1,pc
  00189c9c: add r0,sp,#0x4
  00189c9e: movs r2,#0x0
  00189ca0: blx 0x0006ee18
  00189ca4: add r0,sp,#0x40
  00189ca6: add r1,sp,#0x34
  00189ca8: add r2,sp,#0x4
  00189caa: blx 0x0006ef98
  00189cae: add r1,sp,#0x40
  00189cb0: mov r0,r8
  00189cb2: blx 0x00074794
  00189cb6: ldr r0,[0x00189d58]
  00189cb8: add r0,pc
  00189cba: ldr r5,[r0,#0x0]
  00189cbc: add r0,sp,#0x40
  00189cbe: blx r5
  00189cc0: add r0,sp,#0x4
  00189cc2: blx r5
  00189cc4: add r0,sp,#0x34
  00189cc6: blx r5
  00189cc8: add r0,sp,#0x10
  00189cca: blx r5
  00189ccc: add r0,sp,#0x28
  00189cce: blx r5
  00189cd0: add r0,sp,#0x1c
  00189cd2: blx r5
  00189cd4: movs r0,#0x1
  00189cd6: strb.w r0,[r4,#0x5d]
  00189cda: strb.w r0,[r4,#0xce]
  00189cde: mov r0,r4
  00189ce0: blx 0x0007822c
  00189ce4: ldr r0,[sp,#0x4c]
  00189ce6: ldr.w r1,[r10,#0x0]
  00189cea: subs r0,r1,r0
  00189cec: ittt eq
  00189cee: add.eq sp,#0x50
  00189cf0: pop.eq.w {r8,r9,r10}
  00189cf4: pop.eq {r4,r5,r6,r7,pc}
  00189cf6: blx 0x0006e824
```
