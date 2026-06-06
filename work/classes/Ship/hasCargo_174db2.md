# Ship::hasCargo
elf 0x174db2 body 76
Sig: undefined __stdcall hasCargo(int param_1, int param_2)

## decompile
```c

/* Ship::hasCargo(int, int) */

undefined4 Ship::hasCargo(int param_1,int param_2)

{
  uint *puVar1;
  Item *this;
  int iVar2;
  int in_r2;
  uint uVar3;
  
  puVar1 = *(uint **)(param_1 + 0x70);
  if (puVar1 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      this = *(Item **)(puVar1[1] + uVar3 * 4);
      if (((this != (Item *)0x0) && (iVar2 = Item::getIndex(this), iVar2 == param_2)) &&
         (iVar2 = Item::getAmount(), in_r2 <= iVar2)) {
        return 1;
      }
      puVar1 = *(uint **)(param_1 + 0x70);
    }
  }
  return 0;
}

```
## target disasm
```
  00184db2: push {r4,r5,r6,r7,lr}
  00184db4: add r7,sp,#0xc
  00184db6: push.w r8
  00184dba: mov r6,r0
  00184dbc: ldr r0,[r0,#0x70]
  00184dbe: mov r8,r2
  00184dc0: mov r5,r1
  00184dc2: cbz r0,0x00184df2
  00184dc4: movs r4,#0x0
  00184dc6: b 0x00184dec
  00184dc8: ldr r0,[r0,#0x4]
  00184dca: ldr.w r0,[r0,r4,lsl #0x2]
  00184dce: cbz r0,0x00184de8
  00184dd0: blx 0x000718d8
  00184dd4: cmp r0,r5
  00184dd6: bne 0x00184de8
  00184dd8: ldr r0,[r6,#0x70]
  00184dda: ldr r0,[r0,#0x4]
  00184ddc: ldr.w r0,[r0,r4,lsl #0x2]
  00184de0: blx 0x0007183c
  00184de4: cmp r0,r8
  00184de6: bge 0x00184dfa
  00184de8: ldr r0,[r6,#0x70]
  00184dea: adds r4,#0x1
  00184dec: ldr r1,[r0,#0x0]
  00184dee: cmp r4,r1
  00184df0: bcc 0x00184dc8
  00184df2: movs r0,#0x0
  00184df4: pop.w r8
  00184df8: pop {r4,r5,r6,r7,pc}
  00184dfa: movs r0,#0x1
  00184dfc: b 0x00184df4
```
