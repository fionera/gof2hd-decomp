# WantedWindow::render3D
elf 0xe182c body 12
Sig: undefined __thiscall render3D(WantedWindow * this)

## decompile
```c

/* WantedWindow::render3D() */

void __thiscall WantedWindow::render3D(WantedWindow *this)

{
  if (this[0x14] == (WantedWindow)0x0) {
    return;
  }
  (*(code *)(DAT_001ac264 + 0x1ac268))(*(undefined4 *)(this + 4));
  return;
}

```

## target disasm
```
  000f182c: ldrb r1,[r0,#0x14]
  000f182e: cbz r1,0x000f1836
  000f1830: ldr r0,[r0,#0x4]
  000f1832: b.w 0x001ac258
  000f1836: bx lr
```
