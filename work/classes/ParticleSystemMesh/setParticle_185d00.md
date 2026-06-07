# ParticleSystemMesh::setParticle
elf 0x185d00 body 892
Sig: undefined __stdcall setParticle(Vector * param_1, float param_2, uint param_3, float param_4, float param_5, float param_6, float param_7, bool param_8, float param_9, float param_10, Vector * param_11, bool param_12)

## decompile
```c

/* ParticleSystemMesh::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float,
   float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&, bool) */

void ParticleSystemMesh::setParticle
               (Vector *param_1,float param_2,uint param_3,float param_4,float param_5,float param_6
               ,float param_7,bool param_8,float param_9,float param_10,Vector *param_11,
               bool param_12)

{
  int *piVar1;
  uint uVar2;
  code *pcVar3;
  code *pcVar4;
  int iVar5;
  int iVar6;
  Vector *pVVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  float fVar11;
  undefined3 in_stack_00000001;
  undefined4 in_stack_00000004;
  undefined4 in_stack_00000008;
  undefined4 in_stack_0000000c;
  int in_stack_00000010;
  float in_stack_00000014;
  int in_stack_00000020;
  Vector aVStack_8c [12];
  Vector aVStack_80 [12];
  Matrix aMStack_74 [12];
  Matrix aMStack_68 [12];
  Matrix aMStack_5c [12];
  Vector aVStack_50 [12];
  int local_44;
  
  piVar1 = *(int **)(DAT_00196080 + 0x195d1e);
  local_44 = *piVar1;
  fVar11 = (float)AbyssEngine::AEMath::MatrixGetRight(aMStack_5c);
  AbyssEngine::AEMath::operator*(aVStack_50,fVar11);
  if (param_1[0x4c] != (Vector)0x0) {
    AbyssEngine::AEMath::operator-((AEMath *)aMStack_5c,aVStack_50);
    AbyssEngine::AEMath::Vector::operator=(aVStack_50,(Vector *)aMStack_5c);
  }
  AbyssEngine::AEMath::MatrixGetUp(aMStack_68);
  fVar11 = (float)(uint)param_8;
  if (in_stack_00000014 != 0.0) {
    fVar11 = in_stack_00000014;
  }
  AbyssEngine::AEMath::operator*((Vector *)aMStack_5c,fVar11);
  fVar11 = (float)AbyssEngine::AEMath::MatrixGetDir(aMStack_74);
  AbyssEngine::AEMath::operator*((Vector *)aMStack_68,fVar11);
  uVar2 = *(uint *)(param_1 + 0x34);
  if ((uVar2 & 0x20000) != 0) {
    fVar11 = (float)AbyssEngine::AEMath::operator-(aVStack_80,(Vector *)aMStack_5c);
    AbyssEngine::AEMath::operator*((Vector *)aMStack_74,fVar11);
    fVar11 = (float)AbyssEngine::AEMath::operator+(aVStack_8c,aVStack_50);
    AbyssEngine::AEMath::operator*(aVStack_80,fVar11);
    AbyssEngine::AEMath::Vector::operator=(aVStack_50,aVStack_80);
    AbyssEngine::AEMath::Vector::operator=((Vector *)aMStack_5c,(Vector *)aMStack_74);
    uVar2 = *(uint *)(param_1 + 0x34);
  }
  pVVar7 = (Vector *)
           (*(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x9c) * *(int *)(param_1 + 0x50) * 4);
  if ((int)(uVar2 << 0x13) < 0) {
    AbyssEngine::AEMath::operator-(aVStack_80,(Vector *)param_3);
    AbyssEngine::AEMath::operator-((Vector *)aMStack_74,aVStack_80);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7);
    AbyssEngine::AEMath::operator+(aVStack_80,(Vector *)param_3);
    AbyssEngine::AEMath::operator+((Vector *)aMStack_74,aVStack_80);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7 + 2);
    iVar5 = 8;
    uVar2 = *(uint *)(param_1 + 0x34);
    if (param_1[0x74] == (Vector)0x0) {
      iVar5 = 4;
    }
    pVVar7 = pVVar7 + iVar5;
  }
  if ((int)(uVar2 << 0x12) < 0) {
    AbyssEngine::AEMath::operator-(aVStack_80,(Vector *)param_3);
    AbyssEngine::AEMath::operator+((Vector *)aMStack_74,aVStack_80);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7);
    AbyssEngine::AEMath::operator+(aVStack_80,(Vector *)param_3);
    AbyssEngine::AEMath::operator-((Vector *)aMStack_74,aVStack_80);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7 + 2);
    iVar5 = 8;
    uVar2 = *(uint *)(param_1 + 0x34);
    if (param_1[0x74] == (Vector)0x0) {
      iVar5 = 4;
    }
    pVVar7 = pVVar7 + iVar5;
  }
  if ((int)(uVar2 << 0x11) < 0) {
    AbyssEngine::AEMath::operator+((Vector *)aMStack_74,(Vector *)param_3);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7);
    AbyssEngine::AEMath::operator-((Vector *)aMStack_74,(Vector *)param_3);
    setQuadEdge(param_1,(int)aMStack_74,pVVar7 + 2);
  }
  pVVar7 = param_11;
  if ((in_stack_00000010 != 0) && (in_stack_00000020 == 0)) {
    uVar2 = 0xff;
    if (param_1[0x45] == (Vector)0x0) {
      uVar2 = 0xffffff00;
    }
    pVVar7 = (Vector *)(uVar2 & (uint)param_11);
  }
  if (in_stack_00000020 != 0) {
    uVar2 = 0xff;
    if (param_1[0x45] == (Vector)0x0) {
      uVar2 = 0xffffff00;
    }
    param_11 = (Vector *)((uint)param_11 & uVar2);
  }
  iVar5 = *(int *)(param_1 + 0x9c);
  uVar2 = *(int *)(param_1 + 0x58) + iVar5 * *(int *)(param_1 + 0x50) * 4;
  pcVar3 = *(code **)(DAT_00196084 + 0x195f8a);
  pcVar4 = *(code **)(DAT_00196088 + 0x195f9c);
  for (iVar6 = 0; iVar6 < iVar5; iVar6 = iVar6 + 1) {
    (*pcVar3)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar2 & 0xffff,param_11);
    uVar8 = uVar2 + 1 & 0xffff;
    (*pcVar3)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar8,param_11);
    uVar10 = uVar2 + 2 & 0xffff;
    (*pcVar3)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar10,pVVar7);
    uVar9 = uVar2 + 3 & 0xffff;
    (*pcVar3)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar9,pVVar7);
    (*pcVar4)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar2 & 0xffff,_param_12,
              in_stack_00000004);
    (*pcVar4)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar8,in_stack_00000008,
              in_stack_00000004);
    (*pcVar4)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar10,_param_12,
              in_stack_0000000c);
    (*pcVar4)(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x54),uVar9,in_stack_00000008,
              in_stack_0000000c);
    uVar2 = uVar2 + 4;
    iVar5 = *(int *)(param_1 + 0x9c);
  }
  if (*piVar1 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00195d00: push {r4,r5,r6,r7,lr}
  00195d02: add r7,sp,#0xc
  00195d04: push {r7,r8,r9,r10,r11}
  00195d08: vpush {d8,d9,d10}
  00195d0c: sub sp,#0x78
  00195d0e: str r3,[sp,#0x20]
  00195d10: mov r11,r0
  00195d12: ldr r0,[0x00196080]
  00195d14: mov r8,r1
  00195d16: add r6,sp,#0x5c
  00195d18: mov r4,r2
  00195d1a: add r0,pc
  00195d1c: ldr r0,[r0,#0x0]
  00195d1e: str r0,[sp,#0x8]
  00195d20: ldr r0,[r0,#0x0]
  00195d22: str r0,[sp,#0x74]
  00195d24: mov r0,r6
  00195d26: ldr.w r1,[r11,#0x18]
  00195d2a: blx 0x0006f4e4
  00195d2e: add r0,sp,#0x68
  00195d30: mov r1,r6
  00195d32: mov r2,r4
  00195d34: blx 0x0006ec74
  00195d38: ldrb.w r0,[r11,#0x4c]
  00195d3c: vmov s18,r4
  00195d40: vldr.32 s16,[r7,#0x20]
  00195d44: vldr.32 s20,[r7,#0x1c]
  00195d48: cbz r0,0x00195d5e
  00195d4a: add r4,sp,#0x5c
  00195d4c: add r6,sp,#0x68
  00195d4e: mov r0,r4
  00195d50: mov r1,r6
  00195d52: blx 0x0006f4cc
  00195d56: mov r0,r6
  00195d58: mov r1,r4
  00195d5a: blx 0x0006eb3c
  00195d5e: add r4,sp,#0x50
  00195d60: ldr.w r1,[r11,#0x18]
  00195d64: mov r0,r4
  00195d66: blx 0x0006f4d8
  00195d6a: vcmpe.f32 s20,#0
  00195d6e: add r0,sp,#0x5c
  00195d70: vmrs apsr,fpscr
  00195d74: vmov.f32 s0,s18
  00195d78: mov r1,r4
  00195d7a: it ne
  00195d7c: vmov.ne.f32 s0,s20
  00195d80: vmov r2,s0
  00195d84: blx 0x0006ec74
  00195d88: add r4,sp,#0x44
  00195d8a: ldr.w r1,[r11,#0x18]
  00195d8e: mov r0,r4
  00195d90: blx 0x0006f4c0
  00195d94: vcmpe.f32 s16,#0
  00195d98: add r0,sp,#0x50
  00195d9a: vmrs apsr,fpscr
  00195d9e: mov r1,r4
  00195da0: it ne
  00195da2: vmov.ne.f32 s18,s16
  00195da6: vmov r2,s18
  00195daa: blx 0x0006ec74
  00195dae: ldr.w r0,[r11,#0x34]
  00195db2: tst r0,#0x20000
  00195db6: beq 0x00195e04
  00195db8: add r4,sp,#0x38
  00195dba: add.w r9,sp,#0x5c
  00195dbe: add r5,sp,#0x68
  00195dc0: mov r0,r4
  00195dc2: mov r1,r9
  00195dc4: mov r2,r5
  00195dc6: blx 0x0006ec38
  00195dca: add.w r10,sp,#0x44
  00195dce: ldr r2,[0x0019607c]
  00195dd0: mov r1,r4
  00195dd2: mov r0,r10
  00195dd4: blx 0x0006ec74
  00195dd8: add r4,sp,#0x2c
  00195dda: mov r1,r5
  00195ddc: mov r2,r9
  00195dde: mov r0,r4
  00195de0: blx 0x0006f1cc
  00195de4: add r6,sp,#0x38
  00195de6: ldr r2,[0x0019607c]
  00195de8: mov r1,r4
  00195dea: mov r0,r6
  00195dec: blx 0x0006ec74
  00195df0: mov r0,r5
  00195df2: mov r1,r6
  00195df4: blx 0x0006eb3c
  00195df8: mov r0,r9
  00195dfa: mov r1,r10
  00195dfc: blx 0x0006eb3c
  00195e00: ldr.w r0,[r11,#0x34]
  00195e04: ldr.w r1,[r11,#0x50]
  00195e08: ldr.w r3,[r11,#0x9c]
  00195e0c: ldr.w r2,[r11,#0x58]
  00195e10: ldr.w r9,[r7,#0x24]
  00195e14: muls r1,r3
  00195e16: add.w r6,r2,r1, lsl #0x2
  00195e1a: lsls r1,r0,#0x13
  00195e1c: bpl 0x00195e80
  00195e1e: add r5,sp,#0x38
  00195e20: add.w r10,sp,#0x50
  00195e24: mov r1,r8
  00195e26: mov r0,r5
  00195e28: mov r2,r10
  00195e2a: blx 0x0006ec38
  00195e2e: add r4,sp,#0x44
  00195e30: mov r1,r5
  00195e32: mov r2,r9
  00195e34: mov r0,r4
  00195e36: blx 0x0006ec38
  00195e3a: add r5,sp,#0x68
  00195e3c: mov r0,r11
  00195e3e: mov r1,r4
  00195e40: mov r2,r6
  00195e42: mov r3,r5
  00195e44: blx 0x00078b74
  00195e48: add r4,sp,#0x38
  00195e4a: mov r1,r8
  00195e4c: mov r2,r10
  00195e4e: mov r0,r4
  00195e50: blx 0x0006f1cc
  00195e54: add.w r10,sp,#0x44
  00195e58: mov r1,r4
  00195e5a: mov r2,r9
  00195e5c: mov r0,r10
  00195e5e: blx 0x0006f1cc
  00195e62: adds r2,r6,#0x2
  00195e64: mov r0,r11
  00195e66: mov r1,r10
  00195e68: mov r3,r5
  00195e6a: blx 0x00078b74
  00195e6e: ldrb.w r1,[r11,#0x74]
  00195e72: movs r2,#0x8
  00195e74: ldr.w r0,[r11,#0x34]
  00195e78: cmp r1,#0x0
  00195e7a: it eq
  00195e7c: mov.eq r2,#0x4
  00195e7e: add r6,r2
  00195e80: lsls r1,r0,#0x12
  00195e82: bpl 0x00195ee6
  00195e84: add r5,sp,#0x38
  00195e86: add.w r10,sp,#0x50
  00195e8a: mov r1,r8
  00195e8c: mov r0,r5
  00195e8e: mov r2,r10
  00195e90: blx 0x0006ec38
  00195e94: add r4,sp,#0x44
  00195e96: mov r1,r5
  00195e98: mov r2,r9
  00195e9a: mov r0,r4
  00195e9c: blx 0x0006f1cc
  00195ea0: add r5,sp,#0x5c
  00195ea2: mov r0,r11
  00195ea4: mov r1,r4
  00195ea6: mov r2,r6
  00195ea8: mov r3,r5
  00195eaa: blx 0x00078b74
  00195eae: add r4,sp,#0x38
  00195eb0: mov r1,r8
  00195eb2: mov r2,r10
  00195eb4: mov r0,r4
  00195eb6: blx 0x0006f1cc
  00195eba: add.w r10,sp,#0x44
  00195ebe: mov r1,r4
  00195ec0: mov r2,r9
  00195ec2: mov r0,r10
  00195ec4: blx 0x0006ec38
  00195ec8: adds r2,r6,#0x2
  00195eca: mov r0,r11
  00195ecc: mov r1,r10
  00195ece: mov r3,r5
  00195ed0: blx 0x00078b74
  00195ed4: ldrb.w r1,[r11,#0x74]
  00195ed8: movs r2,#0x8
  00195eda: ldr.w r0,[r11,#0x34]
  00195ede: cmp r1,#0x0
  00195ee0: it eq
  00195ee2: mov.eq r2,#0x4
  00195ee4: add r6,r2
  00195ee6: ldr r5,[r7,#0x18]
  00195ee8: lsls r0,r0,#0x11
  00195eea: bpl 0x00195f24
  00195eec: add r4,sp,#0x44
  00195eee: add.w r9,sp,#0x5c
  00195ef2: mov r1,r8
  00195ef4: mov r0,r4
  00195ef6: mov r2,r9
  00195ef8: blx 0x0006f1cc
  00195efc: add.w r10,sp,#0x68
  00195f00: mov r0,r11
  00195f02: mov r1,r4
  00195f04: mov r2,r6
  00195f06: mov r3,r10
  00195f08: blx 0x00078b74
  00195f0c: add r4,sp,#0x44
  00195f0e: mov r1,r8
  00195f10: mov r2,r9
  00195f12: mov r0,r4
  00195f14: blx 0x0006ec38
  00195f18: adds r2,r6,#0x2
  00195f1a: mov r0,r11
  00195f1c: mov r1,r4
  00195f1e: mov r3,r10
  00195f20: blx 0x00078b74
  00195f24: ldr.w r8,[sp,#0x20]
  00195f28: cmp r5,#0x0
  00195f2a: ldr r0,[r7,#0x28]
  00195f2c: mov r1,r8
  00195f2e: beq 0x00195f48
  00195f30: cmp r0,#0x0
  00195f32: mov r1,r8
  00195f34: bne 0x00195f48
  00195f36: ldrb.w r1,[r11,#0x45]
  00195f3a: movs r2,#0xff
  00195f3c: cmp r1,#0x0
  00195f3e: it eq
  00195f40: mvn.eq r2,#0xff
  00195f44: and.w r1,r2,r8
  00195f48: vldr.32 s0,[r7,#0x10]
  00195f4c: cmp r0,#0x0
  00195f4e: vldr.32 s2,[r7,#0x8]
  00195f52: str r1,[sp,#0x1c]
  00195f54: beq 0x00195f68
  00195f56: ldrb.w r0,[r11,#0x45]
  00195f5a: movs r1,#0xff
  00195f5c: cmp r0,#0x0
  00195f5e: it eq
  00195f60: mvn.eq r1,#0xff
  00195f64: and.w r8,r8,r1
  00195f68: ldr.w r1,[r11,#0x50]
  00195f6c: movs r3,#0x0
  00195f6e: ldr.w r0,[r11,#0x9c]
  00195f72: ldr.w r2,[r11,#0x58]
  00195f76: vldr.32 s16,[r7,#0x14]
  00195f7a: muls r1,r0
  00195f7c: vldr.32 s18,[r7,#0xc]
  00195f80: add.w r10,r2,r1, lsl #0x2
  00195f84: ldr r1,[0x00196084]
  00195f86: add r1,pc
  00195f88: ldr r1,[r1,#0x0]
  00195f8a: str r1,[sp,#0x18]
  00195f8c: vmov r1,s2
  00195f90: str r1,[sp,#0x14]
  00195f92: ldr r1,[0x00196088]
  00195f94: str.w r8,[sp,#0x20]
  00195f98: add r1,pc
  00195f9a: ldr r1,[r1,#0x0]
  00195f9c: str r1,[sp,#0x10]
  00195f9e: vmov r1,s0
  00195fa2: str r1,[sp,#0xc]
  00195fa4: b 0x0019605a
  00195fa6: uxth.w r2,r10
  00195faa: ldr.w r0,[r11,#0x8]
  00195fae: ldr.w r1,[r11,#0x54]
  00195fb2: mov r5,r11
  00195fb4: strd r2,r3,[sp,#0x24]
  00195fb8: mov r3,r8
  00195fba: ldr r4,[sp,#0x18]
  00195fbc: blx r4
  00195fbe: add.w r2,r10,#0x1
  00195fc2: ldr.w r0,[r11,#0x8]
  00195fc6: ldr.w r1,[r11,#0x54]
  00195fca: mov r3,r8
  00195fcc: uxth r6,r2
  00195fce: mov r2,r6
  00195fd0: blx r4
  00195fd2: add.w r2,r10,#0x2
  00195fd6: ldr.w r0,[r11,#0x8]
  00195fda: ldr.w r1,[r11,#0x54]
  00195fde: ldr.w r11,[sp,#0x1c]
  00195fe2: uxth.w r9,r2
  00195fe6: mov r2,r9
  00195fe8: mov r3,r11
  00195fea: blx r4
  00195fec: add.w r2,r10,#0x3
  00195ff0: ldr r0,[r5,#0x8]
  00195ff2: ldr r1,[r5,#0x54]
  00195ff4: mov r3,r11
  00195ff6: uxth.w r8,r2
  00195ffa: mov r2,r8
  00195ffc: blx r4
  00195ffe: ldr r4,[sp,#0x14]
  00196000: mov r11,r5
  00196002: ldr r0,[r5,#0x8]
  00196004: ldr r1,[r5,#0x54]
  00196006: ldr r2,[sp,#0x24]
  00196008: mov r3,r4
  0019600a: ldr r5,[sp,#0x10]
  0019600c: vstr.32 s18,[sp]
  00196010: blx r5
  00196012: mov r2,r6
  00196014: ldr r6,[sp,#0xc]
  00196016: ldr.w r0,[r11,#0x8]
  0019601a: ldr.w r1,[r11,#0x54]
  0019601e: mov r3,r6
  00196020: vstr.32 s18,[sp]
  00196024: blx r5
  00196026: ldr.w r0,[r11,#0x8]
  0019602a: mov r2,r9
  0019602c: ldr.w r1,[r11,#0x54]
  00196030: mov r3,r4
  00196032: vstr.32 s16,[sp]
  00196036: blx r5
  00196038: ldr.w r0,[r11,#0x8]
  0019603c: mov r2,r8
  0019603e: ldr.w r1,[r11,#0x54]
  00196042: mov r3,r6
  00196044: vstr.32 s16,[sp]
  00196048: ldr.w r8,[sp,#0x20]
  0019604c: blx r5
  0019604e: ldr r3,[sp,#0x28]
  00196050: add.w r10,r10,#0x4
  00196054: ldr.w r0,[r11,#0x9c]
  00196058: adds r3,#0x1
  0019605a: cmp r3,r0
  0019605c: blt 0x00195fa6
  0019605e: ldr r0,[sp,#0x74]
  00196060: ldr r1,[sp,#0x8]
  00196062: ldr r1,[r1,#0x0]
  00196064: subs r0,r1,r0
  00196066: itttt eq
  00196068: add.eq sp,#0x78
  0019606a: vpop.eq {d8,d9,d10}
  0019606e: add.eq sp,#0x4
  00196070: pop.eq.w {r8,r9,r10,r11}
  00196074: it eq
  00196076: pop.eq {r4,r5,r6,r7,pc}
  00196078: blx 0x0006e824
```
