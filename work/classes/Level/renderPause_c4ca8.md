# Level::renderPause
elf 0xc4ca8 body 214
Sig: undefined __stdcall renderPause(longlong param_1)

## decompile
```c

/* Level::renderPause(long long) */

void Level::renderPause(longlong param_1)

{
  int iVar1;
  uint *puVar2;
  int *piVar3;
  uint uVar4;
  
  iVar1 = (int)param_1;
  puVar2 = *(uint **)(iVar1 + 0xe4);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      (**(code **)(**(int **)(puVar2[1] + uVar4 * 4) + 0x14))();
      puVar2 = *(uint **)(iVar1 + 0xe4);
    }
  }
  puVar2 = *(uint **)(iVar1 + 0xe8);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      (**(code **)(**(int **)(puVar2[1] + uVar4 * 4) + 0x14))();
      puVar2 = *(uint **)(iVar1 + 0xe8);
    }
  }
  puVar2 = *(uint **)(iVar1 + 0xf8);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      (**(code **)(**(int **)(puVar2[1] + uVar4 * 4) + 0x24))();
      puVar2 = *(uint **)(iVar1 + 0xf8);
    }
  }
  puVar2 = *(uint **)(iVar1 + 0xfc);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      (**(code **)(**(int **)(puVar2[1] + uVar4 * 4) + 0x24))();
      puVar2 = *(uint **)(iVar1 + 0xfc);
    }
  }
  puVar2 = *(uint **)(iVar1 + 0xf4);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      (**(code **)(**(int **)(puVar2[1] + uVar4 * 4) + 0x24))();
      puVar2 = *(uint **)(iVar1 + 0xf4);
    }
  }
  puVar2 = *(uint **)(iVar1 + 0x100);
  if (puVar2 != (uint *)0x0) {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      piVar3 = *(int **)(puVar2[1] + uVar4 * 4);
      if (piVar3 != (int *)0x0) {
        (**(code **)(*piVar3 + 0x24))();
        puVar2 = *(uint **)(iVar1 + 0x100);
      }
    }
  }
  return;
}

```
## target disasm
```
  000d4ca8: push {r4,r5,r7,lr}
  000d4caa: add r7,sp,#0x8
  000d4cac: mov r4,r0
  000d4cae: ldr.w r0,[r0,#0xe4]
  000d4cb2: cbz r0,0x000d4cd0
  000d4cb4: movs r5,#0x0
  000d4cb6: b 0x000d4cca
  000d4cb8: ldr r0,[r0,#0x4]
  000d4cba: ldr.w r0,[r0,r5,lsl #0x2]
  000d4cbe: ldr r1,[r0,#0x0]
  000d4cc0: ldr r1,[r1,#0x14]
  000d4cc2: blx r1
  000d4cc4: ldr.w r0,[r4,#0xe4]
  000d4cc8: adds r5,#0x1
  000d4cca: ldr r1,[r0,#0x0]
  000d4ccc: cmp r5,r1
  000d4cce: bcc 0x000d4cb8
  000d4cd0: ldr.w r0,[r4,#0xe8]
  000d4cd4: cbz r0,0x000d4cf2
  000d4cd6: movs r5,#0x0
  000d4cd8: b 0x000d4cec
  000d4cda: ldr r0,[r0,#0x4]
  000d4cdc: ldr.w r0,[r0,r5,lsl #0x2]
  000d4ce0: ldr r1,[r0,#0x0]
  000d4ce2: ldr r1,[r1,#0x14]
  000d4ce4: blx r1
  000d4ce6: ldr.w r0,[r4,#0xe8]
  000d4cea: adds r5,#0x1
  000d4cec: ldr r1,[r0,#0x0]
  000d4cee: cmp r5,r1
  000d4cf0: bcc 0x000d4cda
  000d4cf2: ldr.w r0,[r4,#0xf8]
  000d4cf6: cbz r0,0x000d4d14
  000d4cf8: movs r5,#0x0
  000d4cfa: b 0x000d4d0e
  000d4cfc: ldr r0,[r0,#0x4]
  000d4cfe: ldr.w r0,[r0,r5,lsl #0x2]
  000d4d02: ldr r1,[r0,#0x0]
  000d4d04: ldr r1,[r1,#0x24]
  000d4d06: blx r1
  000d4d08: ldr.w r0,[r4,#0xf8]
  000d4d0c: adds r5,#0x1
  000d4d0e: ldr r1,[r0,#0x0]
  000d4d10: cmp r5,r1
  000d4d12: bcc 0x000d4cfc
  000d4d14: ldr.w r0,[r4,#0xfc]
  000d4d18: cbz r0,0x000d4d36
  000d4d1a: movs r5,#0x0
  000d4d1c: b 0x000d4d30
  000d4d1e: ldr r0,[r0,#0x4]
  000d4d20: ldr.w r0,[r0,r5,lsl #0x2]
  000d4d24: ldr r1,[r0,#0x0]
  000d4d26: ldr r1,[r1,#0x24]
  000d4d28: blx r1
  000d4d2a: ldr.w r0,[r4,#0xfc]
  000d4d2e: adds r5,#0x1
  000d4d30: ldr r1,[r0,#0x0]
  000d4d32: cmp r5,r1
  000d4d34: bcc 0x000d4d1e
  000d4d36: ldr.w r0,[r4,#0xf4]
  000d4d3a: cbz r0,0x000d4d58
  000d4d3c: movs r5,#0x0
  000d4d3e: b 0x000d4d52
  000d4d40: ldr r0,[r0,#0x4]
  000d4d42: ldr.w r0,[r0,r5,lsl #0x2]
  000d4d46: ldr r1,[r0,#0x0]
  000d4d48: ldr r1,[r1,#0x24]
  000d4d4a: blx r1
  000d4d4c: ldr.w r0,[r4,#0xf4]
  000d4d50: adds r5,#0x1
  000d4d52: ldr r1,[r0,#0x0]
  000d4d54: cmp r5,r1
  000d4d56: bcc 0x000d4d40
  000d4d58: ldr.w r1,[r4,#0x100]
  000d4d5c: cbz r1,0x000d4d7c
  000d4d5e: movs r5,#0x0
  000d4d60: b 0x000d4d76
  000d4d62: ldr r0,[r1,#0x4]
  000d4d64: ldr.w r0,[r0,r5,lsl #0x2]
  000d4d68: cbz r0,0x000d4d74
  000d4d6a: ldr r1,[r0,#0x0]
  000d4d6c: ldr r1,[r1,#0x24]
  000d4d6e: blx r1
  000d4d70: ldr.w r1,[r4,#0x100]
  000d4d74: adds r5,#0x1
  000d4d76: ldr r0,[r1,#0x0]
  000d4d78: cmp r5,r0
  000d4d7a: bcc 0x000d4d62
  000d4d7c: pop {r4,r5,r7,pc}
```
