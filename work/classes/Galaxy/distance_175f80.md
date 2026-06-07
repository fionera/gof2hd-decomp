# Galaxy::distance
elf 0x175f80 body 264
Sig: undefined __stdcall distance(SolarSystem * param_1, SolarSystem * param_2)

## decompile
```c

/* Galaxy::distance(SolarSystem*, SolarSystem*) */

void Galaxy::distance(SolarSystem *param_1,SolarSystem *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  SolarSystem *in_r2;
  undefined4 in_r3;
  int *piVar7;
  uint in_fpscr;
  undefined4 uVar8;
  undefined4 extraout_s1;
  SolarSystem *local_3c;
  SolarSystem *local_38;
  undefined4 local_34;
  
  piVar7 = *(int **)(DAT_00186090 + 0x185f92);
  iVar1 = *piVar7;
  local_3c = param_2;
  local_38 = in_r2;
  local_34 = in_r3;
  iVar2 = SolarSystem::getIndex();
  iVar3 = SolarSystem::getIndex();
  if (iVar2 != iVar3) {
    uVar4 = SolarSystem::getX(param_2);
    uVar5 = SolarSystem::getY(param_2);
    uVar6 = SolarSystem::getZ(param_2);
    uVar6 = __aeabi_idiv(uVar6,10);
    uVar8 = VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
    VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
    uVar4 = SolarSystem::getX(in_r2);
    uVar5 = SolarSystem::getY(in_r2);
    uVar6 = SolarSystem::getZ(in_r2);
    uVar6 = __aeabi_idiv(uVar6,10);
    local_3c = (SolarSystem *)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    local_38 = (SolarSystem *)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
    local_34 = VectorSignedToFloat(uVar6,(byte)(in_fpscr >> 0x16) & 3);
    AbyssEngine::AEMath::Vector::operator-=((Vector *)&stack0xffffffd0,(Vector *)&local_3c);
    Globals::sqrt((Globals *)**(undefined4 **)(DAT_00186094 + 0x186050),
                  (double)CONCAT44(extraout_s1,uVar8));
  }
  iVar1 = *piVar7 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  00185f80: push {r4,r5,r6,r7,lr}
  00185f82: add r7,sp,#0xc
  00185f84: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  00185f88: ldr r0,[0x00186090]
  00185f8a: mov r4,r2
  00185f8c: mov r5,r1
  00185f8e: add r0,pc
  00185f90: ldr.w r9,[r0,#0x0]
  00185f94: ldr.w r0,[r9,#0x0]
  00185f98: str r0,[sp,#0x1c]
  00185f9a: mov r0,r1
  00185f9c: blx 0x00071a7c
  00185fa0: mov r6,r0
  00185fa2: mov r0,r4
  00185fa4: blx 0x00071a7c
  00185fa8: cmp r6,r0
  00185faa: bne 0x00185fb2
  00185fac: vldr.32 s0,[pc,#0xd8]
  00185fb0: b 0x0018606e
  00185fb2: mov r0,r5
  00185fb4: blx 0x00071968
  00185fb8: mov r8,r0
  00185fba: mov r0,r5
  00185fbc: blx 0x00071974
  00185fc0: mov r6,r0
  00185fc2: mov r0,r5
  00185fc4: blx 0x00074548
  00185fc8: movs r1,#0xa
  00185fca: blx 0x0007198c
  00185fce: vmov s0,r8
  00185fd2: vmov s4,r0
  00185fd6: mov r0,r4
  00185fd8: vmov s2,r6
  00185fdc: vcvt.f32.s32 s0,s0
  00185fe0: vcvt.f32.s32 s2,s2
  00185fe4: vcvt.f32.s32 s4,s4
  00185fe8: vstr.32 s0,[sp,#0x10]
  00185fec: vstr.32 s2,[sp,#0x14]
  00185ff0: vstr.32 s4,[sp,#0x18]
  00185ff4: blx 0x00071968
  00185ff8: mov r5,r0
  00185ffa: mov r0,r4
  00185ffc: blx 0x00071974
  00186000: mov r6,r0
  00186002: mov r0,r4
  00186004: blx 0x00074548
  00186008: movs r1,#0xa
  0018600a: blx 0x0007198c
  0018600e: vmov s0,r5
  00186012: add r1,sp,#0x4
  00186014: vmov s4,r0
  00186018: add r0,sp,#0x10
  0018601a: vmov s2,r6
  0018601e: vcvt.f32.s32 s0,s0
  00186022: vcvt.f32.s32 s2,s2
  00186026: vcvt.f32.s32 s4,s4
  0018602a: vstr.32 s0,[sp,#0x4]
  0018602e: vstr.32 s2,[sp,#0x8]
  00186032: vstr.32 s4,[sp,#0xc]
  00186036: blx 0x00072694
  0018603a: vldr.32 s2,[sp,#0x14]
  0018603e: vldr.32 s0,[sp,#0x10]
  00186042: vmul.f32 s2,s2,s2
  00186046: vldr.32 s4,[sp,#0x18]
  0018604a: ldr r0,[0x00186094]
  0018604c: add r0,pc
  0018604e: ldr r0,[r0,#0x0]
  00186050: vmla.f32 s2,s0,s0
  00186054: ldr r0,[r0,#0x0]
  00186056: vmla.f32 s2,s4,s4
  0018605a: vmov r1,s2
  0018605e: blx 0x000760e4
  00186062: vldr.32 s0,[pc,#0x28]
  00186066: vmov s2,r0
  0018606a: vmul.f32 s0,s2,s0
  0018606e: ldr r0,[sp,#0x1c]
  00186070: ldr.w r1,[r9,#0x0]
  00186074: subs r0,r1,r0
  00186076: itttt eq
  00186078: vmov.eq r0,s0
  0018607c: add.eq sp,#0x20
  0018607e: pop.eq.w {r8,r9,r11}
  00186082: pop.eq {r4,r5,r6,r7,pc}
  00186084: blx 0x0006e824
```
