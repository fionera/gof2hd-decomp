# Player::isActive
elf 0xa36e6 body 6
Sig: undefined __stdcall isActive(void)

## decompile
```c

/* Player::isActive() */

undefined1 Player::isActive(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xc0);
}

```
## target disasm
```
  000b36e6: ldrb.w r0,[r0,#0xc0]
  000b36ea: bx lr
```
