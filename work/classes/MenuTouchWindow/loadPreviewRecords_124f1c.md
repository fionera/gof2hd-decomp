# MenuTouchWindow::loadPreviewRecords
elf 0x124f1c body 162
Sig: undefined __thiscall loadPreviewRecords(MenuTouchWindow * this)

## decompile
```c

/* MenuTouchWindow::loadPreviewRecords() */

void __thiscall MenuTouchWindow::loadPreviewRecords(MenuTouchWindow *this)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  void *pvVar4;
  RecordHandler *this_00;
  undefined4 uVar5;
  undefined4 *puVar6;
  int *piVar7;
  int iVar8;
  
  iVar1 = DAT_00134fd4;
  puVar2 = (undefined4 *)(DAT_00134fcc + 0x134f36);
  puVar6 = (undefined4 *)(DAT_00134fd0 + 0x134f38);
  *(undefined4 *)(this + 0x194) = 0;
  *(undefined4 *)(this + 0x215) = 0;
  *(undefined4 *)(this + 0x219) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x21d) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x221) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar7 = (int *)*puVar6;
  piVar3 = (int *)*puVar2;
  *(undefined4 *)(this + 0x20c) = 0;
  *(undefined4 *)(this + 0x210) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x214) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x218) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar8 = *piVar7;
  piVar7 = *(int **)(iVar1 + 0x134f4c);
  *(int *)(this + 0x228) =
       (((*piVar3 - *(int *)(iVar8 + 0x10)) - *(int *)(iVar8 + 0xc)) - *(int *)(iVar8 + 0x20)) -
       *(int *)(iVar8 + 0x24);
  *(int *)(this + 0x22c) = *piVar7 * (*(int *)(iVar8 + 0x70) + *(int *)(this + 0x1b4));
  if (*(Array<GameRecord*> **)(this + 0xbc) != (Array<GameRecord*> *)0x0) {
    pvVar4 = (void *)Array<GameRecord*>::~Array(*(Array<GameRecord*> **)(this + 0xbc));
    operator_delete(pvVar4);
    *(undefined4 *)(this + 0xbc) = 0;
  }
  this_00 = operator_new(0x2c);
  RecordHandler::RecordHandler(this_00);
  uVar5 = RecordHandler::readAllPreviewRecords(this_00);
  *(undefined4 *)(this + 0xbc) = uVar5;
  RecordHandler::~RecordHandler(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00134f1c: push {r4,r5,r6,r7,lr}
  00134f1e: add r7,sp,#0xc
  00134f20: push.w r8
  00134f24: mov r4,r0
  00134f26: ldr r0,[0x00134fcc]
  00134f28: ldr r1,[0x00134fd0]
  00134f2a: vmov.i32 q8,#0x0
  00134f2e: addw r3,r4,#0x215
  00134f32: add r0,pc
  00134f34: add r1,pc
  00134f36: mov.w r8,#0x0
  00134f3a: ldr r2,[0x00134fd4]
  00134f3c: str.w r8,[r4,#0x194]
  00134f40: vst1.8 {d16,d17},[r3]
  00134f44: add.w r3,r4,#0x20c
  00134f48: add r2,pc
  00134f4a: ldr r1,[r1,#0x0]
  00134f4c: ldr r0,[r0,#0x0]
  00134f4e: vst1.32 {d16,d17},[r3]
  00134f52: ldr r1,[r1,#0x0]
  00134f54: ldr r0,[r0,#0x0]
  00134f56: ldrd r3,r5,[r1,#0xc]
  00134f5a: ldr.w r12,[r2,#0x0]
  00134f5e: subs r0,r0,r5
  00134f60: ldrd r6,r2,[r1,#0x20]
  00134f64: subs r3,r0,r3
  00134f66: ldr.w r0,[r4,#0xbc]
  00134f6a: subs r3,r3,r6
  00134f6c: ldr.w r5,[r4,#0x1b4]
  00134f70: subs r2,r3,r2
  00134f72: str.w r2,[r4,#0x228]
  00134f76: ldr r1,[r1,#0x70]
  00134f78: cmp r0,#0x0
  00134f7a: ldr.w r2,[r12,#0x0]
  00134f7e: add r1,r5
  00134f80: mul r1,r2,r1
  00134f84: str.w r1,[r4,#0x22c]
  00134f88: beq 0x00134f96
  00134f8a: blx 0x00076534
  00134f8e: blx 0x0006eb48
  00134f92: str.w r8,[r4,#0xbc]
  00134f96: movs r0,#0x2c
  00134f98: blx 0x0006eb24
  00134f9c: mov r5,r0
  00134f9e: blx 0x0007177c
  00134fa2: mov r0,r5
  00134fa4: blx 0x00076540
  00134fa8: str.w r0,[r4,#0xbc]
  00134fac: mov r0,r5
  00134fae: blx 0x00071794
  00134fb2: pop.w r8
  00134fb6: pop.w {r4,r5,r6,r7,lr}
  00134fba: b.w 0x001ab098
```
