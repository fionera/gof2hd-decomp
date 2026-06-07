# ScrollTouchWindow::~ScrollTouchWindow
elf 0x174244 body 32
Sig: undefined __thiscall ~ScrollTouchWindow(ScrollTouchWindow * this)

## decompile
```c

/* ScrollTouchWindow::~ScrollTouchWindow() */

ScrollTouchWindow * __thiscall ScrollTouchWindow::~ScrollTouchWindow(ScrollTouchWindow *this)

{
  void *pvVar1;
  
  if (*(ScrollTouchBox **)this != (ScrollTouchBox *)0x0) {
    pvVar1 = (void *)ScrollTouchBox::~ScrollTouchBox(*(ScrollTouchBox **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  AbyssEngine::String::~String((String *)(this + 4));
  return this;
}

```

## target disasm
```
  00184244: push {r4,r6,r7,lr}
  00184246: add r7,sp,#0x8
  00184248: mov r4,r0
  0018424a: ldr r0,[r0,#0x0]
  0018424c: cbz r0,0x00184256
  0018424e: blx 0x000753a0
  00184252: blx 0x0006eb48
  00184256: movs r1,#0x0
  00184258: mov r0,r4
  0018425a: stmia r0!,{r1}
  0018425c: blx 0x0006ee30
  00184260: mov r0,r4
  00184262: pop {r4,r6,r7,pc}
```
