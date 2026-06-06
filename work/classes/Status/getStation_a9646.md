# Status::getStation
elf 0xa9646 body 6
Sig: undefined __stdcall getStation(void)

## decompile
```c

/* Status::getStation() */

undefined4 Status::getStation(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x19c);
}

```
## target disasm
```
  000b9646: ldr.w r0,[r0,#0x19c]
  000b964a: bx lr
```
