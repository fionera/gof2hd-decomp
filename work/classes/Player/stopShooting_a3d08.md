# Player::stopShooting
elf 0xa3d08 body 84
Sig: undefined __thiscall stopShooting(Player * this, int param_1, int param_2)

## decompile
```c

/* Player::stopShooting(int, int) */

void __thiscall Player::stopShooting(Player *this,int param_1,int param_2)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  
  if ((((param_2 - 0x16U < 9) && (puVar1 = *(uint **)this, puVar1 != (uint *)0x0)) && (-1 < param_1)
      ) && (((uint)param_1 < *puVar1 &&
            (puVar1 = *(uint **)(puVar1[1] + param_1 * 4), puVar1 != (uint *)0x0)))) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      iVar2 = *(int *)(puVar1[1] + uVar3 * 4);
      stopShootSound(this,*(int *)(iVar2 + 0x58),*(int *)(iVar2 + 0x5c));
      puVar1 = *(uint **)(*(int *)(*(int *)this + 4) + param_1 * 4);
    }
  }
  return;
}

```
## target disasm
```
  000b3d08: push {r4,r5,r6,r7,lr}
  000b3d0a: add r7,sp,#0xc
  000b3d0c: push.w r11
  000b3d10: mov r5,r0
  000b3d12: sub.w r0,r2,#0x16
  000b3d16: mov r4,r1
  000b3d18: cmp r0,#0x8
  000b3d1a: bhi 0x000b3d56
  000b3d1c: ldr r0,[r5,#0x0]
  000b3d1e: cbz r0,0x000b3d56
  000b3d20: cmp r4,#0x0
  000b3d22: blt 0x000b3d56
  000b3d24: ldr r1,[r0,#0x0]
  000b3d26: cmp r1,r4
  000b3d28: bls 0x000b3d56
  000b3d2a: ldr r0,[r0,#0x4]
  000b3d2c: ldr.w r0,[r0,r4,lsl #0x2]
  000b3d30: cbz r0,0x000b3d56
  000b3d32: movs r6,#0x0
  000b3d34: b 0x000b3d50
  000b3d36: ldr r0,[r0,#0x4]
  000b3d38: ldr.w r0,[r0,r6,lsl #0x2]
  000b3d3c: ldrd r1,r2,[r0,#0x58]
  000b3d40: mov r0,r5
  000b3d42: blx 0x00072fd0
  000b3d46: ldr r0,[r5,#0x0]
  000b3d48: adds r6,#0x1
  000b3d4a: ldr r0,[r0,#0x4]
  000b3d4c: ldr.w r0,[r0,r4,lsl #0x2]
  000b3d50: ldr r1,[r0,#0x0]
  000b3d52: cmp r6,r1
  000b3d54: bcc 0x000b3d36
  000b3d56: pop.w r11
  000b3d5a: pop {r4,r5,r6,r7,pc}
```
