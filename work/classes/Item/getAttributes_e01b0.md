# Item::getAttributes
elf 0xe01b0 body 4
Sig: undefined __thiscall getAttributes(Item * this)

## decompile
```c

/* Item::getAttributes() */

undefined4 __thiscall Item::getAttributes(Item *this)

{
  return *(undefined4 *)(this + 0x30);
}

```
## target disasm
```
  000f01b0: ldr r0,[r0,#0x30]
  000f01b2: bx lr
```
