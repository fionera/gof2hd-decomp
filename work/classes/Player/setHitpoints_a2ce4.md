# Player::setHitpoints
elf 0xa2ce4 body 18
Sig: undefined __stdcall setHitpoints(int param_1)

## decompile
```c

/* Player::setHitpoints(int) */

void Player::setHitpoints(int param_1)

{
  int in_r1;
  
  *(int *)(param_1 + 0x78) = in_r1;
  if (*(int *)(param_1 + 0x84) < in_r1) {
    *(int *)(param_1 + 0x84) = in_r1;
  }
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2ce4: ldr.w r2,[r0,#0x84]
  000b2ce8: str r1,[r0,#0x78]
  000b2cea: cmp r2,r1
  000b2cec: it lt
  000b2cee: str.w.lt r1,[r0,#0x84]
  000b2cf2: b.w 0x001abcd8
```
