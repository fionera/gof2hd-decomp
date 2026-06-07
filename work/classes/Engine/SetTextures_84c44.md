# Engine::SetTextures
elf 0x84c44 body 292
Sig: undefined __thiscall SetTextures(Engine * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::Engine::SetTextures(unsigned int, unsigned int) */

void __thiscall AbyssEngine::Engine::SetTextures(Engine *this,uint param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  float fVar5;
  
  iVar1 = **(int **)(this + 0x30);
  if ((*(int *)(iVar1 + 0x10) != 0) && (uVar3 = *(int *)(iVar1 + 0x10) - 1, param_1 <= uVar3)) {
    iVar4 = **(int **)(*(int *)(iVar1 + 0x14) + param_1 * 4);
    if (*(int *)(this + 0x7c) != iVar4) {
      glActiveTexture(0x84c0);
      iVar1 = DAT_00094d6c;
      fVar5 = *(float *)(*(int *)(*(int *)(**(int **)(this + 0x30) + 0x14) + param_1 * 4) + 0x10);
      if (**(float **)(DAT_00094d68 + 0x94c86) != fVar5) {
        **(float **)(DAT_00094d68 + 0x94c86) = fVar5;
        if (**(char **)(iVar1 + 0x94ca6) == '\0') {
          glTexEnvf(0x8500,0x8501,fVar5);
        }
        else if (**(char **)(DAT_00094d70 + 0x94cb0) != '\0') {
          **(char **)(DAT_00094d70 + 0x94cb0) = '\0';
        }
      }
      glBindTexture(0xde1,iVar4);
      *(int *)(this + 0x7c) = iVar4;
      iVar1 = **(int **)(this + 0x30);
      uVar3 = *(int *)(iVar1 + 0x10) - 1;
    }
    if (uVar3 < param_2) {
      if (*(int *)(this + 0x80) != -1) {
        if (**(char **)(DAT_00094d74 + 0x94cf0) == '\0') {
          glActiveTexture(0x84c1);
          glDisable(0xde1);
          glActiveTexture(0x84c0);
        }
        *(undefined4 *)(this + 0x80) = 0xffffffff;
      }
    }
    else {
      iVar1 = **(int **)(*(int *)(iVar1 + 0x14) + param_2 * 4);
      if (*(int *)(this + 0x80) != iVar1) {
        glActiveTexture(0x84c1);
        GlEnable((uint)this,true);
        if ((**(char **)(DAT_00094d78 + 0x94d40) == '\0') ||
           ((*(uint *)(this + 0x420) & 0x80008) == 0)) {
          uVar2 = 0xde1;
        }
        else {
          uVar2 = 0x8513;
        }
        glBindTexture(uVar2,iVar1);
        *(int *)(this + 0x80) = iVar1;
      }
    }
  }
  return;
}

```

## target disasm
```
  00094c44: push {r4,r5,r6,r7,lr}
  00094c46: add r7,sp,#0xc
  00094c48: push.w r8
  00094c4c: mov r4,r0
  00094c4e: ldr r0,[r0,#0x30]
  00094c50: mov r5,r1
  00094c52: mov r8,r2
  00094c54: ldr r0,[r0,#0x0]
  00094c56: ldr r1,[r0,#0x10]
  00094c58: cmp r1,#0x0
  00094c5a: beq.w 0x00094d62
  00094c5e: subs r1,#0x1
  00094c60: cmp r1,r5
  00094c62: bcc 0x00094d62
  00094c64: ldr r2,[r0,#0x14]
  00094c66: ldr r3,[r4,#0x7c]
  00094c68: ldr.w r2,[r2,r5,lsl #0x2]
  00094c6c: ldr r6,[r2,#0x0]
  00094c6e: cmp r3,r6
  00094c70: beq 0x00094cde
  00094c72: movw r0,#0x84c0
  00094c76: blx 0x0006fb8c
  00094c7a: ldr r0,[r4,#0x30]
  00094c7c: ldr r0,[r0,#0x0]
  00094c7e: ldr r1,[0x00094d68]
  00094c80: ldr r2,[r0,#0x14]
  00094c82: add r1,pc
  00094c84: ldr r0,[r1,#0x0]
  00094c86: ldr.w r1,[r2,r5,lsl #0x2]
  00094c8a: vldr.32 s2,[r0]
  00094c8e: vldr.32 s0,[r1,#0x10]
  00094c92: vcmpe.f32 s2,s0
  00094c96: vmrs apsr,fpscr
  00094c9a: beq 0x00094cca
  00094c9c: ldr r1,[0x00094d6c]
  00094c9e: vstr.32 s0,[r0]
  00094ca2: add r1,pc
  00094ca4: ldr r1,[r1,#0x0]
  00094ca6: ldrb r1,[r1,#0x0]
  00094ca8: cbz r1,0x00094cba
  00094caa: ldr r0,[0x00094d70]
  00094cac: add r0,pc
  00094cae: ldr r0,[r0,#0x0]
  00094cb0: ldrb r1,[r0,#0x0]
  00094cb2: cbz r1,0x00094cca
  00094cb4: movs r1,#0x0
  00094cb6: strb r1,[r0,#0x0]
  00094cb8: b 0x00094cca
  00094cba: vmov r2,s0
  00094cbe: mov.w r0,#0x8500
  00094cc2: movw r1,#0x8501
  00094cc6: blx 0x00070744
  00094cca: movw r0,#0xde1
  00094cce: mov r1,r6
  00094cd0: blx 0x0006f244
  00094cd4: ldr r0,[r4,#0x30]
  00094cd6: str r6,[r4,#0x7c]
  00094cd8: ldr r0,[r0,#0x0]
  00094cda: ldr r1,[r0,#0x10]
  00094cdc: subs r1,#0x1
  00094cde: cmp r1,r8
  00094ce0: bcs 0x00094d16
  00094ce2: ldr.w r0,[r4,#0x80]
  00094ce6: adds r0,#0x1
  00094ce8: beq 0x00094d62
  00094cea: ldr r0,[0x00094d74]
  00094cec: add r0,pc
  00094cee: ldr r0,[r0,#0x0]
  00094cf0: ldrb r0,[r0,#0x0]
  00094cf2: cbnz r0,0x00094d0c
  00094cf4: movw r0,#0x84c1
  00094cf8: blx 0x0006fb8c
  00094cfc: movw r0,#0xde1
  00094d00: blx 0x0006edc4
  00094d04: movw r0,#0x84c0
  00094d08: blx 0x0006fb8c
  00094d0c: mov.w r0,#0xffffffff
  00094d10: str.w r0,[r4,#0x80]
  00094d14: b 0x00094d62
  00094d16: ldr r0,[r0,#0x14]
  00094d18: ldr.w r1,[r4,#0x80]
  00094d1c: ldr.w r0,[r0,r8,lsl #0x2]
  00094d20: ldr r5,[r0,#0x0]
  00094d22: cmp r1,r5
  00094d24: beq 0x00094d62
  00094d26: movw r0,#0x84c1
  00094d2a: blx 0x0006fb8c
  00094d2e: mov r0,r4
  00094d30: movw r1,#0xde1
  00094d34: movs r2,#0x1
  00094d36: blx 0x0006eddc
  00094d3a: ldr r0,[0x00094d78]
  00094d3c: add r0,pc
  00094d3e: ldr r0,[r0,#0x0]
  00094d40: ldrb r0,[r0,#0x0]
  00094d42: cbz r0,0x00094d54
  00094d44: ldr.w r0,[r4,#0x420]
  00094d48: tst r0,#0x80008
  00094d4c: beq 0x00094d54
  00094d4e: movw r0,#0x8513
  00094d52: b 0x00094d58
  00094d54: movw r0,#0xde1
  00094d58: mov r1,r5
  00094d5a: blx 0x0006f244
  00094d5e: str.w r5,[r4,#0x80]
  00094d62: pop.w r8
  00094d66: pop {r4,r5,r6,r7,pc}
```
