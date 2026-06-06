# Player::setActive
elf 0xa36e0 body 6
Sig: undefined __stdcall setActive(bool param_1)

## decompile
```c

/* Player::setActive(bool) */

void Player::setActive(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0xc0) = in_r1;
  return;
}

```
## target disasm
```
  000b36e0: strb.w r1,[r0,#0xc0]
  000b36e4: bx lr
```
