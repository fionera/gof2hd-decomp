# PaintCanvas::FogSetParameter
elf 0x7cf28 body 324
Sig: undefined __thiscall FogSetParameter(PaintCanvas * this, FogMode param_1, float param_2, float param_3, float param_4, uint param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::FogSetParameter(AbyssEngine::FogMode, float, float, float, unsigned
   int) */

void __thiscall
AbyssEngine::PaintCanvas::FogSetParameter
          (PaintCanvas *this,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,uint param_6)

{
  int iVar1;
  code *pcVar2;
  int *piVar3;
  uint in_fpscr;
  undefined4 uVar4;
  float fVar5;
  float fVar6;
  float in_s14;
  double dVar7;
  double dVar8;
  Vector local_3c [4];
  float local_38;
  float local_34;
  float local_30;
  int local_2c;
  
  piVar3 = *(int **)(DAT_0008d080 + 0x8cf46);
  local_2c = *piVar3;
  local_30 = in_s14;
  if (**(char **)(DAT_0008d07c + 0x8cf40) == '\0') {
    uVar4 = VectorUnsignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    pcVar2 = *(code **)(DAT_0008d084 + 0x8cfd2);
    (*pcVar2)(0xb65,uVar4);
    (*pcVar2)(0xb62,param_5);
    (*pcVar2)(0xb63,param_3);
    (*pcVar2)(0xb64,param_4);
    fVar5 = (float)VectorUnsignedToFloat((param_6 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
    fVar6 = (float)VectorUnsignedToFloat((param_6 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
    VectorUnsignedToFloat(param_6 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
    local_30 = (float)VectorUnsignedToFloat(param_6 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
    local_30 = local_30 / DAT_0008d078;
    _local_38 = CONCAT44(fVar6 / DAT_0008d078,fVar5 / DAT_0008d078);
    glFogfv(0xb66,local_3c);
  }
  else {
    dVar7 = (double)VectorUnsignedToFloat((param_6 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3)
    ;
    dVar7 = dVar7 / DAT_0008d070;
    VectorUnsignedToFloat(param_6 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
    dVar8 = (double)VectorUnsignedToFloat((param_6 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
    iVar1 = *(int *)(this + 0x34);
    dVar8 = dVar8 / DAT_0008d070;
    *(undefined4 *)(iVar1 + 1000) = param_3;
    *(undefined4 *)(iVar1 + 0x3ec) = param_4;
    _local_38 = CONCAT44((float)dVar8,(float)dVar7);
    AEMath::Vector::operator=((Vector *)(iVar1 + 0x3f0),local_3c);
  }
  if (*piVar3 != local_2c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008cf28: push {r4,r5,r6,r7,lr}
  0008cf2a: add r7,sp,#0xc
  0008cf2c: push {r8,r9,r11}
  0008cf30: vpush {d5,d6,d7,d8}
  0008cf34: mov r5,r2
  0008cf36: ldr r2,[0x0008d07c]
  0008cf38: mov r4,r3
  0008cf3a: ldr r3,[0x0008d080]
  0008cf3c: add r2,pc
  0008cf3e: ldr.w r9,[r7,#0xc]
  0008cf42: add r3,pc
  0008cf44: vmov s0,r4
  0008cf48: ldr r2,[r2,#0x0]
  0008cf4a: vmov s2,r5
  0008cf4e: ldr.w r8,[r3,#0x0]
  0008cf52: ldrb r2,[r2,#0x0]
  0008cf54: ldr.w r3,[r8,#0x0]
  0008cf58: cmp r2,#0x0
  0008cf5a: str r3,[sp,#0x14]
  0008cf5c: beq 0x0008cfc0
  0008cf5e: ubfx r1,r9,#0x10,#0x8
  0008cf62: vldr.64 d18,[pc,#0x10c]
  0008cf66: vmov s4,r1
  0008cf6a: lsr.w r1,r9,#0x18
  0008cf6e: vcvt.f64.u32 d16,s4
  0008cf72: vmov s4,r1
  0008cf76: vdiv.f64 d16,d16,d18
  0008cf7a: vcvt.f64.u32 d17,s4
  0008cf7e: ubfx r1,r9,#0x8,#0x8
  0008cf82: vmov s4,r1
  0008cf86: add r1,sp,#0x4
  0008cf88: vdiv.f64 d17,d17,d18
  0008cf8c: vcvt.f64.u32 d19,s4
  0008cf90: ldr r0,[r0,#0x34]
  0008cf92: vdiv.f64 d18,d19,d18
  0008cf96: vcvt.f32.f64 s4,d16
  0008cf9a: vcvt.f32.f64 s6,d17
  0008cf9e: vcvt.f32.f64 s8,d18
  0008cfa2: vstr.32 s2,[r0,#0x3e8]
  0008cfa6: vstr.32 s0,[r0,#0x3ec]
  0008cfaa: add.w r0,r0,#0x3f0
  0008cfae: vstr.32 s4,[sp,#0x8]
  0008cfb2: vstr.32 s6,[sp,#0x4]
  0008cfb6: vstr.32 s8,[sp,#0xc]
  0008cfba: blx 0x0006eb3c
  0008cfbe: b 0x0008d054
  0008cfc0: vmov s0,r1
  0008cfc4: vldr.32 s16,[r7,#0x8]
  0008cfc8: vcvt.f32.u32 s0,s0
  0008cfcc: ldr r0,[0x0008d084]
  0008cfce: add r0,pc
  0008cfd0: ldr r6,[r0,#0x0]
  0008cfd2: movw r0,#0xb65
  0008cfd6: vmov r1,s0
  0008cfda: blx r6
  0008cfdc: vmov r1,s16
  0008cfe0: movw r0,#0xb62
  0008cfe4: blx r6
  0008cfe6: movw r0,#0xb63
  0008cfea: mov r1,r5
  0008cfec: blx r6
  0008cfee: movw r0,#0xb64
  0008cff2: mov r1,r4
  0008cff4: blx r6
  0008cff6: ubfx r0,r9,#0x10,#0x8
  0008cffa: lsr.w r1,r9,#0x18
  0008cffe: vmov s6,r1
  0008d002: vldr.32 s2,[pc,#0x74]
  0008d006: vmov s0,r0
  0008d00a: add r1,sp,#0x4
  0008d00c: vcvt.f32.u32 s0,s0
  0008d010: ubfx r0,r9,#0x8,#0x8
  0008d014: vmov s4,r0
  0008d018: uxtb.w r0,r9
  0008d01c: vmov s8,r0
  0008d020: movw r0,#0xb66
  0008d024: vcvt.f32.u32 s4,s4
  0008d028: vcvt.f32.u32 s6,s6
  0008d02c: vcvt.f32.u32 s8,s8
  0008d030: vdiv.f32 s0,s0,s2
  0008d034: vdiv.f32 s6,s6,s2
  0008d038: vdiv.f32 s4,s4,s2
  0008d03c: vdiv.f32 s2,s8,s2
  0008d040: vstr.32 s0,[sp,#0x8]
  0008d044: vstr.32 s6,[sp,#0x4]
  0008d048: vstr.32 s4,[sp,#0xc]
  0008d04c: vstr.32 s2,[sp,#0x10]
  0008d050: blx 0x0006feb0
  0008d054: ldr r0,[sp,#0x14]
  0008d056: ldr.w r1,[r8,#0x0]
  0008d05a: subs r0,r1,r0
  0008d05c: ittt eq
  0008d05e: vpop.eq {d5,d6,d7,d8}
  0008d062: pop.eq.w {r8,r9,r11}
  0008d066: pop.eq {r4,r5,r6,r7,pc}
  0008d068: blx 0x0006e824
```
