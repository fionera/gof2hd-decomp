# Player::refillGunDelay
elf 0xa39dc body 54
Sig: undefined __thiscall refillGunDelay(Player * this, int param_1)

## decompile
```c

/* Player::refillGunDelay(int) */

void __thiscall Player::refillGunDelay(Player *this,int param_1)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  
  puVar1 = *(uint **)this;
  if ((((puVar1 != (uint *)0x0) && (-1 < param_1)) && ((uint)param_1 < *puVar1)) &&
     (piVar5 = *(int **)(puVar1[1] + param_1 * 4), piVar5 != (int *)0x0)) {
    iVar2 = *piVar5;
    for (iVar3 = 0; iVar2 != iVar3; iVar3 = iVar3 + 1) {
      iVar4 = *(int *)(piVar5[1] + iVar3 * 4);
      *(undefined4 *)(iVar4 + 0x6c) = *(undefined4 *)(iVar4 + 0x48);
    }
  }
  return;
}

```
## target disasm
```
  000b39dc: ldr r0,[r0,#0x0]
  000b39de: cbz r0,0x000b3a10
  000b39e0: cmp r1,#0x0
  000b39e2: blt 0x000b3a10
  000b39e4: ldr r2,[r0,#0x0]
  000b39e6: cmp r2,r1
  000b39e8: bls 0x000b3a10
  000b39ea: ldr r0,[r0,#0x4]
  000b39ec: ldr.w r12,[r0,r1,lsl #0x2]
  000b39f0: cmp.w r12,#0x0
  000b39f4: beq 0x000b3a10
  000b39f6: ldr.w r1,[r12,#0x0]
  000b39fa: movs r2,#0x0
  000b39fc: b 0x000b3a0c
  000b39fe: ldr.w r3,[r12,#0x4]
  000b3a02: ldr.w r3,[r3,r2,lsl #0x2]
  000b3a06: adds r2,#0x1
  000b3a08: ldr r0,[r3,#0x48]
  000b3a0a: str r0,[r3,#0x6c]
  000b3a0c: cmp r1,r2
  000b3a0e: bne 0x000b39fe
  000b3a10: bx lr
```
