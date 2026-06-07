# Gun::setOffset
elf 0x152328 body 128
Sig: undefined __thiscall setOffset(Gun * this, int param_1, int param_2)

## decompile
```c

/* Gun::setOffset(int, int) */

undefined8 __thiscall Gun::setOffset(Gun *this,int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  uint in_fpscr;
  float local_20;
  float local_1c;
  float local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_001623a8 + 0x16233c);
  iVar1 = DAT_001623ac + 0x16233e + param_2 * 0x3c + param_1 * 6;
  local_1c = (float)VectorSignedToFloat((int)*(short *)(iVar1 + -0x3a),(byte)(in_fpscr >> 0x16) & 3)
  ;
  local_20 = (float)VectorSignedToFloat((int)*(short *)(iVar1 + -0x3c),(byte)(in_fpscr >> 0x16) & 3)
  ;
  local_18 = (float)VectorSignedToFloat((int)*(short *)(iVar1 + -0x38),(byte)(in_fpscr >> 0x16) & 3)
  ;
  local_14 = *piVar2;
  local_1c = *(float *)(this + 0x80) + local_1c;
  local_20 = *(float *)(this + 0x7c) + local_20;
  local_18 = *(float *)(this + 0x84) + local_18;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x7c),(Vector *)&local_20);
  if (*piVar2 == local_14) {
    return CONCAT44(local_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00162328: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0016232a: add r7,sp,#0x18
  0016232c: ldr r3,[0x001623a8]
  0016232e: rsb r2,r2,r2, lsl #0x4
  00162332: ldr r4,[0x001623ac]
  00162334: add.w r1,r1,r1, lsl #0x1
  00162338: add r3,pc
  0016233a: add r4,pc
  0016233c: add.w r2,r4,r2, lsl #0x2
  00162340: ldr r5,[r3,#0x0]
  00162342: add.w r1,r2,r1, lsl #0x1
  00162346: ldrsh.w r2,[r1,#-0x3c]
  0016234a: ldrsh.w r4,[r1,#-0x3a]
  0016234e: ldrsh.w r1,[r1,#-0x38]
  00162352: vmov s2,r2
  00162356: vmov s0,r4
  0016235a: vmov s4,r1
  0016235e: vcvt.f32.s32 s0,s0
  00162362: vcvt.f32.s32 s2,s2
  00162366: vcvt.f32.s32 s4,s4
  0016236a: ldr r1,[r5,#0x0]
  0016236c: str r1,[sp,#0xc]
  0016236e: mov r1,sp
  00162370: vldr.32 s6,[r0,#0x7c]
  00162374: vldr.32 s8,[r0,#0x80]
  00162378: vldr.32 s10,[r0,#0x84]
  0016237c: adds r0,#0x7c
  0016237e: vadd.f32 s0,s8,s0
  00162382: vadd.f32 s2,s6,s2
  00162386: vadd.f32 s4,s10,s4
  0016238a: vstr.32 s0,[sp,#0x4]
  0016238e: vstr.32 s2,[sp]
  00162392: vstr.32 s4,[sp,#0x8]
  00162396: blx 0x0006eb3c
  0016239a: ldr r0,[sp,#0xc]
  0016239c: ldr r1,[r5,#0x0]
  0016239e: subs r0,r1,r0
  001623a0: it eq
  001623a2: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  001623a4: blx 0x0006e824
```
