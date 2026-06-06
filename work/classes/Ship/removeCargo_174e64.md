# Ship::removeCargo
elf 0x174e64 body 118
Sig: undefined __stdcall removeCargo(int param_1, int param_2)

## decompile
```c

/* Ship::removeCargo(int, int) */

undefined4 Ship::removeCargo(int param_1,int param_2)

{
  int iVar1;
  Array *pAVar2;
  int in_r2;
  uint uVar3;
  
  pAVar2 = *(Array **)(param_1 + 0x70);
  if (pAVar2 != (Array *)0x0) {
    for (uVar3 = 0; uVar3 < *(uint *)pAVar2; uVar3 = uVar3 + 1) {
      iVar1 = Item::getIndex(*(Item **)(*(uint *)(pAVar2 + 4) + uVar3 * 4));
      if (iVar1 == param_2) {
        Item::changeAmount(*(Item **)(*(int *)(*(int *)(param_1 + 0x70) + 4) + uVar3 * 4),-in_r2);
        iVar1 = Item::getAmount();
        pAVar2 = *(Array **)(param_1 + 0x70);
        if (iVar1 < 1) {
          pAVar2 = (Array *)Item::extractItems(pAVar2,true);
          setCargo((Ship *)param_1,pAVar2);
          return 1;
        }
        break;
      }
      pAVar2 = *(Array **)(param_1 + 0x70);
    }
    setCargo((Ship *)param_1,pAVar2);
  }
  return 0;
}

```
## target disasm
```
  00184e64: push {r4,r5,r6,r7,lr}
  00184e66: add r7,sp,#0xc
  00184e68: push.w r8
  00184e6c: mov r6,r1
  00184e6e: ldr r1,[r0,#0x70]
  00184e70: mov r8,r2
  00184e72: mov r4,r0
  00184e74: cbz r1,0x00184ebc
  00184e76: movs r5,#0x0
  00184e78: b 0x00184e8c
  00184e7a: ldr r0,[r1,#0x4]
  00184e7c: ldr.w r0,[r0,r5,lsl #0x2]
  00184e80: blx 0x000718d8
  00184e84: cmp r0,r6
  00184e86: beq 0x00184e94
  00184e88: ldr r1,[r4,#0x70]
  00184e8a: adds r5,#0x1
  00184e8c: ldr r0,[r1,#0x0]
  00184e8e: cmp r5,r0
  00184e90: bcc 0x00184e7a
  00184e92: b 0x00184eb6
  00184e94: ldr r0,[r4,#0x70]
  00184e96: rsb.w r1,r8,#0x0
  00184e9a: ldr r0,[r0,#0x4]
  00184e9c: ldr.w r0,[r0,r5,lsl #0x2]
  00184ea0: blx 0x00071854
  00184ea4: ldr r0,[r4,#0x70]
  00184ea6: ldr r0,[r0,#0x4]
  00184ea8: ldr.w r0,[r0,r5,lsl #0x2]
  00184eac: blx 0x0007183c
  00184eb0: ldr r1,[r4,#0x70]
  00184eb2: cmp r0,#0x0
  00184eb4: ble 0x00184ec6
  00184eb6: mov r0,r4
  00184eb8: blx 0x000738b8
  00184ebc: movs r5,#0x0
  00184ebe: mov r0,r5
  00184ec0: pop.w r8
  00184ec4: pop {r4,r5,r6,r7,pc}
  00184ec6: mov r0,r1
  00184ec8: movs r1,#0x1
  00184eca: movs r5,#0x1
  00184ecc: blx 0x000770d4
  00184ed0: mov r1,r0
  00184ed2: mov r0,r4
  00184ed4: blx 0x000738b8
  00184ed8: b 0x00184ebe
```
