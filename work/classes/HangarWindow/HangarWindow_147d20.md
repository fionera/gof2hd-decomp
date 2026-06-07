# HangarWindow::HangarWindow
elf 0x147d20 body 114
Sig: undefined __thiscall HangarWindow(HangarWindow * this)

## decompile
```c

/* HangarWindow::HangarWindow() */

void __thiscall HangarWindow::HangarWindow(HangarWindow *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  
  puVar4 = (undefined4 *)(DAT_00157d94 + 0x157d34);
  puVar7 = (undefined4 *)(DAT_00157d98 + 0x157d38);
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  puVar4 = (undefined4 *)*puVar4;
  this[0x3c] = (HangarWindow)0x0;
  this[0xac] = (HangarWindow)0x0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar6 = (int *)*puVar7;
  this[0x11e] = (HangarWindow)0x0;
  *puVar4 = 1;
  this[0x130] = (HangarWindow)0x0;
  this[0xd0] = (HangarWindow)0x0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0xae) = 0;
  iVar5 = *piVar6;
  uVar1 = *(undefined4 *)(iVar5 + 0x23c);
  uVar2 = *(undefined4 *)(iVar5 + 0x240);
  uVar3 = *(undefined4 *)(iVar5 + 0x244);
  *(undefined4 *)(this + 0x100) = *(undefined4 *)(iVar5 + 0x238);
  *(undefined4 *)(this + 0x104) = uVar1;
  *(undefined4 *)(this + 0x108) = uVar2;
  *(undefined4 *)(this + 0x10c) = uVar3;
  *(undefined4 *)(this + 0x110) = *(undefined4 *)(iVar5 + 0x248);
  *(undefined4 *)(this + 0x114) = *(undefined4 *)(iVar5 + 0x24c);
  *(undefined4 *)(this + 0x118) = *(undefined4 *)(iVar5 + 0x250);
  return;
}

```

## target disasm
```
  00157d20: push {r4,r6,r7,lr}
  00157d22: add r7,sp,#0x8
  00157d24: ldr r1,[0x00157d94]
  00157d26: vmov.i32 q8,#0x0
  00157d2a: ldr r4,[0x00157d98]
  00157d2c: add.w r2,r0,#0x14
  00157d30: add r1,pc
  00157d32: movs r3,#0x0
  00157d34: add r4,pc
  00157d36: strd r3,r3,[r0,#0x80]
  00157d3a: ldr r1,[r1,#0x0]
  00157d3c: strb.w r3,[r0,#0x3c]
  00157d40: strb.w r3,[r0,#0xac]
  00157d44: vst1.32 {d16,d17},[r2]
  00157d48: ldr r2,[r4,#0x0]
  00157d4a: movs r4,#0x1
  00157d4c: strb.w r3,[r0,#0x11e]
  00157d50: str r4,[r1,#0x0]
  00157d52: strb.w r3,[r0,#0x130]
  00157d56: strb.w r3,[r0,#0xd0]
  00157d5a: str r3,[r0,#0x4]
  00157d5c: str r3,[r0,#0x24]
  00157d5e: str.w r3,[r0,#0x90]
  00157d62: str.w r3,[r0,#0xae]
  00157d66: ldr r1,[r2,#0x0]
  00157d68: add.w r2,r1,#0x238
  00157d6c: vld1.32 {d16,d17},[r2]
  00157d70: add.w r2,r0,#0x100
  00157d74: vst1.32 {d16,d17},[r2]
  00157d78: ldr.w r2,[r1,#0x248]
  00157d7c: str.w r2,[r0,#0x110]
  00157d80: ldr.w r2,[r1,#0x24c]
  00157d84: str.w r2,[r0,#0x114]
  00157d88: ldr.w r1,[r1,#0x250]
  00157d8c: str.w r1,[r0,#0x118]
  00157d90: pop {r4,r6,r7,pc}
```
