# Player::isAlwaysEnemy
elf 0xa2a2e body 6
Sig: undefined __stdcall isAlwaysEnemy(void)

## decompile
```c

/* Player::isAlwaysEnemy() */

undefined1 Player::isAlwaysEnemy(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xec);
}

```
## target disasm
```
  000b2a2e: ldrb.w r0,[r0,#0xec]
  000b2a32: bx lr
```
