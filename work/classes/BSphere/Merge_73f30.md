# BSphere::Merge
elf 0x73f30 body 202
Sig: undefined __thiscall Merge(BSphere * this, Transform * param_1)

## decompile
```c

/* AbyssEngine::AEMath::BSphere::Merge(AbyssEngine::Transform const&) */

void __thiscall AbyssEngine::AEMath::BSphere::Merge(BSphere *this,Transform *param_1)

{
  int *piVar1;
  float fVar2;
  float fVar3;
  float local_48;
  float fStack_44;
  float fStack_40;
  float local_3c;
  float fStack_38;
  float local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  float local_24;
  undefined4 local_20;
  int local_1c;
  
  piVar1 = *(int **)(DAT_00083ffc + 0x83f48);
  local_1c = *piVar1;
  local_20 = 0x3f800000;
  MatrixTransformVector((Matrix *)&local_3c,(Vector *)param_1);
  local_30 = local_3c;
  uStack_2c = fStack_38;
  uStack_28 = local_34;
  local_3c = *(float *)(param_1 + 0xe0);
  fStack_38 = local_3c;
  local_34 = local_3c;
  MatrixRotateVector((Matrix *)&local_48,(Vector *)param_1);
  fVar2 = -local_48;
  if (0.0 < local_48) {
    fVar2 = local_48;
  }
  fVar3 = -fStack_44;
  if (0.0 < fStack_44) {
    fVar3 = fStack_44;
  }
  local_24 = -fStack_40;
  if (0.0 < fStack_40) {
    local_24 = fStack_40;
  }
  if (fVar3 < fVar2) {
    fVar3 = fVar2;
  }
  if (local_24 < fVar3) {
    local_24 = fVar3;
  }
  local_3c = local_48;
  fStack_38 = fStack_44;
  local_34 = fStack_40;
  Merge(this,(BSphere *)&local_30);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00083f30: push {r4,r5,r6,r7,lr}
  00083f32: add r7,sp,#0xc
  00083f34: push.w r11
  00083f38: sub sp,#0x30
  00083f3a: mov r4,r0
  00083f3c: ldr r0,[0x00083ffc]
  00083f3e: add.w r2,r1,#0xd4
  00083f42: mov r5,r1
  00083f44: add r0,pc
  00083f46: ldr r6,[r0,#0x0]
  00083f48: ldr r0,[r6,#0x0]
  00083f4a: str r0,[sp,#0x2c]
  00083f4c: mov.w r0,#0x3f800000
  00083f50: str r0,[sp,#0x28]
  00083f52: add r0,sp,#0xc
  00083f54: blx 0x0006f688
  00083f58: add r2,sp,#0xc
  00083f5a: add r3,sp,#0x18
  00083f5c: ldmia r2,{r0,r1,r2}
  00083f5e: stmia r3!,{r0,r1,r2}
  00083f60: add r2,sp,#0xc
  00083f62: mov r1,r5
  00083f64: ldr.w r0,[r5,#0xe0]
  00083f68: strd r0,r0,[sp,#0xc]
  00083f6c: str r0,[sp,#0x14]
  00083f6e: mov r0,sp
  00083f70: blx 0x0006f694
  00083f74: ldm.w sp,{r0,r1,r2}
  00083f78: add r3,sp,#0xc
  00083f7a: vmov s0,r0
  00083f7e: vmov s2,r1
  00083f82: vcmpe.f32 s0,#0
  00083f86: vmrs apsr,fpscr
  00083f8a: vneg.f32 s4,s0
  00083f8e: vcmpe.f32 s2,#0
  00083f92: vmov s8,r2
  00083f96: vneg.f32 s6,s2
  00083f9a: vneg.f32 s10,s8
  00083f9e: it gt
  00083fa0: vmov.gt.f32 s4,s0
  00083fa4: vmrs apsr,fpscr
  00083fa8: vcmpe.f32 s8,#0
  00083fac: it gt
  00083fae: vmov.gt.f32 s6,s2
  00083fb2: vmrs apsr,fpscr
  00083fb6: vcmpe.f32 s4,s6
  00083fba: it gt
  00083fbc: vmov.gt.f32 s10,s8
  00083fc0: vmrs apsr,fpscr
  00083fc4: it gt
  00083fc6: vmov.gt.f32 s6,s4
  00083fca: vcmpe.f32 s6,s10
  00083fce: vmrs apsr,fpscr
  00083fd2: it gt
  00083fd4: vmov.gt.f32 s10,s6
  00083fd8: stmia r3!,{r0,r1,r2}
  00083fda: add r1,sp,#0x18
  00083fdc: mov r0,r4
  00083fde: vstr.32 s10,[sp,#0x24]
  00083fe2: blx 0x0006eca4
  00083fe6: ldr r0,[sp,#0x2c]
  00083fe8: ldr r1,[r6,#0x0]
  00083fea: subs r0,r1,r0
  00083fec: ittt eq
  00083fee: add.eq sp,#0x30
  00083ff0: pop.eq.w r11
  00083ff4: pop.eq {r4,r5,r6,r7,pc}
  00083ff6: blx 0x0006e824
```
