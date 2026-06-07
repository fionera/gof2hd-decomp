# Achievements::initCheckEquipmentAndWeapons
elf 0x156da4 body 160
Sig: undefined __thiscall initCheckEquipmentAndWeapons(Achievements * this)

## decompile
```c

/* Achievements::initCheckEquipmentAndWeapons() */

void __thiscall Achievements::initCheckEquipmentAndWeapons(Achievements *this)

{
  Achievements AVar1;
  int iVar2;
  Ship *this_00;
  uint *puVar3;
  Item *this_01;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  iVar2 = Status::getCurrentCampaignMission();
  if (iVar2 < 8) {
    AVar1 = (Achievements)0x1;
  }
  else {
    this_00 = (Ship *)Status::getShip();
    puVar3 = (uint *)Ship::getEquipment(this_00);
    iVar2 = 0;
    iVar6 = 0;
    if (puVar3 != (uint *)0x0) {
      for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
        this_01 = *(Item **)(puVar3[1] + uVar5 * 4);
        if ((this_01 != (Item *)0x0) && (iVar4 = Item::getType(this_01), iVar4 != 4)) {
          iVar4 = Item::getType(*(Item **)(puVar3[1] + uVar5 * 4));
          if (iVar4 == 0) {
            *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
          }
          else {
            iVar4 = Item::getType(*(Item **)(puVar3[1] + uVar5 * 4));
            if (iVar4 == 3) {
              iVar6 = iVar6 + 1;
              goto LAB_00166e16;
            }
          }
          iVar2 = iVar2 + 1;
        }
LAB_00166e16:
      }
    }
    AVar1 = (Achievements)(0 < iVar6 && 0 < iVar2);
  }
  this[0x18] = AVar1;
  return;
}

```

## target disasm
```
  00166da4: push {r4,r5,r6,r7,lr}
  00166da6: add r7,sp,#0xc
  00166da8: push {r8,r9,r10}
  00166dac: mov r10,r0
  00166dae: ldr r0,[0x00166e44]
  00166db0: add r0,pc
  00166db2: ldr r4,[r0,#0x0]
  00166db4: ldr r0,[r4,#0x0]
  00166db6: blx 0x00071770
  00166dba: cmp r0,#0x8
  00166dbc: blt 0x00166e38
  00166dbe: ldr r0,[r4,#0x0]
  00166dc0: blx 0x00071a58
  00166dc4: blx 0x0007333c
  00166dc8: mov r5,r0
  00166dca: mov.w r8,#0x0
  00166dce: movs r6,#0x0
  00166dd0: mov.w r9,#0x0
  00166dd4: cbz r0,0x00166e1e
  00166dd6: movs r4,#0x0
  00166dd8: b 0x00166e18
  00166dda: ldr r0,[r5,#0x4]
  00166ddc: ldr.w r0,[r0,r4,lsl #0x2]
  00166de0: cbz r0,0x00166e16
  00166de2: blx 0x000718fc
  00166de6: cmp r0,#0x4
  00166de8: beq 0x00166e16
  00166dea: ldr r0,[r5,#0x4]
  00166dec: ldr.w r0,[r0,r4,lsl #0x2]
  00166df0: blx 0x000718fc
  00166df4: cbz r0,0x00166e0a
  00166df6: ldr r0,[r5,#0x4]
  00166df8: ldr.w r0,[r0,r4,lsl #0x2]
  00166dfc: blx 0x000718fc
  00166e00: cmp r0,#0x3
  00166e02: bne 0x00166e14
  00166e04: add.w r9,r9,#0x1
  00166e08: b 0x00166e16
  00166e0a: ldr.w r0,[r10,#0x14]
  00166e0e: adds r0,#0x1
  00166e10: str.w r0,[r10,#0x14]
  00166e14: adds r6,#0x1
  00166e16: adds r4,#0x1
  00166e18: ldr r0,[r5,#0x0]
  00166e1a: cmp r4,r0
  00166e1c: bcc 0x00166dda
  00166e1e: cmp.w r9,#0x0
  00166e22: mov.w r0,#0x0
  00166e26: it gt
  00166e28: mov.gt r0,#0x1
  00166e2a: cmp r6,#0x0
  00166e2c: it gt
  00166e2e: mov.gt.w r8,#0x1
  00166e32: and.w r0,r0,r8
  00166e36: b 0x00166e3a
  00166e38: movs r0,#0x1
  00166e3a: strb.w r0,[r10,#0x18]
  00166e3e: pop.w {r8,r9,r10}
  00166e42: pop {r4,r5,r6,r7,pc}
```
