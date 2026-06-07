# MenuTouchWindow::showSupernovaMessage
elf 0x125be0 body 70
Sig: undefined __stdcall showSupernovaMessage(void)

## decompile
```c

/* MenuTouchWindow::showSupernovaMessage() */

void MenuTouchWindow::showSupernovaMessage(void)

{
  int in_r0;
  String *pSVar1;
  String *pSVar2;
  int *piVar3;
  ChoiceWindow *this;
  
  this = *(ChoiceWindow **)(in_r0 + 0x104);
  piVar3 = *(int **)(DAT_00135c28 + 0x135bf8);
  pSVar1 = (String *)GameText::getText(*piVar3);
  pSVar2 = (String *)GameText::getText(*piVar3);
  ChoiceWindow::set(this,pSVar1,pSVar2,false);
  *(undefined1 *)(in_r0 + 0x180) = 1;
  *(undefined1 *)(in_r0 + 0x170) = 1;
  return;
}

```

## target disasm
```
  00135be0: push {r4,r5,r6,r7,lr}
  00135be2: add r7,sp,#0xc
  00135be4: push.w r8
  00135be8: mov r4,r0
  00135bea: ldr r0,[0x00135c28]
  00135bec: movw r1,#0x266
  00135bf0: ldr.w r8,[r4,#0x104]
  00135bf4: add r0,pc
  00135bf6: ldr r5,[r0,#0x0]
  00135bf8: ldr r0,[r5,#0x0]
  00135bfa: blx 0x00072f70
  00135bfe: mov r6,r0
  00135c00: ldr r0,[r5,#0x0]
  00135c02: movw r1,#0x267
  00135c06: blx 0x00072f70
  00135c0a: mov r2,r0
  00135c0c: mov r0,r8
  00135c0e: mov r1,r6
  00135c10: movs r3,#0x0
  00135c12: blx 0x00074ea8
  00135c16: movs r0,#0x1
  00135c18: strb.w r0,[r4,#0x180]
  00135c1c: strb.w r0,[r4,#0x170]
  00135c20: pop.w r8
  00135c24: pop {r4,r5,r6,r7,pc}
```
