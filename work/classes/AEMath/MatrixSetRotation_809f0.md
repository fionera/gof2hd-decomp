# AbyssEngine::AEMath::MatrixSetRotation
elf 0x809f0 body 704
Sig: undefined __stdcall MatrixSetRotation(Matrix * param_1, float param_2, float param_3, float param_4, RotationOrder param_5)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, float, float, float,
   AbyssEngine::AEMath::RotationOrder) */

void AbyssEngine::AEMath::MatrixSetRotation
               (undefined4 param_1,float *param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  code *pcVar7;
  code *pcVar8;
  
  pcVar7 = *(code **)(DAT_00090cbc + 0x90a06);
  fVar1 = (float)(*pcVar7)(param_3);
  pcVar8 = *(code **)(DAT_00090cc0 + 0x90a16);
  fVar2 = (float)(*pcVar7)(param_4);
  fVar3 = (float)(*pcVar7)(param_5);
  fVar4 = (float)(*pcVar8)(param_3);
  fVar5 = (float)(*pcVar8)(param_4);
  fVar6 = (float)(*pcVar8)(param_5);
  switch(param_6) {
  case 0:
    *param_2 = fVar5 * fVar6;
    param_2[1] = -(fVar3 * fVar5);
    param_2[2] = fVar2;
    param_2[8] = fVar1 * fVar3 - fVar2 * fVar4 * fVar6;
    param_2[9] = fVar1 * fVar6 + fVar3 * fVar2 * fVar4;
    param_2[10] = fVar4 * fVar5;
    param_2[4] = fVar2 * fVar1 * fVar6 + fVar3 * fVar4;
    param_2[5] = fVar4 * fVar6 - fVar1 * fVar2 * fVar3;
    param_2[6] = -(fVar1 * fVar5);
    break;
  case 1:
    *param_2 = fVar5 * fVar6;
    param_2[1] = -fVar3;
    param_2[2] = fVar2 * fVar6;
    param_2[4] = fVar1 * fVar2 + fVar3 * fVar4 * fVar5;
    param_2[5] = fVar4 * fVar6;
    param_2[6] = fVar3 * fVar2 * fVar4 - fVar1 * fVar5;
    param_2[8] = fVar3 * fVar1 * fVar5 - fVar2 * fVar4;
    param_2[9] = fVar1 * fVar6;
    param_2[10] = fVar4 * fVar5 + fVar1 * fVar2 * fVar3;
    break;
  case 2:
    param_2[4] = fVar3 * fVar4;
    param_2[5] = fVar4 * fVar6;
    param_2[6] = -fVar1;
    *param_2 = fVar5 * fVar6 + fVar1 * fVar2 * fVar3;
    param_2[1] = fVar2 * fVar1 * fVar6 - fVar3 * fVar5;
    param_2[2] = fVar2 * fVar4;
    param_2[8] = fVar3 * fVar1 * fVar5 - fVar2 * fVar6;
    param_2[9] = fVar2 * fVar3 + fVar1 * fVar5 * fVar6;
    param_2[10] = fVar4 * fVar5;
    break;
  case 3:
    param_2[4] = fVar3;
    param_2[5] = fVar4 * fVar6;
    param_2[6] = -(fVar1 * fVar6);
    param_2[8] = -(fVar2 * fVar6);
    param_2[9] = fVar1 * fVar5 + fVar4 * fVar2 * fVar3;
    param_2[10] = fVar4 * fVar5 - fVar1 * fVar2 * fVar3;
    *param_2 = fVar5 * fVar6;
    param_2[1] = fVar1 * fVar2 - fVar4 * fVar3 * fVar5;
    param_2[2] = fVar1 * fVar3 * fVar5 + fVar2 * fVar4;
    break;
  case 4:
    param_2[8] = -(fVar2 * fVar4);
    param_2[9] = fVar1;
    param_2[10] = fVar4 * fVar5;
    *param_2 = fVar5 * fVar6 - fVar1 * fVar2 * fVar3;
    param_2[1] = -(fVar3 * fVar4);
    param_2[2] = fVar2 * fVar6 + fVar1 * fVar3 * fVar5;
    param_2[4] = fVar2 * fVar1 * fVar6 + fVar3 * fVar5;
    param_2[5] = fVar4 * fVar6;
    param_2[6] = fVar2 * fVar3 - fVar1 * fVar5 * fVar6;
    break;
  case 5:
    param_2[8] = -fVar2;
    param_2[9] = fVar1 * fVar5;
    param_2[10] = fVar4 * fVar5;
    param_2[4] = fVar3 * fVar5;
    param_2[5] = fVar4 * fVar6 + fVar1 * fVar2 * fVar3;
    param_2[6] = fVar4 * fVar2 * fVar3 - fVar1 * fVar6;
    *param_2 = fVar5 * fVar6;
    param_2[1] = fVar2 * fVar1 * fVar6 - fVar3 * fVar4;
    param_2[2] = fVar2 * fVar4 * fVar6 + fVar1 * fVar3;
  }
  __aeabi_memcpy(param_1,param_2,0x3c);
  return;
}

```
## target disasm
```
  000909f0: push {r4,r5,r6,r7,lr}
  000909f2: add r7,sp,#0xc
  000909f4: push {r5,r6,r7,r8,r9,r10,r11}
  000909f8: str r0,[sp,#0x8]
  000909fa: mov r4,r3
  000909fc: ldr r0,[0x00090cbc]
  000909fe: mov r5,r2
  00090a00: mov r11,r1
  00090a02: add r0,pc
  00090a04: ldr r6,[r0,#0x0]
  00090a06: mov r0,r2
  00090a08: blx r6
  00090a0a: str r0,[sp,#0x4]
  00090a0c: ldr r0,[0x00090cc0]
  00090a0e: ldr.w r9,[r7,#0x8]
  00090a12: add r0,pc
  00090a14: ldr.w r8,[r0,#0x0]
  00090a18: mov r0,r4
  00090a1a: blx r6
  00090a1c: mov r10,r0
  00090a1e: mov r0,r9
  00090a20: blx r6
  00090a22: mov r6,r0
  00090a24: mov r0,r5
  00090a26: blx r8
  00090a28: mov r5,r0
  00090a2a: mov r0,r4
  00090a2c: blx r8
  00090a2e: mov r4,r0
  00090a30: mov r0,r9
  00090a32: blx r8
  00090a34: ldr r1,[r7,#0xc]
  00090a36: cmp r1,#0x5
  00090a38: bhi.w 0x00090cac
  00090a3c: ldr r2,[sp,#0x4]
  00090a3e: vmov s10,r0
  00090a42: vmov s2,r10
  00090a46: vmov s0,r6
  00090a4a: vmov s4,r2
  00090a4e: vmov s6,r5
  00090a52: vmov s8,r4
  00090a56: tbh [pc,r1]
  00090a66: vmul.f32 s12,s4,s10
  00090a6a: vmul.f32 s1,s6,s10
  00090a6e: vmul.f32 s14,s4,s2
  00090a72: vmul.f32 s3,s2,s6
  00090a76: vmul.f32 s5,s4,s0
  00090a7a: vmul.f32 s10,s8,s10
  00090a7e: vnmul.f32 s7,s0,s8
  00090a82: vmul.f32 s9,s2,s12
  00090a86: vnmul.f32 s4,s4,s8
  00090a8a: vmul.f32 s8,s6,s8
  00090a8e: vmls.f32 s5,s2,s1
  00090a92: vmls.f32 s1,s14,s0
  00090a96: vmla.f32 s12,s0,s3
  00090a9a: vstr.32 s10,[r11]
  00090a9e: vmla.f32 s9,s0,s6
  00090aa2: vstr.32 s7,[r11,#0x4]
  00090aa6: vstr.32 s2,[r11,#0x8]
  00090aaa: vstr.32 s5,[r11,#0x20]
  00090aae: vstr.32 s12,[r11,#0x24]
  00090ab2: vstr.32 s8,[r11,#0x28]
  00090ab6: vstr.32 s9,[r11,#0x10]
  00090aba: vstr.32 s1,[r11,#0x14]
  00090abe: vstr.32 s4,[r11,#0x18]
  00090ac2: b 0x00090cac
  00090ac4: vmul.f32 s3,s4,s8
  00090ac8: vmul.f32 s1,s4,s2
  00090acc: vmul.f32 s12,s6,s8
  00090ad0: vmul.f32 s14,s2,s6
  00090ad4: vmul.f32 s8,s8,s10
  00090ad8: vmul.f32 s2,s2,s10
  00090adc: vmul.f32 s6,s6,s10
  00090ae0: vmov.f32 s7,s3
  00090ae4: vmul.f32 s4,s4,s10
  00090ae8: vmov.f32 s10,s1
  00090aec: vneg.f32 s5,s0
  00090af0: vstr.32 s8,[r11]
  00090af4: vnmls.f32 s7,s0,s14
  00090af8: vnmls.f32 s14,s0,s3
  00090afc: vmla.f32 s10,s0,s12
  00090b00: vmla.f32 s12,s1,s0
  00090b04: vstr.32 s5,[r11,#0x4]
  00090b08: vstr.32 s2,[r11,#0x8]
  00090b0c: vstr.32 s10,[r11,#0x10]
  00090b10: vstr.32 s6,[r11,#0x14]
  00090b14: vstr.32 s7,[r11,#0x18]
  00090b18: vstr.32 s14,[r11,#0x20]
  00090b1c: vstr.32 s4,[r11,#0x24]
  00090b20: vstr.32 s12,[r11,#0x28]
  00090b24: b 0x00090cac
  00090b26: vmul.f32 s12,s4,s10
  00090b2a: vmul.f32 s14,s4,s8
  00090b2e: vmul.f32 s3,s8,s10
  00090b32: vmul.f32 s1,s2,s0
  00090b36: vmul.f32 s7,s0,s6
  00090b3a: vmul.f32 s5,s2,s6
  00090b3e: vmul.f32 s9,s6,s10
  00090b42: vmul.f32 s12,s2,s12
  00090b46: vmul.f32 s14,s0,s14
  00090b4a: vmov.f32 s13,s3
  00090b4e: vneg.f32 s11,s4
  00090b52: vmul.f32 s6,s6,s8
  00090b56: vstr.32 s7,[r11,#0x10]
  00090b5a: vmls.f32 s12,s0,s8
  00090b5e: vstr.32 s9,[r11,#0x14]
  00090b62: vmls.f32 s14,s2,s10
  00090b66: vmla.f32 s13,s4,s1
  00090b6a: vmla.f32 s1,s4,s3
  00090b6e: vstr.32 s11,[r11,#0x18]
  00090b72: vstr.32 s13,[r11]
  00090b76: vstr.32 s12,[r11,#0x4]
  00090b7a: vstr.32 s5,[r11,#0x8]
  00090b7e: vstr.32 s14,[r11,#0x20]
  00090b82: vstr.32 s1,[r11,#0x24]
  00090b86: vstr.32 s6,[r11,#0x28]
  00090b8a: b 0x00090cac
  00090b8c: vmul.f32 s12,s4,s0
  00090b90: vstr.32 s0,[r11,#0x10]
  00090b94: vmul.f32 s14,s0,s8
  00090b98: vmul.f32 s1,s2,s0
  00090b9c: vmul.f32 s3,s4,s2
  00090ba0: vmul.f32 s5,s4,s8
  00090ba4: vmul.f32 s9,s6,s8
  00090ba8: vmul.f32 s7,s8,s10
  00090bac: vmul.f32 s8,s12,s8
  00090bb0: vmul.f32 s11,s6,s10
  00090bb4: vnmul.f32 s4,s4,s10
  00090bb8: vnmul.f32 s10,s2,s10
  00090bbc: vmls.f32 s9,s3,s0
  00090bc0: vmla.f32 s5,s6,s1
  00090bc4: vmls.f32 s3,s6,s14
  00090bc8: vmla.f32 s8,s2,s6
  00090bcc: vstr.32 s11,[r11,#0x14]
  00090bd0: vstr.32 s4,[r11,#0x18]
  00090bd4: vstr.32 s10,[r11,#0x20]
  00090bd8: vstr.32 s5,[r11,#0x24]
  00090bdc: vstr.32 s9,[r11,#0x28]
  00090be0: vstr.32 s7,[r11]
  00090be4: vstr.32 s3,[r11,#0x4]
  00090be8: vstr.32 s8,[r11,#0x8]
  00090bec: b 0x00090cac
  00090bee: vmul.f32 s12,s4,s10
  00090bf2: vmul.f32 s3,s8,s10
  00090bf6: vmul.f32 s14,s2,s0
  00090bfa: vmul.f32 s1,s4,s0
  00090bfe: vmul.f32 s5,s2,s10
  00090c02: vnmul.f32 s7,s0,s6
  00090c06: vmul.f32 s10,s6,s10
  00090c0a: vmul.f32 s12,s2,s12
  00090c0e: vmov.f32 s9,s3
  00090c12: vnmul.f32 s2,s2,s6
  00090c16: vmul.f32 s6,s6,s8
  00090c1a: vmla.f32 s5,s1,s8
  00090c1e: vmla.f32 s12,s0,s8
  00090c22: vmls.f32 s9,s4,s14
  00090c26: vmls.f32 s14,s4,s3
  00090c2a: vstr.32 s2,[r11,#0x20]
  00090c2e: vstr.32 s4,[r11,#0x24]
  00090c32: vstr.32 s6,[r11,#0x28]
  00090c36: vstr.32 s9,[r11]
  00090c3a: vstr.32 s7,[r11,#0x4]
  00090c3e: vstr.32 s5,[r11,#0x8]
  00090c42: vstr.32 s12,[r11,#0x10]
  00090c46: vstr.32 s10,[r11,#0x14]
  00090c4a: vstr.32 s14,[r11,#0x18]
  00090c4e: b 0x00090cac
  00090c50: vmul.f32 s12,s4,s10
  00090c54: vmul.f32 s14,s6,s10
  00090c58: vmul.f32 s1,s2,s0
  00090c5c: vneg.f32 s7,s2
  00090c60: vmul.f32 s10,s8,s10
  00090c64: vmul.f32 s3,s0,s8
  00090c68: vmul.f32 s9,s4,s8
  00090c6c: vmul.f32 s5,s2,s12
  00090c70: vmul.f32 s2,s2,s14
  00090c74: vmul.f32 s8,s6,s8
  00090c78: vmla.f32 s14,s4,s1
  00090c7c: vstr.32 s7,[r11,#0x20]
  00090c80: vnmls.f32 s12,s6,s1
  00090c84: vmls.f32 s5,s0,s6
  00090c88: vstr.32 s9,[r11,#0x24]
  00090c8c: vmla.f32 s2,s4,s0
  00090c90: vstr.32 s8,[r11,#0x28]
  00090c94: vstr.32 s3,[r11,#0x10]
  00090c98: vstr.32 s14,[r11,#0x14]
  00090c9c: vstr.32 s12,[r11,#0x18]
  00090ca0: vstr.32 s10,[r11]
  00090ca4: vstr.32 s5,[r11,#0x4]
  00090ca8: vstr.32 s2,[r11,#0x8]
  00090cac: ldr r0,[sp,#0x8]
  00090cae: mov r1,r11
  00090cb0: movs r2,#0x3c
  00090cb2: blx 0x0006f1e4
  00090cb6: pop.w {r1,r2,r3,r8,r9,r10,r11}
  00090cba: pop {r4,r5,r6,r7,pc}
```
