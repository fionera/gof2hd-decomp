# MissionsWindow::~MissionsWindow
elf 0x15040c body 162
Sig: undefined __thiscall ~MissionsWindow(MissionsWindow * this)

## decompile
```c

/* MissionsWindow::~MissionsWindow() */

MissionsWindow * __thiscall MissionsWindow::~MissionsWindow(MissionsWindow *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0x18) != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>(*(Array **)(this + 0x18));
    if (*(Array<ImagePart*> **)(this + 0x18) != (Array<ImagePart*> *)0x0) {
      pvVar1 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(this + 0x18));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(Array **)(this + 0x14) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0x14));
    if (*(Array<TouchButton*> **)(this + 0x14) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0x14));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x14) = 0;
  if (*(ScrollTouchWindow **)this != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  if (*(ScrollTouchWindow **)(this + 4) != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(ChoiceWindow **)(this + 0xc) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 0xc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(TouchButton **)(this + 0x24) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x24));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x24) = 0;
  if (*(TouchButton **)(this + 0x28) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x28));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x28) = 0;
  if (*(TouchButton **)(this + 0x2c) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x2c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x2c) = 0;
  if (*(WantedWindow **)(this + 0x10) != (WantedWindow *)0x0) {
    pvVar1 = (void *)WantedWindow::~WantedWindow(*(WantedWindow **)(this + 0x10));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x10) = 0;
  return this;
}

```

## target disasm
```
  0016040c: push {r4,r5,r7,lr}
  0016040e: add r7,sp,#0x8
  00160410: mov r4,r0
  00160412: ldr r0,[r0,#0x18]
  00160414: cbz r0,0x00160426
  00160416: blx 0x00075ae4
  0016041a: ldr r0,[r4,#0x18]
  0016041c: cbz r0,0x00160426
  0016041e: blx 0x00075af0
  00160422: blx 0x0006eb48
  00160426: ldr r0,[r4,#0x14]
  00160428: movs r5,#0x0
  0016042a: str r5,[r4,#0x18]
  0016042c: cbz r0,0x0016043e
  0016042e: blx 0x00075328
  00160432: ldr r0,[r4,#0x14]
  00160434: cbz r0,0x0016043e
  00160436: blx 0x00075334
  0016043a: blx 0x0006eb48
  0016043e: ldr r0,[r4,#0x0]
  00160440: str r5,[r4,#0x14]
  00160442: cbz r0,0x0016044c
  00160444: blx 0x00075afc
  00160448: blx 0x0006eb48
  0016044c: ldr r0,[r4,#0x4]
  0016044e: movs r5,#0x0
  00160450: str r5,[r4,#0x0]
  00160452: cbz r0,0x0016045c
  00160454: blx 0x00075afc
  00160458: blx 0x0006eb48
  0016045c: ldr r0,[r4,#0xc]
  0016045e: str r5,[r4,#0x4]
  00160460: cbz r0,0x0016046a
  00160462: blx 0x00074d88
  00160466: blx 0x0006eb48
  0016046a: ldr r0,[r4,#0x24]
  0016046c: movs r5,#0x0
  0016046e: str r5,[r4,#0xc]
  00160470: cbz r0,0x0016047a
  00160472: blx 0x00074d7c
  00160476: blx 0x0006eb48
  0016047a: ldr r0,[r4,#0x28]
  0016047c: str r5,[r4,#0x24]
  0016047e: cbz r0,0x00160488
  00160480: blx 0x00074d7c
  00160484: blx 0x0006eb48
  00160488: ldr r0,[r4,#0x2c]
  0016048a: movs r5,#0x0
  0016048c: str r5,[r4,#0x28]
  0016048e: cbz r0,0x00160498
  00160490: blx 0x00074d7c
  00160494: blx 0x0006eb48
  00160498: ldr r0,[r4,#0x10]
  0016049a: str r5,[r4,#0x2c]
  0016049c: cbz r0,0x001604a6
  0016049e: blx 0x000772a8
  001604a2: blx 0x0006eb48
  001604a6: movs r0,#0x0
  001604a8: str r0,[r4,#0x10]
  001604aa: mov r0,r4
  001604ac: pop {r4,r5,r7,pc}
```
