# MenuTouchWindow::saveGame
elf 0x125b28 body 164
Sig: undefined __thiscall saveGame(MenuTouchWindow * this, int param_1)

## decompile
```c

/* MenuTouchWindow::saveGame(int) */

void __thiscall MenuTouchWindow::saveGame(MenuTouchWindow *this,int param_1)

{
  RecordHandler *this_00;
  GameRecord *this_01;
  void *pvVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  String *pSVar4;
  ChoiceWindow *this_02;
  
  this_00 = operator_new(0x2c);
  RecordHandler::RecordHandler(this_00);
  RecordHandler::recordStoreWrite(this_00,param_1);
  RecordHandler::recordStoreWritePreview(this_00,param_1);
  this_01 = *(GameRecord **)(*(int *)(*(int *)(this + 0xbc) + 4) + param_1 * 4);
  if (this_01 == (GameRecord *)0x0) {
    puVar2 = (undefined4 *)(*(int *)(*(int *)(this + 0xbc) + 4) + param_1 * 4);
  }
  else {
    pvVar1 = (void *)GameRecord::~GameRecord(this_01);
    operator_delete(pvVar1);
    puVar2 = (undefined4 *)(*(int *)(*(int *)(this + 0xbc) + 4) + param_1 * 4);
  }
  *puVar2 = 0;
  uVar3 = RecordHandler::recordStoreReadPreview((int)this_00);
  *(undefined4 *)(*(int *)(*(int *)(this + 0xbc) + 4) + param_1 * 4) = uVar3;
  pvVar1 = (void *)RecordHandler::~RecordHandler(this_00);
  operator_delete(pvVar1);
  createRecordButtons(SUB41(this,0));
  this_02 = *(ChoiceWindow **)(this + 0x104);
  pSVar4 = (String *)GameText::getText(**(int **)(DAT_00135bdc + 0x135bae));
  ChoiceWindow::set(this_02,pSVar4,false);
  this[0x173] = (MenuTouchWindow)0x0;
  this[0x170] = (MenuTouchWindow)0x1;
  return;
}

```

## target disasm
```
  00135b28: push {r4,r5,r6,r7,lr}
  00135b2a: add r7,sp,#0xc
  00135b2c: push.w r8
  00135b30: mov r4,r0
  00135b32: movs r0,#0x2c
  00135b34: mov r6,r1
  00135b36: blx 0x0006eb24
  00135b3a: mov r5,r0
  00135b3c: blx 0x0007177c
  00135b40: mov r0,r5
  00135b42: mov r1,r6
  00135b44: blx 0x000717a0
  00135b48: mov r0,r5
  00135b4a: mov r1,r6
  00135b4c: blx 0x000717ac
  00135b50: ldr.w r0,[r4,#0xbc]
  00135b54: ldr r1,[r0,#0x4]
  00135b56: ldr.w r0,[r1,r6,lsl #0x2]
  00135b5a: cbz r0,0x00135b70
  00135b5c: blx 0x000765ac
  00135b60: blx 0x0006eb48
  00135b64: ldr.w r0,[r4,#0xbc]
  00135b68: ldr r0,[r0,#0x4]
  00135b6a: add.w r0,r0,r6, lsl #0x2
  00135b6e: b 0x00135b74
  00135b70: add.w r0,r1,r6, lsl #0x2
  00135b74: mov.w r8,#0x0
  00135b78: mov r1,r6
  00135b7a: str.w r8,[r0,#0x0]
  00135b7e: mov r0,r5
  00135b80: blx 0x000747f4
  00135b84: ldr.w r1,[r4,#0xbc]
  00135b88: ldr r1,[r1,#0x4]
  00135b8a: str.w r0,[r1,r6,lsl #0x2]
  00135b8e: mov r0,r5
  00135b90: blx 0x00071794
  00135b94: blx 0x0006eb48
  00135b98: mov r0,r4
  00135b9a: movs r1,#0x1
  00135b9c: movs r6,#0x1
  00135b9e: blx 0x000765b8
  00135ba2: ldr r0,[0x00135bdc]
  00135ba4: movs r1,#0x32
  00135ba6: ldr.w r5,[r4,#0x104]
  00135baa: add r0,pc
  00135bac: ldr r0,[r0,#0x0]
  00135bae: ldr r0,[r0,#0x0]
  00135bb0: blx 0x00072f70
  00135bb4: mov r1,r0
  00135bb6: mov r0,r5
  00135bb8: movs r2,#0x0
  00135bba: blx 0x000746f8
  00135bbe: strb.w r8,[r4,#0x173]
  00135bc2: strb.w r6,[r4,#0x170]
  00135bc6: pop.w r8
  00135bca: pop {r4,r5,r6,r7,pc}
```
