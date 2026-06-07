# MenuTouchWindow::loadGame
elf 0x125c2c body 282
Sig: undefined __thiscall loadGame(MenuTouchWindow * this, int param_1)

## decompile
```c

/* MenuTouchWindow::loadGame(int) */

undefined4 __thiscall MenuTouchWindow::loadGame(MenuTouchWindow *this,int param_1)

{
  RecordHandler *this_00;
  GameRecord *this_01;
  undefined4 *puVar1;
  String *pSVar2;
  void *pvVar3;
  ModStation *this_02;
  uint uVar4;
  uint *puVar5;
  ChoiceWindow *pCVar6;
  
  this_00 = operator_new(0x2c);
  RecordHandler::RecordHandler(this_00);
  this_01 = (GameRecord *)RecordHandler::readRecord((int)this_00);
  if (this_01 == (GameRecord *)0x0) {
    Status::resetGame();
    pCVar6 = *(ChoiceWindow **)(this + 0x104);
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_00135d64 + 0x135c88));
    ChoiceWindow::set(pCVar6,pSVar2,false);
    this[0x17e] = (MenuTouchWindow)0x1;
    this[0x170] = (MenuTouchWindow)0x1;
  }
  else {
    if ((this_01[0x117] == (GameRecord)0x0) ||
       (*(char *)(*(int *)(DAT_00135d54 + 0x135c5a) + 0x37) != '\0')) {
      if ((this_01[0x115] == (GameRecord)0x0) ||
         (*(char *)(*(int *)(DAT_00135d54 + 0x135c5a) + 0x35) != '\0')) {
        Status::resetGame();
        GameRecord::load();
        pvVar3 = (void *)RecordHandler::~RecordHandler(this_00);
        operator_delete(pvVar3);
        pvVar3 = (void *)GameRecord::~GameRecord(this_01);
        operator_delete(pvVar3);
        puVar5 = *(uint **)(DAT_00135d6c + 0x135d24);
        this_02 = (ModStation *)AbyssEngine::ApplicationManager::GetApplicationModule(*puVar5);
        ModStation::setGameLoaded(this_02);
        uVar4 = *puVar5;
        **(undefined4 **)(DAT_00135d70 + 0x135d36) = 0;
        AbyssEngine::ApplicationManager::SetCurrentApplicationModule(uVar4);
        return 1;
      }
      pCVar6 = *(ChoiceWindow **)(this + 0x104);
      puVar1 = (undefined4 *)(DAT_00135d5c + 0x135cc0);
    }
    else {
      pCVar6 = *(ChoiceWindow **)(this + 0x104);
      puVar1 = (undefined4 *)(DAT_00135d58 + 0x135c70);
    }
    pSVar2 = (String *)GameText::getText(*(int *)*puVar1);
    ChoiceWindow::set(pCVar6,pSVar2,false);
    this[0x170] = (MenuTouchWindow)0x1;
    pvVar3 = (void *)GameRecord::~GameRecord(this_01);
    operator_delete(pvVar3);
  }
  pvVar3 = (void *)RecordHandler::~RecordHandler(this_00);
  operator_delete(pvVar3);
  return 0;
}

```

## target disasm
```
  00135c2c: push {r4,r5,r6,r7,lr}
  00135c2e: add r7,sp,#0xc
  00135c30: push {r8,r9,r11}
  00135c34: mov r5,r0
  00135c36: movs r0,#0x2c
  00135c38: mov r6,r1
  00135c3a: blx 0x0006eb24
  00135c3e: mov r4,r0
  00135c40: blx 0x0007177c
  00135c44: mov r0,r4
  00135c46: mov r1,r6
  00135c48: blx 0x000765c4
  00135c4c: mov r6,r0
  00135c4e: cbz r0,0x00135c70
  00135c50: ldr r0,[0x00135d54]
  00135c52: ldrb.w r1,[r6,#0x117]
  00135c56: add r0,pc
  00135c58: cmp r1,#0x0
  00135c5a: ldr r0,[r0,#0x0]
  00135c5c: beq 0x00135ca8
  00135c5e: ldrb.w r1,[r0,#0x37]
  00135c62: cbnz r1,0x00135ca8
  00135c64: ldr r0,[0x00135d58]
  00135c66: movs r1,#0x66
  00135c68: ldr.w r9,[r5,#0x104]
  00135c6c: add r0,pc
  00135c6e: b 0x00135cbe
  00135c70: ldr r0,[0x00135d60]
  00135c72: add r0,pc
  00135c74: ldr r0,[r0,#0x0]
  00135c76: ldr r0,[r0,#0x0]
  00135c78: blx 0x00074d58
  00135c7c: ldr r0,[0x00135d64]
  00135c7e: movs r1,#0x64
  00135c80: ldr.w r6,[r5,#0x104]
  00135c84: add r0,pc
  00135c86: ldr r0,[r0,#0x0]
  00135c88: ldr r0,[r0,#0x0]
  00135c8a: blx 0x00072f70
  00135c8e: mov r1,r0
  00135c90: mov r0,r6
  00135c92: movs r2,#0x0
  00135c94: mov.w r8,#0x0
  00135c98: blx 0x000746f8
  00135c9c: movs r0,#0x1
  00135c9e: strb.w r0,[r5,#0x17e]
  00135ca2: strb.w r0,[r5,#0x170]
  00135ca6: b 0x00135ce4
  00135ca8: ldrb.w r1,[r6,#0x115]
  00135cac: cbz r1,0x00135cf6
  00135cae: ldrb.w r0,[r0,#0x35]
  00135cb2: cbnz r0,0x00135cf6
  00135cb4: ldr r0,[0x00135d5c]
  00135cb6: movs r1,#0x65
  00135cb8: ldr.w r9,[r5,#0x104]
  00135cbc: add r0,pc
  00135cbe: ldr r0,[r0,#0x0]
  00135cc0: ldr r0,[r0,#0x0]
  00135cc2: blx 0x00072f70
  00135cc6: mov r1,r0
  00135cc8: mov r0,r9
  00135cca: movs r2,#0x0
  00135ccc: mov.w r8,#0x0
  00135cd0: blx 0x000746f8
  00135cd4: movs r0,#0x1
  00135cd6: strb.w r0,[r5,#0x170]
  00135cda: mov r0,r6
  00135cdc: blx 0x000765ac
  00135ce0: blx 0x0006eb48
  00135ce4: mov r0,r4
  00135ce6: blx 0x00071794
  00135cea: blx 0x0006eb48
  00135cee: mov r0,r8
  00135cf0: pop.w {r8,r9,r11}
  00135cf4: pop {r4,r5,r6,r7,pc}
  00135cf6: ldr r0,[0x00135d68]
  00135cf8: add r0,pc
  00135cfa: ldr r0,[r0,#0x0]
  00135cfc: ldr r0,[r0,#0x0]
  00135cfe: blx 0x00074d58
  00135d02: mov r0,r6
  00135d04: blx 0x000765d0
  00135d08: mov r0,r4
  00135d0a: blx 0x00071794
  00135d0e: blx 0x0006eb48
  00135d12: mov r0,r6
  00135d14: blx 0x000765ac
  00135d18: blx 0x0006eb48
  00135d1c: ldr r0,[0x00135d6c]
  00135d1e: movs r1,#0x5
  00135d20: add r0,pc
  00135d22: ldr r4,[r0,#0x0]
  00135d24: ldr r0,[r4,#0x0]
  00135d26: blx 0x00075bd4
  00135d2a: blx 0x000765dc
  00135d2e: ldr r0,[0x00135d70]
  00135d30: movs r2,#0x0
  00135d32: add r0,pc
  00135d34: ldr r1,[r0,#0x0]
  00135d36: ldr r0,[r4,#0x0]
  00135d38: str r2,[r1,#0x0]
  00135d3a: movs r1,#0x5
  00135d3c: blx 0x00071d64
  00135d40: mov.w r8,#0x1
  00135d44: b 0x00135cee
```
