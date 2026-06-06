# Ship::getSlots
elf 0x175388 body 8
Sig: undefined __thiscall getSlots(Ship * this, int param_1)

## decompile
```c

/* Ship::getSlots(int) */

undefined4 __thiscall Ship::getSlots(Ship *this,int param_1)

{
  return *(undefined4 *)(*(int *)(this + 0x68) + param_1 * 4);
}

```
## target disasm
```
  00185388: ldr r0,[r0,#0x68]
  0018538a: ldr.w r0,[r0,r1,lsl #0x2]
  0018538e: bx lr
```
