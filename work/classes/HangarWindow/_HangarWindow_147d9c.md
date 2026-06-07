# HangarWindow::~HangarWindow
elf 0x147d9c body 132
Sig: undefined __thiscall ~HangarWindow(HangarWindow * this)

## decompile
```c

/* HangarWindow::~HangarWindow() */

HangarWindow * __thiscall HangarWindow::~HangarWindow(HangarWindow *this)

{
  void *pvVar1;
  
  if (*(HangarList **)(this + 0x14) != (HangarList *)0x0) {
    pvVar1 = (void *)HangarList::~HangarList(*(HangarList **)(this + 0x14));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14) = 0;
  if (*(ListItemWindow **)(this + 0x18) != (ListItemWindow *)0x0) {
    pvVar1 = (void *)ListItemWindow::~ListItemWindow(*(ListItemWindow **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(ChoiceWindow **)(this + 0x1c) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 0x1c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1c) = 0;
  if (*(ChoiceWindow **)(this + 0x20) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 0x20));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x20) = 0;
  if (*(Array **)(this + 4) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 4));
    if (*(Array<TouchButton*> **)(this + 4) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(Array **)(this + 0x24) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0x24));
    if (*(Array<TouchButton*> **)(this + 0x24) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0x24));
      operator_delete(pvVar1);
    }
  }
  this[0xc] = (HangarWindow)0x0;
  *(undefined4 *)(this + 0x24) = 0;
  if (*(void **)(this + 0x30) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x30));
  }
  *(undefined4 *)(this + 0x30) = 0;
  return this;
}

```

## target disasm
```
  00157d9c: push {r4,r5,r7,lr}
  00157d9e: add r7,sp,#0x8
  00157da0: mov r4,r0
  00157da2: ldr r0,[r0,#0x14]
  00157da4: cbz r0,0x00157dae
  00157da6: blx 0x00076eb8
  00157daa: blx 0x0006eb48
  00157dae: ldr r0,[r4,#0x18]
  00157db0: movs r5,#0x0
  00157db2: str r5,[r4,#0x14]
  00157db4: cbz r0,0x00157dbe
  00157db6: blx 0x00076ec4
  00157dba: blx 0x0006eb48
  00157dbe: ldr r0,[r4,#0x1c]
  00157dc0: str r5,[r4,#0x18]
  00157dc2: cbz r0,0x00157dcc
  00157dc4: blx 0x00074d88
  00157dc8: blx 0x0006eb48
  00157dcc: ldr r0,[r4,#0x20]
  00157dce: movs r5,#0x0
  00157dd0: str r5,[r4,#0x1c]
  00157dd2: cbz r0,0x00157ddc
  00157dd4: blx 0x00074d88
  00157dd8: blx 0x0006eb48
  00157ddc: ldr r0,[r4,#0x4]
  00157dde: str r5,[r4,#0x20]
  00157de0: cbz r0,0x00157df2
  00157de2: blx 0x00075328
  00157de6: ldr r0,[r4,#0x4]
  00157de8: cbz r0,0x00157df2
  00157dea: blx 0x00075334
  00157dee: blx 0x0006eb48
  00157df2: ldr r0,[r4,#0x24]
  00157df4: movs r5,#0x0
  00157df6: str r5,[r4,#0x4]
  00157df8: cbz r0,0x00157e0a
  00157dfa: blx 0x00075328
  00157dfe: ldr r0,[r4,#0x24]
  00157e00: cbz r0,0x00157e0a
  00157e02: blx 0x00075334
  00157e06: blx 0x0006eb48
  00157e0a: ldr r0,[r4,#0x30]
  00157e0c: strb r5,[r4,#0xc]
  00157e0e: cmp r0,#0x0
  00157e10: str r5,[r4,#0x24]
  00157e12: beq 0x00157e18
  00157e14: blx 0x0006ebfc
  00157e18: movs r0,#0x0
  00157e1a: str r0,[r4,#0x30]
  00157e1c: mov r0,r4
  00157e1e: pop {r4,r5,r7,pc}
```
