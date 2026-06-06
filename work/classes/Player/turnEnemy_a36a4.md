# Player::turnEnemy
elf 0xa36a4 body 8
Sig: undefined __stdcall turnEnemy(void)

## decompile
```c

/* Player::turnEnemy() */

void Player::turnEnemy(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0xe0) = 1;
  return;
}

```
## target disasm
```
  000b36a4: movs r1,#0x1
  000b36a6: strb.w r1,[r0,#0xe0]
  000b36aa: bx lr
```
