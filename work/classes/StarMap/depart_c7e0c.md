# StarMap::depart
elf 0xc7e0c body 508
Sig: undefined __thiscall depart(StarMap * this, bool param_1)

## decompile
```c

/* StarMap::depart(bool) */

void __thiscall StarMap::depart(StarMap *this,bool param_1)

{
  uint uVar1;
  int iVar2;
  Ship *this_00;
  Station *pSVar3;
  Array<Station*> *this_01;
  void *pvVar4;
  StarMap *pSVar5;
  Station *this_02;
  int iVar6;
  undefined4 *puVar7;
  
  if (*(int *)(this + 100) < 0) {
    return;
  }
  puVar7 = *(undefined4 **)(DAT_000d8008 + 0xd7e28);
  if (this[0xaa] != (StarMap)0x0) {
    Status::departStation((Station *)*puVar7);
    **(undefined4 **)(DAT_000d800c + 0xd7e4a) = 0;
    Level::setInitStreamOut();
    uVar1 = Status::jumpgateUsed((Status *)*puVar7);
    if (param_1) {
      uVar1 = (uint)(byte)this[0xab];
    }
    if (param_1 && uVar1 != 0) {
      iVar6 = Station::getSystem(*(Station **)
                                  (*(int *)(*(int *)(this + 0x58) + 4) + *(int *)(this + 100) * 4));
      Status::getSystem();
      iVar2 = SolarSystem::getIndex();
      *(bool *)*(undefined4 *)(DAT_000d8014 + 0xd7eb0) = iVar6 != iVar2;
      if (iVar6 != iVar2) {
        **(undefined4 **)(DAT_000d8018 + 0xd7ebe) = *(undefined4 *)(this + 0x1d0);
      }
    }
    else {
      **(undefined1 **)(DAT_000d8010 + 0xd7e6c) = 0;
    }
    goto LAB_000d7f42;
  }
  iVar6 = Status::getCurrentCampaignMission();
  if (iVar6 == 3) goto LAB_000d7f42;
  pSVar3 = (Station *)*puVar7;
  *(undefined4 *)(pSVar3 + 0x5c) = 0xffffffff;
  *(undefined4 *)(pSVar3 + 0x60) = 0xffffffff;
  *(undefined4 *)(pSVar3 + 100) = 0xffffffff;
  *(undefined4 *)(pSVar3 + 0x68) = 0xffffffff;
  Status::getStation();
  Status::departStation(pSVar3);
  this_02 = *(Station **)(*(int *)(*(int *)(this + 0x58) + 4) + *(int *)(this + 100) * 4);
  pSVar3 = (Station *)Status::getStation();
  iVar6 = Station::equals(this_02,pSVar3);
  if (iVar6 == 0) {
    **(undefined4 **)(DAT_000d801c + 0xd7f10) =
         *(undefined4 *)(*(int *)(*(int *)(this + 0x58) + 4) + *(int *)(this + 100) * 4);
  }
  if (param_1) {
    this_00 = (Ship *)Status::getShip();
    iVar6 = Ship::hasVolatileGoods(this_00);
    if (iVar6 != 0) goto LAB_000d7f2c;
    Status::getShip();
    iVar6 = Ship::hasJumpDriveIntegrated();
    if ((iVar6 == 0) && (this[0xab] == (StarMap)0x0)) goto LAB_000d7f2c;
    iVar6 = Station::getSystem((Station *)**(undefined4 **)(DAT_000d8024 + 0xd7fd6));
    Status::getSystem();
    iVar2 = SolarSystem::getIndex();
    *(bool *)*(undefined4 *)(DAT_000d8028 + 0xd7ff6) = iVar6 != iVar2;
    if (iVar6 != iVar2) {
      **(undefined4 **)(DAT_000d802c + 0xd8004) = *(undefined4 *)(this + 0x1d0);
    }
  }
  else {
LAB_000d7f2c:
    **(undefined1 **)(DAT_000d8020 + 0xd7f34) = 0;
  }
  Achievements::resetNewMedals((Achievements *)**(undefined4 **)(DAT_000d8030 + 0xd7f3c));
LAB_000d7f42:
  pSVar5 = this + 0x58;
  iVar6 = 0;
  for (uVar1 = 0; this_01 = *(Array<Station*> **)pSVar5, uVar1 < *(uint *)this_01; uVar1 = uVar1 + 1
      ) {
    if (uVar1 != *(uint *)(this + 100)) {
      pSVar3 = *(Station **)(*(int *)(this_01 + 4) + iVar6);
      if (pSVar3 == (Station *)0x0) {
        puVar7 = (undefined4 *)(*(int *)(this_01 + 4) + uVar1 * 4);
      }
      else {
        pvVar4 = (void *)Station::~Station(pSVar3);
        operator_delete(pvVar4);
        puVar7 = (undefined4 *)(*(int *)(*(int *)pSVar5 + 4) + iVar6);
      }
      *puVar7 = 0;
    }
    iVar6 = iVar6 + 4;
  }
  if (this_01 != (Array<Station*> *)0x0) {
    pvVar4 = (void *)Array<Station*>::~Array(this_01);
    operator_delete(pvVar4);
  }
  iVar6 = DAT_000d8034;
  *(undefined4 *)pSVar5 = 0;
  FModSound::stop(**(int **)(iVar6 + 0xd7f94));
  puVar7 = *(undefined4 **)(DAT_000d803c + 0xd7fa6);
  **(undefined4 **)(DAT_000d8038 + 0xd7fa4) = 1;
  (*(code *)(DAT_001ab904 + 0x1ab908))(*puVar7,2);
  return;
}

```

## target disasm
```
  000d7e0c: push {r4,r5,r6,r7,lr}
  000d7e0e: add r7,sp,#0xc
  000d7e10: push {r8,r9,r10}
  000d7e14: mov r8,r1
  000d7e16: ldr r1,[r0,#0x64]
  000d7e18: mov r10,r0
  000d7e1a: cmp r1,#0x0
  000d7e1c: blt 0x000d7e70
  000d7e1e: ldr r0,[0x000d8008]
  000d7e20: ldrb.w r2,[r10,#0xaa]
  000d7e24: add r0,pc
  000d7e26: cmp r2,#0x0
  000d7e28: ldr.w r9,[r0,#0x0]
  000d7e2c: ldr.w r0,[r9,#0x0]
  000d7e30: beq 0x000d7e76
  000d7e32: mov r4,r10
  000d7e34: ldr.w r2,[r4,#0x58]!
  000d7e38: ldr r2,[r2,#0x4]
  000d7e3a: ldr.w r1,[r2,r1,lsl #0x2]
  000d7e3e: blx 0x000745fc
  000d7e42: ldr r0,[0x000d800c]
  000d7e44: movs r5,#0x0
  000d7e46: add r0,pc
  000d7e48: ldr r0,[r0,#0x0]
  000d7e4a: str r5,[r0,#0x0]
  000d7e4c: blx 0x00074608
  000d7e50: ldr.w r0,[r9,#0x0]
  000d7e54: blx 0x00074614
  000d7e58: cmp.w r8,#0x0
  000d7e5c: itt ne
  000d7e5e: ldrb.ne.w r0,[r10,#0xab]
  000d7e62: cmp.ne r0,#0x0
  000d7e64: bne 0x000d7e84
  000d7e66: ldr r0,[0x000d8010]
  000d7e68: add r0,pc
  000d7e6a: ldr r0,[r0,#0x0]
  000d7e6c: strb r5,[r0,#0x0]
  000d7e6e: b 0x000d7f42
  000d7e70: pop.w {r8,r9,r10}
  000d7e74: pop {r4,r5,r6,r7,pc}
  000d7e76: blx 0x00071770
  000d7e7a: cmp r0,#0x3
  000d7e7c: bne 0x000d7ec2
  000d7e7e: add.w r4,r10,#0x58
  000d7e82: b 0x000d7f42
  000d7e84: ldr.w r0,[r10,#0x58]
  000d7e88: ldr.w r1,[r10,#0x64]
  000d7e8c: ldr r0,[r0,#0x4]
  000d7e8e: ldr.w r0,[r0,r1,lsl #0x2]
  000d7e92: blx 0x0007189c
  000d7e96: mov r6,r0
  000d7e98: ldr.w r0,[r9,#0x0]
  000d7e9c: blx 0x00071a10
  000d7ea0: blx 0x00071a7c
  000d7ea4: ldr r1,[0x000d8014]
  000d7ea6: subs r0,r6,r0
  000d7ea8: it ne
  000d7eaa: mov.ne r0,#0x1
  000d7eac: add r1,pc
  000d7eae: ldr r1,[r1,#0x0]
  000d7eb0: strb r0,[r1,#0x0]
  000d7eb2: beq 0x000d7f42
  000d7eb4: ldr r0,[0x000d8018]
  000d7eb6: ldr.w r1,[r10,#0x1d0]
  000d7eba: add r0,pc
  000d7ebc: ldr r0,[r0,#0x0]
  000d7ebe: str r1,[r0,#0x0]
  000d7ec0: b 0x000d7f42
  000d7ec2: ldr.w r6,[r9,#0x0]
  000d7ec6: mov.w r0,#0xffffffff
  000d7eca: strd r0,r0,[r6,#0x5c]
  000d7ece: strd r0,r0,[r6,#0x64]
  000d7ed2: mov r0,r6
  000d7ed4: blx 0x00071c14
  000d7ed8: mov r1,r0
  000d7eda: mov r0,r6
  000d7edc: blx 0x000745fc
  000d7ee0: mov r4,r10
  000d7ee2: ldr.w r0,[r4,#0x58]!
  000d7ee6: ldr r1,[r4,#0xc]
  000d7ee8: ldr r2,[r0,#0x4]
  000d7eea: ldr.w r0,[r9,#0x0]
  000d7eee: ldr.w r6,[r2,r1,lsl #0x2]
  000d7ef2: blx 0x00071c14
  000d7ef6: mov r1,r0
  000d7ef8: mov r0,r6
  000d7efa: blx 0x00073678
  000d7efe: cbnz r0,0x000d7f16
  000d7f00: ldr.w r0,[r10,#0x58]
  000d7f04: ldr.w r1,[r10,#0x64]
  000d7f08: ldr r2,[0x000d801c]
  000d7f0a: ldr r0,[r0,#0x4]
  000d7f0c: add r2,pc
  000d7f0e: ldr r2,[r2,#0x0]
  000d7f10: ldr.w r0,[r0,r1,lsl #0x2]
  000d7f14: str r0,[r2,#0x0]
  000d7f16: cmp.w r8,#0x0
  000d7f1a: beq 0x000d7f2c
  000d7f1c: ldr.w r0,[r9,#0x0]
  000d7f20: blx 0x00071a58
  000d7f24: blx 0x000722e0
  000d7f28: cmp r0,#0x0
  000d7f2a: beq 0x000d7fba
  000d7f2c: ldr r0,[0x000d8020]
  000d7f2e: movs r1,#0x0
  000d7f30: add r0,pc
  000d7f32: ldr r0,[r0,#0x0]
  000d7f34: strb r1,[r0,#0x0]
  000d7f36: ldr r0,[0x000d8030]
  000d7f38: add r0,pc
  000d7f3a: ldr r0,[r0,#0x0]
  000d7f3c: ldr r0,[r0,#0x0]
  000d7f3e: blx 0x00074620
  000d7f42: mov.w r8,#0x0
  000d7f46: movs r6,#0x0
  000d7f48: movs r5,#0x0
  000d7f4a: b 0x000d7f76
  000d7f4c: ldr.w r1,[r10,#0x64]
  000d7f50: cmp r5,r1
  000d7f52: beq 0x000d7f72
  000d7f54: ldr r1,[r0,#0x4]
  000d7f56: ldr r0,[r1,r6]
  000d7f58: cbz r0,0x000d7f6a
  000d7f5a: blx 0x0007360c
  000d7f5e: blx 0x0006eb48
  000d7f62: ldr r0,[r4,#0x0]
  000d7f64: ldr r0,[r0,#0x4]
  000d7f66: add r0,r6
  000d7f68: b 0x000d7f6e
  000d7f6a: add.w r0,r1,r5, lsl #0x2
  000d7f6e: str.w r8,[r0,#0x0]
  000d7f72: adds r6,#0x4
  000d7f74: adds r5,#0x1
  000d7f76: ldr r0,[r4,#0x0]
  000d7f78: ldr r1,[r0,#0x0]
  000d7f7a: cmp r5,r1
  000d7f7c: bcc 0x000d7f4c
  000d7f7e: cbz r0,0x000d7f88
  000d7f80: blx 0x000735e8
  000d7f84: blx 0x0006eb48
  000d7f88: ldr r0,[0x000d8034]
  000d7f8a: movs r1,#0x0
  000d7f8c: str r1,[r4,#0x0]
  000d7f8e: movs r1,#0x66
  000d7f90: add r0,pc
  000d7f92: ldr r0,[r0,#0x0]
  000d7f94: ldr r0,[r0,#0x0]
  000d7f96: blx 0x000724a8
  000d7f9a: ldr r0,[0x000d8038]
  000d7f9c: movs r2,#0x1
  000d7f9e: ldr r1,[0x000d803c]
  000d7fa0: add r0,pc
  000d7fa2: add r1,pc
  000d7fa4: ldr r0,[r0,#0x0]
  000d7fa6: ldr r1,[r1,#0x0]
  000d7fa8: str r2,[r0,#0x0]
  000d7faa: ldr r0,[r1,#0x0]
  000d7fac: movs r1,#0x2
  000d7fae: pop.w {r8,r9,r10}
  000d7fb2: pop.w {r4,r5,r6,r7,lr}
  000d7fb6: b.w 0x001ab8f8
  000d7fba: ldr.w r0,[r9,#0x0]
  000d7fbe: blx 0x00071a58
  000d7fc2: blx 0x000719bc
  000d7fc6: cbnz r0,0x000d7fd0
  000d7fc8: ldrb.w r0,[r10,#0xab]
  000d7fcc: cmp r0,#0x0
  000d7fce: beq 0x000d7f2c
  000d7fd0: ldr r0,[0x000d8024]
  000d7fd2: add r0,pc
  000d7fd4: ldr r0,[r0,#0x0]
  000d7fd6: ldr r0,[r0,#0x0]
  000d7fd8: blx 0x0007189c
  000d7fdc: mov r6,r0
  000d7fde: ldr.w r0,[r9,#0x0]
  000d7fe2: blx 0x00071a10
  000d7fe6: blx 0x00071a7c
  000d7fea: ldr r1,[0x000d8028]
  000d7fec: subs r0,r6,r0
  000d7fee: it ne
  000d7ff0: mov.ne r0,#0x1
  000d7ff2: add r1,pc
  000d7ff4: ldr r1,[r1,#0x0]
  000d7ff6: strb r0,[r1,#0x0]
  000d7ff8: beq 0x000d7f36
  000d7ffa: ldr r0,[0x000d802c]
  000d7ffc: ldr.w r1,[r10,#0x1d0]
  000d8000: add r0,pc
  000d8002: ldr r0,[r0,#0x0]
  000d8004: str r1,[r0,#0x0]
  000d8006: b 0x000d7f36
```
