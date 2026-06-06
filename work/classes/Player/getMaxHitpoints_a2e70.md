# Player::getMaxHitpoints
elf 0xa2e70 body 6
Sig: undefined __stdcall getMaxHitpoints(void)

## decompile
```c

/* Player::getMaxHitpoints() */

undefined4 Player::getMaxHitpoints(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x84);
}

```
## target disasm
```
  000b2e70: ldr.w r0,[r0,#0x84]
  000b2e74: bx lr
```
