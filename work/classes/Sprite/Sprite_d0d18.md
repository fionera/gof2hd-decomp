# Sprite::Sprite
elf 0xd0d18 body 106
Sig: undefined __thiscall Sprite(Sprite * this, uint param_1, int param_2, int param_3)

## decompile
```c

/* Sprite::Sprite(unsigned int, int, int) */

Sprite * __thiscall Sprite::Sprite(Sprite *this,uint param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  puVar1 = (undefined4 *)(DAT_000e0d84 + 0xe0d2a);
  *(undefined4 *)this = 0;
  *(uint *)(this + 4) = param_1;
  *(int *)(this + 0x18) = param_2;
  *(int *)(this + 0x1c) = param_3;
  puVar5 = (uint *)*puVar1;
  uVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar5);
  *(undefined4 *)(this + 0x20) = uVar2;
  uVar2 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar5);
  uVar6 = *(undefined4 *)(this + 0x18);
  uVar7 = *(undefined4 *)(this + 0x20);
  iVar3 = __aeabi_idiv(uVar2,*(undefined4 *)(this + 0x1c));
  iVar4 = __aeabi_idiv(uVar7,uVar6);
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x24) = uVar2;
  *(int *)(this + 0x30) = iVar4;
  *(int *)(this + 0x34) = iVar3;
  *(int *)(this + 0x3c) = iVar3 * iVar4;
  setFrame(this,0);
  return this;
}

```

## target disasm
```
  000e0d18: push {r4,r5,r6,r7,lr}
  000e0d1a: add r7,sp,#0xc
  000e0d1c: push {r8,r9,r11}
  000e0d20: mov r4,r0
  000e0d22: ldr r0,[0x000e0d84]
  000e0d24: movs r6,#0x0
  000e0d26: add r0,pc
  000e0d28: strd r6,r1,[r4,#0x0]
  000e0d2c: strd r2,r3,[r4,#0x18]
  000e0d30: ldr r5,[r0,#0x0]
  000e0d32: ldr r0,[r5,#0x0]
  000e0d34: blx 0x00072d84
  000e0d38: ldr r1,[r4,#0x4]
  000e0d3a: str r0,[r4,#0x20]
  000e0d3c: ldr r0,[r5,#0x0]
  000e0d3e: blx 0x00072d90
  000e0d42: ldrd r8,r1,[r4,#0x18]
  000e0d46: mov r5,r0
  000e0d48: ldr.w r9,[r4,#0x20]
  000e0d4c: blx 0x0007198c
  000e0d50: mov r6,r0
  000e0d52: mov r0,r9
  000e0d54: mov r1,r8
  000e0d56: blx 0x0007198c
  000e0d5a: vmov.i32 q8,#0x0
  000e0d5e: mul r2,r6,r0
  000e0d62: add.w r1,r4,#0x8
  000e0d66: vst1.32 {d16,d17},[r1]
  000e0d6a: movs r1,#0x0
  000e0d6c: str r5,[r4,#0x24]
  000e0d6e: strd r0,r6,[r4,#0x30]
  000e0d72: mov r0,r4
  000e0d74: str r2,[r4,#0x3c]
  000e0d76: blx 0x00074d70
  000e0d7a: mov r0,r4
  000e0d7c: pop.w {r8,r9,r11}
  000e0d80: pop {r4,r5,r6,r7,pc}
```
