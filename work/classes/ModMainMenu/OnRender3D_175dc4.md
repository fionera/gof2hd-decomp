# ModMainMenu::OnRender3D
elf 0x175dc4 body 48
Sig: undefined __stdcall OnRender3D(void)

## decompile
```c

/* ModMainMenu::OnRender3D() */

void ModMainMenu::OnRender3D(void)

{
  uint *puVar1;
  
  puVar1 = *(uint **)(DAT_00185df4 + 0x185dd2);
  AbyssEngine::PaintCanvas::ClearBuffer(*puVar1);
  CutScene::renderBG();
  AbyssEngine::PaintCanvas::Begin3d();
  CutScene::render3D();
  (*(code *)(DAT_001ab924 + 0x1ab928))(*puVar1);
  return;
}

```

## target disasm
```
  00185dc4: push {r4,r5,r7,lr}
  00185dc6: add r7,sp,#0x8
  00185dc8: mov r4,r0
  00185dca: ldr r0,[0x00185df4]
  00185dcc: movs r1,#0x0
  00185dce: add r0,pc
  00185dd0: ldr r5,[r0,#0x0]
  00185dd2: ldr r0,[r5,#0x0]
  00185dd4: blx 0x00071d94
  00185dd8: ldr r0,[r4,#0x1c]
  00185dda: blx 0x000756a0
  00185dde: ldr r0,[r5,#0x0]
  00185de0: blx 0x00071da0
  00185de4: ldr r0,[r4,#0x1c]
  00185de6: blx 0x00075694
  00185dea: ldr r0,[r5,#0x0]
  00185dec: pop.w {r4,r5,r7,lr}
  00185df0: b.w 0x001ab918
```
