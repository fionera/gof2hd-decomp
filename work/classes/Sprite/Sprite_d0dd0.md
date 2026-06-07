# Sprite::Sprite
elf 0xd0dd0 body 92
Sig: undefined __thiscall Sprite(Sprite * this, uint * param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Sprite::Sprite(unsigned int*, int, int, int) */

Sprite * __thiscall Sprite::Sprite(Sprite *this,uint *param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  uint *puVar3;
  
  puVar1 = (undefined4 *)(DAT_000e0e2c + 0xe0de6);
  *(uint **)this = param_1;
  *(undefined4 *)(this + 4) = 0xffffffff;
  puVar3 = (uint *)*puVar1;
  *(int *)(this + 0x18) = param_3;
  *(int *)(this + 0x1c) = param_4;
  uVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar3);
  *(undefined4 *)(this + 0x20) = uVar2;
  uVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar3);
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x24) = uVar2;
  *(undefined4 *)(this + 0x30) = 1;
  *(undefined4 *)(this + 0x34) = 1;
  *(int *)(this + 0x3c) = param_2;
  setFrame(this,0);
  return this;
}

```

## target disasm
```
  000e0dd0: push {r4,r5,r6,r7,lr}
  000e0dd2: add r7,sp,#0xc
  000e0dd4: push.w r11
  000e0dd8: mov r4,r0
  000e0dda: ldr r0,[0x000e0e2c]
  000e0ddc: ldr.w r12,[r7,#0x8]
  000e0de0: mov r5,r2
  000e0de2: add r0,pc
  000e0de4: mov.w r2,#0xffffffff
  000e0de8: strd r1,r2,[r4,#0x0]
  000e0dec: ldr r6,[r0,#0x0]
  000e0dee: strd r3,r12,[r4,#0x18]
  000e0df2: ldr r1,[r1,#0x0]
  000e0df4: ldr r0,[r6,#0x0]
  000e0df6: blx 0x00072d84
  000e0dfa: ldr r1,[r4,#0x0]
  000e0dfc: str r0,[r4,#0x20]
  000e0dfe: ldr r0,[r6,#0x0]
  000e0e00: ldr r1,[r1,#0x0]
  000e0e02: blx 0x00072d90
  000e0e06: vmov.i32 q8,#0x0
  000e0e0a: add.w r1,r4,#0x8
  000e0e0e: vst1.32 {d16,d17},[r1]
  000e0e12: movs r1,#0x0
  000e0e14: str r0,[r4,#0x24]
  000e0e16: movs r0,#0x1
  000e0e18: strd r0,r0,[r4,#0x30]
  000e0e1c: mov r0,r4
  000e0e1e: str r5,[r4,#0x3c]
  000e0e20: blx 0x00074d70
  000e0e24: mov r0,r4
  000e0e26: pop.w r11
  000e0e2a: pop {r4,r5,r6,r7,pc}
```
