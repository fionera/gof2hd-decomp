# Player::getHitVector
elf 0xa2eb2 body 16
Sig: undefined __stdcall getHitVector(void)

## decompile
```c

/* Player::getHitVector() */

void Player::getHitVector(void)

{
  undefined8 *in_r0;
  int in_r1;
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(in_r1 + 0xc4);
  *(undefined4 *)(in_r0 + 1) = *(undefined4 *)(in_r1 + 0xcc);
  *in_r0 = uVar1;
  return;
}

```
## target disasm
```
  000b2eb2: vldr.64 d16,[r1,#0xc4]
  000b2eb6: ldr.w r1,[r1,#0xcc]
  000b2eba: str r1,[r0,#0x8]
  000b2ebc: vstr.64 d16,[r0]
  000b2ec0: bx lr
```
