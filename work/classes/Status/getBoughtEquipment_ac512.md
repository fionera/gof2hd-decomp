# Status::getBoughtEquipment
elf 0xac512 body 6
Sig: undefined __thiscall getBoughtEquipment(Status * this)

## decompile
```c

/* Status::getBoughtEquipment() */

undefined4 __thiscall Status::getBoughtEquipment(Status *this)

{
  return *(undefined4 *)(this + 0x1e4);
}

```
## target disasm
```
  000bc512: ldr.w r0,[r0,#0x1e4]
  000bc516: bx lr
```
