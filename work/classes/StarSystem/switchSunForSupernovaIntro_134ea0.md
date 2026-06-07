# StarSystem::switchSunForSupernovaIntro
elf 0x134ea0 body 258
Sig: undefined __stdcall switchSunForSupernovaIntro(void)

## decompile
```c

/* StarSystem::switchSunForSupernovaIntro() */

void StarSystem::switchSunForSupernovaIntro(void)

{
  int in_r0;
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  AEGeometry *this;
  code *pcVar4;
  code *pcVar5;
  int *piVar6;
  undefined4 *puVar7;
  float fVar8;
  
  piVar6 = *(int **)(DAT_00144fac + 0x144eb4);
  puVar7 = *(undefined4 **)(DAT_00144fb0 + 0x144eb6);
  iVar1 = *piVar6;
  AbyssEngine::PaintCanvas::TextureCreate
            ((ushort)*puVar7,(uint *)0x2df3,SUB41(*(undefined4 *)(*(int *)(in_r0 + 0x14) + 4),0));
  AbyssEngine::PaintCanvas::TextureCreate
            ((ushort)*puVar7,(uint *)0x2df4,(bool)((char)in_r0 + '\x10'));
  AEGeometry::setMesh(*(AEGeometry **)(in_r0 + 0x40),0x2df2);
  this = *(AEGeometry **)(in_r0 + 0x40);
  fVar8 = (float)AEGeometry::getScaling();
  AbyssEngine::AEMath::operator*((Vector *)&stack0xffffffd0,fVar8);
  AEGeometry::setScaling(this,(Vector *)&stack0xffffffd0);
  fVar8 = (float)AEGeometry::setMesh((AEGeometry *)**(undefined4 **)(*(int *)(in_r0 + 0x1c) + 4),
                                     0x2df1);
  AEGeometry::setScaling(fVar8);
  pcVar4 = *(code **)(DAT_00144fb4 + 0x144f36);
  uVar2 = (*pcVar4)(*puVar7,*(undefined4 *)(**(int **)(*(int *)(in_r0 + 0x1c) + 4) + 0xc));
  pcVar5 = *(code **)(DAT_00144fb8 + 0x144f46);
  (*pcVar5)(uVar2,0,0);
  uVar2 = (*pcVar4)(*puVar7,*(undefined4 *)(**(int **)(*(int *)(in_r0 + 0x1c) + 4) + 0xc));
  (*pcVar5)(uVar2,3,0);
  uVar2 = (*pcVar4)(*puVar7,*(undefined4 *)(**(int **)(*(int *)(in_r0 + 0x1c) + 4) + 0xc));
  (*pcVar5)(uVar2,1,0);
  Status::getSystem();
  iVar3 = SolarSystem::getIndex();
  *(undefined4 *)(in_r0 + 0x3c) = *(undefined4 *)(DAT_00144fc0 + 0x144f88 + iVar3 * 4);
  if (*piVar6 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00144ea0: push {r4,r5,r6,r7,lr}
  00144ea2: add r7,sp,#0xc
  00144ea4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10}
  00144ea8: mov r4,r0
  00144eaa: ldr r0,[0x00144fac]
  00144eac: ldr r1,[0x00144fb0]
  00144eae: movs r3,#0x0
  00144eb0: add r0,pc
  00144eb2: add r1,pc
  00144eb4: ldr.w r9,[r0,#0x0]
  00144eb8: ldr.w r10,[r1,#0x0]
  00144ebc: ldr.w r0,[r9,#0x0]
  00144ec0: str r0,[sp,#0x1c]
  00144ec2: ldr r1,[r4,#0x14]
  00144ec4: ldr.w r0,[r10,#0x0]
  00144ec8: ldr r2,[r1,#0x4]
  00144eca: movw r1,#0x2df3
  00144ece: blx 0x0006fbb0
  00144ed2: ldr.w r0,[r10,#0x0]
  00144ed6: add.w r2,r4,#0x10
  00144eda: movw r1,#0x2df4
  00144ede: movs r3,#0x0
  00144ee0: blx 0x0006fbb0
  00144ee4: ldr r0,[r4,#0x40]
  00144ee6: movw r1,#0x2df2
  00144eea: blx 0x00076a14
  00144eee: ldr r5,[r4,#0x40]
  00144ef0: add.w r8,sp,#0x4
  00144ef4: mov r0,r8
  00144ef6: mov r1,r5
  00144ef8: blx 0x00072760
  00144efc: add r6,sp,#0x10
  00144efe: ldr r2,[0x00144fa4]
  00144f00: mov r1,r8
  00144f02: mov r0,r6
  00144f04: blx 0x0006ec74
  00144f08: mov r0,r5
  00144f0a: mov r1,r6
  00144f0c: blx 0x00073de0
  00144f10: ldr r0,[r4,#0x1c]
  00144f12: movw r1,#0x2df1
  00144f16: ldr r0,[r0,#0x4]
  00144f18: ldr r0,[r0,#0x0]
  00144f1a: blx 0x00076a14
  00144f1e: ldr r0,[r4,#0x1c]
  00144f20: ldr r1,[0x00144fa8]
  00144f22: ldr r0,[r0,#0x4]
  00144f24: ldr r0,[r0,#0x0]
  00144f26: blx 0x00076a20
  00144f2a: ldr r0,[r4,#0x1c]
  00144f2c: ldr r0,[r0,#0x4]
  00144f2e: ldr r2,[0x00144fb4]
  00144f30: ldr r1,[r0,#0x0]
  00144f32: add r2,pc
  00144f34: ldr.w r0,[r10,#0x0]
  00144f38: ldr r5,[r2,#0x0]
  00144f3a: ldr r1,[r1,#0xc]
  00144f3c: blx r5
  00144f3e: ldr r1,[0x00144fb8]
  00144f40: movs r2,#0x0
  00144f42: add r1,pc
  00144f44: ldr r6,[r1,#0x0]
  00144f46: movs r1,#0x0
  00144f48: blx r6
  00144f4a: ldr r0,[r4,#0x1c]
  00144f4c: ldr r0,[r0,#0x4]
  00144f4e: ldr r1,[r0,#0x0]
  00144f50: ldr.w r0,[r10,#0x0]
  00144f54: ldr r1,[r1,#0xc]
  00144f56: blx r5
  00144f58: movs r1,#0x3
  00144f5a: movs r2,#0x0
  00144f5c: blx r6
  00144f5e: ldr r0,[r4,#0x1c]
  00144f60: ldr r0,[r0,#0x4]
  00144f62: ldr r1,[r0,#0x0]
  00144f64: ldr.w r0,[r10,#0x0]
  00144f68: ldr r1,[r1,#0xc]
  00144f6a: blx r5
  00144f6c: movs r1,#0x1
  00144f6e: movs r2,#0x0
  00144f70: blx r6
  00144f72: ldr r0,[0x00144fbc]
  00144f74: add r0,pc
  00144f76: ldr r0,[r0,#0x0]
  00144f78: ldr r0,[r0,#0x0]
  00144f7a: blx 0x00071a10
  00144f7e: blx 0x00071a7c
  00144f82: ldr r1,[0x00144fc0]
  00144f84: add r1,pc
  00144f86: ldr.w r0,[r1,r0,lsl #0x2]
  00144f8a: str r0,[r4,#0x3c]
  00144f8c: ldr r0,[sp,#0x1c]
  00144f8e: ldr.w r1,[r9,#0x0]
  00144f92: subs r0,r1,r0
  00144f94: ittt eq
  00144f96: add.eq sp,#0x20
  00144f98: pop.eq.w {r8,r9,r10}
  00144f9c: pop.eq {r4,r5,r6,r7,pc}
  00144f9e: blx 0x0006e824
```
