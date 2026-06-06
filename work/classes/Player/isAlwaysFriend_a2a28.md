# Player::isAlwaysFriend
elf 0xa2a28 body 6
Sig: undefined __stdcall isAlwaysFriend(void)

## decompile
```c

/* Player::isAlwaysFriend() */

undefined1 Player::isAlwaysFriend(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xed);
}

```
## target disasm
```
  000b2a28: ldrb.w r0,[r0,#0xed]
  000b2a2c: bx lr
```
