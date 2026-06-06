# Player::setArmorHP
elf 0xa2d1c body 18
Sig: undefined __stdcall setArmorHP(int param_1)

## decompile
```c

/* Player::setArmorHP(int) */

void Player::setArmorHP(int param_1)

{
  int in_r1;
  
  if (*(int *)(param_1 + 0x90) < in_r1) {
    in_r1 = *(int *)(param_1 + 0x90);
  }
  *(int *)(param_1 + 0x8c) = in_r1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d1c: ldr.w r2,[r0,#0x90]
  000b2d20: cmp r2,r1
  000b2d22: it lt
  000b2d24: mov.lt r1,r2
  000b2d26: str.w r1,[r0,#0x8c]
  000b2d2a: b.w 0x001abcd8
```
