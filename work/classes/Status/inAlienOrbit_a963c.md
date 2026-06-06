# Status::inAlienOrbit
elf 0xa963c body 12
Sig: undefined __stdcall inAlienOrbit(void)

## decompile
```c

/* Status::inAlienOrbit() */

void Status::inAlienOrbit(void)

{
  int in_r0;
  
  (*(code *)(DAT_001abf24 + 0x1abf28))(*(undefined4 *)(in_r0 + 0x19c),*(undefined4 *)(in_r0 + 0x78))
  ;
  return;
}

```
## target disasm
```
  000b963c: ldr r1,[r0,#0x78]
  000b963e: ldr.w r0,[r0,#0x19c]
  000b9642: b.w 0x001abf18
  001abf18: bx pc
```
