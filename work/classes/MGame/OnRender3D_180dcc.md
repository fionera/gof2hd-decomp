# MGame::OnRender3D
elf 0x180dcc body 246
Sig: undefined __thiscall OnRender3D(MGame * this)

## decompile
```c

/* MGame::OnRender3D() */

void __thiscall MGame::OnRender3D(MGame *this)

{
  undefined1 uVar1;
  int *piVar2;
  uint *puVar3;
  
  if ((char)this->field_54 == '\0') {
    return;
  }
  puVar3 = *(uint **)(DAT_00190eb8 + 0x190de4);
  AbyssEngine::PaintCanvas::ClearBuffer(*puVar3);
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
    piVar2 = this->field_78;
LAB_00190e06:
    Level::renderBG((int)piVar2);
    AbyssEngine::PaintCanvas::Begin3d();
    Level::render((int)this->field_78);
    uVar1 = SUB41(this->m_pPlayerEgo,0);
  }
  else {
    if (*(char *)((int)&this[1].field_94 + 2) != '\0') {
      piVar2 = this->field_78;
      goto LAB_00190e06;
    }
    if (*(char *)((int)&this[1].field_0 + 1) != '\0') {
      AbyssEngine::PaintCanvas::Begin3d();
      MenuTouchWindow::render3D();
      goto LAB_00190e4c;
    }
    if (this->field_0xc7 != '\0') {
      AbyssEngine::PaintCanvas::Begin3d();
      StarMap::render();
      goto LAB_00190e4c;
    }
    Level::renderBG((int)this->field_78);
    AbyssEngine::PaintCanvas::Begin3d();
    Level::render((int)this->field_78);
    uVar1 = SUB41(this->m_pPlayerEgo,0);
  }
  PlayerEgo::render((bool)uVar1);
  if (this[1].field_4C != 0) {
    AEGeometry::render();
  }
  LevelScript::render3D();
LAB_00190e4c:
  (*(code *)(DAT_001ab924 + 0x1ab928))(*puVar3);
  return;
}

```

## target disasm
```
  00190dcc: push {r4,r5,r6,r7,lr}
  00190dce: add r7,sp,#0xc
  00190dd0: push.w r11
  00190dd4: mov r4,r0
  00190dd6: ldrb.w r0,[r0,#0x54]
  00190dda: cbz r0,0x00190dfa
  00190ddc: ldr r0,[0x00190eb8]
  00190dde: movs r1,#0xff
  00190de0: add r0,pc
  00190de2: ldr r6,[r0,#0x0]
  00190de4: ldr r0,[r6,#0x0]
  00190de6: blx 0x00071d94
  00190dea: ldrb.w r0,[r4,#0x5d]
  00190dee: ldrb.w r1,[r4,#0x15e]
  00190df2: cbz r0,0x00190e00
  00190df4: cbz r1,0x00190e5a
  00190df6: ldr r0,[r4,#0x78]
  00190df8: b 0x00190e04
  00190dfa: pop.w r11
  00190dfe: pop {r4,r5,r6,r7,pc}
  00190e00: ldr r0,[r4,#0x78]
  00190e02: cbz r1,0x00190e70
  00190e04: movs r1,#0x0
  00190e06: blx 0x00072214
  00190e0a: ldr r0,[r6,#0x0]
  00190e0c: blx 0x00071da0
  00190e10: ldrb.w r1,[r4,#0x15e]
  00190e14: ldr r0,[r4,#0x78]
  00190e16: cmp r1,#0x0
  00190e18: ite eq
  00190e1a: ldr.eq r1,[r4,#0x40]
  00190e1c: mov.ne r1,#0x0
  00190e1e: blx 0x0007222c
  00190e22: ldrb.w r1,[r4,#0x5f]
  00190e26: ldr r0,[r4,#0x58]
  00190e28: cbz r1,0x00190e2e
  00190e2a: movs r1,#0x0
  00190e2c: b 0x00190e38
  00190e2e: ldrb.w r1,[r4,#0xdc]
  00190e32: clz r1,r1
  00190e36: lsrs r1,r1,#0x5
  00190e38: blx 0x00078730
  00190e3c: ldr.w r0,[r4,#0x114]
  00190e40: cbz r0,0x00190e46
  00190e42: blx 0x00072238
  00190e46: ldr r0,[r4,#0x7c]
  00190e48: blx 0x0007873c
  00190e4c: ldr r0,[r6,#0x0]
  00190e4e: pop.w r11
  00190e52: pop.w {r4,r5,r6,r7,lr}
  00190e56: b.w 0x001ab918
  00190e5a: ldrb.w r0,[r4,#0xc9]
  00190e5e: cbz r0,0x00190e74
  00190e60: ldr r0,[r6,#0x0]
  00190e62: blx 0x00071da0
  00190e66: ldr.w r0,[r4,#0x88]
  00190e6a: blx 0x00078748
  00190e6e: b 0x00190e4c
  00190e70: ldr r1,[r4,#0x40]
  00190e72: b 0x00190e06
  00190e74: ldrb.w r0,[r4,#0xc7]
  00190e78: cbz r0,0x00190e8a
  00190e7a: ldr r0,[r6,#0x0]
  00190e7c: blx 0x00071da0
  00190e80: ldr.w r0,[r4,#0x90]
  00190e84: blx 0x0007567c
  00190e88: b 0x00190e4c
  00190e8a: ldr r0,[r4,#0x78]
  00190e8c: movs r1,#0x0
  00190e8e: movs r5,#0x0
  00190e90: blx 0x00072214
  00190e94: ldr r0,[r6,#0x0]
  00190e96: blx 0x00071da0
  00190e9a: ldr r0,[r4,#0x78]
  00190e9c: movs r1,#0x0
  00190e9e: blx 0x0007222c
  00190ea2: ldrb.w r1,[r4,#0x5f]
  00190ea6: ldr r0,[r4,#0x58]
  00190ea8: cbnz r1,0x00190eb4
  00190eaa: ldrb.w r1,[r4,#0xdc]
  00190eae: clz r1,r1
  00190eb2: lsrs r5,r1,#0x5
  00190eb4: mov r1,r5
  00190eb6: b 0x00190e38
  001ab918: bx pc
  001ab91c: ldr r12,[0x1ab924]
  001ab920: add pc,r12,pc
```
