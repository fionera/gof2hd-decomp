# Status::incEquipmentBought
elf 0xac500 body 12
Sig: undefined __thiscall incEquipmentBought(Status * this)

## decompile
```c

/* Status::incEquipmentBought() */

void __thiscall Status::incEquipmentBought(Status *this)

{
  *(int *)(this + 0x1e4) = *(int *)(this + 0x1e4) + 1;
  return;
}

```
## target disasm
```
  000bc500: ldr.w r1,[r0,#0x1e4]
  000bc504: adds r1,#0x1
  000bc506: str.w r1,[r0,#0x1e4]
  000bc50a: bx lr
```
