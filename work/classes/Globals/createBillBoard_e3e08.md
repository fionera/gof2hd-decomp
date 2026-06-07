# Globals::createBillBoard
elf 0xe3e08 body 662
Sig: undefined __stdcall createBillBoard(int param_1, int param_2, float param_3, float param_4, float param_5, float param_6, int param_7)

## decompile
```c

/* Globals::createBillBoard(int, int, float, float, float, float, int) */

void Globals::createBillBoard
               (int param_1,int param_2,float param_3,float param_4,float param_5,float param_6,
               int param_7)

{
  uint *puVar1;
  code *pcVar2;
  undefined4 *puVar3;
  uint in_fpscr;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 in_stack_00000000;
  undefined4 in_stack_00000004;
  undefined4 in_stack_00000008;
  undefined2 in_stack_0000000c;
  undefined8 local_50;
  
  puVar1 = *(uint **)(DAT_000f40a0 + 0xf3e28);
  puVar3 = *(undefined4 **)(DAT_000f40a4 + 0xf3e2c);
  local_50 = (ulonglong)*puVar1 << 0x20;
  AbyssEngine::PaintCanvas::MeshCreate
            ((PaintCanvas *)*puVar3,0xc,6,0x13,in_stack_0000000c,&local_50);
  pcVar2 = *(code **)(DAT_000f40a8 + 0xf3e62);
  (*pcVar2)(*puVar3,(undefined4)local_50,0,0,1,2);
  (*pcVar2)(*puVar3,(undefined4)local_50,1,2,1,3);
  (*pcVar2)(*puVar3,(undefined4)local_50,2,4,5,6);
  (*pcVar2)(*puVar3,(undefined4)local_50,3,6,5,7);
  (*pcVar2)(*puVar3,(undefined4)local_50,4,8,9,10);
  (*pcVar2)(*puVar3,(undefined4)local_50,5,10,9,0xb);
  pcVar2 = *(code **)(DAT_000f40ac + 0xf3ecc);
  (*pcVar2)(*puVar3,(undefined4)local_50,0);
  (*pcVar2)(*puVar3,(undefined4)local_50,1);
  (*pcVar2)(*puVar3,(undefined4)local_50,2,in_stack_00000004,in_stack_00000000);
  (*pcVar2)(*puVar3,(undefined4)local_50,3,in_stack_00000004,in_stack_00000008);
  (*pcVar2)(*puVar3,(undefined4)local_50,4);
  (*pcVar2)(*puVar3,(undefined4)local_50,5);
  (*pcVar2)(*puVar3,(undefined4)local_50,6,in_stack_00000004,in_stack_00000000);
  (*pcVar2)(*puVar3,(undefined4)local_50,7,in_stack_00000004,in_stack_00000008);
  (*pcVar2)(*puVar3,(undefined4)local_50,8);
  (*pcVar2)(*puVar3,(undefined4)local_50,9);
  (*pcVar2)(*puVar3,(undefined4)local_50,10,in_stack_00000004,in_stack_00000000);
  (*pcVar2)(*puVar3,(undefined4)local_50,0xb,in_stack_00000004,in_stack_00000008);
  uVar6 = VectorSignedToFloat(param_7,(byte)(in_fpscr >> 0x16) & 3);
  pcVar2 = *(code **)(DAT_000f40b0 + 0xf3f90);
  uVar4 = VectorSignedToFloat(-param_2,(byte)(in_fpscr >> 0x16) & 3);
  (*pcVar2)(*puVar3,(undefined4)local_50,0,uVar4,0,uVar6);
  uVar7 = VectorSignedToFloat(-param_7,(byte)(in_fpscr >> 0x16) & 3);
  (*pcVar2)(*puVar3,(undefined4)local_50,1,uVar4,0,uVar7);
  uVar5 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  (*pcVar2)(*puVar3,(undefined4)local_50,2,uVar5,0,uVar6);
  (*pcVar2)(*puVar3,(undefined4)local_50,3,uVar5,0,uVar7);
  (*pcVar2)(*puVar3,(undefined4)local_50,4,0,uVar4,uVar6);
  (*pcVar2)(*puVar3,(undefined4)local_50,5,0,uVar4,uVar7);
  (*pcVar2)(*puVar3,(undefined4)local_50,6,0,uVar5,uVar6);
  (*pcVar2)(*puVar3,(undefined4)local_50,7,0,uVar5,uVar7);
  (*pcVar2)(*puVar3,(undefined4)local_50,8,uVar4,uVar5,0);
  (*pcVar2)(*puVar3,(undefined4)local_50,9,uVar4,uVar4,0);
  (*pcVar2)(*puVar3,(undefined4)local_50,10,uVar5,uVar5,0);
  (*pcVar2)(*puVar3,(undefined4)local_50,0xb,uVar5,uVar4,0);
  if (*puVar1 != local_50._4_4_) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail((undefined4)local_50);
  }
  return;
}

```

## target disasm
```
  000f3e08: push {r4,r5,r6,r7,lr}
  000f3e0a: add r7,sp,#0xc
  000f3e0c: push {r7,r8,r9,r10,r11}
  000f3e10: vpush {d4,d5,d6,d7,d8,d9,d10,d11}
  000f3e14: strd r2,r1,[sp,#0xc]
  000f3e18: add r2,sp,#0x18
  000f3e1a: ldr r0,[0x000f40a0]
  000f3e1c: mov.w r11,#0x0
  000f3e20: ldr r1,[0x000f40a4]
  000f3e22: mov r10,r3
  000f3e24: add r0,pc
  000f3e26: movs r3,#0x13
  000f3e28: add r1,pc
  000f3e2a: mov.w r9,#0x6
  000f3e2e: ldr r0,[r0,#0x0]
  000f3e30: str r0,[sp,#0x14]
  000f3e32: ldr r5,[r1,#0x0]
  000f3e34: ldr r0,[r0,#0x0]
  000f3e36: str r0,[sp,#0x1c]
  000f3e38: ldr r0,[r5,#0x0]
  000f3e3a: ldrh r1,[r7,#0x14]
  000f3e3c: str.w r11,[sp,#0x18]
  000f3e40: strd r1,r2,[sp,#0x0]
  000f3e44: movs r1,#0xc
  000f3e46: movs r2,#0x6
  000f3e48: blx 0x00075da8
  000f3e4c: movs r2,#0x2
  000f3e4e: mov.w r8,#0x1
  000f3e52: ldr r0,[r5,#0x0]
  000f3e54: movs r3,#0x0
  000f3e56: ldr r1,[sp,#0x18]
  000f3e58: strd r8,r2,[sp,#0x0]
  000f3e5c: ldr r2,[0x000f40a8]
  000f3e5e: add r2,pc
  000f3e60: ldr r4,[r2,#0x0]
  000f3e62: movs r2,#0x0
  000f3e64: blx r4
  000f3e66: ldr r0,[r5,#0x0]
  000f3e68: movs r2,#0x3
  000f3e6a: ldr r1,[sp,#0x18]
  000f3e6c: movs r3,#0x2
  000f3e6e: strd r8,r2,[sp,#0x0]
  000f3e72: movs r2,#0x1
  000f3e74: blx r4
  000f3e76: ldr r0,[r5,#0x0]
  000f3e78: movs r6,#0x5
  000f3e7a: ldr r1,[sp,#0x18]
  000f3e7c: movs r2,#0x2
  000f3e7e: movs r3,#0x4
  000f3e80: strd r6,r9,[sp,#0x0]
  000f3e84: blx r4
  000f3e86: ldr r0,[r5,#0x0]
  000f3e88: movs r2,#0x7
  000f3e8a: ldr r1,[sp,#0x18]
  000f3e8c: movs r3,#0x6
  000f3e8e: strd r6,r2,[sp,#0x0]
  000f3e92: movs r2,#0x3
  000f3e94: blx r4
  000f3e96: ldr r0,[r5,#0x0]
  000f3e98: movs r2,#0xa
  000f3e9a: ldr r1,[sp,#0x18]
  000f3e9c: movs r6,#0x9
  000f3e9e: strd r6,r2,[sp,#0x0]
  000f3ea2: movs r2,#0x4
  000f3ea4: movs r3,#0x8
  000f3ea6: blx r4
  000f3ea8: ldr r0,[r5,#0x0]
  000f3eaa: movs r2,#0xb
  000f3eac: ldr r1,[sp,#0x18]
  000f3eae: movs r3,#0xa
  000f3eb0: strd r6,r2,[sp,#0x0]
  000f3eb4: movs r2,#0x5
  000f3eb6: blx r4
  000f3eb8: vldr.32 s16,[r7,#0x8]
  000f3ebc: mov r3,r10
  000f3ebe: ldr r0,[r5,#0x0]
  000f3ec0: vstr.32 s16,[sp]
  000f3ec4: ldr r1,[sp,#0x18]
  000f3ec6: ldr r2,[0x000f40ac]
  000f3ec8: add r2,pc
  000f3eca: ldr r4,[r2,#0x0]
  000f3ecc: movs r2,#0x0
  000f3ece: blx r4
  000f3ed0: ldr r0,[r5,#0x0]
  000f3ed2: movs r2,#0x1
  000f3ed4: ldr r1,[sp,#0x18]
  000f3ed6: mov r3,r10
  000f3ed8: vldr.32 s18,[r7,#0x10]
  000f3edc: vstr.32 s18,[sp]
  000f3ee0: blx r4
  000f3ee2: ldr.w r9,[r7,#0xc]
  000f3ee6: movs r2,#0x2
  000f3ee8: ldr r0,[r5,#0x0]
  000f3eea: ldr r1,[sp,#0x18]
  000f3eec: mov r3,r9
  000f3eee: vstr.32 s16,[sp]
  000f3ef2: blx r4
  000f3ef4: ldr r0,[r5,#0x0]
  000f3ef6: movs r2,#0x3
  000f3ef8: ldr r1,[sp,#0x18]
  000f3efa: mov r3,r9
  000f3efc: vstr.32 s18,[sp]
  000f3f00: blx r4
  000f3f02: ldr r0,[r5,#0x0]
  000f3f04: movs r2,#0x4
  000f3f06: ldr r1,[sp,#0x18]
  000f3f08: mov r3,r10
  000f3f0a: vstr.32 s16,[sp]
  000f3f0e: blx r4
  000f3f10: ldr r0,[r5,#0x0]
  000f3f12: movs r2,#0x5
  000f3f14: ldr r1,[sp,#0x18]
  000f3f16: mov r3,r10
  000f3f18: vstr.32 s18,[sp]
  000f3f1c: blx r4
  000f3f1e: ldr r0,[r5,#0x0]
  000f3f20: movs r2,#0x6
  000f3f22: ldr r1,[sp,#0x18]
  000f3f24: mov r3,r9
  000f3f26: vstr.32 s16,[sp]
  000f3f2a: blx r4
  000f3f2c: ldr r0,[r5,#0x0]
  000f3f2e: movs r2,#0x7
  000f3f30: ldr r1,[sp,#0x18]
  000f3f32: mov r3,r9
  000f3f34: vstr.32 s18,[sp]
  000f3f38: blx r4
  000f3f3a: ldr r0,[r5,#0x0]
  000f3f3c: movs r2,#0x8
  000f3f3e: ldr r1,[sp,#0x18]
  000f3f40: mov r3,r10
  000f3f42: vstr.32 s16,[sp]
  000f3f46: blx r4
  000f3f48: ldr r0,[r5,#0x0]
  000f3f4a: movs r2,#0x9
  000f3f4c: ldr r1,[sp,#0x18]
  000f3f4e: mov r3,r10
  000f3f50: vstr.32 s18,[sp]
  000f3f54: blx r4
  000f3f56: ldr r0,[r5,#0x0]
  000f3f58: movs r2,#0xa
  000f3f5a: ldr r1,[sp,#0x18]
  000f3f5c: mov r3,r9
  000f3f5e: vstr.32 s16,[sp]
  000f3f62: blx r4
  000f3f64: ldr r0,[r5,#0x0]
  000f3f66: movs r2,#0xb
  000f3f68: ldr r1,[sp,#0x18]
  000f3f6a: mov r3,r9
  000f3f6c: vstr.32 s18,[sp]
  000f3f70: blx r4
  000f3f72: ldr r6,[sp,#0xc]
  000f3f74: vmov s0,r6
  000f3f78: vcvt.f32.s32 s20,s0
  000f3f7c: str.w r11,[sp,#0x0]
  000f3f80: ldr r0,[r5,#0x0]
  000f3f82: ldr r1,[sp,#0x18]
  000f3f84: vstr.32 s20,[sp,#0x4]
  000f3f88: ldr r2,[0x000f40b0]
  000f3f8a: ldr r4,[sp,#0x10]
  000f3f8c: add r2,pc
  000f3f8e: ldr.w r9,[r2,#0x0]
  000f3f92: rsbs r2,r4
  000f3f94: vmov s0,r2
  000f3f98: movs r2,#0x0
  000f3f9a: vcvt.f32.s32 s16,s0
  000f3f9e: vmov r8,s16
  000f3fa2: mov r3,r8
  000f3fa4: blx r9
  000f3fa6: rsbs r0,r6
  000f3fa8: movs r2,#0x1
  000f3faa: mov r3,r8
  000f3fac: vmov s0,r0
  000f3fb0: vcvt.f32.s32 s22,s0
  000f3fb4: ldr r0,[r5,#0x0]
  000f3fb6: ldr r1,[sp,#0x18]
  000f3fb8: str.w r11,[sp,#0x0]
  000f3fbc: vstr.32 s22,[sp,#0x4]
  000f3fc0: blx r9
  000f3fc2: vmov s0,r4
  000f3fc6: ldr r0,[r5,#0x0]
  000f3fc8: ldr r1,[sp,#0x18]
  000f3fca: movs r2,#0x2
  000f3fcc: vstr.32 s20,[sp,#0x4]
  000f3fd0: str.w r11,[sp,#0x0]
  000f3fd4: vcvt.f32.s32 s18,s0
  000f3fd8: vmov r4,s18
  000f3fdc: mov r3,r4
  000f3fde: blx r9
  000f3fe0: ldr r0,[r5,#0x0]
  000f3fe2: movs r2,#0x3
  000f3fe4: ldr r1,[sp,#0x18]
  000f3fe6: mov r3,r4
  000f3fe8: vstr.32 s22,[sp,#0x4]
  000f3fec: str.w r11,[sp,#0x0]
  000f3ff0: blx r9
  000f3ff2: ldr r0,[r5,#0x0]
  000f3ff4: movs r2,#0x4
  000f3ff6: ldr r1,[sp,#0x18]
  000f3ff8: movs r3,#0x0
  000f3ffa: vstr.32 s16,[sp]
  000f3ffe: vstr.32 s20,[sp,#0x4]
  000f4002: blx r9
  000f4004: ldr r0,[r5,#0x0]
  000f4006: movs r2,#0x5
  000f4008: ldr r1,[sp,#0x18]
  000f400a: movs r3,#0x0
  000f400c: vstr.32 s16,[sp]
  000f4010: vstr.32 s22,[sp,#0x4]
  000f4014: blx r9
  000f4016: ldr r0,[r5,#0x0]
  000f4018: movs r2,#0x6
  000f401a: ldr r1,[sp,#0x18]
  000f401c: movs r3,#0x0
  000f401e: vstr.32 s18,[sp]
  000f4022: vstr.32 s20,[sp,#0x4]
  000f4026: blx r9
  000f4028: ldr r0,[r5,#0x0]
  000f402a: movs r2,#0x7
  000f402c: ldr r1,[sp,#0x18]
  000f402e: movs r3,#0x0
  000f4030: vstr.32 s18,[sp]
  000f4034: vstr.32 s22,[sp,#0x4]
  000f4038: blx r9
  000f403a: ldr r0,[r5,#0x0]
  000f403c: movs r2,#0x8
  000f403e: ldr r1,[sp,#0x18]
  000f4040: mov r3,r8
  000f4042: str.w r11,[sp,#0x4]
  000f4046: vstr.32 s18,[sp]
  000f404a: blx r9
  000f404c: ldr r0,[r5,#0x0]
  000f404e: movs r2,#0x9
  000f4050: ldr r1,[sp,#0x18]
  000f4052: mov r3,r8
  000f4054: str.w r11,[sp,#0x4]
  000f4058: vstr.32 s16,[sp]
  000f405c: blx r9
  000f405e: ldr r0,[r5,#0x0]
  000f4060: movs r2,#0xa
  000f4062: ldr r1,[sp,#0x18]
  000f4064: mov r3,r4
  000f4066: str.w r11,[sp,#0x4]
  000f406a: vstr.32 s18,[sp]
  000f406e: blx r9
  000f4070: ldr r0,[r5,#0x0]
  000f4072: movs r2,#0xb
  000f4074: ldr r1,[sp,#0x18]
  000f4076: mov r3,r4
  000f4078: str.w r11,[sp,#0x4]
  000f407c: vstr.32 s16,[sp]
  000f4080: blx r9
  000f4082: ldr r0,[sp,#0x18]
  000f4084: ldr r1,[sp,#0x1c]
  000f4086: ldr r2,[sp,#0x14]
  000f4088: ldr r2,[r2,#0x0]
  000f408a: subs r1,r2,r1
  000f408c: itttt eq
  000f408e: vpop.eq {d4,d5,d6,d7,d8,d9,d10,d11}
  000f4092: add.eq sp,#0x4
  000f4094: pop.eq.w {r8,r9,r10,r11}
  000f4098: pop.eq {r4,r5,r6,r7,pc}
  000f409a: blx 0x0006e824
```
