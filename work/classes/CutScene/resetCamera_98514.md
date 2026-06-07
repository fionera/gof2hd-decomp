# CutScene::resetCamera
elf 0x98514 body 222
Sig: undefined __stdcall resetCamera(void)

## decompile
```c

/* CutScene::resetCamera() */

void CutScene::resetCamera(void)

{
  int in_r0;
  int iVar1;
  undefined4 in_r3;
  undefined4 *puVar2;
  uint *puVar3;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s2_00;
  undefined8 uVar4;
  undefined4 uVar5;
  
  if (*(int *)(in_r0 + 0x88) == 0x17) {
    Status::getSystem();
    iVar1 = SolarSystem::getRace();
    if (iVar1 == 1) {
      puVar2 = *(undefined4 **)(DAT_000a8620 + 0xa85a2);
      in_r3 = DAT_000a85f4;
      AbyssEngine::PaintCanvas::FogSetParameter
                ((PaintCanvas *)*puVar2,0x2601,0,DAT_000a85f0,0x3f800000);
      AbyssEngine::PaintCanvas::FogEnable(*puVar2,1,1);
    }
    puVar3 = *(uint **)(DAT_000a8624 + 0xa85c6);
    AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar3,*(uint *)(in_r0 + 0x70));
    uVar5 = DAT_000a8600;
    AbyssEngine::PaintCanvas::CameraSetPerspective
              (*puVar3,extraout_s0_00,extraout_s1_00,extraout_s2_00);
    uVar4 = Level::getEnemies();
    (*(code *)(DAT_001ab9b4 + 0x1ab9b8))
              (**(undefined4 **)((int)uVar4 + 4),(int)((ulonglong)uVar4 >> 0x20),uVar5,in_r3);
    return;
  }
  if (*(int *)(in_r0 + 0x88) != 4) {
    return;
  }
  Status::getSystem();
  iVar1 = SolarSystem::getRace();
  if (iVar1 == 1) {
    puVar2 = *(undefined4 **)(DAT_000a8614 + 0xa8546);
    AbyssEngine::PaintCanvas::FogSetParameter
              ((PaintCanvas *)*puVar2,0x2601,0,DAT_000a8604,0x3f800000,DAT_000a85f4);
    AbyssEngine::PaintCanvas::FogEnable(*puVar2,1,1);
  }
  puVar3 = *(uint **)(DAT_000a8618 + 0xa856a);
  AbyssEngine::PaintCanvas::CameraSetCurrent((PaintCanvas *)*puVar3,*(uint *)(in_r0 + 0x6c));
  AbyssEngine::PaintCanvas::CameraSetPerspective(*puVar3,extraout_s0,extraout_s1,extraout_s2);
  return;
}

```

## target disasm
```
  000a8514: push {r2,r3,r4,r5,r7,lr}
  000a8516: add r7,sp,#0x10
  000a8518: mov r4,r0
  000a851a: ldr.w r0,[r0,#0x88]
  000a851e: cmp r0,#0x17
  000a8520: beq 0x000a8582
  000a8522: cmp r0,#0x4
  000a8524: bne 0x000a85ee
  000a8526: ldr r0,[0x000a8610]
  000a8528: add r0,pc
  000a852a: ldr r0,[r0,#0x0]
  000a852c: ldr r0,[r0,#0x0]
  000a852e: blx 0x00071a10
  000a8532: blx 0x00071950
  000a8536: cmp r0,#0x1
  000a8538: bne 0x000a8562
  000a853a: ldr r0,[0x000a8614]
  000a853c: mov.w r2,#0x3f800000
  000a8540: ldr r1,[0x000a85f4]
  000a8542: add r0,pc
  000a8544: ldr r3,[0x000a8604]
  000a8546: ldr r5,[r0,#0x0]
  000a8548: ldr r0,[r5,#0x0]
  000a854a: strd r2,r1,[sp,#0x0]
  000a854e: movw r1,#0x2601
  000a8552: movs r2,#0x0
  000a8554: blx 0x00071ff8
  000a8558: ldr r0,[r5,#0x0]
  000a855a: movs r1,#0x1
  000a855c: movs r2,#0x1
  000a855e: blx 0x00071fe0
  000a8562: ldr r0,[0x000a8618]
  000a8564: ldr r1,[r4,#0x6c]
  000a8566: add r0,pc
  000a8568: ldr r5,[r0,#0x0]
  000a856a: ldr r0,[r5,#0x0]
  000a856c: blx 0x0006fd9c
  000a8570: ldr r1,[r4,#0x6c]
  000a8572: ldr r2,[0x000a8608]
  000a8574: ldr r3,[0x000a85fc]
  000a8576: ldr r0,[r5,#0x0]
  000a8578: ldr r4,[0x000a860c]
  000a857a: str r4,[sp,#0x0]
  000a857c: blx 0x00072004
  000a8580: pop {r2,r3,r4,r5,r7,pc}
  000a8582: ldr r0,[0x000a861c]
  000a8584: add r0,pc
  000a8586: ldr r0,[r0,#0x0]
  000a8588: ldr r0,[r0,#0x0]
  000a858a: blx 0x00071a10
  000a858e: blx 0x00071950
  000a8592: cmp r0,#0x1
  000a8594: bne 0x000a85be
  000a8596: ldr r0,[0x000a8620]
  000a8598: mov.w r2,#0x3f800000
  000a859c: ldr r1,[0x000a85f4]
  000a859e: add r0,pc
  000a85a0: ldr r3,[0x000a85f0]
  000a85a2: ldr r5,[r0,#0x0]
  000a85a4: ldr r0,[r5,#0x0]
  000a85a6: strd r2,r1,[sp,#0x0]
  000a85aa: movw r1,#0x2601
  000a85ae: movs r2,#0x0
  000a85b0: blx 0x00071ff8
  000a85b4: ldr r0,[r5,#0x0]
  000a85b6: movs r1,#0x1
  000a85b8: movs r2,#0x1
  000a85ba: blx 0x00071fe0
  000a85be: ldr r0,[0x000a8624]
  000a85c0: ldr r1,[r4,#0x70]
  000a85c2: add r0,pc
  000a85c4: ldr r5,[r0,#0x0]
  000a85c6: ldr r0,[r5,#0x0]
  000a85c8: blx 0x0006fd9c
  000a85cc: ldr r0,[r5,#0x0]
  000a85ce: ldr r1,[r4,#0x70]
  000a85d0: ldr r2,[0x000a85f8]
  000a85d2: ldr r3,[0x000a85fc]
  000a85d4: ldr r5,[0x000a8600]
  000a85d6: str r5,[sp,#0x0]
  000a85d8: blx 0x00072004
  000a85dc: ldr r0,[r4,#0x0]
  000a85de: blx 0x00071eb4
  000a85e2: ldr r0,[r0,#0x4]
  000a85e4: ldr r0,[r0,#0x0]
  000a85e6: pop.w {r2,r3,r4,r5,r7,lr}
  000a85ea: b.w 0x001ab9a8
  000a85ee: pop {r2,r3,r4,r5,r7,pc}
  001ab9a8: bx pc
```
