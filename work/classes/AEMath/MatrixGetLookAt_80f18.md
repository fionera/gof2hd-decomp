# AbyssEngine::AEMath::MatrixGetLookAt
elf 0x80f18 body 166
Sig: undefined __thiscall MatrixGetLookAt(AEMath * this, Vector * param_1, Vector * param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetLookAt(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) */

void __thiscall
AbyssEngine::AEMath::MatrixGetLookAt(AEMath *this,Vector *param_1,Vector *param_2,Vector *param_3)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  int local_28;
  
  piVar1 = *(int **)(DAT_00090fc0 + 0x90f2e);
  local_28 = *piVar1;
  operator-((Vector *)&local_40,param_1);
  VectorNormalize((AEMath *)&uStack_34,(Vector *)&local_40);
  VectorCross((Vector *)&local_4c,param_3);
  VectorNormalize((AEMath *)&local_40,(Vector *)&local_4c);
  VectorCross((Vector *)&local_4c,(Vector *)&uStack_34);
  uVar2 = *(undefined4 *)param_1;
  uVar4 = *(undefined4 *)(param_1 + 4);
  uVar3 = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)this = local_40;
  *(undefined4 *)(this + 4) = local_4c;
  *(undefined4 *)(this + 8) = uStack_34;
  *(undefined4 *)(this + 0xc) = uVar2;
  *(undefined4 *)(this + 0x10) = uStack_3c;
  *(undefined4 *)(this + 0x14) = uStack_48;
  *(undefined4 *)(this + 0x18) = local_30;
  *(undefined4 *)(this + 0x1c) = uVar4;
  *(undefined4 *)(this + 0x20) = local_38;
  *(undefined4 *)(this + 0x24) = uStack_44;
  *(undefined4 *)(this + 0x28) = uStack_2c;
  *(undefined4 *)(this + 0x2c) = uVar3;
  *(undefined4 *)(this + 0x30) = 0x3f800000;
  *(undefined4 *)(this + 0x34) = 0x3f800000;
  *(undefined4 *)(this + 0x38) = 0x3f800000;
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00090f18: push {r4,r5,r6,r7,lr}
  00090f1a: add r7,sp,#0xc
  00090f1c: push {r8,r9,r10,r11}
  00090f20: sub sp,#0x2c
  00090f22: mov r4,r0
  00090f24: ldr r0,[0x00090fc0]
  00090f26: add r6,sp,#0x10
  00090f28: mov r8,r3
  00090f2a: add r0,pc
  00090f2c: mov r5,r1
  00090f2e: ldr r0,[r0,#0x0]
  00090f30: str r0,[sp,#0x0]
  00090f32: ldr r0,[r0,#0x0]
  00090f34: str r0,[sp,#0x28]
  00090f36: mov r0,r6
  00090f38: blx 0x0006ec38
  00090f3c: add.w r9,sp,#0x1c
  00090f40: mov r1,r6
  00090f42: mov r0,r9
  00090f44: blx 0x0006ec80
  00090f48: add r6,sp,#0x4
  00090f4a: mov r1,r8
  00090f4c: mov r2,r9
  00090f4e: mov r0,r6
  00090f50: blx 0x0006ec8c
  00090f54: add.w r8,sp,#0x10
  00090f58: mov r1,r6
  00090f5a: mov r0,r8
  00090f5c: blx 0x0006ec80
  00090f60: add r0,sp,#0x4
  00090f62: mov r1,r9
  00090f64: mov r2,r8
  00090f66: blx 0x0006ec8c
  00090f6a: add.w r12,sp,#0x4
  00090f6e: ldrd r9,lr,[r5,#0x0]
  00090f72: ldr.w r10,[r5,#0x8]
  00090f76: ldrd r11,r8,[sp,#0x20]
  00090f7a: ldrd r2,r1,[sp,#0x10]
  00090f7e: ldrd r5,r3,[sp,#0x18]
  00090f82: ldm.w r12,{r0,r6,r12}
  00090f86: strd r2,r0,[r4,#0x0]
  00090f8a: add.w r0,r4,#0x10
  00090f8e: strd r3,r9,[r4,#0x8]
  00090f92: stm r0,{r1,r6,r11,lr}
  00090f96: mov.w r0,#0x3f800000
  00090f9a: strd r5,r12,[r4,#0x20]
  00090f9e: strd r8,r10,[r4,#0x28]
  00090fa2: strd r0,r0,[r4,#0x30]
  00090fa6: str r0,[r4,#0x38]
  00090fa8: ldr r0,[sp,#0x28]
  00090faa: ldr r1,[sp,#0x0]
  00090fac: ldr r1,[r1,#0x0]
  00090fae: subs r0,r1,r0
  00090fb0: ittt eq
  00090fb2: add.eq sp,#0x2c
  00090fb4: pop.eq.w {r8,r9,r10,r11}
  00090fb8: pop.eq {r4,r5,r6,r7,pc}
  00090fba: blx 0x0006e824
```
