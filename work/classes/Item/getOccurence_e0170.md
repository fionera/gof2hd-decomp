# Item::getOccurence
elf 0xe0170 body 4
Sig: undefined __thiscall getOccurence(Item * this)

## decompile
```c

/* Item::getOccurence() */

undefined4 __thiscall Item::getOccurence(Item *this)

{
  return *(undefined4 *)(this + 0x1c);
}

```
## target disasm
```
  000f0170: ldr r0,[r0,#0x1c]
  000f0172: bx lr
```
