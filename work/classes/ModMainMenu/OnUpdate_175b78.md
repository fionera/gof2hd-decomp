# ModMainMenu::OnUpdate
elf 0x175b78 body 128
Sig: undefined __thiscall OnUpdate(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::OnUpdate() */

void __thiscall ModMainMenu::OnUpdate(ModMainMenu *this)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int *piVar4;
  
  iVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
  if ((iVar1 < 0x97) && (iVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis(), iVar1 < 0)
     ) {
    uVar3 = 0;
  }
  else {
    iVar1 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    if (iVar1 < 0x97) {
      uVar3 = AbyssEngine::ApplicationManager::GetElapsedTimeMillis();
    }
    else {
      uVar3 = 0x96;
    }
  }
  iVar1 = DAT_00185bf8;
  *(undefined4 *)(this + 0x14) = uVar3;
  piVar4 = *(int **)(iVar1 + 0x185bb4);
  Layout::update(*piVar4);
  CutScene::update(*(int *)(this + 0x1c));
  if (this[0x28] == (ModMainMenu)0x0) {
    MenuTouchWindow::update(*(MenuTouchWindow **)(this + 0x18),*(int *)(this + 0x14));
  }
  Layout::update(*piVar4);
  puVar2 = *(undefined4 **)(DAT_00185bfc + 0x185be2);
  *(int *)(this + 0x24) = *(int *)(this + 0x14) + *(int *)(this + 0x24);
  FModSound::updateAll((Vector *)*puVar2,(Vector *)0x0,(Vector *)0x0,(Vector *)0x0);
  return;
}

```

## target disasm
```
  00185b78: push {r2,r3,r4,r5,r7,lr}
  00185b7a: add r7,sp,#0x10
  00185b7c: mov r4,r0
  00185b7e: ldr r0,[r0,#0x8]
  00185b80: blx 0x00070e4c
  00185b84: cmp r0,#0x96
  00185b86: bgt 0x00185b92
  00185b88: ldr r0,[r4,#0x8]
  00185b8a: blx 0x00070e4c
  00185b8e: cmp r0,#0x0
  00185b90: blt 0x00185baa
  00185b92: ldr r0,[r4,#0x8]
  00185b94: blx 0x00070e4c
  00185b98: cmp r0,#0x96
  00185b9a: ble 0x00185ba0
  00185b9c: movs r1,#0x96
  00185b9e: b 0x00185bac
  00185ba0: ldr r0,[r4,#0x8]
  00185ba2: blx 0x00070e4c
  00185ba6: mov r1,r0
  00185ba8: b 0x00185bac
  00185baa: movs r1,#0x0
  00185bac: ldr r0,[0x00185bf8]
  00185bae: str r1,[r4,#0x14]
  00185bb0: add r0,pc
  00185bb2: ldr r5,[r0,#0x0]
  00185bb4: ldr r0,[r5,#0x0]
  00185bb6: blx 0x00075448
  00185bba: ldr r1,[r4,#0x14]
  00185bbc: ldr r0,[r4,#0x1c]
  00185bbe: blx 0x000754a8
  00185bc2: ldrb.w r0,[r4,#0x28]
  00185bc6: cbnz r0,0x00185bd0
  00185bc8: ldrd r1,r0,[r4,#0x14]
  00185bcc: blx 0x00075508
  00185bd0: ldr r1,[r4,#0x14]
  00185bd2: ldr r0,[r5,#0x0]
  00185bd4: blx 0x00075448
  00185bd8: ldr r0,[0x00185bfc]
  00185bda: movs r3,#0x0
  00185bdc: ldr r1,[r4,#0x14]
  00185bde: add r0,pc
  00185be0: ldr r2,[r4,#0x24]
  00185be2: ldr r0,[r0,#0x0]
  00185be4: add r1,r2
  00185be6: str r1,[r4,#0x24]
  00185be8: movs r1,#0x0
  00185bea: str r1,[sp,#0x0]
  00185bec: movs r1,#0x0
  00185bee: ldr r0,[r0,#0x0]
  00185bf0: movs r2,#0x0
  00185bf2: blx 0x0007543c
  00185bf6: pop {r2,r3,r4,r5,r7,pc}
```
