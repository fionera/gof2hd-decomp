# ApplicationManager::OnTouchBegin
elf 0x83a38 body 338
Sig: undefined __thiscall OnTouchBegin(ApplicationManager * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnTouchBegin(int, int, void*) */

void __thiscall
AbyssEngine::ApplicationManager::OnTouchBegin
          (ApplicationManager *this,int param_1,int param_2,void *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  float fVar6;
  
  piVar5 = *(int **)(DAT_00093b9c + 0x93a4c);
  iVar1 = *piVar5;
  if ((*(int *)(this + 0x18) != 0) && (*(int *)(this + 0x3c) == 5)) {
    ConvertTouchCoords(this,(int *)&stack0xffffffd8,(int *)&stack0xffffffd4);
    (**(code **)(**(int **)(this + 0x18) + 0x24))(*(int **)(this + 0x18),param_1,param_2,param_3);
    (**(code **)(**(int **)(this + 0x18) + 0x18))(*(int **)(this + 0x18),param_1,param_2);
    iVar4 = DAT_00093ba0;
    *(int *)(this + 0xb4) = param_1;
    *(int *)(this + 0xb8) = param_2;
    if (**(uint **)(iVar4 + 0x93a98) < 4) {
                    /* WARNING: Could not recover jumptable at 0x00093a9e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&switchD_00093a9e::switchdataD_00093aa2 +
                (uint)(&switchD_00093a9e::switchdataD_00093aa2)[**(uint **)(iVar4 + 0x93a98)] * 2))
                ();
      return;
    }
    if (*(char *)(*(int *)(this + 0xa8) + 0x74) != '\0') {
      if (param_2 < 100) {
        **(byte **)(DAT_00093ba4 + 0x93b24) = **(byte **)(DAT_00093ba4 + 0x93b24) ^ 1;
      }
      else {
        iVar4 = PaintCanvas::GetHeight();
        iVar2 = PaintCanvas::GetWidth();
        if (iVar4 + -100 < param_2) {
          uVar3 = DAT_00093b98;
          if (param_1 < iVar2 / 2) {
            uVar3 = 0;
          }
          **(undefined4 **)(DAT_00093ba8 + 0x93b58) = uVar3;
        }
        else {
          fVar6 = DAT_00093b90;
          if (param_1 < iVar2 / 2) {
            fVar6 = DAT_00093b94;
          }
          **(float **)(DAT_00093bac + 0x93b64) = **(float **)(DAT_00093bac + 0x93b64) + fVar6;
        }
      }
    }
  }
  iVar4 = *piVar5;
  iVar1 = iVar4 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1,iVar4);
}

```

## target disasm
```
  00093a38: push {r4,r5,r6,r7,lr}
  00093a3a: add r7,sp,#0xc
  00093a3c: push {r4,r5,r6,r7,r8,r9,r10}
  00093a40: mov r4,r0
  00093a42: ldr r0,[0x00093b9c]
  00093a44: str r1,[sp,#0x8]
  00093a46: mov r5,r3
  00093a48: add r0,pc
  00093a4a: ldr.w r10,[r0,#0x0]
  00093a4e: ldr.w r0,[r10,#0x0]
  00093a52: str r2,[sp,#0x4]
  00093a54: str r0,[sp,#0xc]
  00093a56: ldr r0,[r4,#0x18]
  00093a58: cmp r0,#0x0
  00093a5a: beq.w 0x00093b7a
  00093a5e: ldr r0,[r4,#0x3c]
  00093a60: cmp r0,#0x5
  00093a62: bne.w 0x00093b7a
  00093a66: add r1,sp,#0x8
  00093a68: add r2,sp,#0x4
  00093a6a: mov r0,r4
  00093a6c: blx 0x000704b0
  00093a70: ldr r0,[r4,#0x18]
  00093a72: mov r3,r5
  00093a74: ldr r1,[r0,#0x0]
  00093a76: ldrd r9,r8,[sp,#0x4]
  00093a7a: ldr r6,[r1,#0x24]
  00093a7c: mov r1,r8
  00093a7e: mov r2,r9
  00093a80: blx r6
  00093a82: ldr r0,[r4,#0x18]
  00093a84: mov r2,r9
  00093a86: ldr r1,[r0,#0x0]
  00093a88: ldr r3,[r1,#0x18]
  00093a8a: mov r1,r8
  00093a8c: blx r3
  00093a8e: ldr r0,[0x00093ba0]
  00093a90: strd r8,r9,[r4,#0xb4]
  00093a94: add r0,pc
  00093a96: ldr r5,[r0,#0x0]
  00093a98: ldr r0,[r5,#0x0]
  00093a9a: cmp r0,#0x3
  00093a9c: bhi 0x00093b0e
  00093a9e: tbb [pc,r0]
  00093aa6: cmp.w r8,#0x31
  00093aaa: it le
  00093aac: cmp.le.w r9,#0x31
  00093ab0: bgt 0x00093b0e
  00093ab2: movs r0,#0x1
  00093ab4: b 0x00093b0a
  00093ab6: ldr r0,[r4,#0x0]
  00093ab8: blx 0x0006fafc
  00093abc: subs r0,#0x32
  00093abe: cmp r8,r0
  00093ac0: ble 0x00093b0e
  00093ac2: ldr r0,[r4,#0x0]
  00093ac4: blx 0x0006fb08
  00093ac8: subs r0,#0x32
  00093aca: cmp r9,r0
  00093acc: bgt 0x00093b06
  00093ace: b 0x00093b0e
  00093ad0: cmp.w r8,#0x31
  00093ad4: bgt 0x00093b0e
  00093ad6: ldr r0,[r4,#0x0]
  00093ad8: blx 0x0006fb08
  00093adc: subs r0,#0x32
  00093ade: cmp r9,r0
  00093ae0: ble 0x00093b0e
  00093ae2: ldr.w r0,[r4,#0xa8]
  00093ae6: ldrb.w r1,[r0,#0x74]
  00093aea: eor r1,r1,#0x1
  00093aee: strb.w r1,[r0,#0x74]
  00093af2: b 0x00093b7a
  00093af4: ldr r0,[r4,#0x0]
  00093af6: blx 0x0006fafc
  00093afa: cmp.w r9,#0x31
  00093afe: bgt 0x00093b0e
  00093b00: subs r0,#0x32
  00093b02: cmp r8,r0
  00093b04: ble 0x00093b0e
  00093b06: ldr r0,[r5,#0x0]
  00093b08: adds r0,#0x1
  00093b0a: str r0,[r5,#0x0]
  00093b0c: b 0x00093b7a
  00093b0e: ldr.w r0,[r4,#0xa8]
  00093b12: ldrb.w r0,[r0,#0x74]
  00093b16: cbz r0,0x00093b7a
  00093b18: cmp.w r9,#0x63
  00093b1c: bgt 0x00093b2e
  00093b1e: ldr r0,[0x00093ba4]
  00093b20: add r0,pc
  00093b22: ldr r0,[r0,#0x0]
  00093b24: ldrb r1,[r0,#0x0]
  00093b26: eor r1,r1,#0x1
  00093b2a: strb r1,[r0,#0x0]
  00093b2c: b 0x00093b7a
  00093b2e: ldr r0,[r4,#0x0]
  00093b30: blx 0x0006fb08
  00093b34: mov r5,r0
  00093b36: ldr r0,[r4,#0x0]
  00093b38: blx 0x0006fafc
  00093b3c: add.w r0,r0,r0, lsr #0x1f
  00093b40: asrs r1,r0,#0x1
  00093b42: sub.w r0,r5,#0x64
  00093b46: cmp r9,r0
  00093b48: ble 0x00093b5c
  00093b4a: ldr r0,[0x00093ba8]
  00093b4c: cmp r8,r1
  00093b4e: ite ge
  00093b50: ldr.ge r1,[0x00093b98]
  00093b52: mov.lt r1,#0x0
  00093b54: add r0,pc
  00093b56: ldr r0,[r0,#0x0]
  00093b58: str r1,[r0,#0x0]
  00093b5a: b 0x00093b7a
  00093b5c: ldr r0,[0x00093bac]
  00093b5e: cmp r8,r1
  00093b60: add r0,pc
  00093b62: ldr r0,[r0,#0x0]
  00093b64: vldr.32 s0,[r0]
  00093b68: ite ge
  00093b6a: vldr.ge.32 s2,[pc,#0x24]
  00093b6e: vldr.lt.32 s2,[pc,#0x24]
  00093b72: vadd.f32 s0,s0,s2
  00093b76: vstr.32 s0,[r0]
  00093b7a: ldr r0,[sp,#0xc]
  00093b7c: ldr.w r1,[r10,#0x0]
  00093b80: subs r0,r1,r0
  00093b82: itt eq
  00093b84: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00093b88: pop.eq {r4,r5,r6,r7,pc}
  00093b8a: blx 0x0006e824
```
