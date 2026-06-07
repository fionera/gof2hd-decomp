# PaintCanvas::CameraGetCurrent
elf 0x7b968 body 18
Sig: undefined __stdcall CameraGetCurrent(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraGetCurrent() */

uint AbyssEngine::PaintCanvas::CameraGetCurrent(void)

{
  int in_r0;
  uint uVar1;
  
  uVar1 = *(uint *)(in_r0 + 0x170);
  if (*(uint *)(in_r0 + 0x164) <= uVar1) {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

```

## target disasm
```
  0008b968: ldr.w r1,[r0,#0x164]
  0008b96c: ldr.w r0,[r0,#0x170]
  0008b970: cmp r0,r1
  0008b972: it cs
  0008b974: mov.cs.w r0,#0xffffffff
  0008b978: bx lr
```
