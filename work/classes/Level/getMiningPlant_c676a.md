# Level::getMiningPlant
elf 0xc676a body 22
Sig: undefined __thiscall getMiningPlant(Level * this)

## decompile
```c

/* Level::getMiningPlant() */

undefined4 __thiscall Level::getMiningPlant(Level *this)

{
  undefined4 uVar1;
  
  if (*(int *)(this + 0xac) < 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + *(int *)(this + 0xac) * 4);
  }
  return uVar1;
}

```
## target disasm
```
  000d676a: ldr.w r1,[r0,#0xac]
  000d676e: cmp r1,#0x0
  000d6770: iteee lt
  000d6772: mov.lt r0,#0x0
  000d6774: ldr.ge.w r0,[r0,#0xf8]
  000d6778: ldr.ge r0,[r0,#0x4]
  000d677a: ldr.ge.w r0,[r0,r1,lsl #0x2]
  000d677e: bx lr
```
