# Player::setBombForce
elf 0xa2e9a body 6
Sig: undefined __stdcall setBombForce(float param_1)

## decompile
```c

/* Player::setBombForce(float) */

void Player::setBombForce(float param_1)

{
  int in_r0;
  undefined4 in_r1;
  
  *(undefined4 *)(in_r0 + 0xd4) = in_r1;
  return;
}

```
## target disasm
```
  000b2e9a: str.w r1,[r0,#0xd4]
  000b2e9e: bx lr
```
