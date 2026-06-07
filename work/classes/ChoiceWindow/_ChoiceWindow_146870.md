# ChoiceWindow::~ChoiceWindow
elf 0x146870 body 86
Sig: undefined __thiscall ~ChoiceWindow(ChoiceWindow * this)

## decompile
```c

/* ChoiceWindow::~ChoiceWindow() */

ChoiceWindow * __thiscall ChoiceWindow::~ChoiceWindow(ChoiceWindow *this)

{
  void *pvVar1;
  
  if (*(TouchButton **)(this + 0x10) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x10));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x10) = 0;
  if (*(TouchButton **)(this + 0x14) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x14));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14) = 0;
  if (*(TouchButton **)(this + 0x18) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(ScrollTouchWindow **)(this + 0x1c) != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0x1c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1c) = 0;
  AbyssEngine::String::~String((String *)(this + 0x3c));
  AbyssEngine::String::~String((String *)(this + 0x20));
  return this;
}

```

## target disasm
```
  00156870: push {r4,r5,r7,lr}
  00156872: add r7,sp,#0x8
  00156874: mov r4,r0
  00156876: ldr r0,[r0,#0x10]
  00156878: cbz r0,0x00156882
  0015687a: blx 0x00074d7c
  0015687e: blx 0x0006eb48
  00156882: ldr r0,[r4,#0x14]
  00156884: movs r5,#0x0
  00156886: str r5,[r4,#0x10]
  00156888: cbz r0,0x00156892
  0015688a: blx 0x00074d7c
  0015688e: blx 0x0006eb48
  00156892: ldr r0,[r4,#0x18]
  00156894: str r5,[r4,#0x14]
  00156896: cbz r0,0x001568a0
  00156898: blx 0x00074d7c
  0015689c: blx 0x0006eb48
  001568a0: ldr r0,[r4,#0x1c]
  001568a2: movs r5,#0x0
  001568a4: str r5,[r4,#0x18]
  001568a6: cbz r0,0x001568b0
  001568a8: blx 0x00075afc
  001568ac: blx 0x0006eb48
  001568b0: add.w r0,r4,#0x3c
  001568b4: str r5,[r4,#0x1c]
  001568b6: blx 0x0006ee30
  001568ba: add.w r0,r4,#0x20
  001568be: blx 0x0006ee30
  001568c2: mov r0,r4
  001568c4: pop {r4,r5,r7,pc}
```
