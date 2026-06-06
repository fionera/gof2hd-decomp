# Level::getNumDeliveredPassengers
elf 0xc6792 body 6
Sig: undefined __stdcall getNumDeliveredPassengers(void)

## decompile
```c

/* Level::getNumDeliveredPassengers() */

undefined4 Level::getNumDeliveredPassengers(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x294);
}

```
## target disasm
```
  000d6792: ldr.w r0,[r0,#0x294]
  000d6796: bx lr
```
