# Ship::freeAllSlots
elf 0x1752d8 body 34
Sig: undefined __thiscall freeAllSlots(Ship * this)

## decompile
```c

/* Ship::freeAllSlots() */

void __thiscall Ship::freeAllSlots(Ship *this)

{
  uint uVar1;
  uint uVar2;
  
  for (uVar1 = 0; uVar1 < **(uint **)(this + 0x6c); uVar1 = uVar1 + 1) {
    uVar2 = (*(uint **)(this + 0x6c))[1];
    if (*(int *)(uVar2 + uVar1 * 4) != 0) {
      *(undefined4 *)(uVar2 + uVar1 * 4) = 0;
    }
  }
  (*(code *)(DAT_001abf14 + 0x1abf18))();
  return;
}

```
## target disasm
```
  001852d8: mov.w r12,#0x0
  001852dc: movs r2,#0x0
  001852de: ldr r3,[r0,#0x6c]
  001852e0: ldr r1,[r3,#0x0]
  001852e2: cmp r2,r1
  001852e4: it cs
  001852e6: b.cs.w 0x001abf08
  001852ea: ldr r3,[r3,#0x4]
  001852ec: ldr.w r1,[r3,r2,lsl #0x2]
  001852f0: cbz r1,0x001852f6
  001852f2: str.w r12,[r3,r2,lsl #0x2]
  001852f6: adds r2,#0x1
  001852f8: b 0x001852de
```
