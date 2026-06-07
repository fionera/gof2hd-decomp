# MissionsWindow::MissionsWindow
elf 0x14fa10 body 78
Sig: undefined __thiscall MissionsWindow(MissionsWindow * this)

## decompile
```c

/* MissionsWindow::MissionsWindow() */

MissionsWindow * __thiscall MissionsWindow::MissionsWindow(MissionsWindow *this)

{
  undefined4 *puVar1;
  uint *puVar2;
  int iVar3;
  
  puVar1 = (undefined4 *)(DAT_0015fa60 + 0x15fa20);
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  puVar2 = (uint *)*puVar1;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar3 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar2);
  *(int *)(this + 0x1c) = iVar3 / 2 + -1;
  init((EVP_PKEY_CTX *)this);
  return this;
}

```

## target disasm
```
  0015fa10: push {r4,r5,r7,lr}
  0015fa12: add r7,sp,#0x8
  0015fa14: mov r4,r0
  0015fa16: ldr r0,[0x0015fa60]
  0015fa18: ldr r1,[0x0015fa64]
  0015fa1a: movs r2,#0x0
  0015fa1c: add r0,pc
  0015fa1e: vmov.i32 q8,#0x0
  0015fa22: add r1,pc
  0015fa24: strd r2,r2,[r4,#0x24]
  0015fa28: str r2,[r4,#0x2c]
  0015fa2a: add.w r2,r4,#0xc
  0015fa2e: ldr r0,[r0,#0x0]
  0015fa30: mov r5,r4
  0015fa32: ldr r1,[r1,#0x0]
  0015fa34: vst1.32 {d16,d17},[r2]
  0015fa38: movs r2,#0x1c
  0015fa3a: vst1.32 {d16,d17},[r5],r2
  0015fa3e: ldr r1,[r1,#0x0]
  0015fa40: ldr r0,[r0,#0x0]
  0015fa42: blx 0x0006fe20
  0015fa46: add.w r0,r0,r0, lsr #0x1f
  0015fa4a: mov.w r1,#0xffffffff
  0015fa4e: add.w r0,r1,r0, asr #0x1
  0015fa52: str r0,[r5,#0x0]
  0015fa54: mov r0,r4
  0015fa56: blx 0x000753d0
  0015fa5a: mov r0,r4
  0015fa5c: pop {r4,r5,r7,pc}
```
