# ListItemWindow::~ListItemWindow
elf 0x1315b4 body 138
Sig: undefined __thiscall ~ListItemWindow(ListItemWindow * this)

## decompile
```c

/* ListItemWindow::~ListItemWindow() */

ListItemWindow * __thiscall ListItemWindow::~ListItemWindow(ListItemWindow *this)

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
  if (*(Array **)(this + 4) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 4));
    if (*(Array<AbyssEngine::String*> **)(this + 4) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 4));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 4) = 0;
  }
  if (*(Array **)(this + 8) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 8));
    if (*(Array<int> **)(this + 8) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 8));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 8) = 0;
  }
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayRelease<int>(*(Array **)(this + 0xc));
    if (*(Array<int> **)(this + 0xc) != (Array<int> *)0x0) {
      pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0xc));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0xc) = 0;
  }
  if (*(ScrollTouchWindow **)(this + 0x18) != (ScrollTouchWindow *)0x0) {
    pvVar1 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  AbyssEngine::String::~String((String *)(this + 0x80));
  AbyssEngine::String::~String((String *)(this + 0x74));
  return this;
}

```

## target disasm
```
  001415b4: push {r4,r5,r7,lr}
  001415b6: add r7,sp,#0x8
  001415b8: mov r4,r0
  001415ba: ldr r0,[r0,#0x0]
  001415bc: cbz r0,0x001415d2
  001415be: blx 0x0006facc
  001415c2: ldr r0,[r4,#0x0]
  001415c4: cbz r0,0x001415ce
  001415c6: blx 0x0006f64c
  001415ca: blx 0x0006eb48
  001415ce: movs r0,#0x0
  001415d0: str r0,[r4,#0x0]
  001415d2: ldr r0,[r4,#0x4]
  001415d4: cbz r0,0x001415ea
  001415d6: blx 0x0006facc
  001415da: ldr r0,[r4,#0x4]
  001415dc: cbz r0,0x001415e6
  001415de: blx 0x0006f64c
  001415e2: blx 0x0006eb48
  001415e6: movs r0,#0x0
  001415e8: str r0,[r4,#0x4]
  001415ea: ldr r0,[r4,#0x8]
  001415ec: cbz r0,0x00141602
  001415ee: blx 0x0007369c
  001415f2: ldr r0,[r4,#0x8]
  001415f4: cbz r0,0x001415fe
  001415f6: blx 0x00070204
  001415fa: blx 0x0006eb48
  001415fe: movs r0,#0x0
  00141600: str r0,[r4,#0x8]
  00141602: ldr r0,[r4,#0xc]
  00141604: cbz r0,0x0014161a
  00141606: blx 0x0007369c
  0014160a: ldr r0,[r4,#0xc]
  0014160c: cbz r0,0x00141616
  0014160e: blx 0x00070204
  00141612: blx 0x0006eb48
  00141616: movs r0,#0x0
  00141618: str r0,[r4,#0xc]
  0014161a: ldr r0,[r4,#0x18]
  0014161c: cbz r0,0x00141626
  0014161e: blx 0x00075afc
  00141622: blx 0x0006eb48
  00141626: movs r0,#0x0
  00141628: str r0,[r4,#0x18]
  0014162a: add.w r0,r4,#0x80
  0014162e: blx 0x0006ee30
  00141632: add.w r0,r4,#0x74
  00141636: blx 0x0006ee30
  0014163a: mov r0,r4
  0014163c: pop {r4,r5,r7,pc}
```
