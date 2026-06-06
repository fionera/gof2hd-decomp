# Player::setAlwaysEnemy
elf 0xa2a04 body 16
Sig: undefined __stdcall setAlwaysEnemy(bool param_1)

## decompile
```c

/* Player::setAlwaysEnemy(bool) */

void Player::setAlwaysEnemy(bool param_1)

{
  uint uVar1;
  undefined1 in_r1;
  
  uVar1 = (uint)param_1;
  *(undefined1 *)(uVar1 + 0xec) = in_r1;
  *(undefined2 *)(uVar1 + 0x5c) = 1;
  *(undefined1 *)(uVar1 + 0xe0) = 1;
  return;
}

```
## target disasm
```
  000b2a04: strb.w r1,[r0,#0xec]
  000b2a08: movs r1,#0x1
  000b2a0a: strh.w r1,[r0,#0x5c]
  000b2a0e: strb.w r1,[r0,#0xe0]
  000b2a12: bx lr
```
