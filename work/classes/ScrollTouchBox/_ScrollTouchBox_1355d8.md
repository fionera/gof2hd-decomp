# ScrollTouchBox::~ScrollTouchBox
elf 0x1355d8 body 34
Sig: undefined __thiscall ~ScrollTouchBox(ScrollTouchBox * this)

## decompile
```c

/* ScrollTouchBox::~ScrollTouchBox() */

ScrollTouchBox * __thiscall ScrollTouchBox::~ScrollTouchBox(ScrollTouchBox *this)

{
  void *pvVar1;
  
  if (*(Array **)this != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)this);
    if (*(Array<AbyssEngine::String*> **)this != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array(*(Array<AbyssEngine::String*> **)this);
      operator_delete(pvVar1);
    }
    *(undefined4 *)this = 0;
  }
  return this;
}

```

## target disasm
```
  001455d8: push {r4,r6,r7,lr}
  001455da: add r7,sp,#0x8
  001455dc: mov r4,r0
  001455de: ldr r0,[r0,#0x0]
  001455e0: cbz r0,0x001455f6
  001455e2: blx 0x0006facc
  001455e6: ldr r0,[r4,#0x0]
  001455e8: cbz r0,0x001455f2
  001455ea: blx 0x0006f64c
  001455ee: blx 0x0006eb48
  001455f2: movs r0,#0x0
  001455f4: str r0,[r4,#0x0]
  001455f6: mov r0,r4
  001455f8: pop {r4,r6,r7,pc}
```
