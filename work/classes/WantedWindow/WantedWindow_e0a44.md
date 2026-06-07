# WantedWindow::WantedWindow
elf 0xe0a44 body 172
Sig: undefined __thiscall WantedWindow(WantedWindow * this)

## decompile
```c

/* WantedWindow::WantedWindow() */

undefined8 __thiscall WantedWindow::WantedWindow(WantedWindow *this)

{
  undefined4 *puVar1;
  int iVar2;
  EVP_PKEY_CTX *ctx;
  uint *puVar3;
  
  AbyssEngine::String::String((String *)(this + 0x3c));
  AbyssEngine::String::String((String *)(this + 0x48));
  AbyssEngine::String::String((String *)(this + 0x54));
  AbyssEngine::String::String((String *)(this + 0x60));
  AbyssEngine::String::String((String *)(this + 0x6c));
  AbyssEngine::String::String((String *)(this + 0x78));
  puVar1 = (undefined4 *)(DAT_000f0b30 + 0xf0a9e);
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  puVar3 = (uint *)*puVar1;
  iVar2 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar3);
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(int *)(this + 0x10) = iVar2 / 2 + -1;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar3,0x454,(uint *)(this + 0xac));
  init(this,ctx);
  return CONCAT44((String *)(this + 0x54),this);
}

```

## target disasm
```
  000f0a44: push {r4,r5,r6,r7,lr}
  000f0a46: add r7,sp,#0xc
  000f0a48: push {r5,r6,r7,r8,r9,r10,r11}
  000f0a4c: add.w r8,r0,#0x3c
  000f0a50: mov r5,r0
  000f0a52: mov r0,r8
  000f0a54: blx 0x0006efbc
  000f0a58: add.w r6,r5,#0x48
  000f0a5c: mov r0,r6
  000f0a5e: str.w r8,[sp,#0x8]
  000f0a62: blx 0x0006efbc
  000f0a66: add.w r4,r5,#0x54
  000f0a6a: mov r0,r4
  000f0a6c: str r6,[sp,#0x4]
  000f0a6e: blx 0x0006efbc
  000f0a72: add.w r11,r5,#0x60
  000f0a76: mov r0,r11
  000f0a78: str r4,[sp,#0x0]
  000f0a7a: blx 0x0006efbc
  000f0a7e: add.w r10,r5,#0x6c
  000f0a82: mov r0,r10
  000f0a84: blx 0x0006efbc
  000f0a88: add.w r4,r5,#0x78
  000f0a8c: mov r0,r4
  000f0a8e: blx 0x0006efbc
  000f0a92: ldr r0,[0x000f0b30]
  000f0a94: mov.w r9,#0x0
  000f0a98: ldr r1,[0x000f0b34]
  000f0a9a: add r0,pc
  000f0a9c: str.w r9,[r5,#0x18]
  000f0aa0: add r1,pc
  000f0aa2: strd r9,r9,[r5,#0x4]
  000f0aa6: ldr.w r8,[r0,#0x0]
  000f0aaa: ldr r0,[r1,#0x0]
  000f0aac: ldr r1,[r0,#0x0]
  000f0aae: ldr.w r0,[r8,#0x0]
  000f0ab2: blx 0x0006fe20
  000f0ab6: add.w r0,r0,r0, lsr #0x1f
  000f0aba: mov.w r1,#0xffffffff
  000f0abe: str.w r9,[r5,#0x38]
  000f0ac2: add.w r0,r1,r0, asr #0x1
  000f0ac6: str.w r9,[r5,#0xb0]
  000f0aca: str.w r9,[r5,#0x2c]
  000f0ace: strd r9,r0,[r5,#0xc]
  000f0ad2: ldr.w r0,[r8,#0x0]
  000f0ad6: add.w r2,r5,#0xac
  000f0ada: movw r1,#0x454
  000f0ade: blx 0x00071cf8
  000f0ae2: mov r0,r5
  000f0ae4: blx 0x00075a9c
  000f0ae8: mov r0,r5
  000f0aea: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000f0aee: pop {r4,r5,r6,r7,pc}
```
