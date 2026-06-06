# Ship::spaceAvailable
elf 0x174f96 body 20
Sig: undefined __thiscall spaceAvailable(Ship * this, int param_1)

## decompile
```c

/* Ship::spaceAvailable(int) */

bool __thiscall Ship::spaceAvailable(Ship *this,int param_1)

{
  return param_1 + *(int *)(this + 0x10) <= *(int *)(this + 0xc) + *(int *)(this + 0x28);
}

```
## target disasm
```
  00184f96: ldrd r2,r3,[r0,#0xc]
  00184f9a: add r1,r3
  00184f9c: ldr r0,[r0,#0x28]
  00184f9e: add r2,r0
  00184fa0: movs r0,#0x0
  00184fa2: cmp r1,r2
  00184fa4: it le
  00184fa6: mov.le r0,#0x1
  00184fa8: bx lr
```
