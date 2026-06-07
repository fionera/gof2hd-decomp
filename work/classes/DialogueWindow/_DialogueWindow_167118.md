# DialogueWindow::~DialogueWindow
elf 0x167118 body 132
Sig: undefined __thiscall ~DialogueWindow(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::~DialogueWindow() */

DialogueWindow * __thiscall DialogueWindow::~DialogueWindow(DialogueWindow *this)

{
  void *pvVar1;
  
  if ((Array *)this->field_C != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>((Array *)this->field_C);
    if ((Array<ImagePart*> *)this->field_C != (Array<ImagePart*> *)0x0) {
      pvVar1 = (void *)Array<ImagePart*>::~Array((Array<ImagePart*> *)this->field_C);
      operator_delete(pvVar1);
    }
  }
  this->field_C = (int *)0x0;
  if ((void *)this->field_5C != (void *)0x0) {
    operator_delete__((void *)this->field_5C);
  }
  this->field_5C = 0;
  if ((void *)this->field_60 != (void *)0x0) {
    operator_delete__((void *)this->field_60);
  }
  this->field_60 = 0;
  if ((ScrollTouchWindow *)this->field_40 != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow((ScrollTouchWindow *)this->field_40);
    operator_delete(pvVar1);
  }
  this->field_40 = 0;
  if ((TouchButton *)this->field_0 != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton((TouchButton *)this->field_0);
    operator_delete(pvVar1);
  }
  this->field_0 = (int *)0x0;
  if ((TouchButton *)this->field_4 != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton((TouchButton *)this->field_4);
    operator_delete(pvVar1);
  }
  this->field_4 = (int *)0x0;
  if ((TouchButton *)this->field_8 != (TouchButton *)0x0) {
    pvVar1 = (void *)TouchButton::~TouchButton((TouchButton *)this->field_8);
    operator_delete(pvVar1);
  }
  this->field_8 = (int *)0x0;
  AbyssEngine::String::~String((String *)&(this->m_pAgentName).size);
  AbyssEngine::String::~String((String *)&this->m_pText);
  return this;
}

```

## target disasm
```
  00177118: push {r4,r5,r7,lr}
  0017711a: add r7,sp,#0x8
  0017711c: mov r4,r0
  0017711e: ldr r0,[r0,#0xc]
  00177120: cbz r0,0x00177132
  00177122: blx 0x00075ae4
  00177126: ldr r0,[r4,#0xc]
  00177128: cbz r0,0x00177132
  0017712a: blx 0x00075af0
  0017712e: blx 0x0006eb48
  00177132: ldr r0,[r4,#0x5c]
  00177134: movs r5,#0x0
  00177136: str r5,[r4,#0xc]
  00177138: cbz r0,0x0017713e
  0017713a: blx 0x0006ebfc
  0017713e: ldr r0,[r4,#0x60]
  00177140: str r5,[r4,#0x5c]
  00177142: cbz r0,0x00177148
  00177144: blx 0x0006ebfc
  00177148: ldr r0,[r4,#0x40]
  0017714a: movs r5,#0x0
  0017714c: str r5,[r4,#0x60]
  0017714e: cbz r0,0x00177158
  00177150: blx 0x00075afc
  00177154: blx 0x0006eb48
  00177158: ldr r0,[r4,#0x0]
  0017715a: str r5,[r4,#0x40]
  0017715c: cbz r0,0x00177166
  0017715e: blx 0x00074d7c
  00177162: blx 0x0006eb48
  00177166: ldr r0,[r4,#0x4]
  00177168: movs r5,#0x0
  0017716a: str r5,[r4,#0x0]
  0017716c: cbz r0,0x00177176
  0017716e: blx 0x00074d7c
  00177172: blx 0x0006eb48
  00177176: ldr r0,[r4,#0x8]
  00177178: str r5,[r4,#0x4]
  0017717a: cbz r0,0x00177184
  0017717c: blx 0x00074d7c
  00177180: blx 0x0006eb48
  00177184: movs r0,#0x0
  00177186: str r0,[r4,#0x8]
  00177188: add.w r0,r4,#0x34
  0017718c: blx 0x0006ee30
  00177190: add.w r0,r4,#0x28
  00177194: blx 0x0006ee30
  00177198: mov r0,r4
  0017719a: pop {r4,r5,r7,pc}
```
