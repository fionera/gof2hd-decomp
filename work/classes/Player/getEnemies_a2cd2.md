# Player::getEnemies
elf 0xa2cd2 body 4
Sig: undefined __stdcall getEnemies(void)

## decompile
```c

/* Player::getEnemies() */

undefined4 Player::getEnemies(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x74);
}

```
## target disasm
```
  000b2cd2: ldr r0,[r0,#0x74]
  000b2cd4: bx lr
```
