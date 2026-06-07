# ChoiceWindow::ChoiceWindow
elf 0x1467ec body 108
Sig: undefined __thiscall ChoiceWindow(ChoiceWindow * this)

## decompile
```c

/* ChoiceWindow::ChoiceWindow() */

ChoiceWindow * __thiscall ChoiceWindow::ChoiceWindow(ChoiceWindow *this)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined8 uVar4;
  
  AbyssEngine::String::String((String *)(this + 0x20));
  AbyssEngine::String::String((String *)(this + 0x3c));
  piVar2 = *(int **)(DAT_0015686c + 0x156812);
  iVar1 = **(int **)(DAT_00156868 + 0x156810);
  iVar3 = *(int *)(iVar1 + 0x264);
  *(int *)(this + 8) = iVar3;
  uVar4 = *(undefined8 *)(iVar1 + 0x270);
  *(undefined8 *)(this + 0x48) = *(undefined8 *)(iVar1 + 0x268);
  *(undefined8 *)(this + 0x50) = uVar4;
  iVar1 = *piVar2;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x34) = 0xffffffff;
  *(undefined4 *)(this + 0x38) = 0xffffffff;
  this[0x59] = (ChoiceWindow)0x1;
  *(int *)this = iVar1 / 2 - iVar3 / 2;
  return this;
}

```

## target disasm
```
  001567ec: push {r4,r5,r7,lr}
  001567ee: add r7,sp,#0x8
  001567f0: add.w r5,r0,#0x20
  001567f4: mov r4,r0
  001567f6: mov r0,r5
  001567f8: blx 0x0006efbc
  001567fc: add.w r0,r4,#0x3c
  00156800: blx 0x0006efbc
  00156804: ldr r0,[0x00156868]
  00156806: vmov.i32 q8,#0x0
  0015680a: ldr r1,[0x0015686c]
  0015680c: add r0,pc
  0015680e: add r1,pc
  00156810: ldr r0,[r0,#0x0]
  00156812: ldr r1,[r1,#0x0]
  00156814: ldr r0,[r0,#0x0]
  00156816: ldr.w r2,[r0,#0x264]
  0015681a: add.w r0,r0,#0x268
  0015681e: str r2,[r4,#0x8]
  00156820: vld1.32 {d18,d19},[r0]
  00156824: add.w r0,r4,#0x48
  00156828: vst1.32 {d18,d19},[r0]
  0015682c: ldr r0,[r1,#0x0]
  0015682e: add.w r1,r4,#0x10
  00156832: vst1.32 {d16,d17},[r1]
  00156836: mov.w r1,#0xffffffff
  0015683a: add.w r0,r0,r0, lsr #0x1f
  0015683e: strd r1,r1,[r4,#0x34]
  00156842: movs r1,#0x1
  00156844: strb.w r1,[r4,#0x59]
  00156848: add.w r1,r2,r2, lsr #0x1f
  0015684c: asrs r0,r0,#0x1
  0015684e: sub.w r0,r0,r1, asr #0x1
  00156852: str r0,[r4,#0x0]
  00156854: mov r0,r4
  00156856: pop {r4,r5,r7,pc}
```
