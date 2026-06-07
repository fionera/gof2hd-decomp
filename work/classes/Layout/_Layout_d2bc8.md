# Layout::~Layout
elf 0xd2bc8 body 86
Sig: undefined __thiscall ~Layout(Layout * this)

## decompile
```c

/* Layout::~Layout() */

Layout * __thiscall Layout::~Layout(Layout *this)

{
  void *pvVar1;
  
  if (*(TouchButton **)(this + 0x3b4) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x3b4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x3b4) = 0;
  if (*(TouchButton **)(this + 0x3b8) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x3b8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x3b8) = 0;
  if (*(TouchButton **)(this + 0x3bc) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x3bc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x3bc) = 0;
  if (*(ChoiceWindow **)(this + 0x3c4) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 0x3c4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x3c4) = 0;
  return this;
}

```

## target disasm
```
  000e2bc8: push {r4,r5,r7,lr}
  000e2bca: add r7,sp,#0x8
  000e2bcc: mov r4,r0
  000e2bce: ldr.w r0,[r0,#0x3b4]
  000e2bd2: cbz r0,0x000e2bdc
  000e2bd4: blx 0x00074d7c
  000e2bd8: blx 0x0006eb48
  000e2bdc: ldr.w r0,[r4,#0x3b8]
  000e2be0: movs r5,#0x0
  000e2be2: str.w r5,[r4,#0x3b4]
  000e2be6: cbz r0,0x000e2bf0
  000e2be8: blx 0x00074d7c
  000e2bec: blx 0x0006eb48
  000e2bf0: ldr.w r0,[r4,#0x3bc]
  000e2bf4: str.w r5,[r4,#0x3b8]
  000e2bf8: cbz r0,0x000e2c02
  000e2bfa: blx 0x00074d7c
  000e2bfe: blx 0x0006eb48
  000e2c02: ldr.w r0,[r4,#0x3c4]
  000e2c06: movs r5,#0x0
  000e2c08: str.w r5,[r4,#0x3bc]
  000e2c0c: cbz r0,0x000e2c16
  000e2c0e: blx 0x00074d88
  000e2c12: blx 0x0006eb48
  000e2c16: mov r0,r4
  000e2c18: str.w r5,[r4,#0x3c4]
  000e2c1c: pop {r4,r5,r7,pc}
```
