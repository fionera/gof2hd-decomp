# RocketGun::setRadar
elf 0x15ea38 body 586
Sig: undefined __stdcall setRadar(Radar * param_1)

## decompile
```c

/* RocketGun::setRadar(Radar*) */

void RocketGun::setRadar(Radar *param_1)

{
  int iVar1;
  Array<AbyssEngine::AEMath::Matrix> *pAVar2;
  void *pvVar3;
  undefined4 uVar4;
  int *in_r1;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int in_r3;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  
  *(int **)(param_1 + 0xb0) = in_r1;
  iVar1 = *in_r1;
  iVar6 = *(int *)(param_1 + 8);
  *(undefined4 *)(param_1 + 0xe4) = *(undefined4 *)(iVar1 + 0x80);
  iVar5 = *(int *)(iVar6 + 0x58);
  uVar10 = iVar5 - 0x1c;
  if (2 < uVar10) {
    if (iVar5 != 0xc1) {
      in_r3 = *(int *)(iVar6 + 0x5c);
    }
    if (iVar5 != 0xc1 && in_r3 != 0x28) {
      if ((1 < *(int *)(param_1 + 0xd0) - 4U) && (*(int *)(param_1 + 0xd0) != 0x28)) {
        if (iVar5 == 0xe8) {
          uVar4 = *(undefined4 *)(iVar1 + 0x9c);
          uVar9 = AbyssEngine::PaintCanvas::TransformGetLocal(**(uint **)(DAT_0016ec90 + 0x16ec1a));
          uVar9 = ParticleSystemManager::addSystem(uVar4,uVar9,0x2f,0);
          *(undefined4 *)(param_1 + 0xcc) = uVar9;
          uVar4 = *(undefined4 *)(*in_r1 + 0x9c);
        }
        else {
          uVar4 = *(undefined4 *)(iVar1 + 0x84);
          uVar9 = AbyssEngine::PaintCanvas::TransformGetLocal(**(uint **)(DAT_0016ec94 + 0x16ec48));
          uVar9 = ParticleSystemManager::addSystem(uVar4,uVar9,0xc,0);
          *(undefined4 *)(param_1 + 0xcc) = uVar9;
          uVar4 = *(undefined4 *)(*in_r1 + 0x84);
        }
        (*(code *)(DAT_001aba84 + 0x1aba88))(uVar4,uVar9,0,&stack0xfffffff8);
        return;
      }
      pAVar2 = operator_new(0xc);
      Array<AbyssEngine::AEMath::Matrix>::Array(pAVar2);
      *(Array<AbyssEngine::AEMath::Matrix> **)(param_1 + 0xd8) = pAVar2;
      pvVar3 = operator_new(0xc);
      Array<int>::Array();
      *(void **)(param_1 + 0xdc) = pvVar3;
      pvVar3 = operator_new(0xc);
      Array<int>::Array();
      *(void **)(param_1 + 0xe0) = pvVar3;
      ArraySetLength<AbyssEngine::AEMath::Matrix>
                (*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xd8));
      ArraySetLength<int>(*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xdc));
      ArraySetLength<int>(*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xe0));
      iVar1 = 0;
      for (uVar10 = 0; uVar10 < *(uint *)(*(int *)(param_1 + 8) + 8); uVar10 = uVar10 + 1) {
        uVar9 = ParticleSystemManager::addSystem
                          (*(undefined4 *)(*in_r1 + 0x80),
                           *(int *)(*(int *)(param_1 + 0xd8) + 4) + iVar1,0x27,0);
        *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xdc) + 4) + uVar10 * 4) = uVar9;
        ParticleSystemManager::enableSystemEmit(*(int *)(*in_r1 + 0x80),SUB41(uVar9,0));
        iVar1 = iVar1 + 0x3c;
        *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xe0) + 4) + uVar10 * 4) = 0;
      }
      return;
    }
  }
  if (*(int *)(iVar6 + 0x5c) == 0x28) {
    uVar10 = 0;
  }
  else if (iVar5 == 0xc1) {
    uVar10 = 3;
  }
  pAVar2 = operator_new(0xc);
  Array<AbyssEngine::AEMath::Matrix>::Array(pAVar2);
  *(Array<AbyssEngine::AEMath::Matrix> **)(param_1 + 0xd8) = pAVar2;
  pvVar3 = operator_new(0xc);
  Array<int>::Array();
  *(void **)(param_1 + 0xdc) = pvVar3;
  pvVar3 = operator_new(0xc);
  Array<int>::Array();
  *(void **)(param_1 + 0xe0) = pvVar3;
  ArraySetLength<AbyssEngine::AEMath::Matrix>
            (*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xd8));
  ArraySetLength<int>(*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xdc));
  ArraySetLength<int>(*(uint *)(*(int *)(param_1 + 8) + 8),*(Array **)(param_1 + 0xe0));
  uVar9 = 0x1c;
  if (uVar10 == 2) {
    uVar9 = 0x1b;
  }
  iVar1 = 0;
  for (uVar8 = 0; uVar8 < *(uint *)(*(int *)(param_1 + 8) + 8); uVar8 = uVar8 + 1) {
    if (*(int *)(*(int *)(param_1 + 8) + 0x58) == 0xc1) {
      uVar4 = *(undefined4 *)(*in_r1 + 0x98);
      *(undefined4 *)(param_1 + 0xe4) = uVar4;
    }
    else {
      uVar4 = *(undefined4 *)(param_1 + 0xe4);
    }
    if (uVar10 == 0) {
      uVar7 = 0x19;
    }
    else {
      uVar7 = uVar9;
      if (uVar10 == 1) {
        uVar7 = 0x1a;
      }
    }
    uVar4 = ParticleSystemManager::addSystem
                      (uVar4,*(int *)(*(int *)(param_1 + 0xd8) + 4) + iVar1,uVar7,0);
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xdc) + 4) + uVar8 * 4) = uVar4;
    ParticleSystemManager::enableSystemEmit(*(int *)(param_1 + 0xe4),SUB41(uVar4,0));
    iVar1 = iVar1 + 0x3c;
    *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xe0) + 4) + uVar8 * 4) = 0;
  }
  return;
}

```

## target disasm
```
  0016ea38: push {r4,r5,r6,r7,lr}
  0016ea3a: add r7,sp,#0xc
  0016ea3c: push {r7,r8,r9,r10,r11}
  0016ea40: str.w r1,[r0,#0xb0]
  0016ea44: mov r5,r0
  0016ea46: ldr r0,[r1,#0x0]
  0016ea48: mov r8,r1
  0016ea4a: ldr r2,[r5,#0x8]
  0016ea4c: ldr.w r1,[r0,#0x80]
  0016ea50: str.w r1,[r5,#0xe4]
  0016ea54: ldr r1,[r2,#0x58]
  0016ea56: sub.w r11,r1,#0x1c
  0016ea5a: cmp.w r11,#0x3
  0016ea5e: bcc 0x0016ea6a
  0016ea60: cmp r1,#0xc1
  0016ea62: itt ne
  0016ea64: ldr.ne r3,[r2,#0x5c]
  0016ea66: cmp.ne r3,#0x28
  0016ea68: bne 0x0016eb56
  0016ea6a: ldr r0,[r2,#0x5c]
  0016ea6c: cmp r0,#0x28
  0016ea6e: bne 0x0016ea76
  0016ea70: mov.w r11,#0x0
  0016ea74: b 0x0016ea7e
  0016ea76: cmp r1,#0xc1
  0016ea78: it eq
  0016ea7a: mov.eq.w r11,#0x3
  0016ea7e: movs r0,#0xc
  0016ea80: blx 0x0006eb24
  0016ea84: mov r6,r0
  0016ea86: blx 0x0006f8e0
  0016ea8a: movs r0,#0xc
  0016ea8c: str.w r6,[r5,#0xd8]
  0016ea90: blx 0x0006eb24
  0016ea94: mov r6,r0
  0016ea96: blx 0x000701f8
  0016ea9a: movs r0,#0xc
  0016ea9c: str.w r6,[r5,#0xdc]
  0016eaa0: blx 0x0006eb24
  0016eaa4: mov r6,r0
  0016eaa6: blx 0x000701f8
  0016eaaa: ldr r0,[r5,#0x8]
  0016eaac: ldr.w r1,[r5,#0xd8]
  0016eab0: str.w r6,[r5,#0xe0]
  0016eab4: ldr r0,[r0,#0x8]
  0016eab6: blx 0x0007795c
  0016eaba: ldr r0,[r5,#0x8]
  0016eabc: ldr.w r1,[r5,#0xdc]
  0016eac0: ldr r0,[r0,#0x8]
  0016eac2: blx 0x00071a4c
  0016eac6: ldr r0,[r5,#0x8]
  0016eac8: ldr.w r1,[r5,#0xe0]
  0016eacc: ldr r0,[r0,#0x8]
  0016eace: blx 0x00071a4c
  0016ead2: mov.w r10,#0x1c
  0016ead6: cmp.w r11,#0x2
  0016eada: it eq
  0016eadc: mov.eq.w r10,#0x1b
  0016eae0: mov.w r9,#0x0
  0016eae4: movs r6,#0x0
  0016eae6: movs r4,#0x0
  0016eae8: b 0x0016eb48
  0016eaea: ldr r0,[r0,#0x58]
  0016eaec: cmp r0,#0xc1
  0016eaee: bne 0x0016eafe
  0016eaf0: ldr.w r0,[r8,#0x0]
  0016eaf4: ldr.w r0,[r0,#0x98]
  0016eaf8: str.w r0,[r5,#0xe4]
  0016eafc: b 0x0016eb02
  0016eafe: ldr.w r0,[r5,#0xe4]
  0016eb02: ldr.w r1,[r5,#0xd8]
  0016eb06: cmp.w r11,#0x0
  0016eb0a: ldr r1,[r1,#0x4]
  0016eb0c: add r1,r6
  0016eb0e: beq 0x0016eb1c
  0016eb10: mov r2,r10
  0016eb12: cmp.w r11,#0x1
  0016eb16: it eq
  0016eb18: mov.eq r2,#0x1a
  0016eb1a: b 0x0016eb1e
  0016eb1c: movs r2,#0x19
  0016eb1e: movs r3,#0x0
  0016eb20: blx 0x000724d8
  0016eb24: mov r1,r0
  0016eb26: ldr.w r0,[r5,#0xdc]
  0016eb2a: movs r2,#0x0
  0016eb2c: ldr r0,[r0,#0x4]
  0016eb2e: str.w r1,[r0,r4,lsl #0x2]
  0016eb32: ldr.w r0,[r5,#0xe4]
  0016eb36: blx 0x000723c4
  0016eb3a: ldr.w r0,[r5,#0xe0]
  0016eb3e: adds r6,#0x3c
  0016eb40: ldr r0,[r0,#0x4]
  0016eb42: str.w r9,[r0,r4,lsl #0x2]
  0016eb46: adds r4,#0x1
  0016eb48: ldr r0,[r5,#0x8]
  0016eb4a: ldr r1,[r0,#0x8]
  0016eb4c: cmp r4,r1
  0016eb4e: bcc 0x0016eaea
  0016eb50: pop.w {r3,r8,r9,r10,r11}
  0016eb54: pop {r4,r5,r6,r7,pc}
  0016eb56: ldr.w r2,[r5,#0xd0]
  0016eb5a: subs r3,r2,#0x4
  0016eb5c: cmp r3,#0x2
  0016eb5e: bcc 0x0016eb64
  0016eb60: cmp r2,#0x28
  0016eb62: bne 0x0016ec0c
  0016eb64: movs r0,#0xc
  0016eb66: blx 0x0006eb24
  0016eb6a: mov r6,r0
  0016eb6c: blx 0x0006f8e0
  0016eb70: movs r0,#0xc
  0016eb72: str.w r6,[r5,#0xd8]
  0016eb76: blx 0x0006eb24
  0016eb7a: mov r6,r0
  0016eb7c: blx 0x000701f8
  0016eb80: movs r0,#0xc
  0016eb82: str.w r6,[r5,#0xdc]
  0016eb86: blx 0x0006eb24
  0016eb8a: mov r6,r0
  0016eb8c: blx 0x000701f8
  0016eb90: ldr r0,[r5,#0x8]
  0016eb92: ldr.w r1,[r5,#0xd8]
  0016eb96: str.w r6,[r5,#0xe0]
  0016eb9a: ldr r0,[r0,#0x8]
  0016eb9c: blx 0x0007795c
  0016eba0: ldr r0,[r5,#0x8]
  0016eba2: ldr.w r1,[r5,#0xdc]
  0016eba6: ldr r0,[r0,#0x8]
  0016eba8: blx 0x00071a4c
  0016ebac: ldr r0,[r5,#0x8]
  0016ebae: ldr.w r1,[r5,#0xe0]
  0016ebb2: ldr r0,[r0,#0x8]
  0016ebb4: blx 0x00071a4c
  0016ebb8: mov.w r9,#0x0
  0016ebbc: movs r6,#0x0
  0016ebbe: movs r4,#0x0
  0016ebc0: b 0x0016ec02
  0016ebc2: ldr.w r0,[r5,#0xd8]
  0016ebc6: movs r3,#0x0
  0016ebc8: ldr.w r1,[r8,#0x0]
  0016ebcc: ldr r2,[r0,#0x4]
  0016ebce: ldr.w r0,[r1,#0x80]
  0016ebd2: adds r1,r2,r6
  0016ebd4: movs r2,#0x27
  0016ebd6: blx 0x000724d8
  0016ebda: mov r1,r0
  0016ebdc: ldr.w r0,[r5,#0xdc]
  0016ebe0: movs r2,#0x0
  0016ebe2: ldr r0,[r0,#0x4]
  0016ebe4: str.w r1,[r0,r4,lsl #0x2]
  0016ebe8: ldr.w r0,[r8,#0x0]
  0016ebec: ldr.w r0,[r0,#0x80]
  0016ebf0: blx 0x000723c4
  0016ebf4: ldr.w r0,[r5,#0xe0]
  0016ebf8: adds r6,#0x3c
  0016ebfa: ldr r0,[r0,#0x4]
  0016ebfc: str.w r9,[r0,r4,lsl #0x2]
  0016ec00: adds r4,#0x1
  0016ec02: ldr r0,[r5,#0x8]
  0016ec04: ldr r0,[r0,#0x8]
  0016ec06: cmp r4,r0
  0016ec08: bcc 0x0016ebc2
  0016ec0a: b 0x0016eb50
  0016ec0c: cmp r1,#0xe8
  0016ec0e: bne 0x0016ec3e
  0016ec10: ldr r1,[0x0016ec90]
  0016ec12: ldr.w r6,[r0,#0x9c]
  0016ec16: add r1,pc
  0016ec18: ldr r2,[r1,#0x0]
  0016ec1a: ldr r1,[r5,#0x10]
  0016ec1c: ldr r0,[r2,#0x0]
  0016ec1e: blx 0x000720c4
  0016ec22: mov r1,r0
  0016ec24: mov r0,r6
  0016ec26: movs r2,#0x2f
  0016ec28: movs r3,#0x0
  0016ec2a: blx 0x000724d8
  0016ec2e: str.w r0,[r5,#0xcc]
  0016ec32: mov r1,r0
  0016ec34: ldr.w r0,[r8,#0x0]
  0016ec38: ldr.w r0,[r0,#0x9c]
  0016ec3c: b 0x0016ec6a
  0016ec3e: ldr r1,[0x0016ec94]
  0016ec40: ldr.w r6,[r0,#0x84]
  0016ec44: add r1,pc
  0016ec46: ldr r2,[r1,#0x0]
  0016ec48: ldr r1,[r5,#0x10]
  0016ec4a: ldr r0,[r2,#0x0]
  0016ec4c: blx 0x000720c4
  0016ec50: mov r1,r0
  0016ec52: mov r0,r6
  0016ec54: movs r2,#0xc
  0016ec56: movs r3,#0x0
  0016ec58: blx 0x000724d8
  0016ec5c: str.w r0,[r5,#0xcc]
  0016ec60: mov r1,r0
  0016ec62: ldr.w r0,[r8,#0x0]
  0016ec66: ldr.w r0,[r0,#0x84]
  0016ec6a: movs r2,#0x0
  0016ec6c: pop.w {r3,r8,r9,r10,r11}
  0016ec70: pop.w {r4,r5,r6,r7,lr}
  0016ec74: b.w 0x001aba78
  001aba78: bx pc
  001aba7c: ldr r12,[0x1aba84]
  001aba80: add pc,r12,pc
```
