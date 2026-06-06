# Level::getMessages
elf 0xc454e body 6
Sig: undefined __stdcall getMessages(void)

## decompile
```c

/* Level::getMessages() */

undefined4 Level::getMessages(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x114);
}

```
## target disasm
```
  000d454e: ldr.w r0,[r0,#0x114]
  000d4552: bx lr
```
