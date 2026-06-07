# WantedWindow::~WantedWindow
elf 0xe0ec4 body 168
Sig: undefined __thiscall ~WantedWindow(WantedWindow * this)

## decompile
```c

/* WantedWindow::~WantedWindow() */

WantedWindow * __thiscall WantedWindow::~WantedWindow(WantedWindow *this)

{
  int iVar1;
  void *pvVar2;
  code *pcVar3;
  
  if (*(Array **)(this + 8) != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>(*(Array **)(this + 8));
    if (*(Array<ImagePart*> **)(this + 8) != (Array<ImagePart*> *)0x0) {
      pvVar2 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(this + 8));
      operator_delete(pvVar2);
    }
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xc));
    if (*(Array<TouchButton*> **)(this + 0xc) != (Array<TouchButton*> *)0x0) {
      pvVar2 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xc));
      operator_delete(pvVar2);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(TouchButton **)(this + 0x18) != (TouchButton *)0x0) {
    pvVar2 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x18));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x18) = 0;
  if (*(Array<Wanted*> **)(this + 0x38) != (Array<Wanted*> *)0x0) {
    pvVar2 = (void *)Array<Wanted*>::~Array(*(Array<Wanted*> **)(this + 0x38));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 0x38) = 0;
  if (*(int **)(this + 0xb0) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xb0) + 4))();
  }
  *(undefined4 *)(this + 0xb0) = 0;
  if (*(ScrollTouchWindow **)(this + 0x2c) != (ScrollTouchWindow *)0x0) {
    pvVar2 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0x2c));
    operator_delete(pvVar2);
  }
  iVar1 = DAT_000f0fa0;
  *(undefined4 *)(this + 0x2c) = 0;
  pcVar3 = *(code **)(iVar1 + 0xf0f40);
  (*pcVar3)(this + 0x78);
  (*pcVar3)(this + 0x6c);
  (*pcVar3)(this + 0x60);
  (*pcVar3)(this + 0x54);
  (*pcVar3)(this + 0x48);
  (*pcVar3)(this + 0x3c);
  return this;
}

```

## target disasm
```
  000f0ec4: push {r4,r5,r6,r7,lr}
  000f0ec6: add r7,sp,#0xc
  000f0ec8: push.w r11
  000f0ecc: mov r4,r0
  000f0ece: ldr r0,[r0,#0x8]
  000f0ed0: cbz r0,0x000f0ee2
  000f0ed2: blx 0x00075ae4
  000f0ed6: ldr r0,[r4,#0x8]
  000f0ed8: cbz r0,0x000f0ee2
  000f0eda: blx 0x00075af0
  000f0ede: blx 0x0006eb48
  000f0ee2: ldr r0,[r4,#0xc]
  000f0ee4: movs r5,#0x0
  000f0ee6: str r5,[r4,#0x8]
  000f0ee8: cbz r0,0x000f0efa
  000f0eea: blx 0x00075328
  000f0eee: ldr r0,[r4,#0xc]
  000f0ef0: cbz r0,0x000f0efa
  000f0ef2: blx 0x00075334
  000f0ef6: blx 0x0006eb48
  000f0efa: ldr r0,[r4,#0x18]
  000f0efc: str r5,[r4,#0xc]
  000f0efe: cbz r0,0x000f0f08
  000f0f00: blx 0x00074d7c
  000f0f04: blx 0x0006eb48
  000f0f08: ldr r0,[r4,#0x38]
  000f0f0a: movs r5,#0x0
  000f0f0c: str r5,[r4,#0x18]
  000f0f0e: cbz r0,0x000f0f18
  000f0f10: blx 0x00073600
  000f0f14: blx 0x0006eb48
  000f0f18: ldr.w r0,[r4,#0xb0]
  000f0f1c: str r5,[r4,#0x38]
  000f0f1e: cbz r0,0x000f0f26
  000f0f20: ldr r1,[r0,#0x0]
  000f0f22: ldr r1,[r1,#0x4]
  000f0f24: blx r1
  000f0f26: ldr r0,[r4,#0x2c]
  000f0f28: movs r5,#0x0
  000f0f2a: str.w r5,[r4,#0xb0]
  000f0f2e: cbz r0,0x000f0f38
  000f0f30: blx 0x00075afc
  000f0f34: blx 0x0006eb48
  000f0f38: ldr r0,[0x000f0fa0]
  000f0f3a: str r5,[r4,#0x2c]
  000f0f3c: add r0,pc
  000f0f3e: ldr r5,[r0,#0x0]
  000f0f40: add.w r0,r4,#0x78
  000f0f44: blx r5
  000f0f46: add.w r0,r4,#0x6c
  000f0f4a: blx r5
  000f0f4c: add.w r0,r4,#0x60
  000f0f50: blx r5
  000f0f52: add.w r0,r4,#0x54
  000f0f56: blx r5
  000f0f58: add.w r0,r4,#0x48
  000f0f5c: blx r5
  000f0f5e: add.w r0,r4,#0x3c
  000f0f62: blx r5
  000f0f64: mov r0,r4
  000f0f66: pop.w r11
  000f0f6a: pop {r4,r5,r6,r7,pc}
```
