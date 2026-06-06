# Player::getHitpoints
elf 0xa2e6c body 4
Sig: undefined __stdcall getHitpoints(void)

## decompile
```c

/* Player::getHitpoints() */

undefined4 Player::getHitpoints(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x78);
}

```
## target disasm
```
  000b2e6c: ldr r0,[r0,#0x78]
  000b2e6e: bx lr
```
