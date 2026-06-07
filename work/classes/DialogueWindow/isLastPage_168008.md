# DialogueWindow::isLastPage
elf 0x168008 body 22
Sig: undefined __thiscall isLastPage(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::isLastPage() */

bool __thiscall DialogueWindow::isLastPage(DialogueWindow *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = this->field_48;
  iVar1 = length(this);
  return piVar2 == (int *)(iVar1 + -1);
}

```

## target disasm
```
  00178008: push {r4,r6,r7,lr}
  0017800a: add r7,sp,#0x8
  0017800c: ldr r4,[r0,#0x48]
  0017800e: blx 0x00077c38
  00178012: subs r0,#0x1
  00178014: subs r0,r4,r0
  00178016: clz r0,r0
  0017801a: lsrs r0,r0,#0x5
  0017801c: pop {r4,r6,r7,pc}
```
