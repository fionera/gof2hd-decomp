# Player::setVulnerable
elf 0xa2e8e body 6
Sig: undefined __stdcall setVulnerable(bool param_1)

## decompile
```c

/* Player::setVulnerable(bool) */

void Player::setVulnerable(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0xc2) = in_r1;
  return;
}

```
## target disasm
```
  000b2e8e: strb.w r1,[r0,#0xc2]
  000b2e92: bx lr
```
