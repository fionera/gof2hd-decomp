# Ship::getUnmoddedHandling
elf 0x175698 body 4
Sig: undefined __thiscall getUnmoddedHandling(Ship * this)

## decompile
```c

/* Ship::getUnmoddedHandling() */

undefined4 __thiscall Ship::getUnmoddedHandling(Ship *this)

{
  return *(undefined4 *)(this + 0x18);
}

```
## target disasm
```
  00185698: ldr r0,[r0,#0x18]
  0018569a: bx lr
```
