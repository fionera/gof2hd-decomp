# Player::resetGunDelay
elf 0xa39aa body 50
Sig: undefined __thiscall resetGunDelay(Player * this, int param_1)

## decompile
```c

/* Player::resetGunDelay(int) */

void __thiscall Player::resetGunDelay(Player *this,int param_1)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  puVar1 = *(uint **)this;
  if ((((puVar1 != (uint *)0x0) && (-1 < param_1)) && ((uint)param_1 < *puVar1)) &&
     (piVar2 = *(int **)(puVar1[1] + param_1 * 4), piVar2 != (int *)0x0)) {
    iVar3 = *piVar2;
    for (iVar4 = 0; iVar3 != iVar4; iVar4 = iVar4 + 1) {
      *(undefined4 *)(*(int *)(piVar2[1] + iVar4 * 4) + 0x6c) = 0;
    }
  }
  return;
}

```
## target disasm
```
  000b39aa: ldr r0,[r0,#0x0]
  000b39ac: cbz r0,0x000b39da
  000b39ae: cmp r1,#0x0
  000b39b0: blt 0x000b39da
  000b39b2: ldr r2,[r0,#0x0]
  000b39b4: cmp r2,r1
  000b39b6: bls 0x000b39da
  000b39b8: ldr r0,[r0,#0x4]
  000b39ba: ldr.w r0,[r0,r1,lsl #0x2]
  000b39be: cbz r0,0x000b39da
  000b39c0: ldr r1,[r0,#0x0]
  000b39c2: mov.w r12,#0x0
  000b39c6: movs r3,#0x0
  000b39c8: b 0x000b39d6
  000b39ca: ldr r2,[r0,#0x4]
  000b39cc: ldr.w r2,[r2,r3,lsl #0x2]
  000b39d0: adds r3,#0x1
  000b39d2: str.w r12,[r2,#0x6c]
  000b39d6: cmp r1,r3
  000b39d8: bne 0x000b39ca
  000b39da: bx lr
```
