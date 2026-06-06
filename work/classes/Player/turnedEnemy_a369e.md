# Player::turnedEnemy
elf 0xa369e body 6
Sig: undefined __stdcall turnedEnemy(void)

## decompile
```c

/* Player::turnedEnemy() */

undefined1 Player::turnedEnemy(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xe0);
}

```
## target disasm
```
  000b369e: ldrb.w r0,[r0,#0xe0]
  000b36a2: bx lr
```
