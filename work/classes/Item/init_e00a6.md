# Item::init
elf 0xe00a6 body 82
Sig: int __stdcall init(EVP_PKEY_CTX * ctx)

## decompile
```c

/* Item::init() */

int Item::init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  if (*(int *)(ctx + 0x30) != 0) {
    iVar1 = *(int *)(*(int *)(ctx + 0x30) + 4);
    *(undefined4 *)ctx = *(undefined4 *)(iVar1 + 4);
    *(undefined4 *)(ctx + 4) = *(undefined4 *)(iVar1 + 0xc);
    *(undefined4 *)(ctx + 8) = *(undefined4 *)(iVar1 + 0x14);
    *(undefined4 *)(ctx + 0xc) = *(undefined4 *)(iVar1 + 0x1c);
    *(undefined4 *)(ctx + 0x1c) = *(undefined4 *)(iVar1 + 0x34);
    iVar2 = *(int *)(iVar1 + 0x3c);
    *(int *)(ctx + 0x20) = iVar2;
    iVar3 = *(int *)(iVar1 + 0x44);
    *(int *)(ctx + 0x24) = iVar3;
    *(undefined4 *)(ctx + 0x10) = *(undefined4 *)(iVar1 + 0x24);
    uVar4 = *(undefined4 *)(iVar1 + 0x2c);
    *(undefined4 *)(ctx + 0x34) = 0;
    *(undefined4 *)(ctx + 0x38) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    *(undefined4 *)(ctx + 0x3c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    *(undefined4 *)(ctx + 0x40) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    *(undefined4 *)(ctx + 0x14) = uVar4;
    ctx[0x44] = (EVP_PKEY_CTX)0x0;
    *(int *)(ctx + 0x18) = iVar2 + (iVar3 - iVar2) / 2;
  }
  return (int)ctx;
}

```
## target disasm
```
  000f00a6: ldr r1,[r0,#0x30]
  000f00a8: cbz r1,0x000f00f6
  000f00aa: ldr r1,[r1,#0x4]
  000f00ac: vmov.i32 q8,#0x0
  000f00b0: ldr r2,[r1,#0x4]
  000f00b2: str r2,[r0,#0x0]
  000f00b4: ldr r2,[r1,#0xc]
  000f00b6: str r2,[r0,#0x4]
  000f00b8: ldr r2,[r1,#0x14]
  000f00ba: str r2,[r0,#0x8]
  000f00bc: ldr r2,[r1,#0x1c]
  000f00be: str r2,[r0,#0xc]
  000f00c0: ldr r2,[r1,#0x34]
  000f00c2: str r2,[r0,#0x1c]
  000f00c4: ldr r2,[r1,#0x3c]
  000f00c6: str r2,[r0,#0x20]
  000f00c8: ldr r3,[r1,#0x44]
  000f00ca: str r3,[r0,#0x24]
  000f00cc: ldr.w r12,[r1,#0x24]
  000f00d0: str.w r12,[r0,#0x10]
  000f00d4: ldr.w r12,[r1,#0x2c]
  000f00d8: add.w r1,r0,#0x34
  000f00dc: vst1.32 {d16,d17},[r1]
  000f00e0: movs r1,#0x0
  000f00e2: str.w r12,[r0,#0x14]
  000f00e6: strb.w r1,[r0,#0x44]
  000f00ea: subs r1,r3,r2
  000f00ec: add.w r1,r1,r1, lsr #0x1f
  000f00f0: add.w r1,r2,r1, asr #0x1
  000f00f4: str r1,[r0,#0x18]
  000f00f6: bx lr
```
