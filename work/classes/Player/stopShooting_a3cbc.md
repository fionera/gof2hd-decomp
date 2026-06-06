# Player::stopShooting
elf 0xa3cbc body 76
Sig: undefined __stdcall stopShooting(int param_1)

## decompile
```c

/* Player::stopShooting(int) */

void Player::stopShooting(int param_1)

{
  uint *puVar1;
  int iVar2;
  uint in_r1;
  uint uVar3;
  
  puVar1 = *(uint **)param_1;
  if ((((puVar1 != (uint *)0x0) && (-1 < (int)in_r1)) && (in_r1 < *puVar1)) &&
     (puVar1 = *(uint **)(puVar1[1] + in_r1 * 4), puVar1 != (uint *)0x0)) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      iVar2 = *(int *)(puVar1[1] + uVar3 * 4);
      stopShootSound((Player *)param_1,*(int *)(iVar2 + 0x58),*(int *)(iVar2 + 0x5c));
      puVar1 = *(uint **)(*(int *)(*(int *)param_1 + 4) + in_r1 * 4);
    }
  }
  return;
}

```
## target disasm
```
  000b3cbc: push {r4,r5,r6,r7,lr}
  000b3cbe: add r7,sp,#0xc
  000b3cc0: push.w r11
  000b3cc4: mov r5,r0
  000b3cc6: ldr r0,[r0,#0x0]
  000b3cc8: mov r4,r1
  000b3cca: cbz r0,0x000b3d02
  000b3ccc: cmp r4,#0x0
  000b3cce: blt 0x000b3d02
  000b3cd0: ldr r1,[r0,#0x0]
  000b3cd2: cmp r1,r4
  000b3cd4: bls 0x000b3d02
  000b3cd6: ldr r0,[r0,#0x4]
  000b3cd8: ldr.w r0,[r0,r4,lsl #0x2]
  000b3cdc: cbz r0,0x000b3d02
  000b3cde: movs r6,#0x0
  000b3ce0: b 0x000b3cfc
  000b3ce2: ldr r0,[r0,#0x4]
  000b3ce4: ldr.w r0,[r0,r6,lsl #0x2]
  000b3ce8: ldrd r1,r2,[r0,#0x58]
  000b3cec: mov r0,r5
  000b3cee: blx 0x00072fd0
  000b3cf2: ldr r0,[r5,#0x0]
  000b3cf4: adds r6,#0x1
  000b3cf6: ldr r0,[r0,#0x4]
  000b3cf8: ldr.w r0,[r0,r4,lsl #0x2]
  000b3cfc: ldr r1,[r0,#0x0]
  000b3cfe: cmp r6,r1
  000b3d00: bcc 0x000b3ce2
  000b3d02: pop.w r11
  000b3d06: pop {r4,r5,r6,r7,pc}
```
