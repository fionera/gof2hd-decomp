# MenuTouchWindow::~MenuTouchWindow
elf 0x125170 body 534
Sig: undefined __thiscall ~MenuTouchWindow(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::~MenuTouchWindow() */

MenuTouchWindow * __thiscall MenuTouchWindow::~MenuTouchWindow(MenuTouchWindow *this)

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
  if (*(Array **)(this + 0xac) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xac));
    if (*(Array<TouchButton*> **)(this + 0xac) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xac));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xac) = 0;
  if (*(Array **)(this + 0xc0) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xc0));
    if (*(Array<TouchButton*> **)(this + 0xc0) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xc0));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xc0) = 0;
  if (*(Array **)(this + 0xb4) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xb4));
    if (*(Array<TouchButton*> **)(this + 0xb4) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xb4));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xb4) = 0;
  if (*(Array **)(this + 0xb8) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xb8));
    if (*(Array<TouchButton*> **)(this + 0xb8) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xb8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xb8) = 0;
  if (*(Array **)(this + 0xb0) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xb0));
    if (*(Array<TouchButton*> **)(this + 0xb0) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xb0));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xb0) = 0;
  if (*(Array **)(this + 0xec) != (Array *)0x0) {
    ArrayReleaseClasses<TouchSlider*>(*(Array **)(this + 0xec));
    if (*(Array<TouchSlider*> **)(this + 0xec) != (Array<TouchSlider*> *)0x0) {
      pvVar1 = (void *)Array<TouchSlider*>::~Array(*(Array<TouchSlider*> **)(this + 0xec));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xec) = 0;
  if (*(Array **)(this + 0xf8) != (Array *)0x0) {
    ArrayReleaseClasses<TouchButton*>(*(Array **)(this + 0xf8));
    if (*(Array<TouchButton*> **)(this + 0xf8) != (Array<TouchButton*> *)0x0) {
      pvVar1 = (void *)Array<TouchButton*>::~Array(*(Array<TouchButton*> **)(this + 0xf8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xf8) = 0;
  if (*(Array **)(this + 0x9c) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x9c));
    if (*(Array<AbyssEngine::String*> **)(this + 0x9c) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 0x9c));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x9c) = 0;
  if (*(Array **)(this + 0xa0) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0xa0));
    if (*(Array<AbyssEngine::String*> **)(this + 0xa0) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 0xa0));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xa0) = 0;
  if (*(TouchButton **)(this + 0xcc) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xcc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xcc) = 0;
  if (*(TouchButton **)(this + 0xc4) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xc4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xc4) = 0;
  if (*(TouchButton **)(this + 0xd0) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xd0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xd0) = 0;
  if (*(TouchButton **)(this + 0xd4) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xd4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xd4) = 0;
  if (*(TouchButton **)(this + 0xd8) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xd8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xd8) = 0;
  if (*(TouchButton **)(this + 0xdc) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xdc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xdc) = 0;
  if (*(ChoiceWindow **)(this + 0x104) != (ChoiceWindow *)0x0) {
    pvVar1 = (void *)ChoiceWindow::~ChoiceWindow(*(ChoiceWindow **)(this + 0x104));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x104) = 0;
  if (*(TouchButton **)(this + 200) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 200));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 200) = 0;
  if (*(TouchButton **)(this + 0xe8) != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0xe8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xe8) = 0;
  if (*(ScrollTouchWindow **)(this + 0xf0) != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0xf0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xf0) = 0;
  if (*(ScrollTouchWindow **)(this + 0xf4) != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0xf4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xf4) = 0;
  if (*(void **)(this + 0x134) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x134));
  }
  *(undefined4 *)(this + 0x134) = 0;
  if (*(void **)(this + 0x138) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x138));
  }
  *(undefined4 *)(this + 0x138) = 0;
  return this;
}

```

## target disasm
```
  00135170: push {r4,r5,r7,lr}
  00135172: add r7,sp,#0x8
  00135174: mov r4,r0
  00135176: ldr r0,[r0,#0x4]
  00135178: cbz r0,0x0013518a
  0013517a: blx 0x00075328
  0013517e: ldr r0,[r4,#0x4]
  00135180: cbz r0,0x0013518a
  00135182: blx 0x00075334
  00135186: blx 0x0006eb48
  0013518a: ldr.w r0,[r4,#0xac]
  0013518e: movs r5,#0x0
  00135190: str r5,[r4,#0x4]
  00135192: cbz r0,0x001351a6
  00135194: blx 0x00075328
  00135198: ldr.w r0,[r4,#0xac]
  0013519c: cbz r0,0x001351a6
  0013519e: blx 0x00075334
  001351a2: blx 0x0006eb48
  001351a6: ldr.w r0,[r4,#0xc0]
  001351aa: str.w r5,[r4,#0xac]
  001351ae: cbz r0,0x001351c2
  001351b0: blx 0x00075328
  001351b4: ldr.w r0,[r4,#0xc0]
  001351b8: cbz r0,0x001351c2
  001351ba: blx 0x00075334
  001351be: blx 0x0006eb48
  001351c2: ldr.w r0,[r4,#0xb4]
  001351c6: movs r5,#0x0
  001351c8: str.w r5,[r4,#0xc0]
  001351cc: cbz r0,0x001351e0
  001351ce: blx 0x00075328
  001351d2: ldr.w r0,[r4,#0xb4]
  001351d6: cbz r0,0x001351e0
  001351d8: blx 0x00075334
  001351dc: blx 0x0006eb48
  001351e0: ldr.w r0,[r4,#0xb8]
  001351e4: str.w r5,[r4,#0xb4]
  001351e8: cbz r0,0x001351fc
  001351ea: blx 0x00075328
  001351ee: ldr.w r0,[r4,#0xb8]
  001351f2: cbz r0,0x001351fc
  001351f4: blx 0x00075334
  001351f8: blx 0x0006eb48
  001351fc: ldr.w r0,[r4,#0xb0]
  00135200: movs r5,#0x0
  00135202: str.w r5,[r4,#0xb8]
  00135206: cbz r0,0x0013521a
  00135208: blx 0x00075328
  0013520c: ldr.w r0,[r4,#0xb0]
  00135210: cbz r0,0x0013521a
  00135212: blx 0x00075334
  00135216: blx 0x0006eb48
  0013521a: ldr.w r0,[r4,#0xec]
  0013521e: str.w r5,[r4,#0xb0]
  00135222: cbz r0,0x00135236
  00135224: blx 0x0007654c
  00135228: ldr.w r0,[r4,#0xec]
  0013522c: cbz r0,0x00135236
  0013522e: blx 0x00076558
  00135232: blx 0x0006eb48
  00135236: ldr.w r0,[r4,#0xf8]
  0013523a: movs r5,#0x0
  0013523c: str.w r5,[r4,#0xec]
  00135240: cbz r0,0x00135254
  00135242: blx 0x00075328
  00135246: ldr.w r0,[r4,#0xf8]
  0013524a: cbz r0,0x00135254
  0013524c: blx 0x00075334
  00135250: blx 0x0006eb48
  00135254: ldr.w r0,[r4,#0x9c]
  00135258: str.w r5,[r4,#0xf8]
  0013525c: cbz r0,0x00135270
  0013525e: blx 0x0006facc
  00135262: ldr.w r0,[r4,#0x9c]
  00135266: cbz r0,0x00135270
  00135268: blx 0x0006f64c
  0013526c: blx 0x0006eb48
  00135270: ldr.w r0,[r4,#0xa0]
  00135274: movs r5,#0x0
  00135276: str.w r5,[r4,#0x9c]
  0013527a: cbz r0,0x0013528e
  0013527c: blx 0x0006facc
  00135280: ldr.w r0,[r4,#0xa0]
  00135284: cbz r0,0x0013528e
  00135286: blx 0x0006f64c
  0013528a: blx 0x0006eb48
  0013528e: ldr.w r0,[r4,#0xcc]
  00135292: str.w r5,[r4,#0xa0]
  00135296: cbz r0,0x001352a0
  00135298: blx 0x00074d7c
  0013529c: blx 0x0006eb48
  001352a0: ldr.w r0,[r4,#0xc4]
  001352a4: movs r5,#0x0
  001352a6: str.w r5,[r4,#0xcc]
  001352aa: cbz r0,0x001352b4
  001352ac: blx 0x00074d7c
  001352b0: blx 0x0006eb48
  001352b4: ldr.w r0,[r4,#0xd0]
  001352b8: str.w r5,[r4,#0xc4]
  001352bc: cbz r0,0x001352c6
  001352be: blx 0x00074d7c
  001352c2: blx 0x0006eb48
  001352c6: ldr.w r0,[r4,#0xd4]
  001352ca: movs r5,#0x0
  001352cc: str.w r5,[r4,#0xd0]
  001352d0: cbz r0,0x001352da
  001352d2: blx 0x00074d7c
  001352d6: blx 0x0006eb48
  001352da: ldr.w r0,[r4,#0xd8]
  001352de: str.w r5,[r4,#0xd4]
  001352e2: cbz r0,0x001352ec
  001352e4: blx 0x00074d7c
  001352e8: blx 0x0006eb48
  001352ec: ldr.w r0,[r4,#0xdc]
  001352f0: movs r5,#0x0
  001352f2: str.w r5,[r4,#0xd8]
  001352f6: cbz r0,0x00135300
  001352f8: blx 0x00074d7c
  001352fc: blx 0x0006eb48
  00135300: ldr.w r0,[r4,#0x104]
  00135304: str.w r5,[r4,#0xdc]
  00135308: cbz r0,0x00135312
  0013530a: blx 0x00074d88
  0013530e: blx 0x0006eb48
  00135312: ldr.w r0,[r4,#0xc8]
  00135316: movs r5,#0x0
  00135318: str.w r5,[r4,#0x104]
  0013531c: cbz r0,0x00135326
  0013531e: blx 0x00074d7c
  00135322: blx 0x0006eb48
  00135326: ldr.w r0,[r4,#0xe8]
  0013532a: str.w r5,[r4,#0xc8]
  0013532e: cbz r0,0x00135338
  00135330: blx 0x00074d7c
  00135334: blx 0x0006eb48
  00135338: ldr.w r0,[r4,#0xf0]
  0013533c: movs r5,#0x0
  0013533e: str.w r5,[r4,#0xe8]
  00135342: cbz r0,0x0013534c
  00135344: blx 0x00075afc
  00135348: blx 0x0006eb48
  0013534c: ldr.w r0,[r4,#0xf4]
  00135350: str.w r5,[r4,#0xf0]
  00135354: cbz r0,0x0013535e
  00135356: blx 0x00075afc
  0013535a: blx 0x0006eb48
  0013535e: ldr.w r0,[r4,#0x134]
  00135362: movs r5,#0x0
  00135364: str.w r5,[r4,#0xf4]
  00135368: cbz r0,0x0013536e
  0013536a: blx 0x0006ebfc
  0013536e: ldr.w r0,[r4,#0x138]
  00135372: str.w r5,[r4,#0x134]
  00135376: cbz r0,0x0013537c
  00135378: blx 0x0006ebfc
  0013537c: movs r0,#0x0
  0013537e: str.w r0,[r4,#0x138]
  00135382: mov r0,r4
  00135384: pop {r4,r5,r7,pc}
```
