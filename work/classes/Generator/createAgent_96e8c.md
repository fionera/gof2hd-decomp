# Generator::createAgent
elf 0x96e8c body 748
Sig: undefined __thiscall createAgent(Generator * this, Station * param_1)

## decompile
```c

/* Generator::createAgent(Station*) */

void __thiscall Generator::createAgent(Generator *this,Station *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  Agent *this_00;
  undefined4 uVar5;
  undefined4 uVar6;
  int *piVar7;
  Array *pAVar8;
  void *pvVar9;
  uint uVar10;
  Item *this_01;
  int *piVar11;
  undefined4 *puVar12;
  int *piVar13;
  uint in_fpscr;
  float fVar14;
  float fVar15;
  undefined4 in_s14;
  undefined8 uVar16;
  String aSStack_40 [8];
  undefined4 uStack_38;
  int local_34;
  
  piVar11 = *(int **)(DAT_000a71a0 + 0xa6ea2);
  _uStack_38 = CONCAT44(*piVar11,in_s14);
  Status::getSystem();
  iVar1 = SolarSystem::getRace();
  piVar13 = *(int **)(DAT_000a71a8 + 0xa6ec4);
  iVar2 = AbyssEngine::AERandom::nextInt(*piVar13);
  if (iVar2 < 0x14) {
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar13);
  }
  uVar10 = 0;
  iVar2 = -1;
  while ((uVar10 & 1) == 0) {
    uVar16 = AbyssEngine::AERandom::nextInt(*piVar13);
    uVar10 = (uint)((ulonglong)uVar16 >> 0x20);
    iVar2 = (int)uVar16;
    if (iVar1 == 1) {
      uVar10 = 0;
    }
    if (iVar1 != 1 || iVar2 != 6) {
      uVar10 = 1;
      if (iVar2 - 3U < 8) {
        uVar10 = 0x1cU >> (iVar2 - 3U & 0xff) & 1;
      }
    }
  }
  iVar3 = AbyssEngine::AERandom::nextInt(*piVar13);
  if (iVar3 < 0x21) {
    iVar2 = 0;
  }
  else if ((iVar2 - 5U < 2) && (iVar3 = Status::getCurrentCampaignMission(), iVar3 < 0x10)) {
    iVar2 = 0;
  }
  iVar3 = 1;
  if ((iVar1 == 0) && (iVar2 != 6)) {
    iVar4 = AbyssEngine::AERandom::nextInt(*piVar13);
    iVar3 = 0;
    if (iVar4 < 0x3c) {
      iVar3 = 1;
    }
  }
  this_00 = operator_new(0x98);
  puVar12 = *(undefined4 **)(DAT_000a71ac + 0xa6f68);
  Globals::getRandomName((int)aSStack_40,SUB41(*puVar12,0));
  uVar5 = Station::getIndex(param_1);
  Status::getSystem();
  uVar6 = SolarSystem::getIndex();
  Agent::Agent(this_00,0xffffffff,aSStack_40,uVar5,uVar6,iVar1,iVar3,0xffffffff,0xffffffff,
               0xffffffff);
  AbyssEngine::String::~String(aSStack_40);
  Agent::setOffer(this_00,iVar2);
  piVar7 = (int *)ImageFactory::createChar(SUB41(**(undefined4 **)(DAT_000a71b0 + 0xa6fc4),0),iVar3)
  ;
  Agent::setImageParts(this_00,piVar7);
  iVar1 = Agent::getOffer(this_00);
  if (iVar1 == 6) {
    uVar10 = AbyssEngine::AERandom::nextInt(*piVar13);
    pAVar8 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    ArraySetLength<AbyssEngine::String*>(uVar10,pAVar8);
    for (iVar1 = 0; iVar1 < (int)uVar10; iVar1 = iVar1 + 1) {
      pvVar9 = operator_new(0xc);
      uVar5 = *puVar12;
      Agent::getRace(this_00);
      Globals::getRandomName((int)pvVar9,SUB41(uVar5,0));
      *(void **)(*(int *)(pAVar8 + 4) + iVar1 * 4) = pvVar9;
    }
    Agent::setWingmanFriendNames(this_00,pAVar8);
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar13);
    Agent::setCosts(this_00,(iVar1 + 700) * (uVar10 + 1));
    iVar1 = Status::hardCoreMode();
    if (iVar1 != 0) {
      iVar1 = Agent::getCosts(this_00);
      Agent::setCosts(this_00,iVar1 * 7);
    }
  }
  else {
    iVar1 = Agent::getOffer(this_00);
    if (iVar1 == 2) {
      piVar7 = *(int **)(DAT_000a71b4 + 0xa708a);
      iVar1 = *piVar7;
      do {
        do {
          iVar2 = AbyssEngine::AERandom::nextInt(*piVar13);
        } while (iVar2 - 0xd9U < 2);
      } while ((((iVar2 == 0x83) || (iVar2 == 0xa4 || iVar2 == 0xaf)) ||
               (iVar3 = Item::getIngredients(), iVar3 != 0)) ||
              ((iVar3 = Item::getSinglePrice(*(Item **)(*(int *)(iVar1 + 4) + iVar2 * 4)),
               iVar3 == 0 ||
               (iVar3 = Item::getOccurence(*(Item **)(*(int *)(iVar1 + 4) + iVar2 * 4)), iVar3 == 0)
               )));
      this_01 = *(Item **)(*(int *)(iVar1 + 4) + iVar2 * 4);
      iVar1 = AbyssEngine::AERandom::nextInt(*piVar13);
      iVar3 = Item::getType(this_01);
      if ((iVar3 == 3) ||
         ((iVar3 = Item::getType(this_01), iVar3 == 0 ||
          (iVar3 = Item::getType(this_01), iVar3 == 2)))) {
        iVar1 = 1;
      }
      else {
        iVar1 = iVar1 + 5;
      }
      iVar3 = AbyssEngine::AERandom::nextInt(*piVar13);
      fVar14 = (float)VectorSignedToFloat(iVar3 + 0x28,(byte)(in_fpscr >> 0x16) & 3);
      fVar14 = fVar14 / DAT_000a719c;
      uVar5 = Item::getSinglePrice(*(Item **)(*(int *)(*piVar7 + 4) + iVar2 * 4));
      fVar15 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
      Agent::setSellItemData(this_00,iVar2,iVar1,iVar1 * (int)(fVar14 * fVar15));
    }
  }
  iVar1 = *piVar11 - local_34;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  000a6e8c: push {r4,r5,r6,r7,lr}
  000a6e8e: add r7,sp,#0xc
  000a6e90: push {r7,r8,r9,r10,r11}
  000a6e94: vpush {d1,d2,d3,d4,d5,d6,d7,d8}
  000a6e98: ldr r0,[0x000a71a0]
  000a6e9a: mov r8,r1
  000a6e9c: ldr r1,[0x000a71a4]
  000a6e9e: add r0,pc
  000a6ea0: add r1,pc
  000a6ea2: ldr.w r9,[r0,#0x0]
  000a6ea6: ldr r0,[r1,#0x0]
  000a6ea8: ldr.w r1,[r9,#0x0]
  000a6eac: str r0,[sp,#0x20]
  000a6eae: ldr r0,[r0,#0x0]
  000a6eb0: str r1,[sp,#0x34]
  000a6eb2: blx 0x00071a10
  000a6eb6: blx 0x00071950
  000a6eba: mov r5,r0
  000a6ebc: ldr r0,[0x000a71a8]
  000a6ebe: movs r1,#0x64
  000a6ec0: add r0,pc
  000a6ec2: ldr.w r11,[r0,#0x0]
  000a6ec6: ldr.w r0,[r11,#0x0]
  000a6eca: blx 0x00071848
  000a6ece: cmp r0,#0x13
  000a6ed0: bgt 0x000a6ede
  000a6ed2: ldr.w r0,[r11,#0x0]
  000a6ed6: movs r1,#0x8
  000a6ed8: blx 0x00071848
  000a6edc: mov r5,r0
  000a6ede: movs r1,#0x0
  000a6ee0: mov.w r6,#0xffffffff
  000a6ee4: movs r4,#0x1c
  000a6ee6: b 0x000a6f0c
  000a6ee8: movs r1,#0x7
  000a6eea: blx 0x00071848
  000a6eee: mov r6,r0
  000a6ef0: cmp r5,#0x1
  000a6ef2: itt eq
  000a6ef4: mov.eq r1,#0x0
  000a6ef6: cmp.eq r6,#0x6
  000a6ef8: beq 0x000a6f0c
  000a6efa: subs r0,r6,#0x3
  000a6efc: movs r1,#0x1
  000a6efe: cmp r0,#0x7
  000a6f00: ittt ls
  000a6f02: uxtb.ls r0,r0
  000a6f04: lsr.ls.w r0,r4,r0
  000a6f08: and.ls r1,r0,#0x1
  000a6f0c: ldr.w r0,[r11,#0x0]
  000a6f10: lsls r1,r1,#0x1f
  000a6f12: beq 0x000a6ee8
  000a6f14: movs r1,#0x64
  000a6f16: blx 0x00071848
  000a6f1a: cmp r0,#0x21
  000a6f1c: bge 0x000a6f22
  000a6f1e: movs r6,#0x0
  000a6f20: b 0x000a6f36
  000a6f22: subs r0,r6,#0x5
  000a6f24: cmp r0,#0x1
  000a6f26: bhi 0x000a6f36
  000a6f28: ldr r0,[sp,#0x20]
  000a6f2a: ldr r0,[r0,#0x0]
  000a6f2c: blx 0x00071770
  000a6f30: cmp r0,#0xf
  000a6f32: it le
  000a6f34: mov.le r6,#0x0
  000a6f36: str.w r9,[sp,#0x1c]
  000a6f3a: mov.w r9,#0x1
  000a6f3e: cbnz r5,0x000a6f5a
  000a6f40: cmp r6,#0x6
  000a6f42: beq 0x000a6f5a
  000a6f44: ldr.w r0,[r11,#0x0]
  000a6f48: movs r1,#0x64
  000a6f4a: blx 0x00071848
  000a6f4e: mov.w r9,#0x0
  000a6f52: cmp r0,#0x3c
  000a6f54: it lt
  000a6f56: mov.lt.w r9,#0x1
  000a6f5a: movs r0,#0x98
  000a6f5c: blx 0x0006eb24
  000a6f60: str r0,[sp,#0x24]
  000a6f62: ldr r0,[0x000a71ac]
  000a6f64: add r0,pc
  000a6f66: ldr.w r10,[r0,#0x0]
  000a6f6a: ldr.w r1,[r10,#0x0]
  000a6f6e: add r0,sp,#0x28
  000a6f70: mov r2,r5
  000a6f72: mov r3,r9
  000a6f74: blx 0x00071b24
  000a6f78: mov r0,r8
  000a6f7a: blx 0x00071824
  000a6f7e: mov r8,r0
  000a6f80: ldr r0,[sp,#0x20]
  000a6f82: ldr r0,[r0,#0x0]
  000a6f84: blx 0x00071a10
  000a6f88: blx 0x00071a7c
  000a6f8c: mov.w r1,#0xffffffff
  000a6f90: stm sp,{r0,r5,r9}
  000a6f94: add r2,sp,#0x28
  000a6f96: strd r1,r1,[sp,#0xc]
  000a6f9a: mov r3,r8
  000a6f9c: strd r1,r1,[sp,#0x14]
  000a6fa0: mov.w r1,#0xffffffff
  000a6fa4: ldr r0,[sp,#0x24]
  000a6fa6: blx 0x00071b30
  000a6faa: add r0,sp,#0x28
  000a6fac: blx 0x0006ee30
  000a6fb0: ldr r4,[sp,#0x24]
  000a6fb2: mov r1,r6
  000a6fb4: mov r0,r4
  000a6fb6: blx 0x00071b3c
  000a6fba: ldr r0,[0x000a71b0]
  000a6fbc: mov r1,r9
  000a6fbe: mov r2,r5
  000a6fc0: add r0,pc
  000a6fc2: ldr r0,[r0,#0x0]
  000a6fc4: ldr r0,[r0,#0x0]
  000a6fc6: blx 0x00071b48
  000a6fca: mov r1,r0
  000a6fcc: mov r0,r4
  000a6fce: blx 0x00071b54
  000a6fd2: mov r0,r4
  000a6fd4: blx 0x00071ae8
  000a6fd8: cmp r0,#0x6
  000a6fda: bne 0x000a7076
  000a6fdc: ldr.w r0,[r11,#0x0]
  000a6fe0: movs r1,#0x3
  000a6fe2: blx 0x00071848
  000a6fe6: mov r8,r0
  000a6fe8: movs r0,#0xc
  000a6fea: blx 0x0006eb24
  000a6fee: mov r5,r0
  000a6ff0: blx 0x0006f628
  000a6ff4: mov r0,r8
  000a6ff6: mov r1,r5
  000a6ff8: blx 0x0006fe08
  000a6ffc: mov.w r9,#0x0
  000a7000: b 0x000a702a
  000a7002: movs r0,#0xc
  000a7004: blx 0x0006eb24
  000a7008: ldr.w r4,[r10,#0x0]
  000a700c: mov r6,r0
  000a700e: ldr r0,[sp,#0x24]
  000a7010: blx 0x00071bd8
  000a7014: mov r2,r0
  000a7016: mov r0,r6
  000a7018: mov r1,r4
  000a701a: movs r3,#0x1
  000a701c: blx 0x00071b24
  000a7020: ldr r0,[r5,#0x4]
  000a7022: str.w r6,[r0,r9,lsl #0x2]
  000a7026: add.w r9,r9,#0x1
  000a702a: cmp r9,r8
  000a702c: blt 0x000a7002
  000a702e: ldr r4,[sp,#0x24]
  000a7030: mov r1,r5
  000a7032: mov r0,r4
  000a7034: blx 0x00071be4
  000a7038: ldr.w r0,[r11,#0x0]
  000a703c: movw r1,#0x514
  000a7040: blx 0x00071848
  000a7044: add.w r0,r0,#0x2bc
  000a7048: add.w r1,r8,#0x1
  000a704c: muls r1,r0
  000a704e: mov r0,r4
  000a7050: blx 0x00071bf0
  000a7054: ldr r0,[sp,#0x20]
  000a7056: ldr r0,[r0,#0x0]
  000a7058: blx 0x0007192c
  000a705c: ldr.w r9,[sp,#0x1c]
  000a7060: cmp r0,#0x0
  000a7062: beq 0x000a7156
  000a7064: mov r0,r4
  000a7066: blx 0x00071bfc
  000a706a: rsb r1,r0,r0, lsl #0x3
  000a706e: mov r0,r4
  000a7070: blx 0x00071bf0
  000a7074: b 0x000a7156
  000a7076: mov r0,r4
  000a7078: blx 0x00071ae8
  000a707c: ldr.w r9,[sp,#0x1c]
  000a7080: cmp r0,#0x2
  000a7082: bne 0x000a7156
  000a7084: ldr r0,[0x000a71b4]
  000a7086: add r0,pc
  000a7088: ldr.w r8,[r0,#0x0]
  000a708c: ldr.w r4,[r8,#0x0]
  000a7090: ldr r1,[r4,#0x0]
  000a7092: ldr.w r0,[r11,#0x0]
  000a7096: blx 0x00071848
  000a709a: mov r5,r0
  000a709c: subs r0,#0xd9
  000a709e: cmp r0,#0x2
  000a70a0: bcc 0x000a7090
  000a70a2: cmp r5,#0x83
  000a70a4: beq 0x000a7090
  000a70a6: cmp r5,#0xa4
  000a70a8: it ne
  000a70aa: cmp.ne r5,#0xaf
  000a70ac: beq 0x000a7090
  000a70ae: ldr r0,[r4,#0x4]
  000a70b0: ldr.w r0,[r0,r5,lsl #0x2]
  000a70b4: blx 0x00071914
  000a70b8: cmp r0,#0x0
  000a70ba: bne 0x000a7090
  000a70bc: ldr r0,[r4,#0x4]
  000a70be: ldr.w r0,[r0,r5,lsl #0x2]
  000a70c2: blx 0x00071944
  000a70c6: cmp r0,#0x0
  000a70c8: beq 0x000a7090
  000a70ca: ldr r0,[r4,#0x4]
  000a70cc: ldr.w r0,[r0,r5,lsl #0x2]
  000a70d0: blx 0x000718f0
  000a70d4: cmp r0,#0x0
  000a70d6: beq 0x000a7090
  000a70d8: ldr r0,[r4,#0x4]
  000a70da: movs r1,#0xf
  000a70dc: ldr.w r4,[r0,r5,lsl #0x2]
  000a70e0: ldr.w r0,[r11,#0x0]
  000a70e4: blx 0x00071848
  000a70e8: mov r6,r0
  000a70ea: mov r0,r4
  000a70ec: blx 0x000718fc
  000a70f0: cmp r0,#0x3
  000a70f2: beq 0x000a7106
  000a70f4: mov r0,r4
  000a70f6: blx 0x000718fc
  000a70fa: cbz r0,0x000a7106
  000a70fc: mov r0,r4
  000a70fe: blx 0x000718fc
  000a7102: cmp r0,#0x2
  000a7104: bne 0x000a7174
  000a7106: movs r6,#0x1
  000a7108: ldr.w r0,[r11,#0x0]
  000a710c: movs r1,#0x78
  000a710e: blx 0x00071848
  000a7112: adds r0,#0x28
  000a7114: vldr.32 s2,[pc,#0x84]
  000a7118: vmov s0,r0
  000a711c: vcvt.f32.s32 s0,s0
  000a7120: ldr.w r0,[r8,#0x0]
  000a7124: ldr r0,[r0,#0x4]
  000a7126: ldr.w r0,[r0,r5,lsl #0x2]
  000a712a: vdiv.f32 s16,s0,s2
  000a712e: blx 0x00071944
  000a7132: vmov s0,r0
  000a7136: mov r1,r5
  000a7138: mov r2,r6
  000a713a: vcvt.f32.s32 s0,s0
  000a713e: vmul.f32 s0,s16,s0
  000a7142: vcvt.s32.f32 s0,s0
  000a7146: ldr r4,[sp,#0x24]
  000a7148: vmov r0,s0
  000a714c: mul r3,r6,r0
  000a7150: mov r0,r4
  000a7152: blx 0x00071b0c
  000a7156: ldr r0,[sp,#0x34]
  000a7158: ldr.w r1,[r9,#0x0]
  000a715c: subs r0,r1,r0
  000a715e: itttt eq
  000a7160: mov.eq r0,r4
  000a7162: vpop.eq {d1,d2,d3,d4,d5,d6,d7,d8}
  000a7166: add.eq sp,#0x4
  000a7168: pop.eq.w {r8,r9,r10,r11}
  000a716c: it eq
  000a716e: pop.eq {r4,r5,r6,r7,pc}
  000a7170: blx 0x0006e824
  000a7174: adds r6,#0x5
  000a7176: b 0x000a7108
```
