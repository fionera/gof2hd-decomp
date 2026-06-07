# ApplicationManager::OnTouchEnd
elf 0x83c90 body 22
Sig: undefined __thiscall OnTouchEnd(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnTouchEnd() */

void __thiscall AbyssEngine::ApplicationManager::OnTouchEnd(ApplicationManager *this)

{
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xa0) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xa4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return;
}

```

## target disasm
```
  00093c90: movs r1,#0x0
  00093c92: vmov.i32 q8,#0x0
  00093c96: strd r1,r1,[r0,#0x8]
  00093c9a: strd r1,r1,[r0,#0x80]
  00093c9e: adds r0,#0x98
  00093ca0: vst1.64 {d16,d17},[r0]
  00093ca4: bx lr
```
