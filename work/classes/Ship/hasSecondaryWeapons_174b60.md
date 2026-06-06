# Ship::hasSecondaryWeapons
elf 0x174b60 body 58
Sig: undefined __thiscall hasSecondaryWeapons(Ship * this)

## decompile
```c

/* Ship::hasSecondaryWeapons() */

undefined4 __thiscall Ship::hasSecondaryWeapons(Ship *this)

{
  Item *this_00;
  int iVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  
  if (((*(int *)(*(int *)(this + 0x68) + 4) != 0) &&
      (puVar2 = *(uint **)(this + 0x6c), puVar2 != (uint *)0x0)) && (uVar3 = *puVar2, uVar3 != 0)) {
    for (uVar4 = 0; uVar4 < uVar3; uVar4 = uVar4 + 1) {
      this_00 = *(Item **)(puVar2[1] + uVar4 * 4);
      if (this_00 != (Item *)0x0) {
        iVar1 = Item::getType(this_00);
        if (iVar1 == 1) {
          return 1;
        }
        puVar2 = *(uint **)(this + 0x6c);
        uVar3 = *puVar2;
      }
    }
  }
  return 0;
}

```
## target disasm
```
  00184b60: push {r4,r5,r7,lr}
  00184b62: add r7,sp,#0x8
  00184b64: mov r4,r0
  00184b66: ldr r0,[r0,#0x68]
  00184b68: ldr r0,[r0,#0x4]
  00184b6a: cbz r0,0x00184b92
  00184b6c: ldr r1,[r4,#0x6c]
  00184b6e: cbz r1,0x00184b92
  00184b70: ldr r2,[r1,#0x0]
  00184b72: cbz r2,0x00184b92
  00184b74: movs r5,#0x0
  00184b76: b 0x00184b8e
  00184b78: ldr r0,[r1,#0x4]
  00184b7a: ldr.w r0,[r0,r5,lsl #0x2]
  00184b7e: cbz r0,0x00184b8c
  00184b80: blx 0x000718fc
  00184b84: cmp r0,#0x1
  00184b86: beq 0x00184b96
  00184b88: ldr r1,[r4,#0x6c]
  00184b8a: ldr r2,[r1,#0x0]
  00184b8c: adds r5,#0x1
  00184b8e: cmp r5,r2
  00184b90: bcc 0x00184b78
  00184b92: movs r0,#0x0
  00184b94: pop {r4,r5,r7,pc}
  00184b96: movs r0,#0x1
  00184b98: pop {r4,r5,r7,pc}
```
