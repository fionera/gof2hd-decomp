# CutScene::CutScene
elf 0x983e4 body 56
Sig: undefined __thiscall CutScene(CutScene * this, int param_1)

## decompile
```c

/* CutScene::CutScene(int) */

void __thiscall CutScene::CutScene(CutScene *this,int param_1)

{
  undefined4 uVar1;
  
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(int *)(this + 0x88) = param_1;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0xffffffff;
  *(undefined4 *)(this + 0x70) = 0xffffffff;
  uVar1 = DAT_000a841c;
  this[0x5c] = (CutScene)0x0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x24) = uVar1;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x30) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x34) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  return;
}

```

## target disasm
```
  000a83e4: movs r3,#0x0
  000a83e6: vmov.i32 q8,#0x0
  000a83ea: strd r3,r3,[r0,#0x18]
  000a83ee: str.w r1,[r0,#0x88]
  000a83f2: mov.w r1,#0xffffffff
  000a83f6: str r3,[r0,#0x0]
  000a83f8: strd r3,r3,[r0,#0x64]
  000a83fc: strd r1,r1,[r0,#0x6c]
  000a8400: add.w r1,r0,#0x8
  000a8404: ldr r2,[0x000a841c]
  000a8406: strb.w r3,[r0,#0x5c]
  000a840a: str r3,[r0,#0x38]
  000a840c: str r2,[r0,#0x24]
  000a840e: vst1.32 {d16,d17},[r1]
  000a8412: add.w r1,r0,#0x28
  000a8416: vst1.64 {d16,d17},[r1]
  000a841a: bx lr
```
