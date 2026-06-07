# Engine::SetTexturesExt
elf 0x84b0c body 296
Sig: undefined __stdcall SetTexturesExt(uint param_1, ...)

## decompile
```c

/* AbyssEngine::Engine::SetTexturesExt(unsigned int, ...) */

void AbyssEngine::Engine::SetTexturesExt(uint param_1,...)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  uint in_r1;
  int *piVar5;
  uint in_r2;
  undefined4 in_r3;
  int iVar6;
  int iVar7;
  float *pfVar8;
  char *pcVar9;
  float fVar10;
  uint *puVar11;
  uint local_8;
  undefined4 local_4;
  
  piVar5 = *(int **)(DAT_00094c34 + 0x94b22);
  iVar1 = *piVar5;
  local_8 = in_r2;
  local_4 = in_r3;
  if (*(int *)(**(int **)(param_1 + 0x30) + 0x10) != 0) {
    iVar7 = 0;
    pfVar8 = *(float **)(DAT_00094c38 + 0x94b48);
    pcVar2 = *(char **)(DAT_00094c3c + 0x94b50);
    pcVar9 = *(char **)(DAT_00094c40 + 0x94b58);
    puVar11 = &local_8;
    while (in_r1 != 0xffffffff) {
      if ((in_r1 < *(uint *)(**(int **)(param_1 + 0x30) + 0x10)) &&
         (iVar6 = **(int **)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + in_r1 * 4),
         *(int *)(param_1 + 0x7c + iVar7 * 4) != iVar6)) {
        glActiveTexture(iVar7 + 0x84c0);
        iVar3 = *(int *)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + in_r1 * 4);
        fVar10 = *(float *)(iVar3 + 0x10);
        if (*pfVar8 != fVar10) {
          *pfVar8 = fVar10;
          if (*pcVar2 == '\0') {
            glTexEnvf(0x8500,0x8501,fVar10);
            iVar3 = *(int *)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + in_r1 * 4);
          }
          else if (*pcVar9 != '\0') {
            *pcVar9 = '\0';
          }
        }
        if (*(char *)(iVar3 + 0x14) == '\0') {
          uVar4 = 0xde1;
        }
        else {
          uVar4 = 0x8513;
        }
        glBindTexture(uVar4,iVar6);
        *(int *)(param_1 + 0x7c + iVar7 * 4) = iVar6;
      }
      iVar7 = iVar7 + 1;
      in_r1 = *puVar11;
      puVar11 = puVar11 + 1;
    }
    for (iVar7 = iVar7 + 0x1f; iVar7 - 0x1fU < 0x14; iVar7 = iVar7 + 1) {
      *(undefined4 *)(param_1 + iVar7 * 4) = 0xffffffff;
    }
    glActiveTexture(0x84c0);
  }
  if (*piVar5 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00094b0c: sub sp,#0x8
  00094b0e: push {r4,r5,r6,r7,lr}
  00094b10: add r7,sp,#0xc
  00094b12: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00094b16: mov r4,r0
  00094b18: ldr r0,[0x00094c34]
  00094b1a: mov r5,r1
  00094b1c: str r3,[r7,#0xc]
  00094b1e: add r0,pc
  00094b20: ldr r1,[r0,#0x0]
  00094b22: ldr r0,[r1,#0x0]
  00094b24: str r2,[r7,#0x8]
  00094b26: str r0,[sp,#0x10]
  00094b28: ldr r0,[r4,#0x30]
  00094b2a: ldr r0,[r0,#0x0]
  00094b2c: ldr r0,[r0,#0x10]
  00094b2e: cmp r0,#0x0
  00094b30: beq 0x00094c18
  00094b32: add.w r0,r7,#0x8
  00094b36: str r1,[sp,#0x4]
  00094b38: str r0,[sp,#0xc]
  00094b3a: add.w r9,r4,#0x7c
  00094b3e: ldr r0,[0x00094c38]
  00094b40: mov.w r8,#0x0
  00094b44: add r0,pc
  00094b46: ldr.w r10,[r0,#0x0]
  00094b4a: ldr r0,[0x00094c3c]
  00094b4c: add r0,pc
  00094b4e: ldr r0,[r0,#0x0]
  00094b50: str r0,[sp,#0x8]
  00094b52: ldr r0,[0x00094c40]
  00094b54: add r0,pc
  00094b56: ldr.w r11,[r0,#0x0]
  00094b5a: b 0x00094bf2
  00094b5c: ldr r0,[r4,#0x30]
  00094b5e: ldr r0,[r0,#0x0]
  00094b60: ldr r1,[r0,#0x10]
  00094b62: cmp r5,r1
  00094b64: bcs 0x00094be6
  00094b66: ldr r0,[r0,#0x14]
  00094b68: ldr.w r1,[r9,r8,lsl #0x2]
  00094b6c: ldr.w r0,[r0,r5,lsl #0x2]
  00094b70: ldr r6,[r0,#0x0]
  00094b72: cmp r1,r6
  00094b74: beq 0x00094be6
  00094b76: movw r0,#0x84c0
  00094b7a: add r0,r8
  00094b7c: blx 0x0006fb8c
  00094b80: ldr r0,[r4,#0x30]
  00094b82: vldr.32 s2,[r10]
  00094b86: ldr r0,[r0,#0x0]
  00094b88: ldr r0,[r0,#0x14]
  00094b8a: ldr.w r0,[r0,r5,lsl #0x2]
  00094b8e: vldr.32 s0,[r0,#0x10]
  00094b92: vcmpe.f32 s2,s0
  00094b96: vmrs apsr,fpscr
  00094b9a: beq 0x00094bce
  00094b9c: ldr r1,[sp,#0x8]
  00094b9e: vstr.32 s0,[r10]
  00094ba2: ldrb r1,[r1,#0x0]
  00094ba4: cbz r1,0x00094bb4
  00094ba6: ldrb.w r1,[r11,#0x0]
  00094baa: cbz r1,0x00094bce
  00094bac: movs r1,#0x0
  00094bae: strb.w r1,[r11,#0x0]
  00094bb2: b 0x00094bce
  00094bb4: vmov r2,s0
  00094bb8: mov.w r0,#0x8500
  00094bbc: movw r1,#0x8501
  00094bc0: blx 0x00070744
  00094bc4: ldr r0,[r4,#0x30]
  00094bc6: ldr r0,[r0,#0x0]
  00094bc8: ldr r0,[r0,#0x14]
  00094bca: ldr.w r0,[r0,r5,lsl #0x2]
  00094bce: ldrb r0,[r0,#0x14]
  00094bd0: mov r1,r6
  00094bd2: cmp r0,#0x0
  00094bd4: ite eq
  00094bd6: movw.eq r0,#0xde1
  00094bda: movw.ne r0,#0x8513
  00094bde: blx 0x0006f244
  00094be2: str.w r6,[r9,r8,lsl #0x2]
  00094be6: ldr r0,[sp,#0xc]
  00094be8: add.w r8,r8,#0x1
  00094bec: adds r1,r0,#0x4
  00094bee: str r1,[sp,#0xc]
  00094bf0: ldr r5,[r0,#0x0]
  00094bf2: adds r0,r5,#0x1
  00094bf4: bne 0x00094b5c
  00094bf6: add.w r0,r8,#0x1f
  00094bfa: mov.w r1,#0xffffffff
  00094bfe: b 0x00094c06
  00094c00: str.w r1,[r4,r0,lsl #0x2]
  00094c04: adds r0,#0x1
  00094c06: sub.w r2,r0,#0x1f
  00094c0a: cmp r2,#0x14
  00094c0c: bcc 0x00094c00
  00094c0e: movw r0,#0x84c0
  00094c12: blx 0x0006fb8c
  00094c16: ldr r1,[sp,#0x4]
  00094c18: ldr r0,[sp,#0x10]
  00094c1a: ldr r1,[r1,#0x0]
  00094c1c: subs r0,r1,r0
  00094c1e: itttt eq
  00094c20: add.eq sp,#0x14
  00094c22: pop.eq.w {r8,r9,r10,r11}
  00094c26: pop.eq.w {r4,r5,r6,r7,lr}
  00094c2a: add.eq sp,#0x8
  00094c2c: it eq
  00094c2e: bx.eq lr
  00094c30: blx 0x0006e824
```
