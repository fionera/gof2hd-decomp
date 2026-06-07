# PlayerGasCloud::render
elf 0x176e18 body 254
Sig: undefined __thiscall render(PlayerGasCloud * this)

## decompile
```c

/* PlayerGasCloud::render() */

void __thiscall PlayerGasCloud::render(PlayerGasCloud *this)

{
  undefined4 uVar1;
  uint *puVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  Vector *pVVar6;
  float fVar7;
  Matrix local_80 [12];
  AEMath aAStack_74 [12];
  undefined1 auStack_68 [64];
  int local_28;
  
  piVar3 = *(int **)(DAT_00186f18 + 0x186e2a);
  local_28 = *piVar3;
  if ((this[0xf5] != (PlayerGasCloud)0x0) &&
     (*(int *)(this + 0x88) == 3 || *(int *)(this + 0x88) == 0)) {
    uVar4 = **(uint **)(DAT_00186f1c + 0x186e4a);
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar1 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar4);
    __aeabi_memcpy(auStack_68,uVar1,0x3c);
    AbyssEngine::AEMath::MatrixGetDir(local_80);
    fVar7 = (float)AbyssEngine::AEMath::operator-(aAStack_74,(Vector *)local_80);
    if ((this[0x154] == (PlayerGasCloud)0x0) ||
       (puVar2 = *(uint **)(this + 0x138), puVar2 == (uint *)0x0)) {
      pVVar6 = *(Vector **)(this + 0x134);
      AbyssEngine::AEMath::MatrixGetUp(local_80);
      AEGeometry::setDirection(pVVar6,(Vector *)aAStack_74);
      AEGeometry::render();
    }
    else {
      iVar5 = 0;
      for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
        AEGeometry::setScaling(fVar7);
        pVVar6 = *(Vector **)(*(int *)(*(int *)(this + 0x138) + 4) + iVar5);
        AbyssEngine::AEMath::MatrixGetUp(local_80);
        AEGeometry::setDirection(pVVar6,(Vector *)aAStack_74);
        fVar7 = (float)AEGeometry::render();
        iVar5 = iVar5 + 4;
        puVar2 = *(uint **)(this + 0x138);
      }
    }
  }
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00186e18: push {r4,r5,r6,r7,lr}
  00186e1a: add r7,sp,#0xc
  00186e1c: push {r8,r9,r10,r11}
  00186e20: sub sp,#0x64
  00186e22: mov r4,r0
  00186e24: ldr r0,[0x00186f18]
  00186e26: add r0,pc
  00186e28: ldr r1,[r0,#0x0]
  00186e2a: ldr r0,[r1,#0x0]
  00186e2c: str r0,[sp,#0x60]
  00186e2e: ldrb.w r0,[r4,#0xf5]
  00186e32: cmp r0,#0x0
  00186e34: beq 0x00186f02
  00186e36: ldr.w r0,[r4,#0x88]
  00186e3a: cmp r0,#0x3
  00186e3c: it ne
  00186e3e: cmp.ne r0,#0x0
  00186e40: bne 0x00186f02
  00186e42: str r1,[sp,#0x4]
  00186e44: ldr r0,[0x00186f1c]
  00186e46: add r0,pc
  00186e48: ldr r0,[r0,#0x0]
  00186e4a: ldr r5,[r0,#0x0]
  00186e4c: mov r0,r5
  00186e4e: blx 0x000717f4
  00186e52: mov r1,r0
  00186e54: mov r0,r5
  00186e56: blx 0x0006ff1c
  00186e5a: add r5,sp,#0x20
  00186e5c: mov r1,r0
  00186e5e: movs r2,#0x3c
  00186e60: mov r0,r5
  00186e62: blx 0x0006f1e4
  00186e66: add r6,sp,#0x8
  00186e68: mov r1,r5
  00186e6a: mov r0,r6
  00186e6c: blx 0x0006f4c0
  00186e70: add r0,sp,#0x14
  00186e72: mov r1,r6
  00186e74: blx 0x0006f4cc
  00186e78: ldrb.w r0,[r4,#0x154]
  00186e7c: cbz r0,0x00186ee0
  00186e7e: ldr.w r0,[r4,#0x138]
  00186e82: cbz r0,0x00186ee0
  00186e84: add.w r11,sp,#0x8
  00186e88: add.w r8,sp,#0x20
  00186e8c: add.w r9,sp,#0x14
  00186e90: movs r5,#0x0
  00186e92: mov.w r10,#0x0
  00186e96: b 0x00186ed8
  00186e98: ldr.w r1,[r4,#0x148]
  00186e9c: ldr r0,[r0,#0x4]
  00186e9e: ldr r1,[r1,#0x4]
  00186ea0: ldr r0,[r0,r5]
  00186ea2: ldr r1,[r1,r5]
  00186ea4: blx 0x00076a20
  00186ea8: ldr.w r0,[r4,#0x138]
  00186eac: mov r1,r8
  00186eae: ldr r0,[r0,#0x4]
  00186eb0: ldr r6,[r0,r5]
  00186eb2: mov r0,r11
  00186eb4: blx 0x0006f4d8
  00186eb8: mov r0,r6
  00186eba: mov r1,r9
  00186ebc: mov r2,r11
  00186ebe: blx 0x000726ac
  00186ec2: ldr.w r0,[r4,#0x138]
  00186ec6: ldr r0,[r0,#0x4]
  00186ec8: ldr r0,[r0,r5]
  00186eca: blx 0x00072238
  00186ece: adds r5,#0x4
  00186ed0: ldr.w r0,[r4,#0x138]
  00186ed4: add.w r10,r10,#0x1
  00186ed8: ldr r1,[r0,#0x0]
  00186eda: cmp r10,r1
  00186edc: bcc 0x00186e98
  00186ede: b 0x00186f00
  00186ee0: add r6,sp,#0x8
  00186ee2: add r1,sp,#0x20
  00186ee4: ldr.w r5,[r4,#0x134]
  00186ee8: mov r0,r6
  00186eea: blx 0x0006f4d8
  00186eee: add r1,sp,#0x14
  00186ef0: mov r0,r5
  00186ef2: mov r2,r6
  00186ef4: blx 0x000726ac
  00186ef8: ldr.w r0,[r4,#0x134]
  00186efc: blx 0x00072238
  00186f00: ldr r1,[sp,#0x4]
  00186f02: ldr r0,[sp,#0x60]
  00186f04: ldr r1,[r1,#0x0]
  00186f06: subs r0,r1,r0
  00186f08: ittt eq
  00186f0a: add.eq sp,#0x64
  00186f0c: pop.eq.w {r8,r9,r10,r11}
  00186f10: pop.eq {r4,r5,r6,r7,pc}
  00186f12: blx 0x0006e824
```
