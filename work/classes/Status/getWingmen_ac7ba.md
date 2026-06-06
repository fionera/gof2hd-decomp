# Status::getWingmen
elf 0xac7ba body 4
Sig: undefined __stdcall getWingmen(void)

## decompile
```c

/* Status::getWingmen() */

undefined4 Status::getWingmen(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x24);
}

```
## target disasm
```
  000bc7ba: ldr r0,[r0,#0x24]
  000bc7bc: bx lr
```
