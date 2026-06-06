# Level::getLandmarks
elf 0xc40bc body 6
Sig: undefined __stdcall getLandmarks(void)

## decompile
```c

/* Level::getLandmarks() */

undefined4 Level::getLandmarks(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x100);
}

```
## target disasm
```
  000d40bc: ldr.w r0,[r0,#0x100]
  000d40c0: bx lr
```
