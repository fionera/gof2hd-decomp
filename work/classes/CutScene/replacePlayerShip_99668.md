# CutScene::replacePlayerShip
elf 0x99668 body 268
Sig: undefined __stdcall replacePlayerShip(int param_1, int param_2)

## decompile
```c

/* CutScene::replacePlayerShip(int, int) */

void CutScene::replacePlayerShip(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  void *pvVar3;
  int in_r2;
  code *pcVar4;
  PaintCanvas *this;
  int *piVar5;
  AEGeometry *this_00;
  uint in_fpscr;
  undefined1 local_68 [64];
  int local_28;
  
  piVar5 = *(int **)(DAT_000a9774 + 0xa967e);
  local_28 = *piVar5;
  iVar1 = Level::getEnemies();
  if (iVar1 != 0) {
    iVar1 = Level::getEnemies();
    this_00 = *(AEGeometry **)(**(int **)(iVar1 + 4) + 8);
    if (this_00 != (AEGeometry *)0x0) {
      if (*(int *)(param_1 + 100) != 0) {
        this = (PaintCanvas *)**(undefined4 **)(DAT_000a9778 + 0xa96ae);
        iVar1 = Level::getEnemies();
        AbyssEngine::PaintCanvas::TransformRemoveChild
                  (this,*(uint *)(*(int *)(**(int **)(iVar1 + 4) + 8) + 0xc),
                   *(uint *)(*(int *)(param_1 + 100) + 0xc));
      }
      uVar2 = AEGeometry::getMatrix();
      __aeabi_memcpy(local_68,uVar2,0x3c);
      uVar2 = Globals::getShipGroup
                        ((Globals *)**(undefined4 **)(DAT_000a977c + 0xa96e8),param_2,in_r2,false);
      pcVar4 = *(code **)(DAT_000a9780 + 0xa96f6);
      iVar1 = (*pcVar4)(*(undefined4 *)param_1);
      *(undefined4 *)(**(int **)(iVar1 + 4) + 8) = uVar2;
      iVar1 = (*pcVar4)(*(undefined4 *)param_1);
      AEGeometry::setMatrix(*(Matrix **)(**(int **)(iVar1 + 4) + 8));
      iVar1 = (*pcVar4)(*(undefined4 *)param_1);
      uVar2 = VectorSignedToFloat(*(undefined4 *)(DAT_000a9784 + 0xa971c + param_2 * 4),
                                  (byte)(in_fpscr >> 0x16) & 3);
      (**(code **)(*(int *)**(undefined4 **)(iVar1 + 4) + 0x48))
                ((int *)**(undefined4 **)(iVar1 + 4),0,uVar2,0);
      iVar1 = (*pcVar4)(*(undefined4 *)param_1);
      PlayerFighter::setExhaustVisible(SUB41(**(undefined4 **)(iVar1 + 4),0));
      LODManager::removeObject((LODManager *)**(undefined4 **)param_1,this_00);
      pvVar3 = (void *)AEGeometry::~AEGeometry(this_00);
      operator_delete(pvVar3);
    }
    checkForTurret((CutScene *)param_1);
  }
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a9668: push {r4,r5,r6,r7,lr}
  000a966a: add r7,sp,#0xc
  000a966c: push {r8,r9,r10,r11}
  000a9670: sub sp,#0x44
  000a9672: mov r4,r0
  000a9674: ldr r0,[0x000a9774]
  000a9676: mov r6,r2
  000a9678: mov r8,r1
  000a967a: add r0,pc
  000a967c: ldr.w r10,[r0,#0x0]
  000a9680: ldr.w r0,[r10,#0x0]
  000a9684: str r0,[sp,#0x40]
  000a9686: ldr r0,[r4,#0x0]
  000a9688: blx 0x00071eb4
  000a968c: cmp r0,#0x0
  000a968e: beq 0x000a975e
  000a9690: ldr r0,[r4,#0x0]
  000a9692: blx 0x00071eb4
  000a9696: ldr r0,[r0,#0x4]
  000a9698: ldr r0,[r0,#0x0]
  000a969a: ldr.w r11,[r0,#0x8]
  000a969e: cmp.w r11,#0x0
  000a96a2: beq 0x000a9758
  000a96a4: ldr r0,[r4,#0x64]
  000a96a6: cbz r0,0x000a96ca
  000a96a8: ldr r0,[0x000a9778]
  000a96aa: add r0,pc
  000a96ac: ldr r1,[r0,#0x0]
  000a96ae: ldr r0,[r4,#0x0]
  000a96b0: ldr.w r9,[r1,#0x0]
  000a96b4: blx 0x00071eb4
  000a96b8: ldr r0,[r0,#0x4]
  000a96ba: ldr r1,[r4,#0x64]
  000a96bc: ldr r0,[r0,#0x0]
  000a96be: ldr r2,[r1,#0xc]
  000a96c0: ldr r0,[r0,#0x8]
  000a96c2: ldr r1,[r0,#0xc]
  000a96c4: mov r0,r9
  000a96c6: blx 0x0007210c
  000a96ca: mov r0,r11
  000a96cc: blx 0x000721cc
  000a96d0: mov r9,sp
  000a96d2: mov r1,r0
  000a96d4: mov r0,r9
  000a96d6: movs r2,#0x3c
  000a96d8: blx 0x0006f1e4
  000a96dc: ldr r0,[0x000a977c]
  000a96de: mov r1,r8
  000a96e0: mov r2,r6
  000a96e2: movs r3,#0x0
  000a96e4: add r0,pc
  000a96e6: ldr r0,[r0,#0x0]
  000a96e8: ldr r0,[r0,#0x0]
  000a96ea: blx 0x000721d8
  000a96ee: mov r6,r0
  000a96f0: ldr r0,[0x000a9780]
  000a96f2: add r0,pc
  000a96f4: ldr r5,[r0,#0x0]
  000a96f6: ldr r0,[r4,#0x0]
  000a96f8: blx r5
  000a96fa: ldr r0,[r0,#0x4]
  000a96fc: ldr r0,[r0,#0x0]
  000a96fe: str r6,[r0,#0x8]
  000a9700: ldr r0,[r4,#0x0]
  000a9702: blx r5
  000a9704: ldr r0,[r0,#0x4]
  000a9706: mov r1,r9
  000a9708: ldr r0,[r0,#0x0]
  000a970a: ldr r0,[r0,#0x8]
  000a970c: blx 0x000721e4
  000a9710: ldr r0,[r4,#0x0]
  000a9712: blx r5
  000a9714: ldr r1,[0x000a9784]
  000a9716: movs r3,#0x0
  000a9718: add r1,pc
  000a971a: add.w r1,r1,r8, lsl #0x2
  000a971e: vldr.32 s0,[r1]
  000a9722: vcvt.f32.s32 s0,s0
  000a9726: ldr r0,[r0,#0x4]
  000a9728: ldr r0,[r0,#0x0]
  000a972a: ldr r1,[r0,#0x0]
  000a972c: vmov r2,s0
  000a9730: ldr r6,[r1,#0x48]
  000a9732: movs r1,#0x0
  000a9734: blx r6
  000a9736: ldr r0,[r4,#0x0]
  000a9738: blx r5
  000a973a: ldr r0,[r0,#0x4]
  000a973c: movs r1,#0x0
  000a973e: ldr r0,[r0,#0x0]
  000a9740: blx 0x000721f0
  000a9744: ldr r0,[r4,#0x0]
  000a9746: mov r1,r11
  000a9748: ldr r0,[r0,#0x0]
  000a974a: blx 0x000721fc
  000a974e: mov r0,r11
  000a9750: blx 0x00071fc8
  000a9754: blx 0x0006eb48
  000a9758: mov r0,r4
  000a975a: blx 0x000720dc
  000a975e: ldr r0,[sp,#0x40]
  000a9760: ldr.w r1,[r10,#0x0]
  000a9764: subs r0,r1,r0
  000a9766: ittt eq
  000a9768: add.eq sp,#0x44
  000a976a: pop.eq.w {r8,r9,r10,r11}
  000a976e: pop.eq {r4,r5,r6,r7,pc}
  000a9770: blx 0x0006e824
```
