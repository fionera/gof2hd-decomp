# PlayerJunk::update
elf 0x15e798 body 276
Sig: undefined __thiscall update(PlayerJunk * this, int param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* PlayerJunk::update(int) */

void __thiscall PlayerJunk::update(PlayerJunk *this,int param_1)

{
  int iVar1;
  Array *pAVar2;
  int *piVar3;
  int *piVar4;
  float fVar5;
  float extraout_s0;
  float extraout_s0_00;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 local_28;
  int local_24;
  
  piVar3 = *(int **)(_LAB_0016e8c0 + 0x16e7aa);
  local_24 = *piVar3;
  *(int *)(this + 0x124) = param_1;
  iVar1 = Player::getHitpoints();
  if (iVar1 < 1) {
    iVar1 = *(int *)(this + 0x88);
    if (iVar1 - 3U < 2) goto LAB_0016e88e;
    fVar5 = (float)Level::junkDied();
    iVar1 = _LAB_0016e8c4;
    *(undefined4 *)(this + 0x88) = 3;
    FModSound::play(**(int **)(iVar1 + 0x16e7da),(Vector *)0x16,(Vector *)0x0,fVar5);
    piVar4 = *(int **)(_LAB_0016e8c8 + 0x16e7f0);
    iVar1 = AbyssEngine::AERandom::nextInt(*piVar4);
    if (iVar1 < 10) {
      this[0x4c] = (PlayerJunk)0x1;
      pAVar2 = operator_new(0xc);
      Array<int>::Array();
      *(Array **)(this + 0x50) = pAVar2;
      ArrayAdd<int>(99,pAVar2);
      iVar1 = AbyssEngine::AERandom::nextInt(*piVar4);
      ArrayAdd<int>(iVar1 + 1,*(Array **)(this + 0x50));
      fVar5 = (float)KIPlayer::createCrate((int)this);
      this[0x4c] = (PlayerJunk)0x1;
    }
    else {
      Player::setActive(SUB41(*(undefined4 *)(this + 4),0));
      iVar1 = Level::getPlayer();
      fVar5 = extraout_s0;
      if (*(PlayerJunk **)(*(int *)(iVar1 + 0x14) + 0x1c) == this) {
        iVar1 = Level::getPlayer();
        *(undefined4 *)(*(int *)(iVar1 + 0x14) + 0x1c) = 0;
        fVar5 = extraout_s0_00;
      }
    }
    local_30 = *(undefined4 *)(this + 0x58);
    uStack_2c = *(undefined4 *)(this + 0x5c);
    local_28 = *(undefined4 *)(this + 0x60);
    ParticleSystemManager::emitManual
              (*(int *)(*(int *)(this + 0x54) + 0x74),*(Vector **)(*(int *)(this + 0x54) + 0x34),
               (int)&local_30,(Vector *)0x0,fVar5);
  }
  iVar1 = *(int *)(this + 0x88);
LAB_0016e88e:
  if (iVar1 == 3) {
    *(undefined4 *)(this + 0x88) = 4;
  }
  if (*piVar3 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016e798: push {r4,r5,r6,r7,lr}
  0016e79a: add r7,sp,#0xc
  0016e79c: push {r8,r9,r11}
  0016e7a0: sub sp,#0x28
  0016e7a2: mov r4,r0
  0016e7a4: ldr r0,[0x0016e8c0]
  0016e7a6: add r0,pc
  0016e7a8: ldr r6,[r0,#0x0]
  0016e7aa: ldr r0,[r6,#0x0]
  0016e7ac: str r0,[sp,#0x24]
  0016e7ae: ldr r0,[r4,#0x4]
  0016e7b0: str.w r1,[r4,#0x124]
  0016e7b4: blx 0x000724f0
  0016e7b8: cmp r0,#0x0
  0016e7ba: bgt 0x0016e88a
  0016e7bc: ldr.w r0,[r4,#0x88]
  0016e7c0: subs r1,r0,#0x3
  0016e7c2: cmp r1,#0x2
  0016e7c4: bcc 0x0016e88e
  0016e7c6: ldr r0,[r4,#0x54]
  0016e7c8: blx 0x00077908
  0016e7cc: ldr r0,[0x0016e8c4]
  0016e7ce: movs r1,#0x3
  0016e7d0: str.w r1,[r4,#0x88]
  0016e7d4: movs r1,#0x0
  0016e7d6: add r0,pc
  0016e7d8: str r1,[sp,#0x0]
  0016e7da: movs r1,#0x16
  0016e7dc: movs r2,#0x0
  0016e7de: ldr r0,[r0,#0x0]
  0016e7e0: movs r3,#0x0
  0016e7e2: ldr r0,[r0,#0x0]
  0016e7e4: blx 0x00071548
  0016e7e8: ldr r0,[0x0016e8c8]
  0016e7ea: movs r1,#0x64
  0016e7ec: add r0,pc
  0016e7ee: ldr.w r9,[r0,#0x0]
  0016e7f2: ldr.w r0,[r9,#0x0]
  0016e7f6: blx 0x00071848
  0016e7fa: cmp r0,#0x9
  0016e7fc: bgt 0x0016e83c
  0016e7fe: mov.w r8,#0x1
  0016e802: movs r0,#0xc
  0016e804: strb.w r8,[r4,#0x4c]
  0016e808: blx 0x0006eb24
  0016e80c: mov r5,r0
  0016e80e: blx 0x000701f8
  0016e812: movs r0,#0x63
  0016e814: mov r1,r5
  0016e816: str r5,[r4,#0x50]
  0016e818: blx 0x0007021c
  0016e81c: ldr.w r0,[r9,#0x0]
  0016e820: movs r1,#0xa
  0016e822: blx 0x00071848
  0016e826: ldr r1,[r4,#0x50]
  0016e828: adds r0,#0x1
  0016e82a: blx 0x0007021c
  0016e82e: mov r0,r4
  0016e830: movs r1,#0x3
  0016e832: blx 0x00075904
  0016e836: strb.w r8,[r4,#0x4c]
  0016e83a: b 0x0016e85e
  0016e83c: ldr r0,[r4,#0x4]
  0016e83e: movs r1,#0x0
  0016e840: movs r5,#0x0
  0016e842: blx 0x00072580
  0016e846: ldr r0,[r4,#0x54]
  0016e848: blx 0x00072034
  0016e84c: ldr r0,[r0,#0x14]
  0016e84e: ldr r0,[r0,#0x1c]
  0016e850: cmp r0,r4
  0016e852: bne 0x0016e85e
  0016e854: ldr r0,[r4,#0x54]
  0016e856: blx 0x00072034
  0016e85a: ldr r0,[r0,#0x14]
  0016e85c: str r5,[r0,#0x1c]
  0016e85e: ldr r2,[0x0016e8bc]
  0016e860: ldrd r0,lr,[r4,#0x54]
  0016e864: ldrd r3,r5,[r4,#0x5c]
  0016e868: mov r12,r2
  0016e86a: movs r2,#0x0
  0016e86c: ldr r1,[r0,#0x34]
  0016e86e: ldr r0,[r0,#0x74]
  0016e870: strd lr,r3,[sp,#0x18]
  0016e874: movs r3,#0x0
  0016e876: str r5,[sp,#0x20]
  0016e878: strd r2,r2,[sp,#0xc]
  0016e87c: str r2,[sp,#0x14]
  0016e87e: add r2,sp,#0xc
  0016e880: strd r2,r12,[sp,#0x0]
  0016e884: add r2,sp,#0x18
  0016e886: blx 0x00072a30
  0016e88a: ldr.w r0,[r4,#0x88]
  0016e88e: cmp r0,#0x3
  0016e890: itt eq
  0016e892: mov.eq r0,#0x4
  0016e894: str.w.eq r0,[r4,#0x88]
  0016e898: ldr r0,[sp,#0x24]
  0016e89a: ldr r1,[r6,#0x0]
  0016e89c: subs r0,r1,r0
  0016e89e: ittt eq
  0016e8a0: add.eq sp,#0x28
  0016e8a2: pop.eq.w {r8,r9,r11}
  0016e8a6: pop.eq {r4,r5,r6,r7,pc}
  0016e8a8: blx 0x0006e824
```
