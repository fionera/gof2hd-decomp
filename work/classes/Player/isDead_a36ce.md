# Player::isDead
elf 0xa36ce body 12
Sig: undefined __stdcall isDead(void)

## decompile
```c

/* Player::isDead() */

bool Player::isDead(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x78) < 1;
}

```
## target disasm
```
  000b36ce: ldr r1,[r0,#0x78]
  000b36d0: movs r0,#0x0
  000b36d2: cmp r1,#0x1
  000b36d4: it lt
  000b36d6: mov.lt r0,#0x1
  000b36d8: bx lr
```
