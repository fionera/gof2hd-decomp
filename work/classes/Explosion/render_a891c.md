# Explosion::render
elf 0xa891c body 378
Sig: undefined __stdcall render(void)

## decompile
```c

/* Explosion::render() */

void Explosion::render(void)

{
  uint *in_r0;
  undefined4 uVar1;
  Matrix *pMVar2;
  uint uVar3;
  uint *puVar4;
  Vector *pVVar5;
  int *piVar6;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar7;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar8;
  Matrix aMStack_c8 [12];
  Matrix aMStack_bc [12];
  Matrix aMStack_b0 [60];
  Vector local_74 [8];
  float local_6c;
  Matrix aMStack_68 [64];
  int local_28;
  
  piVar6 = *(int **)(DAT_000b8a98 + 0xb892e);
  local_28 = *piVar6;
  if ((char)in_r0[8] != '\0') {
    if (((0xd < *in_r0) || ((1 << (*in_r0 & 0xff) & 0x2780U) == 0)) && (in_r0[2] != 0)) {
      AEGeometry::render();
    }
    puVar4 = *(uint **)(DAT_000b8a9c + 0xb895c);
    uVar3 = *puVar4;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar1 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar3);
    __aeabi_memcpy(aMStack_68,uVar1,0x3c);
    AEGeometry::getPosition();
    if ((*in_r0 < 0xd) && ((1 << (*in_r0 & 0xff) & 0x1804U) != 0)) {
      AbyssEngine::AEMath::MatrixSetTranslation(aMStack_b0,local_6c,extraout_s1,extraout_s2);
      fVar7 = extraout_s1_00;
      fVar8 = extraout_s2_00;
    }
    else {
      AbyssEngine::AEMath::MatrixGetPosition(aMStack_bc);
      AbyssEngine::AEMath::MatrixGetUp(aMStack_c8);
      AbyssEngine::AEMath::MatrixGetLookAt
                ((AEMath *)aMStack_b0,(Vector *)aMStack_bc,local_74,(Vector *)aMStack_c8);
      AbyssEngine::AEMath::Matrix::operator=(aMStack_68,aMStack_b0);
      fVar7 = extraout_s1_01;
      fVar8 = extraout_s2_01;
    }
    AbyssEngine::AEMath::MatrixSetScaling(aMStack_b0,(float)in_r0[9],fVar7,fVar8);
    if (*in_r0 - 8 < 3) {
      AbyssEngine::AEMath::Matrix::operator*=(aMStack_68,(Matrix *)(in_r0 + 0xb));
    }
    AEGeometry::setMatrix((Matrix *)in_r0[1]);
    AEGeometry::setPosition((AEGeometry *)in_r0[1],local_74);
    uVar3 = *puVar4;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    pMVar2 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetLocal(uVar3);
    AbyssEngine::AEMath::Matrix::operator=(aMStack_68,pMVar2);
    AbyssEngine::AEMath::MatrixGetDir(aMStack_b0);
    pVVar5 = (Vector *)in_r0[1];
    AbyssEngine::AEMath::MatrixGetUp(aMStack_bc);
    AEGeometry::setDirection(pVVar5,(Vector *)aMStack_b0);
    AEGeometry::render();
    puVar4 = (uint *)in_r0[3];
    if (puVar4 != (uint *)0x0) {
      for (uVar3 = 0; uVar3 < *puVar4; uVar3 = uVar3 + 1) {
        AEGeometry::render();
        puVar4 = (uint *)in_r0[3];
      }
    }
  }
  if (*piVar6 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b891c: push {r4,r5,r6,r7,lr}
  000b891e: add r7,sp,#0xc
  000b8920: push {r8,r9,r10,r11}
  000b8924: sub sp,#0xac
  000b8926: mov r4,r0
  000b8928: ldr r0,[0x000b8a98]
  000b892a: add r0,pc
  000b892c: ldr.w r10,[r0,#0x0]
  000b8930: ldr.w r0,[r10,#0x0]
  000b8934: str r0,[sp,#0xa8]
  000b8936: ldrb.w r0,[r4,#0x20]
  000b893a: cmp r0,#0x0
  000b893c: beq.w 0x000b8a72
  000b8940: ldr r0,[r4,#0x0]
  000b8942: cmp r0,#0xd
  000b8944: bhi.w 0x000b8a88
  000b8948: movs r1,#0x1
  000b894a: lsl.w r0,r1,r0
  000b894e: tst r0,#0x2780
  000b8952: beq.w 0x000b8a88
  000b8956: ldr r0,[0x000b8a9c]
  000b8958: add r0,pc
  000b895a: ldr r6,[r0,#0x0]
  000b895c: ldr r5,[r6,#0x0]
  000b895e: mov r0,r5
  000b8960: blx 0x000717f4
  000b8964: mov r1,r0
  000b8966: mov r0,r5
  000b8968: blx 0x0006ff1c
  000b896c: mov r1,r0
  000b896e: add r0,sp,#0x68
  000b8970: movs r2,#0x3c
  000b8972: blx 0x0006f1e4
  000b8976: ldr r1,[r4,#0x4]
  000b8978: add r0,sp,#0x5c
  000b897a: blx 0x000720b8
  000b897e: ldr r0,[r4,#0x0]
  000b8980: cmp r0,#0xc
  000b8982: bhi 0x000b89a8
  000b8984: movs r1,#0x1
  000b8986: lsl.w r0,r1,r0
  000b898a: movw r1,#0x1804
  000b898e: tst r0,r1
  000b8990: beq 0x000b89a8
  000b8992: ldrd r2,r3,[sp,#0x5c]
  000b8996: add r0,sp,#0x20
  000b8998: vldr.32 s0,[sp,#0x64]
  000b899c: add r1,sp,#0x68
  000b899e: vstr.32 s0,[sp]
  000b89a2: blx 0x0006f820
  000b89a6: b 0x000b89da
  000b89a8: add.w r8,sp,#0x14
  000b89ac: add r5,sp,#0x68
  000b89ae: mov r0,r8
  000b89b0: mov r1,r5
  000b89b2: blx 0x0006f16c
  000b89b6: add.w r9,sp,#0x8
  000b89ba: mov r1,r5
  000b89bc: mov r0,r9
  000b89be: blx 0x0006f4d8
  000b89c2: add.w r11,sp,#0x20
  000b89c6: add r2,sp,#0x5c
  000b89c8: mov r1,r8
  000b89ca: mov r3,r9
  000b89cc: mov r0,r11
  000b89ce: blx 0x000725e0
  000b89d2: mov r0,r5
  000b89d4: mov r1,r11
  000b89d6: blx 0x0006f148
  000b89da: vldr.32 s0,[r4,#0x24]
  000b89de: add r0,sp,#0x20
  000b89e0: add r1,sp,#0x68
  000b89e2: vmov r2,s0
  000b89e6: vstr.32 s0,[sp]
  000b89ea: mov r3,r2
  000b89ec: blx 0x0006f814
  000b89f0: ldr r0,[r4,#0x0]
  000b89f2: subs r0,#0x8
  000b89f4: cmp r0,#0x2
  000b89f6: bhi 0x000b8a02
  000b89f8: add.w r1,r4,#0x2c
  000b89fc: add r0,sp,#0x68
  000b89fe: blx 0x0006f4a8
  000b8a02: add.w r9,sp,#0x68
  000b8a06: ldr r0,[r4,#0x4]
  000b8a08: mov r1,r9
  000b8a0a: blx 0x000721e4
  000b8a0e: ldr r0,[r4,#0x4]
  000b8a10: add r1,sp,#0x5c
  000b8a12: blx 0x00072148
  000b8a16: ldr r6,[r6,#0x0]
  000b8a18: mov r0,r6
  000b8a1a: blx 0x000717f4
  000b8a1e: mov r1,r0
  000b8a20: mov r0,r6
  000b8a22: blx 0x0006ff1c
  000b8a26: mov r1,r0
  000b8a28: mov r0,r9
  000b8a2a: blx 0x0006f148
  000b8a2e: add.w r8,sp,#0x20
  000b8a32: mov r1,r9
  000b8a34: mov r0,r8
  000b8a36: blx 0x0006f4c0
  000b8a3a: add r5,sp,#0x14
  000b8a3c: mov r1,r9
  000b8a3e: ldr r6,[r4,#0x4]
  000b8a40: mov r0,r5
  000b8a42: blx 0x0006f4d8
  000b8a46: mov r0,r6
  000b8a48: mov r1,r8
  000b8a4a: mov r2,r5
  000b8a4c: blx 0x000726ac
  000b8a50: ldr r0,[r4,#0x4]
  000b8a52: blx 0x00072238
  000b8a56: ldr r0,[r4,#0xc]
  000b8a58: cbz r0,0x000b8a72
  000b8a5a: movs r5,#0x0
  000b8a5c: b 0x000b8a6c
  000b8a5e: ldr r0,[r0,#0x4]
  000b8a60: ldr.w r0,[r0,r5,lsl #0x2]
  000b8a64: blx 0x00072238
  000b8a68: ldr r0,[r4,#0xc]
  000b8a6a: adds r5,#0x1
  000b8a6c: ldr r1,[r0,#0x0]
  000b8a6e: cmp r5,r1
  000b8a70: bcc 0x000b8a5e
  000b8a72: ldr r0,[sp,#0xa8]
  000b8a74: ldr.w r1,[r10,#0x0]
  000b8a78: subs r0,r1,r0
  000b8a7a: ittt eq
  000b8a7c: add.eq sp,#0xac
  000b8a7e: pop.eq.w {r8,r9,r10,r11}
  000b8a82: pop.eq {r4,r5,r6,r7,pc}
  000b8a84: blx 0x0006e824
  000b8a88: ldr r0,[r4,#0x8]
  000b8a8a: cmp r0,#0x0
  000b8a8c: beq.w 0x000b8956
  000b8a90: blx 0x00072238
  000b8a94: b 0x000b8956
```
