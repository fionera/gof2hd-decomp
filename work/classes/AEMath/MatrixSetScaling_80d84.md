# AbyssEngine::AEMath::MatrixSetScaling
elf 0x80d84 body 192
Sig: undefined __stdcall MatrixSetScaling(Matrix * param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetScaling(AbyssEngine::AEMath::Matrix&, float, float, float) */

void AbyssEngine::AEMath::MatrixSetScaling
               (Matrix *param_1,float param_2,float param_3,float param_4)

{
  float *in_r1;
  float in_r2;
  float in_r3;
  int *piVar1;
  float in_stack_00000000;
  Vector local_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00090e44 + 0x90d9e);
  local_1c = *piVar1;
  *in_r1 = *in_r1 * in_r2;
  in_r1[1] = in_r1[1] * in_r3;
  in_r1[2] = in_r1[2] * in_stack_00000000;
  in_r1[4] = in_r1[4] * in_r2;
  in_r1[5] = in_r1[5] * in_r3;
  in_r1[6] = in_r1[6] * in_stack_00000000;
  in_r1[8] = in_r1[8] * in_r2;
  in_r1[9] = in_r1[9] * in_r3;
  in_r1[10] = in_r1[10] * in_stack_00000000;
  Vector::operator=((Vector *)(in_r1 + 0xc),local_28);
  __aeabi_memcpy(param_1);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00090d84: push {r4,r5,r6,r7,lr}
  00090d86: add r7,sp,#0xc
  00090d88: push.w r11
  00090d8c: sub sp,#0x10
  00090d8e: mov r5,r0
  00090d90: ldr r0,[0x00090e44]
  00090d92: vmov s0,r3
  00090d96: vldr.32 s12,[r7,#0x8]
  00090d9a: add r0,pc
  00090d9c: vmov s10,r2
  00090da0: mov r4,r1
  00090da2: ldr r6,[r0,#0x0]
  00090da4: ldr r0,[r6,#0x0]
  00090da6: str r0,[sp,#0xc]
  00090da8: add.w r0,r1,#0x30
  00090dac: vldr.32 s2,[r1]
  00090db0: vldr.32 s8,[r1,#0x10]
  00090db4: vldr.32 s14,[r1,#0x20]
  00090db8: vmul.f32 s2,s2,s10
  00090dbc: vldr.32 s1,[r1,#0x24]
  00090dc0: vmul.f32 s8,s8,s10
  00090dc4: vldr.32 s4,[r1,#0x4]
  00090dc8: vmul.f32 s10,s14,s10
  00090dcc: vldr.32 s6,[r1,#0x8]
  00090dd0: vmul.f32 s14,s1,s0
  00090dd4: vldr.32 s3,[r1,#0x28]
  00090dd8: vmul.f32 s4,s4,s0
  00090ddc: vldr.32 s1,[r1,#0x18]
  00090de0: vmul.f32 s6,s6,s12
  00090de4: vldr.32 s5,[r1,#0x14]
  00090de8: vmul.f32 s3,s3,s12
  00090dec: vmul.f32 s1,s1,s12
  00090df0: vstr.32 s2,[r1]
  00090df4: vmul.f32 s0,s5,s0
  00090df8: vstr.32 s4,[r1,#0x4]
  00090dfc: vstr.32 s6,[r1,#0x8]
  00090e00: vstr.32 s8,[r1,#0x10]
  00090e04: vstr.32 s0,[r1,#0x14]
  00090e08: vstr.32 s1,[r1,#0x18]
  00090e0c: vstr.32 s10,[r1,#0x20]
  00090e10: vstr.32 s14,[r1,#0x24]
  00090e14: vstr.32 s3,[r1,#0x28]
  00090e18: mov r1,sp
  00090e1a: strd r2,r3,[sp,#0x0]
  00090e1e: vstr.32 s12,[sp,#0x8]
  00090e22: blx 0x0006eb3c
  00090e26: mov r0,r5
  00090e28: mov r1,r4
  00090e2a: movs r2,#0x3c
  00090e2c: blx 0x0006f1e4
  00090e30: ldr r0,[sp,#0xc]
  00090e32: ldr r1,[r6,#0x0]
  00090e34: subs r0,r1,r0
  00090e36: ittt eq
  00090e38: add.eq sp,#0x10
  00090e3a: pop.eq.w r11
  00090e3e: pop.eq {r4,r5,r6,r7,pc}
  00090e40: blx 0x0006e824
```
