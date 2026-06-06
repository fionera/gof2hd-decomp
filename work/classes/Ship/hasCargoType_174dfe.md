# Ship::hasCargoType
elf 0x174dfe body 58
Sig: undefined __thiscall hasCargoType(Ship * this, int param_1)

## decompile
```c

/* Ship::hasCargoType(int) */

undefined4 __thiscall Ship::hasCargoType(Ship *this,int param_1)

{
  Item *this_00;
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(this + 0x70);
  if (puVar2 != (uint *)0x0) {
    for (uVar3 = 0; uVar3 < *puVar2; uVar3 = uVar3 + 1) {
      this_00 = *(Item **)(puVar2[1] + uVar3 * 4);
      if (this_00 != (Item *)0x0) {
        iVar1 = Item::getType(this_00);
        if (iVar1 == param_1) {
          return 1;
        }
        puVar2 = *(uint **)(this + 0x70);
      }
    }
  }
  return 0;
}

```
## target disasm
```
  00184dfe: push {r4,r5,r6,r7,lr}
  00184e00: add r7,sp,#0xc
  00184e02: push.w r11
  00184e06: mov r4,r1
  00184e08: ldr r1,[r0,#0x70]
  00184e0a: mov r5,r0
  00184e0c: cbz r1,0x00184e2c
  00184e0e: movs r6,#0x0
  00184e10: b 0x00184e26
  00184e12: ldr r0,[r1,#0x4]
  00184e14: ldr.w r0,[r0,r6,lsl #0x2]
  00184e18: cbz r0,0x00184e24
  00184e1a: blx 0x000718fc
  00184e1e: cmp r0,r4
  00184e20: beq 0x00184e30
  00184e22: ldr r1,[r5,#0x70]
  00184e24: adds r6,#0x1
  00184e26: ldr r0,[r1,#0x0]
  00184e28: cmp r6,r0
  00184e2a: bcc 0x00184e12
  00184e2c: movs r0,#0x0
  00184e2e: b 0x00184e32
  00184e30: movs r0,#0x1
  00184e32: pop.w r11
  00184e36: pop {r4,r5,r6,r7,pc}
```
