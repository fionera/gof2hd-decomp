# Level::render
elf 0xc4b10 body 402
Sig: undefined __stdcall render(int param_1)

## decompile
```c

/* Level::render(int) */

void Level::render(int param_1)

{
  uint *puVar1;
  int *piVar2;
  uint uVar3;
  
  puVar1 = *(uint **)(param_1 + 0xe4);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0x14))();
      puVar1 = *(uint **)(param_1 + 0xe4);
    }
  }
  puVar1 = *(uint **)(param_1 + 0xe8);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0x14))();
      puVar1 = *(uint **)(param_1 + 0xe8);
    }
  }
  puVar1 = *(uint **)(param_1 + 0xf8);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0x34))();
      (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0xf8) + 4) + uVar3 * 4) + 0x24))();
      puVar1 = *(uint **)(param_1 + 0xf8);
    }
  }
  puVar1 = *(uint **)(param_1 + 0xfc);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0x34))();
      (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0xfc) + 4) + uVar3 * 4) + 0x24))();
      puVar1 = *(uint **)(param_1 + 0xfc);
    }
  }
  puVar1 = *(uint **)(param_1 + 0xf4);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0x34))();
      (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0xf4) + 4) + uVar3 * 4) + 0x24))();
      puVar1 = *(uint **)(param_1 + 0xf4);
    }
  }
  puVar1 = *(uint **)(param_1 + 0x100);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      piVar2 = *(int **)(puVar1[1] + uVar3 * 4);
      if (piVar2 != (int *)0x0) {
        (**(code **)(*piVar2 + 0x34))();
        (**(code **)(**(int **)(*(int *)(*(int *)(param_1 + 0x100) + 4) + uVar3 * 4) + 0x24))();
        puVar1 = *(uint **)(param_1 + 0x100);
      }
    }
  }
  if (*(ParticleSystemManager **)(param_1 + 0x80) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x80));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x74) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x74));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x78) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x78));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x7c) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x7c));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x88) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x88));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x84) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x84));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x8c) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x8c));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x98) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x98));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x94) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x94));
  }
  if (*(ParticleSystemManager **)(param_1 + 0x9c) != (ParticleSystemManager *)0x0) {
    ParticleSystemManager::render3d(*(ParticleSystemManager **)(param_1 + 0x9c));
  }
  (*(code *)(DAT_001abfd4 + 0x1abfd8))(*(undefined4 *)(param_1 + 0xec));
  return;
}

```
## target disasm
```
  000d4b10: push {r4,r5,r6,r7,lr}
  000d4b12: add r7,sp,#0xc
  000d4b14: push.w r11
  000d4b18: mov r4,r0
  000d4b1a: ldr.w r0,[r0,#0xe4]
  000d4b1e: mov r5,r1
  000d4b20: cbz r0,0x000d4b3e
  000d4b22: movs r6,#0x0
  000d4b24: b 0x000d4b38
  000d4b26: ldr r0,[r0,#0x4]
  000d4b28: ldr.w r0,[r0,r6,lsl #0x2]
  000d4b2c: ldr r1,[r0,#0x0]
  000d4b2e: ldr r1,[r1,#0x14]
  000d4b30: blx r1
  000d4b32: ldr.w r0,[r4,#0xe4]
  000d4b36: adds r6,#0x1
  000d4b38: ldr r1,[r0,#0x0]
  000d4b3a: cmp r6,r1
  000d4b3c: bcc 0x000d4b26
  000d4b3e: ldr.w r0,[r4,#0xe8]
  000d4b42: cbz r0,0x000d4b60
  000d4b44: movs r6,#0x0
  000d4b46: b 0x000d4b5a
  000d4b48: ldr r0,[r0,#0x4]
  000d4b4a: ldr.w r0,[r0,r6,lsl #0x2]
  000d4b4e: ldr r1,[r0,#0x0]
  000d4b50: ldr r1,[r1,#0x14]
  000d4b52: blx r1
  000d4b54: ldr.w r0,[r4,#0xe8]
  000d4b58: adds r6,#0x1
  000d4b5a: ldr r1,[r0,#0x0]
  000d4b5c: cmp r6,r1
  000d4b5e: bcc 0x000d4b48
  000d4b60: ldr.w r0,[r4,#0xf8]
  000d4b64: cbz r0,0x000d4b94
  000d4b66: movs r6,#0x0
  000d4b68: b 0x000d4b8e
  000d4b6a: ldr r0,[r0,#0x4]
  000d4b6c: ldr.w r0,[r0,r6,lsl #0x2]
  000d4b70: ldr r1,[r0,#0x0]
  000d4b72: ldr r2,[r1,#0x34]
  000d4b74: mov r1,r5
  000d4b76: blx r2
  000d4b78: ldr.w r0,[r4,#0xf8]
  000d4b7c: ldr r0,[r0,#0x4]
  000d4b7e: ldr.w r0,[r0,r6,lsl #0x2]
  000d4b82: ldr r1,[r0,#0x0]
  000d4b84: ldr r1,[r1,#0x24]
  000d4b86: blx r1
  000d4b88: ldr.w r0,[r4,#0xf8]
  000d4b8c: adds r6,#0x1
  000d4b8e: ldr r1,[r0,#0x0]
  000d4b90: cmp r6,r1
  000d4b92: bcc 0x000d4b6a
  000d4b94: ldr.w r0,[r4,#0xfc]
  000d4b98: cbz r0,0x000d4bc8
  000d4b9a: movs r6,#0x0
  000d4b9c: b 0x000d4bc2
  000d4b9e: ldr r0,[r0,#0x4]
  000d4ba0: ldr.w r0,[r0,r6,lsl #0x2]
  000d4ba4: ldr r1,[r0,#0x0]
  000d4ba6: ldr r2,[r1,#0x34]
  000d4ba8: mov r1,r5
  000d4baa: blx r2
  000d4bac: ldr.w r0,[r4,#0xfc]
  000d4bb0: ldr r0,[r0,#0x4]
  000d4bb2: ldr.w r0,[r0,r6,lsl #0x2]
  000d4bb6: ldr r1,[r0,#0x0]
  000d4bb8: ldr r1,[r1,#0x24]
  000d4bba: blx r1
  000d4bbc: ldr.w r0,[r4,#0xfc]
  000d4bc0: adds r6,#0x1
  000d4bc2: ldr r1,[r0,#0x0]
  000d4bc4: cmp r6,r1
  000d4bc6: bcc 0x000d4b9e
  000d4bc8: ldr.w r0,[r4,#0xf4]
  000d4bcc: cbz r0,0x000d4bfc
  000d4bce: movs r6,#0x0
  000d4bd0: b 0x000d4bf6
  000d4bd2: ldr r0,[r0,#0x4]
  000d4bd4: ldr.w r0,[r0,r6,lsl #0x2]
  000d4bd8: ldr r1,[r0,#0x0]
  000d4bda: ldr r2,[r1,#0x34]
  000d4bdc: mov r1,r5
  000d4bde: blx r2
  000d4be0: ldr.w r0,[r4,#0xf4]
  000d4be4: ldr r0,[r0,#0x4]
  000d4be6: ldr.w r0,[r0,r6,lsl #0x2]
  000d4bea: ldr r1,[r0,#0x0]
  000d4bec: ldr r1,[r1,#0x24]
  000d4bee: blx r1
  000d4bf0: ldr.w r0,[r4,#0xf4]
  000d4bf4: adds r6,#0x1
  000d4bf6: ldr r1,[r0,#0x0]
  000d4bf8: cmp r6,r1
  000d4bfa: bcc 0x000d4bd2
  000d4bfc: ldr.w r1,[r4,#0x100]
  000d4c00: cbz r1,0x000d4c32
  000d4c02: movs r6,#0x0
  000d4c04: b 0x000d4c2c
  000d4c06: ldr r0,[r1,#0x4]
  000d4c08: ldr.w r0,[r0,r6,lsl #0x2]
  000d4c0c: cbz r0,0x000d4c2a
  000d4c0e: ldr r1,[r0,#0x0]
  000d4c10: ldr r2,[r1,#0x34]
  000d4c12: mov r1,r5
  000d4c14: blx r2
  000d4c16: ldr.w r0,[r4,#0x100]
  000d4c1a: ldr r0,[r0,#0x4]
  000d4c1c: ldr.w r0,[r0,r6,lsl #0x2]
  000d4c20: ldr r1,[r0,#0x0]
  000d4c22: ldr r1,[r1,#0x24]
  000d4c24: blx r1
  000d4c26: ldr.w r1,[r4,#0x100]
  000d4c2a: adds r6,#0x1
  000d4c2c: ldr r0,[r1,#0x0]
  000d4c2e: cmp r6,r0
  000d4c30: bcc 0x000d4c06
  000d4c32: ldr.w r0,[r4,#0x80]
  000d4c36: cbz r0,0x000d4c3c
  000d4c38: blx 0x00074440
  000d4c3c: ldr r0,[r4,#0x74]
  000d4c3e: cbz r0,0x000d4c44
  000d4c40: blx 0x00074440
  000d4c44: ldr r0,[r4,#0x78]
  000d4c46: cbz r0,0x000d4c4c
  000d4c48: blx 0x00074440
  000d4c4c: ldr r0,[r4,#0x7c]
  000d4c4e: cbz r0,0x000d4c54
  000d4c50: blx 0x00074440
  000d4c54: ldr.w r0,[r4,#0x88]
  000d4c58: cbz r0,0x000d4c5e
  000d4c5a: blx 0x00074440
  000d4c5e: ldr.w r0,[r4,#0x84]
  000d4c62: cbz r0,0x000d4c68
  000d4c64: blx 0x00074440
  000d4c68: ldr.w r0,[r4,#0x8c]
  000d4c6c: cbz r0,0x000d4c72
  000d4c6e: blx 0x00074440
  000d4c72: ldr.w r0,[r4,#0x98]
  000d4c76: cbz r0,0x000d4c7c
  000d4c78: blx 0x00074440
  000d4c7c: ldr.w r0,[r4,#0x94]
  000d4c80: cbz r0,0x000d4c86
  000d4c82: blx 0x00074440
  000d4c86: ldr.w r0,[r4,#0x9c]
  000d4c8a: cbz r0,0x000d4c90
  000d4c8c: blx 0x00074440
  000d4c90: ldr.w r0,[r4,#0xec]
  000d4c94: pop.w r11
  000d4c98: pop.w {r4,r5,r6,r7,lr}
  000d4c9c: b.w 0x001abfc8
  001abfc8: bx pc
```
