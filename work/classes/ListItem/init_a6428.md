# ListItem::init
elf 0xa6428 body 50
Sig: int __thiscall init(ListItem * this, EVP_PKEY_CTX * ctx)

## decompile
```c

/* ListItem::init() */

int __thiscall ListItem::init(ListItem *this,EVP_PKEY_CTX *ctx)

{
  this[0x24] = (ListItem)0x0;
  *(undefined4 *)(this + 0x28) = 0xffffffff;
  *(undefined4 *)(this + 0x2c) = 0xffffffff;
  *(undefined4 *)(this + 0x30) = 0xffffffff;
  *(undefined4 *)(this + 0x34) = 0xffffffff;
  this[0x38] = (ListItem)0x0;
  *(undefined4 *)(this + 0x3c) = 0xffffffff;
  *(undefined4 *)(this + 0x40) = 0xffffffff;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined2 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return (int)(this + 4);
}

```

## target disasm
```
  000b6428: movs r1,#0x0
  000b642a: vmov.i32 q8,#0x0
  000b642e: mov.w r2,#0xffffffff
  000b6432: strb.w r1,[r0,#0x24]
  000b6436: strd r2,r2,[r0,#0x28]
  000b643a: strd r2,r2,[r0,#0x30]
  000b643e: strb.w r1,[r0,#0x38]
  000b6442: strd r2,r2,[r0,#0x3c]
  000b6446: strd r1,r1,[r0,#0x14]
  000b644a: strd r1,r1,[r0,#0x1c]
  000b644e: strh.w r1,[r0,#0x44]
  000b6452: adds r0,#0x4
  000b6454: vst1.32 {d16,d17},[r0]
  000b6458: bx lr
```
