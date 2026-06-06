# Status::getShip
elf 0xa9658 body 6
Sig: undefined __stdcall getShip(void)

## decompile
```c

/* Status::getShip() */

undefined4 Status::getShip(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 400);
}

```
## target disasm
```
  000b9658: ldr.w r0,[r0,#0x190]
  000b965c: bx lr
```
