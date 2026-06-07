# Hud::touchMove
elf 0x166ab8 body 200
Sig: undefined __stdcall touchMove(uint param_1, uint param_2, void * param_3)

## decompile
```c

/* Hud::touchMove(unsigned int, unsigned int, void*) */

undefined4 Hud::touchMove(uint param_1,uint param_2,void *param_3)

{
  short sVar1;
  short sVar2;
  float extraout_r0;
  uint uVar3;
  int in_r3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint in_fpscr;
  undefined4 uVar7;
  int iVar8;
  undefined8 in_d0;
  
  uVar3 = 0;
  while( true ) {
    if (0x18 < uVar3) {
      uVar7 = (*(code *)(DAT_001ac664 + 0x1ac668))(param_1,param_2,param_3);
      return uVar7;
    }
    if ((*(int *)(*(int *)(*(int *)(param_1 + 0x28c) + 4) + uVar3 * 4) == in_r3) &&
       (*(int *)(*(int *)(param_1 + 0x290) + uVar3 * 4) == 0x20)) break;
    uVar3 = uVar3 + 1;
  }
  iVar5 = param_2 - *(ushort *)(param_1 + 0x424);
  iVar4 = (int)param_3 - (uint)*(ushort *)(param_1 + 0x426);
  uVar7 = VectorSignedToFloat(iVar4 * iVar4 + iVar5 * iVar5,(byte)(in_fpscr >> 0x16) & 3);
  Globals::sqrt((Globals *)**(undefined4 **)(DAT_00176b80 + 0x176b24),
                (double)CONCAT44((int)((ulonglong)in_d0 >> 0x20),uVar7));
  iVar6 = *(int *)(param_1 + 0x4e0);
  iVar8 = (int)extraout_r0;
  if (iVar6 < iVar8) {
    sVar2 = __aeabi_idiv(iVar6 * iVar5,iVar8);
    sVar1 = *(short *)(param_1 + 0x426);
    *(short *)(param_1 + 0x41e) = sVar2 + *(short *)(param_1 + 0x424);
    sVar2 = __aeabi_idiv(iVar6 * iVar4,iVar8);
    *(short *)(param_1 + 0x420) = sVar2 + sVar1;
  }
  else {
    *(short *)(param_1 + 0x420) = (short)param_3;
    *(short *)(param_1 + 0x41e) = (short)param_2;
  }
  return 0x20;
}

```

## target disasm
```
  00176ab8: push {r4,r5,r6,r7,lr}
  00176aba: add r7,sp,#0xc
  00176abc: push {r7,r8,r9,r10,r11}
  00176ac0: mov r4,r0
  00176ac2: ldr.w r0,[r0,#0x28c]
  00176ac6: mov r6,r1
  00176ac8: mov r5,r2
  00176aca: movs r1,#0x0
  00176acc: ldr r0,[r0,#0x4]
  00176ace: b 0x00176ae6
  00176ad0: ldr.w r2,[r0,r1,lsl #0x2]
  00176ad4: cmp r2,r3
  00176ad6: bne 0x00176ae4
  00176ad8: ldr.w r2,[r4,#0x290]
  00176adc: ldr.w r2,[r2,r1,lsl #0x2]
  00176ae0: cmp r2,#0x20
  00176ae2: beq 0x00176afe
  00176ae4: adds r1,#0x1
  00176ae6: cmp r1,#0x18
  00176ae8: bls 0x00176ad0
  00176aea: mov r0,r4
  00176aec: mov r1,r6
  00176aee: mov r2,r5
  00176af0: add sp,#0x4
  00176af2: pop.w {r8,r9,r10,r11}
  00176af6: pop.w {r4,r5,r6,r7,lr}
  00176afa: b.w 0x001ac658
  00176afe: ldrh.w r0,[r4,#0x424]
  00176b02: ldrh.w r1,[r4,#0x426]
  00176b06: sub.w r10,r6,r0
  00176b0a: sub.w r9,r5,r1
  00176b0e: mul r0,r10,r10
  00176b12: mla r0,r9,r9,r0
  00176b16: vmov s0,r0
  00176b1a: vcvt.f32.s32 s0,s0
  00176b1e: ldr r0,[0x00176b80]
  00176b20: add r0,pc
  00176b22: ldr r0,[r0,#0x0]
  00176b24: vmov r1,s0
  00176b28: ldr r0,[r0,#0x0]
  00176b2a: blx 0x000760e4
  00176b2e: vmov s0,r0
  00176b32: ldr.w r11,[r4,#0x4e0]
  00176b36: vcvt.s32.f32 s0,s0
  00176b3a: vmov r8,s0
  00176b3e: cmp r11,r8
  00176b40: bge 0x00176b6e
  00176b42: mul r0,r11,r10
  00176b46: mov r1,r8
  00176b48: blx 0x0007198c
  00176b4c: mul r1,r11,r9
  00176b50: ldrh.w r2,[r4,#0x424]
  00176b54: ldrh.w r5,[r4,#0x426]
  00176b58: add r0,r2
  00176b5a: strh.w r0,[r4,#0x41e]
  00176b5e: mov r0,r1
  00176b60: mov r1,r8
  00176b62: blx 0x0007198c
  00176b66: add r0,r5
  00176b68: strh.w r0,[r4,#0x420]
  00176b6c: b 0x00176b76
  00176b6e: strh.w r5,[r4,#0x420]
  00176b72: strh.w r6,[r4,#0x41e]
  00176b76: movs r0,#0x20
  00176b78: pop.w {r3,r8,r9,r10,r11}
  00176b7c: pop {r4,r5,r6,r7,pc}
  001ac658: bx pc
```
