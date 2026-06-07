# DialogueWindow::nextPage
elf 0x16801e body 38
Sig: undefined __thiscall nextPage(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::nextPage() */

undefined4 __thiscall DialogueWindow::nextPage(DialogueWindow *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = this->field_48;
  iVar1 = length(this);
  if ((int)piVar2 < iVar1 + -1) {
    this->field_48 = (int *)((int)this->field_48 + 1);
    loadContent(this);
    return 1;
  }
  return 0;
}

```

## target disasm
```
  0017801e: push {r4,r5,r7,lr}
  00178020: add r7,sp,#0x8
  00178022: mov r4,r0
  00178024: ldr r5,[r0,#0x48]
  00178026: blx 0x00077c38
  0017802a: subs r0,#0x1
  0017802c: cmp r5,r0
  0017802e: bge 0x00178040
  00178030: ldr r0,[r4,#0x48]
  00178032: adds r0,#0x1
  00178034: str r0,[r4,#0x48]
  00178036: mov r0,r4
  00178038: blx 0x00077bfc
  0017803c: movs r0,#0x1
  0017803e: pop {r4,r5,r7,pc}
  00178040: movs r0,#0x0
  00178042: pop {r4,r5,r7,pc}
```
