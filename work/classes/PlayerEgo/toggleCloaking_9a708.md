# PlayerEgo::toggleCloaking
elf 0x9a708 body 652
Sig: undefined __stdcall toggleCloaking(void)

## decompile
```c

/* PlayerEgo::toggleCloaking() */

void PlayerEgo::toggleCloaking(void)

{
  int *in_r0;
  int iVar1;
  int iVar2;
  Mesh *pMVar3;
  undefined4 uVar4;
  Item *pIVar5;
  int iVar6;
  ushort uVar7;
  PaintCanvas *pPVar8;
  undefined4 uVar9;
  code *pcVar10;
  int *piVar11;
  code *pcVar12;
  undefined4 *puVar13;
  float in_s0;
  float extraout_s0;
  float extraout_s0_00;
  
  piVar11 = *(int **)(DAT_000aa994 + 0xaa718);
  iVar1 = *piVar11;
  if (*(char *)((int)in_r0 + 0x1ad) == '\0') {
    if (((char)in_r0[0x6b] == '\0') && (in_r0[0x83] < 1)) {
      iVar6 = Item::getAttribute(in_r0[0x6c]);
      iVar2 = Status::getShip();
      iVar2 = Ship::getCargo(iVar2);
      if (iVar2 == 0) {
        iVar2 = 0;
      }
      else {
        iVar2 = Item::getAmount();
      }
      if (iVar6 <= iVar2) {
        iVar6 = Status::getShip();
        Ship::removeCargo(iVar6,0x7a);
        *(undefined1 *)((int)in_r0 + 0x1ad) = 1;
        Hud::hudEvent(in_r0[0x88],(PlayerEgo *)0x1e,(int)in_r0);
        Hud::hudEvent(in_r0[0x88],(PlayerEgo *)0x1c,(int)in_r0);
      }
    }
  }
  else if (in_r0[0x85] <= in_r0[0x82]) {
    FModSound::play(**(int **)(DAT_000aa998 + 0xaa786),(Vector *)0x1e,(Vector *)0x0,in_s0);
    puVar13 = *(undefined4 **)(DAT_000aa99c + 0xaa798);
    *(undefined1 *)(*in_r0 + 0x5e) = 1;
    in_r0[0x82] = 0;
    *(undefined1 *)(in_r0 + 0x6b) = 1;
    iVar6 = AbyssEngine::PaintCanvas::MaterialGetMaterial((PaintCanvas *)*puVar13,in_r0[0xe2]);
    *(undefined4 *)(iVar6 + 0x20) = 0xe;
    AbyssEngine::PaintCanvas::MeshChangeMaterial
              ((PaintCanvas *)*puVar13,*(uint *)(in_r0[1] + 0x1c),*(ushort *)(in_r0 + 0xe2));
    pPVar8 = (PaintCanvas *)*puVar13;
    pMVar3 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer(pPVar8,*(uint *)(in_r0[1] + 0x1c));
    AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar8,pMVar3,extraout_s0,0);
    pPVar8 = (PaintCanvas *)*puVar13;
    pMVar3 = (Mesh *)AbyssEngine::PaintCanvas::MeshGetPointer(pPVar8,*(uint *)(in_r0[1] + 0x1c));
    AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(pPVar8,pMVar3,extraout_s0_00,0);
    if ((char)in_r0[0x5c] != '\0') {
      iVar6 = AbyssEngine::PaintCanvas::MaterialGetMaterial((PaintCanvas *)*puVar13,in_r0[0xe3]);
      *(undefined4 *)(iVar6 + 0x20) = 0xe;
      iVar6 = AbyssEngine::PaintCanvas::MaterialGetMaterial((PaintCanvas *)*puVar13,in_r0[0xe4]);
      *(undefined4 *)(iVar6 + 0x20) = 0xe;
      AbyssEngine::PaintCanvas::MeshChangeMaterial
                ((PaintCanvas *)*puVar13,*(uint *)(in_r0[0x37] + 0x1c),*(ushort *)(in_r0 + 0xe3));
      AbyssEngine::PaintCanvas::MeshChangeMaterial
                ((PaintCanvas *)*puVar13,*(uint *)(in_r0[10] + 0x1c),*(ushort *)(in_r0 + 0xe4));
      uVar9 = *puVar13;
      pcVar10 = *(code **)(DAT_000aa9a0 + 0xaa85c);
      uVar4 = (*pcVar10)(uVar9,*(undefined4 *)(in_r0[0x37] + 0x1c));
      pcVar12 = *(code **)(DAT_000aa9a4 + 0xaa872);
      (*pcVar12)(uVar9,uVar4,0,1);
      uVar9 = *puVar13;
      uVar4 = (*pcVar10)(uVar9,*(undefined4 *)(in_r0[0x37] + 0x1c));
      (*pcVar12)(uVar9,uVar4,0,2);
      uVar9 = *puVar13;
      uVar4 = (*pcVar10)(uVar9,*(undefined4 *)(in_r0[10] + 0x1c));
      (*pcVar12)(uVar9,uVar4,0,1);
      uVar9 = *puVar13;
      uVar4 = (*pcVar10)(uVar9,*(undefined4 *)(in_r0[10] + 0x1c));
      (*pcVar12)(uVar9,uVar4,0,2);
      if ((char)in_r0[0x5c] != '\0') {
        iVar6 = Status::getShip();
        pIVar5 = (Item *)Ship::getFirstEquipmentOfSort(iVar6);
        uVar7 = 0x4e8e;
        if ((pIVar5 != (Item *)0x0) && (iVar6 = Item::getIndex(pIVar5), iVar6 == 0xe0)) {
          uVar7 = 0x5e17;
        }
        iVar6 = Status::getShip();
        pIVar5 = (Item *)Ship::getFirstEquipmentOfSort(iVar6);
        if (pIVar5 != (Item *)0x0) {
          iVar6 = Item::getIndex(pIVar5);
          uVar7 = 0x716d;
          if (iVar6 == 199) {
            uVar7 = 0x7167;
          }
          if (iVar6 == 0xc6) {
            uVar7 = 0x7161;
          }
        }
        AbyssEngine::PaintCanvas::MaterialCreate
                  ((PaintCanvas *)*puVar13,uVar7,(uint *)&stack0xffffffd4);
        AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
                  ((PaintCanvas *)*puVar13,*(uint *)(in_r0[0x37] + 0x1c),uVar7);
        AbyssEngine::PaintCanvas::MeshChangeResourceMaterial
                  ((PaintCanvas *)*puVar13,*(uint *)(in_r0[10] + 0x1c),uVar7);
      }
    }
  }
  iVar6 = *piVar11;
  iVar1 = iVar6 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar6);
  }
  return;
}

```

## target disasm
```
  000aa708: push {r4,r5,r6,r7,lr}
  000aa70a: add r7,sp,#0xc
  000aa70c: push {r5,r6,r7,r8,r9,r10,r11}
  000aa710: mov r4,r0
  000aa712: ldr r0,[0x000aa994]
  000aa714: add r0,pc
  000aa716: ldr.w r8,[r0,#0x0]
  000aa71a: ldr.w r0,[r8,#0x0]
  000aa71e: str r0,[sp,#0x8]
  000aa720: ldrb.w r0,[r4,#0x1ad]
  000aa724: cbz r0,0x000aa738
  000aa726: ldr.w r0,[r4,#0x208]
  000aa72a: ldr.w r1,[r4,#0x214]
  000aa72e: cmp r0,r1
  000aa730: bge 0x000aa77a
  000aa732: add.w r9,r4,#0x1ac
  000aa736: b 0x000aa938
  000aa738: ldrb.w r0,[r4,#0x1ac]
  000aa73c: add.w r9,r4,#0x1ac
  000aa740: cmp r0,#0x0
  000aa742: bne.w 0x000aa938
  000aa746: ldr.w r0,[r4,#0x20c]
  000aa74a: cmp r0,#0x0
  000aa74c: bgt.w 0x000aa938
  000aa750: ldr.w r0,[r4,#0x1b0]
  000aa754: movs r1,#0x26
  000aa756: blx 0x000718e4
  000aa75a: mov r5,r0
  000aa75c: ldr r0,[0x000aa9ac]
  000aa75e: add r0,pc
  000aa760: ldr r6,[r0,#0x0]
  000aa762: ldr r0,[r6,#0x0]
  000aa764: blx 0x00071a58
  000aa768: movs r1,#0x7a
  000aa76a: blx 0x00072424
  000aa76e: cmp r0,#0x0
  000aa770: beq.w 0x000aa958
  000aa774: blx 0x0007183c
  000aa778: b 0x000aa95a
  000aa77a: ldr r0,[0x000aa998]
  000aa77c: movs r5,#0x0
  000aa77e: movs r1,#0x1e
  000aa780: movs r2,#0x0
  000aa782: add r0,pc
  000aa784: movs r3,#0x0
  000aa786: str r5,[sp,#0x0]
  000aa788: ldr r0,[r0,#0x0]
  000aa78a: ldr r0,[r0,#0x0]
  000aa78c: blx 0x00071548
  000aa790: ldr r0,[0x000aa99c]
  000aa792: ldr r1,[r4,#0x0]
  000aa794: add r0,pc
  000aa796: ldr.w r11,[r0,#0x0]
  000aa79a: movs r0,#0x1
  000aa79c: strb.w r0,[r1,#0x5e]
  000aa7a0: str.w r5,[r4,#0x208]
  000aa7a4: strb.w r0,[r4,#0x1ac]
  000aa7a8: str r5,[sp,#0x4]
  000aa7aa: ldr.w r0,[r11,#0x0]
  000aa7ae: ldr.w r1,[r4,#0x388]
  000aa7b2: blx 0x00072430
  000aa7b6: mov.w r10,#0xe
  000aa7ba: str.w r10,[r0,#0x20]
  000aa7be: ldr r1,[r4,#0x4]
  000aa7c0: ldr.w r0,[r11,#0x0]
  000aa7c4: ldrh.w r2,[r4,#0x388]
  000aa7c8: ldr r1,[r1,#0x1c]
  000aa7ca: blx 0x0007243c
  000aa7ce: ldr r0,[r4,#0x4]
  000aa7d0: ldr.w r5,[r11,#0x0]
  000aa7d4: ldr r1,[r0,#0x1c]
  000aa7d6: mov r0,r5
  000aa7d8: blx 0x00072370
  000aa7dc: mov r1,r0
  000aa7de: mov r0,r5
  000aa7e0: movs r2,#0x0
  000aa7e2: movs r3,#0x1
  000aa7e4: blx 0x0006fc58
  000aa7e8: ldr r0,[r4,#0x4]
  000aa7ea: ldr.w r5,[r11,#0x0]
  000aa7ee: ldr r1,[r0,#0x1c]
  000aa7f0: mov r0,r5
  000aa7f2: blx 0x00072370
  000aa7f6: mov r1,r0
  000aa7f8: mov r0,r5
  000aa7fa: movs r2,#0x0
  000aa7fc: movs r3,#0x2
  000aa7fe: blx 0x0006fc58
  000aa802: ldrb.w r0,[r4,#0x170]
  000aa806: add.w r9,r4,#0x1ac
  000aa80a: cmp r0,#0x0
  000aa80c: beq.w 0x000aa938
  000aa810: ldr.w r1,[r4,#0x38c]
  000aa814: ldr.w r0,[r11,#0x0]
  000aa818: blx 0x00072430
  000aa81c: str.w r10,[r0,#0x20]
  000aa820: ldr.w r0,[r11,#0x0]
  000aa824: ldr.w r1,[r4,#0x390]
  000aa828: blx 0x00072430
  000aa82c: str.w r10,[r0,#0x20]
  000aa830: ldr.w r1,[r4,#0xdc]
  000aa834: ldr.w r0,[r11,#0x0]
  000aa838: ldrh.w r2,[r4,#0x38c]
  000aa83c: ldr r1,[r1,#0x1c]
  000aa83e: blx 0x0007243c
  000aa842: ldr r1,[r4,#0x28]
  000aa844: ldrh.w r2,[r4,#0x390]
  000aa848: ldr.w r0,[r11,#0x0]
  000aa84c: ldr r1,[r1,#0x1c]
  000aa84e: blx 0x0007243c
  000aa852: ldr r0,[0x000aa9a0]
  000aa854: ldr.w r1,[r4,#0xdc]
  000aa858: add r0,pc
  000aa85a: ldr.w r5,[r11,#0x0]
  000aa85e: ldr r6,[r0,#0x0]
  000aa860: ldr r1,[r1,#0x1c]
  000aa862: mov r0,r5
  000aa864: blx r6
  000aa866: mov r1,r0
  000aa868: ldr r0,[0x000aa9a4]
  000aa86a: movs r2,#0x0
  000aa86c: movs r3,#0x1
  000aa86e: add r0,pc
  000aa870: ldr.w r10,[r0,#0x0]
  000aa874: mov r0,r5
  000aa876: blx r10
  000aa878: ldr.w r0,[r4,#0xdc]
  000aa87c: ldr.w r5,[r11,#0x0]
  000aa880: ldr r1,[r0,#0x1c]
  000aa882: mov r0,r5
  000aa884: blx r6
  000aa886: mov r1,r0
  000aa888: mov r0,r5
  000aa88a: movs r2,#0x0
  000aa88c: movs r3,#0x2
  000aa88e: blx r10
  000aa890: ldr r0,[r4,#0x28]
  000aa892: ldr.w r5,[r11,#0x0]
  000aa896: ldr r1,[r0,#0x1c]
  000aa898: mov r0,r5
  000aa89a: blx r6
  000aa89c: mov r1,r0
  000aa89e: mov r0,r5
  000aa8a0: movs r2,#0x0
  000aa8a2: movs r3,#0x1
  000aa8a4: blx r10
  000aa8a6: ldr r0,[r4,#0x28]
  000aa8a8: ldr.w r5,[r11,#0x0]
  000aa8ac: ldr r1,[r0,#0x1c]
  000aa8ae: mov r0,r5
  000aa8b0: blx r6
  000aa8b2: mov r1,r0
  000aa8b4: mov r0,r5
  000aa8b6: movs r2,#0x0
  000aa8b8: movs r3,#0x2
  000aa8ba: blx r10
  000aa8bc: ldrb.w r0,[r4,#0x170]
  000aa8c0: cbz r0,0x000aa938
  000aa8c2: ldr r0,[0x000aa9a8]
  000aa8c4: add r0,pc
  000aa8c6: ldr r6,[r0,#0x0]
  000aa8c8: ldr r0,[r6,#0x0]
  000aa8ca: blx 0x00071a58
  000aa8ce: movs r1,#0x8
  000aa8d0: blx 0x0007228c
  000aa8d4: movw r5,#0x4e8e
  000aa8d8: cbz r0,0x000aa8e6
  000aa8da: blx 0x000718d8
  000aa8de: cmp r0,#0xe0
  000aa8e0: it eq
  000aa8e2: movw.eq r5,#0x5e17
  000aa8e6: ldr r0,[r6,#0x0]
  000aa8e8: blx 0x00071a58
  000aa8ec: movs r1,#0x23
  000aa8ee: blx 0x0007228c
  000aa8f2: cbz r0,0x000aa90c
  000aa8f4: blx 0x000718d8
  000aa8f8: movw r5,#0x716d
  000aa8fc: cmp r0,#0xc7
  000aa8fe: it eq
  000aa900: movw.eq r5,#0x7167
  000aa904: cmp r0,#0xc6
  000aa906: it eq
  000aa908: movw.eq r5,#0x7161
  000aa90c: ldr.w r0,[r11,#0x0]
  000aa910: uxth r5,r5
  000aa912: add r2,sp,#0x4
  000aa914: mov r1,r5
  000aa916: blx 0x0006fc1c
  000aa91a: ldr.w r1,[r4,#0xdc]
  000aa91e: mov r2,r5
  000aa920: ldr.w r0,[r11,#0x0]
  000aa924: ldr r1,[r1,#0x1c]
  000aa926: blx 0x00072448
  000aa92a: ldr r1,[r4,#0x28]
  000aa92c: mov r2,r5
  000aa92e: ldr.w r0,[r11,#0x0]
  000aa932: ldr r1,[r1,#0x1c]
  000aa934: blx 0x00072448
  000aa938: ldrb.w r6,[r9,#0x0]
  000aa93c: cmp r6,#0x0
  000aa93e: it ne
  000aa940: mov.ne r6,#0x1
  000aa942: ldr r0,[sp,#0x8]
  000aa944: ldr.w r1,[r8,#0x0]
  000aa948: subs r0,r1,r0
  000aa94a: ittt eq
  000aa94c: mov.eq r0,r6
  000aa94e: pop.eq.w {r1,r2,r3,r8,r9,r10,r11}
  000aa952: pop.eq {r4,r5,r6,r7,pc}
  000aa954: blx 0x0006e824
  000aa958: movs r0,#0x0
  000aa95a: cmp r0,r5
  000aa95c: bge 0x000aa962
  000aa95e: movs r6,#0x0
  000aa960: b 0x000aa942
  000aa962: ldr r0,[r6,#0x0]
  000aa964: blx 0x00071a58
  000aa968: movs r1,#0x7a
  000aa96a: mov r2,r5
  000aa96c: blx 0x00072454
  000aa970: ldr.w r0,[r4,#0x220]
  000aa974: movs r6,#0x1
  000aa976: movs r1,#0x1e
  000aa978: mov r2,r4
  000aa97a: mov r3,r5
  000aa97c: strb.w r6,[r4,#0x1ad]
  000aa980: blx 0x00072418
  000aa984: ldr.w r0,[r4,#0x220]
  000aa988: movs r1,#0x1c
  000aa98a: mov r2,r4
  000aa98c: movs r3,#0x0
  000aa98e: blx 0x00072418
  000aa992: b 0x000aa942
```
