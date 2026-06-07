# StatusWindow::~StatusWindow
elf 0x158164 body 102
Sig: undefined __thiscall ~StatusWindow(StatusWindow * this)

## decompile
```c

/* StatusWindow::~StatusWindow() */

StatusWindow * __thiscall StatusWindow::~StatusWindow(StatusWindow *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 4) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 4));
    if (*(Array<TouchButton*> **)(this + 4) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(Array **)(this + 8) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 8));
    if (*(Array<TouchButton*> **)(this + 8) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>(*(Array **)(this + 0xc));
    if (*(Array<ImagePart*> **)(this + 0xc) != (Array<ImagePart*> *)0x0) {
      pvVar1 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(this + 0xc));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(Array **)(this + 0x10) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x10));
    if (*(Array<AbyssEngine::String*> **)(this + 0x10) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 0x10));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x10) = 0;
  return this;
}

```

## target disasm
```
  00168164: push {r4,r5,r7,lr}
  00168166: add r7,sp,#0x8
  00168168: mov r4,r0
  0016816a: ldr r0,[r0,#0x4]
  0016816c: cbz r0,0x0016817e
  0016816e: blx 0x00075328
  00168172: ldr r0,[r4,#0x4]
  00168174: cbz r0,0x0016817e
  00168176: blx 0x00075334
  0016817a: blx 0x0006eb48
  0016817e: ldr r0,[r4,#0x8]
  00168180: movs r5,#0x0
  00168182: str r5,[r4,#0x4]
  00168184: cbz r0,0x00168196
  00168186: blx 0x00075328
  0016818a: ldr r0,[r4,#0x8]
  0016818c: cbz r0,0x00168196
  0016818e: blx 0x00075334
  00168192: blx 0x0006eb48
  00168196: ldr r0,[r4,#0xc]
  00168198: str r5,[r4,#0x8]
  0016819a: cbz r0,0x001681ac
  0016819c: blx 0x00075ae4
  001681a0: ldr r0,[r4,#0xc]
  001681a2: cbz r0,0x001681ac
  001681a4: blx 0x00075af0
  001681a8: blx 0x0006eb48
  001681ac: ldr r0,[r4,#0x10]
  001681ae: movs r5,#0x0
  001681b0: str r5,[r4,#0xc]
  001681b2: cbz r0,0x001681c4
  001681b4: blx 0x0006facc
  001681b8: ldr r0,[r4,#0x10]
  001681ba: cbz r0,0x001681c4
  001681bc: blx 0x0006f64c
  001681c0: blx 0x0006eb48
  001681c4: mov r0,r4
  001681c6: str r5,[r4,#0x10]
  001681c8: pop {r4,r5,r7,pc}
```
