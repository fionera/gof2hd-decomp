# Player::doesNeverAttack
elf 0xa29fe body 6
Sig: undefined __stdcall doesNeverAttack(void)

## decompile
```c

/* Player::doesNeverAttack() */

undefined1 Player::doesNeverAttack(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xee);
}

```
## target disasm
```
  000b29fe: ldrb.w r0,[r0,#0xee]
  000b2a02: bx lr
```
