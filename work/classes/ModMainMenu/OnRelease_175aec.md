# ModMainMenu::OnRelease
elf 0x175aec body 120
Sig: undefined __thiscall OnRelease(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::OnRelease() */

void __thiscall ModMainMenu::OnRelease(ModMainMenu *this)

{
  void *pvVar1;
  int iVar2;
  
  if (*(CutScene **)(this + 0x1c) != (CutScene *)0x0) {
    pvVar1 = (void *)CutScene::~CutScene(*(CutScene **)(this + 0x1c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x1c) = 0;
  if (*(MenuTouchWindow **)(this + 0x18) != (MenuTouchWindow *)0x0) {
    pvVar1 = (void *)MenuTouchWindow::~MenuTouchWindow(*(MenuTouchWindow **)(this + 0x18));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x18) = 0;
  AbyssEngine::PaintCanvas::ReleaseAllResources();
  iVar2 = **(int **)(DAT_00185b68 + 0x185b22);
  GameText::getLanguage();
  Globals::loadFont(iVar2);
  if (**(int **)(DAT_00185b6c + 0x185b36) != 0) {
    Layout::reload();
    ImageFactory::reload();
    Layout::initTip();
  }
  if (**(int **)(DAT_00185b74 + 0x185b56) == 0) {
    return;
  }
  (*(code *)(DAT_001ac174 + 0x1ac178))();
  return;
}

```

## target disasm
```
  00185aec: push {r4,r5,r7,lr}
  00185aee: add r7,sp,#0x8
  00185af0: mov r4,r0
  00185af2: ldr r0,[r0,#0x1c]
  00185af4: cbz r0,0x00185afe
  00185af6: blx 0x00075370
  00185afa: blx 0x0006eb48
  00185afe: ldr r0,[r4,#0x18]
  00185b00: movs r5,#0x0
  00185b02: str r5,[r4,#0x1c]
  00185b04: cbz r0,0x00185b0e
  00185b06: blx 0x0007537c
  00185b0a: blx 0x0006eb48
  00185b0e: ldr r0,[0x00185b64]
  00185b10: str r5,[r4,#0x18]
  00185b12: add r0,pc
  00185b14: ldr r0,[r0,#0x0]
  00185b16: ldr r0,[r0,#0x0]
  00185b18: blx 0x0006f964
  00185b1c: ldr r0,[0x00185b68]
  00185b1e: add r0,pc
  00185b20: ldr r0,[r0,#0x0]
  00185b22: ldr r4,[r0,#0x0]
  00185b24: blx 0x0006f544
  00185b28: mov r1,r0
  00185b2a: mov r0,r4
  00185b2c: blx 0x00071d04
  00185b30: ldr r0,[0x00185b6c]
  00185b32: add r0,pc
  00185b34: ldr r4,[r0,#0x0]
  00185b36: ldr r0,[r4,#0x0]
  00185b38: cbz r0,0x00185b50
  00185b3a: blx 0x00071d10
  00185b3e: ldr r0,[0x00185b70]
  00185b40: add r0,pc
  00185b42: ldr r0,[r0,#0x0]
  00185b44: ldr r0,[r0,#0x0]
  00185b46: blx 0x00071d1c
  00185b4a: ldr r0,[r4,#0x0]
  00185b4c: blx 0x00071d28
  00185b50: ldr r0,[0x00185b74]
  00185b52: add r0,pc
  00185b54: ldr r0,[r0,#0x0]
  00185b56: ldr r0,[r0,#0x0]
  00185b58: cbz r0,0x00185b62
  00185b5a: pop.w {r4,r5,r7,lr}
  00185b5e: b.w 0x001ac168
  00185b62: pop {r4,r5,r7,pc}
```
