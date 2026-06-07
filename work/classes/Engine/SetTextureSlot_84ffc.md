# Engine::SetTextureSlot
elf 0x84ffc body 184
Sig: undefined __stdcall SetTextureSlot(uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::Engine::SetTextureSlot(unsigned int, unsigned int) */

void AbyssEngine::Engine::SetTextureSlot(uint param_1,uint param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  uint in_r2;
  float fVar6;
  
  iVar5 = *(int *)(**(int **)(param_1 + 0x30) + 0x10);
  if (((iVar5 != 0) && (in_r2 < 8)) && (param_2 <= iVar5 - 1U)) {
    piVar1 = (int *)(param_1 + in_r2 * 4 + 0x7c);
    iVar5 = **(int **)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + param_2 * 4);
    if (*piVar1 != iVar5) {
      glActiveTexture(in_r2 + 0x84c0);
      iVar2 = DAT_000950b8;
      iVar3 = *(int *)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + param_2 * 4);
      fVar6 = *(float *)(iVar3 + 0x10);
      if (**(float **)(DAT_000950b4 + 0x95048) != fVar6) {
        **(float **)(DAT_000950b4 + 0x95048) = fVar6;
        if (**(char **)(iVar2 + 0x95068) == '\0') {
          glTexEnvf(0x8500,0x8501,fVar6);
          iVar3 = *(int *)(*(int *)(**(int **)(param_1 + 0x30) + 0x14) + param_2 * 4);
        }
        else if (**(char **)(DAT_000950bc + 0x95072) != '\0') {
          **(char **)(DAT_000950bc + 0x95072) = '\0';
        }
      }
      if (*(char *)(iVar3 + 0x14) == '\0') {
        uVar4 = 0xde1;
      }
      else {
        uVar4 = 0x8513;
      }
      glBindTexture(uVar4,iVar5);
      *piVar1 = iVar5;
    }
  }
  return;
}

```

## target disasm
```
  00094ffc: push {r4,r5,r6,r7,lr}
  00094ffe: add r7,sp,#0xc
  00095000: push.w r8
  00095004: mov r6,r0
  00095006: ldr r0,[r0,#0x30]
  00095008: mov r5,r1
  0009500a: ldr r0,[r0,#0x0]
  0009500c: ldr r1,[r0,#0x10]
  0009500e: cmp r1,#0x0
  00095010: beq 0x000950ae
  00095012: cmp r2,#0x7
  00095014: bhi 0x000950ae
  00095016: subs r1,#0x1
  00095018: cmp r1,r5
  0009501a: bcc 0x000950ae
  0009501c: ldr r0,[r0,#0x14]
  0009501e: add.w r4,r6,r2, lsl #0x2
  00095022: ldr.w r1,[r4,#0x7c]!
  00095026: ldr.w r0,[r0,r5,lsl #0x2]
  0009502a: ldr.w r8,[r0,#0x0]
  0009502e: cmp r1,r8
  00095030: beq 0x000950ae
  00095032: movw r0,#0x84c0
  00095036: add r0,r2
  00095038: blx 0x0006fb8c
  0009503c: ldr r0,[r6,#0x30]
  0009503e: ldr r0,[r0,#0x0]
  00095040: ldr r1,[0x000950b4]
  00095042: ldr r0,[r0,#0x14]
  00095044: add r1,pc
  00095046: ldr r1,[r1,#0x0]
  00095048: ldr.w r0,[r0,r5,lsl #0x2]
  0009504c: vldr.32 s2,[r1]
  00095050: vldr.32 s0,[r0,#0x10]
  00095054: vcmpe.f32 s2,s0
  00095058: vmrs apsr,fpscr
  0009505c: beq 0x00095096
  0009505e: ldr r2,[0x000950b8]
  00095060: vstr.32 s0,[r1]
  00095064: add r2,pc
  00095066: ldr r2,[r2,#0x0]
  00095068: ldrb r2,[r2,#0x0]
  0009506a: cbz r2,0x0009507c
  0009506c: ldr r1,[0x000950bc]
  0009506e: add r1,pc
  00095070: ldr r1,[r1,#0x0]
  00095072: ldrb r2,[r1,#0x0]
  00095074: cbz r2,0x00095096
  00095076: movs r2,#0x0
  00095078: strb r2,[r1,#0x0]
  0009507a: b 0x00095096
  0009507c: vmov r2,s0
  00095080: mov.w r0,#0x8500
  00095084: movw r1,#0x8501
  00095088: blx 0x00070744
  0009508c: ldr r0,[r6,#0x30]
  0009508e: ldr r0,[r0,#0x0]
  00095090: ldr r0,[r0,#0x14]
  00095092: ldr.w r0,[r0,r5,lsl #0x2]
  00095096: ldrb r0,[r0,#0x14]
  00095098: mov r1,r8
  0009509a: cmp r0,#0x0
  0009509c: ite eq
  0009509e: movw.eq r0,#0xde1
  000950a2: movw.ne r0,#0x8513
  000950a6: blx 0x0006f244
  000950aa: str.w r8,[r4,#0x0]
  000950ae: pop.w r8
  000950b2: pop {r4,r5,r6,r7,pc}
```
